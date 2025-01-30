//
// Created by bruns on 04/10/2024.
//

#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/ext/IBM949.h>
#include <meta/charset/ext/IBM949C.h>

namespace core {
    namespace charset {
        IBM949C::IBM949C(): Charset("IBM949C"_S, {
                                        "cp949C"_S,
                                        "ibm949C"_S,
                                        "ibm-949C"_S,
                                        "949C"_S
                                    }) {
        }

        gbool IBM949C::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<IBM949C>::hasInstance(cs);
        }

        CharsetDecoder &IBM949C::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, IBM949::Holder::b2c, Holder::b2cSB, 0xA1, 0xFE);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM949C::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharArray IBM949C::Holder::b2cSB = {};
        CharArray IBM949C::Holder::c2b = {};
        CharArray IBM949C::Holder::c2bIndex = {};
        gint IBM949C::Holder::initDecoderStatus = 123456789;
        gint IBM949C::Holder::initEncoderStatus = 123456789;

        void IBM949C::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
                    IBM949::Holder::loadDecoderData();
                    b2cSB = CharArray(0x100);
                    for (gint i = 0; i < 0x80; i++)
                        b2cSB[i] = i;
                    for (gint i = 0x80; i < 0x100; i++)
                        IBM949::Holder::b2cSB[i];
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM949C Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM949C::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    IBM949::Holder::loadEncoderData();
                    c2b = util::Arrays::copyOf(IBM949::Holder::c2b, IBM949::Holder::c2b.length());
                    c2bIndex = util::Arrays::copyOf(IBM949::Holder::c2bIndex, IBM949::Holder::c2bIndex.length());
                    for (gint i = 0; i < 0x80; i++)
                        c2b[c2bIndex[i >> 8] + (i & 0xff)] = i;
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM949C Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
