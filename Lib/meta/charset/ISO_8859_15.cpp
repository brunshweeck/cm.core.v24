//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/ISO_8859_15.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        ISO_8859_15::ISO_8859_15()
            : Charset("ISO-8859-15"_S, StringArray::of(
                          "ISO_8859-15"_S,
                          "Latin-9"_S,
                          "csISO885915"_S,
                          "8859_15"_S,
                          "ISO-8859-15"_S,
                          "ISO8859_15"_S,
                          "ISO8859-15"_S,
                          "IBM923"_S,
                          "IBM-923"_S,
                          "cp923"_S,
                          "923"_S,
                          "LATIN0"_S,
                          "LATIN9"_S,
                          "L9"_S,
                          "csISOlatin0"_S,
                          "csISOlatin9"_S,
                          "ISO8859_15_FDIS"_S)) {
        }

        gbool ISO_8859_15::contains(const Charset& cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<ISO_8859_15>::hasInstance(cs);
        }

        CharsetDecoder& ISO_8859_15::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& ISO_8859_15::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray ISO_8859_15::Holder::b2c = {};
        CharArray ISO_8859_15::Holder::c2b = {};
        CharArray ISO_8859_15::Holder::c2bIndex = {};
        gint ISO_8859_15::Holder::initStatus = 123456789;

        void ISO_8859_15::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/ISO_8859_15.db>
                    };
                    c2b = CharArray(0x300);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load ISO_8859_15 Encoder/Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
