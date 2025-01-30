//
// Created by brunshweeck on 25 mai 2024.
//

#include <core/IndexOutOfBoundsException.h>
#include <core/Integer.h>
#include <core/Long.h>
#include <core/misc/Unsafe.h>

namespace core {
    IndexOutOfBoundsException::IndexOutOfBoundsException(String const& message)
        : Throwable(message), RuntimeException(message) {}

    IndexOutOfBoundsException::IndexOutOfBoundsException(gint index)
        : IndexOutOfBoundsException(Integer::toString(index)) {}

    IndexOutOfBoundsException::IndexOutOfBoundsException(glong index)
        : IndexOutOfBoundsException(Long::toString(index)) {}

    Object& IndexOutOfBoundsException::clone() const {
        try {
            return UNSAFE::newInstance<IndexOutOfBoundsException>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void IndexOutOfBoundsException::selfThrow() const {
        throw IndexOutOfBoundsException(*this);
    }
} // core
