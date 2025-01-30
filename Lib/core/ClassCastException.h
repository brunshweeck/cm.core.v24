//
// Created by brunshweeck on 13 juin 2024.
//

#ifndef CORE24_CLASSCASTEXCEPTION_H
#define CORE24_CLASSCASTEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    /**
     * Thrown to indicate that the code has attempted to cast an object
     * to a subclass of which it is not an instance. For example, the
     * following code generates a @c ClassCastException
     */
    class ClassCastException : public virtual RuntimeException {
    public:
        /**
         * Construct new instance of @c ClassCastException
         * without details message.
         */
        ClassCastException() = default;

        /**
         * Construct new instance of @c ClassCastException
         * with given details message.
         *
         * @param message the details message
         */
        CORE_EXPLICIT ClassCastException(String const& message);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_CLASSCASTEXCEPTION_H
