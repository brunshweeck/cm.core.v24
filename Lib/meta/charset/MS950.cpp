//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/MS950.h>

namespace core {
    namespace charset {
        MS950::MS950(): Charset("MS950"_S, StringArray::of("windows-936"_S, "CP936"_S)) {
        }

        gbool MS950::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<MS950>::hasInstance(cs);
        }

        CharsetDecoder &MS950::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFE, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &MS950::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String MS950::Holder::b2cSBStr = {};
        StringArray MS950::Holder::b2cStr = {};
        CharArray2D MS950::Holder::b2c = {};
        CharArray MS950::Holder::b2cSB = {};
        CharArray MS950::Holder::c2b = {};
        CharArray MS950::Holder::c2bIndex = {};
        gint MS950::Holder::initDecoderStatus = 123456789;
        gint MS950::Holder::initEncoderStatus = 123456789;

        void MS950::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/MS950.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load MS950 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void MS950::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x7B00);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = String(CharArray{
                        0xA2A4, 0x2550, 0xA2A5, 0x255E, 0xA2A6, 0x256A, 0xA2A7, 0x2561,
                        0xA2CC, 0x5341, 0xA2CE, 0x5345, 0xF9FA, 0x256D, 0xF9FB, 0x256E,
                        0xF9FC, 0x2570, 0xF9FD, 0x256F,
                    });
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load MS950 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
