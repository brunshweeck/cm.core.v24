//
// Created by brunshweeck on 6 sept. 2024.
//

#ifndef CORE24_UNICODE_H
#define CORE24_UNICODE_H

#include <core/charset/Charset.h>

namespace core {
    namespace charset {
        class Unicode: public Charset {
        public:
            CORE_EXPLICIT Unicode(String canonicalName, StringArray aliases);

            gbool contains(const Charset &cs) const override;
        };
    } // charset
} // core

#endif //CORE24_UNICODE_H
