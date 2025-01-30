//
// Created by admin on 07/01/25.
//

#include "NotLinkException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        NotLinkException::NotLinkException(const String &file) : FileSystemException(file) {}

        NotLinkException::NotLinkException(const String &file, const String &other, const String &reason)
                : FileSystemException(file, other, reason) {}

        Object &NotLinkException::clone() const {
            try {
                return UNSAFE::newInstance<NotLinkException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void NotLinkException::selfThrow() const {
            throw NotLinkException(*this);
        }
    } // io
} // core