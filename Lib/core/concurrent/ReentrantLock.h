//
// Created by bruns on 01/10/2024.
//

#ifndef CORE24_REENTRANTLOCK_H
#define CORE24_REENTRANTLOCK_H

#include <core/concurrent/Lock.h>
#include <core/concurrent/QueuedSynchronizer.h>

namespace core {
    namespace concurrent {
        /**
         * A reentrant mutual exclusion @b Lock with the same basic
         * behavior and semantics as the implicit monitor lock accessed using
         * @c synchronized methods and statements, but with extended
         * capabilities.
         *
         * <p>
         * A @c ReentrantLock is <em>owned</em> by the thread last
         * successfully locking, but not yet unlocking it. A thread invoking
         * @c lock will return, successfully acquiring the lock, when
         * the lock is not owned by another thread. The method will return
         * immediately if the current thread already owns the lock. This can
         * be checked using methods @b isHeldByCurrentThread, and
         * @b holdCount.
         * </p>
         * <p>
         * The constructor for this class accepts an optional
         * <em>fairness</em> parameter.  When set @c true, under
         * contention, concurrent favor granting access to the longest-waiting
         * thread.  Otherwise, this lock does not guarantee any particular
         * access order.  Programs using fair concurrent accessed by many threads
         * may display lower overall throughput (i.e., are slower; often much
         * slower) than those using the default setting, but have smaller
         * variances in times to obtain concurrent and guarantee lack of
         * starvation. Note however, that fairness of concurrent does not guarantee
         * fairness of thread scheduling. Thus, one of many threads using a
         * fair lock may obtain it multiple times in succession while other
         * active threads are not progressing and not currently holding the
         * lock.
         * Also note that the untimed @b tryLock() method does not
         * honor the fairness setting. It will succeed if the lock
         * is available even if other threads are waiting.
         * </p>
         * <p>
         * It is recommended practice to <em>always</em> immediately
         * follow a call to @c lock with a @c try block, most
         * typically in a before/after construction such as:
         *
         * @code
         * class X {
         *   ReentrantLock lock;
         *   // ...
         *
         *   public :
         *   void m() {
         *     lock.lock();  // block until condition holds
         *     try {
         *       // ... method body
         *       lock.unlock();
         *     } catch(...) { lock.unlock(); }
         *   }
         * };
         * @endcode
         * </p>
         * <p>
         * In addition to implementing the @b Lock interface, this
         * class defines a number of @c public and @c protected
         * methods for inspecting the state of the lock.  Some of these
         * methods are only useful for instrumentation and monitoring.
         * </p>
         * <p>
         * Serialization of this class behaves in the same way as built-in
         * concurrent: a deserialized lock is in the unlocked state, regardless of
         * its state when serialized.
         * </p>
         * <p>
         * This lock supports a maximum of 2147483647 recursive concurrent by
         * the same thread. Attempts to exceed this limit result in
         * @b Error throws from locking methods.
         * </p>
         */
        class ReentrantLock : public virtual Lock {
            class Synchronizer;
            class FairSync;
            class NonFairSync;

            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(Sync, Class<Synchronizer>::Pointer);

            /** Synchronizer providing all implementation mechanics */
            Sync sync = {};

        public:
            /**
             * Creates an instance of @c ReentrantLock.
             * This is equivalent to using @c ReentrantLock(false).
             */
            CORE_IMPLICIT ReentrantLock();

            /**
             * Creates an instance of @c ReentrantLock with the
             * given fairness policy.
             *
             * @param fair @c true if this lock should use a fair ordering policy
             */
            CORE_EXPLICIT ReentrantLock(gbool fair);

            /**
             * Acquires the lock.
             *
             * <p>
             * Acquires the lock if it is not held by another thread and returns
             * immediately, setting the lock hold count to one.
             * </p>
             * <p>
             * If the current thread already holds the lock then the hold
             * count is incremented by one and the method returns immediately.
             * </p>
             * <p>
             * If the lock is held by another thread then the
             * current thread becomes disabled for thread scheduling
             * purposes and lies dormant until the lock has been acquired,
             * at which time the lock hold count is set to one.
             * </p>
             */
            void lock() override;

            /**
             * Acquires the lock unless the current thread is
             * @b interrupted.
             *
             * <p>
             * Acquires the lock if it is not held by another thread and returns
             * immediately, setting the lock hold count to one.
             * </p>
             * <p>
             * If the current thread already holds this lock then the hold count
             * is incremented by one and the method returns immediately.
             * </p>
             * <p>
             * If the lock is held by another thread then the
             * current thread becomes disabled for thread scheduling
             * purposes and lies dormant until one of two things happens:
             *
             *
             * @li The lock is acquired by the current thread; or
             *
             * @li Some other thread @b interrupts the
             * current thread.
             * </p>
             *
             * <p>
             * If the lock is acquired by the current thread then the lock hold
             * count is set to one.
             * </p>
             * <p>
             * If the current thread:
             *
             * @li has its interrupted status set on entry to this method; or
             *
             * @li is @b interrupted while acquiring
             * the lock,
             *
             * then @b InterruptedException is thrown and the current thread's
             * interrupted status is cleared.
             * </p>
             * <p>
             * In this implementation, as this method is an explicit
             * interruption point, preference is given to responding to the
             * interrupt over normal or reentrant acquisition of the lock.
             * </p>
             * @throws InterruptedException if the current thread is interrupted
             */
            void lockInterruptibly() override;

            /**
             * Acquires the lock only if it is not held by another thread at the time
             * of invocation.
             *
             * <p>
             * Acquires the lock if it is not held by another thread and
             * returns immediately with the value @c true, setting the
             * lock hold count to one. Even when this lock has been set to use a
             * fair ordering policy, a call to @c tryLock() <em>will</em>
             * immediately acquire the lock if it is available, whether
             * other threads are currently waiting for the lock.
             * This &quot;barging&quot; behavior can be useful in certain
             * circumstances, even though it breaks fairness. If you want to honor
             * the fairness setting for this lock, then use
             * <b> tryLock(0, ChronoUnit::SECONDS)</b>
             * which is almost equivalent (it also detects interruption).
             * </p>
             * <p>
             * If the current thread already holds this lock then the hold
             * count is incremented by one and the method returns @c true.
             * </p>
             * <p>
             * If the lock is held by another thread then this method will return
             * immediately with the value @c false.
             * </p>
             * @return @c true if the lock was free and was acquired by the
             *         current thread, or the lock was already held by the current
             *         thread; and @c false otherwise
             */
            gbool tryLock() override;

            /**
             * Acquires the lock if it is not held by another thread within the given
             * waiting time and the current thread has not been
             * @b interrupted.
             *
             * <p>
             * Acquires the lock if it is not held by another thread and returns
             * immediately with the value @c true, setting the lock hold count
             * to one. If this lock has been set to use a fair ordering policy then
             * an available lock <em>will not</em> be acquired if any other threads
             * are waiting for the lock. This is in contrast to the @b tryLock()
             * method. If you want a timed @c tryLock that does permit barging on
             * a fair lock then combine the timed and un-timed forms together:
             *
             * @code
             * if (lock.tryLock() ||lock.tryLock(timeout, unit)) {
             *   ...
             * }
             * @endcode
             * </p>
             * <p>
             * If the current thread
             * already holds this lock then the hold count is incremented by one and
             * the method returns @c true.
             * </p>
             * <p>
             * If the lock is held by another thread then the
             * current thread becomes disabled for thread scheduling
             * purposes and lies dormant until one of three things happens:
             *
             *
             * @li The lock is acquired by the current thread; or
             *
             * @li Some other thread @b interrupts
             * the current thread; or
             *
             * @li The specified waiting time elapses
             * </p>
             *
             * <p>
             * If the lock is acquired then the value @c true is returned and
             * the lock hold count is set to one.
             * </p>
             * <p>
             * If the current thread:
             *
             * @li has its interrupted status set on entry to this method; or
             *
             * @li is @b interrupted while
             * acquiring the lock,
             *
             * then @b InterruptedException is thrown and the current thread's
             * interrupted status is cleared.
             * </p>
             * <p>
             * If the specified waiting time elapses then the value @c false
             * is returned.  If the time is less than or equal to zero, the method
             * will not wait at all.
             * </p>
             * <p>
             * In this implementation, as this method is an explicit
             * interruption point, preference is given to responding to the
             * interrupt over normal or reentrant acquisition of the lock, and
             * over reporting the elapsed of the waiting time.
             * </p>
             * @param timeout the time to wait for the lock
             * @param unit the time unit of the timeout argument
             * @return @c true if the lock was free and was acquired by the
             *         current thread, or the lock was already held by the current
             *         thread; and @c false if the waiting time elapsed before
             *         the lock could be acquired
             * @throws InterruptedException if the current thread is interrupted
             */
            gbool tryLock(glong timeout, time::Temporal::ChronoUnit unit) override;

            /**
             * Attempts to release this lock.
             *
             * <p>
             * If the current thread is the holder of this lock then the hold
             * count is decremented.  If the hold count is now zero then the lock
             * is released.  If the current thread is not the holder of this
             * lock then @b IllegalStateException is thrown.
             * </p>
             * @throws IllegalStateException if the current thread does not
             *         hold this lock
             */
            void unlock() override;

            /**
             * Returns a @b Condition instance for use with this
             * @b Lock instance.
             *
             *
             * @li If this lock is not held when any of the @b Condition
             * @em waiting or @em signalling methods are called,
             * then an @b IllegalStateException is thrown.
             *
             * @li When the condition @em waiting
             * methods are called the lock is released and, before they
             * return, the lock is reacquired and the lock hold count restored
             * to what it was when the method was called.
             *
             * @li If a thread is @b interrupted
             * while waiting then the wait will terminate, an
             * @b InterruptedException will be thrown, and the thread's
             * interrupted status will be cleared.
             *
             * @li Waiting threads are signalled in FIFO order.
             *
             * @li The ordering of lock reacquisition for threads returning
             * from waiting methods is the same as for threads initially
             * acquiring the lock, which is in the default case not specified,
             * but for <em>fair</em> concurrent favors those threads that have been
             * waiting the longest.
             *
             *
             * @return the Condition object
             */
            Condition &newCondition() override;

            /**
             * Queries the number of holds on this lock by the current thread.
             *
             * <p>
             * A thread has a hold on a lock for each lock action that is not
             * matched by an unlock action.
             * </p>
             * <p>
             * The hold count information is typically only used for testing and
             * debugging purposes. For example, if a certain section of code should
             * not be entered with the lock already held then we can assert that
             * fact:
             *
             * @code
             * class X {
             *   final ReentrantLock lock = new ReentrantLock();
             *   // ...
             *   public void m() {
             *     assert lock.getHoldCount() == 0;
             *     lock.lock();
             *     try {
             *       // ... method body
             *     } finally {
             *       lock.unlock();
             *     }
             *   }
             * }
             * @endcode
             * </p>
             * @return the number of holds on this lock by the current thread,
             *         or zero if this lock is not held by the current thread
             */
            virtual gint holdCount() const;

            /**
             * Queries if this lock is held by the current thread.
             *
             * <p>
             * Analogous to the @b Thread::holdsLock(Object) method for
             * built-in monitor concurrent, this method is typically used for
             * debugging and testing. For example, a method that should only be
             * called while a lock is held can assert that this is the case:
             *
             * @code
             * class X {
             *   ReentrantLock lock;
             *   // ...
             *
             *   public :
             *   void m() {
             *       CORE_ASSERT(lock.isHeldByCurrentThread());
             *       // ... method body
             *   }
             * }
             * @endcode
             * </p>
             * <p>
             * It can also be used to ensure that a reentrant lock is used
             * in a non-reentrant manner, for example:
             *
             * @code
             * class X {
             *   final ReentrantLock lock = new ReentrantLock();
             *   // ...
             *
             *   public void m() {
             *       assert !lock.isHeldByCurrentThread();
             *       lock.lock();
             *       try {
             *           // ... method body
             *           lock.unlock();
             *       } catch(...) { lock.unlock(); }
             *   }
             * };
             * @endcode
             * </p>
             * @return @c true if current thread holds this lock and
             *         @c false otherwise
             */
            gbool isHeldByCurrentThread() const;

            /**
             * Queries if this lock is held by any thread. This method is
             * designed for use in monitoring of the system state,
             * not for synchronization control.
             *
             * @return @c true if any thread holds this lock and
             *         @c false otherwise
             */
            virtual gbool isLocked() const;

            /**
             * Returns @c true if this lock has fairness set true.
             *
             * @return @c true if this lock has fairness set true
             */
            virtual gbool isFair() const;

            /**
             * Queries whether any threads are waiting to acquire this lock. Note that
             * because cancellations may occur at any time, a @c true
             * return does not guarantee that any other thread will ever
             * acquire this lock.  This method is designed primarily for use in
             * monitoring of the system state.
             *
             * @return @c true if there may be other threads waiting to
             *         acquire the lock
             */
            virtual gbool hasQueuedThreads() const;

            /**
             * Queries whether the given thread is waiting to acquire this
             * lock. Note that because cancellations may occur at any time, a
             * @c true return does not guarantee that this thread
             * will ever acquire this lock.  This method is designed primarily for use
             * in monitoring of the system state.
             *
             * @param thread the thread
             * @return @c true if the given thread is queued waiting for this lock
             */
            virtual gbool hasQueuedThreads(Thread const &thread) const;

            /**
             * Returns an estimate of the number of threads waiting to acquire
             * this lock.  The value is only an estimate because the number of
             * threads may change dynamically while this method traverses
             * internal data structures.  This method is designed for use in
             * monitoring system state, not for synchronization control.
             *
             * @return the estimated number of threads waiting for this lock
             */
            virtual gint getQueueLength() const;

            /**
             * Queries whether any threads are waiting on the given condition
             * associated with this lock. Note that because timeouts and
             * interrupts may occur at any time, a @c true return does
             * not guarantee that a future @c signal will awaken any
             * threads.  This method is designed primarily for use in
             * monitoring of the system state.
             *
             * @param condition the condition
             * @return @c true if there are any waiting threads
             * @throws IllegalStateException if this lock is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this lock
             */
            virtual gbool hasWaiters(Condition const &condition) const;

            /**
             * Returns an estimate of the number of threads waiting on the
             * given condition associated with this lock. Note that because
             * timeouts and interrupts may occur at any time, the estimate
             * serves only as an upper bound on the actual number of waiters.
             * This method is designed for use in monitoring of the system
             * state, not for synchronization control.
             *
             * @param condition the condition
             * @return the estimated number of waiting threads
             * @throws IllegalStateException if this lock is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this lock
             */
            virtual gint getWaitQueueLength(Condition const &condition) const;

            /**
             * Returns a string identifying this lock, as well as its lock state.
             * The state, in brackets, includes either the String @c "Unlocked"
             * or the String @c "Locked by" followed by the
             * @em name of the owning thread.
             *
             * @return a string identifying this lock, as well as its lock state
             */
            String toString() const override;

            ~ReentrantLock() override;

        protected:
            /**
             * Returns the thread that currently owns this lock, or
             * @c null if not owned. When this method is called by a
             * thread that is not the owner, the return value reflects a
             * best-effort approximation of current lock status. For example,
             * the owner may be momentarily @c null even if there are
             * threads trying to acquire the lock but have not yet done so.
             * This method is designed to facilitate construction of
             * subclasses that provide more extensive lock monitoring
             * facilities.
             *
             * @return the owner, or @c null if not owned
             */
            virtual Object &getOwner() const;

            /**
             * Returns a collection containing threads that may be waiting to
             * acquire this lock.  Because the actual set of threads may change
             * dynamically while constructing this result, the returned
             * collection is only a best-effort estimate.  The elements of the
             * returned collection are in no particular order.  This method is
             * designed to facilitate construction of subclasses that provide
             * more extensive monitoring facilities.
             *
             * @return the collection of threads
             */
            virtual util::ArrayList<Thread> getQueuedThreads() const;

            /**
             * Returns a collection containing those threads that may be
             * waiting on the given condition associated with this lock.
             * Because the actual set of threads may change dynamically while
             * constructing this result, the returned collection is only a
             * best-effort estimate. The elements of the returned collection
             * are in no particular order.  This method is designed to
             * facilitate construction of subclasses that provide more
             * extensive condition monitoring facilities.
             *
             * @param condition the condition
             * @return the collection of threads
             * @throws IllegalStateException if this lock is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this lock
             */
            virtual util::ArrayList<Thread> getWaiters(Condition const &condition) const;
        };

        /**
         * Base of synchronization control for this lock. Subclassed
         * into fair and non-fair versions below. Uses QS state to
         * represent the number of holds on the lock.
         */
        class ReentrantLock::Synchronizer : public virtual QueuedSynchronizer {
            CORE_ADD_AS_FRIEND(ReentrantLock);

            /**
             * Performs non-fair tryLock.
             */
            gbool tryLock();

            /**
             * Checks for reentrancy and acquires if lock immediately
             * available under fair vs non-fair rules. Locking methods
             * perform initialTryLock check before relaying to
             * corresponding AQS acquire methods.
             */
            virtual gbool initialTryLock() = 0;

            void lock();

            void lockInterruptibly();

            gbool tryLockNanos(glong nanos);

        protected:
            gbool tryRelease(gint releases) override;

            gbool isHeldExclusively() const override;

            virtual ConditionObject &newCondition();

            // Methods relayed from outer class

            virtual Object &getOwner() const;

            virtual gint holdCount() const;

            virtual gbool isLocked() const;
        };

        /**
         * Sync object for non-fair concurrent
         */
        class ReentrantLock::NonFairSync final : public virtual Synchronizer {
            gbool initialTryLock() override;

        protected:
            /**
             * Acquire for non-reentrant cases after initialTryLock pre-screen
             */
            gbool tryAcquire(gint acquires) override;
        };

        /**
         * Sync object for fair concurrent
         */
        class ReentrantLock::FairSync final : public virtual Synchronizer {
            /**
             * Acquires only if reentrant or queue is empty.
             */
            gbool initialTryLock() override;

        protected:
            /**
             * Acquires only if thread is first waiter or empty
             */
            gbool tryAcquire(gint acquires) override;
        };
    } // concurrent
} // core

#endif //CORE24_REENTRANTLOCK_H
