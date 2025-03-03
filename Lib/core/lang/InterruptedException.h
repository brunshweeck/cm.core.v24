//
// Created by bruns on 01/10/2024.
//

#ifndef CORE24_INTERRUPTEDEXCEPTION_H
#define CORE24_INTERRUPTEDEXCEPTION_H

#include <core/lang/Exception.h>

namespace core {
    /**
     * Thrown when a thread is waiting, sleeping, or otherwise occupied,
     * and the thread is interrupted, either before or during the activity.
     * Occasionally a method may wish to test whether the current
     * thread has been interrupted, and if so, to immediately throw
     * this exception.  The following code can be used to achieve
     * this effect:
     * @code
     * if (Thread.interrupted())  // Clears interrupted status!
     *     InterruptedException().throws($ftrace());
     * @endcode
     *
     * @see Thread::sleep(glong)
     * @see Thread::interrupt()
     * @see Thread::interrupted()
     */
    class InterruptedException : public virtual Exception {
    public:
        /**
         * Constructs an @c InterruptedException with no detail  message.
         */
        CORE_IMPLICIT InterruptedException() = default;

        /**
         * Constructs an @c InterruptedException with the
         * specified detail message.
         *
         * @param s   the detail message.
         */
        CORE_EXPLICIT InterruptedException(const String& s);

        Object& clone() const override;

    protected:
        void selfThrow() const override;
    };
} // core

#endif //CORE24_INTERRUPTEDEXCEPTION_H
