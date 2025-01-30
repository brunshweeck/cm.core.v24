//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/GB2312.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        GB2312::GB2312(): Charset("GB2312"_S, {
                                      "gb2312"_S,
                                      "gb2312-80"_S,
                                      "gb2312-1980"_S,
                                      "euc-cn"_S,
                                      "euccn"_S,
                                      "x-EUC-CN"_S,
                                      "EUC_CN"_S
                                  }) {
        }

        gbool GB2312::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<GB2312>::hasInstance(cs);
        }

        CharsetDecoder &GB2312::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0xA1, 0xFE, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &GB2312::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String GB2312::Holder::b2cSBStr = {};
        StringArray GB2312::Holder::b2cStr = {};
        CharArray2D GB2312::Holder::b2c = {};
        CharArray GB2312::Holder::b2cSB = {};
        CharArray GB2312::Holder::c2b = {};
        CharArray GB2312::Holder::c2bIndex = {};
        gint GB2312::Holder::initDecoderStatus = 123456789;
        gint GB2312::Holder::initEncoderStatus = 123456789;

        void GB2312::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/GB2312.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load GB2312 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void GB2312::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x6200);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0xA1, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load GB2312 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
