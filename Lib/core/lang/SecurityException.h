//
// Created by bruns on 03/10/2024.
//

#ifndef CORE24_SECURITYEXCEPTION_H
#define CORE24_SECURITYEXCEPTION_H
#include "RuntimeException.h"

namespace core {
    /**
     * Thrown by the security manager to indicate a security violation.
     */
    class SecurityException : public virtual RuntimeException {
    public:
        /**
         * Constructs a @c SecurityException with no detail message.
         */
        CORE_EXPLICIT SecurityException() = default;

        /**
         * Constructs a @c SecurityException with the specified
         * detail message.
         *
         * @param message  the detail message.
         */
        CORE_EXPLICIT SecurityException(String const& message);

        /**
         * Creates a @c SecurityException with the specified
         * detail message and cause.
         *
         * @param message the detail message (which is saved for later retrieval
         *        by the @b message() method).
         * @param cause the cause (which is saved for later retrieval by the
         *        @b cause() method)
         */
        CORE_EXPLICIT SecurityException(String const& message, Throwable const& cause);

        /**
         * Creates a @c SecurityException with the specified cause
         * and a detail message of @c cause.toString()
         * (which typically contains the class and detail message of
         * @c cause).
         *
         * @param cause the cause (which is saved for later retrieval by the
         *        @b cause() method).
         */
        CORE_EXPLICIT SecurityException(Throwable const& cause);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_SECURITYEXCEPTION_H
