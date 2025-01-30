//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/MS1257.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        MS1257::MS1257()
            : Charset("windows-1257"_S, StringArray::of("cp1257"_S, "cp5353"_S)) {
        }

        gbool MS1257::contains(const Charset& cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<MS1257>::hasInstance(cs);
        }

        CharsetDecoder& MS1257::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& MS1257::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray MS1257::Holder::b2c = {};
        CharArray MS1257::Holder::c2b = {};
        CharArray MS1257::Holder::c2bIndex = {};
        gint MS1257::Holder::initStatus = 123456789;

        void MS1257::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/MS1257.db>
                    };
                    c2b = CharArray(0x600);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load MS1257 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
