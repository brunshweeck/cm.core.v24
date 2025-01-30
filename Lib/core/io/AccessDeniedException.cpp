//
// Created by admin on 13/01/25.
//

#include "AccessDeniedException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        AccessDeniedException::AccessDeniedException(const String &file) : FileSystemException(file) {}

        AccessDeniedException::AccessDeniedException(const String &file, const String &other, const String &reason)
                : FileSystemException(file, other, reason) {}

        Object &AccessDeniedException::clone() const {
            try {
                return UNSAFE::newInstance<AccessDeniedException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void AccessDeniedException::selfThrow() const {
            throw AccessDeniedException(*this);
        }
    } // io
} // core