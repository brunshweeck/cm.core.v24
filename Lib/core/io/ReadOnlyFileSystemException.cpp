//
// Created by admin on 07/01/25.
//

#include "ReadOnlyFileSystemException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        Object &ReadOnlyFileSystemException::clone() const {
            try {
                return UNSAFE::newInstance<ReadOnlyFileSystemException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void ReadOnlyFileSystemException::selfThrow() const {
            throw ReadOnlyFileSystemException(*this);
        }
    } // io
} // core