//
// Created by brunshweeck on 11 juin 2024.
//

#ifndef CORE24_ASSERTIONERROR_H
#define CORE24_ASSERTIONERROR_H

#include <core/lang/Error.h>

namespace core {
    /**
     * Thrown to indicate that an assertion has failed.
     *
     * <p>
     * The seven one-argument public constructors provided by this
     * class ensure that the assertion error returned by the invocation:
     * @code
     *  AssertionError(expression)
     * @endcode
     * has as its detail message the <i>string conversion</i> of
     * <i>expression</i>, regardless of the type of <i>expression</i>.
     * </p>
     */
    class AssertionError : public virtual Error {
    public:
        CORE_IMPLICIT AssertionError();

        CORE_EXPLICIT AssertionError(String const& message);

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified object, which is converted to a string.
         * <p>
         * If the specified object is an instance of @c Throwable, it
         * becomes the <i>cause</i> of the newly constructed assertion error.
         * </p>
         * @param message value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(Object const& message);

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified @c gbool, which is converted to
         * a string.
         *
         * @param message value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gbool message);

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified @c gint, which is converted to a
         * string as.
         *
         * @param message value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gint message);

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified @c glong, which is converted to a
         * string as.
         *
         * @param message value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(glong message);

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified @c gfloat, which is converted to a
         * string as.
         *
         * @param message value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gfloat message);

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified @c gdouble, which is converted to a
         * string as.
         *
         * @param message value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gdouble message);

        /**
         * Constructs an AssertionError with its detail message derived
         * from the specified @c gchar, which is converted to a
         * string as.
         *
         * @param message value to be used in constructing detail message
         */
        CORE_EXPLICIT AssertionError(gchar message);

        /**
         * Constructs a new @c AssertionError with the specified
         * detail message and cause.
         *
         * <p>Note that the detail message associated with
         * @c cause is <i>not</i> automatically incorporated in
         * this error's detail message.
         *
         * @param  message the detail message
         * @param  cause the cause
         *
         *
         */
        CORE_EXPLICIT AssertionError(String const& message, Throwable const& cause);

        CORE_EXPLICIT AssertionError(Throwable const& cause);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_ASSERTIONERROR_H
