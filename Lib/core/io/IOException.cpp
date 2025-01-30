//
// Created by brunshweeck on 1 sept. 2024.
//

#include <core/io/IOException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        IOException::IOException(String const& message)
            : Throwable(message), Exception(message) {}

        IOException::IOException(String const& message, Throwable const& cause)
            : Throwable(message, cause), Exception(message, cause) {}

        IOException::IOException(Throwable const& cause)
            : IOException(cause.toString(), cause) {}

        Object& IOException::clone() const {
            try {
                return UNSAFE::newInstance<IOException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void IOException::selfThrow() const {
            throw IOException(*this);
        }
    } // io
} // core
