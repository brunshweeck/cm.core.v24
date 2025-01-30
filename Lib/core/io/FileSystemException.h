//
// Created by admin on 07/01/25.
//

#ifndef CORE24_FILESYSTEMEXCEPTION_H
#define CORE24_FILESYSTEMEXCEPTION_H

#include <core/io/IOException.h>

namespace core {
    namespace io {

        class FileSystemException : public virtual IOException {

            /**
             *  String identifying the file or @c "" if not known.
             */
            String file;

            /**
             *  String identifying the other file or {@code null} if there isn't
             *  another file or if not known.
             */
            String other;

        public:

            /**
             * Constructs an instance of this class. This constructor should be used
             * when an operation involving one file fails and there isn't any additional
             * information to explain the reason.
             *
             * @param   file
             *          a string identifying the file or {@code null} if not known.
             */
            CORE_EXPLICIT FileSystemException(String const &file);

            /**
             * Constructs an instance of this class. This constructor should be used
             * when an operation involving two files fails, or there is additional
             * information to explain the reason.
             *
             * @param   file
             *          a string identifying the file or {@code null} if not known.
             * @param   other
             *          a string identifying the other file or {@code null} if there
             *          isn't another file or if not known
             * @param   reason
             *          a reason message with additional information or {@code null}
             */
            CORE_EXPLICIT FileSystemException(String const &file, String const &other, String const &reason);

            /**
             * Returns the file used to create this exception.
             *
             * @return  the file (can be empty)
             */
            String getFile() const;

            /**
             * Returns the other file used to create this exception.
             *
             * @return  the other file (can be empty)
             */
            String otherFile() const;

            /**
             * Returns the string explaining why the file system operation failed.
             *
             * @return  the string explaining why the file system operation failed
             */
            String getReason() const;

            /**
             * Returns the detail message string.
             */
            String message() const override;

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_FILESYSTEMEXCEPTION_H
