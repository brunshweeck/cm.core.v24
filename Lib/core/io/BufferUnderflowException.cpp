//
// Created by brunshweeck on 26 août 2024.
//

#include <core/io/BufferUnderflowException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        Object& BufferUnderflowException::clone() const {
            try {
                return UNSAFE::newInstance<BufferUnderflowException>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void BufferUnderflowException::selfThrow() const {
            throw BufferUnderflowException(*this);
        }
    } // io
} // core
