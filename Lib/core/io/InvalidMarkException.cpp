//
// Created by brunshweeck on 26 août 2024.
//

#include <core/io/InvalidMarkException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        Object& InvalidMarkException::clone() const {
            try {
                return UNSAFE::newInstance<InvalidMarkException>();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void InvalidMarkException::selfThrow() const {
            throw InvalidMarkException(*this);
        }
    } // io
} // core
