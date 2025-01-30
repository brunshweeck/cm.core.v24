//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/HKSCS_Map.h>
#include <meta/charset/MS950_HKSCS.h>

#include "MS950_HKSCS_XP.h"

namespace core {
    namespace charset {

        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        MS950_HKSCS::MS950_HKSCS()
            : Charset("x-MS950-HKSCS"_S, StringArray::of("MS950_HKSCS"_S)) {
        }

        gbool MS950_HKSCS::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) ||
                   Class<MS950>::hasInstance(cs) ||
                   Class<MS950_HKSCS>::hasInstance(cs);
        }

        CharsetDecoder &MS950_HKSCS::newDecoder() const {
            try {
                loadDecoderData();
                return UNSAFE::newInstance<Decoder>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &MS950_HKSCS::newEncoder() const {
            try {
                loadEncoderData();
                return UNSAFE::newInstance<Encoder>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        MS950_HKSCS::Decoder::Decoder(Charset const &cs)
            : CharsetDecoder(cs, 0.5F, 1.0F),
              HKSCS::Decoder(cs, *ms950Decoder, b2cBmp, b2cSupp),
              DoubleByte::Decoder(cs, 0.5, 1.0F, b2c, b2cSB, 0, 0, true){
        }

        DoubleByte::Decoder *MS950_HKSCS::Decoder::ms950Decoder = {};
        CharArray2D MS950_HKSCS::Decoder::b2cBmp = {};
        CharArray2D MS950_HKSCS::Decoder::b2cSupp = {};

        MS950_HKSCS::Encoder::Encoder(Charset const &cs)
            : CharsetEncoder(cs, 2.0F, 2.0F),
              HKSCS::Encoder(cs, *ms950Encoder, c2bBmp, c2bSupp),
              DoubleByte::Encoder(cs, HKSCS::Encoder::c2b, HKSCS::Encoder::c2bIndex, true) {
        }

        DoubleByte::Encoder *MS950_HKSCS::Encoder::ms950Encoder = {};
        CharArray2D MS950_HKSCS::Encoder::c2bBmp = {};
        CharArray2D MS950_HKSCS::Encoder::c2bSupp = {};

        gint MS950_HKSCS::initDecoderStatus = 123456789;
        gint MS950_HKSCS::initEncoderStatus = 123456789;

        void MS950_HKSCS::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
                    HKSCS_Map::loadData();
                    Decoder::b2cBmp = CharArray2D(0x100);
                    Decoder::b2cSupp = CharArray2D(0x100);
                    Decoder::initB2C(Decoder::b2cBmp, HKSCS_Map::b2cBmpStr);
                    Decoder::initB2C(Decoder::b2cSupp, HKSCS_Map::b2cSuppStr);
                } catch (Throwable const &ex) {
                    Error("Unable to load x-MS950-HKSCS Decoder table"_S, ex).throws($ftrace());
                }
                initDecoderStatus = 123454321;
            }
            if (Decoder::ms950Decoder == null) {
                MS950 &ms950 = UNSAFE::newInstance<MS950>();
                Decoder::ms950Decoder = CORE_DCAST(DoubleByte::Decoder *, &ms950.newDecoder());
                CORE_ASSERT(Decoder::ms950Decoder != null);
            }
            CORE_ASSERT(initDecoderStatus == 123454321);
        }

        void MS950_HKSCS::loadEncoderData() {
            if (initEncoderStatus == 123456789 && initDecoderStatus == 123456789) {
                try {
                    HKSCS_Map::loadData();
                    Encoder::c2bBmp = CharArray2D(0x100);
                    Encoder::c2bSupp = CharArray2D(0x100);
                    Encoder::initC2B(Encoder::c2bBmp, HKSCS_Map::b2cBmpStr, HKSCS_Map::pua);
                    String pua = {};
                    Encoder::initC2B(Encoder::c2bSupp, HKSCS_Map::b2cSuppStr, pua);
                } catch (Throwable const &ex) {
                    Error("Unable to load x-MS950-HKSCS Encoder table"_S, ex).throws($ftrace());
                }
                initEncoderStatus = 123454321;
            }
            if (Encoder::ms950Encoder == null) {
                MS950 &ms950 = UNSAFE::newInstance<MS950>();
                Encoder::ms950Encoder = CORE_DCAST(DoubleByte::Encoder *, &ms950.newEncoder());
                CORE_ASSERT(Encoder::ms950Encoder != null);
            }
            CORE_ASSERT(initEncoderStatus == 123454321);
        }

        CORE_WARNING_POP
    } // charset
} // core
