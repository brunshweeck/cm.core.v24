//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_MS1255_H
#define CORE24_MS1255_H

#include <core/charset/Charset.h>

namespace core {
    namespace charset {
        class MS1255 final : public Charset {
        public:
            CORE_IMPLICIT MS1255();

            gbool contains(const Charset& cs) const override;

            CharsetDecoder& newDecoder() const override;

            CharsetEncoder& newEncoder() const override;

        private:
            class Holder {
            public:
                static CharArray b2c;
                static CharArray c2b;
                static CharArray c2bIndex;

                static gint initStatus;
                static void loadCharsetData();
            };
        };
    } // charset
} // core

#endif //CORE24_MS1255_H
