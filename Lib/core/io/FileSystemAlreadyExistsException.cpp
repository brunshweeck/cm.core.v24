//
// Created by admin on 07/01/25.
//

#include "FileSystemAlreadyExistsException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        FileSystemAlreadyExistsException::FileSystemAlreadyExistsException(const String &message)
                : Throwable(message), RuntimeException(message) {}

        Object &FileSystemAlreadyExistsException::clone() const {
            try {
                return UNSAFE::newInstance<FileSystemAlreadyExistsException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void FileSystemAlreadyExistsException::selfThrow() const {
            throw FileSystemAlreadyExistsException(*this);
        }
    } // io
} // core