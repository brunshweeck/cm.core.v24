//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM874.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM874::IBM874()
            : Charset("IBM874"_S, StringArray::of("cp874"_S, "ibm874"_S, "ibm-874"_S, "874"_S)) {
        }

        gbool IBM874::contains(const Charset& cs) const {
            return Class<IBM874>::hasInstance(cs);
        }

        CharsetDecoder& IBM874::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM874::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM874::Holder::b2c = {};
        CharArray IBM874::Holder::c2b = {};
        CharArray IBM874::Holder::c2bIndex = {};
        gint IBM874::Holder::initStatus = 123456789;

        void IBM874::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM874.db>
                    };
                    c2b = CharArray(0x400);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    // remove non-roundtrip entries
                    b2cMap[32] = SingleByte::UNMAPPABLE_DECODING;
                    b2cMap[91] = SingleByte::UNMAPPABLE_DECODING;
                    b2cMap[92] = SingleByte::UNMAPPABLE_DECODING;
                    b2cMap[93] = SingleByte::UNMAPPABLE_DECODING;
                    b2cMap[94] = SingleByte::UNMAPPABLE_DECODING;
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM874 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }

    } // charset
} // core
