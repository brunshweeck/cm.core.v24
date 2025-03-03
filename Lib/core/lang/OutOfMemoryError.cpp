//
// Created by brunshweeck on 25 mai 2024.
//

#include "OutOfMemoryError.h"
#include <core/misc/Unsafe.h>

namespace core {
    OutOfMemoryError::OutOfMemoryError(String const& message)
        : Throwable(message), Error(message) {}

    Object& OutOfMemoryError::clone() const {
        try {
            return UNSAFE::newInstance<OutOfMemoryError>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void OutOfMemoryError::selfThrow() const {
        throw OutOfMemoryError(*this);
    }
} // core
