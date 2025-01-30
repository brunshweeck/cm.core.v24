//
// Created by admin on 07/01/25.
//

#ifndef CORE24_DIRECTORYNOTEMPTYEXCEPTION_H
#define CORE24_DIRECTORYNOTEMPTYEXCEPTION_H

#include "FileSystemException.h"

namespace core {
    namespace io {

        /**
         * Checked exception thrown when a file system operation fails because a
         * directory is not empty.
         */
        class DirectoryNotEmptyException: public FileSystemException {
        public:

            /**
             * Constructs an instance of this class.
             *
             * @param   dir
             *          a string identifying the directory or @c "" if not known
             */
            CORE_EXPLICIT DirectoryNotEmptyException(const String &dir);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_DIRECTORYNOTEMPTYEXCEPTION_H
