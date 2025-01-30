//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM866.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM866::IBM866()
            : Charset("IBM866"_S, StringArray::of(
                          "cp866"_S,
                          "ibm866"_S,
                          "ibm-866"_S,
                          "866"_S,
                          "csIBM866"_S)) {
        }

        gbool IBM866::contains(const Charset& cs) const {
            return Class<IBM866>::hasInstance(cs);
        }

        CharsetDecoder& IBM866::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM866::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM866::Holder::b2c = {};
        CharArray IBM866::Holder::c2b = {};
        CharArray IBM866::Holder::c2bIndex = {};
        gint IBM866::Holder::initStatus = 123456789;

        void IBM866::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM866.db>
                    };
                    c2b = CharArray(0x500);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM866 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }

    } // charset
} // core
