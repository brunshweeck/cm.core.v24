//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM857.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM857::IBM857()
            : Charset("IBM857"_S, StringArray::of( "cp857"_S, "ibm857"_S, "ibm-857"_S, "857"_S, "csIBM857"_S)) {
        }

        gbool IBM857::contains(const Charset& cs) const {
            return Class<IBM857>::hasInstance(cs);
        }

        CharsetDecoder& IBM857::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM857::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM857::Holder::b2c = {};
        CharArray IBM857::Holder::c2b = {};
        CharArray IBM857::Holder::c2bIndex = {};
        gint IBM857::Holder::initStatus = 123456789;

        void IBM857::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM857.db>
                    };
                    c2b = CharArray(0x400);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM857 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }

    } // charset
} // core
