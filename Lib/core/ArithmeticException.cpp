//
// Created by brunshweeck on 25 mai 2024.
//

#include <core/ArithmeticException.h>
#include <core/misc/Unsafe.h>

namespace core {
    Object& ArithmeticException::clone() const {
        try {
            return UNSAFE::newInstance<ArithmeticException>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ArithmeticException::ArithmeticException(String const& message)
        : Throwable(message), RuntimeException(message) {}

    void ArithmeticException::selfThrow() const {
        throw ArithmeticException(*this);
    }
} // core
