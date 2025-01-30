//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_ILLEGALSTATEEXCEPTION_H
#define CORE24_ILLEGALSTATEEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    class IllegalStateException : public virtual RuntimeException {
    public:
        /**
         * Constructs an IllegalStateException with no detail message.
         * A detail message is a String that describes this particular exception.
         */
        CORE_IMPLICIT IllegalStateException() = default;

        /**
         * Constructs an IllegalStateException with the specified detail
         * message.  A detail message is a String that describes this particular
         * exception.
         *
         * @param s the String that contains a detailed message
         */
        CORE_EXPLICIT IllegalStateException(String const& message);

        /**
         * Constructs a new exception with the specified detail message and
         * cause.
         *
         * <p>Note that the detail message associated with @c cause is
         * <i>not</i> automatically incorporated in this exception's detail
         * message.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the @c Throwable::message() method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Throwable::cause() method).
         *
         */
        CORE_EXPLICIT IllegalStateException(String const& message, Throwable const& cause);

        /**
         * Constructs a new exception with the specified cause and a detail
         * message of @c cause.toString() (which
         * typically contains the class and detail message of @c cause).
         * This constructor is useful for exceptions that are little more than
         * wrappers for other throwables.
         *
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Throwable::cause() method).
         *
         */
        CORE_EXPLICIT IllegalStateException(Throwable const& cause);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_ILLEGALSTATEEXCEPTION_H
