//
// Created by brunshweeck on 25 mai 2024.
//

#ifndef CORE24_INDEXOUTOFBOUNDSEXCEPTION_H
#define CORE24_INDEXOUTOFBOUNDSEXCEPTION_H

#include "RuntimeException.h"

namespace core {
    /**
     * Thrown to indicate that an index of some sort (such as to an array, to a
     * string, or to a vector) is out of range.
     * <p>
     * Applications can subclass this class to indicate similar exceptions.
     */
    class IndexOutOfBoundsException : public virtual RuntimeException {
    public:
        /**
         * Constructs an @c IndexOutOfBoundsException with no detail message.
         */
        IndexOutOfBoundsException() = default;

        /**
         * Constructs an @c IndexOutOfBoundsException with the specified detail
         * message.
         *
         * @param s the detail message
         */
        CORE_EXPLICIT IndexOutOfBoundsException(String const& message);

        /**
         * Constructs a new @c IndexOutOfBoundsException class with an
         * argument indicating the illegal index.
         *
         * <p>The index is included in this exception's detail message.  The
         * exact presentation format of the detail message is unspecified.
         *
         * @param index the illegal index.
         *
         */
        CORE_EXPLICIT IndexOutOfBoundsException(gint index);

        /**
         * Constructs a new @c IndexOutOfBoundsException class with an
         * argument indicating the illegal index.
         *
         * <p>The index is included in this exception's detail message.  The
         * exact presentation format of the detail message is unspecified.
         *
         * @param index the illegal index.
         *
         */
        CORE_EXPLICIT IndexOutOfBoundsException(glong index);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_INDEXOUTOFBOUNDSEXCEPTION_H
