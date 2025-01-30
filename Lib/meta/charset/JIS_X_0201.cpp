//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/JIS_X_0201.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        JIS_X_0201::JIS_X_0201()
            : Charset("JIS_X0201"_S, StringArray::of(
                          "JIS0201"_S,
                          "JIS_X0201"_S,
                          "X0201"_S,
                          "csHalfWidthKatakana"_S)) {
        }

        gbool JIS_X_0201::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<JIS_X_0201>::hasInstance(cs);
        }

        CharsetDecoder &JIS_X_0201::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &JIS_X_0201::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharArray JIS_X_0201::Holder::b2c = {};
        CharArray JIS_X_0201::Holder::c2b = {};
        CharArray JIS_X_0201::Holder::c2bIndex = {};
        gint JIS_X_0201::Holder::initStatus = 123456789;

        void JIS_X_0201::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/JIS_X_0201.db>
                    };
                    c2b = CharArray(0x300);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    // non-roundtrip c2b only entries
                    c2bNR = CharArray(4);
                    c2bNR[0] = 0x007E; c2bNR[1] = 0x203E;
                    c2bNR[2] = 0x005C; c2bNR[3] = 0x00A5;
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load JIS_X_0201 Encoder/Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
