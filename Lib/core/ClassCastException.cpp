//
// Created by brunshweeck on 13 juin 2024.
//

#include <core/ClassCastException.h>
#include <core/misc/Unsafe.h>

namespace core {
    ClassCastException::ClassCastException(String const& message)
        : Throwable(message), RuntimeException(message) {}

    Object& ClassCastException::clone() const {
        try {
            return UNSAFE::newInstance<ClassCastException>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ClassCastException::selfThrow() const {
        throw ClassCastException(*this);
    }
} // core
