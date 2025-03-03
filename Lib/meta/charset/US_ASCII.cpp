//
// Created by brunshweeck on 6 sept. 2024.
//

#include <core/io/ByteBuffer.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/US_ASCII.h>

namespace core {
    namespace charset {
        US_ASCII::US_ASCII()
            : Charset("US-ASCII"_S, StringArray::of(
                          "iso-ir-6"_S,
                          "ANSI_X3.4-1986"_S,
                          "ISO_646.irv:1991"_S,
                          "ASCII"_S,
                          "ISO646-US"_S,
                          "us"_S,
                          "IBM367"_S,
                          "cp367"_S,
                          "csASCII"_S,
                          "646"_S,
                          "iso_646.irv:1983"_S,
                          "ANSI_X3.4-1968"_S,
                          "ascii7"_S
                      )) {
        }

        gbool US_ASCII::contains(const Charset &cs) const { return Class<US_ASCII>::hasInstance(*this); }

        CharsetDecoder &US_ASCII::newDecoder() const {
            try {
                return UNSAFE::newInstance<Decoder>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &US_ASCII::newEncoder() const {
            try {
                return UNSAFE::newInstance<Encoder>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint US_ASCII::decodeASCII(ByteArray const &src, gint srcOff, CharArray &dst, gint dstOff, gint len) {
            // 1- Count positives bytes
            gint n = 0;
            gint lim = srcOff + len;
            for (gint i = srcOff; i < lim && src[i] >= 0; i++, n++) {
                //
            }

            // 2- Inflate
            inflateBytesToChars(src, srcOff, dst, dstOff, n);

            return n;
        }

        gint US_ASCII::encodeASCII(CharArray const &src, gint srcOff, ByteArray &dst, gint dstOff, gint len) {
            // 1- Count ascii chars
            gint n = 0;
            gint lim = srcOff + len;
            for (gint i = srcOff; i < lim && src[i] < 0x80; i++, n++) {
                //
            }

            // 2- Inflate
            inflateCharsToBytes(src, srcOff, dst, dstOff, n);

            return n;
        }

        void US_ASCII::inflateBytesToChars(ByteArray const &src, gint srcOff, CharArray &dst, gint dstOff, gint len) {
            for (int i = 0; i < len; ++i) {
                dst[dstOff++] = CORE_CAST(gchar, src[srcOff++] & 0xff);
            }
        }

        void US_ASCII::inflateCharsToBytes(CharArray const &src, gint srcOff, ByteArray &dst, gint dstOff, gint len) {
            for (int i = 0; i < len; ++i) {
                dst[dstOff++] = CORE_CAST(gbyte, src[srcOff++]);
            }
        }

        US_ASCII::Decoder::Decoder(Charset const &cs)
            : CharsetDecoder(cs, 1.0F, 1.0F) {
        }

        CoderResult US_ASCII::Decoder::decode0(io::ByteBuffer &in, io::CharBuffer &out) {
            try {
                if (in.hasArray() && out.hasArray())
                    return decodeArray(in, out);

                return decodeBuffer(in, out);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CoderResult US_ASCII::Decoder::decodeArray(io::ByteBuffer &src, io::CharBuffer &dst) {
            try {
                ByteArray &sa = src.array();
                gint soff = src.arrayOffset();
                gint sp = soff + src.position();
                gint sl = soff + src.limit();

                CharArray &da = dst.array();
                gint doff = dst.arrayOffset();
                gint dp = doff + dst.position();
                gint dl = doff + dst.limit();

                // ASCII only loop
                gint n = decodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
                sp += n;
                dp += n;
                src.position(sp - soff);
                dst.position(dp - doff);
                if (sp < sl) {
                    if (dp >= dl) {
                        return CoderResult::OVERFLOW;
                    }
                    return CoderResult::malformedForLength(1);
                }
                return CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CoderResult US_ASCII::Decoder::decodeBuffer(io::ByteBuffer &src, io::CharBuffer &dst) {
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = {};

            try {
                while (src.hasRemaining()) {
                    gbyte b = src.get();
                    if (b >= 0) {
                        if (!dst.hasRemaining()) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        dst.put(CORE_CAST(gchar, b));
                        mark++;
                        continue;
                    }
                    cr = CoderResult::malformedForLength(1);
                    goto FINALLY;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { catchEx = &ex; }

        FINALLY:
            src.position(mark);

            if (catchEx)
                catchEx->throws($ftrace());

            return cr;
        }

        US_ASCII::Encoder::Encoder(Charset const &cs)
            : CharsetEncoder(cs, 1.0F, 1.0F) {
        }

        gbool US_ASCII::Encoder::isLegalReplacement(const ByteArray &repl) const {
            return (repl.length() == 1 && repl[0] >= 0) || CharsetEncoder::isLegalReplacement(repl);
        }

        gbool US_ASCII::Encoder::canEncode(gchar c) { return c < 0x80; }

        CoderResult US_ASCII::Encoder::encode0(io::CharBuffer &in, io::ByteBuffer &out) {
            try {
                if (in.hasArray() && out.hasArray())
                    return encodeArray(in, out);

                return encodeBuffer(in, out);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CoderResult US_ASCII::Encoder::encodeArray(io::CharBuffer &src, io::ByteBuffer &dst) {
            CharArray &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();
            CORE_ASSERT(sp <= sl);
            sp = (sp <= sl ? sp : sl);
            ByteArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();
            CORE_ASSERT(dp <= dl);
            dp = (dp <= dl ? dp : dl);

            gint n = encodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = {};

            try {
                while (sp < sl) {
                    gchar c = sa[sp];
                    if (c < 0x80) {
                        if (dp >= dl) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        da[dp] = CORE_CAST(gbyte, c);
                        sp++;
                        dp++;
                        continue;
                    }
                    if (sgp.parse(c, sa, sp, sl) < 0)
                        cr = sgp.error();
                    else
                        cr = sgp.unmappableResult();

                    goto FINALLY;
                }

                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { catchEx = &ex; }


        FINALLY:
            src.position(sp - src.arrayOffset());
            dst.position(dp - dst.arrayOffset());

            if (catchEx)
                catchEx->throws($ftrace());

            return cr;
        }

        CoderResult US_ASCII::Encoder::encodeBuffer(io::CharBuffer &src, io::ByteBuffer &dst) {
            gint mark = src.position();
            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = {};

            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    if (c < 0x80) {
                        if (!dst.hasRemaining()) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        dst.put(CORE_CAST(gbyte, c));
                        mark++;
                        continue;
                    }
                    if (sgp.parse(c, src) < 0)
                        cr = sgp.error();
                    else
                        cr = sgp.unmappableResult();
                    goto FINALLY;
                }

                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { catchEx = &ex; }

        FINALLY:
            src.position(mark);

            if (catchEx)
                catchEx->throws($ftrace());

            return cr;
        }
    } // charset
} // core
