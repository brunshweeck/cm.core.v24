//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM775.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM775::IBM775()
            : Charset("IBM775"_S, StringArray::of("cp775"_S, "ibm775"_S, "ibm-775"_S, "775"_S)) {
        }

        gbool IBM775::contains(const Charset& cs) const {
            return Class<IBM775>::hasInstance(cs);
        }

        CharsetDecoder& IBM775::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM775::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM775::Holder::b2c = {};
        CharArray IBM775::Holder::c2b = {};
        CharArray IBM775::Holder::c2bIndex = {};
        gint IBM775::Holder::initStatus = 123456789;

        void IBM775::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM775.db>
                    };
                    c2b = CharArray(0x500);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM775 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }

    } // charset
} // core
