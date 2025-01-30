//
// Created by admin on 07/01/25.
//

#include "FileSystemNotFoundException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        FileSystemNotFoundException::FileSystemNotFoundException(const String &message)
                : Throwable(message), RuntimeException(message) {}

        Object &FileSystemNotFoundException::clone() const {
            try {
                return UNSAFE::newInstance<FileSystemNotFoundException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void FileSystemNotFoundException::selfThrow() const {
            throw FileSystemNotFoundException(*this);
        }
    } // io
} // core