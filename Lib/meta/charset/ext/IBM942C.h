//
// Created by bruns on 04/10/2024.
//

#ifndef CORE24_IBM942C_H
#define CORE24_IBM942C_H

#include <core/charset/Charset.h>

namespace core {
    namespace charset {
        class IBM942C final : public Charset {
        public:
            CORE_IMPLICIT IBM942C();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &newDecoder() const override;

            CharsetEncoder &newEncoder() const override;

            class Holder final : public Object {
            public:
                // ------------------- Decoder ----------------------- //
                static CharArray b2cSB;

                // ------------------- Encoder ----------------------- //
                static CharArray c2b;
                static CharArray c2bIndex;

                static gint initDecoderStatus;
                static gint initEncoderStatus;

                static void loadDecoderData();

                static void loadEncoderData();
            };
        };
    } // charset
} // core

#endif //CORE24_IBM942C_H
