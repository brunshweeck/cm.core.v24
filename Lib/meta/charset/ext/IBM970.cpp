//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM970.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM970::IBM970(): Charset("x-IBM970"_S, {
                                      "cp970"_S,
                                      "ibm970"_S,
                                      "ibm-970"_S,
                                      "ibm-eucKR"_S,
                                      "970"_S
                                  }) {
        }

        gbool IBM970::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<IBM970>::hasInstance(cs);
        }

        CharsetDecoder &IBM970::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderEUC_SIM>(*this, Holder::b2c, Holder::b2cSB, 0xA1, 0xFE,
                                                                       true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM970::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderEUC_SIM>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM970::Holder::b2cSBStr = {};
        StringArray IBM970::Holder::b2cStr = {};
        CharArray2D IBM970::Holder::b2c = {};
        CharArray IBM970::Holder::b2cSB = {};
        CharArray IBM970::Holder::c2b = {};
        CharArray IBM970::Holder::c2bIndex = {};
        gint IBM970::Holder::initDecoderStatus = 123456789;
        gint IBM970::Holder::initEncoderStatus = 123456789;

        void IBM970::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM970.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM970 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM970::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x9300);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = String(CharArray{
                        0xA1A4, 0x00B7, 0xA1A9, 0x00AD, 0xA1AA, 0x2015, 0xA1AD, 0x223C,
                        0xA2A6, 0xFF5E, 0xA2C1, 0x2299, 0xA3DC, 0x20A9
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0xA1, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM970 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
