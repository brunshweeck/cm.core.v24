//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/ISO_8859_4.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        ISO_8859_4::ISO_8859_4()
            : Charset("ISO-8859-4"_S, StringArray::of(
                          "iso8859_4"_S,
                          "iso8859-4"_S,
                          "8859_4"_S,
                          "iso-ir-110"_S,
                          "ISO_8859-4"_S,
                          "ISO_8859-4:1988"_S,
                          "latin4"_S,
                          "l4"_S,
                          "ibm914"_S,
                          "ibm-914"_S,
                          "cp914"_S,
                          "914"_S,
                          "csISOLatin4"_S)) {
        }

        gbool ISO_8859_4::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<ISO_8859_4>::hasInstance(cs);
        }

        CharsetDecoder &ISO_8859_4::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &ISO_8859_4::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharArray ISO_8859_4::Holder::b2c = {};
        CharArray ISO_8859_4::Holder::c2b = {};
        CharArray ISO_8859_4::Holder::c2bIndex = {};
        gint ISO_8859_4::Holder::initStatus = 123456789;

        void ISO_8859_4::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/ISO_8859_4.db>
                    };
                    c2b = CharArray(0x300);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load ISO_8859_4 Encoder/Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
