//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM862.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM862::IBM862()
            : Charset("IBM862"_S, StringArray::of(
                          "cp862"_S,
                          "ibm862"_S,
                          "ibm-862"_S,
                          "862"_S,
                          "csIBM862"_S,
                          "cspc862latinhebrew"_S)) {
        }

        gbool IBM862::contains(const Charset& cs) const {
            return Class<IBM862>::hasInstance(cs);
        }

        CharsetDecoder& IBM862::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM862::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM862::Holder::b2c = {};
        CharArray IBM862::Holder::c2b = {};
        CharArray IBM862::Holder::c2bIndex = {};
        gint IBM862::Holder::initStatus = 123456789;

        void IBM862::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM862.db>
                    };
                    c2b = CharArray(0x800);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM862 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
