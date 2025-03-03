//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM935.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM935::IBM935(): Charset("x-IBM935"_S, {
                                      "cp935"_S,
                                      "ibm935"_S,
                                      "ibm-935"_S,
                                      "935"_S
                                  }) {
        }

        gbool IBM935::contains(const Charset &cs) const {
            return Class<IBM935>::hasInstance(cs);
        }

        CharsetDecoder &IBM935::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderEBCDIC>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFE, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM935::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderEBCDIC>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM935::Holder::b2cSBStr = {};
        StringArray IBM935::Holder::b2cStr = {};
        CharArray2D IBM935::Holder::b2c = {};
        CharArray IBM935::Holder::b2cSB = {};
        CharArray IBM935::Holder::c2b = {};
        CharArray IBM935::Holder::c2bIndex = {};
        gint IBM935::Holder::initDecoderStatus = 123456789;
        gint IBM935::Holder::initEncoderStatus = 123456789;

        void IBM935::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM935.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM935 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM935::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x6b00);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = String(CharArray({0x0025, 0x000A}));
                    String c2bNR = String(CharArray({0x0015, 0x0085}));
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM935 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
