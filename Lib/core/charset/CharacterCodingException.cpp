//
// Created by brunshweeck on 1 sept. 2024.
//

#include "CharacterCodingException.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace charset {
        Object& CharacterCodingException::clone() const {
            try {
                return UNSAFE::newInstance<CharacterCodingException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void CharacterCodingException::selfThrow() const { throw CharacterCodingException(*this); }
    } // text
} // core
