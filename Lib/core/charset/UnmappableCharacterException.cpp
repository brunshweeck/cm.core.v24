//
// Created by brunshweeck on 1 sept. 2024.
//

#include <core/charset/UnmappableCharacterException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace charset {
        UnmappableCharacterException::UnmappableCharacterException(gint inputLength)
            : Exception("Input length: "_Sl + inputLength),
              Throwable("Input length: "_Sl + inputLength),
              inputLength(inputLength) {}

        gint UnmappableCharacterException::length() const {
            return inputLength;
        }

        Object& UnmappableCharacterException::clone() const {
            try {
                return UNSAFE::newInstance<UnmappableCharacterException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void UnmappableCharacterException::selfThrow() const {
            throw UnmappableCharacterException(*this);
        }
    } // charset
} // core
