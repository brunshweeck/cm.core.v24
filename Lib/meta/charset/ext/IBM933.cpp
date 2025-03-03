//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM933.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM933::IBM933(): Charset("x-IBM933"_S, {
                                      "cp933"_S,
                                      "ibm933"_S,
                                      "ibm-933"_S,
                                      "933"_S
                                  }) {
        }

        gbool IBM933::contains(const Charset &cs) const {
            return Class<IBM933>::hasInstance(cs);
        }

        CharsetDecoder &IBM933::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderEBCDIC>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFE,
                                                                      false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM933::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderEBCDIC>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM933::Holder::b2cSBStr = {};
        StringArray IBM933::Holder::b2cStr = {};
        CharArray2D IBM933::Holder::b2c = {};
        CharArray IBM933::Holder::b2cSB = {};
        CharArray IBM933::Holder::c2b = {};
        CharArray IBM933::Holder::c2bIndex = {};
        gint IBM933::Holder::initDecoderStatus = 123456789;
        gint IBM933::Holder::initEncoderStatus = 123456789;

        void IBM933::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM933.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM933 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM933::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x9b00);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = String(CharArray({0x0015, 0x0085}));
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM933 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
