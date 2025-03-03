//
// Created by brunshweeck on 25 mai 2024.
//

#include "IllegalArgumentException.h"
#include <core/misc/Unsafe.h>

namespace core {
    IllegalArgumentException::IllegalArgumentException(String const& message)
        : Throwable(message), RuntimeException(message) {}

    IllegalArgumentException::IllegalArgumentException(String const& message, const Throwable& cause)
        : Throwable(message, cause), RuntimeException(message, cause) {}

    IllegalArgumentException::IllegalArgumentException(const Throwable& cause)
        : Throwable(cause.toString(), cause), RuntimeException(cause) {}

    Object& IllegalArgumentException::clone() const {
        try {
            return UNSAFE::newInstance<IllegalArgumentException>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void IllegalArgumentException::selfThrow() const {
        throw IllegalArgumentException(*this);
    }
} // core
