//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/ISO_8859_16.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        ISO_8859_16::ISO_8859_16()
            : Charset("ISO-8859-16"_S, StringArray::of(
                          "iso-ir-226"_S,
                          "ISO_8859-16:2001"_S,
                          "ISO_8859-16"_S,
                          "ISO8859_16"_S,
                          "latin10"_S,
                          "l10"_S,
                          "csISO885916"_S)) {
        }

        gbool ISO_8859_16::contains(const Charset& cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<ISO_8859_16>::hasInstance(cs);
        }

        CharsetDecoder& ISO_8859_16::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& ISO_8859_16::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray ISO_8859_16::Holder::b2c = {};
        CharArray ISO_8859_16::Holder::c2b = {};
        CharArray ISO_8859_16::Holder::c2bIndex = {};
        gint ISO_8859_16::Holder::initStatus = 123456789;

        void ISO_8859_16::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/ISO_8859_16.db>
                    };
                    c2b = CharArray(0x400);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load ISO_8859_16 Encoder/Decoder table"_S, ex).throws(
                        $ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
