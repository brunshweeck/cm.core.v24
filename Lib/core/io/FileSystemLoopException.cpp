//
// Created by admin on 13/01/25.
//

#include "FileSystemLoopException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        FileSystemLoopException::FileSystemLoopException(const String &file)
                : FileSystemException(file) {}

        Object &FileSystemLoopException::clone() const {
            try {
                return UNSAFE::newInstance<FileSystemLoopException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void FileSystemLoopException::selfThrow() const {
            throw FileSystemLoopException(*this);
        }
    } // io
} // core