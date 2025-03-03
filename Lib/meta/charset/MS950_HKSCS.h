//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_MS950_HKSCS_H
#define CORE24_MS950_HKSCS_H

#include <core/charset/Charset.h>
#include <meta/charset/HKSCS.h>
#include <meta/charset/MS950.h>

namespace core {
    namespace charset {
        class MS950_HKSCS final : public Charset {
        public:
            CORE_IMPLICIT MS950_HKSCS();

            gbool contains(const Charset& cs) const override;

            CharsetDecoder& newDecoder() const override;

            CharsetEncoder& newEncoder() const override;

            static gint initDecoderStatus;
            static gint initEncoderStatus;
            static void loadDecoderData();
            static void loadEncoderData();

            class Decoder final: public HKSCS::Decoder {
            public:
                static DoubleByte::Decoder *ms950Decoder;
                static CharArray2D b2cBmp;
                static CharArray2D b2cSupp;

                CORE_EXPLICIT Decoder(Charset const& cs);
            };

            class Encoder final: public HKSCS::Encoder {
            public:
                static DoubleByte::Encoder *ms950Encoder;
                static CharArray2D c2bBmp;
                static CharArray2D c2bSupp;


                CORE_EXPLICIT Encoder(Charset const& cs);
            };
        };
    } // charset
} // core

#endif //CORE24_MS950_HKSCS_H
