//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM1364.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM1364::IBM1364(): Charset("x-IBM1364"_S, {
                                        "cp1364"_S,
                                        "ibm1364"_S,
                                        "ibm-1364"_S,
                                        "1364"_S
                                    }) {
        }

        gbool IBM1364::contains(const Charset &cs) const {
            return Class<IBM1364>::hasInstance(cs);
        }

        CharsetDecoder &IBM1364::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::DecoderEBCDIC>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFE,
                                                                      false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM1364::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::EncoderEBCDIC>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM1364::Holder::b2cSBStr = {};
        StringArray IBM1364::Holder::b2cStr = {};
        CharArray2D IBM1364::Holder::b2c = {};
        CharArray IBM1364::Holder::b2cSB = {};
        CharArray IBM1364::Holder::c2b = {};
        CharArray IBM1364::Holder::c2bIndex = {};
        gint IBM1364::Holder::initDecoderStatus = 123456789;
        gint IBM1364::Holder::initEncoderStatus = 123456789;

        void IBM1364::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM1364.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM1364 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM1364::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x9C00);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = String(CharArray{
                        0x4148, 0x00AD, 0x4143, 0x00B7, 0x4149, 0x2015, 0x42A1, 0x223C,
                        0x496F, 0x2299, 0x4954, 0xFF5E
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM1364 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
