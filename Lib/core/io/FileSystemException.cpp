//
// Created by admin on 07/01/25.
//

#include "FileSystemException.h"
#include <core/lang/XString.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        FileSystemException::FileSystemException(const String &file) : file(file) {
        }

        FileSystemException::FileSystemException(const String &file, const String &other, const String &reason)
                : IOException(reason), file(file), other(other) {
        }

        String FileSystemException::getFile() const {
            return file;
        }

        String FileSystemException::otherFile() const {
            return other;
        }

        String FileSystemException::getReason() const {
            return IOException::message();
        }

        String FileSystemException::message() const {
            if (file.isEmpty() && other.isEmpty())
                return getReason();
            XString str;
            if (!file.isEmpty())
                str.append(file);
            if (!other.isEmpty())
                str.append(" -> ").append(other);
            String reason = getReason();
            if (!getReason().isEmpty())
                str.append(": ").append(reason);
            return str.toString();
        }

        Object &FileSystemException::clone() const {
            try {
                return UNSAFE::newInstance<FileSystemException>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void FileSystemException::selfThrow() const {
            throw FileSystemException(*this);
        }
    } // io
} // core