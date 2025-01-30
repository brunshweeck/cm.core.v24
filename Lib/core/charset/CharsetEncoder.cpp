//
// Created by brunshweeck on 2 sept. 2024.
//

#include "CharsetEncoder.h"

#include <core/io/ByteBuffer.h>
#include <core/util/Arrays.h>

#include "CharsetDecoder.h"
#include "CharacterCodingException.h"
#include "CoderMalfunctionError.h"

namespace core {
    namespace charset {
        using namespace io;
        using namespace util;
        using namespace misc;

        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        CharsetEncoder::CharsetEncoder(Charset const &cs,
                                       gfloat averageBytesPerChar,
                                       gfloat maxBytesPerChar,
                                       ByteArray replacement): cs(cs) {
            // Use !(a > 0.0f) rather than (a <= 0.0f) to exclude NaN values
            if (!(averageBytesPerChar > 0.0f))
                IllegalArgumentException("Non-positive averageBytesPerChar"_S)
                        .throws($ftrace());

            // Use !(a > 0.0f) rather than (a <= 0.0f) to exclude NaN values
            if (!(maxBytesPerChar > 0.0f))
                IllegalArgumentException("Non-positive maxBytesPerChar"_S)
                        .throws($ftrace());

            if (averageBytesPerChar > maxBytesPerChar)
                IllegalArgumentException("averageBytesPerChar exceeds maxBytesPerChar"_S)
                        .throws($ftrace());

            repl = UNSAFE::moveInstance(replacement);
            averageBpC = averageBytesPerChar;
            maxBpC = maxBytesPerChar;
            replaceWith(repl);
        }

        CharsetEncoder::CharsetEncoder(Charset const &cs, gfloat averageBytesPerChar, gfloat maxBytesPerChar)
            : CharsetEncoder(cs, averageBytesPerChar, maxBytesPerChar, ByteArray::of((gbyte)'?')) {
        }

        Charset const &CharsetEncoder::charset() const { return cs; }

        ByteArray CharsetEncoder::replacement() const { return repl; }

        CharsetEncoder &CharsetEncoder::replaceWith(ByteArray const &newReplacement) {
            gint len = newReplacement.length();
            if (len == 0)
                IllegalArgumentException("Empty replacement"_S).throws($ftrace());
            if (len > maxBpC)
                IllegalArgumentException("Replacement too long"_S).throws($ftrace());

            if (!isLegalReplacement(newReplacement))
                IllegalArgumentException("Illegal replacement"_S).throws($ftrace());
            repl = newReplacement;

            replaceWith0(repl);
            return *this;
        }

        gbool CharsetEncoder::isLegalReplacement(ByteArray const &repl) const {
            Decoder dec = cachedDecoder;
            if (dec == null) {
                CharsetDecoder &cd = cs.newDecoder();
                cd.onMalformedInput(Charset::CodingErrorAction::REPORT);
                cd.onUnmappableCharacter(Charset::CodingErrorAction::REPORT);
                cachedDecoder = dec = &cd;
            } else {
                dec->reset();
            }
            ByteBuffer &bb = ByteBuffer::wrap((ByteArray&)repl);
            CharBuffer &cb = CharBuffer::allocate((gint)(bb.remaining() * dec->maxCharsPerByte()));

            CoderResult cr = dec->decode(bb, cb, true);
            return !cr.isError();
        }

        Charset::CodingErrorAction CharsetEncoder::malformedInputAction() const { return mia; }

        CharsetEncoder &CharsetEncoder::onMalformedInput(Charset::CodingErrorAction newAction) {
            mia = newAction;
            onMalformedInput0(newAction);
            return *this;
        }

        Charset::CodingErrorAction CharsetEncoder::unmappableCharacterAction() const { return uca; }

        CharsetEncoder &CharsetEncoder::onUnmappableCharacter(Charset::CodingErrorAction newAction) {
            uca = newAction;
            onUnmappableCharacter0(newAction);
            return *this;
        }

        gfloat CharsetEncoder::averageBytesPerChar() const { return averageBpC; }

        gfloat CharsetEncoder::maxBytesPerChar() const { return maxBpC; }

        static char const *stateNames[] = {"RESET", "CODING", "CODING_END", "FLUSHED"};

        CoderResult CharsetEncoder::encode(CharBuffer &in, ByteBuffer &out, gbool endOfInput) {
            State newState = endOfInput ? State::ST_END : State::ST_CODING;
            if (state != State::ST_RESET && state != State::ST_CODING
                && !(endOfInput && state == State::ST_END))
                IllegalStateException("Current state = "_S + stateNames[((gint)state)]
                                      + ", new state = " + stateNames[((gint)newState)]).throws($ftrace());
            state = newState;

            for (;;) {
                CoderResult cr = CoderResult::UNDERFLOW;
                try {
                    cr = encode0(in, out);
                } catch (RuntimeException const &x) {
                    CoderMalfunctionError(x).throws($ftrace());
                }

                if (cr.isOverflow())
                    return cr;

                if (cr.isUnderflow()) {
                    if (endOfInput && in.hasRemaining()) {
                        cr = CoderResult::malformedForLength(in.remaining());
                        // Fall through to malformed-input case
                    } else {
                        return cr;
                    }
                }

                Charset::CodingErrorAction action;
                if (cr.isMalformed())
                    action = mia;
                else if (cr.isUnmappable())
                    action = uca;
                else
                    AssertionError(cr.toString()).throws($ftrace());

                if (action == Charset::CodingErrorAction::REPORT)
                    return cr;

                if (action == Charset::CodingErrorAction::REPLACE) {
                    if (out.remaining() < repl.length())
                        return CoderResult::OVERFLOW;
                    out.put(repl);
                }

                if (action == Charset::CodingErrorAction::IGNORE || action == Charset::CodingErrorAction::REPLACE) {
                    // Skip erroneous input either way
                    in.position(in.position() + cr.length());
                    continue;
                }

                CORE_ASSERT(false);
            }
        }

        CoderResult CharsetEncoder::flush(ByteBuffer &out) {
            if (state == State::ST_END) {
                CoderResult cr = flush0(out);
                if (cr.isUnderflow())
                    state = State::ST_FLUSHED;
                return cr;
            }

            if (state != State::ST_FLUSHED) {
                IllegalStateException("Current state = "_S + stateNames[((gint)state)]
                                      + ", new state = " + stateNames[((gint)State::ST_FLUSHED)]).throws($ftrace());
            }

            return CoderResult::UNDERFLOW; // Already flushed
        }

        CharsetEncoder &CharsetEncoder::reset() {
            reset0();
            state = State::ST_RESET;
            return *this;
        }

        ByteBuffer &CharsetEncoder::encode(CharBuffer &in) {
            int n = Math::min((gint)(in.remaining() * averageBytesPerChar()), Arrays::SOFT_MAX_ARRAY_LENGTH);
            ByteBuffer &out = ByteBuffer::allocate(n);

            if (n == 0 && in.remaining() == 0)
                return out;
            reset();
            for (;;) {
                CoderResult cr = in.hasRemaining() ? encode(in, out, true) : CoderResult::UNDERFLOW;
                if (cr.isUnderflow())
                    cr = flush(out);

                if (cr.isUnderflow())
                    break;
                if (cr.isOverflow()) {
                    // Ensure progress; n might be 0!
                    n = Arrays::newLength(n, Math::min(n + 1, 1024), n + 1);
                    ByteBuffer &o = ByteBuffer::allocate(n);
                    out.flip();
                    o.put(out);

                    // out = o;
                    CORE_XCAST(ByteArray, out.base()) = UNSAFE::moveInstance(CORE_XCAST(ByteArray, o.base()));
                    UNSAFE::swapValues(out.capacity_, o.capacity_);
                    UNSAFE::swapValues(out.limit_, o.limit_);
                    UNSAFE::swapValues(out.position_, o.position_);
                    UNSAFE::swapValues(out.mark_, o.mark_);

                    UNSAFE::deleteRegInstance(o.base());
                    UNSAFE::deleteRegInstance(o);

                    continue;
                }
                cr.throwException();
            }
            out.flip();
            return out;
        }

        gbool CharsetEncoder::canEncode(gchar c) {
            CharBuffer &cb = CharBuffer::allocate(1);
            cb.put(c);
            cb.flip();
            return canEncode(cb);
        }

        gbool CharsetEncoder::canEncode(CharSequence const &cs) {
            gbool r;
            if (Class<CharBuffer>::hasInstance(cs)) {
                CharBuffer &cb = CORE_XCAST(CharBuffer const, cs).duplicate();
                r = canEncode(cb);

                UNSAFE::deleteInstance(cb);
            } else {
                CharBuffer &cb = CharBuffer::wrap(cs.toString());
                r = canEncode(cb);

                UNSAFE::deleteInstance(cb);
            }
            return r;
        }

        void CharsetEncoder::replaceWith0(ByteArray const &newReplacement) {
        }

        void CharsetEncoder::onMalformedInput0(Charset::CodingErrorAction newAction) {
        }

        void CharsetEncoder::onUnmappableCharacter0(Charset::CodingErrorAction newAction) {
        }

        CoderResult CharsetEncoder::flush0(ByteBuffer &out) { return CoderResult::UNDERFLOW; }

        void CharsetEncoder::reset0() {
        }

        gbool CharsetEncoder::canEncode(CharBuffer &cb) {
            if (state == State::ST_FLUSHED)
                reset();
            else if (state != State::ST_RESET)
                IllegalStateException("Current state = "_S + stateNames[((gint)state)]
                                      + ", new state = " + stateNames[((gint)State::ST_CODING)]).throws($ftrace());
            Charset::CodingErrorAction ma = malformedInputAction();
            Charset::CodingErrorAction ua = unmappableCharacterAction();

            gbool success = true;
            try {
                onMalformedInput(Charset::CodingErrorAction::REPORT);
                onUnmappableCharacter(Charset::CodingErrorAction::REPORT);
                encode(cb);
            } catch (CharacterCodingException const &) { success = false; }

            // finally
            onMalformedInput(ma);
            onUnmappableCharacter(ua);
            reset();

            return success;
        }

        CORE_WARNING_POP
    } // charset
} // core
