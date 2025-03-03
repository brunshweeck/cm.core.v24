//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_JIS_X_0201_H
#define CORE24_JIS_X_0201_H

#include <core/charset/Charset.h>

namespace core {
    namespace charset {
        class JIS_X_0201 final : public Charset {
        public:
            CORE_IMPLICIT JIS_X_0201();

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

#endif //CORE24_JIS_X_0201_H
