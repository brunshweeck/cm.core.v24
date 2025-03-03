//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/Big5.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        Big5::Big5(): Charset("Big5"_S, {"csBig5"_S}) {
        }

        gbool Big5::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<Big5>::hasInstance(cs);
        }

        CharsetDecoder &Big5::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFE, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &Big5::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String Big5::Holder::b2cSBStr = {};
        StringArray Big5::Holder::b2cStr = {};
        CharArray2D Big5::Holder::b2c = {};
        CharArray Big5::Holder::b2cSB = {};
        CharArray Big5::Holder::c2b = {};
        CharArray Big5::Holder::c2bIndex = {};
        gint Big5::Holder::initDecoderStatus = 123456789;
        gint Big5::Holder::initEncoderStatus = 123456789;

        void Big5::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
                    #include <meta/charset/ext/Big5.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load Big5 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void Big5::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x6400);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = String(CharArray{
                        0xA15A, 0xFF3F, 0xA1FE, 0x2571, 0xA240, 0x2572, 0xA2CC, 0x5341, 0xA2CE, 0x5345
                    });
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load Big5 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
