//
// Created by bruns on 04/10/2024.
//

#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/ext/IBM943.h>
#include <meta/charset/ext/IBM943C.h>

namespace core {
    namespace charset {
        IBM943C::IBM943C(): Charset("x-IBM943C"_S, {
                                        "cp943C"_S,
                                        "ibm943C"_S,
                                        "ibm-943C"_S,
                                        "943C"_S
                                    }) {
        }

        gbool IBM943C::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<IBM943C>::hasInstance(cs);
        }

        CharsetDecoder &IBM943C::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, IBM943::Holder::b2c, Holder::b2cSB, 0x40, 0xFC);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM943C::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharArray IBM943C::Holder::b2cSB = {};
        CharArray IBM943C::Holder::c2b = {};
        CharArray IBM943C::Holder::c2bIndex = {};
        gint IBM943C::Holder::initDecoderStatus = 123456789;
        gint IBM943C::Holder::initEncoderStatus = 123456789;

        void IBM943C::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
                    IBM943::Holder::loadDecoderData();
                    b2cSB = CharArray(0x100);
                    for (gint i = 0; i < 0x80; i++)
                        b2cSB[i] = i;
                    for (gint i = 0x80; i < 0x100; i++)
                        IBM943::Holder::b2cSB[i];
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM943C Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM943C::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    IBM943::Holder::loadEncoderData();
                    c2b = util::Arrays::copyOf(IBM943::Holder::c2b, IBM943::Holder::c2b.length());
                    c2bIndex = util::Arrays::copyOf(IBM943::Holder::c2bIndex, IBM943::Holder::c2bIndex.length());
                    for (gint i = 0; i < 0x80; i++)
                        c2b[c2bIndex[i >> 8] + (i & 0xff)] = i;
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM943C Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
