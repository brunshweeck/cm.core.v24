//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM942.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM942::IBM942(): Charset("x-IBM942"_S, {
                                      "cp942"_S,
                                      "ibm942"_S,
                                      "ibm-942"_S,
                                      "942"_S
                                  }) {
        }

        gbool IBM942::contains(const Charset &cs) const {
            return Class<IBM942>::hasInstance(cs);
        }

        CharsetDecoder &IBM942::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFC, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM942::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM942::Holder::b2cSBStr = {};
        StringArray IBM942::Holder::b2cStr = {};
        CharArray2D IBM942::Holder::b2c = {};
        CharArray IBM942::Holder::b2cSB = {};
        CharArray IBM942::Holder::c2b = {};
        CharArray IBM942::Holder::c2bIndex = {};
        gint IBM942::Holder::initDecoderStatus = 123456789;
        gint IBM942::Holder::initEncoderStatus = 123456789;

        void IBM942::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM942.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM942 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM942::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x6A00);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = String(CharArray{
                        0x90E4, 0x551E, 0x93C0, 0x6D00, 0x91CB, 0x6F1E, 0x8BA0, 0x70FF,
                        0x88A0, 0x8741, 0xFA59, 0x8F91, 0x8A9A, 0x92CA, 0x815C, 0x2015,
                        0x8160, 0xFF5E, 0x8161, 0x2225, 0x817C, 0xFF0D, 0x88A0, 0x555E,
                        0x898B, 0x7130, 0x89A8, 0x9DD7, 0x8A9A, 0x5699, 0x8BA0, 0x4FE0,
                        0x8BEB, 0x8EC0, 0x8C71, 0x7E6B, 0x8C74, 0x8346, 0x8CB2, 0x9E7C,
                        0x8D8D, 0x9EB4, 0x8DF2, 0x6805, 0x8EC6, 0x5C62, 0x8F4A, 0x7E61,
                        0x8FD3, 0x8523, 0x8FDD, 0x91AC, 0x90E4, 0x87EC, 0x917E, 0x6414,
                        0x9189, 0x7626, 0x91CB, 0x9A52, 0x925C, 0x7C1E, 0x92CD, 0x6451,
                        0x9355, 0x5861, 0x935E, 0x985A, 0x9398, 0x79B1, 0x93C0, 0x7006,
                        0x9458, 0x56CA, 0x948D, 0x525D, 0x94AC, 0x6F51, 0x94AE, 0x91B1,
                        0x966A, 0x9830, 0x96CB, 0x9EB5, 0x9789, 0x840A, 0x9858, 0x881F,
                        0x9BA0, 0x5C5B, 0x9DB7, 0x6522, 0x9E94, 0x688E, 0xE379, 0x7E48,
                        0xE445, 0x8141, 0xE8F6, 0x9839, 0xFA55, 0xFFE4, 0xFA59, 0xF86F
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFC, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM942 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
