//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/IBM437.h>
#include <meta/charset/SingleByte.h>

#include "IBM855.h"

namespace core {
    namespace charset {
        IBM437::IBM437()
            : Charset("IBM437"_S, StringArray::of(
                          "cp437"_S,
                          "ibm437"_S,
                          "ibm-437"_S,
                          "437"_S,
                          "cspc8codepage437"_S,
                          "windows-437"_S)) {
        }

        gbool IBM437::contains(const Charset &cs) const {
            return Class<IBM437>::hasInstance(cs);
        }

        CharsetDecoder &IBM437::newDecoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Decoder>(*this, Holder::b2c, false, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM437::newEncoder() const {
            try {
                Holder::loadCharsetData();
                return UNSAFE::newInstance<SingleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex, false);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharArray IBM437::Holder::b2c = {};
        CharArray IBM437::Holder::c2b = {};
        CharArray IBM437::Holder::c2bIndex = {};
        gint IBM437::Holder::initStatus = 123456789;

        void IBM437::Holder::loadCharsetData() {
            if (initStatus == 123456789) {
                try {
                    b2c = {
                        #include <meta/charset/IBM437.db>
                    };
                    c2b = CharArray(0x700);
                    c2bIndex = CharArray(0x100);

                    CharArray b2cMap = b2c;
                    CharArray c2bNR = {};
                    SingleByte::initC2B(b2cMap, c2bNR, c2b, c2bIndex);
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM437 Encoder/Decoder table"_S, ex).throws($ftrace());
                }
                initStatus = 123454321;
            }

            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
