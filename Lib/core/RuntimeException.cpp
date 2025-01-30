//
// Created by brunshweeck on 25 mai 2024.
//

#include <core/RuntimeException.h>
#include <core/misc/Unsafe.h>

namespace core {
    RuntimeException::RuntimeException() {
    }

    RuntimeException::RuntimeException(String const &message) CORE_NOTHROW
        : Throwable(message) {
    }

    RuntimeException::RuntimeException(String const &message, const Throwable &cause)
        : Throwable(message, cause) {
    }

    RuntimeException::RuntimeException(const Throwable &cause) : Throwable(cause.toString(), cause) {
    }

    Object &RuntimeException::clone() const {
        try {
            return UNSAFE::newInstance<RuntimeException>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    void RuntimeException::selfThrow() const {
        throw RuntimeException(*this);
    }
} // core
