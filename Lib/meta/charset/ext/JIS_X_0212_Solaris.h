//
// Created by bruns on 04/10/2024.
//

#ifndef CORE24_JIS_X_0212_SOLARIS_H
#define CORE24_JIS_X_0212_SOLARIS_H

#include <core/charset/Charset.h>

namespace core {
    namespace charset {
        class JIS_X_0212_Solaris final : public Charset {
        public:
            CORE_IMPLICIT JIS_X_0212_Solaris();

            gbool contains(const Charset &cs) const override;

            CharsetDecoder &newDecoder() const override;

            CharsetEncoder &newEncoder() const override;

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

#endif //CORE24_JIS_X_0212_SOLARIS_H
