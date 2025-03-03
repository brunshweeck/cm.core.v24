//
// Created by admin on 13/01/25.
//

#ifndef CORE24_CORE24_FILESYSTEMLOOPEXCEPTION_H
#define CORE24_CORE24_FILESYSTEMLOOPEXCEPTION_H

#include "FileSystemException.h"

namespace core {
    namespace io {

        /**
         * Checked exception thrown when a file system loop, or cycle, is encountered.
         */
        class FileSystemLoopException : public FileSystemException {
        public:

            /**
             * Constructs an instance of this class.
             *
             * @param   file
             *          a string identifying the file causing the cycle or @c "" if
             *          not known
             */
            CORE_EXPLICIT FileSystemLoopException(const String &file);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_CORE24_FILESYSTEMLOOPEXCEPTION_H
