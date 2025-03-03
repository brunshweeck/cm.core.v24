//
// Created by admin on 07/01/25.
//

#include "AtomicMoveNotSupportedException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        AtomicMoveNotSupportedException::AtomicMoveNotSupportedException(const String &file,
                                                                         const String &other,
                                                                         const String &reason)
                : FileSystemException(file, other, reason) {}

        Object &AtomicMoveNotSupportedException::clone() const {
            try {
                return UNSAFE::newInstance<AtomicMoveNotSupportedException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void AtomicMoveNotSupportedException::selfThrow() const {
            throw AtomicMoveNotSupportedException(*this);
        }
    } // io
} // core