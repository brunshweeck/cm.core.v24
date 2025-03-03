//
// Created by brunshweeck on 21 juin 2024.
//

#include <core/util/NoSuchElementException.h>

#include <core/misc/Unsafe.h>

namespace core {
    namespace util {
        NoSuchElementException::NoSuchElementException(String const& message)
            : Throwable(message), RuntimeException(message) {}

        NoSuchElementException::NoSuchElementException(String const& message, Throwable const& cause)
            : Throwable(message, cause), RuntimeException(message, cause) {}

        NoSuchElementException::NoSuchElementException(Throwable const& cause)
            : NoSuchElementException(cause.toString(), cause) {}

        Object& NoSuchElementException::clone() const {
            try {
                return UNSAFE::newInstance<NoSuchElementException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void NoSuchElementException::selfThrow() const {
            throw NoSuchElementException(*this);
        }
    } // util
} // core
