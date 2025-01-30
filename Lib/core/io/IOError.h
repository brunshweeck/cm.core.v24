//
// Created by brunshweeck on 15 sept. 2024.
//

#ifndef CORE24_IOERROR_H
#define CORE24_IOERROR_H

#include <core/Error.h>

namespace core {
    namespace io {
        /**
         * Thrown when a serious I/O error has occurred.
         */
        class IOError : public virtual Error {
        public:
            /**
             * Constructs a new instance of IOError with the specified cause. The
             * IOError is created with the detail message of
             * @c cause.toString() (which typically contains the class and detail message of cause).
             *
             * @param  cause
             *         The cause of this error
             */
            CORE_EXPLICIT IOError(Throwable const& cause);

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // io
} // core

#endif //CORE24_IOERROR_H
