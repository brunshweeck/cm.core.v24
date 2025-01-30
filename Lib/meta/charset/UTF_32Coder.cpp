//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/Throwable.h>
#include <core/io/ByteBuffer.h>
#include <meta/charset/UTF_32Coder.h>

namespace core {
    namespace charset {
        UTF_32Coder::Decoder::Decoder(Charset const& cs, gint bo)
            : CharsetDecoder(cs, 0.25F, 1.0F) {
            expectedBO = bo;
            currentBO = NONE;
        }

        CoderResult UTF_32Coder::Decoder::decode0(io::ByteBuffer& src, io::CharBuffer& dst) {
            if (src.remaining() < 4)
                return CoderResult::UNDERFLOW;
            gint mark = src.position();
            gint cp;

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                if (currentBO == NONE) {
                    cp = ((src.get() & 0xff) << 24) |
                         ((src.get() & 0xff) << 16) |
                         ((src.get() & 0xff) <<  8) |
                         (src.get() & 0xff);
                    if (cp == BOM_BIG && expectedBO != LITTLE) {
                        currentBO = BIG;
                        mark += 4;
                    } else if (cp == BOM_LITTLE && expectedBO != BIG) {
                        currentBO = LITTLE;
                        mark += 4;
                    } else {
                        if (expectedBO == NONE)
                            currentBO = BIG;
                        else
                            currentBO = expectedBO;
                        src.position(mark);
                    }
                }
                while (src.remaining() >= 4) {
                    cp = getCP(src);
                    if (Character::isBmpCodePoint(cp)) {
                        if (!dst.hasRemaining()) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        mark += 4;
                        dst.put((gchar) cp);
                    } else if (Character::isValidCodePoint(cp)) {
                        if (dst.remaining() < 2) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        mark += 4;
                        dst.put(Character::highSurrogate(cp));
                        dst.put(Character::lowSurrogate(cp));
                    } else {
                        cr = CoderResult::malformedForLength(4);
                        goto FINALLY;
                    }
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }
            FINALLY: {
                src.position(mark);

                if (catchEx)
                    catchEx->throws($ftrace());
                return cr;
            }
        }

        void UTF_32Coder::Decoder::reset0() {
            currentBO = NONE;
        }

        gint UTF_32Coder::Decoder::getCP(io::ByteBuffer& src) const {
            return (currentBO == BIG)
                       ? (((src.get() & 0xff) << 24) |
                           ((src.get() & 0xff) << 16) |
                           ((src.get() & 0xff) << 8) |
                           (src.get() & 0xff))
                       : ((src.get() & 0xff) |
                           ((src.get() & 0xff) << 8) |
                           ((src.get() & 0xff) << 16) |
                           ((src.get() & 0xff) << 24));
        }

        UTF_32Coder::Encoder::Encoder(Charset const& cs, gint byteOrder, gbool doBOM)
            : CharsetEncoder(cs,
                4.0F,
                doBOM? 8.0F : 4.0F,
                (byteOrder == BIG)
                ? ByteArray::of((gbyte)0, (gbyte)0, (gbyte)0xff, (gbyte)0xfd)
                : ByteArray::of((gbyte)0xfd, (gbyte)0xff, (gbyte)0, (gbyte)0)),
        byteOrder(byteOrder), doBOM(doBOM), doneBOM(!doBOM){
        }

        CoderResult UTF_32Coder::Encoder::encode0(io::CharBuffer& src, io::ByteBuffer& dst) {
            gint mark = src.position();
            if (!doneBOM && src.hasRemaining()) {
                if (dst.remaining() < 4)
                    return CoderResult::OVERFLOW;
                put(BOM_BIG, dst);
                doneBOM = true;
            }

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;
            
            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    if (!Character::isSurrogate(c)) {
                        if (dst.remaining() < 4) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        mark++;
                        put(c, dst);
                    } else if (Character::isHighSurrogate(c)) {
                        if (!src.hasRemaining()) {
                            cr = CoderResult::UNDERFLOW;
                            goto FINALLY;
                        }
                        gchar low = src.get();
                        if (Character::isLowSurrogate(low)) {
                            if (dst.remaining() < 4) {
                                cr = CoderResult::OVERFLOW;
                                goto FINALLY;
                            }
                            mark += 2;
                            put(Character::toCodePoint(c, low), dst);
                        } else {
                            cr = CoderResult::malformedForLength(1);
                            goto FINALLY;
                        }
                    } else {
                        // assert Character::isLowSurrogate(c);
                        cr = CoderResult::malformedForLength(1);
                        goto FINALLY;
                    }
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }
            FINALLY: {
                src.position(mark);

                if (catchEx)
                    catchEx->throws($ftrace());
                return cr;
            }
        }

        void UTF_32Coder::Encoder::reset0() { doneBOM = !doBOM; }

        void UTF_32Coder::Encoder::put(gint cp, io::ByteBuffer& dst) const {
            if (byteOrder== BIG) {
                dst.put((gbyte)(cp >> 24));
                dst.put((gbyte)(cp >> 16));
                dst.put((gbyte)(cp >> 8));
                dst.put((gbyte)cp);
            } else {
                dst.put((gbyte)cp);
                dst.put((gbyte)(cp >>  8));
                dst.put((gbyte)(cp >> 16));
                dst.put((gbyte)(cp >> 24));
            }
        }
    } // charset
    } // core
