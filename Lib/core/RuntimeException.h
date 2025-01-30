//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_RUNTIMEEXCEPTION_H
#define CORE24_RUNTIMEEXCEPTION_H

#include <core/Exception.h>

namespace core {
    /**
     * @c RuntimeException is the superclass of those
     * exceptions that can be thrown during the normal operation.
     */
    class RuntimeException : public virtual Exception {
    public:
        /**
         * Constructs a new runtime exception with @c "" as its
         * detail message.
         */
        CORE_IMPLICIT RuntimeException();

        /** Constructs a new runtime exception with the specified detail message.
         *
         * @param   message   the detail message. The detail message is saved for
         *          later retrieval by the @c RuntimeException::message() method.
         */
        CORE_EXPLICIT RuntimeException(String const &message) CORE_NOTHROW;

        /**
         * Constructs a new runtime exception with the specified detail message and
         * cause.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the @c RuntimeException::message() method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c RuntimeException::cause() method).
         *
         */
        CORE_EXPLICIT RuntimeException(String const &message, Throwable const &cause);

        /** Constructs a new runtime exception with the specified cause and a
         * detail message of @c cause.toString() (which typically contains the
         * class and detail message of @c cause).
         *
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c RuntimeException::cause() method).
         *
         */
        CORE_EXPLICIT RuntimeException(Throwable const &cause);

        Object &clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_RUNTIMEEXCEPTION_H
