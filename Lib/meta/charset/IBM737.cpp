//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM737.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM737::IBM737()
            : Charset("IBM737"_S, StringArray::of("cp737"_S, "ibm737"_S, "ibm-737"_S, "737"_S)) {
        }

        gbool IBM737::contains(const Charset& cs) const {
            return Class<IBM737>::hasInstance(cs);
        }

        CharsetDecoder& IBM737::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM737::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM737::Holder::b2c = {};
        CharArray IBM737::Holder::c2b = {};
        CharArray IBM737::Holder::c2bIndex = {};
        gint IBM737::Holder::initStatus = 123456789;

        void IBM737::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM737.db>
                    };
                    c2b = CharArray(0x500);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM737 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }

    } // charset
} // core
