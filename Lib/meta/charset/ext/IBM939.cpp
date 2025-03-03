//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM939.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM939::IBM939(): Charset("x-IBM939"_S, {
                                      "cp939"_S,
                                      "ibm939"_S,
                                      "ibm-939"_S,
                                      "939"_S
                                  }) {
        }

        gbool IBM939::contains(const Charset &cs) const {
            return Class<IBM939>::hasInstance(cs);
        }

        CharsetDecoder &IBM939::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderEBCDIC>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFE,
                                                                      false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM939::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderEBCDIC>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM939::Holder::b2cSBStr = {};
        StringArray IBM939::Holder::b2cStr = {};
        CharArray2D IBM939::Holder::b2c = {};
        CharArray IBM939::Holder::b2cSB = {};
        CharArray IBM939::Holder::c2b = {};
        CharArray IBM939::Holder::c2bIndex = {};
        gint IBM939::Holder::initDecoderStatus = 123456789;
        gint IBM939::Holder::initEncoderStatus = 123456789;

        void IBM939::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM939.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM939 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM939::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x7400);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = String(CharArray({0x0025, 0x000A}));
                    String c2bNR = String(CharArray{
                        0x0015, 0x0085, 0x4260, 0x2212, 0x426A, 0x00A6, 0x43A1, 0x301C,
                        0x444A, 0x2014, 0x446E, 0xF86F, 0x447C, 0x2016, 0x4C7D, 0x9E7C,
                        0x4EB3, 0x9830, 0x4F5E, 0x5861, 0x507F, 0x91AC, 0x5190, 0x56CA,
                        0x51F1, 0x6805, 0x51FA, 0x91B1, 0x5261, 0x9EB4, 0x52A1, 0x881F,
                        0x52C9, 0x840A, 0x52DA, 0x7E61, 0x52EC, 0x4FE0, 0x5353, 0x8EC0,
                        0x5373, 0x7E6B, 0x53B3, 0x8346, 0x53DA, 0x9A52, 0x53E8, 0x87EC,
                        0x53EE, 0x7130, 0x53F8, 0x8523, 0x5443, 0x5C5B, 0x5464, 0x9DD7,
                        0x547D, 0x5699, 0x5481, 0x525D, 0x54A3, 0x6414, 0x54A4, 0x7626,
                        0x54CA, 0x7C1E, 0x54CD, 0x6451, 0x54D4, 0x555E, 0x54FA, 0x6F51,
                        0x5550, 0x7006, 0x5553, 0x79B1, 0x555F, 0x9EB5, 0x55C0, 0x5C62,
                        0x55C1, 0x985A, 0x5B72, 0x6522, 0x5BFE, 0x688E, 0x60F1, 0x7E48,
                        0x61B0, 0x8141, 0x66C8, 0x9839, 0x54D4, 0x6BE1, 0x5550, 0x6D00,
                        0x52EC, 0x70FF, 0x53E8, 0x841D, 0x52A1, 0x841F, 0x446E, 0x8F91,
                        0x547D, 0x92CA, 0x53DA, 0x9B7E
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM939 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
