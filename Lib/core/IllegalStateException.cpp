//
// Created by brunshweeck on 25 mai 2024.
//

#include <core/IllegalStateException.h>
#include <core/misc/Unsafe.h>

namespace core {
    IllegalStateException::IllegalStateException(String const& message)
        : Throwable(message), RuntimeException(message) {}

    IllegalStateException::IllegalStateException(String const& message, const Throwable& cause)
        : Throwable(message, cause), RuntimeException(message, cause) {}

    IllegalStateException::IllegalStateException(const Throwable& cause)
        : Throwable(cause.toString(), cause), RuntimeException(cause) {}

    Object& IllegalStateException::clone() const {
        try {
            return UNSAFE::newInstance<IllegalStateException>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void IllegalStateException::selfThrow() const {
        throw IllegalStateException(*this);
    }
} // core
