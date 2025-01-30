//
// Created by admin on 07/01/25.
//

#ifndef CORE24_READONLYFILESYSTEMEXCEPTION_H
#define CORE24_READONLYFILESYSTEMEXCEPTION_H

#include "core/UnsupportedOperationException.h"

namespace core {
    namespace io {

        /**
         * Unchecked exception thrown when an attempt is made to update an object
         * associated with a @b read-only @c FileSystem.
         */
        class ReadOnlyFileSystemException : public UnsupportedOperationException {
        public:
            /**
             * Constructs an instance of this class.
             */
            ReadOnlyFileSystemException() = default;

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_READONLYFILESYSTEMEXCEPTION_H
