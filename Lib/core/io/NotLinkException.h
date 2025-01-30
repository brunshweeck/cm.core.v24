//
// Created by admin on 07/01/25.
//

#ifndef CORE24_NOTLINKEXCEPTION_H
#define CORE24_NOTLINKEXCEPTION_H

#include "FileSystemException.h"

namespace core {
    namespace io {

        /**
         * Checked exception thrown when a file system operation fails because a file
         * is not a symbolic link.
         */
        class NotLinkException : public FileSystemException {
        public:

            /**
             * Constructs an instance of this class.
             *
             * @param   file
             *          a string identifying the file or {@code null} if not known
             */
            CORE_EXPLICIT NotLinkException(const String &file);

            /**
             * Constructs an instance of this class.
             *
             * @param   file
             *          a string identifying the file or {@code null} if not known
             * @param   other
             *          a string identifying the other file or {@code null} if not known
             * @param   reason
             *          a reason message with additional information or {@code null}
             */
            CORE_EXPLICIT NotLinkException(const String &file, const String &other, const String &reason);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_NOTLINKEXCEPTION_H
