//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_ILLEGALARGUMENTEXCEPTION_H
#define CORE24_ILLEGALARGUMENTEXCEPTION_H

#include <core/lang/RuntimeException.h>

namespace core {
    /**
     * Thrown to indicate that a method has been passed an illegal or
     * inappropriate argument.
     */
    class IllegalArgumentException : public virtual RuntimeException {
    public:
        /**
         * Constructs an @c IllegalArgumentException with no
         * detail message.
         */
        CORE_IMPLICIT IllegalArgumentException() = default;

        /**
         * Constructs an @c IllegalArgumentException with the
         * specified detail message.
         *
         * @param   s   the detail message.
         */
        CORE_EXPLICIT IllegalArgumentException(String const& message);

        /**
         * Constructs a new exception with the specified detail message and
         * cause.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the @c Throwable::message() method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Throwable::cause() method).
         *
         */
        CORE_EXPLICIT IllegalArgumentException(String const& message, Throwable const& cause);

        /**
         * Constructs a new exception with the specified cause and a detail
         * message of @c cause.toString() (which
         * typically contains the class and detail message of @c cause).
         *
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Throwable::cause() method).
         *
         */
        CORE_EXPLICIT IllegalArgumentException(Throwable const& cause);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_ILLEGALARGUMENTEXCEPTION_H
