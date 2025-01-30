//
// Created by bruns on 01/10/2024.
//

#include <core/concurrent/RejectedExecutionException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace concurrent {
        RejectedExecutionException::RejectedExecutionException(String const& message)
            : Throwable(message), RuntimeException(message) {}

        RejectedExecutionException::RejectedExecutionException(String const& message, Throwable const& cause)
            : Throwable(message, cause), RuntimeException(message, cause) {}

        RejectedExecutionException::RejectedExecutionException(Throwable const& cause)
            : RejectedExecutionException(cause.toString(), cause) {}

        Object& RejectedExecutionException::clone() const {
            try {
                return UNSAFE::newInstance<RejectedExecutionException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void RejectedExecutionException::selfThrow() const {
            throw RejectedExecutionException(*this);
        }
    } // concurrent
} // core
