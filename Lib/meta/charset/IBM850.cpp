//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM850.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM850::IBM850()
            : Charset("IBM850"_S, StringArray::of("cp850"_S, "ibm-850"_S, "ibm850"_S, "850"_S, "cspc850multilingual"_S)) {
        }

        gbool IBM850::contains(const Charset& cs) const {
            return Class<IBM850>::hasInstance(cs);
        }

        CharsetDecoder& IBM850::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM850::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM850::Holder::b2c = {};
        CharArray IBM850::Holder::c2b = {};
        CharArray IBM850::Holder::c2bIndex = {};
        gint IBM850::Holder::initStatus = 123456789;

        void IBM850::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM850.db>
                    };
                    c2b = CharArray(0x400);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM850 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }

    } // charset
} // core
