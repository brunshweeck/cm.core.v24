//
// Created by admin on 07/01/25.
//

#ifndef CORE24_NOSUCHFILEEXCEPTION_H
#define CORE24_NOSUCHFILEEXCEPTION_H

#include <core/io/FileSystemException.h>

namespace core {
    namespace io {

        /**
         * Checked exception thrown when an attempt is made to access a file that does
         * not exist.
         */
        class NoSuchFileException final : public FileSystemException {
        public:

            /**
             * Constructs an instance of this class.
             *
             * @param   file
             *          a string identifying the file or @c "" if not known.
             */
            CORE_EXPLICIT NoSuchFileException(const String &file);

            /**
             * Constructs an instance of this class.
             *
             * @param   file
             *          a string identifying the file or @c "" if not known.
             * @param   other
             *          a string identifying the other file or @c "" if not known.
             * @param   reason
             *          a reason message with additional information or @c ""
             */
            CORE_EXPLICIT NoSuchFileException(const String &file, const String &other, const String &reason);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_NOSUCHFILEEXCEPTION_H
