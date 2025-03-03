//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_MS932_H
#define CORE24_MS932_H

#include <core/charset/Charset.h>

namespace core {
    namespace charset {
        class MS932 final: public Charset {
        public:
            CORE_IMPLICIT MS932();

            gbool contains(const Charset& cs) const override;

            CharsetDecoder& newDecoder() const override;

            CharsetEncoder& newEncoder() const override;

            class Holder final : public Object {
            public:
                // ------------------- Decoder ----------------------- //
                static String b2cSBStr;
                static StringArray b2cStr;
                static CharArray2D b2c;
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

#endif //CORE24_MS932_H
