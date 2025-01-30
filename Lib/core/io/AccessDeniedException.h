//
// Created by admin on 13/01/25.
//

#ifndef CORE24_CORE24_ACCESSDENIEDEXCEPTION_H
#define CORE24_CORE24_ACCESSDENIEDEXCEPTION_H

#include <core/io/FileSystemException.h>

namespace core {
    namespace io {

        /**
         * Checked exception thrown when a file system operation is denied, typically
         * due to a file permission or other access check.
         */
        class AccessDeniedException: public virtual FileSystemException {
        public:

            /**
             * Constructs an instance of this class.
             *
             * @param   file
             *          a string identifying the file or {@code null} if not known
             */
            CORE_EXPLICIT AccessDeniedException(const String &file);

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
            CORE_EXPLICIT AccessDeniedException(const String &file, const String &other, const String &reason);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_CORE24_ACCESSDENIEDEXCEPTION_H
