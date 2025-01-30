//
// Created by bruns on 04/10/2024.
//

#include <meta/charset/ext/IBM1381.h>
#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>

namespace core {
    namespace charset {
        IBM1381::IBM1381(): Charset("x-IBM1381"_S, {
                                        "cp1381"_S,
                                        "ibm1381"_S,
                                        "ibm-1381"_S,
                                        "1381"_S
                                    }) {
        }

        gbool IBM1381::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<IBM1381>::hasInstance(cs);
        }

        CharsetDecoder &IBM1381::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0xA1, 0xFE, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM1381::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String IBM1381::Holder::b2cSBStr = {};
        StringArray IBM1381::Holder::b2cStr = {};
        CharArray2D IBM1381::Holder::b2c = {};
        CharArray IBM1381::Holder::b2cSB = {};
        CharArray IBM1381::Holder::c2b = {};
        CharArray IBM1381::Holder::c2bIndex = {};
        gint IBM1381::Holder::initDecoderStatus = 123456789;
        gint IBM1381::Holder::initEncoderStatus = 123456789;

        void IBM1381::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/ext/IBM1381.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM1381 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM1381::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x6B00);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = String(CharArray{
                        0xA1AA, 0x2014, 0xA1A4, 0x00B7, 0xA1A4, 0x7AC2
                    });
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0xA1, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM1381 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
