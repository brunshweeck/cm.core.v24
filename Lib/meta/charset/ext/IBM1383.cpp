//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM1383.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM1383::IBM1383(): Charset("x-IBM1383"_S, {
                                        "cp1383"_S,
                                        "ibm1383"_S,
                                        "ibm-1383"_S,
                                        "1383"_S,
                                        "ibmeuccn"_S,
                                        "ibm-euccn"_S,
                                        "cpeuccn"_S
                                    }) {
        }

        gbool IBM1383::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<IBM1383>::hasInstance(cs);
        }

        CharsetDecoder &IBM1383::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderEUC_SIM>(*this, Holder::b2c, Holder::b2cSB, 0xA1, 0xFE,
                                                                       true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM1383::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderEUC_SIM>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM1383::Holder::b2cSBStr = {};
        StringArray IBM1383::Holder::b2cStr = {};
        CharArray2D IBM1383::Holder::b2c = {};
        CharArray IBM1383::Holder::b2cSB = {};
        CharArray IBM1383::Holder::c2b = {};
        CharArray IBM1383::Holder::c2bIndex = {};
        gint IBM1383::Holder::initDecoderStatus = 123456789;
        gint IBM1383::Holder::initEncoderStatus = 123456789;

        void IBM1383::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM1383.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM1383 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM1383::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x6900);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = String(CharArray{
                        0xFEF4, 0x4EDD, 0xFEEE, 0xFFE5
                    });
                    String c2bNR = String(CharArray{
                        0xA1AA, 0x2014, 0xA1A4, 0x00B7, 0xA1A4, 0x8EA2, 0xFEEE, 0xF83D,
                        0xFEF4, 0xF83E
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0xA1, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM1383 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
