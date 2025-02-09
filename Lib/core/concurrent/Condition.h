//
// Created by bruns on 30/09/2024.
//

#ifndef CORE24_CONDITION_H
#define CORE24_CONDITION_H

#include <core/time/Temporal.h>

namespace core {
    namespace concurrent {
        /**
         * @c Condition factors out the @c Object monitor
         * methods into distinct objects to
         * give the effect of having multiple wait-sets per object, by
         * combining them with the use of arbitrary @b Lock implementations.
         * Where a @c Lock replaces the use of @c synchronized methods
         * and statements, a @c Condition replaces the use of the Object
         * monitor methods.
         *
         * <p>
         * Conditions (also known as <em>condition queues</em> or
         * <em>condition variables</em>) provide a means for one thread to
         * suspend execution (to ¨wait¨) until notified by another
         * thread that some state condition may now be true.  Because access
         * to this shared state information occurs in different threads, it
         * must be protected, so a lock of some form is associated with the
         * condition. The key property that waiting for a condition provides
         * is that it <em>atomically</em> releases the associated lock and
         * suspends the current thread, just like @c Object::wait.
         * </p>
         *
         * <p>
         * A @c Condition instance is intrinsically bound to a lock.
         * To obtain a @c Condition instance for a particular @b Lock
         * instance use its @b newCondition() method.
         * </p>
         *
         * <p>
         * As an example, suppose we have a bounded buffer which supports
         * @c put and @c take methods.  If a
         * @c take is attempted on an empty buffer, then the thread will block
         * until an item becomes available; if a @c put is attempted on a
         * full buffer, then the thread will block until a space becomes available.
         * We would like to keep waiting @c put threads and @c take
         * threads in separate wait-sets so that we can use the optimization of
         * only notifying a single thread at a time when items or spaces become
         * available in the buffer. This can be achieved using two
         * @b Condition instances.
         * @code
         * template <class E>
         * class BoundedBuffer {
         *    Lock &lock = Unsafe::newInstance<ReentrantLock>();
         *    Condition notFull  = lock.newCondition();
         *    Condition notEmpty = lock.newCondition();
         *
         *    Array<> items = Array<Object>(100);
         *    gint putPtr, takePtr, count;
         *
         *  public:
         *   void put(E const &x) throws InterruptedException {
         *     lock.lock();
         *     try {
         *       while (count == items.length())
         *         notFull.await();
         *         items.set(putPtr, x);
         *         if (++putPtr == items.length()) putPtr = 0;
         *         ++count;
         *         notEmpty.notify();
         *         lock.unlock();
         *      } catch(...) { lock.unlock(); }
         *   }
         *
         *   E &take() {
         *     lock.lock();
         *     try {
         *       while (count == 0)
         *          notEmpty.await();
         *       E &x = (E &) items[takePtr];
         *       if (++takePtr == items.length()) takePtr = 0;
         *       --count;
         *       notFull.signal();</b>
         *       lock.unlock();
         *       return x;
         *     } catch(...) { lock.unlock(); }
         *   }
         * };
         * @endcode
         *
         * </p>
         * <p>
         * A @c Condition implementation can provide behavior and semantics
         * that is
         * different from that of the @c Object monitor methods, such as
         * guaranteed ordering for notifications, or not requiring a lock to be held
         * when performing notifications.
         * If an implementation provides such specialized semantics then the
         * implementation must document those semantics.
         * </p>
         *
         * <p>
         * Note that @c Condition instances are just normal objects and can
         * themselves be used as the target in a @c synchronized statement,
         * and can have their own monitor @b wait and
         * @b notify methods invoked.
         * Acquiring the monitor lock of a @c Condition instance, or using its
         * monitor methods, has no specified relationship with acquiring the
         * @b Lock associated with that @c Condition or the use of its
         * @em waiting and @em signalling methods.
         * It is recommended that to avoid confusion you never use @c Condition
         * instances in this way, except perhaps within their own implementation.
         * </p>
         *
         * <h3>Implementation Considerations</h3>
         *
         * <p>
         * When waiting upon a @c Condition, a &quot;<em>spurious
         * wakeup</em>&quot; is permitted to occur, in
         * general, as a concession to the underlying platform semantics.
         * This has little practical impact on most application programs as a
         * @c Condition should always be waited upon in a loop, testing
         * the state predicate that is being waited for.  An implementation is
         * free to remove the possibility of spurious wakeups but it is
         * recommended that applications programmers always assume that they can
         * occur and so always wait in a loop.
         * </p>
         *
         * <p>
         * The three forms of condition waiting
         * (interruptible, non-interruptible, and timed) may differ in their ease of
         * implementation on some platforms and in their performance characteristics.
         * In particular, it may be difficult to provide these features and maintain
         * specific semantics such as ordering guarantees.
         * Further, the ability to interrupt the actual suspension of the thread may
         * not always be feasible to implement on all platforms.
         * </p>
         *
         * <p>
         * Consequently, an implementation is not required to define exactly the
         * same guarantees or semantics for all three forms of waiting, nor is it
         * required to support interruption of the actual suspension of the thread.
         * </p>
         *
         * <p>
         * An implementation is required to
         * clearly document the semantics and guarantees provided by each of the
         * waiting methods, and when an implementation does support interruption of
         * thread suspension then it must obey the interruption semantics as defined
         * in this interface.
         * </p>
         *
         * <p>
         * As interruption generally implies cancellation, and checks for
         * interruption are often infrequent, an implementation can favor responding
         * to an interrupt over normal method return. This is true even if it can be
         * shown that the interrupt occurred after another action that may have
         * unblocked the thread. An implementation should document this behavior.
         * </p>
         */
        class Condition : public virtual Object {
        public:
            /**
             * Causes the current thread to wait until it is signalled or
             * @em interrupted.
             *
             * <p>
             * The lock associated with this @c Condition is atomically
             * released and the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until <em>one</em> of four things happens: <br/>
             *
             *  - Some other thread invokes the @b notify method for this
             * @c Condition and the current thread happens to be chosen as the
             * thread to be awakened; or <br/>
             *  - Some other thread invokes the @b notifyAll method for this
             * @c Condition; or <br/>
             *  - Some other thread @b interrupts the
             * current thread, and interruption of thread suspension is supported; or <br/>
             *  - A &quot;<em>spurious wakeup</em>&quot; occurs.
             *
             * </p>
             *
             * <p>
             * In all cases, before this method can return the current thread must
             * re-acquire the lock associated with this condition. When the
             * thread returns it is <em>guaranteed</em> to hold this lock.
             * </p>
             *
             * <p>
             * If the current thread: <br/>
             *
             *  - has its interrupted status set on entry to this method; or <br/>
             *  - is @em interrupted while waiting <br/>
             * and interruption of thread suspension is supported,
             *
             * then @b InterruptedException is thrown and the current thread's
             * interrupted status is cleared. It is not specified, in the first
             * case, whether the test for interruption occurs before the lock
             * is released.
             * </p>
             *
             * <p>
             * <b>Implementation Considerations</b>
             * </p>
             *
             * <p>
             * The current thread is assumed to hold the lock associated with this
             * @c Condition when this method is called.
             * It is up to the implementation to determine if this is
             * the case and if not, how to respond. Typically, an exception will be
             * thrown (such as @b IllegalMonitorStateException) and the
             * implementation must document that fact.
             * </p>
             *
             * <p>
             * An implementation can favor responding to an interrupt over normal
             * method return in response to a signal. In that case the implementation
             * must ensure that the signal is redirected to another waiting thread, if
             * there is one.
             * </p>
             *
             * @throws InterruptedException if the current thread is interrupted
             *         (and interruption of thread suspension is supported)
             */
            virtual void await() = 0;

            /**
             * Causes the current thread to wait until it is signalled.
             *
             * <p>
             * The lock associated with this condition is atomically
             * released and the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until <em>one</em> of three things happens: <br/>
             *
             *  - Some other thread invokes the @b notify method for this
             * @c Condition and the current thread happens to be chosen as the
             * thread to be awakened; or <br/>
             *  - Some other thread invokes the @b notifyAll method for this
             * @c Condition; or <br/>
             *  - A &quot;<em>spurious wakeup</em>&quot; occurs. <br/>
             * </p>
             *
             * <p>
             * In all cases, before this method can return the current thread must
             * re-acquire the lock associated with this condition. When the
             * thread returns it is <em>guaranteed</em> to hold this lock.
             * </p>
             *
             * <p>
             * If the current thread's interrupted status is set when it enters
             * this method, or it is @em interrupted
             * while waiting, it will continue to wait until signalled. When it finally
             * returns from this method its interrupted status will still
             * be set.
             * </p>
             *
             * <p>
             * <b>Implementation Considerations</b>
             * </p>
             *
             * <p>
             * The current thread is assumed to hold the lock associated with this
             * @c Condition when this method is called.
             * It is up to the implementation to determine if this is
             * the case and if not, how to respond. Typically, an exception will be
             * thrown (such as @b IllegalMonitorStateException) and the
             * implementation must document that fact.
             * </p>
             */
            virtual void awaitUninterruptibly() = 0;

            /**
             * Causes the current thread to wait until it is signalled or interrupted,
             * or the specified waiting time elapses.
             *
             * <p>
             * The lock associated with this condition is atomically
             * released and the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until <em>one</em> of five things happens: <br/>
             *
             *  - Some other thread invokes the @b notify method for this
             * @c Condition and the current thread happens to be chosen as the
             * thread to be awakened; or <br/>
             *  - Some other thread invokes the @b notifyAll method for this
             * @c Condition; or <br/>
             *  - Some other thread @b interrupts the
             * current thread, and interruption of thread suspension is supported; or <br/>
             *  - The specified waiting time elapses; or <br/>
             *  - A &quot;<em>spurious wakeup</em>&quot; occurs. <br/>
             *
             * </p>
             *
             * <p>
             * In all cases, before this method can return the current thread must
             * re-acquire the lock associated with this condition. When the
             * thread returns it is <em>guaranteed</em> to hold this lock.
             * </p>
             *
             * <p>
             * If the current thread: <br/>
             *
             *  - has its interrupted status set on entry to this method; or <br/>
             *  - is @em interrupted while waiting <br/>
             * and interruption of thread suspension is supported,
             *
             * then @b InterruptedException is thrown and the current thread's
             * interrupted status is cleared. It is not specified, in the first
             * case, whether the test for interruption occurs before the lock
             * is released.
             * </p>
             *
             * <p>
             * The method returns an estimate of the number of nanoseconds
             * remaining to wait given the supplied @c nanosTimeout
             * value upon return, or a value less than or equal to zero if it
             * timed out. This value can be used to determine whether and how
             * long to re-wait in cases where the wait returns but an awaited
             * condition still does not hold. Typical uses of this method take
             * the following form:
             *
             * @code
             *  gbool aMethod(glong timeout, ChronoUnit unit)  {
             *   glong nanosRemaining = unit.toNanos(timeout);
             *   lock.lock();
             *   try {
             *     while (!conditionBeingWaitedFor()) {
             *       if (nanosRemaining <= 0L)
             *         return false;
             *       nanosRemaining = theCondition.awaitNanos(nanosRemaining);
             *     }
             *     // ...
             *     lock.unlock();
             *     return true;
             *   } catch(...) { lock.unlock(); }
             * }
             * @endcode
             * </p>
             *
             * <p>
             * Design note: This method requires a nanosecond argument
             * to avoid truncation errors in reporting remaining times.
             * Such precision loss would make it difficult for programmers to
             * ensure that total waiting times are not systematically shorter
             * than specified when re-waits occur.
             * </p>
             *
             * <p>
             * <b>Implementation Considerations</b>
             * </p>
             *
             * <p>
             * The current thread is assumed to hold the lock associated with this
             * @c Condition when this method is called.
             * It is up to the implementation to determine if this is
             * the case and if not, how to respond. Typically, an exception will be
             * thrown (such as @b IllegalMonitorStateException) and the
             * implementation must document that fact.
             * </p>
             *
             * <p>
             * An implementation can favor responding to an interrupt over normal
             * method return in response to a signal, or over indicating the elapse
             * of the specified waiting time. In either case the implementation
             * must ensure that the signal is redirected to another waiting thread, if
             * there is one.
             * </p>
             * @param nanosTimeout the maximum time to wait, in nanoseconds
             * @return an estimate of the @c nanosTimeout value minus
             *         the time spent waiting upon return from this method.
             *         A positive value may be used as the argument to a
             *         subsequent call to this method to finish waiting out
             *         the desired time.  A value less than or equal to zero
             *         indicates that no time remains.
             * @throws InterruptedException if the current thread is interrupted
             *         (and interruption of thread suspension is supported)
             */
            virtual glong awaitNanos(glong nanosTimeout) = 0;

            /**
             * Causes the current thread to wait until it is signalled or interrupted,
             * or the specified waiting time elapses. This method is behaviorally
             * equivalent to:
             * @code awaitNanos(unit.toNanos(time)) > 0 @endcode
             *
             * @param time the maximum time to wait
             * @param unit the time unit of the @c time argument
             * @return @c false if the waiting time detectably elapsed
             *         before return from the method, else @c true
             * @throws InterruptedException if the current thread is interrupted
             *         (and interruption of thread suspension is supported)
             */
            virtual gbool await(glong time, time::Temporal::ChronoUnit unit) = 0;

            /**
             * Causes the current thread to wait until it is signalled or interrupted,
             * or the specified deadline elapses.
             *
             * <p>
             * The lock associated with this condition is atomically
             * released and the current thread becomes disabled for thread scheduling
             * purposes and lies dormant until <em>one</em> of five things happens: <br/>
             *
             *  - Some other thread invokes the @b notify method for this
             * @c Condition and the current thread happens to be chosen as the
             * thread to be awakened; or <br/>
             *  - Some other thread invokes the @b notifyAll method for this
             * @c Condition; or <br/>
             *  - Some other thread @b interrupts the
             * current thread, and interruption of thread suspension is supported; or <br/>
             *  - The specified deadline elapses; or <br/>
             *  - A &quot;<em>spurious wakeup</em>&quot; occurs. <br/>
             * </p>
             *
             * <p>
             * In all cases, before this method can return the current thread must
             * re-acquire the lock associated with this condition. When the
             * thread returns it is <em>guaranteed</em> to hold this lock.
             * </p>
             *
             * <p>
             * If the current thread: <br/>
             *
             *  - has its interrupted status set on entry to this method; or <br/>
             *  - is @em interrupted while waiting <br/>
             * and interruption of thread suspension is supported,
             *
             * then @b InterruptedException is thrown and the current thread's
             * interrupted status is cleared. It is not specified, in the first
             * case, whether or not the test for interruption occurs before the lock
             * is released.
             * </p>
             *
             * <p>
             * The return value indicates whether the deadline has elapsed,
             * which can be used as follows:
             * @code
             *  gbool aMethod(LocalDateTime deadline) {
             *    gbool stillWaiting = true;
             *    lock.lock();
             *    try {
             *     while (!conditionBeingWaitedFor()) {
             *       if (!stillWaiting)
             *         return false;
             *       stillWaiting = theCondition.awaitUntil(deadline);
             *     }
             *     // ...
             *     lock.unlock();
             *     return true;
             *   } catch(...) { lock.unlock(); }
             * }
             * @endcode
             * </p>
             *
             * <p>
             * <b>Implementation Considerations</b>
             * </p>
             *
             * <p>
             * The current thread is assumed to hold the lock associated with this
             * @c Condition when this method is called.
             * It is up to the implementation to determine if this is
             * the case and if not, how to respond. Typically, an exception will be
             * thrown (such as @b IllegalMonitorStateException) and the
             * implementation must document that fact.
             * </p>
             *
             * <p>
             * An implementation can favor responding to an interrupt over normal
             * method return in response to a signal, or over indicating the passing
             * of the specified deadline. In either case the implementation
             * must ensure that the signal is redirected to another waiting thread, if
             * there is one.
             * </p>
             *
             * @param deadline the absolute time to wait until
             * @return @c false if the deadline has elapsed upon return, else
             *         @c true
             * @throws InterruptedException if the current thread is interrupted
             *         (and interruption of thread suspension is supported)
             */
            virtual gbool awaitUntil(time::LocalDateTime const &deadline) = 0;

            /**
             * Wakes up one waiting thread.
             *
             * <p>
             * If any threads are waiting on this condition then one
             * is selected for waking up. That thread must then re-acquire the
             * lock before returning from @c await.
             * </p>
             *
             * <p>
             * <b>Implementation Considerations</b>
             * </p>
             *
             * <p>
             * An implementation may (and typically does) require that the
             * current thread hold the lock associated with this
             * @c Condition when this method is called. Implementations must
             * document this precondition and any actions taken if the lock is
             * not held. Typically, an exception such as
             * @b IllegalMonitorStateException will be thrown.
             * </p>
             */
            virtual void notify() = 0;

            /**
             * Wakes up all waiting threads.
             *
             * <p>
             * If any threads are waiting on this condition then they are
             * all woken up. Each thread must re-acquire the lock before it can
             * return from @c await.
             * </p>
             *
             * <p>
             * <b>Implementation Considerations</b>
             * </p>
             *
             * <p>
             * An implementation may (and typically does) require that the
             * current thread hold the lock associated with this
             * @c Condition when this method is called. Implementations must
             * document this precondition and any actions taken if the lock is
             * not held. Typically, an exception such as
             * @b IllegalMonitorStateException will be thrown.
             * </p>
             */
            virtual void notifyAll() = 0;
        };
    } // concurrent
} // core

#endif //CORE24_CONDITION_H
