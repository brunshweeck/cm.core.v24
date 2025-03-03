//
// Created by admin on 07/01/25.
//

#include "NoSuchFileException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        NoSuchFileException::NoSuchFileException(const String &file)
                : FileSystemException(file) {}

        NoSuchFileException::NoSuchFileException(const String &file, const String &other, const String &reason)
                : FileSystemException(file, other, reason) {}

        Object &NoSuchFileException::clone() const {
            try {
                return UNSAFE::newInstance<NoSuchFileException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void NoSuchFileException::selfThrow() const {
            throw NoSuchFileException(*this);
        }
    } // io
} // core