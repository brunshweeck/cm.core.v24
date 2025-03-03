//
// Created by brunshweeck on 15 sept. 2024.
//

#ifndef CORE24_URL_H
#define CORE24_URL_H

#include <core/lang/Object.h>

namespace core {
    namespace net {
        class URL: public Object {

        public:
            static URL of(URI const &uri);
        };
    } // net
} // core

#endif //CORE24_URL_H
