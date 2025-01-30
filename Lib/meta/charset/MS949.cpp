//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/MS949.h>

namespace core {
    namespace charset {
        MS949::MS949(): Charset("x-windows-949"_S, StringArray::of(
                                    "ms949"_S,
                                    "windows949"_S,
                                    "windows-949"_S,
                                    "ms_949"_S
                                )) {
        }

        gbool MS949::contains(const Charset& cs) const {
            return (cs.name().equals("US-ASCII"_S) || Class<MS949>::hasInstance(cs));
        }

        CharsetDecoder& MS949::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0x41, 0xFE, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& MS949::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String MS949::Holder::b2cSBStr = {};
        StringArray MS949::Holder::b2cStr = {};
        CharArray2D MS949::Holder::b2c = {};
        CharArray MS949::Holder::b2cSB = {};
        CharArray MS949::Holder::c2b = {};
        CharArray MS949::Holder::c2bIndex = {};
        gint MS949::Holder::initDecoderStatus = 123456789;
        gint MS949::Holder::initEncoderStatus = 123456789;

        void MS949::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/MS949.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load MS949 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void MS949::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x9300);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x41, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load MS949 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }

    } // charset
} // core
