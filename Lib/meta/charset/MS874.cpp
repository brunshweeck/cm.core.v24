//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/MS874.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        MS874::MS874()
            : Charset("x-windows-874"_S, StringArray::of(
                          "ms874"_S,
                          "ms-874"_S,
                          "windows-874"_S)) {
        }

        gbool MS874::contains(const Charset& cs) const {
            return Class<MS874>::hasInstance(cs);
        }

        CharsetDecoder& MS874::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& MS874::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray MS874::Holder::b2c = {};
        CharArray MS874::Holder::c2b = {};
        CharArray MS874::Holder::c2bIndex = {};
        gint MS874::Holder::initStatus = 123456789;

        void MS874::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/MS874.db>
                    };
                    c2b = CharArray(0x400);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load MS874 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
