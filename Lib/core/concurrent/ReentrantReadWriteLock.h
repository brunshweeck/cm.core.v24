//
// Created by bruns on 06/10/2024.
//

#ifndef CORE24_REENTRANTREADWRITELOCK_H
#define CORE24_REENTRANTREADWRITELOCK_H

#include <core/concurrent/Lock.h>
#include <core/concurrent/LockSupport.h>
#include <core/concurrent/QueuedSynchronizer.h>
#include <core/concurrent/ReadWriteLock.h>

namespace core {
    namespace concurrent {
        class ReentrantReadWriteLock : public virtual ReadWriteLock {
            class Synchronizer;
            class FairSync;
            class NonFairSync;
            class ReadLock;
            class WriteLock;

            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(Sync, Class<Synchronizer>::Pointer);
            CORE_ALIAS(ReaderLock, Class<ReadLock>::Pointer);
            CORE_ALIAS(WriterLock, Class<WriteLock>::Pointer);

            /** Inner class providing readLock */
            ReaderLock readerLock = {};

            /** Inner class providing writeLock */
            WriterLock writerLock = {};

            /** Performs all synchronization mechanics */
            Sync sync = {};

        public:
            /**
             * Creates a new {@code ReentrantReadWriteLock} with
             * default (non-fair) ordering properties.
             */
            CORE_IMPLICIT ReentrantReadWriteLock();

            /**
             * Creates a new {@code ReentrantReadWriteLock} with
             * the given fairness policy.
             *
             * @param fair {@code true} if this lock should use a fair ordering policy
             */
            CORE_EXPLICIT ReentrantReadWriteLock(gbool fair);

            Lock &readLock() const override;

            Lock &writeLock() const override;

            /**
             * Returns {@code true} if this lock has fairness set true.
             *
             * @return {@code true} if this lock has fairness set true
             */
            virtual gbool isFair() const;

            /**
             * Returns the thread that currently owns the write lock, or
             * {@code null} if not owned. When this method is called by a
             * thread that is not the owner, the return value reflects a
             * best-effort approximation of current lock status. For example,
             * the owner may be momentarily {@code null} even if there are
             * threads trying to acquire the lock but have not yet done so.
             * This method is designed to facilitate construction of
             * subclasses that provide more extensive lock monitoring
             * facilities.
             *
             * @return the owner, or {@code null} if not owned
             */
            virtual Object &getOwner() const;

            /**
             * Queries the number of read concurrent held for this lock. This
             * method is designed for use in monitoring system state, not for
             * synchronization control.
             * @return the number of read concurrent held
             */
            virtual gint getReadLockCount() const;

            /**
             * Queries if the write lock is held by any thread. This method is
             * designed for use in monitoring system state, not for
             * synchronization control.
             *
             * @return {@code true} if any thread holds the write lock and
             *         {@code false} otherwise
             */
            virtual gbool isWriteLocked() const;

            /**
             * Queries if the write lock is held by the current thread.
             *
             * @return {@code true} if the current thread holds the write lock and
             *         {@code false} otherwise
             */
            virtual gbool isWriteLockedByCurrentThread() const;

            /**
             * Queries the number of reentrant write holds on this lock by the
             * current thread.  A writer thread has a hold on a lock for
             * each lock action that is not matched by an unlock action.
             *
             * @return the number of holds on the write lock by the current thread,
             *         or zero if the write lock is not held by the current thread
             */
            virtual gint getWriteHoldCount() const;

            /**
             * Queries the number of reentrant read holds on this lock by the
             * current thread.  A reader thread has a hold on a lock for
             * each lock action that is not matched by an unlock action.
             *
             * @return the number of holds on the read lock by the current thread,
             *         or zero if the read lock is not held by the current thread
             * @since 1.6
             */
            virtual gint getReadHoldCount() const;

            /**
             * Returns a collection containing threads that may be waiting to
             * acquire the write lock.  Because the actual set of threads may
             * change dynamically while constructing this result, the returned
             * collection is only a best-effort estimate.  The elements of the
             * returned collection are in no particular order.  This method is
             * designed to facilitate construction of subclasses that provide
             * more extensive lock monitoring facilities.
             *
             * @return the collection of threads
             */
            virtual util::ArrayList<Thread> getQueuedWriterThreads() const;

            /**
             * Returns a collection containing threads that may be waiting to
             * acquire the read lock.  Because the actual set of threads may
             * change dynamically while constructing this result, the returned
             * collection is only a best-effort estimate.  The elements of the
             * returned collection are in no particular order.  This method is
             * designed to facilitate construction of subclasses that provide
             * more extensive lock monitoring facilities.
             *
             * @return the collection of threads
             */
            virtual util::ArrayList<Thread> getQueuedReaderThreads() const;

            /**
             * Queries whether any threads are waiting to acquire the read or
             * write lock. Note that because cancellations may occur at any
             * time, a {@code true} return does not guarantee that any other
             * thread will ever acquire a lock.  This method is designed
             * primarily for use in monitoring of the system state.
             *
             * @return {@code true} if there may be other threads waiting to
             *         acquire the lock
             */
            virtual gbool hasQueuedThreads() const;

            /**
             * Queries whether the given thread is waiting to acquire either
             * the read or write lock. Note that because cancellations may
             * occur at any time, a {@code true} return does not guarantee
             * that this thread will ever acquire a lock.  This method is
             * designed primarily for use in monitoring of the system state.
             *
             * @param thread the thread
             * @return {@code true} if the given thread is queued waiting for this lock
             * @throws NullPointerException if the thread is null
             */
            virtual gbool hasQueuedThread(Thread const &thread) const;

            /**
             * Returns an estimate of the number of threads waiting to acquire
             * either the read or write lock.  The value is only an estimate
             * because the number of threads may change dynamically while this
             * method traverses internal data structures.  This method is
             * designed for use in monitoring system state, not for
             * synchronization control.
             *
             * @return the estimated number of threads waiting for this lock
             */
            virtual gint getQueueLength() const;

            /**
             * Queries whether any threads are waiting on the given condition
             * associated with the write lock. Note that because timeouts and
             * interrupts may occur at any time, a {@code true} return does
             * not guarantee that a future {@code signal} will awaken any
             * threads.  This method is designed primarily for use in
             * monitoring of the system state.
             *
             * @param condition the condition
             * @return {@code true} if there are any waiting threads
             * @throws IllegalMonitorStateException if this lock is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this lock
             */
            virtual gbool hasWaiters(Condition const &condition) const;

            /**
             * Returns an estimate of the number of threads waiting on the
             * given condition associated with the write lock. Note that because
             * timeouts and interrupts may occur at any time, the estimate
             * serves only as an upper bound on the actual number of waiters.
             * This method is designed for use in monitoring of the system
             * state, not for synchronization control.
             *
             * @param condition the condition
             * @return the estimated number of waiting threads
             * @throws IllegalMonitorStateException if this lock is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this lock
             */
            virtual gbool getWaitQueueLength(Condition const &condition) const;

            /**
             * Returns a string identifying this lock, as well as its lock state.
             * The state, in brackets, includes the String {@code "Write concurrent ="}
             * followed by the number of reentrantly held write concurrent, and the
             * String {@code "Read concurrent ="} followed by the number of held
             * read concurrent.
             *
             * @return a string identifying this lock, as well as its lock state
             */
            String toString() const override;

        protected:
            /**
             * Returns a collection containing threads that may be waiting to
             * acquire either the read or write lock.  Because the actual set
             * of threads may change dynamically while constructing this
             * result, the returned collection is only a best-effort estimate.
             * The elements of the returned collection are in no particular
             * order.  This method is designed to facilitate construction of
             * subclasses that provide more extensive monitoring facilities.
             *
             * @return the collection of threads
             */
            virtual util::ArrayList<Thread> getQueuedThreads() const = 0;

            /**
             * Returns a collection containing those threads that may be
             * waiting on the given condition associated with the write lock.
             * Because the actual set of threads may change dynamically while
             * constructing this result, the returned collection is only a
             * best-effort estimate. The elements of the returned collection
             * are in no particular order.  This method is designed to
             * facilitate construction of subclasses that provide more
             * extensive condition monitoring facilities.
             *
             * @param condition the condition
             * @return the collection of threads
             * @throws IllegalMonitorStateException if this lock is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this lock
             */
            virtual util::ArrayList<Thread> getWaitingThreads(Condition const &condition) const;
        };

        class ReentrantReadWriteLock::Synchronizer : public QueuedSynchronizer {
            CORE_ALIAS(THREAD, Class<Thread>::Pointer);

            /*
             * Read vs write count extraction constants and functions.
             * Lock state is logically divided into two unsigned shorts:
             * The lower one representing the exclusive (writer) lock hold count,
             * and the upper the shared (reader) hold count.
             */

            static CORE_FAST gint SHARED_SHIFT = 16;
            static CORE_FAST gint SHARED_UNIT = (1 << SHARED_SHIFT);
            static CORE_FAST gint MAX_COUNT = (1 << SHARED_SHIFT) - 1;
            static CORE_FAST gint EXCLUSIVE_MASK = (1 << SHARED_SHIFT) - 1;

            /** Returns the number of shared holds represented in count. */
            static gint sharedCount(gint c);

            /** Returns the number of exclusive holds represented in count. */
            static gint exclusiveCount(gint c);

            /**
             * A counter for per-thread read hold counts.
             * Maintained as a ThreadLocal; cached in cachedHoldCounter.
             */
            class HoldCounter final : public Object {
            public:
                int count = {}; // initially 0
                // Use id, not reference, to avoid garbage retention
                glong tid = LockSupport::getThreadID(Thread::currentThread());
            };

            /**
             * ThreadLocal subclass. Easiest to explicitly define for sake
             * of deserialization mechanics.
             */
            class TLHoldCounter final : public Object {
            public:
                HoldCounter initialValue() const;
            };

            /**
             * The number of reentrant read concurrent held by current thread.
             * Initialized only in constructor and readObject.
             * Removed whenever a thread's read hold count drops to 0.
             */
            TLHoldCounter readHolds;

            /**
             * The hold count of the last thread to successfully acquire
             * readLock. This saves ThreadLocal lookup in the common case
             * where the next thread to release is the last one to
             * acquire. This is non-volatile since it is just used
             * as a heuristic, and would be great for threads to cache.
             *
             * <p>Can outlive the Thread for which it is caching the read
             * hold count, but avoids garbage retention by not retaining a
             * reference to the Thread.
             *
             * <p>Accessed via a benign data race; relies on the memory
             * model's final field and out-of-thin-air guarantees.
             */
            HoldCounter cacheHoldCounter;

            /**
             * firstReader is the first thread to have acquired the read lock.
             * firstReaderHoldCount is firstReader's hold count.
             *
             * <p>More precisely, firstReader is the unique thread that last
             * changed the shared count from 0 to 1, and has not released the
             * read lock since then; null if there is no such thread.
             *
             * <p>Cannot cause garbage retention unless the thread terminated
             * without relinquishing its read concurrent, since tryReleaseShared
             * sets it to null.
             *
             * <p>Accessed via a benign data race; relies on the memory
             * model's out-of-thin-air guarantees for references.
             *
             * <p>This allows tracking of read holds for uncontended read
             * concurrent to be very cheap.
             */
            THREAD firstReader = {};
            gint firstReaderHoldCount = {};

        public:
            Synchronizer();

            /*
             * Acquires and releases use the same code for fair and
             * non-fair concurrent, but differ in whether/how they allow barging
             * when queues are non-empty.
             */
        };

        class ReentrantReadWriteLock::NonFairSync : public Synchronizer {
        };

        class ReentrantReadWriteLock::FairSync : public Synchronizer {
        };

        class ReentrantReadWriteLock::ReadLock : public Lock {
        };

        class ReentrantReadWriteLock::WriteLock : public Lock {
        };
    } // concurrent
} // core

#endif //CORE24_REENTRANTREADWRITELOCK_H
