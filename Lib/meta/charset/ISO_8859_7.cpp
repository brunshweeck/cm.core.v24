//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/ISO_8859_7.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        ISO_8859_7::ISO_8859_7()
            : Charset("ISO-8859-7"_S, StringArray::of(
                          "iso8859_7"_S,
                          "8859_7"_S,
                          "iso-ir-126"_S,
                          "ISO_8859-7"_S,
                          "ISO_8859-7:1987"_S,
                          "ELOT_928"_S,
                          "ECMA-118"_S,
                          "greek"_S,
                          "greek8"_S,
                          "csISOLatinGreek"_S,
                          "sun_eu_greek"_S,
                          "ibm813"_S,
                          "ibm-813"_S,
                          "813"_S,
                          "cp813"_S,
                          "iso8859-7"_S)) {
        }

        gbool ISO_8859_7::contains(const Charset& cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<ISO_8859_7>::hasInstance(cs);
        }

        CharsetDecoder& ISO_8859_7::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& ISO_8859_7::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray ISO_8859_7::Holder::b2c = {};
        CharArray ISO_8859_7::Holder::c2b = {};
        CharArray ISO_8859_7::Holder::c2bIndex = {};
        gint ISO_8859_7::Holder::initStatus = 123456789;

        void ISO_8859_7::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/ISO_8859_7.db>
                    };
                    c2b = CharArray(0x400);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load ISO_8859_7 Encoder/Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
