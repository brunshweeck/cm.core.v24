//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_UTF_16_H
#define CORE24_UTF_16_H

#include <meta/charset/Unicode.h>

namespace core {
    namespace charset {
        class UTF_16 final: public Unicode {
        public:
            CORE_IMPLICIT UTF_16();

            CharsetDecoder& newDecoder() const override;

            CharsetEncoder& newEncoder() const override;
        };
    } // charset
} // core

#endif //CORE24_UTF_16_H
