//
// Created by brunshweeck on 6 sept. 2024.
//

#include <core/io/Buffer.h>
#include <core/io/ByteBuffer.h>
#include <meta/charset/SingleByte.h>
#include <meta/charset/StringUTF16.h>
#include <meta/charset/US_ASCII.h>

namespace core {
    namespace charset {
        SingleByte::Decoder::Decoder(Charset const &cs, CharArray &b2c)
            : CharsetDecoder(cs, 1.0F, 1.0F),
              b2c(b2c),
              asciiCompatible(false),
              latin1Decodable(false) {
        }

        SingleByte::Decoder::Decoder(Charset const &cs, CharArray &b2c, gbool isASCIICompatible)
            : CharsetDecoder(cs, 1.0F, 1.0F),
              b2c(b2c),
              asciiCompatible(isASCIICompatible),
              latin1Decodable(false) {
        }

        SingleByte::Decoder::Decoder(Charset const &cs, CharArray &b2c, gbool isASCIICompatible,
                                     gbool isLatin1Decodable)
            : CharsetDecoder(cs, 1.0F, 1.0F),
              b2c(b2c),
              asciiCompatible(isASCIICompatible),
              latin1Decodable(isLatin1Decodable) {
        }

        gchar SingleByte::Decoder::decode(gint b) const {
            return b2c[b + 128];
        }

        gint SingleByte::Decoder::decodeToLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) {
            if (len > dst.length())
                len = dst.length();

            gint dp = 0;
            while (dp < len) {
                dst[dp++] = (gbyte) decode(src[sp++]);
            }
            return dp;
        }

        gint SingleByte::Decoder::decode(const ByteArray &src, gint sp, gint len, CharArray &dst) {
            if (len > dst.length())
                len = dst.length();
            gint dp = 0;
            while (dp < len) {
                dst[dp] = decode(src[sp++]);
                if (dst[dp] == UNMAPPABLE_DECODING) {
                    dst[dp] = repl;
                }
                dp++;
            }
            return dp;
        }

        gbool SingleByte::Decoder::isASCIICompatible() const { return asciiCompatible; }

        gbool SingleByte::Decoder::isLatin1Decodable() const { return latin1Decodable; }

        CoderResult SingleByte::Decoder::decode0(io::ByteBuffer &src, io::CharBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArray(src, dst);

            return decodeBuffer(src, dst);
        }

        void SingleByte::Decoder::replaceWith0(const String &newReplacement) {
            repl = newReplacement.charAt(0);
        }

        CoderResult SingleByte::Decoder::decodeArray(io::ByteBuffer &src, io::CharBuffer &dst) {
            ByteArray &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            CharArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();

            CoderResult cr = CoderResult::UNDERFLOW;
            if ((dl - dp) < (sl - sp)) {
                sl = sp + (dl - dp);
                cr = CoderResult::OVERFLOW;
            }

            if (asciiCompatible) {
                gint n = US_ASCII::decodeASCII(sa, sp, da, dp, Math::min(dl - dp, sl - sp));
                sp += n;
                dp += n;
            }
            while (sp < sl) {
                gchar c = decode(sa[sp]);
                if (c == UNMAPPABLE_DECODING) {
                    return withResult(CoderResult::unmappableForLength(1),
                                      src, sp, dst, dp);
                }
                da[dp++] = c;
                sp++;
            }
            return withResult(cr, src, sp, dst, dp);
        }

        CoderResult SingleByte::Decoder::decodeBuffer(io::ByteBuffer &src, io::CharBuffer &dst) {
            gint mark = src.position();
            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gchar c = decode(src.get());
                    if (c == UNMAPPABLE_DECODING) {
                        cr = CoderResult::unmappableForLength(1);
                        goto FINALLY;
                    }
                    if (!dst.hasRemaining()) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    dst.put(c);
                    mark++;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) {
                catchEx = &ex;
            }
        FINALLY:
            src.position(mark);
            if (catchEx)
                catchEx->throws($ftrace());
            return cr;
        }

        SingleByte::Encoder::Encoder(Charset const &cs, CharArray &c2b, CharArray &c2bIndex, gbool isASCIICompatible)
            : CharsetEncoder(cs, 1.0F, 1.0F),
              c2b(c2b),
              c2bIndex(c2bIndex),
              asciiCompatible(isASCIICompatible) {
        }

        gbool SingleByte::Encoder::canEncode(gchar c) {
            return encode(c) != UNMAPPABLE_ENCODING;
        }

        gbool SingleByte::Encoder::isLegalReplacement(const ByteArray &repl) const {
            return ((repl.length() == 1 && repl[0] == (gbyte) '?') ||
                    CharsetEncoder::isLegalReplacement(repl));
        }

        gint SingleByte::Encoder::encode(const CharArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint sl = sp + Math::min(len, dst.length());
            while (sp < sl) {
                gchar c = src[sp++];
                gint b = encode(c);
                if (b != UNMAPPABLE_ENCODING) {
                    dst[dp++] = (gbyte) b;
                    continue;
                }
                if (Character::isHighSurrogate(c) && sp < sl &&
                    Character::isLowSurrogate(src[sp])) {
                    if (len > dst.length()) {
                        sl++;
                        len--;
                    }
                    sp++;
                }
                dst[dp++] = repl;
            }
            return dp;
        }

        gint SingleByte::Encoder::encode(gchar ch) const {
            gchar index = c2bIndex[ch >> 8];
            if (index == UNMAPPABLE_ENCODING)
                return UNMAPPABLE_ENCODING;
            return c2b[index + (ch & 0xff)];
        }

        gint SingleByte::Encoder::encodeFromLatin1(const ByteArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint sl = sp + Math::min(len, dst.length());
            while (sp < sl) {
                gchar c = (gchar) (src[sp++] & 0xff);
                gint b = encode(c);
                if (b == UNMAPPABLE_ENCODING) {
                    dst[dp++] = repl;
                } else {
                    dst[dp++] = (gbyte) b;
                }
            }
            return dp;
        }

        gint SingleByte::Encoder::encodeFromUTF16(const ByteArray &src, gint sp, gint len, ByteArray &dst) {
            gint dp = 0;
            gint sl = sp + Math::min(len, dst.length());
            while (sp < sl) {
                gchar c = StringUTF16::getChar(src, sp++);
                gint b = encode(c);
                if (b != UNMAPPABLE_ENCODING) {
                    dst[dp++] = (gbyte) b;
                    continue;
                }
                if (Character::isHighSurrogate(c) && sp < sl &&
                    Character::isLowSurrogate(StringUTF16::getChar(src, sp))) {
                    if (len > dst.length()) {
                        sl++;
                        len--;
                    }
                    sp++;
                }
                dst[dp++] = repl;
            }
            return dp;
        }

        gbool SingleByte::Encoder::isASCIICompatible() const { return asciiCompatible; }

        CoderResult SingleByte::Encoder::encode0(io::CharBuffer &src, io::ByteBuffer &dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArray(src, dst);
            else
                return encodeBuffer(src, dst);
        }

        void SingleByte::Encoder::replaceWith0(ByteArray const &newReplacement) {
            repl = newReplacement[0];
        }

        CoderResult SingleByte::Encoder::encodeArray(io::CharBuffer &src, io::ByteBuffer &dst) {
            CharArray &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            ByteArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();
            gint len = Math::min(dl - dp, sl - sp);

            if (asciiCompatible) {
                gint n = US_ASCII::encodeASCII(sa, sp, da, dp, len);
                sp += n;
                dp += n;
                len -= n;
            }
            while (len-- > 0) {
                gchar c = sa[sp];
                gint b = encode(c);
                if (b == UNMAPPABLE_ENCODING) {
                    if (Character::isSurrogate(c)) {
                        if (sgp.parse(c, sa, sp, sl) < 0) {
                            return withResult(sgp.error(), src, sp, dst, dp);
                        }
                        return withResult(sgp.unmappableResult(), src, sp, dst, dp);
                    }
                    return withResult(CoderResult::unmappableForLength(1),
                                      src, sp, dst, dp);
                }
                da[dp++] = (gbyte) b;
                sp++;
            }
            return withResult(sp < sl ? CoderResult::OVERFLOW : CoderResult::UNDERFLOW,
                              src, sp, dst, dp);
        }

        CoderResult SingleByte::Encoder::encodeBuffer(io::CharBuffer &src, io::ByteBuffer &dst) {
            gint mark = src.position();
            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    gint b = encode(c);
                    if (b == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            if (sgp.parse(c, src) < 0) {
                                cr = sgp.error();
                            } else
                                cr = sgp.unmappableResult();
                            goto FINALLY;
                        }
                        return CoderResult::unmappableForLength(1);
                    }
                    if (!dst.hasRemaining()) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    dst.put((gbyte) b);
                    mark++;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { catchEx = &ex; }

        FINALLY:
            src.position(mark);

            if (catchEx)
                catchEx->throws($ftrace());

            return cr;
        }

        void SingleByte::initC2B(CharArray &b2c, CharArray &c2bNR, CharArray &c2b, CharArray &c2bIndex) {
            {
                for (gint i = 0; i < c2bIndex.length(); i++)
                    c2bIndex[i] = UNMAPPABLE_ENCODING;
                for (gint i = 0; i < c2b.length(); i++)
                    c2b[i] = UNMAPPABLE_ENCODING;
                gint off = 0;
                for (gint i = 0; i < b2c.length(); i++) {
                    gchar c = b2c[i];
                    if (c == UNMAPPABLE_DECODING)
                        continue;
                    gint index = (c >> 8);
                    if (c2bIndex[index] == UNMAPPABLE_ENCODING) {
                        c2bIndex[index] = (gchar) off;
                        off += 0x100;
                    }
                    index = c2bIndex[index] + (c & 0xff);
                    c2b[index] = (gchar) ((i >= 0x80) ? (i - 0x80) : (i + 0x80));
                }
                if (!c2bNR.isEmpty()) {
                    // c-->b nr entries
                    gint i = 0;
                    while (i < c2bNR.length()) {
                        gchar b = c2bNR[i++];
                        gchar c = c2bNR[i++];
                        gint index = (c >> 8);
                        if (c2bIndex[index] == UNMAPPABLE_ENCODING) {
                            c2bIndex[index] = (gchar) off;
                            off += 0x100;
                        }
                        index = c2bIndex[index] + (c & 0xff);
                        c2b[index] = b;
                    }
                }
            }
        }

        CoderResult SingleByte::withResult(CoderResult cr,
                                           io::Buffer &src, gint sp,
                                           io::Buffer &dst, gint dp) {
            src.position(sp - src.arrayOffset());
            dst.position(dp - dst.arrayOffset());
            return cr;
        }
    } // charset
} // core
