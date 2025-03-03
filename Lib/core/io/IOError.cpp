//
// Created by brunshweeck on 15 sept. 2024.
//

#include "IOError.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        IOError::IOError(Throwable const &cause)
                : Throwable(cause.toString(), cause), Error(cause) {}

        Object &IOError::clone() const {
            try {
                return UNSAFE::newInstance<IOError>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void IOError::selfThrow() const { throw IOError(*this); }
    } // io
} // core
