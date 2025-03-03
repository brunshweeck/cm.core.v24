//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/io/ByteBuffer.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/ISO_8859_1.h>
#include <meta/charset/US_ASCII.h>

namespace core {
    namespace charset {
        ISO_8859_1::ISO_8859_1()
            : Charset("ISO-8859-1"_S, StringArray::of(
                          "iso-ir-100"_S,
                          "ISO_8859-1"_S,
                          "latin1"_S,
                          "l1"_S,
                          "IBM819"_S,
                          "cp819"_S,
                          "csISOLatin1"_S,
                          "819"_S,
                          "IBM-819"_S,
                          "ISO8859_1"_S,
                          "ISO_8859-1:1987"_S,
                          "ISO_8859_1"_S,
                          "8859_1"_S,
                          "ISO8859-1"_S)) {
        }

        gbool ISO_8859_1::contains(const Charset& cs) const {
            return Class<charset::US_ASCII>::hasInstance(cs)
                || Class<ISO_8859_1>::hasInstance(cs);
        }

        CharsetDecoder& ISO_8859_1::newDecoder() const {
            try {
                return UNSAFE::newInstance<Decoder>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& ISO_8859_1::newEncoder() const {
            try {
                return UNSAFE::newInstance<Encoder>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ISO_8859_1::Decoder::Decoder(Charset const& cs)
            : CharsetDecoder(cs, 1.0F, 1.0F) {
        }

        CoderResult ISO_8859_1::Decoder::decode0(io::ByteBuffer& src, io::CharBuffer& dst) {
            try {
                if (src.hasArray() && dst.hasArray())
                    return decodeArray(src, dst);

                return decodeBuffer(src, dst);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CoderResult ISO_8859_1::Decoder::decodeArray(io::ByteBuffer& src, io::CharBuffer& dst) {
            ByteArray& sa = src.array();
            gint soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint sl = soff + src.limit();

            CharArray& da = dst.array();
            gint doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint dl = doff + dst.limit();

            gint decodeLen = Math::min(sl - sp, dl - dp);
            US_ASCII::inflateBytesToChars(sa, sp, da, dp, decodeLen);
            sp += decodeLen;
            dp += decodeLen;
            src.position(sp - soff);
            dst.position(dp - doff);
            if (sl - sp > dl - dp) {
                return CoderResult::OVERFLOW;
            }
            return CoderResult::UNDERFLOW;
        }

        CoderResult ISO_8859_1::Decoder::decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst) {
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gbyte b = src.get();
                    if (!dst.hasRemaining()) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    dst.put(CORE_CAST(gchar, b & 0xff));
                    mark++;
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

        ISO_8859_1::Encoder::Encoder(Charset const& cs)
            : CharsetEncoder(cs, 1.0F, 1.0F) {
        }

        gbool ISO_8859_1::Encoder::isLegalReplacement(const ByteArray& repl) const {
            return true; // we accept any gbyte value
        }

        gbool ISO_8859_1::Encoder::canEncode(gchar c) {
            return c <= u'\u00FF';
        }

        CoderResult ISO_8859_1::Encoder::encode0(io::CharBuffer& src, io::ByteBuffer& dst) {
            try {
                if (src.hasArray() && dst.hasArray())
                    return encodeArray(src, dst);
                else
                    return encodeBuffer(src, dst);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CoderResult ISO_8859_1::Encoder::encodeArray(io::CharBuffer& src, io::ByteBuffer& dst) {
            CharArray& sa = src.array();
            gint soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint sl = soff + src.limit();
            CORE_ASSERT(sp <= sl);
            sp = (sp <= sl ? sp : sl);
            ByteArray& da = dst.array();
            gint doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint dl = doff + dst.limit();
            CORE_ASSERT(dp <= dl);
            dp = (dp <= dl ? dp : dl);
            gint dlen = dl - dp;
            gint slen = sl - sp;
            gint len = (dlen < slen) ? dlen : slen;

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                gint ret = encodeISOArray(sa, sp, da, dp, len);
                sp = sp + ret;
                dp = dp + ret;
                if (ret != len) {
                    if (sgp.parse(sa[sp], sa, sp, sl) < 0)
                        cr = sgp.error();
                    else
                        cr = sgp.unmappableResult();
                    goto FINALLY;
                }
                if (len < slen)
                    cr = CoderResult::OVERFLOW;
                else
                    cr = CoderResult::UNDERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }
        FINALLY: {
                src.position(sp - soff);
                dst.position(dp - doff);

                if (catchEx)
                    catchEx->throws($ftrace());

                return cr;
            }
        }

        CoderResult ISO_8859_1::Encoder::encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst) {
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    if (c <= u'\u00FF') {
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
            } catch (Throwable const& ex) { catchEx = &ex; }
        FINALLY: {
                src.position(mark);

                if (catchEx)
                    catchEx->throws($ftrace());

                return cr;
            }
        }

        gint ISO_8859_1::Encoder::encodeISOArray(CharArray const& sa, gint sp, ByteArray& da, gint dp, gint len) {
            if (len <= 0) {
                return 0;
            }
            try {
                encodeISOArrayCheck(sa, sp, da, dp, len);
                return encodeISOArray0(sa, sp, da, dp, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ISO_8859_1::Encoder::encodeISOArray0(CharArray const& sa, gint sp, ByteArray& da, gint dp, gint len) {
            try {
                gint i = 0;
                for (; i < len; i++) {
                    gchar c = sa[sp++];
                    if (c > u'\u00FF')
                        break;
                    da[dp++] = CORE_CAST(gbyte, c);
                }
                return i;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void ISO_8859_1::Encoder::encodeISOArrayCheck(CharArray const& sa, gint sp, ByteArray& da, gint dp, gint len) {
            try {
                misc::Preconditions::checkIndex(sp, sa.length());
                misc::Preconditions::checkIndex(dp, da.length());

                misc::Preconditions::checkIndex(sp + len - 1, sa.length());
                misc::Preconditions::checkIndex(dp + len - 1, da.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

    } // charset
} // core
