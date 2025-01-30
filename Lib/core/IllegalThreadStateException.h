//
// Created by brunshweeck on 13 juin 2024.
//

#ifndef CORE24_ILLEGALTHREADSTATEEXCEPTION_H
#define CORE24_ILLEGALTHREADSTATEEXCEPTION_H
#include <core/RuntimeException.h>

namespace core {
    /**
     * Thrown to indicate that a thread is not in an appropriate state
     * for the requested operation.
     *
     * @see Thread::start
     */
    class IllegalThreadStateException : public virtual RuntimeException {
    public:
        /**
         * Constructs an @c IllegalThreadStateException with no
         * detail message.
         */
        IllegalThreadStateException() = default;

        /**
         * Construct new instance of @c IllegalThreadStateException
         * with given detail message.
         *
         * @param message the detail message
         */
        CORE_EXPLICIT IllegalThreadStateException(String const& message);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_ILLEGALTHREADSTATEEXCEPTION_H
