//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_UTF_32LE_H
#define CORE24_UTF_32LE_H

#include <meta/charset/Unicode.h>

namespace core {
    namespace charset {
        class UTF_32LE final: public Unicode {
        public:
            CORE_IMPLICIT UTF_32LE();

            CharsetDecoder& newDecoder() const override;

            CharsetEncoder& newEncoder() const override;
        };
    } // charset
} // core

#endif //CORE24_UTF_32LE_H
