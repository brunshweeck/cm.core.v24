//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/JIS_X_0208_MS932.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        JIS_X_0208_MS932::JIS_X_0208_MS932(): Charset("x-JIS0208_MS932"_S, {}) {
        }

        gbool JIS_X_0208_MS932::contains(const Charset &cs) const {
            return Class<JIS_X_0208_MS932>::hasInstance(cs);
        }

        CharsetDecoder &JIS_X_0208_MS932::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderDBCS_ONLY>(
                    *this, Holder::b2c, Holder::b2cSB, 0x21, 0x7E, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &JIS_X_0208_MS932::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderDBCS_ONLY>(*this, ByteArray({0x21, 0x29}), Holder::c2b,
                                                                         Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String JIS_X_0208_MS932::Holder::b2cSBStr = {};
        StringArray JIS_X_0208_MS932::Holder::b2cStr = {};
        CharArray2D JIS_X_0208_MS932::Holder::b2c = {};
        CharArray JIS_X_0208_MS932::Holder::b2cSB = {};
        CharArray JIS_X_0208_MS932::Holder::c2b = {};
        CharArray JIS_X_0208_MS932::Holder::c2bIndex = {};
        gint JIS_X_0208_MS932::Holder::initDecoderStatus = 123456789;
        gint JIS_X_0208_MS932::Holder::initEncoderStatus = 123456789;

        void JIS_X_0208_MS932::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/JIS_X_0208_MS932.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load JIS_X_0208_MS932 Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void JIS_X_0208_MS932::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x6300);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = String(CharArray{
                        0x224C, 0xFFE2, 0x225D, 0x22A5, 0x2265, 0x221A, 0x2269, 0x222B,
                        0x2D70, 0x2252, 0x2D71, 0x2261, 0x2D77, 0x2220, 0x2D7A, 0x2235,
                        0x2D7B, 0x2229, 0x2D7C, 0x222A
                    });
                    String c2bNR = String(CharArray{
                        0x2124, 0x00B8, 0x2126, 0x00B7, 0x2131, 0x00AF, 0x2263, 0x00AB,
                        0x2264, 0x00BB, 0x2574, 0x3094, 0x264C, 0x00B5
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x21, 0x7E, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load JIS_X_0208_MS932 Encoder table"_S, ex).throws(
                        $ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
