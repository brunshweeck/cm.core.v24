//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/io/ByteBuffer.h>
#include <core/util/Arrays.h>
#include <meta/charset/HKSCS.h>
#include <meta/charset/StringUTF16.h>

namespace core {
    namespace charset {

        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        HKSCS::Decoder::Decoder(Charset const& cs,
                                DoubleByte::Decoder& big5Dec,
                                CharArray2D& b2cBmp,
                                CharArray2D& b2cSupp)
            : CharsetDecoder(cs, 0.5F, 1.0F),
            DoubleByte::Decoder(cs, 0.5F, 1.0F, b2c, b2cSB, 0, 0, true),
              b2cBmp(b2cBmp), b2cSupp(b2cSupp), big5Dec(big5Dec) {
        }

        gchar HKSCS::Decoder::decodeSingle(gint b) const { return big5Dec.decodeSingle(b); }

        gchar HKSCS::Decoder::decodeBig5(gint b1, gint b2) const { return big5Dec.decodeDouble(b1, b2); }

        gchar HKSCS::Decoder::decodeDouble(gint b1, gint b2) const { return b2cBmp[b1][b2 - b2Min]; }

        gchar HKSCS::Decoder::decodeDoubleEx(gint b1, gint b2) const {
            /* if the b2cSupp is null, the subclass need
               to override the method
            if (b2cSupp == null)
                return UNMAPPABLE_DECODING;
             */
            return b2cSupp[b1][b2 - b2Min];
        }

        gint HKSCS::Decoder::decode(const ByteArray& src, gint sp, gint len, CharArray& dst) {
            gint dp = 0;
            gint sl = sp + len;
            gchar repl = replacement().charAt(0);
            while (sp < sl) {
                gint b1 = src[sp++] & 0xff;
                gchar c = decodeSingle(b1);
                if (c == UNMAPPABLE_DECODING) {
                    if (sl == sp) {
                        c = repl;
                    } else {
                        gint b2 = src[sp++] & 0xff;
                        if (b2 < b2Min || b2 > b2Max) {
                            c = repl;
                        } else if ((c = decodeDouble(b1, b2)) == UNMAPPABLE_DECODING) {
                            c = decodeDoubleEx(b1, b2); //supp
                            if (c == UNMAPPABLE_DECODING) {
                                c = decodeBig5(b1, b2); //big5
                                if (c == UNMAPPABLE_DECODING)
                                    c = repl;
                            } else {
                                // supplementary character in u+2xxxx area
                                dst[dp++] = Surrogate::high(0x20000 + c);
                                dst[dp++] = Surrogate::low(0x20000 + c);
                                continue;
                            }
                        }
                    }
                }
                dst[dp++] = c;
            }
            return dp;
        }

        CoderResult HKSCS::Decoder::decode0(io::ByteBuffer& src, io::CharBuffer& dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArray(src, dst);

            return decodeBuffer(src, dst);
        }

        void HKSCS::Decoder::initB2C(CharArray2D& b2c, StringArray& b2cStr) {
            for (gint i = 0; i < b2cStr.length(); i++) {
                if (b2cStr[i].isEmpty())
                    b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                else
                    b2c[i] = b2cStr[i].toChars();
            }
        }

        CoderResult HKSCS::Decoder::decodeArray(io::ByteBuffer& src, io::CharBuffer& dst) {
            ByteArray& sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            CharArray& da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (sp < sl) {
                    gint b1 = sa[sp] & 0xff;
                    gchar c = decodeSingle(b1);
                    gint inSize = 1, outSize = 1;
                    if (c == UNMAPPABLE_DECODING) {
                        if (sl - sp < 2) {
                            cr = CoderResult::UNDERFLOW;
                            goto FINALLY;
                        }
                        gint b2 = sa[sp + 1] & 0xff;
                        inSize++;
                        if (b2 < b2Min || b2 > b2Max) {
                            cr = CoderResult::unmappableForLength(2);
                            goto FINALLY;
                        }
                        c = decodeDouble(b1, b2); //bmp
                        if (c == UNMAPPABLE_DECODING) {
                            c = decodeDoubleEx(b1, b2); //supp
                            if (c == UNMAPPABLE_DECODING) {
                                c = decodeBig5(b1, b2); //big5
                                if (c == UNMAPPABLE_DECODING) {
                                    cr = CoderResult::unmappableForLength(2);
                                    goto FINALLY;
                                }
                            } else {
                                // supplementary character in u+2xxxx area
                                outSize = 2;
                            }
                        }
                    }
                    if (dl - dp < outSize) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    if (outSize == 2) {
                        // supplementary characters
                        da[dp++] = Surrogate::high(0x20000 + c);
                        da[dp++] = Surrogate::low(0x20000 + c);
                    } else {
                        da[dp++] = c;
                    }
                    sp += inSize;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }
        FINALLY: {
                src.position(sp - src.arrayOffset());
                dst.position(dp - dst.arrayOffset());

                if (catchEx)
                    catchEx->throws($ftrace());

                return cr;
            }
        }

        CoderResult HKSCS::Decoder::decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst) {
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gint b1 = src.get() & 0xff;
                    gint inSize = 1, outSize = 1;
                    gchar c = decodeSingle(b1);
                    if (c == UNMAPPABLE_DECODING) {
                        if (src.remaining() < 1) {
                            cr = CoderResult::UNDERFLOW;
                            goto FINALLY;
                        }
                        gint b2 = src.get() & 0xff;
                        inSize++;
                        if (b2 < b2Min || b2 > b2Max) {
                            cr = CoderResult::unmappableForLength(2);
                            goto FINALLY;
                        }
                        c = decodeDouble(b1, b2); //bmp
                        if (c == UNMAPPABLE_DECODING) {
                            c = decodeDoubleEx(b1, b2); //supp
                            if (c == UNMAPPABLE_DECODING) {
                                c = decodeBig5(b1, b2); //big5
                                if (c == UNMAPPABLE_DECODING) {
                                    cr = CoderResult::unmappableForLength(2);
                                    goto FINALLY;
                                }
                            } else {
                                outSize = 2;
                            }
                        }
                    }
                    if (dst.remaining() < outSize) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    if (outSize == 2) {
                        dst.put(Surrogate::high(0x20000 + c));
                        dst.put(Surrogate::low(0x20000 + c));
                    } else {
                        dst.put(c);
                    }
                    mark += inSize;
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

        HKSCS::Encoder::Encoder(Charset const& cs, DoubleByte::Encoder& big5Enc, CharArray2D& c2bBmp,
                                CharArray2D& c2bSupp)
            : CharsetEncoder(cs, 2.0F, 2.0F),
              DoubleByte::Encoder(cs, c2b, c2bIndex, true), big5Enc(big5Enc), c2bBmp(c2bBmp), c2bSupp(c2bSupp) {

        }

        gint HKSCS::Encoder::encodeBig5(gchar ch) const { return big5Enc.encodeChar(ch); }

        gint HKSCS::Encoder::encodeChar(gchar ch) {
            gint bb = c2bBmp[ch >> 8][ch & 0xff];
            if (bb == UNMAPPABLE_ENCODING)
                return encodeBig5(ch);
            return bb;
        }

        gint HKSCS::Encoder::encodeSupp(gint cp) const {
            if ((cp & 0xf0000) != 0x20000)
                return UNMAPPABLE_ENCODING;
            return c2bSupp[(cp >> 8) & 0xff][cp & 0xff];
        }

        gbool HKSCS::Encoder::canEncode(gchar c) {
            return encodeChar(c) != UNMAPPABLE_ENCODING;
        }

        gint HKSCS::Encoder::encode(const CharArray& src, gint sp, gint len, ByteArray& dst) {
            gint dp = 0;
            gint sl = sp + len;
            while (sp < sl) {
                gchar c = src[sp++];
                gint bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    if (!Character::isHighSurrogate(c) || sp == sl ||
                        !Character::isLowSurrogate(src[sp]) ||
                        (bb = encodeSupp(Character::toCodePoint(c, src[sp++])))
                        == UNMAPPABLE_ENCODING) {
                        dst[dp++] = repl[0];
                        if (repl.length() > 1)
                            dst[dp++] = repl[1];
                        continue;
                    }
                }
                if (bb > MAX_SINGLEBYTE) {
                    // DoubleByte
                    dst[dp++] = CORE_CAST(gbyte, bb >> 8);
                    dst[dp++] = CORE_CAST(gbyte, bb);
                } else {
                    // SingleByte
                    dst[dp++] = CORE_CAST(gbyte, bb);
                }
            }
            return dp;
        }

        gint HKSCS::Encoder::encodeFromUTF16(const ByteArray& src, gint sp, gint len, ByteArray& dst) {
            gint dp = 0;
            gint sl = sp + len;
            gint dl = dst.length();
            while (sp < sl) {
                gchar c = StringUTF16::getChar(src, sp++);
                gint bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    if (!Character::isHighSurrogate(c) || sp == sl ||
                        !Character::isLowSurrogate(StringUTF16::getChar(src, sp)) ||
                        (bb = encodeSupp(Character::toCodePoint(c, StringUTF16::getChar(src, sp++))))
                        == UNMAPPABLE_ENCODING) {
                        dst[dp++] = repl[0];
                        if (repl.length() > 1)
                            dst[dp++] = repl[1];
                        continue;
                    }
                }
                if (bb > MAX_SINGLEBYTE) {
                    // DoubleByte
                    dst[dp++] = CORE_CAST(gbyte, bb >> 8);
                    dst[dp++] = CORE_CAST(gbyte, bb);
                } else {
                    // SingleByte
                    dst[dp++] = CORE_CAST(gbyte, bb);
                }
            }
            return dp;
        }

        void HKSCS::Encoder::initC2B(CharArray2D& c2b, StringArray &b2cStr, String& pua) {
            // init c2b/c2bSupp from b2cStr and supp
            gint b2Min = 0x40;
            for (gint i = 0; i < c2b.length(); ++i) c2b[i] = C2B_UNMAPPABLE;

            for (gint b1 = 0; b1 < 0x100; b1++) {
                String const& s = b2cStr[b1];
                if (s.isEmpty())
                    continue;
                for (gint i = 0; i < s.length(); i++) {
                    gchar c = s.charAt(i);
                    if (c == UNMAPPABLE_DECODING)
                        continue;
                    gint hi = c >> 8;
                    if (c2b[hi] == C2B_UNMAPPABLE) {
                        c2b[hi] = CharArray(0x100, UNMAPPABLE_ENCODING);
                    }
                    c2b[hi][c & 0xff] = CORE_CAST(gchar, (b1 << 8) | (i + b2Min));
                }
            }
            if (!pua.isEmpty()) {
                // add the compatibility pua entries
                gchar c = u'\ue000'; //first pua character
                for (gint i = 0; i < pua.length(); i++) {
                    gchar bb = pua.charAt(i);
                    if (bb != UNMAPPABLE_DECODING) {
                        gint hi = c >> 8;
                        if (c2b[hi] == C2B_UNMAPPABLE) {
                            c2b[hi] = CharArray(0x100, UNMAPPABLE_ENCODING);
                        }
                        c2b[hi][c & 0xff] = bb;
                    }
                    c++;
                }
            }
        }

        CoderResult HKSCS::Encoder::encodeArray(io::CharBuffer& src, io::ByteBuffer& dst) {
            CharArray& sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            ByteArray& da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (sp < sl) {
                    gchar c = sa[sp];
                    gint inSize = 1;
                    gint bb = encodeChar(c);
                    if (bb == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            gint cp;
                            if ((cp = sgp.parse(c, sa, sp, sl)) < 0) {
                                cr = sgp.error();
                                goto FINALLY;
                            }
                            bb = encodeSupp(cp);
                            if (bb == UNMAPPABLE_ENCODING) {
                                cr = CoderResult::unmappableForLength(2);
                                goto FINALLY;
                            }
                            inSize = 2;
                        } else {
                            cr = CoderResult::unmappableForLength(1);
                            goto FINALLY;
                        }
                    }
                    if (bb > MAX_SINGLEBYTE) {
                        // DoubleByte
                        if (dl - dp < 2) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        da[dp++] = CORE_CAST(gbyte, bb >> 8);
                        da[dp++] = CORE_CAST(gbyte, bb);
                    } else {
                        // SingleByte
                        if (dl - dp < 1) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        da[dp++] = CORE_CAST(gbyte, bb);
                    }
                    sp += inSize;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }
        FINALLY: {
                src.position(sp - src.arrayOffset());
                dst.position(dp - dst.arrayOffset());

                if (catchEx)
                    catchEx->throws($ftrace());

                return cr;
            }
        }

        CoderResult HKSCS::Encoder::encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst) {
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gint inSize = 1;
                    gchar c = src.get();
                    gint bb = encodeChar(c);
                    if (bb == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            gint cp;
                            if ((cp = sgp.parse(c, src)) < 0) {
                                cr = sgp.error();
                            }
                            bb = encodeSupp(cp);
                            if (bb == UNMAPPABLE_ENCODING) {
                                cr = CoderResult::unmappableForLength(2);
                                goto FINALLY;
                            }
                            inSize = 2;
                        } else {
                            cr = CoderResult::unmappableForLength(1);
                            goto FINALLY;
                        }
                    }
                    if (bb > MAX_SINGLEBYTE) {
                        // DoubleByte
                        if (dst.remaining() < 2) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        dst.put(CORE_CAST(gbyte, bb >> 8));
                        dst.put(CORE_CAST(gbyte, bb));
                    } else {
                        if (dst.remaining() < 1) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        dst.put(CORE_CAST(gbyte, bb));
                    }
                    mark += inSize;
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

        CoderResult HKSCS::Encoder::encode0(io::CharBuffer& src, io::ByteBuffer& dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArray(src, dst);

            return encodeBuffer(src, dst);
        }

        void HKSCS::Encoder::replaceWith0(const ByteArray& newReplacement) { repl = newReplacement; }

        CharArray HKSCS::Encoder::C2B_UNMAPPABLE = CharArray(0x100, UNMAPPABLE_ENCODING);

        CORE_WARNING_POP
    } // charset
} // core
