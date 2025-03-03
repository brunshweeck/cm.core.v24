//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_EXCEPTION_H
#define CORE24_EXCEPTION_H

#include <core/lang/Throwable.h>

namespace core {
    /**
     * The class @c Exception and its subclasses are a form of
     * @c Throwable that indicates conditions that a reasonable
     * application might want to catch.
     *
     * <p>The class @c Exception and any subclasses that are not also
     * subclasses of @c RuntimeException are <em>checked
     * exceptions</em>.  Checked exceptions need to be declared in a
     * method or constructor's @c throws clause if they can be thrown
     * by the execution of the method or constructor and propagate outside
     * the method or constructor boundary.
     */
    class Exception : public virtual Throwable {
    public:
        /**
         * Constructs a new exception with @c "" as its detail message.
         */
        CORE_IMPLICIT Exception() = default;

        /**
         * Constructs a new exception with the specified detail message.
         *
         * @param   message   the detail message. The detail message is saved for
         *          later retrieval by the @c Exception::message() method.
         */
        CORE_EXPLICIT Exception(String const& message) CORE_NOTHROW;

        /**
         * Constructs a new exception with the specified detail message and
         * cause.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the @c Exception::message() method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Exception::cause() method).
         *
         */
        CORE_EXPLICIT Exception(String const& message, Throwable const& cause);

        /**
         * Constructs a new exception with the specified cause and a detail
         * message of @c cause.toString() (which typically contains the class
         * and detail message of @c cause).
         * This constructor is useful for exceptions that are little more than
         * wrappers for other throwables.
         *
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Exception::cause() method).
         *
         */
        CORE_EXPLICIT Exception(Throwable const& cause);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_EXCEPTION_H
