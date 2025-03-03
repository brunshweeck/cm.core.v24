//
// Created by bruns on 04/10/2024.
//

#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>
#include <meta/charset/DoubleByte.h>
#include <meta/charset/ext/IBM942.h>
#include <meta/charset/ext/IBM942C.h>

namespace core {
    namespace charset {
        IBM942C::IBM942C(): Charset("x-IBM942C"_S, {
                                        "cp942C"_S,
                                        "ibm942C"_S,
                                        "ibm-942C"_S,
                                        "942C"_S,
                                        "cp932"_S,
                                        "ibm932"_S,
                                        "ibm-932"_S,
                                        "932"_S,
                                        "x-ibm932"_S
                                    }) {
        }

        gbool IBM942C::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) || Class<IBM942C>::hasInstance(cs);
        }

        CharsetDecoder &IBM942C::newDecoder() const {
            try {
                Holder::loadDecoderData();
                return UNSAFE::newInstance<DoubleByte::Decoder>(*this, IBM942::Holder::b2c, Holder::b2cSB, 0x40, 0xFC);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &IBM942C::newEncoder() const {
            try {
                Holder::loadEncoderData();
                return UNSAFE::newInstance<DoubleByte::Encoder>(*this, Holder::c2b, Holder::c2bIndex);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharArray IBM942C::Holder::b2cSB = {};
        CharArray IBM942C::Holder::c2b = {};
        CharArray IBM942C::Holder::c2bIndex = {};
        gint IBM942C::Holder::initDecoderStatus = 123456789;
        gint IBM942C::Holder::initEncoderStatus = 123456789;

        void IBM942C::Holder::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
                    IBM942::Holder::loadDecoderData();
                    // the mappings that need updating are
                    //    u+001a  <-> 0x1a
                    //    u+001c  <-> 0x1c
                    //    u+005c  <-> 0x5c
                    //    u+007e  <-> 0x7e
                    //    u+007f  <-> 0x7f
                    b2cSB = util::Arrays::copyOf(IBM942::Holder::b2cSB, IBM942::Holder::b2cSB.length());
                    b2cSB[0x1a] = 0x1a;
                    b2cSB[0x1c] = 0x1c;
                    b2cSB[0x5c] = 0x5c;
                    b2cSB[0x7e] = 0x7e;
                    b2cSB[0x7f] = 0x7f;
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM942C Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void IBM942C::Holder::loadEncoderData() {
            if (initEncoderStatus == 123456789) {
                try {
                    IBM942::Holder::loadEncoderData();
                    c2b = util::Arrays::copyOf(IBM942::Holder::c2b, IBM942::Holder::c2b.length());
                    c2bIndex = util::Arrays::copyOf(IBM942::Holder::c2bIndex, IBM942::Holder::c2bIndex.length());
                    c2b[c2bIndex[0] + 0x1a] = 0x1a;
                    c2b[c2bIndex[0] + 0x1c] = 0x1c;
                    c2b[c2bIndex[0] + 0x5c] = 0x5c;
                    c2b[c2bIndex[0] + 0x7e] = 0x7e;
                    c2b[c2bIndex[0] + 0x7f] = 0x7f;
                } catch (Throwable const &ex) {
                    Error("Unable to load IBM942C Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }
    } // charset
} // core
