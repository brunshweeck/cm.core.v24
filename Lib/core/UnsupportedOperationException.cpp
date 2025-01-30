//
// Created by brunshweeck on 6 juin 2024.
//

#include <core/UnsupportedOperationException.h>
#include <core/misc/Unsafe.h>

namespace core {
    UnsupportedOperationException::UnsupportedOperationException(String const& message)
        : Throwable(message), RuntimeException(message) {}

    UnsupportedOperationException::UnsupportedOperationException(String const& message, Throwable const& cause)
        : Throwable(message, cause), RuntimeException(message, cause) {}

    UnsupportedOperationException::UnsupportedOperationException(Throwable const& cause)
        : UnsupportedOperationException(cause.toString(), cause) {}

    Object& UnsupportedOperationException::clone() const {
        try {
            return UNSAFE::newInstance<UnsupportedOperationException>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void UnsupportedOperationException::selfThrow() const {
        throw UnsupportedOperationException(*this);
    }
} // core
