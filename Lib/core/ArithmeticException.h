//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_ARITHMETICEXCEPTION_H
#define CORE24_ARITHMETICEXCEPTION_H

#include <core/RuntimeException.h>

namespace core {
    /**
     * Thrown when an exceptional arithmetic condition has occurred. For
     * example, an integer @c "divide by zero" throws an instance of this class.
     */
    class ArithmeticException : public virtual RuntimeException {
    public:
        /**
         * Constructs an @c ArithmeticException with no detail
         * message.
         */
        CORE_IMPLICIT ArithmeticException() = default;

        /**
         * Constructs an @c ArithmeticException with the specified
         * detail message.
         *
         * @param message the detail message.
         */
        CORE_EXPLICIT ArithmeticException(String const& message);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_ARITHMETICEXCEPTION_H
