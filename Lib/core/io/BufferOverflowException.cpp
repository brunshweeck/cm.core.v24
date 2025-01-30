//
// Created by brunshweeck on 26 août 2024.
//

#include <core/io/BufferOverflowException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        Object& BufferOverflowException::clone() const {
            try {
                return UNSAFE::newInstance<BufferOverflowException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void BufferOverflowException::selfThrow() const {
            throw BufferOverflowException(*this);
        }
    } // io
} // core
