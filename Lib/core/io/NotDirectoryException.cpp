//
// Created by admin on 07/01/25.
//

#include "NotDirectoryException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        NotDirectoryException::NotDirectoryException(const String &file)
                : FileSystemException(file) {}

        Object &NotDirectoryException::clone() const {
            try {
                return UNSAFE::newInstance<NotDirectoryException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void NotDirectoryException::selfThrow() const {
            throw NotDirectoryException(*this);
        }
    } // io
} // core