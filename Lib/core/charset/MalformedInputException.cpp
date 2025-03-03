//
// Created by brunshweeck on 1 sept. 2024.
//

#include <core/charset/MalformedInputException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace charset {
        MalformedInputException::MalformedInputException(gint inputLength)
            : Exception("Input length: "_Sl + inputLength),
              Throwable("Input length: "_Sl + inputLength),
              inputLength(inputLength) {}

        gint MalformedInputException::length() const {
            return inputLength;
        }

        Object& MalformedInputException::clone() const {
            try {
                return UNSAFE::newInstance<MalformedInputException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void MalformedInputException::selfThrow() const {
            throw MalformedInputException(*this);
        }
    } // charset
} // core
