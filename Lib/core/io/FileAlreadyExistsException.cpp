//
// Created by admin on 07/01/25.
//

#include "FileAlreadyExistsException.h"
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        FileAlreadyExistsException::FileAlreadyExistsException(const String &file)
                : FileSystemException(file) {}

        FileAlreadyExistsException::FileAlreadyExistsException(const String &file,
                                                               const String &other,
                                                               const String &reason)
                : FileSystemException(file, other, reason) {}

        Object &FileAlreadyExistsException::clone() const {
            try {
                return UNSAFE::newInstance<FileAlreadyExistsException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void FileAlreadyExistsException::selfThrow() const {
            throw FileAlreadyExistsException(*this);
        }
    } // io
} // core