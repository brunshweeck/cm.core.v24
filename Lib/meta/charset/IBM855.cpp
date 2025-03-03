//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM855.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM855::IBM855()
            : Charset("IBM855"_S, StringArray::of("cp855"_S, "ibm-855"_S, "ibm855"_S, "855"_S, "cspcp855"_S)) {
        }

        gbool IBM855::contains(const Charset& cs) const {
            return Class<IBM855>::hasInstance(cs);
        }

        CharsetDecoder& IBM855::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM855::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM855::Holder::b2c = {};
        CharArray IBM855::Holder::c2b = {};
        CharArray IBM855::Holder::c2bIndex = {};
        gint IBM855::Holder::initStatus = 123456789;

        void IBM855::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM855.db>
                    };
                    c2b = CharArray(0x400);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM855 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }


    } // charset
} // core
