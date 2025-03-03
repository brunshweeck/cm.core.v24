//
// Created by admin on 07/01/25.
//

#ifndef CORE24_NOTDIRECTORYEXCEPTION_H
#define CORE24_NOTDIRECTORYEXCEPTION_H

#include "FileSystemException.h"

namespace core {
    namespace io {

        /**
         * Checked exception thrown when a file system operation, intended for a
         * directory, fails because the file is not a directory.
         */
        class NotDirectoryException : public FileSystemException {
        public:

            /**
             * Constructs an instance of this class.
             *
             * @param   file
             *          a string identifying the file or @c "" if not known
             */
            CORE_EXPLICIT NotDirectoryException(const String &file);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_NOTDIRECTORYEXCEPTION_H
