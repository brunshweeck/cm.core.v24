//
// Created by admin on 07/01/25.
//

#include "DirectoryNotEmptyException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        DirectoryNotEmptyException::DirectoryNotEmptyException(const String &dir)
                : FileSystemException(dir) {}

        Object &DirectoryNotEmptyException::clone() const {
            try {
                return UNSAFE::newInstance<DirectoryNotEmptyException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void DirectoryNotEmptyException::selfThrow() const {
            throw DirectoryNotEmptyException(*this);
        }
    } // io
} // core