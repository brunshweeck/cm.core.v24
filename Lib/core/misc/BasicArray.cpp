//
// Created by brunshweeck on 4 sept. 2024.
//

#include <core/misc/BasicArray.h>

namespace core {
    namespace misc {
        BasicArray::BasicArray() {
            CORE_IGNORE(this);
        }

        gbool BasicArray::isEmpty() const {
            return length() == 0;
        }
    } // misc
} // core
