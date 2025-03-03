//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/KOI8_R.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        KOI8_R::KOI8_R()
            : Charset("KOI8-R"_S, StringArray::of(
                          "koi8_r"_S,
                          "koi8"_S,
                          "cskoi8r"_S)) {
        }

        gbool KOI8_R::contains(const Charset& cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<KOI8_R>::hasInstance(cs);
        }

        CharsetDecoder& KOI8_R::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, true, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& KOI8_R::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray KOI8_R::Holder::b2c = {};
        CharArray KOI8_R::Holder::c2b = {};
        CharArray KOI8_R::Holder::c2bIndex = {};
        gint KOI8_R::Holder::initStatus = 123456789;

        void KOI8_R::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/KOI8_R.db>
                    };
                    c2b = CharArray(0x500);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load KOI8_R Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
