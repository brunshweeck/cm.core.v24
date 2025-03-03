//
// Created by brunshweeck on 25 mai 2024.
//

#include <core/lang/NumberFormatException.h>
#include <core/misc/Unsafe.h>

namespace core {
    NumberFormatException::NumberFormatException(String const& message)
        : Throwable(message), RuntimeException(message) {}

    Object& NumberFormatException::clone() const {
        try {
            return UNSAFE::newInstance<NumberFormatException>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void NumberFormatException::selfThrow() const {
        throw NumberFormatException(*this);
    }
} // core
