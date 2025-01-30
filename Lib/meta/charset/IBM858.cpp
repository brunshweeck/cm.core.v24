//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM858.h>
#include <meta/charset/SingleByte.h>

namespace core {
    namespace charset {
        IBM858::IBM858()
            : Charset("IBM858"_S, StringArray::of(
                          "cp858"_S,
                          "ccsid00858"_S,
                          "cp00858"_S,
                          "858"_S,
                          "PC-Multilingual-850+euro"_S,
                          "ibm858"_S,
                          "ibm-858"_S)) {
        }

        gbool IBM858::contains(const Charset& cs) const {
            return Class<IBM858>::hasInstance(cs);
        }

        CharsetDecoder& IBM858::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder& IBM858::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharArray IBM858::Holder::b2c = {};
        CharArray IBM858::Holder::c2b = {};
        CharArray IBM858::Holder::c2bIndex = {};
        gint IBM858::Holder::initStatus = 123456789;

        void IBM858::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
#include <meta/charset/IBM858.db>
                    };
                    c2b = CharArray(0x400);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM858 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
