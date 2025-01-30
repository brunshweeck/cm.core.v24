//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/io/Buffer.h>
#include <core/io/ByteBuffer.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/US_ASCII.h>
#include <meta/charset/UTF_8.h>

namespace core {
    namespace charset {
        UTF_8::UTF_8()
            : Unicode("UTF-8"_S, StringArray::of("UTF8"_S, "unicode-1-1-utf-8"_S)) {
        }

        CharsetDecoder& UTF_8::newDecoder() const {
            try {
                return UNSAFE::newInstance<Decoder>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& UTF_8::newEncoder() const {
            try {
                return UNSAFE::newInstance<Encoder>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void UTF_8::updatePositions(io::Buffer& src, int sp, io::Buffer& dst, int dp) {
            src.position(sp - src.arrayOffset());
            dst.position(dp - dst.arrayOffset());
        }

        UTF_8::Decoder::Decoder(Charset const& cs)
            : CharsetDecoder(cs, 1.0F, 1.0F) {
        }

        CoderResult UTF_8::Decoder::decode0(io::ByteBuffer& in, io::CharBuffer& out) {
            if (in.hasArray() && out.hasArray())
                return decodeArray(in, out);
            else
                return decodeBuffer(in, out);
        }

        gbool UTF_8::Decoder::isNotContinuation(gint b) { return (b & 0xc0) != 0x80; }

        gbool UTF_8::Decoder::isMalformed3(gint b1, gint b2, gint b3) {
            return (b1 == (gbyte)0xe0 && (b2 & 0xe0) == 0x80) ||
                (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80;
        }

        gbool UTF_8::Decoder::isMalformed3_2(gint b1, gint b2) {
            return (b1 == (gbyte)0xe0 && (b2 & 0xe0) == 0x80) ||
                (b2 & 0xc0) != 0x80;
        }

        gbool UTF_8::Decoder::isMalformed4(gint b2, gint b3, gint b4) {
            return (b2 & 0xc0) != 0x80 || (b3 & 0xc0) != 0x80 ||
                (b4 & 0xc0) != 0x80;
        }

        gbool UTF_8::Decoder::isMalformed4_2(gint b1, gint b2) {
            return (b1 == 0xf0 && (b2 < 0x90 || b2 > 0xbf)) ||
                (b1 == 0xf4 && (b2 & 0xf0) != 0x80) ||
                (b2 & 0xc0) != 0x80;
        }

        gbool UTF_8::Decoder::isMalformed4_3(gint b3) {
            return (b3 & 0xc0) != 0x80;
        }

        CoderResult UTF_8::Decoder::malformedN(io::ByteBuffer& src, gint nb) {
            gint b1, b2;
            switch (nb) {
            case 1:
            case 2: // always 1
                return CoderResult::malformedForLength(1);
            case 3:
                b1 = src.get();
                b2 = src.get(); // no need to lookup b3
                return CoderResult::malformedForLength(
                    ((b1 == (gbyte)0xe0 && (b2 & 0xe0) == 0x80) ||
                        isNotContinuation(b2))
                        ? 1
                        : 2);
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

        CoderResult UTF_8::Decoder::malformed(io::ByteBuffer& src, gint sp, io::CharBuffer& dst, gint dp, gint nb) {
            src.position(sp - src.arrayOffset());
            CoderResult cr = malformedN(src, nb);
            updatePositions(src, sp, dst, dp);
            return cr;
        }

        CoderResult UTF_8::Decoder::malformed(io::ByteBuffer& src, gint mark, gint nb) {
            src.position(mark);
            CoderResult cr = malformedN(src, nb);
            src.position(mark);
            return cr;
        }

        CoderResult UTF_8::Decoder::malformedForLength(io::ByteBuffer& src, gint sp, io::CharBuffer& dst, gint dp,
                                                       gint malformedNB) {
            updatePositions(src, sp, dst, dp);
            return CoderResult::malformedForLength(malformedNB);
        }

        CoderResult UTF_8::Decoder::malformedForLength(io::ByteBuffer& src, gint mark, gint malformedNB) {
            src.position(mark);
            return CoderResult::malformedForLength(malformedNB);
        }

        CoderResult UTF_8::Decoder::xflow(io::Buffer& src, gint sp, gint sl, io::Buffer& dst, gint dp, gint nb) {
            updatePositions(src, sp, dst, dp);
            return (nb == 0 || sl - sp < nb)
                       ? CoderResult::UNDERFLOW
                       : CoderResult::OVERFLOW;
        }

        CoderResult UTF_8::Decoder::xflow(io::Buffer& src, gint mark, gint nb) {
            src.position(mark);
            return (nb == 0 || src.remaining() < nb)
                       ? CoderResult::UNDERFLOW
                       : CoderResult::OVERFLOW;
        }

        CoderResult UTF_8::Decoder::decodeArray(io::ByteBuffer& src, io::CharBuffer& dst) {
            // This method is optimized for ASCII input.
            ByteArray& sa = src.array();
            int soff = src.arrayOffset();
            int sp = soff + src.position();
            int sl = soff + src.limit();

            CharArray& da = dst.array();
            int doff = dst.arrayOffset();
            int dp = doff + dst.position();
            int dl = doff + dst.limit();

            int n = US_ASCII::decodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            while (sp < sl) {
                int b1 = sa[sp];
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    if (dp >= dl)
                        return xflow(src, sp, sl, dst, dp, 1);
                    da[dp++] = (gchar)b1;
                    sp++;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    //                   [C2..DF] [80..BF]
                    if (sl - sp < 2 || dp >= dl)
                        return xflow(src, sp, sl, dst, dp, 2);
                    int b2 = sa[sp + 1];
                    // Now we check the first byte of 2-byte sequence as
                    //     if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0)
                    // no longer need to check b1 against c1 & c0 for
                    // malformed as we did in previous version
                    //   (b1 & 0x1e) == 0x0 || (b2 & 0xc0) != 0x80;
                    // only need to check the second gbyte b2.
                    if (isNotContinuation(b2))
                        return malformedForLength(src, sp, dst, dp, 1);
                    da[dp++] = (gchar)(((b1 << 6) ^ b2) ^ (((gbyte)0xC0 << 6) ^ ((gbyte)0x80 << 0)));
                    sp += 2;
                } else if ((b1 >> 4) == -2) {
                    // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                    int srcRemaining = sl - sp;
                    if (srcRemaining < 3 || dp >= dl) {
                        if (srcRemaining > 1 && isMalformed3_2(b1, sa[sp + 1]))
                            return malformedForLength(src, sp, dst, dp, 1);
                        return xflow(src, sp, sl, dst, dp, 3);
                    }
                    int b2 = sa[sp + 1];
                    int b3 = sa[sp + 2];
                    if (isMalformed3(b1, b2, b3))
                        return malformed(src, sp, dst, dp, 3);
                    gchar c = (gchar)
                    ((b1 << 12) ^ (b2 << 6) ^ (b3 ^ (((gbyte)0xE0 << 12) ^ ((gbyte)0x80 << 6) ^ ((gbyte)0x80 << 0))));
                    if (Character::isSurrogate(c))
                        return malformedForLength(src, sp, dst, dp, 3);
                    da[dp++] = c;
                    sp += 3;
                } else if ((b1 >> 3) == -2) {
                    // 4 bytes, 21 bits: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    int srcRemaining = sl - sp;
                    if (srcRemaining < 4 || dl - dp < 2) {
                        b1 &= 0xff;
                        if (b1 > 0xf4 ||
                            srcRemaining > 1 && isMalformed4_2(b1, sa[sp + 1] & 0xff))
                            return malformedForLength(src, sp, dst, dp, 1);
                        if (srcRemaining > 2 && isMalformed4_3(sa[sp + 2]))
                            return malformedForLength(src, sp, dst, dp, 2);
                        return xflow(src, sp, sl, dst, dp, 4);
                    }
                    int b2 = sa[sp + 1];
                    int b3 = sa[sp + 2];
                    int b4 = sa[sp + 3];
                    int uc = ((b1 << 18) ^
                        (b2 << 12) ^
                        (b3 << 6) ^
                        (b4 ^
                            (((gbyte)0xF0 << 18) ^
                                ((gbyte)0x80 << 12) ^
                                ((gbyte)0x80 << 6) ^
                                ((gbyte)0x80 << 0))));
                    if (isMalformed4(b2, b3, b4) ||
                        // shortest form check
                        !Character::isSupplementary(uc)) {
                        return malformed(src, sp, dst, dp, 4);
                    }
                    da[dp++] = Character::highSurrogate(uc);
                    da[dp++] = Character::lowSurrogate(uc);
                    sp += 4;
                } else
                    return malformed(src, sp, dst, dp, 1);
            }
            return xflow(src, sp, sl, dst, dp, 0);
        }

        CoderResult UTF_8::Decoder::decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst) {
            int mark = src.position();
            int limit = src.limit();
            while (mark < limit) {
                int b1 = src.get();
                if (b1 >= 0) {
                    // 1 gbyte, 7 bits: 0xxxxxxx
                    if (dst.remaining() < 1)
                        return xflow(src, mark, 1); // overflow
                    dst.put((gchar)b1);
                    mark++;
                } else if ((b1 >> 5) == -2 && (b1 & 0x1e) != 0) {
                    // 2 bytes, 11 bits: 110xxxxx 10xxxxxx
                    if (limit - mark < 2 || dst.remaining() < 1)
                        return xflow(src, mark, 2);
                    int b2 = src.get();
                    if (isNotContinuation(b2))
                        return malformedForLength(src, mark, 1);
                    dst.put((gchar)(((b1 << 6) ^ b2)
                        ^
                        (((gbyte)0xC0 << 6) ^
                            ((gbyte)0x80 << 0))));
                    mark += 2;
                } else if ((b1 >> 4) == -2) {
                    // 3 bytes, 16 bits: 1110xxxx 10xxxxxx 10xxxxxx
                    int srcRemaining = limit - mark;
                    if (srcRemaining < 3 || dst.remaining() < 1) {
                        if (srcRemaining > 1 && isMalformed3_2(b1, src.get()))
                            return malformedForLength(src, mark, 1);
                        return xflow(src, mark, 3);
                    }
                    int b2 = src.get();
                    int b3 = src.get();
                    if (isMalformed3(b1, b2, b3))
                        return malformed(src, mark, 3);
                    gchar c = (gchar)
                    ((b1 << 12) ^
                        (b2 << 6) ^
                        (b3 ^
                            (((gbyte)0xE0 << 12) ^
                                ((gbyte)0x80 << 6) ^
                                ((gbyte)0x80 << 0))));
                    if (Character::isSurrogate(c))
                        return malformedForLength(src, mark, 3);
                    dst.put(c);
                    mark += 3;
                } else if ((b1 >> 3) == -2) {
                    // 4 bytes, 21 bits: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                    int srcRemaining = limit - mark;
                    if (srcRemaining < 4 || dst.remaining() < 2) {
                        b1 &= 0xff;
                        if (b1 > 0xf4 ||
                            srcRemaining > 1 && isMalformed4_2(b1, src.get() & 0xff))
                            return malformedForLength(src, mark, 1);
                        if (srcRemaining > 2 && isMalformed4_3(src.get()))
                            return malformedForLength(src, mark, 2);
                        return xflow(src, mark, 4);
                    }
                    int b2 = src.get();
                    int b3 = src.get();
                    int b4 = src.get();
                    int uc = ((b1 << 18) ^
                        (b2 << 12) ^
                        (b3 << 6) ^
                        (b4 ^
                            (((gbyte)0xF0 << 18) ^
                                ((gbyte)0x80 << 12) ^
                                ((gbyte)0x80 << 6) ^
                                ((gbyte)0x80 << 0))));
                    if (isMalformed4(b2, b3, b4) ||
                        // shortest form check
                        !Character::isSupplementary(uc)) {
                        return malformed(src, mark, 4);
                    }
                    dst.put(Character::highSurrogate(uc));
                    dst.put(Character::lowSurrogate(uc));
                    mark += 4;
                } else {
                    return malformed(src, mark, 1);
                }
            }
            return xflow(src, mark, 0);
        }

        UTF_8::Encoder::Encoder(Charset const& cs)
            : CharsetEncoder(cs, 1.1F, 3.0F) {
        }

        gbool UTF_8::Encoder::canEncode(gchar c) {
            return !Character::isSurrogate(c);
        }

        gbool UTF_8::Encoder::isLegalReplacement(const ByteArray& repl) const {
            return ((repl.length() == 1 && repl[0] >= 0) ||
                CharsetEncoder::isLegalReplacement(repl));
        }

        CoderResult UTF_8::Encoder::encode0(io::CharBuffer& src, io::ByteBuffer& dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArray(src, dst);
            else
                return encodeBuffer(src, dst);
        }

        CoderResult UTF_8::Encoder::overflow(io::CharBuffer& src, gint sp, io::ByteBuffer& dst, gint dp) {
            updatePositions(src, sp, dst, dp);
            return CoderResult::OVERFLOW;
        }

        CoderResult UTF_8::Encoder::overflow(io::CharBuffer& src, gint mark) {
            src.position(mark);
            return CoderResult::OVERFLOW;
        }

        CoderResult UTF_8::Encoder::encodeArray(io::CharBuffer& src, io::ByteBuffer& dst) {
            CharArray& sa = src.array();
            int sp = src.arrayOffset() + src.position();
            int sl = src.arrayOffset() + src.limit();

            ByteArray& da = dst.array();
            int dp = dst.arrayOffset() + dst.position();
            int dl = dst.arrayOffset() + dst.limit();

            // Handle ASCII-only prefix
            int n = US_ASCII::encodeASCII(sa, sp, da, dp, Math::min(sl - sp, dl - dp));
            sp += n;
            dp += n;

            if (sp < sl) {
                return encodeArraySlow(src, sa, sp, sl, dst, da, dp, dl);
            } else {
                updatePositions(src, sp, dst, dp);
                return CoderResult::UNDERFLOW;
            }
        }

        CoderResult UTF_8::Encoder::encodeArraySlow(io::CharBuffer& src, CharArray& sa, gint sp, gint sl,
            io::ByteBuffer& dst, ByteArray& da, gint dp, gint dl) {
            while (sp < sl) {
                gchar c = sa[sp];
                if (c < 0x80) {
                    // Have at most seven bits
                    if (dp >= dl)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte)c;
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    if (dl - dp < 2)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte)(0xc0 | (c >> 6));
                    da[dp++] = (gbyte)(0x80 | (c & 0x3f));
                } else if (Character::isSurrogate(c)) {
                    // Have a surrogate pair
                    int uc = sgp.parse(c, sa, sp, sl);
                    if (uc < 0) {
                        updatePositions(src, sp, dst, dp);
                        return sgp.error();
                    }
                    if (dl - dp < 4)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte)(0xf0 | ((uc >> 18)));
                    da[dp++] = (gbyte)(0x80 | ((uc >> 12) & 0x3f));
                    da[dp++] = (gbyte)(0x80 | ((uc >>  6) & 0x3f));
                    da[dp++] = (gbyte)(0x80 | (uc & 0x3f));
                    sp++;  // 2 chars
                } else {
                    // 3 bytes, 16 bits
                    if (dl - dp < 3)
                        return overflow(src, sp, dst, dp);
                    da[dp++] = (gbyte)(0xe0 | ((c >> 12)));
                    da[dp++] = (gbyte)(0x80 | ((c >>  6) & 0x3f));
                    da[dp++] = (gbyte)(0x80 | (c & 0x3f));
                }
                sp++;
            }
            updatePositions(src, sp, dst, dp);
            return CoderResult::UNDERFLOW;
        }

        CoderResult UTF_8::Encoder::encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst) {
            int mark = src.position();
            while (src.hasRemaining()) {
                gchar c = src.get();
                if (c < 0x80) {
                    // Have at most seven bits
                    if (!dst.hasRemaining())
                        return overflow(src, mark);
                    dst.put((gbyte)c);
                } else if (c < 0x800) {
                    // 2 bytes, 11 bits
                    if (dst.remaining() < 2)
                        return overflow(src, mark);
                    dst.put((gbyte)(0xc0 | (c >> 6)));
                    dst.put((gbyte)(0x80 | (c & 0x3f)));
                } else if (Character::isSurrogate(c)) {
                    // Have a surrogate pair
                    int uc = sgp.parse(c, src);
                    if (uc < 0) {
                        src.position(mark);
                        return sgp.error();
                    }
                    if (dst.remaining() < 4)
                        return overflow(src, mark);
                    dst.put((gbyte)(0xf0 | ((uc >> 18))));
                    dst.put((gbyte)(0x80 | ((uc >> 12) & 0x3f)));
                    dst.put((gbyte)(0x80 | ((uc >>  6) & 0x3f)));
                    dst.put((gbyte)(0x80 | (uc & 0x3f)));
                    mark++;  // 2 chars
                } else {
                    // 3 bytes, 16 bits
                    if (dst.remaining() < 3)
                        return overflow(src, mark);
                    dst.put((gbyte)(0xe0 | ((c >> 12))));
                    dst.put((gbyte)(0x80 | ((c >>  6) & 0x3f)));
                    dst.put((gbyte)(0x80 | (c & 0x3f)));
                }
                mark++;
            }
            src.position(mark);
            return CoderResult::UNDERFLOW;
        }
    } // charset
} // core
