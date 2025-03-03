//
// Created by brunshweeck on 13 juin 2024.
//

#include "IllegalThreadStateException.h"
#include <core/misc/Unsafe.h>

namespace core {
    IllegalThreadStateException::IllegalThreadStateException(String const& message)
        : Throwable(message), RuntimeException(message) {}

    Object& IllegalThreadStateException::clone() const {
        try {
            return UNSAFE::newInstance<IllegalThreadStateException>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void IllegalThreadStateException::selfThrow() const {
        throw IllegalThreadStateException(*this);
    }
} // core
