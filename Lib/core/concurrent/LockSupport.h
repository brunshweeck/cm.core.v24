//
// Created by bruns on 01/10/2024.
//

#ifndef CORE24_LOCKSUPPORT_H
#define CORE24_LOCKSUPPORT_H
#include "core/Object.h"

namespace core {
    namespace concurrent {
        /**
         * Basic thread blocking primitives for creating concurrent and other
         * synchronization classes.
         *
         * <p>
         * This class associates, with each thread that uses it, a permit
         * (in the sense of the @b Semaphore class).
         * A call to @c park will return immediately
         * if the permit is available, consuming it in the process; otherwise
         * it <em>may</em> block.  A call to @c unpark makes the permit
         * available, if it was not already available. (Unlike with Semaphores
         * though, permits do not accumulate. There is at most one.)
         * Reliable usage requires the use of volatile (or atomic) variables
         * to control when to park or unpark.  Orderings of calls to these
         * methods are maintained with respect to volatile variable accesses,
         * but not necessarily non-volatile variable accesses.
         * </p>
         * <p>
         * Methods @c park and @c unpark provide efficient
         * means of blocking and unblocking threads that do not encounter the
         * problems that cause the deprecated methods @c Thread::suspend
         * and @c Thread::resume to be unusable for such purposes: Races
         * between one thread invoking @c park and another thread trying
         * to @c unpark it will preserve liveness, due to the
         * permit. Additionally, @c park will return if the caller's
         * thread was interrupted, and timeout versions are supported. The
         * @c park method may also return at any other time, for "no
         * reason", so in general must be invoked within a loop that rechecks
         * conditions upon return. In this sense @c park serves as an
         * optimization of a "busy wait" that does not waste as much time
         * spinning, but must be paired with an @c unpark to be
         * effective.
         * </p>
         * <p>
         * The three forms of @c park each also support a
         * @c blocker object parameter. This object is recorded while
         * the thread is blocked to permit monitoring and diagnostic tools to
         * identify the reasons that threads are blocked. (Such tools may
         * access blockers using method <b> getBlocker(Thread) </b>.)
         * The use of these forms rather than the original forms without this
         * parameter is strongly encouraged. The normal argument to supply as
         * a @c blocker within a lock implementation is @c this.
         * </p>
         * <p>
         * These methods are designed to be used as tools for creating
         * higher-level synchronization utilities, and are not in themselves
         * useful for most concurrency control applications.  The @c park
         * method is designed for use only in constructions of the form:
         *
         * @code
         * while (!canProceed()) {
         *   // ensure request to unpark is visible to other threads
         *   ...
         *   LockSupport.park(*this);
         * }
         * @endcode
         *
         * where no actions by the thread publishing a request to unpark,
         * prior to the call to @c park, entail locking or blocking.
         * Because only one permit is associated with each thread, any
         * intermediary uses of @c park, including implicitly via class
         * loading, could lead to an unresponsive thread (a "lost unpark").
         * </p>
         * <p>
         * <b>Sample Usage.</b> Here is a sketch of a first-in-first-out
         * non-reentrant lock class:
         * @code
         * class FIFOMutex {
         *   AtomicBoolean locked = new AtomicBoolean(false);
         *   Queue<Thread> &waiters = *new ConcurrentLinkedQueue<>();
         *
         *   public:
         *   void lock() {
         *     gbool wasInterrupted = false;
         *     // publish current thread for unparkers
         *     waiters.add(Thread.currentThread());
         *
         *     // Block while not first in queue or cannot acquire lock
         *     while (waiters.peek() != Thread::currentThread() ||
         *            !locked.compareAndSet(false, true)) {
         *       LockSupport::park(*this);
         *       // ignore interrupts while waiting
         *       if (Thread::interrupted())
         *         wasInterrupted = true;
         *     }
         *
         *     waiters.remove();
         *     // ensure correct interrupt status on return
         *     if (wasInterrupted)
         *       Thread::currentThread().interrupt();
         *   }
         *
         *   void unlock() {
         *     locked.set(false);
         *     LockSupport::unpark(waiters.peek());
         *   }
         * };
         * @endcode
         */
        class LockSupport final : public Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            CORE_IMPLICIT LockSupport() = default;

            static void setBlocker(Thread &t, Object &arg);

        public:
            /**
             * Sets the object to be returned by invocations of
             * @b getBlocker for the current thread. This method may
             * be used before invoking the no-argument version of
             * @b park() from non-public objects, allowing
             * more helpful diagnostics, or retaining compatibility with
             * previous implementations of blocking methods.  Previous values
             * of the blocker are not automatically restored after blocking.
             * To obtain the effects of @c park(b), use
             * @code setCurrentBlocker(b); park(); setCurrentBlocker(null); @endcode
             *
             * @param blocker the blocker object
             */
            static void setCurrentBlocker(Object &blocker);

            /**
             * Makes available the permit for the given thread, if it
             * was not already available.  If the thread was blocked on
             * @c park then it will unblock.  Otherwise, its next call
             * to @c park is guaranteed not to block. This operation
             * is not guaranteed to have any effect at all if the given
             * thread has not been started.
             *
             * @param thread the thread to unpark.
             */
            static void unpark(Thread &thread);

            /**
             * Disables the current thread for thread scheduling purposes unless the
             * permit is available.
             *
             * <p>If the permit is available then it is consumed and the call returns
             * immediately; otherwise
             * the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until one of three things happens:
             *
             *
             * @li Some other thread invokes @b unpark with the
             * current thread as the target; or
             *
             * @li Some other thread @em interrupts
             * the current thread; or
             *
             * @li The call spuriously (that is, for no reason) returns.
             *
             *
             * <p>
             * This method does <em>not</em> report which of these caused the
             * method to return. Callers should re-check the conditions which caused
             * the thread to park in the first place. Callers may also determine,
             * for example, the interrupt status of the thread upon return.
             * </p>
             * @param blocker the synchronization object responsible for this
             *        thread parking
             */
            static void park(Object &blocker);

            /**
             * Disables the current thread for thread scheduling purposes, for up to
             * the specified waiting time, unless the permit is available.
             *
             * <p>
             * If the specified waiting time is zero or negative, the
             * method does nothing. Otherwise, if the permit is available then
             * it is consumed and the call returns immediately; otherwise the
             * current thread becomes disabled for thread scheduling purposes
             * and lies dormant until one of four things happens:
             *
             *
             * @li Some other thread invokes @b unpark with the
             * current thread as the target; or
             *
             * @li Some other thread @em interrupts
             * the current thread; or
             *
             * @li The specified waiting time elapses; or
             *
             * @li The call spuriously (that is, for no reason) returns.
             * </p>
             *
             * <p>
             * This method does <em>not</em> report which of these caused the
             * method to return. Callers should re-check the conditions which caused
             * the thread to park in the first place. Callers may also determine,
             * for example, the interrupt status of the thread, or the elapsed time
             * upon return.
             * </p>
             * @param blocker the synchronization object responsible for this
             *        thread parking
             * @param nanos the maximum number of nanoseconds to wait
             */
            static void parkNanos(Object &blocker, glong nanos);

            /**
             * Disables the current thread for thread scheduling purposes, until
             * the specified deadline, unless the permit is available.
             *
             * <p>
             * If the permit is available then it is consumed and the call
             * returns immediately; otherwise the current thread becomes disabled
             * for thread scheduling purposes and lies dormant until one of four
             * things happens:
             *
             * @li Some other thread invokes @b unpark with the
             * current thread as the target; or
             *
             * @li Some other thread @em interrupts the
             * current thread; or
             *
             * @li The specified deadline passes; or
             *
             * @li The call spuriously (that is, for no reason) returns.
             * </p>
             *
             * <p>
             * This method does <em>not</em> report which of these caused the
             * method to return. Callers should re-check the conditions which caused
             * the thread to park in the first place. Callers may also determine,
             * for example, the interrupt status of the thread, or the current time
             * upon return.
             * </p>
             * @param blocker the synchronization object responsible for this
             *        thread parking
             * @param deadline the absolute time, in milliseconds from the Epoch,
             *        to wait until
             */
            static void parkUntil(Object &blocker, glong deadline);

            /**
             * Returns the blocker object supplied to the most recent
             * invocation of a park method that has not yet unblocked, or null
             * if not blocked.  The value returned is just a momentary
             * snapshot -- the thread may have since unblocked or blocked on a
             * different blocker object.
             *
             * @param t the thread
             * @return the blocker
             */
            static Object &getCurrentBlocker(Thread const &t);

            /**
             * Disables the current thread for thread scheduling purposes unless the
             * permit is available.
             *
             * <p>
             * If the permit is available then it is consumed and the call
             * returns immediately; otherwise the current thread becomes disabled
             * for thread scheduling purposes and lies dormant until one of three
             * things happens:
             *
             * @li Some other thread invokes @b unpark with the
             * current thread as the target; or
             *
             * @li Some other thread @em interrupts
             * the current thread; or
             *
             * @li The call spuriously (that is, for no reason) returns.
             * </p>
             *
             * <p>
             * This method does <em>not</em> report which of these caused the
             * method to return. Callers should re-check the conditions which caused
             * the thread to park in the first place. Callers may also determine,
             * for example, the interrupt status of the thread upon return.
             * </p>
             */
            static void park();

            /**
             * Disables the current thread for thread scheduling purposes, for up to
             * the specified waiting time, unless the permit is available.
             *
             * <p>
             * If the specified waiting time is zero or negative, the
             * method does nothing. Otherwise, if the permit is available then
             * it is consumed and the call returns immediately; otherwise the
             * current thread becomes disabled for thread scheduling purposes
             * and lies dormant until one of four things happens:
             *
             *
             * @li Some other thread invokes @b unpark with the
             * current thread as the target; or
             *
             * @li Some other thread @em interrupts
             * the current thread; or
             *
             * @li The specified waiting time elapses; or
             *
             * @li The call spuriously (that is, for no reason) returns.
             * </p>
             *
             * <p>
             * This method does <em>not</em> report which of these caused the
             * method to return. Callers should re-check the conditions which caused
             * the thread to park in the first place. Callers may also determine,
             * for example, the interrupt status of the thread, or the elapsed time
             * upon return.
             * </p>
             * @param nanos the maximum number of nanoseconds to wait
             */
            static void parkNanos(glong nanos);

            /**
             * Disables the current thread for thread scheduling purposes, until
             * the specified deadline, unless the permit is available.
             *
             * <p>
             * If the permit is available then it is consumed and the call
             * returns immediately; otherwise the current thread becomes disabled
             * for thread scheduling purposes and lies dormant until one of four
             * things happens:
             *
             *
             * @li Some other thread invokes @b unpark with the
             * current thread as the target; or
             *
             * @li Some other thread @em interrupts
             * the current thread; or
             *
             * @li The specified deadline passes; or
             *
             * @li The call spuriously (that is, for no reason) returns.
             * </p>
             *
             * <p>
             * This method does <em>not</em> report which of these caused the
             * method to return. Callers should re-check the conditions which caused
             * the thread to park in the first place. Callers may also determine,
             * for example, the interrupt status of the thread, or the current time
             * upon return.
             * </p>
             * @param deadline the absolute time, in milliseconds from the Epoch,
             *        to wait until
             */
            static void parkUntil(glong deadline);

            /**
             * Returns the thread id for the given thread.
             */
            static glong getThreadID(Thread const &t);
        };
    } // concurrent
} // core

#endif //CORE24_LOCKSUPPORT_H
