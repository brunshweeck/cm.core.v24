//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/ISO_8859_9.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        ISO_8859_9::ISO_8859_9()
            : Charset("ISO-8859-9"_S, StringArray::of(
                          "iso8859_9"_S,
                          "8859_9"_S,
                          "iso-ir-148"_S,
                          "ISO_8859-9"_S,
                          "ISO_8859-9:1989"_S,
                          "ISO8859-9"_S,
                          "latin5"_S,
                          "l5"_S,
                          "ibm920"_S,
                          "ibm-920"_S,
                          "920"_S,
                          "cp920"_S,
                          "csISOLatin5"_S)) {
        }

        gbool ISO_8859_9::contains(const Charset& cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<ISO_8859_9>::hasInstance(cs);
        }

        CharsetDecoder& ISO_8859_9::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& ISO_8859_9::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray ISO_8859_9::Holder::b2c = {};
        CharArray ISO_8859_9::Holder::c2b = {};
        CharArray ISO_8859_9::Holder::c2bIndex = {};
        gint ISO_8859_9::Holder::initStatus = 123456789;

        void ISO_8859_9::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/ISO_8859_9.db>
                    };
                    c2b = CharArray(0x200);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load ISO_8859_9 Encoder/Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
