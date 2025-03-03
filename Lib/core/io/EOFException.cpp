//
// Created by bruns on 13/10/2024.
//

#include <core/io/EOFException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        EOFException::EOFException(String const& message)
            : Throwable(message), IOException(message) {}

        Object& EOFException::clone() const {
            try {
                return UNSAFE::newInstance<EOFException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void EOFException::selfThrow() const {
            throw EOFException(*this);
        }
    } // io
} // core
