//
// Created by brunshweeck on 27 août 2024.
//

#include <core/io/ReadOnlyBufferException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        Object& ReadOnlyBufferException::clone() const {
            try {
                return UNSAFE::newInstance<ReadOnlyBufferException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void ReadOnlyBufferException::selfThrow() const {
            throw ReadOnlyBufferException(*this);
        }
    } // io
} // core
