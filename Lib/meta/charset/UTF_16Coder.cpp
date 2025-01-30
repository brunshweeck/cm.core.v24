//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <meta/charset/UTF_16Coder.h>

namespace core {
    namespace charset {
        UTF_16Coder::Decoder::Decoder(Charset const& cs, gint bo)
            : CharsetDecoder(cs, 0.5F, 1.0F),
              exceptedByteOrder(bo), currentByteOrder(bo) {
        }

        UTF_16Coder::Decoder::Decoder(Charset const& cs, gint bo, gint defaultBO)
            : Decoder(cs, bo) {
            defaultByteOrder = defaultBO;
        }

        CoderResult UTF_16Coder::Decoder::decode0(io::ByteBuffer& src, io::CharBuffer& dst) {
            int mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (src.remaining() > 1) {
                    int b1 = src.get() & 0xff;
                    int b2 = src.get() & 0xff;

                    // Byte Order Mark interpretation
                    if (currentByteOrder == NONE) {
                        gchar c = (gchar)((b1 << 8) | b2);
                        if (c == BYTE_ORDER_MARK) {
                            currentByteOrder = BIG;
                            mark += 2;
                            continue;
                        } else if (c == REVERSED_MARK) {
                            currentByteOrder = LITTLE;
                            mark += 2;
                            continue;
                        } else {
                            currentByteOrder = defaultByteOrder;
                            // FALL THROUGH to process b1, b2 normally
                        }
                    }

                    gchar c = decode(b1, b2);

                    // Surrogates
                    if (Character::isSurrogate(c)) {
                        if (Character::isHighSurrogate(c)) {
                            if (src.remaining() < 2) {
                                cr = CoderResult::UNDERFLOW;
                                goto FINALLY;
                            }
                            gchar c2 = decode(src.get() & 0xff, src.get() & 0xff);
                            if (!Character::isLowSurrogate(c2)) {
                                cr = CoderResult::malformedForLength(4);
                                goto FINALLY;
                            }
                            if (dst.remaining() < 2) {
                                cr = CoderResult::OVERFLOW;
                                goto FINALLY;
                            }
                            mark += 4;
                            dst.put(c);
                            dst.put(c2);
                            continue;
                        }
                        // Unpaired low surrogate
                        cr = CoderResult::malformedForLength(2);
                        goto FINALLY;
                    }

                    if (!dst.hasRemaining()) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    mark += 2;
                    dst.put(c);
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

        void UTF_16Coder::Decoder::reset0() {
            currentByteOrder = exceptedByteOrder;
        }

        gchar UTF_16Coder::Decoder::decode(gint b1, gint b2) const {
            if (currentByteOrder == BIG)
                return (gchar)((b1 << 8) | b2);
            else
                return (gchar)((b2 << 8) | b1);
        }

        UTF_16Coder::Encoder::Encoder(Charset const& cs, gint bo, gbool m)
            : CharsetEncoder(
                cs, 2.0f,
                // Four bytes max if you need a BOM
                m ? 4.0f : 2.0f,
                // Replacement depends upon gbyte order
                ((bo == BIG)
                     ? ByteArray::of((gbyte)0xff, (gbyte)0xfd)
                     : ByteArray::of((gbyte)0xfd, (gbyte)0xff))) {
            usesMark = needsMark = m;
            byteOrder = bo;
        }

        gbool UTF_16Coder::Encoder::canEncode(gchar c) { return !Character::isSurrogate(c); }

        void UTF_16Coder::Encoder::reset0() {needsMark = usesMark; }

        CoderResult UTF_16Coder::Encoder::encode0(io::CharBuffer& src, io::ByteBuffer& dst) {
            int mark = src.position();

            if (needsMark && src.hasRemaining()) {
                if (dst.remaining() < 2)
                    return CoderResult::OVERFLOW;
                put(BYTE_ORDER_MARK, dst);
                needsMark = false;
            }

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    if (!Character::isSurrogate(c)) {
                        if (dst.remaining() < 2) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        mark++;
                        put(c, dst);
                        continue;
                    }
                    int d = sgp.parse(c, src);
                    if (d < 0) {
                        cr = sgp.error();
                        goto FINALLY;
                    }
                    if (dst.remaining() < 4) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    mark += 2;
                    put(Character::highSurrogate(d), dst);
                    put(Character::lowSurrogate(d), dst);
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

        void UTF_16Coder::Encoder::put(gchar c, io::ByteBuffer& dst) {
            if (byteOrder == BIG) {
                dst.put((gbyte)(c >> 8));
                dst.put((gbyte)(c & 0xff));
            } else {
                dst.put((gbyte)(c & 0xff));
                dst.put((gbyte)(c >> 8));
            }
        }
    } // charset
} // core
