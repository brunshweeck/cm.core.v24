//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM949.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM949::IBM949(): Charset("x-IBM949"_S, {
                                      "cp949"_S,
                                      "ibm949"_S,
                                      "ibm-949"_S,
                                      "949"_S
                                  }) {
        }

        gbool IBM949::contains(const Charset &cs) const {
            return Class<IBM949>::hasInstance(cs);
        }

        CharsetDecoder &IBM949::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0xA1, 0xFE, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM949::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM949::Holder::b2cSBStr = {};
        StringArray IBM949::Holder::b2cStr = {};
        CharArray2D IBM949::Holder::b2c = {};
        CharArray IBM949::Holder::b2cSB = {};
        CharArray IBM949::Holder::c2b = {};
        CharArray IBM949::Holder::c2bIndex = {};
        gint IBM949::Holder::initDecoderStatus = 123456789;
        gint IBM949::Holder::initEncoderStatus = 123456789;

        void IBM949::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM949.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM949 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM949::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x9800);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0xA1, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM949 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
