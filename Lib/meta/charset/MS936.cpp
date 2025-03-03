//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/MS936.h>

namespace core {
    namespace charset {
        MS936::MS936(): Charset("x-mswin-936"_S, StringArray::of("ms936"_S, "ms_936"_S)) {
        }

        gbool MS936::contains(const Charset& cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<MS936>::hasInstance(cs);
        }

        CharsetDecoder& MS936::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, Holder::b2c, Holder::b2cSB, 0x40, 0xFE, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& MS936::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String MS936::Holder::b2cSBStr = {};
        StringArray MS936::Holder::b2cStr = {};
        CharArray2D MS936::Holder::b2c = {};
        CharArray MS936::Holder::b2cSB = {};
        CharArray MS936::Holder::c2b = {};
        CharArray MS936::Holder::c2bIndex = {};
        gint MS936::Holder::initDecoderStatus = 123456789;
        gint MS936::Holder::initEncoderStatus = 123456789;

        void MS936::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
#include <meta/charset/MS936.db>
                    b2c = CharArray2D(b2cStr.length());
                    for (gint i = 0; i < b2cStr.length(); i++) {
                        if (b2cStr[i].isEmpty())
                            b2c[i] = DoubleByte::B2C_UNMAPPABLE;
                        else
                            b2c[i] = b2cStr[i].toChars();
                    }
                } catch (Throwable const &ex) {
                    Error("Unable to load MS936 Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void MS936::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    loadDecoderData();
                    c2b = CharArray(0x7000);
                    c2bIndex = CharArray(0x100);
                    String b2cNR = {};
                    String c2bNR = {};
                    DoubleByte::Encoder::initC2B(b2cStr, b2cSBStr, b2cNR, c2bNR, 0x40, 0xFE, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load MS936 Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }

    } // charset
} // core
