//
// Created by brunshweeck on 2 sept. 2024.
//

#include "CharsetDecoder.h"
#include "CoderMalfunctionError.h"

#include <core/UnsupportedOperationException.h>
#include <core/io/ByteBuffer.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace io;
    using namespace util;
    using namespace misc;

    namespace charset {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        CharsetDecoder::CharsetDecoder(Charset const& cs,
                                       gfloat averageCharsPerByte,
                                       gfloat maxCharsPerByte,
                                       String replacement): cs(cs) {
            // Use !(a > 0.0f) rather than (a <= 0.0f) to exclude NaN values
            if (!(averageCharsPerByte > 0.0f))
                IllegalArgumentException("Non-positive averageCharsPerByte"_S)
                        .throws($ftrace());

            // Use !(a > 0.0f) rather than (a <= 0.0f) to exclude NaN values
            if (!(maxCharsPerByte > 0.0f))
                IllegalArgumentException("Non-positive maxCharsPerByte"_S)
                        .throws($ftrace());

            if (averageCharsPerByte > maxCharsPerByte)
                IllegalArgumentException("averageCharsPerByte exceeds maxCharsPerByte"_S)
                        .throws($ftrace());

            repl = UNSAFE::moveInstance(replacement);
            averageCpB = averageCharsPerByte;
            maxCpB = maxCharsPerByte;
            replaceWith(repl);
        }

        CharsetDecoder::CharsetDecoder(Charset const& cs, gfloat averageCharsPerByte, gfloat maxCharsPerByte)
            : CharsetDecoder(cs, averageCharsPerByte, maxCharsPerByte, u"\uFFFD"_S) {}

        Charset const& CharsetDecoder::charset() const { return cs; }

        String CharsetDecoder::replacement() const { return repl; }

        CharsetDecoder& CharsetDecoder::replaceWith(String const& newReplacement) {
            gint len = newReplacement.length();
            if (len == 0)
                IllegalArgumentException("Empty replacement"_S).throws($ftrace());

            if (len > maxCpB)
                IllegalArgumentException("Replacement too long"_S).throws($ftrace());

            repl = newReplacement;

            replaceWith0(repl);
            return *this;
        }

        Charset::CodingErrorAction CharsetDecoder::malformedInputAction() const { return mia; }

        CharsetDecoder& CharsetDecoder::onMalformedInput(Charset::CodingErrorAction newAction) {
            mia = newAction;
            onMalformedInput0(newAction);
            return *this;
        }

        Charset::CodingErrorAction CharsetDecoder::unmappableCharacterAction() const { return uca; }

        CharsetDecoder& CharsetDecoder::onUnmappableCharacter(Charset::CodingErrorAction newAction) {
            uca = newAction;
            onUnmappableCharacter0(newAction);
            return *this;
        }

        gfloat CharsetDecoder::averageCharsPerByte() const { return averageCpB; }

        gfloat CharsetDecoder::maxCharsPerByte() const { return maxCpB; }

        static char const* stateNames[] = {"RESET", "CODING", "CODING_END", "FLUSHED"};

        CoderResult CharsetDecoder::decode(ByteBuffer& in, CharBuffer& out, gbool endOfInput) {
            State newState = endOfInput ? State::ST_END : State::ST_CODING;
            if (state != State::ST_RESET && state != State::ST_CODING && !(endOfInput && state == State::ST_END))
                IllegalStateException("Current state = "_S + stateNames[((gint) state)]
                    + ", new state = " + stateNames[((gint) newState)]).throws($ftrace());

            state = newState;

            for (;;) {
                CoderResult cr = CoderResult::UNDERFLOW;
                try {
                    cr = decode0(in, out);
                } catch (RuntimeException const& cse) {
                    CoderMalfunctionError(cse).throws($ftrace());
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

        CoderResult CharsetDecoder::flush(CharBuffer& out) {
            if (state == State::ST_END) {
                CoderResult cr = flush0(out);
                if (cr.isUnderflow())
                    state = State::ST_FLUSHED;
                return cr;
            }

            if (state != State::ST_FLUSHED) {
                IllegalStateException("Current state = "_S + stateNames[((gint) state)]
                    + ", new state = " + stateNames[((gint) State::ST_FLUSHED)]).throws($ftrace());
            }

            return CoderResult::UNDERFLOW; // Already flushed
        }

        CharsetDecoder& CharsetDecoder::reset() {
            reset0();
            state = State::ST_RESET;
            return *this;
        }

        CharBuffer& CharsetDecoder::decode(ByteBuffer& in) {
            gint n = Math::min((gint) (in.remaining() * averageCharsPerByte()), Arrays::SOFT_MAX_ARRAY_LENGTH);
            CharBuffer& out = CharBuffer::allocate(n);

            if (n == 0 && in.remaining() == 0)
                return out;
            reset();
            for (;;) {
                CoderResult cr = in.hasRemaining() ? decode(in, out, true) : CoderResult::UNDERFLOW;
                if (cr.isUnderflow())
                    cr = flush(out);

                if (cr.isUnderflow())
                    break;
                if (cr.isOverflow()) {
                    // Ensure progress; n might be 0!
                    n = Arrays::newLength(n, Math::min(n + 1, 1024), n + 1);
                    CharBuffer& o = CharBuffer::allocate(n);
                    out.flip();
                    o.put(out);

                    // out = o;
                    CORE_XCAST(CharArray, out.base()) = UNSAFE::moveInstance(CORE_XCAST(CharArray, o.base()));
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

        gbool CharsetDecoder::isAutoDetecting() { return false; }

        gbool CharsetDecoder::isCharsetDetected() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object const& CharsetDecoder::detectCharset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        void CharsetDecoder::replaceWith0(String const& newReplacement) {}

        void CharsetDecoder::onMalformedInput0(Charset::CodingErrorAction newAction) {}

        void CharsetDecoder::onUnmappableCharacter0(Charset::CodingErrorAction newAction) {}

        CoderResult CharsetDecoder::flush0(CharBuffer& out) { return CoderResult::UNDERFLOW; }

        void CharsetDecoder::reset0() {}

        CORE_WARNING_POP
    } // charset
} // core
