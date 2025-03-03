//
// Created by brunshweeck on 21 juin 2024.
//

#ifndef CORE24_NOSUCHELEMENTEXCEPTION_H
#define CORE24_NOSUCHELEMENTEXCEPTION_H

#include <core/lang/RuntimeException.h>

namespace core {
    namespace util {
        /**
         * Thrown by various accessor methods to indicate that the element
         * being requested does not exist.
         */
        class NoSuchElementException : public virtual RuntimeException {
        public:
            /**
             * Construct new instance of @c NoSuchElementException
             * without detail message.
             */
            CORE_IMPLICIT NoSuchElementException() = default;

            /**
             * Construct new instance of @c NoSuchElementException
             * with specified details message.
             *
             * @param message the detail message.
             */
            CORE_EXPLICIT NoSuchElementException(String const& message);

            /**
             * Construct new instance of @c NoSuchElementException with
             * specified detail message and cause
             *
             * @param message the detail message.
             * @param cause the cause.
             */
            CORE_EXPLICIT NoSuchElementException(String const& message, Throwable const& cause);

            /**
             * Construct new instance of @c NoSuchElementException with
             * specified cause.
             *
             * @param cause the cause.
             */
            CORE_EXPLICIT NoSuchElementException(Throwable const& cause);

            Object& clone() const override;

        protected:
            void selfThrow() const override;
        };
    } // util
} // core

#endif //CORE24_NOSUCHELEMENTEXCEPTION_H
