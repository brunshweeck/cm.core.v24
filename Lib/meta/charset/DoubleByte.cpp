//
// Created by brunshweeck on 6 sept. 2024.
//

#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <core/util/Arrays.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/StringUTF16.h>
#include <meta/charset/US_ASCII.h>

namespace core {
    namespace charset {

        CharArray DoubleByte::B2C_UNMAPPABLE = CharArray(0x100, UNMAPPABLE_DECODING);
         CharArray DoubleByte::DecoderDBCS_ONLY::B2CSB_UNMAPPABLE = CharArray(0x100, UNMAPPABLE_DECODING);

        DoubleByte::Decoder::Decoder(Charset const& cs, gfloat averageCharsPerByte, gfloat maxCharsPerByte,
                                     CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max,
                                     gbool isAsciiCompatible)
            : CharsetDecoder(cs, averageCharsPerByte, maxCharsPerByte),
              b2c(b2c), b2cSB(b2cSB), b2Min(b2Min), b2Max(b2Max), asciiCompatible(isAsciiCompatible) {
        }

        DoubleByte::Decoder::Decoder(Charset const& cs, CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max,
                                     gbool isAsciiCompatible)
            : Decoder(cs, 0.5F, 1.0F, b2c, b2cSB, b2Min, b2Max, isAsciiCompatible) {
        }

        DoubleByte::Decoder::Decoder(Charset const& cs, CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max)
            : Decoder(cs, 0.5F, 1.0F, b2c, b2cSB, b2Min, b2Max, false) {
        }

        CoderResult DoubleByte::Decoder::flush0(io::CharBuffer& out) { return CharsetDecoder::flush0(out); }

        void DoubleByte::Decoder::reset0() { CharsetDecoder::reset0(); }

        CoderResult DoubleByte::Decoder::decode0(io::ByteBuffer& src, io::CharBuffer& dst) {
            if (src.hasArray() && dst.hasArray())
                return decodeArray(src, dst);

            return decodeBuffer(src, dst);
        }

        gint DoubleByte::Decoder::decode(ByteArray const& src, gint sp, gint len, CharArray& dst) {
            gint dp = 0;
            gint sl = sp + len;
            gchar repl = replacement().charAt(0);
            while (sp < sl) {
                gint b1 = src[sp++] & 0xff;
                gchar c = b2cSB[b1];
                if (c == UNMAPPABLE_DECODING) {
                    if (sp < sl) {
                        gint b2 = src[sp++] & 0xff;
                        if (b2 < b2Min || b2 > b2Max ||
                            (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                            if (crMalformedOrUnmappable(b1, b2).length() == 1) {
                                sp--;
                            }
                        }
                    }
                    if (c == UNMAPPABLE_DECODING) {
                        c = repl;
                    }
                }
                dst[dp++] = c;
            }
            return dp;
        }

        gbool DoubleByte::Decoder::isASCIICompatible() const { return asciiCompatible; }

        gchar DoubleByte::Decoder::decodeSingle(gint b) const { return b2cSB[b]; }

        gchar DoubleByte::Decoder::decodeDouble(gint b1, gint b2) const {
            if (b1 < 0 || b1 > b2c.length() ||
                b2 < b2Min || b2 > b2Max)
                return UNMAPPABLE_DECODING;
            return b2c[b1][b2 - b2Min];
        }

        CoderResult DoubleByte::Decoder::crMalformedOrUnderFlow(gint b) { return CoderResult::UNDERFLOW; }

        CoderResult DoubleByte::Decoder::crMalformedOrUnmappable(gint b1, gint b2) {
            if (b2c[b1] == B2C_UNMAPPABLE || // isNotLeadingByte(b1)
                b2c[b2] != B2C_UNMAPPABLE || // isLeadingByte(b2)
                decodeSingle(b2) != UNMAPPABLE_DECODING) {
                // isSingle(b2)
                return CoderResult::malformedForLength(1);
            }
            return CoderResult::unmappableForLength(2);
        }

        CoderResult DoubleByte::Decoder::decodeArray(io::ByteBuffer& src, io::CharBuffer& dst) {
            ByteArray& sa = src.array();
            gint soff = src.arrayOffset();
            gint sp = soff + src.position();
            gint sl = soff + src.limit();

            CharArray& da = dst.array();
            gint doff = dst.arrayOffset();
            gint dp = doff + dst.position();
            gint dl = doff + dst.limit();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                if (asciiCompatible) {
                    gint n = US_ASCII::decodeASCII(sa, sp, da, dp, Math::min(dl - dp, sl - sp));
                    dp += n;
                    sp += n;
                }
                while (sp < sl && dp < dl) {
                    // inline the decodeSingle/Double() for better performance
                    gint inSize = 1;
                    gint b1 = sa[sp] & 0xff;
                    gchar c = b2cSB[b1];
                    if (c == UNMAPPABLE_DECODING) {
                        if (sl - sp < 2) {
                            cr = crMalformedOrUnderFlow(b1);
                            goto FINALLY;
                        }
                        gint b2 = sa[sp + 1] & 0xff;
                        if (b2 < b2Min || b2 > b2Max ||
                            (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                            cr = crMalformedOrUnmappable(b1, b2);
                            goto FINALLY;
                        }
                        inSize++;
                    }
                    da[dp++] = c;
                    sp += inSize;
                }
                cr = (sp >= sl)
                         ? CoderResult::UNDERFLOW
                         : CoderResult::OVERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }

        FINALLY:
            src.position(sp - soff);
            dst.position(dp - doff);

            if (catchEx)
                catchEx->throws($ftrace());

            return cr;
        }

        CoderResult DoubleByte::Decoder::decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst) {
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (src.hasRemaining() && dst.hasRemaining()) {
                    gint b1 = src.get() & 0xff;
                    gchar c = b2cSB[b1];
                    gint inSize = 1;
                    if (c == UNMAPPABLE_DECODING) {
                        if (src.remaining() < 1) {
                            cr = crMalformedOrUnderFlow(b1);
                            goto FINALLY;
                        }
                        gint b2 = src.get() & 0xff;
                        if (b2 < b2Min || b2 > b2Max ||
                            (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                            cr = crMalformedOrUnmappable(b1, b2);
                            goto FINALLY;
                        }
                        inSize++;
                    }
                    dst.put(c);
                    mark += inSize;
                }
                cr = src.hasRemaining()
                         ? CoderResult::OVERFLOW
                         : CoderResult::UNDERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }

        FINALLY:
            src.position(mark);

            if (catchEx)
                catchEx->throws($ftrace());

            return cr;
        }

         DoubleByte::DecoderEBCDIC::DecoderEBCDIC(Charset const& cs,
                                                  CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max,
                                                  gbool isAsciiCompatible)
             : CharsetDecoder(cs, 0.5F, 1.0F),
             Decoder(cs, b2c, b2cSB, b2Min, b2Max, isAsciiCompatible) {
         }

         DoubleByte::DecoderEBCDIC::DecoderEBCDIC(Charset const& cs,
                                                  CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max)
             : CharsetDecoder(cs, 0.5F, 1.0F),
             Decoder(cs, b2c, b2cSB, b2Min, b2Max) {
         }

         void DoubleByte::DecoderEBCDIC::reset0() {
             state = SBCS;
         }

         gint DoubleByte::DecoderEBCDIC::decode(ByteArray const& src, gint sp, gint len, CharArray& dst) {
             gint dp = 0;
             gint sl = sp + len;
             state = SBCS;
             gchar repl = replacement().charAt(0);
             while (sp < sl) {
                 gint b1 = src[sp++] & 0xff;
                 if (b1 == SO) {
                     // Shift out
                     if (state != SBCS)
                         dst[dp++] = repl;
                     else
                         state = DBCS;
                 } else if (b1 == SI) {
                     if (state != DBCS)
                         dst[dp++] = repl;
                     else
                         state = SBCS;
                 } else {
                     gchar c = UNMAPPABLE_DECODING;
                     if (state == SBCS) {
                         c = b2cSB[b1];
                         if (c == UNMAPPABLE_DECODING)
                             c = repl;
                     } else {
                         if (sl == sp) {
                             c = repl;
                         } else {
                             gint b2 = src[sp++] & 0xff;
                             if (b2 < b2Min || b2 > b2Max ||
                                 (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                                 c = repl;
                             }
                         }
                     }
                     dst[dp++] = c;
                 }
             }
             return dp;
         }

         CoderResult DoubleByte::DecoderEBCDIC::decodeArray(io::ByteBuffer& src, io::CharBuffer& dst) {
             ByteArray& sa = src.array();
             gint sp = src.arrayOffset() + src.position();
             gint sl = src.arrayOffset() + src.limit();
             CharArray& da = dst.array();
             gint dp = dst.arrayOffset() + dst.position();
             gint dl = dst.arrayOffset() + dst.limit();

             CoderResult cr = CoderResult::UNDEFINED;
             Throwable const* catchEx = null;

             try {
                 // don't check dp/dl together here, it's possible to
                 // decode a SO/SI without space in output buffer.
                 while (sp < sl) {
                     gint b1 = sa[sp] & 0xff;
                     gint inSize = 1;
                     if (b1 == SO) {
                         // Shift out
                         if (state != SBCS) {
                             cr = CoderResult::malformedForLength(1);
                             goto FINALLY;
                         }
                         state = DBCS;
                     } else if (b1 == SI) {
                         if (state != DBCS) {
                             cr = CoderResult::malformedForLength(1);
                             goto FINALLY;
                         }
                         state = SBCS;
                     } else {
                         gchar c;
                         if (state == SBCS) {
                             c = b2cSB[b1];
                             if (c == UNMAPPABLE_DECODING) {
                                 cr = CoderResult::unmappableForLength(1);
                                 goto FINALLY;
                             }
                         } else {
                             if (sl - sp < 2) {
                                 cr = CoderResult::UNDERFLOW;
                                 goto FINALLY;
                             }
                             gint b2 = sa[sp + 1] & 0xff;
                             if (b2 < b2Min || b2 > b2Max ||
                                 (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                                 if (!isDoubleByte(b1, b2))
                                     cr = CoderResult::malformedForLength(2);
                                 else
                                     cr = CoderResult::unmappableForLength(2);
                                 goto FINALLY;
                             }
                             inSize++;
                         }
                         if (dl - dp < 1) {
                             cr = CoderResult::OVERFLOW;
                             goto FINALLY;
                         }

                         da[dp++] = c;
                     }
                     sp += inSize;
                 }
                 cr = CoderResult::UNDERFLOW;
             } catch (Throwable const& ex) { catchEx = &ex; }
         FINALLY:
             src.position(sp - src.arrayOffset());
             dst.position(dp - dst.arrayOffset());

             if (catchEx)
                 catchEx->throws($ftrace());

             return cr;
         }

         CoderResult DoubleByte::DecoderEBCDIC::decodeBuffer(io::ByteBuffer& src, io::CharBuffer& dst) {
             gint mark = src.position();

             CoderResult cr = CoderResult::UNDEFINED;
             Throwable const* catchEx = null;

             try {
                 while (src.hasRemaining()) {
                     gint b1 = src.get() & 0xff;
                     gint inSize = 1;
                     if (b1 == SO) {
                         // Shift out
                         if (state != SBCS) {
                             cr = CoderResult::malformedForLength(1);
                             goto FINALLY;
                         }
                         state = DBCS;
                     } else if (b1 == SI) {
                         if (state != DBCS) {
                             cr = CoderResult::malformedForLength(1);
                             goto FINALLY;
                         }
                         state = SBCS;
                     } else {
                         gchar c = UNMAPPABLE_DECODING;
                         if (state == SBCS) {
                             c = b2cSB[b1];
                             if (c == UNMAPPABLE_DECODING) {
                                 cr = CoderResult::unmappableForLength(1);
                                 goto FINALLY;
                             }
                         } else {
                             if (src.remaining() < 1) {
                                 cr = CoderResult::UNDERFLOW;
                                 goto FINALLY;
                             }
                             gint b2 = src.get() & 0xff;
                             if (b2 < b2Min || b2 > b2Max ||
                                 (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                                 if (!isDoubleByte(b1, b2))
                                     cr = CoderResult::malformedForLength(2);
                                 else
                                     cr = CoderResult::unmappableForLength(2);
                                 goto FINALLY;
                             }
                             inSize++;
                         }

                         if (dst.remaining() < 1) {
                             cr = CoderResult::OVERFLOW;
                             goto FINALLY;
                         }

                         dst.put(c);
                     }
                     mark += inSize;
                 }
                 cr = CoderResult::UNDERFLOW;
             } catch (Throwable const& ex) { catchEx = &ex; }
         FINALLY:
             src.position(mark);

             if (catchEx)
                 catchEx->throws($ftrace());

             return cr;
         }

         gbool DoubleByte::DecoderEBCDIC::isDoubleByte(gint b1, gint b2) {
             return (0x41 <= b1 && b1 <= 0xfe && 0x41 <= b2 && b2 <= 0xfe)
                 || (b1 == 0x40 && b2 == 0x40); // DBCS-HOST SPACE
         }

         DoubleByte::DecoderDBCS_ONLY::DecoderDBCS_ONLY(Charset const& cs,
                                                        CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max,
                                                        gbool isAsciiCompatible)
             :CharsetDecoder(cs, 0.5F, 1.0F),
             Decoder(cs, 0.5F, 1.0F, b2c, b2cSB, b2Min, b2Max, isAsciiCompatible) {
         }

         DoubleByte::DecoderDBCS_ONLY::DecoderDBCS_ONLY(Charset const& cs,
                                                        CharArray2D& b2c, CharArray& b2cSB, gint b2Min, gint b2Max)
             :CharsetDecoder(cs, 0.5F, 1.0F),
             Decoder(cs, 0.5F, 1.0F, b2c, b2cSB, b2Min, b2Max, false) {
         }

         CoderResult DoubleByte::DecoderDBCS_ONLY::crMalformedOrUnmappable(gint b1, gint b2) {
             return CoderResult::unmappableForLength(2);
         }

         DoubleByte::DecoderEUC_SIM::DecoderEUC_SIM(Charset const& cs, CharArray2D& b2c, CharArray& b2cSB, gint b2Min,
                                                    gint b2Max, gbool isAsciiCompatible)
            : CharsetDecoder(cs, 0.5F, 1.0F),
            Decoder(cs, b2c, b2cSB, b2Min,b2Max, isAsciiCompatible) {
         }

         DoubleByte::DecoderEUC_SIM::DecoderEUC_SIM(Charset const& cs, CharArray2D& b2c, CharArray& b2cSB, gint b2Min,
                                                    gint b2Max)
            : CharsetDecoder(cs, 0.5F, 1.0F),
            Decoder(cs, b2c, b2cSB, b2Min, b2Max) {
         }

         gint DoubleByte::DecoderEUC_SIM::decode(ByteArray const& src, gint sp, gint len, CharArray& dst) {
             gint dp = 0;
             gint sl = sp + len;
             gchar repl = replacement().charAt(0);
             while (sp < sl) {
                 gint b1 = src[sp++] & 0xff;
                 gchar c = b2cSB[b1];
                 if (c == UNMAPPABLE_DECODING) {
                     if (sp < sl) {
                         gint b2 = src[sp++] & 0xff;
                         if (b2 < b2Min || b2 > b2Max ||
                             (c = b2c[b1][b2 - b2Min]) == UNMAPPABLE_DECODING) {
                             if (b1 == SS2 || b1 == SS3) {
                                 sp--;
                             }
                             c = repl;
                         }
                     } else {
                         c = repl;
                     }
                 }
                 dst[dp++] = c;
             }
             return dp;
         }

         CoderResult DoubleByte::DecoderEUC_SIM::crMalformedOrUnderFlow(gint b) {
             if (b == SS2 || b == SS3)
                 return CoderResult::malformedForLength(1);
             return CoderResult::UNDERFLOW;
         }

         CoderResult DoubleByte::DecoderEUC_SIM::crMalformedOrUnmappable(gint b1, gint b2) {
             if (b1 == SS2 || b1 == SS3)
                 return CoderResult::malformedForLength(1);
             return CoderResult::unmappableForLength(2);
         }

        DoubleByte::Encoder::Encoder(Charset const& cs, CharArray& c2b, CharArray& c2bIndex)
            : Encoder(cs, c2b, c2bIndex, false) {
        }

        DoubleByte::Encoder::Encoder(Charset const& cs, CharArray& c2b, CharArray& c2bIndex, gbool isAsciiCompatible)
            : CharsetEncoder(cs, 2.0F, 2.0F),
              c2b(c2b), c2bIndex(c2bIndex), asciiCompatible(isAsciiCompatible) {
        }

        DoubleByte::Encoder::Encoder(Charset const& cs,
                                     gfloat averageBytesPerChar, gfloat maxBytesPerChar, ByteArray const& repl,
                                     CharArray& c2b, CharArray& c2bIndex, gbool isAsciiCompatible)
            : CharsetEncoder(cs, averageBytesPerChar, maxBytesPerChar, repl),
              c2b(c2b),
              c2bIndex(c2bIndex),
              asciiCompatible(isAsciiCompatible) {
        }

        gbool DoubleByte::Encoder::canEncode(gchar c) {
            return encodeChar(c) != UNMAPPABLE_ENCODING;
        }

        gint DoubleByte::Encoder::encode(CharArray const& src, gint sp, gint len, ByteArray& dst) {
            gint dp = 0;
            gint sl = sp + len;
            if (asciiCompatible) {
                gint n = US_ASCII::encodeASCII(src, sp, dst, dp, len);
                sp += n;
                dp += n;
            }
            while (sp < sl) {
                gchar c = src[sp++];
                gint bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    if (Character::isHighSurrogate(c) && sp < sl &&
                        Character::isLowSurrogate(src[sp])) {
                        sp++;
                    }
                    dst[dp++] = repl[0];
                    if (repl.length() > 1)
                        dst[dp++] = repl[1];
                    continue;
                } //else
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

        gint DoubleByte::Encoder::encodeFromLatin1(ByteArray const& src, gint sp, gint len, ByteArray& dst) {
            gint dp = 0;
            gint sl = sp + len;
            while (sp < sl) {
                gchar c = CORE_CAST(gchar, src[sp++] & 0xff);
                gint bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    // no surrogate pair in latin1 string
                    dst[dp++] = repl[0];
                    if (repl.length() > 1) {
                        dst[dp++] = repl[1];
                    }
                    continue;
                } //else
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

        gint DoubleByte::Encoder::encodeFromUTF16(ByteArray const& src, gint sp, gint len, ByteArray& dst) {
            gint dp = 0;
            gint sl = sp + len;
            while (sp < sl) {
                gchar c = StringUTF16::getChar(src, sp++);
                gint bb = encodeChar(c);
                if (bb == UNMAPPABLE_ENCODING) {
                    if (Character::isHighSurrogate(c) && sp < sl &&
                        Character::isLowSurrogate(StringUTF16::getChar(src, sp))) {
                        sp++;
                    }
                    dst[dp++] = repl[0];
                    if (repl.length() > 1) {
                        dst[dp++] = repl[1];
                    }
                    continue;
                } //else
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

        gbool DoubleByte::Encoder::isASCIICompatible() const { return asciiCompatible; }

        gint DoubleByte::Encoder::encodeChar(gchar ch) {
            return c2b[c2bIndex[ch >> 8] + (ch & 0xff)];
        }

        void DoubleByte::Encoder::initC2B(StringArray const& b2c, String const& b2cSB, String const& b2cNR,
                                          String const& c2bNR, gint b2Min, gint b2Max, CharArray& c2b,
                                          CharArray& c2bIndex) {
            util::Arrays::fill(c2b, CORE_CAST(gchar, UNMAPPABLE_ENCODING));
            gint off = 0x100;

            CharArray2D b2c_ca = CharArray2D(b2c.length());
            CharArray b2cSB_ca = {};
            if (!b2cSB.isEmpty())
                b2cSB_ca = b2cSB.toChars();

            for (gint i = 0; i < b2c.length(); i++) {
                if (b2c[i] == null)
                    continue;
                b2c_ca[i] = b2c[i].toChars();
            }

            if (!b2cNR.isEmpty()) {
                gint j = 0;
                while (j < b2cNR.length()) {
                    gchar b = b2cNR.charAt(j++);
                    gchar c = b2cNR.charAt(j++);
                    if (b < 0x100 && !b2cSB_ca.isEmpty()) {
                        if (b2cSB_ca[b] == c)
                            b2cSB_ca[b] = UNMAPPABLE_DECODING;
                    } else {
                        if (b2c_ca[b >> 8][(b & 0xff) - b2Min] == c)
                            b2c_ca[b >> 8][(b & 0xff) - b2Min] = UNMAPPABLE_DECODING;
                    }
                }
            }

            if (!b2cSB_ca.isEmpty()) {
                // SingleByte
                for (gint b = 0; b < b2cSB_ca.length(); b++) {
                    gchar c = b2cSB_ca[b];
                    if (c == UNMAPPABLE_DECODING)
                        continue;
                    gint index = c2bIndex[c >> 8];
                    if (index == 0) {
                        index = off;
                        off += 0x100;
                        c2bIndex[c >> 8] = CORE_CAST(gchar, index);
                    }
                    c2b[index + (c & 0xff)] = CORE_CAST(gchar, b);
                }
            }

            for (gint b1 = 0; b1 < b2c.length(); b1++) {
                // DoubleByte
                CharArray& db = b2c_ca[b1];
                if (db.isEmpty())
                    continue;
                for (gint b2 = b2Min; b2 <= b2Max; b2++) {
                    gchar c = db[b2 - b2Min];
                    if (c == UNMAPPABLE_DECODING)
                        continue;
                    gint index = c2bIndex[c >> 8];
                    if (index == 0) {
                        index = off;
                        off += 0x100;
                        c2bIndex[c >> 8] = CORE_CAST(gchar, index);
                    }
                    c2b[index + (c & 0xff)] = CORE_CAST(gchar, (b1 << 8) | b2);
                }
            }

            if (!c2bNR.isEmpty()) {
                // add c->b only nr entries
                for (gint i = 0; i < c2bNR.length(); i += 2) {
                    gchar b = c2bNR.charAt(i);
                    gchar c = c2bNR.charAt(i + 1);
                    gint index = (c >> 8);
                    if (c2bIndex[index] == 0) {
                        c2bIndex[index] = CORE_CAST(gchar, off);
                        off += 0x100;
                    }
                    index = c2bIndex[index] + (c & 0xff);
                    c2b[index] = b;
                }
            }
        }

        CoderResult DoubleByte::Encoder::encodeArray(io::CharBuffer& src, io::ByteBuffer& dst) {
            CharArray& sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            ByteArray& da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                if (asciiCompatible) {
                    gint n = US_ASCII::encodeASCII(sa, sp, da, dp, Math::min(dl - dp, sl - sp));
                    sp += n;
                    dp += n;
                }
                while (sp < sl) {
                    gchar c = sa[sp];
                    gint bb = encodeChar(c);
                    if (bb == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            if (sgp.parse(c, sa, sp, sl) < 0)
                                cr = sgp.error();
                            else
                                cr = sgp.unmappableResult();
                            goto FINALLY;
                        }
                        cr = CoderResult::unmappableForLength(1);
                        goto FINALLY;
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

                    sp++;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }
        FINALLY:
            src.position(sp - src.arrayOffset());
            dst.position(dp - dst.arrayOffset());

            if (catchEx)
                catchEx->throws($ftrace());

            return cr;
        }

        CoderResult DoubleByte::Encoder::encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst) {
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const* catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    gint bb = encodeChar(c);
                    if (bb == UNMAPPABLE_ENCODING) {
                        if (Character::isSurrogate(c)) {
                            if (sgp.parse(c, src) < 0)
                                cr = sgp.error();
                            else
                                cr = sgp.unmappableResult();
                            goto FINALLY;
                        }
                        cr = CoderResult::unmappableForLength(1);
                        goto FINALLY;
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
                    mark++;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const& ex) { catchEx = &ex; }
        FINALLY:
            src.position(mark);

            if (catchEx)
                catchEx->throws($ftrace());

            return cr;
        }

        CoderResult DoubleByte::Encoder::encode0(io::CharBuffer& src, io::ByteBuffer& dst) {
            if (src.hasArray() && dst.hasArray())
                return encodeArray(src, dst);

            return encodeBuffer(src, dst);
        }

        void DoubleByte::Encoder::replaceWith0(ByteArray const& newReplacement) {
            repl = newReplacement;
        }


         DoubleByte::EncoderEBCDIC::EncoderEBCDIC(Charset const& cs,
                                                  CharArray& c2b, CharArray& c2bIndex,
                                                  gbool isAsciiCompatible)
             : CharsetEncoder(cs, 4.0F, 5.0F),
               Encoder(cs, 4.0F, 5.0F, ByteArray::of(CORE_CAST(gbyte, 0x6F)),
                       c2b, c2bIndex, isAsciiCompatible) {
         }

         gint DoubleByte::EncoderEBCDIC::encode(CharArray const& src, gint sp, gint len, ByteArray& dst) {
             gint dp = 0;
             gint sl = sp + len;
             while (sp < sl) {
                 gchar c = src[sp++];
                 gint bb = encodeChar(c);

                 if (bb == UNMAPPABLE_ENCODING) {
                     if (Character::isHighSurrogate(c) && sp < sl &&
                         Character::isLowSurrogate(src[sp])) {
                         sp++;
                     }
                     dst[dp++] = repl[0];
                     if (repl.length() > 1)
                         dst[dp++] = repl[1];
                     continue;
                 } //else
                 if (bb > MAX_SINGLEBYTE) {
                     // DoubleByte
                     if (state == SBCS) {
                         state = DBCS;
                         dst[dp++] = SO;
                     }
                     dst[dp++] = CORE_CAST(gbyte, bb >> 8);
                     dst[dp++] = CORE_CAST(gbyte, bb);
                 } else {
                     // SingleByte
                     if (state == DBCS) {
                         state = SBCS;
                         dst[dp++] = SI;
                     }
                     dst[dp++] = CORE_CAST(gbyte, bb);
                 }
             }

             if (state == DBCS) {
                 state = SBCS;
                 dst[dp++] = SI;
             }
             return dp;
         }

         gint DoubleByte::EncoderEBCDIC::encodeFromLatin1(ByteArray const& src, gint sp, gint len, ByteArray& dst) {
             gint dp = 0;
             gint sl = sp + len;
             while (sp < sl) {
                 gchar c = CORE_CAST(gchar, src[sp++] & 0xff);
                 gint bb = encodeChar(c);
                 if (bb == UNMAPPABLE_ENCODING) {
                     // no surrogate pair in latin1 string
                     dst[dp++] = repl[0];
                     if (repl.length() > 1)
                         dst[dp++] = repl[1];
                     continue;
                 } //else
                 if (bb > MAX_SINGLEBYTE) {
                     // DoubleByte
                     if (state == SBCS) {
                         state = DBCS;
                         dst[dp++] = SO;
                     }
                     dst[dp++] = CORE_CAST(gbyte, bb >> 8);
                     dst[dp++] = CORE_CAST(gbyte, bb);
                 } else {
                     // SingleByte
                     if (state == DBCS) {
                         state = SBCS;
                         dst[dp++] = SI;
                     }
                     dst[dp++] = CORE_CAST(gbyte, bb);
                 }
             }
             if (state == DBCS) {
                 state = SBCS;
                 dst[dp++] = SI;
             }
             return dp;
         }

         gint DoubleByte::EncoderEBCDIC::encodeFromUTF16(ByteArray const& src, gint sp, gint len, ByteArray& dst) {
             gint dp = 0;
             gint sl = sp + len;
             while (sp < sl) {
                 gchar c = StringUTF16::getChar(src, sp++);
                 gint bb = encodeChar(c);
                 if (bb == UNMAPPABLE_ENCODING) {
                     if (Character::isHighSurrogate(c) && sp < sl &&
                         Character::isLowSurrogate(StringUTF16::getChar(src, sp))) {
                         sp++;
                     }
                     dst[dp++] = repl[0];
                     if (repl.length() > 1)
                         dst[dp++] = repl[1];
                     continue;
                 } //else
                 if (bb > MAX_SINGLEBYTE) {
                     // DoubleByte
                     if (state == SBCS) {
                         state = DBCS;
                         dst[dp++] = SO;
                     }
                     dst[dp++] = CORE_CAST(gbyte, bb >> 8);
                     dst[dp++] = CORE_CAST(gbyte, bb);
                 } else {
                     // SingleByte
                     if (state == DBCS) {
                         state = SBCS;
                         dst[dp++] = SI;
                     }
                     dst[dp++] = CORE_CAST(gbyte, bb);
                 }
             }
             if (state == DBCS) {
                 state = SBCS;
                 dst[dp++] = SI;
             }
             return dp;
         }

         CoderResult DoubleByte::EncoderEBCDIC::flush0(io::ByteBuffer& out) {
             if (state == DBCS) {
                 if (out.remaining() < 1)
                     return CoderResult::OVERFLOW;
                 out.put(SI);
             }
             reset0();
             return CoderResult::UNDERFLOW;
         }

         void DoubleByte::EncoderEBCDIC::reset0() {
             state = SBCS;
         }

         CoderResult DoubleByte::EncoderEBCDIC::encodeArray(io::CharBuffer& src, io::ByteBuffer& dst) {
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
                     gint bb = encodeChar(c);
                     if (bb == UNMAPPABLE_ENCODING) {
                         if (Character::isSurrogate(c)) {
                             if (sgp.parse(c, sa, sp, sl) < 0)
                                 cr = sgp.error();
                             else
                                 cr = sgp.unmappableResult();
                             goto FINALLY;
                         }
                         cr = CoderResult::unmappableForLength(1);
                         goto FINALLY;
                     }
                     if (bb > MAX_SINGLEBYTE) {
                         // DoubleByte
                         if (state == SBCS) {
                             if (dl - dp < 1) {
                                 cr = CoderResult::OVERFLOW;
                                 goto FINALLY;
                             }
                             state = DBCS;
                             da[dp++] = SO;
                         }
                         if (dl - dp < 2) {
                             cr = CoderResult::OVERFLOW;
                             goto FINALLY;
                         }
                         da[dp++] = CORE_CAST(gbyte, bb >> 8);
                         da[dp++] = CORE_CAST(gbyte, bb);
                     } else {
                         // SingleByte
                         if (state == DBCS) {
                             if (dl - dp < 1) {
                                 cr = CoderResult::OVERFLOW;
                                 goto FINALLY;
                             }
                             state = SBCS;
                             da[dp++] = SI;
                         }
                         if (dl - dp < 1) {
                             cr = CoderResult::OVERFLOW;
                             goto FINALLY;
                         }
                         da[dp++] = CORE_CAST(gbyte, bb);
                     }
                     sp++;
                 }
                 cr = CoderResult::UNDERFLOW;
             } catch (Throwable const& ex) { catchEx = &ex; }
         FINALLY:
             src.position(sp - src.arrayOffset());
             dst.position(dp - dst.arrayOffset());

             if (catchEx)
                 catchEx->throws($ftrace());

             return cr;
         }

         CoderResult DoubleByte::EncoderEBCDIC::encodeBuffer(io::CharBuffer& src, io::ByteBuffer& dst) {
             gint mark = src.position();

             CoderResult cr = CoderResult::UNDEFINED;
             Throwable const* catchEx = null;

             try {
                 while (src.hasRemaining()) {
                     gchar c = src.get();
                     gint bb = encodeChar(c);
                     if (bb == UNMAPPABLE_ENCODING) {
                         if (Character::isSurrogate(c)) {
                             if (sgp.parse(c, src) < 0)
                                 cr = sgp.error();
                             else
                                 cr = sgp.unmappableResult();
                             goto FINALLY;
                         }
                         cr = CoderResult::unmappableForLength(1);
                         goto FINALLY;
                     }
                     if (bb > MAX_SINGLEBYTE) {
                         // DoubleByte
                         if (state == SBCS) {
                             if (dst.remaining() < 1) {
                                 cr = CoderResult::OVERFLOW;
                                 goto FINALLY;
                             }
                             state = DBCS;
                             dst.put(SO);
                         }
                         if (dst.remaining() < 2) {
                             cr = CoderResult::OVERFLOW;
                             goto FINALLY;
                         }
                         dst.put(CORE_CAST(gbyte, bb >> 8));
                         dst.put(CORE_CAST(gbyte, bb));
                     } else {
                         // Single-gbyte
                         if (state == DBCS) {
                             if (dst.remaining() < 1) {
                                 cr = CoderResult::OVERFLOW;
                                 goto FINALLY;
                             }
                             state = SBCS;
                             dst.put(SI);
                         }
                         if (dst.remaining() < 1) {
                             cr = CoderResult::OVERFLOW;
                             goto FINALLY;
                         }
                         dst.put(CORE_CAST(gbyte, bb));
                     }
                     mark++;
                 }
                 cr = CoderResult::UNDERFLOW;
             } catch (Throwable const& ex) { catchEx = &ex; }
         FINALLY:
             src.position(mark);

             if (catchEx)
                 catchEx->throws($ftrace());

             return cr;
         }

         DoubleByte::EncoderDBCS_ONLY::EncoderDBCS_ONLY(Charset const& cs, ByteArray const& repl,
                                                        CharArray& c2b, CharArray& c2bIndex, gbool isAsciiCompatible)
             : CharsetEncoder(cs, 2.0F, 2.0F),
               Encoder(cs, 2.0F, 2.0F, repl, c2b, c2bIndex, isAsciiCompatible) {
         }

         gint DoubleByte::EncoderDBCS_ONLY::encodeChar(gchar ch) {
             gint bb = Encoder::encodeChar(ch);
             if (bb <= MAX_SINGLEBYTE)
                 return UNMAPPABLE_ENCODING;
             return bb;
         }

         DoubleByte::EncoderEUC_SIM::EncoderEUC_SIM(Charset const& cs, CharArray& c2b, CharArray& c2bIndex,
                                                    gbool isAsciiCompatible)
                : CharsetEncoder(cs, 2.0F, 2.0F),
                  Encoder(cs, c2b, c2bIndex, isAsciiCompatible) {
         }

    } // charset
} // core
