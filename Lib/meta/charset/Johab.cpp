//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/Johab.h>

namespace core {
    namespace charset {
        Johab::Johab(): Charset("X-Johab"_S, StringArray::of(
                                    "ksc5601-1992"_S,
                                    "ksc5601_1992"_S,
                                    "ms1361"_S,
                                    "johab"_S)) {
        }

        gbool Johab::contains(const Charset& cs) const {
            return ((cs.name().equals("US-ASCII"_S)) || Class<Johab>::hasInstance(cs));
        }

        CharsetDecoder& Johab::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0x31, 0xFE, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& Johab::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        } 

        String Johab::Holder::b2cSBStr = {};
        StringArray Johab::Holder::b2cStr = {};
        CharArray2D Johab::Holder::b2c = {};
        CharArray Johab::Holder::b2cSB = {};
        CharArray Johab::Holder::c2b = {};
        CharArray Johab::Holder::c2bIndex = {};
        gint Johab::Holder::initDecoderStatus = 123456789;
        gint Johab::Holder::initEncoderStatus = 123456789;

        void Johab::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/Johab.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load Johab Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void Johab::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x9200);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x31, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load Johab Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
