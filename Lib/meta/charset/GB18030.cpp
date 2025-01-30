//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/CharArray.h>
#include <core/ShortArray.h>
#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/GB18030.h>

namespace core {
    namespace charset {
        GB18030::GB18030(): Charset("GB18030"_S, StringArray::of("gb18030-2022"_S)) {
        }

        gbool GB18030::contains(Charset const &cs) const {
            return ((cs.name().equals("US-ASCII"_S))
                    || (cs.name().equals("GBK"_S))
                    || (cs.name().equals("ISO-8859-1"_S))
                    || (cs.name().equals("ISO-8859-2"_S))
                    || (cs.name().equals("ISO-8859-3"_S))
                    || (cs.name().equals("ISO-8859-4"_S))
                    || (cs.name().equals("ISO-8859-5"_S))
                    || (cs.name().equals("ISO-8859-6"_S))
                    || (cs.name().equals("ISO-8859-7"_S))
                    || (cs.name().equals("ISO-8859-8"_S))
                    || (cs.name().equals("ISO-8859-9"_S))
                    || (cs.name().equals("ISO-8859-13"_S))
                    || (cs.name().equals("ISO-8859-15"_S))
                    || (cs.name().equals("ISO-8859-16"_S))
                    || (cs.name().equals("UTF-8"_S))
                    || (cs.name().equals("UTF-16"_S))
                    || (cs.name().equals("UTF-16LE"_S))
                    || (cs.name().equals("UTF-16BE"_S))
                    || (cs.name().equals("windows-1251"_S))
                    || (cs.name().equals("windows-1252"_S))
                    || (cs.name().equals("windows-1253"_S))
                    || (cs.name().equals("windows-1254"_S))
                    || (cs.name().equals("windows-1255"_S))
                    || (cs.name().equals("windows-1256"_S))
                    || (cs.name().equals("windows-1257"_S))
                    || (cs.name().equals("windows-1258"_S))
                    || (cs.name().equals("windows-932"_S))
                    || (cs.name().equals("x-mswin-936"_S))
                    || (cs.name().equals("x-windows-949"_S))
                    || (cs.name().equals("x-windows-950"_S))
                    || (cs.name().equals("windows-31j"_S))
                    || (cs.name().equals("JIS_X0201"_S))
                    || (cs.name().equals("JIS_X0208-1990"_S))
                    || (cs.name().equals("JIS_X0212"_S))
                    || (cs.name().equals("Shift_JIS"_S))
                    || (cs.name().equals("GB2312"_S))
                    || (cs.name().equals("EUC-KR"_S))
                    || (cs.name().equals("x-EUC-TW"_S))
                    || (cs.name().equals("EUC-JP"_S))
                    || (cs.name().equals("euc-jp-linux"_S))
                    || (cs.name().equals("KOI8-R"_S))
                    || (cs.name().equals("TIS-620"_S))
                    || (cs.name().equals("x-ISCII91"_S))
                    || (cs.name().equals("Big5"_S))
                    || (cs.name().equals("Big5-HKSCS"_S))
                    || (cs.name().equals("x-MS950-HKSCS"_S))
                    || (cs.name().equals("ISO-2022-JP"_S))
                    || (cs.name().equals("ISO-2022-KR"_S))
                    || (cs.name().equals("x-ISO-2022-CN-CNS"_S))
                    || (cs.name().equals("x-ISO-2022-CN-GB"_S))
                    || (cs.name().equals("x-Johab"_S))
                    || (Class<GB18030>::hasInstance(cs)));
        }

        CharsetDecoder &GB18030::newDecoder() const {
            try {
                if(IS_2000) {
                    initializeDecoder2000Index();
                    initialize2000Index();
                } else {
                    initializeDecoderIndex();
                    initializeIndex();
                }
                return UNSAFE::newInstance<Decoder>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &GB18030::newEncoder() const {
            try {
                if(IS_2000)
                    initializeEncoder2000Index();
                else
                    initializeEncoderIndex();
                return UNSAFE::newInstance<Encoder>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void GB18030::initializeIndex() {
            if(initStatus == 123456789) {
                #include <meta/charset/GB18030Index.db>
                initStatus = 123454321;
            }
            CORE_ASSERT(initStatus == 123454321);
        }

        void GB18030::initialize2000Index() {
            if(initStatus == 123456789) {
                #include <meta/charset/GB18030Index2000.db>
                initStatus = 123454321;
            }
            CORE_ASSERT(initStatus == 123454321);
        }

        void GB18030::initializeDecoderIndex() {
            if(initDecoderStatus == 123456789) {
                #include <meta/charset/GB18030DecoderIndex.db>
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void GB18030::initializeDecoder2000Index() {
            if(initDecoderStatus == 123456789) {
                #include <meta/charset/GB18030DecoderIndex2000.db>
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void GB18030::initializeEncoderIndex() {
            if(initEncoderStatus == 123456789) {
                #include <meta/charset/GB18030EncoderIndex.db>
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }

        void GB18030::initializeEncoder2000Index() {
            if(initEncoderStatus == 123456789) {
                #include <meta/charset/GB18030EncoderIndex2000.db>
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }

        GB18030::Decoder::Decoder(Charset const &cs): CharsetDecoder(cs, 1.0F, 2.0F) {
        }

        gchar GB18030::Decoder::decodeDouble(gint byte1, gint byte2) {
            gint start = 0x40, end = 0xFE;
            if (((byte1 < 0) || (byte1 > index1.length()))
                || ((byte2 < start) || (byte2 > end)))
                return u'\uFFFD';

            gint n = (index1[byte1] & 0xf) * (end - start + 1) + (byte2 - start);
            return index2[index1[byte1] >> 4].charAt(n);
        }

        void GB18030::Decoder::reset0() { currentState = GB18030_DOUBLE_BYTE; }

        CoderResult GB18030::Decoder::decode0(io::ByteBuffer &src, io::CharBuffer &dst) {
            try {
                if (src.hasArray() && dst.hasArray())
                    return decodeArray(src, dst);

                return decodeBuffer(src, dst);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gchar GB18030::Decoder::getChar(gint offset) {
            gint byte1 = (offset >> 8) & 0xFF;
            gint byte2 = (offset & 0xFF);
            gint start = 0, end = 0xFF;

            if (((byte1 < 0) || (byte1 > decoderIndex1.length()))
                || ((byte2 < start) || (byte2 > end))) {
                return REPLACE_CHAR;
            }

            gint n = (decoderIndex1[byte1] & 0xf) * (end - start + 1) + (byte2 - start);
            return decoderIndex2[decoderIndex1[byte1] >> 4].charAt(n);
        }

        CoderResult GB18030::Decoder::decodeArray(io::ByteBuffer &src, io::CharBuffer &dst) {
            ByteArray &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            CharArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();

            gint inputSize = 1;

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = null;

            try {
                while (sp < sl) {
                    gint byte1 = 0, byte2 = 0, byte3 = 0, byte4 = 0;
                    // Get the input gbyte
                    byte1 = sa[sp] & 0xFF;
                    inputSize = 1;

                    if ((byte1 & CORE_CAST(gbyte, 0x80)) == 0) {
                        // US-ASCII range
                        currentState = GB18030_SINGLE_BYTE;
                    } else if (byte1 < 0x81 || byte1 > 0xfe) {
                        cr = CoderResult::malformedForLength(1);
                        goto FINALLY;
                    } else {
                        // Either 2 or 4 gbyte sequence follows
                        if (sl - sp < 2) {
                            cr = CoderResult::UNDERFLOW;
                            goto FINALLY;
                        }
                        byte2 = sa[sp + 1] & 0xFF;
                        inputSize = 2;

                        if (byte2 < 0x30) {
                            cr = CoderResult::malformedForLength(1);
                            goto FINALLY;
                        }
                        if (byte2 >= 0x30 && byte2 <= 0x39) {
                            currentState = GB18030_FOUR_BYTE;

                            if (sl - sp < 4) {
                                cr = CoderResult::UNDERFLOW;
                                goto FINALLY;
                            }

                            byte3 = sa[sp + 2] & 0xFF;
                            if (byte3 < 0x81 || byte3 > 0xfe) {
                                cr = CoderResult::malformedForLength(3);
                                goto FINALLY;
                            }

                            byte4 = sa[sp + 3] & 0xFF;
                            inputSize = 4;

                            if (byte4 < 0x30 || byte4 > 0x39) {
                                cr = CoderResult::malformedForLength(4);
                                goto FINALLY;
                            }
                        } else if (byte2 == 0x7f || byte2 == 0xff ||
                                   (byte2 < 0x40)) {
                            cr = CoderResult::malformedForLength(2);
                            goto FINALLY;
                        } else
                            currentState = GB18030_DOUBLE_BYTE;
                    }

                    if (dl - dp < 1) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    switch (currentState) {
                        case GB18030_SINGLE_BYTE:
                            da[dp++] = (gchar) byte1;
                            break;
                        case GB18030_DOUBLE_BYTE:
                            da[dp++] = decodeDouble(byte1, byte2);
                            break;
                        case GB18030_FOUR_BYTE:
                            gint offset = (((byte1 - 0x81) * 10 +
                                            (byte2 - 0x30)) * 126 +
                                           byte3 - 0x81) * 10 + byte4 - 0x30;
                            gint hiByte = (offset >> 8) & 0xFF;
                            gint lowByte = (offset & 0xFF);

                        // Mixture of table lookups and algorithmic calculation
                        // of character values.

                        // BMP Ranges
                            if (offset <= 0x4A62)
                                da[dp++] = getChar(offset);
                            else if (offset > 0x4A62 && offset <= 0x82BC) {
                                if (offset >= 0x4A71 && offset <= 0x4A78 && !IS_2000) {
                                    da[dp++] = getChar(offset);
                                } else {
                                    da[dp++] = (gchar) (offset + 0x5543);
                                }
                            } else if (offset >= 0x82BD && offset <= 0x830D)
                                da[dp++] = getChar(offset);
                            else if (offset >= 0x830D && offset <= 0x93A8)
                                da[dp++] = (gchar) (offset + 0x6557);
                            else if (offset >= 0x93A9 && offset <= 0x99FB)
                                da[dp++] = getChar(offset);
                                // Supplemental UCS planes handled via surrogates
                            else if (offset >= 0x2E248 && offset < 0x12E248) {
                                if (offset >= 0x12E248) {
                                    cr = CoderResult::malformedForLength(4);
                                    goto FINALLY;
                                }
                                offset -= 0x1e248;
                                if (dl - dp < 2) {
                                    cr = CoderResult::OVERFLOW;
                                    goto FINALLY;
                                }
                                // emit high + low surrogate
                                da[dp++] = (gchar) ((offset - 0x10000) / 0x400 + 0xD800);
                                da[dp++] = (gchar) ((offset - 0x10000) % 0x400 + 0xDC00);
                            } else {
                                cr = CoderResult::malformedForLength(inputSize);
                                goto FINALLY;
                            }
                            break;
                    }
                    sp += inputSize;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { catchEx = &ex; }

        FINALLY: {
                src.position(sp - src.arrayOffset());
                dst.position(dp - dst.arrayOffset());

                if (catchEx)
                    catchEx->throws($ftrace());

                return cr;
            }
        }

        CoderResult GB18030::Decoder::decodeBuffer(io::ByteBuffer &src, io::CharBuffer &dst) {
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gint byte1 = 0, byte2 = 0, byte3 = 0, byte4 = 0;
                    byte1 = src.get() & 0xFF;
                    gint inputSize = 1;

                    if ((byte1 & CORE_CAST(gbyte, 0x80)) == 0) {
                        // US-ASCII range
                        currentState = GB18030_SINGLE_BYTE;
                    } else if (byte1 < 0x81 || byte1 > 0xfe) {
                        cr = CoderResult::malformedForLength(1);
                        goto FINALLY;
                    } else {
                        // Either 2 or 4 gbyte sequence follows
                        if (src.remaining() < 1) {
                            cr = CoderResult::UNDERFLOW;
                            goto FINALLY;
                        }
                        byte2 = src.get() & 0xFF;
                        inputSize = 2;

                        if (byte2 < 0x30) {
                            cr = CoderResult::malformedForLength(1);
                            goto FINALLY;
                        } else if (byte2 >= 0x30 && byte2 <= 0x39) {
                            currentState = GB18030_FOUR_BYTE;

                            if (src.remaining() < 2) {
                                cr = CoderResult::UNDERFLOW;
                                goto FINALLY;
                            }

                            byte3 = src.get() & 0xFF;
                            if (byte3 < 0x81 || byte3 > 0xfe) {
                                cr = CoderResult::malformedForLength(3);
                                goto FINALLY;
                            }

                            byte4 = src.get() & 0xFF;
                            inputSize = 4;

                            if (byte4 < 0x30 || byte4 > 0x39) {
                                cr = CoderResult::malformedForLength(4);
                                goto FINALLY;
                            }
                        } else if (byte2 == 0x7f || byte2 == 0xff ||
                                   (byte2 < 0x40)) {
                            cr = CoderResult::malformedForLength(2);
                            goto FINALLY;
                        } else
                            currentState = GB18030_DOUBLE_BYTE;
                    }

                    if (dst.remaining() < 1) {
                        cr = CoderResult::OVERFLOW;
                        goto FINALLY;
                    }
                    switch (currentState) {
                        case GB18030_SINGLE_BYTE:
                            dst.put((gchar) byte1);
                            break;
                        case GB18030_DOUBLE_BYTE:
                            dst.put(decodeDouble(byte1, byte2));
                            break;
                        case GB18030_FOUR_BYTE:
                            gint offset = (((byte1 - 0x81) * 10 +
                                            (byte2 - 0x30)) * 126 +
                                           byte3 - 0x81) * 10 + byte4 - 0x30;
                            gint hiByte = (offset >> 8) & 0xFF;
                            gint lowByte = (offset & 0xFF);

                        // Mixture of table lookups and algorithmic calculation
                        // of character values.

                        // BMP Ranges
                            if (offset <= 0x4A62)
                                dst.put(getChar(offset));
                            else if (offset > 0x4A62 && offset <= 0x82BC) {
                                if (offset >= 0x4A71 && offset <= 0x4A78 && !IS_2000) {
                                    dst.put(getChar(offset));
                                } else {
                                    dst.put((gchar) (offset + 0x5543));
                                }
                            } else if (offset >= 0x82BD && offset <= 0x830D)
                                dst.put(getChar(offset));
                            else if (offset >= 0x830D && offset <= 0x93A8)
                                dst.put((gchar) (offset + 0x6557));
                            else if (offset >= 0x93A9 && offset <= 0x99FB)
                                dst.put(getChar(offset));
                                // Supplemental UCS planes handled via surrogates
                            else if (offset >= 0x2E248 && offset < 0x12E248) {
                                if (offset >= 0x12E248) {
                                    cr = CoderResult::malformedForLength(4);
                                    goto FINALLY;
                                }
                                offset -= 0x1e248;
                                if (dst.remaining() < 2) {
                                    cr = CoderResult::OVERFLOW;
                                    goto FINALLY;
                                }
                                // emit high + low surrogate
                                dst.put((gchar) ((offset - 0x10000) / 0x400 + 0xD800));
                                dst.put((gchar) ((offset - 0x10000) % 0x400 + 0xDC00));
                            } else {
                                cr = CoderResult::malformedForLength(inputSize);
                                goto FINALLY;
                            }
                    }
                    mark += inputSize;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { catchEx = &ex; }

        FINALLY: {
                src.position(mark);

                if (catchEx)
                    catchEx->throws($ftrace());

                return cr;
            }
        }

        GB18030::Encoder::Encoder(Charset const &cs)
            : CharsetEncoder(cs, 4.0F, 4.0F) {
            // max of 4 bytes per gchar
        }

        gbool GB18030::Encoder::canEncode(gchar c) { return !Character::isSurrogate(c); }

        void GB18030::Encoder::reset0() { currentState = GB18030_DOUBLE_BYTE; }

        CoderResult GB18030::Encoder::encode0(io::CharBuffer &src, io::ByteBuffer &dst) {
            try {
                if (src.hasArray() && dst.hasArray())
                    return encodeArray(src, dst);

                return encodeBuffer(src, dst);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint GB18030::Encoder::getGB18030(ShortArray const &outerIndex, StringArray const &innerEncoderIndex,
                                          gchar ch) {
            gint offset = outerIndex[((ch & 0xff00) >> 8)] << 8;
            return innerEncoderIndex[offset >> 12].charAt((offset & 0xfff) + (ch & 0xff));
        }

        CoderResult GB18030::Encoder::encodeArray(io::CharBuffer &src, io::ByteBuffer &dst) {
            CharArray &sa = src.array();
            gint sp = src.arrayOffset() + src.position();
            gint sl = src.arrayOffset() + src.limit();

            ByteArray &da = dst.array();
            gint dp = dst.arrayOffset() + dst.position();
            gint dl = dst.arrayOffset() + dst.limit();

            gint condensedKey = 0; // expands to a four gbyte sequence
            gint hiByte = 0, loByte = 0;
            currentState = GB18030_DOUBLE_BYTE;

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = null;

            try {
                while (sp < sl) {
                    gint inputSize = 1;
                    gchar c = sa[sp];

                    if (Character::isSurrogate(c)) {
                        if ((condensedKey = sgp.parse(c, sa, sp, sl)) < 0) {
                            cr = sgp.error();
                            goto FINALLY;
                        }
                        // Character::toCodePoint looks like
                        // (((high & 0x3ff) << 10) | (low & 0x3ff)) + 0x10000;
                        // so we add (0x2e248 - 0x10000) to get the "key".
                        condensedKey += 0x1E248;
                        currentState = GB18030_FOUR_BYTE;
                        inputSize = sgp.increment();
                    } else if (c >= 0x0000 && c <= 0x007F) {
                        currentState = GB18030_SINGLE_BYTE;
                    } else if (c <= 0xA4C6 || c >= 0xE000) {
                        gint outByteVal = getGB18030(encoderIndex1, encoderIndex2, c);
                        if (outByteVal == 0xFFFD) {
                            cr = CoderResult::unmappableForLength(1);
                            goto FINALLY;
                        }

                        hiByte = (outByteVal & 0xFF00) >> 8;
                        loByte = (outByteVal & 0xFF);

                        condensedKey = (hiByte - 0x20) * 256 + loByte;

                        if (c >= 0xE000 && c < 0xF900) {
                            if (IS_2000) {
                                condensedKey += 0x82BD;
                            } else {
                                switch (c) {
                                    case 0xE7C7:
                                    case 0xE81E:
                                    case 0xE826:
                                    case 0xE82B:
                                    case 0xE82C:
                                    case 0xE832:
                                    case 0xE843:
                                    case 0xE854:
                                    case 0xE864:
                                        break;
                                    default:
                                        condensedKey = condensedKey + 0x82BD;
                                };
                            }
                        } else if (c >= 0xF900)
                            condensedKey += 0x93A9;

                        if (hiByte > 0x80)
                            currentState = GB18030_DOUBLE_BYTE;
                        else
                            currentState = GB18030_FOUR_BYTE;
                    } else if (c >= 0xA4C7 && c <= 0xD7FF) {
                        condensedKey = c - 0x5543;
                        currentState = GB18030_FOUR_BYTE;
                    }

                    switch (currentState) {
                        case GB18030_SINGLE_BYTE:
                            if (dl - dp < 1) {
                                cr = CoderResult::OVERFLOW;
                                goto FINALLY;
                            }
                            da[dp++] = (gbyte) c;
                            break;

                        case GB18030_DOUBLE_BYTE:
                            if (dl - dp < 2) {
                                cr = CoderResult::OVERFLOW;
                                goto FINALLY;
                            }
                            da[dp++] = (gbyte) hiByte;
                            da[dp++] = (gbyte) loByte;
                            break;

                        case GB18030_FOUR_BYTE: // Four Byte encoding
                            gbyte b1, b2, b3, b4;

                            if (dl - dp < 4) {
                                cr = CoderResult::OVERFLOW;
                                goto FINALLY;
                            }
                        // Decompose the condensed key into its 4 gbyte equivalent
                            b4 = (gbyte) ((condensedKey % 10) + 0x30);
                            condensedKey /= 10;
                            b3 = (gbyte) ((condensedKey % 126) + 0x81);
                            condensedKey /= 126;
                            b2 = (gbyte) ((condensedKey % 10) + 0x30);
                            b1 = (gbyte) ((condensedKey / 10) + 0x81);
                            da[dp++] = b1;
                            da[dp++] = b2;
                            da[dp++] = b3;
                            da[dp++] = b4;
                            break;
                        default:
                            CORE_ASSERT(false);
                    }
                    sp += inputSize;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { catchEx = &ex; }
        FINALLY: {
                src.position(sp - src.arrayOffset());
                dst.position(dp - dst.arrayOffset());

                if (catchEx)
                    catchEx->throws($ftrace());

                return cr;
            }
        }

        CoderResult GB18030::Encoder::encodeBuffer(io::CharBuffer &src, io::ByteBuffer &dst) {
            gint condensedKey = 0;
            gint hiByte = 0, loByte = 0;
            currentState = GB18030_DOUBLE_BYTE;
            gint mark = src.position();

            CoderResult cr = CoderResult::UNDEFINED;
            Throwable const *catchEx = null;

            try {
                while (src.hasRemaining()) {
                    gchar c = src.get();
                    gint inputSize = 1;
                    if (Character::isSurrogate(c)) {
                        if ((condensedKey = sgp.parse(c, src)) < 0) {
                            cr = sgp.error();
                            goto FINALLY;
                        }
                        condensedKey += 0x1e248;
                        currentState = GB18030_FOUR_BYTE;
                        inputSize = 2;
                    } else if (c >= 0x0000 && c <= 0x007F) {
                        currentState = GB18030_SINGLE_BYTE;
                    } else if (c <= 0xA4C6 || c >= 0xE000) {
                        gint outByteVal = getGB18030(encoderIndex1,
                                                     encoderIndex2,
                                                     c);
                        if (outByteVal == 0xFFFD) {
                            cr = CoderResult::unmappableForLength(1);
                            goto FINALLY;
                        }

                        hiByte = (outByteVal & 0xFF00) >> 8;
                        loByte = (outByteVal & 0xFF);

                        condensedKey = (hiByte - 0x20) * 256 + loByte;

                        if (c >= 0xE000 && c < 0xF900) {
                            if (IS_2000) {
                                condensedKey += 0x82BD;
                            } else {
                                switch (c) {
                                    case 0xE7C7:
                                    case 0xE81E:
                                    case 0xE826:
                                    case 0xE82B:
                                    case 0xE82C:
                                    case 0xE832:
                                    case 0xE843:
                                    case 0xE854:
                                    case 0xE864: break;
                                    default: condensedKey = condensedKey + 0x82BD;
                                };
                            }
                        } else if (c >= 0xF900)
                            condensedKey += 0x93A9;

                        if (hiByte > 0x80)
                            currentState = GB18030_DOUBLE_BYTE;
                        else
                            currentState = GB18030_FOUR_BYTE;
                    } else if (c >= 0xA4C7 && c <= 0xD7FF) {
                        condensedKey = c - 0x5543;
                        currentState = GB18030_FOUR_BYTE;
                    }

                    if (currentState == GB18030_SINGLE_BYTE) {
                        if (dst.remaining() < 1) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        dst.put((gbyte) c);
                    } else if (currentState == GB18030_DOUBLE_BYTE) {
                        if (dst.remaining() < 2) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        dst.put((gbyte) hiByte);
                        dst.put((gbyte) loByte);
                    } else {
                        // Four Byte encoding
                        gbyte b1, b2, b3, b4;

                        if (dst.remaining() < 4) {
                            cr = CoderResult::OVERFLOW;
                            goto FINALLY;
                        }
                        // Decompose the condensed key into its 4 gbyte equivalent
                        b4 = (gbyte) ((condensedKey % 10) + 0x30);
                        condensedKey /= 10;
                        b3 = (gbyte) ((condensedKey % 126) + 0x81);
                        condensedKey /= 126;
                        b2 = (gbyte) ((condensedKey % 10) + 0x30);
                        b1 = (gbyte) ((condensedKey / 10) + 0x81);
                        dst.put(b1);
                        dst.put(b2);
                        dst.put(b3);
                        dst.put(b4);
                    }
                    mark += inputSize;
                }
                cr = CoderResult::UNDERFLOW;
            } catch (Throwable const &ex) { catchEx = &ex; }
        FINALLY: {
                src.position(mark);

                if (catchEx)
                    catchEx->throws($ftrace());

                return cr;
            }
        }


        ShortArray GB18030::decoderIndex1 = {};

        StringArray GB18030::decoderIndex2 = {};

        ShortArray GB18030::index1 = {};

        StringArray GB18030::index2 = {};

        ShortArray GB18030::encoderIndex1 = {};

        StringArray GB18030::encoderIndex2 = {};

        gint GB18030::initStatus = 123456789;
        gint GB18030::initDecoderStatus = 123456789;
        gint GB18030::initEncoderStatus = 123456789;
    } // charset
} // core
