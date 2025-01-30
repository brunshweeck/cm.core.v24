//
// Created by admin on 07/01/25.
//

#ifndef CORE24_FILESYSTEMNOTFOUNDEXCEPTION_H
#define CORE24_FILESYSTEMNOTFOUNDEXCEPTION_H

#include "core/RuntimeException.h"

namespace core {
    namespace io {

        /**
         * Runtime exception thrown when a file system cannot be found.
         */
        class FileSystemNotFoundException : public RuntimeException {
        public:

            /**
             * Constructs an instance of this class.
             */
            FileSystemNotFoundException() = default;

            /**
             * Constructs an instance of this class.
             *
             * @param   msg
             *          the detail message
             */
            CORE_EXPLICIT FileSystemNotFoundException(const String &msg);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_FILESYSTEMNOTFOUNDEXCEPTION_H
