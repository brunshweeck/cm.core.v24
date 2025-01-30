//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_ERROR_H
#define CORE24_ERROR_H

#include <core/Throwable.h>

namespace core {
    /**
     * An @c Error is a subclass of @c Throwable
     * that indicates serious problems that a reasonable application
     * should not try to catch. Most such errors are abnormal conditions.
     * <p>
     * A method is not required to declare in its @c throws
     * clause any subclasses of @c Error that might be thrown
     * during the execution of the method but not caught, since these
     * errors are abnormal conditions that should never occur.
     *
     * That is, @c Error and its subclasses are regarded as unchecked
     * exceptions for the purposes of compile-time checking of exceptions.
     */
    class Error : public virtual Throwable {
    public:
        /**
         * Constructs a new error with @c "" as its detail message.
         */
        CORE_IMPLICIT Error() = default;

        /**
         * Constructs a new error with the specified detail message.
         *
         * @param   message   the detail message. The detail message is saved for
         *          later retrieval by the @c Exception::message() method.
         */
        CORE_EXPLICIT Error(String const& message) CORE_NOTHROW;

        /**
         * Constructs a new error with the specified detail message and
         * cause.  <p>Note that the detail message associated with
         * @c cause is <i>not</i> automatically incorporated in
         * this error's detail message.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the @c Error::message() method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Error::cause() method).
         *
         */
        CORE_EXPLICIT Error(String const& message, Throwable const& cause);

        /**
         * Constructs a new error with the specified cause and a detail
         * message of @c cause.toString() (which
         * typically contains the class and detail message of @c cause).
         * This constructor is useful for errors that are little more than
         * wrappers for other throwables.
         *
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Error::cause() method).
         *
         */
        CORE_EXPLICIT Error(Throwable const& cause);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_ERROR_H
