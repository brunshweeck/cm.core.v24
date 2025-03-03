//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_NUMBERFORMATEXCEPTION_H
#define CORE24_NUMBERFORMATEXCEPTION_H

#include "RuntimeException.h"

namespace core {
    /**
     * Thrown to indicate that the application has attempted to convert
     * a string to one of the numeric types, but that the string does not
     * have the appropriate format.
     */
    class NumberFormatException : public virtual RuntimeException {
    public:
        /**
         * Constructs a @c NumberFormatException with no detail message.
         */
        CORE_IMPLICIT NumberFormatException() = default;

        /**
         * Constructs a @c NumberFormatException with the
         * specified detail message.
         *
         * @param  message   the detail message.
         */
        CORE_EXPLICIT NumberFormatException(String const& message);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_NUMBERFORMATEXCEPTION_H
