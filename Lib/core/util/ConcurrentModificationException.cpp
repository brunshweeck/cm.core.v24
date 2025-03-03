//
// Created by brunshweeck on 2 juil. 2024.
//

#include <core/util/ConcurrentModificationException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace util {
        ConcurrentModificationException::ConcurrentModificationException(String const& message)
            : Throwable(message), RuntimeException(message) {}

        ConcurrentModificationException::ConcurrentModificationException(String const& message,
                                                                         Throwable const& cause)
            : Throwable(message, cause), RuntimeException(message, cause) {}

        ConcurrentModificationException::ConcurrentModificationException(Throwable const& cause)
            : ConcurrentModificationException(cause.toString(), cause) {}

        Object& ConcurrentModificationException::clone() const {
            try {
                return UNSAFE::newInstance<ConcurrentModificationException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void ConcurrentModificationException::selfThrow() const {
            throw ConcurrentModificationException(*this);
        }
    } // util
} // core
