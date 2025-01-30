//
// Created by brunshweeck on 1 sept. 2024.
//

#ifndef CORE24_IOEXCEPTION_H
#define CORE24_IOEXCEPTION_H

#include <core/Exception.h>

namespace core {
    namespace io {
        /**
         * Signals that an I/O exception, of some sort has occurred. This
         * class is the general class of exceptions produced by failed or
         * interrupted I/O operations.
         *
         * @see  io::InputStream
         * @see  io::OutputStream
         */
        class IOException : public virtual Exception {
        public:
            /**
             * Constructs an @c IOException with no detail message.
             */
            CORE_IMPLICIT IOException() = default;

            /**
             * Constructs an @c IOException with the specified detail message.
             *
             * @param message
             *        The detail message (which is saved for later retrieval
             *        by the @b message() method)
             */
            CORE_EXPLICIT IOException(String const& message);

            /**
             * Constructs an @c IOException with the specified detail message
             * and cause.
             *
             * <p> Note that the detail message associated with @c cause is
             * <i>not</i> automatically incorporated into this exception's detail
             * message.
             *
             * @param message
             *        The detail message (which is saved for later retrieval
             *        by the @b message() method)
             *
             * @param cause
             *        The cause (which is saved for later retrieval by the
             *        @b cause() method).
             */
            CORE_EXPLICIT IOException(String const& message, Throwable const& cause);

            /**
             * Constructs an @c IOException with the specified cause and a
             * detail message of @c cause.toString()
             * (which typically contains the class and detail message of @c cause).
             * This constructor is useful for IO exceptions that are little more
             * than wrappers for other throwables.
             *
             * @param cause
             *        The cause (which is saved for later retrieval by the
             *        @b cause() method).
             */
            CORE_EXPLICIT IOException(Throwable const& cause);

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // io
} // core

#endif //CORE24_IOEXCEPTION_H
