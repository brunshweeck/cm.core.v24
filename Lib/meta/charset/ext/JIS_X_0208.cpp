//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/JIS_X_0208.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        JIS_X_0208::JIS_X_0208(): Charset("x-JIS0208"_S, {
                                              "JIS0208"_S,
                                              "JIS_C6226-1983"_S,
                                              "iso-ir-87"_S,
                                              "x0208"_S,
                                              "JIS_X0208-1983"_S,
                                              "csISO87JISX0208"_S
                                          }) {
        }

        gbool JIS_X_0208::contains(const Charset &cs) const {
            return Class<JIS_X_0208>::hasInstance(cs);
        }

        CharsetDecoder &JIS_X_0208::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderDBCS_ONLY>(
                    *this, Holder::b2c, Holder::b2cSB, 0x21, 0x7E, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &JIS_X_0208::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderDBCS_ONLY>(
                    *this, ByteArray({0x21, 0x29}), Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String JIS_X_0208::Holder::b2cSBStr = {};
        StringArray JIS_X_0208::Holder::b2cStr = {};
        CharArray2D JIS_X_0208::Holder::b2c = {};
        CharArray JIS_X_0208::Holder::b2cSB = {};
        CharArray JIS_X_0208::Holder::c2b = {};
        CharArray JIS_X_0208::Holder::c2bIndex = {};
        gint JIS_X_0208::Holder::initDecoderStatus = 123456789;
        gint JIS_X_0208::Holder::initEncoderStatus = 123456789;

        void JIS_X_0208::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/JIS_X_0208.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load JIS_X_0208 Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void JIS_X_0208::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x5E00);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x21, 0x7E, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load JIS_X_0208 Encoder table"_S, ex)
                            .throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
