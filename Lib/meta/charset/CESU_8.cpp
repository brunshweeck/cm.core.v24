//
// Created by brunshweeck on 6 sept. 2024.
//

#include <core/lang/AssertionError.h>
#include <core/io/Buffer.h>
#include <core/io/ByteBuffer.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/CESU_8.h>
#include <meta/charset/US_ASCII.h>

namespace core {
    namespace charset {
        CESU_8::CESU_8(): Unicode("CESU-8"_S, StringArray::of("CESU8"_S, "csCESU-8"_S)) {
        }

        CharsetDecoder &CESU_8::newDecoder() const { return UNSAFE::newInstance<Decoder>(*this); }

        CharsetEncoder &CESU_8::newEncoder() const { return UNSAFE::newInstance<Encoder>(*this); }

        void CESU_8::updatePositions(io::Buffer &src, gint sp, io::Buffer &dst, gint dp) {
            src.position(sp - src.arrayOffset());
            dst.position(dp - dst.arrayOffset());
        }

        CESU_8::Decoder::Decoder(Charset const &cs): CharsetDecoder(cs, 1.0F, 1.0F) {
        }

        gint CESU_8::Decoder::decode(ByteArray const &sa, gint sp, gint len, CharArray &da) {
            gint sl = sp + len;
            gint dp = 0;
            gint dlASCII = Math::min(len, da.length());
            io::ByteBuffer *bb = null; // only necessary if malformed

            // ASCII only optimized loop
            while (dp < dlASCII && sa[sp] >= 0)
                da[dp++] = (gchar) sa[sp++];

            while (sp < sl) {
                gint b1 = sa[sp++];
                if (b1 >= 0) {
                    // 1 byte, 7 bits: 0xxxxxxx
                    da[dp++] = (gchar) b1;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    if (sp < sl) {
                        gint b2 = sa[sp++];
                        if (isNotContinuation(b2)) {
                            if (malformedInputAction() != CodingErrorAction::REPLACE)
                                return -1;
                            da[dp++] = replacement().charAt(0);
                            sp--; // malformedN(bb, 2) always returns 1
                        } else {
                            da[dp++] = (gchar) (((b1 << 6) ^ b2) ^
                                                (((gbyte) 0xC0 << 6) ^
                                                 ((gbyte) 0x80 << 0)));
                        }
                        continue;
                    }
                    if (malformedInputAction() != CodingErrorAction::REPLACE)
                        return -1;
                    da[dp++] = replacement().charAt(0);
                    return dp;
                } else if ((b1 >> 4) == -2) {
                    // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                    if (sp + 1 < sl) {
                        gint b2 = sa[sp++];
                        gint b3 = sa[sp++];
                        if (isMalformed3(b1, b2, b3)) {
                            if (malformedInputAction() != CodingErrorAction::REPLACE)
                                return -1;
                            da[dp++] = replacement().charAt(0);
                            sp -= 3;
                            if (!bb)
                                bb = &io::ByteBuffer::wrap(CORE_CAST(ByteArray &, sa));
                            bb->position(sp);

                            sp += malformedN(*bb, 3).length();
                        } else {
                            da[dp++] = (gchar) ((b1 << 12) ^
                                                (b2 << 6) ^
                                                (b3 ^
                                                 (((gbyte) 0xE0 << 12) ^
                                                  ((gbyte) 0x80 << 6) ^
                                                  ((gbyte) 0x80 << 0))));
                        }
                        continue;
                    }
                    if (malformedInputAction() != CodingErrorAction::REPLACE)
                        return -1;
                    if (sp < sl && isMalformed3_2(b1, sa[sp])) {
                        da[dp++] = replacement().charAt(0);
                        continue;
                    }
                    da[dp++] = replacement().charAt(0);
                    return dp;
                } else {
                    if (malformedInputAction() != CodingErrorAction::REPLACE)
                        return -1;
                    da[dp++] = replacement().charAt(0);
                }
            }
            return dp;
        }

        CoderResult CESU_8::Decoder::decode0(io::ByteBuffer &in, io::CharBuffer &out) {
            try {
                if (in.hasArray() && out.hasArray())
                    return decodeArray(in, out);

                return decodeBuffer(in, out);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gbool CESU_8::Decoder::isNotContinuation(gint b) { return (b & 0xc0) != 0x80; }

        gbool CESU_8::Decoder::isMalformed3(gint b1, gint b2, gint b3) {
            return (b1 == CORE_CAST(gbyte, 0xe0) && (b2 & 0xe0) == 0x80) ||
                   (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80;
        }

        gbool CESU_8::Decoder::isMalformed3_2(gint b1, gint b2) {
            return (b1 == CORE_CAST(gbyte, 0xe0) && (b2 & 0xe0) == 0x80) ||
                   (b2 & 0xc0) != 0x80;
        }

        CoderResult CESU_8::Decoder::malformedN(io::ByteBuffer &src, gint nb) {
            gint b1, b2;
            switch (nb) {
                case 1:
                case 2: // always 1
                    return CoderResult::malformedForLength(1);
                case 3:
                    b1 = src.get();
                    b2 = src.get(); // no need to lookup b3
                    return CoderResult::malformedForLength(
                        ((b1 == CORE_CAST(gbyte, 0xe0) && (b2 & 0xe0) == 0x80) || isNotContinuation(b2)) ? 1 : 2);
                case 4: // we don't care the speed here
                    b1 = src.get() & 0xff;
                    b2 = src.get() & 0xff;
                    if (b1 > 0xf4 ||
                        (b1 == 0xf0 && (b2 < 0x90 || b2 > 0xbf)) ||
                        (b1 == 0xf4 && (b2 & 0xf0) != 0x80) ||
                        isNotContinuation(b2))
                        return CoderResult::malformedForLength(1);
                    if (isNotContinuation(src.get()))
                        return CoderResult::malformedForLength(2);
                    return CoderResult::malformedForLength(3);
                default:
                    CORE_ASSERT(false);
                    return CoderResult::UNDEFINED;
            }
        }

        CoderResult CESU_8::Decoder::malformed(io::ByteBuffer &src, gint sp, io::CharBuffer &dst, gint dp, gint nb) {
            src.position(sp - src.arrayOffset());
            CoderResult cr = malformedN(src, nb);
            updatePositions(src, sp, dst, dp);
            return cr;
        }

        CoderResult CESU_8::Decoder::malformed(io::ByteBuffer &src, gint mark, gint nb) {
            src.position(mark);
            CoderResult cr = malformedN(src, nb);
            src.position(mark);
            return cr;
        }

        CoderResult CESU_8::Decoder::malformedForLength(io::ByteBuffer &src, gint sp,
                                                        io::CharBuffer &dst, gint dp,
                                                        gint malformedNB) {
            updatePositions(src, sp, dst, dp);
            return CoderResult::malformedForLength(malformedNB);
        }

        CoderResult CESU_8::Decoder::malformedForLength(io::ByteBuffer &src, gint mark, gint malformedNB) {
            src.position(mark);
            return CoderResult::malformedForLength(malformedNB);
        }

        CoderResult CESU_8::Decoder::xflow(io::Buffer &src, gint sp, gint sl, io::Buffer &dst, gint dp, gint nb) {
            updatePositions(src, sp, dst, dp);
            return (nb == 0 || sl - sp < nb)
                       ? CoderResult::UNDERFLOW
                       : CoderResult::OVERFLOW;
        }

        CoderResult CESU_8::Decoder::xflow(io::Buffer &src, gint mark, gint nb) {
            src.position(mark);
            return (nb == 0 || src.remaining() < nb)
                       ? CoderResult::UNDERFLOW
                       : CoderResult::OVERFLOW;
        }

        CoderResult CESU_8::Decoder::decodeArray(io::ByteBuffer &src, io::CharBuffer &dst) {
            // This method is optimized for ASCII input.
            ByteArray &sa = src.array();
            gint soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint sl = soff + src.limit();

            CharArray &da = dst.array();
            gint doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint dl = doff + dst.limit();

            gint n = US_ASCII::decodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            while (sp < sl) {
                gint b1 = sa[sp];
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    if (dp >= dl)
                        return xflow(src, sp, sl, dst, dp, 1);
                    da[dp++] = (gchar) b1;
                    sp++;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    if (sl - sp < 2 || dp >= dl)
                        return xflow(src, sp, sl, dst, dp, 2);
                    gint b2 = sa[sp + 1];
                    if (isNotContinuation(b2))
                        return malformedForLength(src, sp, dst, dp, 1);
                    da[dp++] = (gchar) (((b1 << 6) ^ b2)
                                        ^
                                        (((gbyte) 0xC0 << 6) ^
                                         ((gbyte) 0x80 << 0)));
                    sp += 2;
                } else if ((b1 >> 4) == -2) {
                    // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                    gint srcRemaining = sl - sp;
                    if (srcRemaining < 3 || dp >= dl) {
                        if (srcRemaining > 1 && isMalformed3_2(b1, sa[sp + 1]))
                            return malformedForLength(src, sp, dst, dp, 1);
                        return xflow(src, sp, sl, dst, dp, 3);
                    }
                    gint b2 = sa[sp + 1];
                    gint b3 = sa[sp + 2];
                    if (isMalformed3(b1, b2, b3))
                        return malformed(src, sp, dst, dp, 3);
                    da[dp++] = (gchar)
                    ((b1 << 12) ^
                     (b2 << 6) ^
                     (b3 ^
                      (((gbyte) 0xE0 << 12) ^
                       ((gbyte) 0x80 << 6) ^
                       ((gbyte) 0x80 << 0))));
                    sp += 3;
                } else {
                    return malformed(src, sp, dst, dp, 1);
                }
            }
            return xflow(src, sp, sl, dst, dp, 0);
        }

        CoderResult CESU_8::Decoder::decodeBuffer(io::ByteBuffer &src, io::CharBuffer &dst) {
            gint mark = src.position();
            gint limit = src.limit();
            while (mark < limit) {
                gint b1 = src.get();
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    if (dst.remaining() < 1)
                        return xflow(src, mark, 1); // overflow
                    dst.put((gchar) b1);
                    mark++;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    if (limit - mark < 2 || dst.remaining() < 1)
                        return xflow(src, mark, 2);
                    gint b2 = src.get();
                    if (isNotContinuation(b2))
                        return malformedForLength(src, mark, 1);
                    dst.put((gchar) (((b1 << 6) ^ b2)
                                     ^
                                     (((gbyte) 0xC0 << 6) ^
                                      ((gbyte) 0x80 << 0))));
                    mark += 2;
                } else if ((b1 >> 4) == -2) {
                    // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                    gint srcRemaining = limit - mark;
                    if (srcRemaining < 3 || dst.remaining() < 1) {
                        if (srcRemaining > 1 && isMalformed3_2(b1, src.get()))
                            return malformedForLength(src, mark, 1);
                        return xflow(src, mark, 3);
                    }
                    gint b2 = src.get();
                    gint b3 = src.get();
                    if (isMalformed3(b1, b2, b3))
                        return malformed(src, mark, 3);
                    dst.put((gchar)
                    ((b1 << 12) ^
                     (b2 << 6) ^
                     (b3 ^
                      (((gbyte) 0xE0 << 12) ^
                       ((gbyte) 0x80 << 6) ^
                       ((gbyte) 0x80 << 0)))));
                    mark += 3;
                } else {
                    return malformed(src, mark, 1);
                }
            }
            return xflow(src, mark, 0);
        }

        CESU_8::Encoder::Encoder(Charset const &cs): CharsetEncoder(cs, 1.1F, 3.0F) {
        }

        gbool CESU_8::Encoder::canEncode(gchar c) { return !Character::isSurrogate(c); }

        gbool CESU_8::Encoder::isLegalReplacement(const ByteArray &repl) const {
            return (repl.length() == 1 && repl[0] >= 0) || CharsetEncoder::isLegalReplacement(repl);
        }

        gint CESU_8::Encoder::encode(CharArray const &sa, gint sp, gint len, ByteArray &da) {
            gint sl = sp + len;
            gint dp = 0;

            // Handle ASCII-only prefix
            gint n = US_ASCII::encodeASCII(sa, sp, da, dp, Math::min(len, da.length()));
            sp += n;
            dp += n;

            while (sp < sl) {
                gchar c = sa[sp++];
                if (c < 0x80) {
                    // Have at most seven bits
                    da[dp++] = (gbyte) c;
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    da[dp++] = (gbyte) (0xc0 | (c >> 6));
                    da[dp++] = (gbyte) (0x80 | (c & 0x3f));
                } else if (Character::isSurrogate(c)) {
                    //
                    gint uc = sgp.parse(c, sa, sp - 1, sl);
                    if (uc < 0) {
                        if (malformedInputAction() != CodingErrorAction::REPLACE)
                            return -1;
                        da[dp++] = replacement()[0];
                    } else {
                        to3Bytes(da, dp, Character::highSurrogate(uc));
                        dp += 3;
                        to3Bytes(da, dp, Character::lowSurrogate(uc));
                        dp += 3;
                        sp++; // 2 chars
                    }
                } else {
                    // 3 bytes, 16 bits
                    to3Bytes(da, dp, c);
                    dp += 3;
                }
            }
            return dp;
        }

        CoderResult CESU_8::Encoder::encode0(io::CharBuffer &in, io::ByteBuffer &out) {
            if (in.hasArray() && out.hasArray())
                return encodeArray(in, out);

            return encodeBuffer(in, out);
        }

        CoderResult CESU_8::Encoder::overflow(io::CharBuffer &src, gint sp, io::ByteBuffer &dst, gint dp) {
            updatePositions(src, sp, dst, dp);
            return CoderResult::OVERFLOW;
        }

        CoderResult CESU_8::Encoder::overflow(io::CharBuffer &src, gint mark) {
            src.position(mark);
            return CoderResult::OVERFLOW;
        }

        void CESU_8::Encoder::to3Bytes(ByteArray &da, gint dp, gchar c) {
            da[dp] = (gbyte) (0xe0 | ((c >> 12)));
            da[dp + 1] = (gbyte) (0x80 | ((c >> 6) & 0x3f));
            da[dp + 2] = (gbyte) (0x80 | (c & 0x3f));
        }

        void CESU_8::Encoder::to3Bytes(io::ByteBuffer &dst, gchar c) {
            dst.put((gbyte) (0xe0 | ((c >> 12))));
            dst.put((gbyte) (0x80 | ((c >> 6) & 0x3f)));
            dst.put((gbyte) (0x80 | (c & 0x3f)));
        }

        CoderResult CESU_8::Encoder::encodeArray(io::CharBuffer &src, io::ByteBuffer &dst) {
            CharArray &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            ByteArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();

            // Handle ASCII-only prefix
            gint n = US_ASCII::encodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            while (sp < sl) {
                gchar c = sa[sp];
                if (c < 0x80) {
                    // Have at most seven bits
                    if (dp >= dl)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte) c;
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    if (dl - dp < 2)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte) (0xc0 | (c >> 6));
                    da[dp++] = (gbyte) (0x80 | (c & 0x3f));
                } else if (Character::isSurrogate(c)) {
                    // Have a surrogate pair
                    gint uc = sgp.parse(c, sa, sp, sl);
                    if (uc < 0) {
                        updatePositions(src, sp, dst, dp);
                        return sgp.error();
                    }
                    if (dl - dp < 6)
                        return overflow(src, sp, dst, dp);
                    to3Bytes(da, dp, Character::highSurrogate(uc));
                    dp += 3;
                    to3Bytes(da, dp, Character::lowSurrogate(uc));
                    dp += 3;
                    sp++; // 2 chars
                } else {
                    // 3 bytes, 16 bits
                    if (dl - dp < 3)
                        return overflow(src, sp, dst, dp);
                    to3Bytes(da, dp, c);
                    dp += 3;
                }
                sp++;
            }
            updatePositions(src, sp, dst, dp);
            return CoderResult::UNDERFLOW;
        }

        CoderResult CESU_8::Encoder::encodeBuffer(io::CharBuffer &src, io::ByteBuffer &dst) {
            gint mark = src.position();
            while (src.hasRemaining()) {
                gchar c = src.get();
                if (c < 0x80) {
                    // Have at most seven bits
                    if (!dst.hasRemaining())
                        return overflow(src, mark);
                    dst.put((gbyte) c);
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    if (dst.remaining() < 2)
                        return overflow(src, mark);
                    dst.put((gbyte) (0xc0 | (c >> 6)));
                    dst.put((gbyte) (0x80 | (c & 0x3f)));
                } else if (Character::isSurrogate(c)) {
                    // Have a surrogate pair
                    gint uc = sgp.parse(c, src);
                    if (uc < 0) {
                        src.position(mark);
                        return sgp.error();
                    }
                    if (dst.remaining() < 6)
                        return overflow(src, mark);
                    to3Bytes(dst, Character::highSurrogate(uc));
                    to3Bytes(dst, Character::lowSurrogate(uc));
                    mark++; // 2 chars
                } else {
                    // 3 bytes, 16 bits
                    if (dst.remaining() < 3)
                        return overflow(src, mark);
                    to3Bytes(dst, c);
                }
                mark++;
            }
            src.position(mark);
            return CoderResult::UNDERFLOW;
        }
    } // charset
} // core
