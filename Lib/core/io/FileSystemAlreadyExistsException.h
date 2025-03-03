//
// Created by admin on 07/01/25.
//

#ifndef CORE24_FILESYSTEMALREADYEXISTSEXCEPTION_H
#define CORE24_FILESYSTEMALREADYEXISTSEXCEPTION_H

#include <core/lang/RuntimeException.h>

namespace core {
    namespace io {

        /**
         * Runtime exception thrown when an attempt is made to create a file system that
         * already exists.
         */
        class FileSystemAlreadyExistsException: public RuntimeException {
        public:

            /**
             * Constructs an instance of this class.
             */
            FileSystemAlreadyExistsException() = default;

            /**
             * Constructs an instance of this class.
             *
             * @param   msg
             *          the detail message
             */
            CORE_EXPLICIT FileSystemAlreadyExistsException(const String &msg);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_FILESYSTEMALREADYEXISTSEXCEPTION_H
