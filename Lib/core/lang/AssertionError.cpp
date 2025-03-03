//
// Created by brunshweeck on 11 juin 2024.
//

#include "AssertionError.h"
#include <core/misc/Unsafe.h>

namespace core {
    AssertionError::AssertionError() {
        CORE_IGNORE(this);
    }

    AssertionError::AssertionError(String const& message)
        : Throwable(message), Error(message) {}

    AssertionError::AssertionError(Object const& message)
        : Error(String::valueOf(message),
                !Class<Throwable>::hasInstance(message)
                    ? *this
                    : CORE_XCAST(Throwable const, message)) {}

    AssertionError::AssertionError(gbool message): AssertionError(String::valueOf(message)) {}

    AssertionError::AssertionError(gint message): AssertionError(String::valueOf(message)) {}

    AssertionError::AssertionError(glong message): AssertionError(String::valueOf(message)) {}

    AssertionError::AssertionError(gfloat message): AssertionError(String::valueOf(message)) {}

    AssertionError::AssertionError(gdouble message): AssertionError(String::valueOf(message)) {}

    AssertionError::AssertionError(gchar message): AssertionError(String::valueOf(message)) {}

    AssertionError::AssertionError(String const& message, Throwable const& cause)
        : Throwable(message, cause), Error(message, cause) {}

    AssertionError::AssertionError(Throwable const& cause)
        : AssertionError(cause.toString(), cause) {}

    Object& AssertionError::clone() const {
        try {
            return UNSAFE::newInstance<AssertionError>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void AssertionError::selfThrow() const {
        throw AssertionError(*this);
    }
} // core
