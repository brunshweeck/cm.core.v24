//
// Created by brunshweeck on 9 sept. 2024.
//

#include <core/misc/Unsafe.h>
#include <meta/charset/HKSCS_Map.h>
#include <meta/charset/HKSCS_XPMap.h>
#include <meta/charset/MS950_HKSCS_XP.h>

namespace core {
    namespace charset {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        static CharArray2D IGNORE_CA = {};

        MS950_HKSCS_XP::MS950_HKSCS_XP()
            : Charset("x-MS950-HKSCS-XP"_S, StringArray::of("MS950_HKSCS_XP"_S)) {
        }

        gbool MS950_HKSCS_XP::contains(const Charset &cs) const {
            return cs.name().equals("US-ASCII"_S) ||
                   Class<MS950>::hasInstance(cs) ||
                   Class<MS950_HKSCS_XP>::hasInstance(cs);
        }

        CharsetDecoder &MS950_HKSCS_XP::newDecoder() const {
            try {
                loadDecoderData();
                return UNSAFE::newInstance<Decoder>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        CharsetEncoder &MS950_HKSCS_XP::newEncoder() const {
            try {
                loadEncoderData();
                return UNSAFE::newInstance<Encoder>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }



        MS950_HKSCS_XP::Decoder::Decoder(Charset const &cs)
            : CharsetDecoder(cs, 0.5F, 1.0F),
              HKSCS::Decoder(cs, *ms950Decoder, b2cBmp, IGNORE_CA),
              DoubleByte::Decoder(cs, 0.5F, 1.0F, b2c, b2cSB, 0, 0, true){
        }

        gchar MS950_HKSCS_XP::Decoder::decodeDoubleEx(gint b1, gint b2) const {
            return HKSCS::UNMAPPABLE_DECODING;
        }

        CharArray2D MS950_HKSCS_XP::Decoder::b2cBmp = CharArray2D(0x100);

        MS950_HKSCS_XP::Encoder::Encoder(Charset const &cs)
            : CharsetEncoder(cs, 2.0F, 2.0F),
              HKSCS::Encoder(cs, *ms950Encoder, c2bBmp, IGNORE_CA),
              DoubleByte::Encoder(cs, c2b, c2bIndex, true) {
        }

        gint MS950_HKSCS_XP::Encoder::encodeSupp(gint cp) const {
            return HKSCS::UNMAPPABLE_ENCODING;
        }

        CharArray2D MS950_HKSCS_XP::Encoder::c2bBmp{};

        DoubleByte::Decoder *MS950_HKSCS_XP::Decoder::ms950Decoder = {};
        DoubleByte::Encoder *MS950_HKSCS_XP::Encoder::ms950Encoder = {};

        gint MS950_HKSCS_XP::initDecoderStatus = 123456789;
        gint MS950_HKSCS_XP::initEncoderStatus = 123456789;

        void MS950_HKSCS_XP::loadDecoderData() {
            if (initDecoderStatus == 123456789 && initEncoderStatus == 123456789) {
                try {
                    HKSCS_Map::loadData();
                    Decoder::b2cBmp = CharArray2D(0x100);
                    Decoder::initB2C(Decoder::b2cBmp, HKSCS_Map::b2cBmpStr);
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

        void MS950_HKSCS_XP::loadEncoderData() {
            if (initEncoderStatus == 123456789 && initDecoderStatus == 123456789) {
                try {
                    HKSCS_Map::loadData();
                    Encoder::c2bBmp = CharArray2D(0x100);
                    Encoder::initC2B(Encoder::c2bBmp, HKSCS_XPMap::b2cBmpStr, HKSCS_XPMap::pua);
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
