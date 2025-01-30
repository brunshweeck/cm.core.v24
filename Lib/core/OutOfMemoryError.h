//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_OUTOFMEMORYERROR_H
#define CORE24_OUTOFMEMORYERROR_H

#include <core/Error.h>

namespace core {
    /**
     * Thrown when the Machine cannot allocate an object
     * because it is out of memory, and no more memory could be made
     * available by the garbage collector.
     */
    class OutOfMemoryError final : public virtual Error {
    public:
        /**
         * Constructs an @c OutOfMemoryError with no detail message.
         */
        CORE_IMPLICIT OutOfMemoryError() = default;

        /**
         * Constructs an @c OutOfMemoryError with the specified
         * detail message.
         *
         * @param message   the detail message.
         */
        CORE_EXPLICIT OutOfMemoryError(String const& message);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_OUTOFMEMORYERROR_H
