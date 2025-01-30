//
// Created by bruns on 03/10/2024.
//

#include "SecurityException.h"

#include "misc/Unsafe.h"

namespace core {
    SecurityException::SecurityException(String const &message)
        : Throwable(message), RuntimeException(message) {
    }

    SecurityException::SecurityException(String const &message, Throwable const &cause)
        : Throwable(message, cause), RuntimeException(message, cause) {
    }

    SecurityException::SecurityException(Throwable const &cause)
        : SecurityException(cause.toString(), cause) {
    }

    Object &SecurityException::clone() const {
        try {
            return UNSAFE::newInstance<SecurityException>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    void SecurityException::selfThrow() const {
        throw SecurityException(*this);
    }
} // core
