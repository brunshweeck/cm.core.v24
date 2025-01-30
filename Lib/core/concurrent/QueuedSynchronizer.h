//
// Created by bruns on 30/09/2024.
//

#ifndef CORE24_QUEUEDSYNCHRONIZER_H
#define CORE24_QUEUEDSYNCHRONIZER_H

#include "Condition.h"
#include "OwnableSynchronizer.h"

namespace core {
    namespace concurrent {
        /**
         * Provides a framework for implementing blocking concurrent and related
         * synchronizers (semaphores, events, etc) that rely on
         * first-in-first-out (FIFO) wait queues.  This class is designed to
         * be a useful basis for most kinds of synchronizers that rely on a
         * single atomic @c int value to represent state. Subclasses
         * must define the protected methods that change this state, and which
         * define what that state means in terms of this object being acquired
         * or released.  Given these, the other methods in this class carry
         * out all queuing and blocking mechanics. Subclasses can maintain
         * other state fields, but only the atomically updated @c int
         * value manipulated using methods @b state,
         * @b setState and @b compareAndSetState is tracked with respect
         * to synchronization.
         *
         * <p>Subclasses should be defined as non-public internal helper
         * classes that are used to implement the synchronization properties
         * of their enclosing class.  Class
         * @c QueuedSynchronizer does not implement any
         * synchronization interface.  Instead it defines methods such as
         * @b acquireInterruptibly that can be invoked as
         * appropriate by concrete concurrent and related synchronizers to
         * implement their public methods.
         *
         * <p>This class supports either or both a default <em>exclusive</em>
         * mode and a <em>shared</em> mode. When acquired in exclusive mode,
         * attempted acquires by other threads cannot succeed. Shared mode
         * acquires by multiple threads may (but need not) succeed. This class
         * does not &quot;understand&quot; these differences except in the
         * mechanical sense that when a shared mode acquire succeeds, the next
         * waiting thread (if one exists) must also determine whether it can
         * acquire as well. Threads waiting in the different modes share the
         * same FIFO queue. Usually, implementation subclasses support only
         * one of these modes, but both can come into play for example in a
         * @b ReadWriteLock. Subclasses that support only exclusive or
         * only shared modes need not define the methods supporting the unused mode.
         *
         * <p>This class defines a nested @b ConditionObject class that
         * can be used as a @b Condition implementation by subclasses
         * supporting exclusive mode for which method @b isHeldExclusively
         * reports whether synchronization is exclusively
         * held with respect to the current thread, method @b release
         * invoked with the current @b state value fully releases
         * this object, and @b acquire, given this saved state value,
         * eventually restores this object to its previous acquired state.  No
         * @c QueuedSynchronizer method otherwise creates such a
         * condition, so if this constraint cannot be met, do not use it.  The
         * behavior of @b ConditionObject depends of course on the
         * semantics of its synchronizer implementation.
         *
         * <p>This class provides inspection, instrumentation, and monitoring
         * methods for the internal queue, as well as similar methods for
         * condition objects. These can be exported as desired into classes
         * using an @c QueuedSynchronizer for their
         * synchronization mechanics.
         *
         * <p>Serialization of this class stores only the underlying atomic
         * integer maintaining state, so deserialized objects have empty
         * thread queues. Typical subclasses requiring serializability will
         * define a @c readObject method that restores this to a known
         * initial state upon deserialization.
         *
         * <h2>Usage</h2>
         *
         * <p>To use this class as the basis of a synchronizer, redefine the
         * following methods, as applicable, by inspecting and/or modifying
         * the synchronization state using @b state,
         * @b setState and/or @b compareAndSetState:
         *
         *
         * @li @b tryAcquire
         * @li @b tryRelease
         * @li @b tryAcquireShared
         * @li @b tryReleaseShared
         * @li @b isHeldExclusively
         *
         *
         * Each of these methods by default throws
         * @b UnsupportedOperationException.  Implementations of these methods
         * must be internally thread-safe, and should in general be short and
         * not block. Defining these methods is the <em>only</em> supported
         * means of using this class. All other methods are not declared
         * @c virtual because they cannot be independently varied.
         *
         * <p>You may also find the inherited methods from
         * @b OwnableSynchronizer useful to keep track of the thread
         * owning an exclusive synchronizer.  You are encouraged to use them
         * -- this enables monitoring and diagnostic tools to assist users in
         * determining which threads hold concurrent.
         *
         * <p>Even though this class is based on an internal FIFO queue, it
         * does not automatically enforce FIFO acquisition policies.  The core
         * of exclusive synchronization takes the form:
         *
         * <pre>
         * <em>Acquire:</em>
         *     while (!tryAcquire(arg)) {
         *        <em>enqueue thread if it is not already queued</em>;
         *        <em>possibly block current thread</em>;
         *     }
         *
         * <em>Release:</em>
         *     if (tryRelease(arg))
         *        <em>unblock the first queued thread</em>;
         * </pre>
         *
         * (Shared mode is similar but may involve cascading signals.)
         *
         * <p>Because checks in acquire are invoked before
         * enqueuing, a newly acquiring thread may <em>barge</em> ahead of
         * others that are blocked and queued.  However, you can, if desired,
         * define @c tryAcquire and/or @c tryAcquireShared to
         * disable barging by internally invoking one or more of the inspection
         * methods, thereby providing a <em>fair</em> FIFO acquisition order.
         * In particular, most fair synchronizers can define @c tryAcquire
         * to return @c false if @b hasQueuedPredecessors (a method
         * specifically designed to be used by fair synchronizers) returns
         * @c true.  Other variations are possible.
         *
         * <p>Throughput and scalability are generally highest for the
         * default barging (also known as <em>greedy</em>,
         * <em>renouncement</em>, and <em>convoy-avoidance</em>) strategy.
         * While this is not guaranteed to be fair or starvation-free, earlier
         * queued threads are allowed to recontend before later queued
         * threads, and each recontention has an unbiased chance to succeed
         * against incoming threads.  Also, while acquires do not
         * &quot;spin&quot; in the usual sense, they may perform multiple
         * invocations of @c tryAcquire interspersed with other
         * computations before blocking.  This gives most of the benefits of
         * spins when exclusive synchronization is only briefly held, without
         * most of the liabilities when it isn't. If so desired, you can
         * augment this by preceding calls to acquire methods with
         * "fast-path" checks, possibly prechecking @b hasContended
         * and/or @b hasQueuedThreads to only do so if the synchronizer
         * is likely not to be contended.
         *
         * <p>This class provides an efficient and scalable basis for
         * synchronization in part by specializing its range of use to
         * synchronizers that can rely on @c int state, acquire, and
         * release parameters, and an internal FIFO wait queue. When this does
         * not suffice, you can build synchronizers from a lower level using
         * @b atomic classes, your own custom
         * @b util::Queue classes, and @b LockSupport blocking
         * support.
         *
         * <h2>Usage Examples</h2>
         *
         * <p>Here is a non-reentrant mutual exclusion lock class that uses
         * the value zero to represent the unlocked state, and one to
         * represent the locked state. While a non-reentrant lock
         * does not strictly require recording of the current owner
         * thread, this class does so anyway to make usage easier to monitor.
         * It also supports conditions and exposes some instrumentation methods:
         *
         * @code
         * class Mutex: public Lock {
         *
         *   // Our internal helper class
         *   class Sync: public QueuedSynchronizer {
         *     public:
         *     // Acquires the lock if state is zero
         *     gbool tryAcquire(int acquires) {
         *       CORE_ASSERT(acquires == 1); // Otherwise unused
         *       if (compareAndSetState(0, 1)) {
         *         setExclusiveOwnerThread(Thread::currentThread());
         *         return true;
         *       }
         *       return false;
         *     }
         *
         *     protected :
         *     // Releases the lock by setting state to zero
         *     gbool tryRelease(int releases) {
         *       CORE_ASSERT(releases == 1); // Otherwise unused
         *       if (!isHeldExclusively())
         *         IllegalStateException().throws($ftrace());
         *       setExclusiveOwnerThread(null);
         *       setState(0);
         *       return true;
         *     }
         *
         *     public :
         *     // Reports whether in locked state
         *     gbool isLocked() {
         *       return state() != 0;
         *     }
         *
         *     gbool isHeldExclusively() {
         *       // a data race, but safe due to out-of-thin-air guarantees
         *       return getExclusiveOwnerThread() == Thread::currentThread();
         *     }
         *
         *     // Provides a Condition
         *     Condition newCondition() {
         *       return new ConditionObject();
         *     }
         *
         *     private:
         *     // Deserializes properly
         *     void readObject(ObjectInputStream &s) {
         *       s.defaultReadObject();
         *       setState(0); // reset to unlocked state
         *     }
         *   }
         *
         *   // The sync object does all the hard work. We just forward to it.
         *   Sync &sync = *new Sync();
         *
         *   public :
         *   void lock()              { sync.acquire(1); }
         *   gbool tryLock()        { return sync.tryAcquire(1); }
         *   void unlock()            { sync.release(1); }
         *   Condition newCondition() { return sync.newCondition(); }
         *   gbool isLocked()       { return sync.isLocked(); }
         *   gbool isHeldByCurrentThread() { return sync.isHeldExclusively(); }
         *   gbool hasQueuedThreads() { return sync.hasQueuedThreads(); }
         *   void lockInterruptibly() { sync.acquireInterruptibly(1); }
         *   gbool tryLock(glong timeout, TimeUnit unit) {
         *     return sync.tryAcquireNanos(1, unit.toNanos(timeout));
         *   }
         * }
         * @endcode
         *
         * <p>Here is a latch class that is like a
         * @b CountDownLatch
         * except that it only requires a single @c signal to
         * fire. Because a latch is non-exclusive, it uses the @c shared
         * acquire and release methods.
         *
         * @code
         * class BooleanLatch {
         *
         *   class Sync: public QueuedSynchronizer {
         *     gbool isSignalled() { return state() != 0; }
         *
         *     protected:
         *     int tryAcquireShared(int ignore) {
         *       return isSignalled() ? 1 : -1;
         *     }
         *
         *     gbool tryReleaseShared(int ignore) {
         *       setState(1);
         *       return true;
         *     }
         *   };
         *
         *   Sync &sync = *new Sync();
         *
         *   public:
         *   gbool isSignalled() { return sync.isSignalled(); }
         *   void signal()         { sync.releaseShared(1); }
         *   void await() {
         *     sync.acquireSharedInterruptibly(1);
         *   }
         * };
         * @endcode
         */
        class QueuedSynchronizer : public virtual OwnableSynchronizer {
            CORE_ALIAS(THREAD, Class<Thread>::Pointer);
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(Chronometer, time::Chrono);

            /*
               Overview.

               The wait queue is a variant of a "CLH" (Craig, Landin, and
               Hagersten) lock queue. CLH concurrent are normally used for
               spinlocks.  We instead use them for blocking synchronizers by
               including explicit ("prev" and "next") links plus a "status"
               field that allow nodes to signal successors when releasing
               concurrent, and handle cancellation due to interrupts and timeouts.
               The status field includes bits that track whether a thread
               needs a signal (using LockSupport.unpark). Despite these
               additions, we maintain most CLH locality properties.

               To enqueue into a CLH lock, you atomically splice it in as new
               tail. To dequeue, you set the head field, so the next eligible
               waiter becomes first.

                +------+  prev +-------+       +------+
                | head | <---- | first | <---- | tail |
                +------+       +-------+       +------+

               Insertion into a CLH queue requires only a single atomic
               operation on "tail", so there is a simple point of demarcation
               from unqueued to queued. The "next" link of the predecessor is
               set by the enqueuing thread after successful CAS. Even though
               non-atomic, this suffices to ensure that any blocked thread is
               signalled by a predecessor when eligible (although in the case
               of cancellation, possibly with the assistance of a signal in
               method cleanQueue). Signalling is based in part on a
               Dekker-like scheme in which the to-be waiting thread indicates
               WAITING status, then retries acquiring, and then rechecks
               status before blocking. The signaller atomically clears WAITING
               status when unparking.

               Dequeuing on acquire involves detaching (nulling) a node's
               "prev" node and then updating the "head". Other threads check
               if a node is or was dequeued by checking "prev" rather than
               head. We enforce the nulling then setting order by spin-waiting
               if necessary. Because of this, the lock algorithm is not itself
               strictly "lock-free" because an acquiring thread may need to
               wait for a previous acquire to make progress. When used with
               exclusive concurrent, such progress is required anyway. However
               Shared mode may (uncommonly) require a spin-wait before
               setting head field to ensure proper propagation. (Historical
               note: This allows some simplifications and efficiencies
               compared to previous versions of this class.)

               A node's predecessor can change due to cancellation while it is
               waiting, until the node is first in queue, at which point it
               cannot change. The acquire methods cope with this by rechecking
               "prev" before waiting. The prev and next fields are modified
               only via CAS by cancelled nodes in method cleanQueue. The
               unsplice strategy is reminiscent of Michael-Scott queues in
               that after a successful CAS to prev field, other threads help
               fix next fields.  Because cancellation often occurs in bunches
               that complicate decisions about necessary signals, each call to
               cleanQueue traverses the queue until a clean sweep. Nodes that
               become relinked as first are unconditionally unparked
               (sometimes unnecessarily, but those cases are not worth
               avoiding).

               A thread may try to acquire if it is first (frontmost) in the
               queue, and sometimes before.  Being first does not guarantee
               success; it only gives the right to contend. We balance
               throughput, overhead, and fairness by allowing incoming threads
               to "barge" and acquire the synchronizer while in the process of
               enqueuing, in which case an awakened first thread may need to
               rewait.  To counteract possible repeated unlucky rewaits, we
               exponentially increase retries (up to 256) to acquire each time
               a thread is unparked. Except in this case, AQS concurrent do not
               spin; they instead interleave attempts to acquire with
               bookkeeping steps. (Users who want spinlocks can use
               tryAcquire.)

               To improve garbage collectibility, fields of nodes not yet on
               list are null. (It is not rare to create and then throw away a
               node without using it.) Fields of nodes coming off the list are
               nulled out as soon as possible. This accentuates the challenge
               of externally determining the first waiting thread (as in
               method getFirstQueuedThread). This sometimes requires the
               fallback of traversing backwards from the atomically updated
               "tail" when fields appear null. (This is never needed in the
               process of signalling though.)

               CLH queues need a dummy header node to get started. But
               we don't create them on construction, because it would be wasted
               effort if there is never contention. Instead, the node
               is constructed and head and tail pointers are set upon first
               contention.

               Shared mode operations differ from Exclusive in that an acquire
               signals the next waiter to try to acquire if it is also
               Shared. The tryAcquireShared API allows users to indicate the
               degree of propagation, but in most applications, it is more
               efficient to ignore this, allowing the successor to try
               acquiring in any case.

               Threads waiting on Conditions use nodes with an additional
               link to maintain the (FIFO) list of conditions. Conditions only
               need to link nodes in simple (non-concurrent) linked queues
               because they are only accessed when exclusively held.  Upon
               await, a node is inserted into a condition queue.  Upon signal,
               the node is enqueued on the main queue.  A special status field
               value is used to track and atomically trigger this.

               Accesses to fields head, tail, and state use full Volatile
               mode, along with CAS. Node fields status, prev and next also do
               so while threads may be signallable, but sometimes use weaker
               modes otherwise. Accesses to field "waiter" (the thread to be
               signalled) are always sandwiched between other atomic accesses
               so are used in Plain mode.

               Most of the above is performed by primary internal method
               acquire, that is invoked in some way by all exported acquire
               methods.  (It is usually easy for compilers to optimize
               call-site specializations when heavily used.)

               Most AQS methods may be called by components that cannot be
               allowed to fail when encountering OutOfMemoryErrors. The main
               acquire method resorts to spin-waits with backoff if nodes
               cannot be allocated. Condition waits release and reacquire
               concurrent upon OOME at a slow fixed rate (OOME_COND_WAIT_DELAY)
               designed with the hope that eventually enough memory will be
               recovered; if not performance can be very slow. Effectiveness
               is also limited by the possibility of class loading triggered
               by first-time usages, that may encounter unrecoverable
               OOMEs. Also, it is possible for OutOfMemoryErrors to be thrown
               when attempting to create and throw
               IllegalStateExceptions and InterruptedExceptions.

               There are several arbitrary decisions about when and how to
               check interrupts in both acquire and await before and/or after
               blocking. The decisions are less arbitrary in implementation
               updates because some users appear to rely on original behaviors
               in ways that are racy and so (rarely) wrong in general but hard
               to justify changing.

               Thanks go to Dave Dice, Mark Moir, Victor Luchangco, Bill
               Scherer and Michael Scott, along with members of JSR-166
               expert group, for helpful ideas, discussions, and critiques
               on the design of this class.
             */

            // Node status bits, also used as argument and return values

            static CORE_FAST gint WAITING = 1; // must be 1
            static CORE_FAST gint CANCELLED = 0x80000000; // must be negative
            static CORE_FAST gint COND = 2; // in a condition wait

            /** CLH Nodes */
            class Node;
            class ExclusiveNode;
            class SharedNode;
            class ConditionNode;

            CORE_ALIAS(NODE, Class<Node>::Pointer);
            CORE_ALIAS(EXCL_NODE, Class<ExclusiveNode>::Pointer);
            CORE_ALIAS(SHARED_NODE, Class<SharedNode>::Pointer);
            CORE_ALIAS(COND_NODE, Class<ConditionNode>::Pointer);

            CORE_ADD_AS_FRIEND(Node);
            CORE_ADD_AS_FRIEND(ExclusiveNode);
            CORE_ADD_AS_FRIEND(SharedNode);
            CORE_ADD_AS_FRIEND(ConditionNode);

            /**
             * Head of the wait queue, lazily initialized.
             */
            NODE volatile head = null;

            /**
             * Tail of the wait queue. After initialization, modified only via casTail.
             */
            NODE volatile tail = null;

            /**
             * The synchronization state.
             */
            gint volatile state_ = 0;

        protected:
            /**
             * Creates a new @c QueuedSynchronizer instance
             * with initial synchronization state of zero.
             */
            CORE_IMPLICIT QueuedSynchronizer() = default;

            /**
             * Returns the current value of synchronization state.
             * This operation has memory semantics of a @c volatile read.
             * @return current state value
             */
            gint state() const;

            /**
             * Sets the value of synchronization state.
             * This operation has memory semantics of a @c volatile write.
             * @param newState the new state value
             */
            void setState(gint newState);

            /**
             * Atomically sets synchronization state to the given updated
             * value if the current state value equals the expected value.
             * This operation has memory semantics of a @c volatile read
             * and write.
             *
             * @param expect the expected value
             * @param update the new value
             * @return @c true if successful. False return indicates that the actual
             *         value was not equal to the expected value.
             */
            gbool compareAndSetState(gint expected, gint update);

            // Main exported methods

            /**
             * Attempts to acquire in exclusive mode. This method should query
             * if the state of the object permits it to be acquired in the
             * exclusive mode, and if so to acquire it.
             *
             * <p>This method is always invoked by the thread performing
             * acquire.  If this method reports failure, the acquire method
             * may queue the thread, if it is not already queued, until it is
             * signalled by a release from some other thread. This can be used
             * to implement method @b Lock::tryLock().
             *
             * <p>The default
             * implementation throws @b UnsupportedOperationException.
             *
             * @param arg the acquire argument. This value is always the one
             *        passed to an acquire method, or is the value saved on entry
             *        to a condition wait.  The value is otherwise uninterpreted
             *        and can represent anything you like.
             * @return @c true if successful. Upon success, this object has
             *         been acquired.
             * @throws IllegalStateException if acquiring would place this
             *         synchronizer in an illegal state. This exception must be
             *         thrown in a consistent fashion for synchronization to work
             *         correctly.
             * @throws UnsupportedOperationException if exclusive mode is not supported
             */
            virtual gbool tryAcquire(gint arg);

            /**
             * Attempts to set the state to reflect a release in exclusive
             * mode.
             *
             * <p>This method is always invoked by the thread performing release.
             *
             * <p>The default implementation throws
             * @b UnsupportedOperationException.
             *
             * @param arg the release argument. This value is always the one
             *        passed to a release method, or the current state value upon
             *        entry to a condition wait.  The value is otherwise
             *        uninterpreted and can represent anything you like.
             * @return @c true if this object is now in a fully released
             *         state, so that any waiting threads may attempt to acquire;
             *         and @c false otherwise.
             * @throws IllegalStateException if releasing would place this
             *         synchronizer in an illegal state. This exception must be
             *         thrown in a consistent fashion for synchronization to work
             *         correctly.
             * @throws UnsupportedOperationException if exclusive mode is not supported
             */
            virtual gbool tryRelease(gint arg);

            /**
             * Attempts to acquire in shared mode. This method should query if
             * the state of the object permits it to be acquired in the shared
             * mode, and if so to acquire it.
             *
             * <p>This method is always invoked by the thread performing
             * acquire.  If this method reports failure, the acquire method
             * may queue the thread, if it is not already queued, until it is
             * signalled by a release from some other thread.
             *
             * <p>The default implementation throws
             * @b UnsupportedOperationException.
             *
             * @param arg the acquire argument. This value is always the one
             *        passed to an acquire method, or is the value saved on entry
             *        to a condition wait.  The value is otherwise uninterpreted
             *        and can represent anything you like.
             * @return a negative value on failure; zero if acquisition in shared
             *         mode succeeded but no subsequent shared-mode acquire can
             *         succeed; and a positive value if acquisition in shared
             *         mode succeeded and subsequent shared-mode acquires might
             *         also succeed, in which case a subsequent waiting thread
             *         must check availability. (Support for three different
             *         return values enables this method to be used in contexts
             *         where acquires only sometimes act exclusively.)  Upon
             *         success, this object has been acquired.
             * @throws IllegalStateException if acquiring would place this
             *         synchronizer in an illegal state. This exception must be
             *         thrown in a consistent fashion for synchronization to work
             *         correctly.
             * @throws UnsupportedOperationException if shared mode is not supported
             */
            virtual gint tryAcquireShared(gint arg);

            /**
             * Attempts to set the state to reflect a release in shared mode.
             *
             * <p>This method is always invoked by the thread performing release.
             *
             * <p>The default implementation throws
             * @b UnsupportedOperationException.
             *
             * @param arg the release argument. This value is always the one
             *        passed to a release method, or the current state value upon
             *        entry to a condition wait.  The value is otherwise
             *        uninterpreted and can represent anything you like.
             * @return @c true if this release of shared mode may permit a
             *         waiting acquire (shared or exclusive) to succeed; and
             *         @c false otherwise
             * @throws IllegalStateException if releasing would place this
             *         synchronizer in an illegal state. This exception must be
             *         thrown in a consistent fashion for synchronization to work
             *         correctly.
             * @throws UnsupportedOperationException if shared mode is not supported
             */
            virtual gbool tryReleaseShared(gint arg);

            /**
             * Returns @c true if synchronization is held exclusively with
             * respect to the current (calling) thread.  This method is invoked
             * upon each call to a @b ConditionObject method.
             *
             * <p>The default implementation throws
             * @b UnsupportedOperationException. This method is invoked
             * internally only within @b ConditionObject methods, so need
             * not be defined if conditions are not used.
             *
             * @return @c true if synchronization is held exclusively;
             *         @c false otherwise
             * @throws UnsupportedOperationException if conditions are not supported
             */
            virtual gbool isHeldExclusively() const;

        public:
            /**
             * Acquires in exclusive mode, ignoring interrupts.  Implemented
             * by invoking at least once @b tryAcquire,
             * returning on success.  Otherwise the thread is queued, possibly
             * repeatedly blocking and unblocking, invoking
             * @b tryAcquire until success.  This method can be used
             * to implement method @b Lock::lock.
             *
             * @param arg the acquire argument.  This value is conveyed to
             *        @b tryAcquire but is otherwise uninterpreted and
             *        can represent anything you like.
             */
            virtual void acquire(gint arg);

            /**
             * Acquires in exclusive mode, aborting if interrupted.
             * Implemented by first checking interrupt status, then invoking
             * at least once @b tryAcquire, returning on
             * success.  Otherwise the thread is queued, possibly repeatedly
             * blocking and unblocking, invoking @b tryAcquire
             * until success or the thread is interrupted.  This method can be
             * used to implement method @b Lock::lockInterruptibly.
             *
             * @param arg the acquire argument.  This value is conveyed to
             *        @b tryAcquire but is otherwise uninterpreted and
             *        can represent anything you like.
             * @throws InterruptedException if the current thread is interrupted
             */
            virtual void acquireInterruptibly(gint arg);

            /**
             * Attempts to acquire in exclusive mode, aborting if interrupted,
             * and failing if the given timeout elapses.  Implemented by first
             * checking interrupt status, then invoking at least once
             * @b tryAcquire, returning on success.  Otherwise, the thread is
             * queued, possibly repeatedly blocking and unblocking, invoking
             * @b tryAcquire until success or the thread is interrupted
             * or the timeout elapses.  This method can be used to implement
             * method <b> Lock::tryLock(glong, TimeUnit) </b>.
             *
             * @param arg the acquire argument.  This value is conveyed to
             *        @b tryAcquire but is otherwise uninterpreted and
             *        can represent anything you like.
             * @param nanosTimeout the maximum number of nanoseconds to wait
             * @return @c true if acquired; @c false if timed out
             * @throws InterruptedException if the current thread is interrupted
             */
            virtual gbool tryAcquireNanos(gint arg, glong nanosTimeout);

            /**
             * Releases in exclusive mode.  Implemented by unblocking one or
             * more threads if @b tryRelease returns true.
             * This method can be used to implement method @b Lock::unlock.
             *
             * @param arg the release argument.  This value is conveyed to
             *        @b tryRelease but is otherwise uninterpreted and
             *        can represent anything you like.
             * @return the value returned from @b tryRelease
             */
            virtual gbool release(gint arg);

            /**
             * Acquires in shared mode, ignoring interrupts.  Implemented by
             * first invoking at least once @b tryAcquireShared,
             * returning on success.  Otherwise, the thread is queued, possibly
             * repeatedly blocking and unblocking, invoking
             * @b tryAcquireShared until success.
             *
             * @param arg the acquire argument.  This value is conveyed to
             *        @b tryAcquireShared but is otherwise uninterpreted
             *        and can represent anything you like.
             */
            virtual void acquireShared(gint arg);

            /**
             * Acquires in shared mode, aborting if interrupted.  Implemented
             * by first checking interrupt status, then invoking at least once
             * @b tryAcquireShared, returning on success.  Otherwise the
             * thread is queued, possibly repeatedly blocking and unblocking,
             * invoking @b tryAcquireShared until success or the thread
             * is interrupted.
             * @param arg the acquire argument.
             * This value is conveyed to @b tryAcquireShared but is
             * otherwise uninterpreted and can represent anything
             * you like.
             * @throws InterruptedException if the current thread is interrupted
             */
            virtual void acquireSharedInterruptibly(gint arg);

            /**
             * Attempts to acquire in shared mode, aborting if interrupted, and
             * failing if the given timeout elapses.  Implemented by first
             * checking interrupt status, then invoking at least once
             * @b tryAcquireShared, returning on success.  Otherwise, the
             * thread is queued, possibly repeatedly blocking and unblocking,
             * invoking @b tryAcquireShared until success or the thread
             * is interrupted or the timeout elapses.
             *
             * @param arg the acquire argument.  This value is conveyed to
             *        @b tryAcquireShared but is otherwise uninterpreted
             *        and can represent anything you like.
             * @param nanosTimeout the maximum number of nanoseconds to wait
             * @return @c true if acquired; @c false if timed out
             * @throws InterruptedException if the current thread is interrupted
             */
            virtual gbool tryAcquireSharedNanos(gint arg, glong nanosTimeout);

            /**
             * Releases in shared mode.  Implemented by unblocking one or more
             * threads if @b tryReleaseShared returns true.
             *
             * @param arg the release argument.  This value is conveyed to
             *        @b tryReleaseShared but is otherwise uninterpreted
             *        and can represent anything you like.
             * @return the value returned from @b tryReleaseShared
             */
            virtual gbool releaseShared(gint arg);

            // Queue inspection methods

            /**
             * Queries whether any threads are waiting to acquire. Note that
             * because cancellations due to interrupts and timeouts may occur
             * at any time, a @c true return does not guarantee that any
             * other thread will ever acquire.
             *
             * @return @c true if there may be other threads waiting to acquire
             */
            virtual gbool hasQueuedThreads() const;

            /**
             * Queries whether any threads have ever contended to acquire this
             * synchronizer; that is, if an acquire method has ever blocked.
             *
             * <p>In this implementation, this operation returns in
             * constant time.
             *
             * @return @c true if there has ever been contention
             */
            virtual gbool hasContended() const;

            /**
             * Returns the first (longest-waiting) thread in the queue, or
             * @c null if no threads are currently queued.
             *
             * <p>In this implementation, this operation normally returns in
             * constant time, but may iterate upon contention if other threads are
             * concurrently modifying the queue.
             *
             * @return the first (longest-waiting) thread in the queue, or
             *         @c null if no threads are currently queued
             */
            virtual Object &firstQueuedThread();

            /**
             * Returns true if the given thread is currently queued.
             *
             * <p>This implementation traverses the queue to determine
             * presence of the given thread.
             *
             * @param thread the thread
             * @return @c true if the given thread is on the queue
             */
            virtual gbool isQueued(Thread const &thread);

            /**
             * Returns @c true if the apparent first queued thread, if one
             * exists, is waiting in exclusive mode.  If this method returns
             * @c true, and the current thread is attempting to acquire in
             * shared mode (that is, this method is invoked from
             * @b tryAcquireShared) then it is guaranteed that the current thread
             * is not the first queued thread.  Used only as a heuristic in
             * ReentrantReadWriteLock.
             */
            virtual gbool apparentlyFirstQueuedIsExclusive();

            /**
             * Queries whether any threads have been waiting to acquire longer
             * than the current thread.
             *
             * <p>An invocation of this method is equivalent to (but may be
             * more efficient than):
             * @code
             * getFirstQueuedThread() != Thread.currentThread()
             *   && hasQueuedThreads()}</pre>
             *
             * <p>Note that because cancellations due to interrupts and
             * timeouts may occur at any time, a @c true return does not
             * guarantee that some other thread will acquire before the current
             * thread.  Likewise, it is possible for another thread to win a
             * race to enqueue after this method has returned @c false,
             * due to the queue being empty.
             *
             * <p>This method is designed to be used by a fair synchronizer to
             * avoid <a href="QueuedSynchronizer.html#barging">barging</a>.
             * Such a synchronizer's @b tryAcquire method should return
             * @c false, and its @b tryAcquireShared method should
             * return a negative value, if this method returns @c true
             * (unless this is a reentrant acquire).  For example, the
             * @b tryAcquire method for a fair, reentrant, exclusive mode
             * synchronizer might look like this:
             *
             * @code
             * protected gbool tryAcquire(int arg) {
             *   if (isHeldExclusively()) {
             *     // A reentrant acquire; increment hold count
             *     return true;
             *   } else if (hasQueuedPredecessors()) {
             *     return false;
             *   } else {
             *     // try to acquire normally
             *   }
             * }}</pre>
             *
             * @return @c true if there is a queued thread preceding the
             *         current thread, and @c false if the current thread
             *         is at the head of the queue or the queue is empty
             */
            virtual gbool hasQueuedPredecessors();

            // Instrumentation and monitoring methods

            /**
             * Returns an estimate of the number of threads waiting to
             * acquire.  The value is only an estimate because the number of
             * threads may change dynamically while this method traverses
             * internal data structures.  This method is designed for use in
             * monitoring system state, not for synchronization control.
             *
             * @return the estimated number of threads waiting to acquire
             */
            virtual gint queueLength();

            /**
             * Returns a collection containing threads that may be waiting to
             * acquire.  Because the actual set of threads may change
             * dynamically while constructing this result, the returned
             * collection is only a best-effort estimate.  The elements of the
             * returned collection are in no particular order.  This method is
             * designed to facilitate construction of subclasses that provide
             * more extensive monitoring facilities.
             *
             * @return the collection of threads
             */
            virtual util::ArrayList<Thread> queuedThreads() const;

            /**
             * Returns a collection containing threads that may be waiting to
             * acquire in exclusive mode. This has the same properties
             * as @b queuedThreads except that it only returns
             * those threads waiting due to an exclusive acquire.
             *
             * @return the collection of threads
             */
            virtual util::ArrayList<Thread> exclusiveQueuedThreads() const;

            /**
             * Returns a collection containing threads that may be waiting to
             * acquire in shared mode. This has the same properties
             * as @b queuedThreads except that it only returns
             * those threads waiting due to a shared acquire.
             *
             * @return the collection of threads
             */
            virtual util::ArrayList<Thread> sharedQueuedThreads() const;

            /**
             * Returns a string identifying this synchronizer, as well as its state.
             * The state, in brackets, includes the String @c "State ="
             * followed by the current value of @b state, and either
             * @c "nonempty" or @c "empty" depending on whether the
             * queue is empty.
             *
             * @return a string identifying this synchronizer, as well as its state
             */
            String toString() const override;

            // Instrumentation methods for conditions

            /**
             * Condition implementation for a @b QueuedSynchronizer
             * serving as the basis of a @b Lock implementation.
             *
             * <p>Method documentation for this class describes mechanics,
             * not behavioral specifications from the point of view of Lock
             * and Condition users. Exported versions of this class will in
             * general need to be accompanied by documentation describing
             * condition semantics that rely on those of the associated
             * @c QueuedSynchronizer.
             *
             * <p>This class is Serializable, but all fields are transient,
             * so deserialized conditions have no waiters.
             */
            class ConditionObject : public virtual Condition {
                QueuedSynchronizer &owner;

                /** First node of condition queue. */
                COND_NODE mutable firstWaiter;
                /** Last node of condition queue. */
                COND_NODE mutable lastWaiter;

                /**
                 * Fixed delay in nanoseconds between releasing and reacquiring
                 * lock during Condition waits that encounter OutOfMemoryErrors
                 */
                static CORE_FAST glong OOME_COND_WAIT_DELAY = 10L * 1000L * 1000L; // 10 ms

                CORE_ADD_AS_FRIEND(QueuedSynchronizer);

            public:
                /**
                 * Creates a new @c ConditionObject instance.
                 */
                CORE_EXPLICIT ConditionObject(QueuedSynchronizer &owner): owner(owner) {
                };

                // Signalling methods

                /**
                 * Moves the longest-waiting thread, if one exists, from the
                 * wait queue for this condition to the wait queue for the
                 * owning lock.
                 *
                 * @throws IllegalStateException if @b isHeldExclusively
                 *         returns @c false
                 */
                void notify() override;

                /**
                 * Moves all threads from the wait queue for this condition to
                 * the wait queue for the owning lock.
                 *
                 * @throws IllegalStateException if @b isHeldExclusively
                 *         returns @c false
                 */
                void notifyAll() override;

                // Waiting methods

                /**
                 * Implements uninterruptible condition wait.
                 * <ol>
                 * @li Save lock state returned by @b state.
                 * @li Invoke @b release with saved state as argument,
                 *     throwing IllegalStateException if it fails.
                 * @li Block until signalled.
                 * @li Reacquire by invoking specialized version of
                 *     @b acquire with saved state as argument.
                 * </ol>
                 */
                void awaitUninterruptibly() override;

                /**
                 * Implements interruptible condition wait.
                 * <ol>
                 * @li If current thread is interrupted, throw InterruptedException.
                 * @li Save lock state returned by @b state.
                 * @li Invoke @b release with saved state as argument,
                 *     throwing IllegalStateException if it fails.
                 * @li Block until signalled or interrupted.
                 * @li Reacquire by invoking specialized version of
                 *     @b acquire with saved state as argument.
                 * @li If interrupted while blocked in step 4, throw InterruptedException.
                 * </ol>
                 */
                void await() override;

                /**
                 * Implements timed condition wait.
                 * <ol>
                 * @li If current thread is interrupted, throw InterruptedException.
                 * @li Save lock state returned by @b state.
                 * @li Invoke @b release with saved state as argument,
                 *     throwing IllegalStateException if it fails.
                 * @li Block until signalled, interrupted, or timed out.
                 * @li Reacquire by invoking specialized version of
                 *     @b acquire with saved state as argument.
                 * @li If interrupted while blocked in step 4, throw InterruptedException.
                 * </ol>
                 */
                glong awaitNanos(glong nanosTimeout) override;

                /**
                 * Implements absolute timed condition wait.
                 * <ol>
                 * @li If current thread is interrupted, throw InterruptedException.
                 * @li Save lock state returned by @b state.
                 * @li Invoke @b release with saved state as argument,
                 *     throwing IllegalStateException if it fails.
                 * @li Block until signalled, interrupted, or timed out.
                 * @li Reacquire by invoking specialized version of
                 *     @b acquire with saved state as argument.
                 * @li If interrupted while blocked in step 4, throw InterruptedException.
                 * @li If timed out while blocked in step 4, return false, else true.
                 * </ol>
                 */
                gbool awaitUntil(const time::LocalDateTime &deadline) override;

                /**
                 * Implements timed condition wait.
                 * <ol>
                 * @li If current thread is interrupted, throw InterruptedException.
                 * @li Save lock state returned by @b state.
                 * @li Invoke @b release with saved state as argument,
                 *     throwing IllegalStateException if it fails.
                 * @li Block until signalled, interrupted, or timed out.
                 * @li Reacquire by invoking specialized version of
                 *     @b acquire with saved state as argument.
                 * @li If interrupted while blocked in step 4, throw InterruptedException.
                 * @li If timed out while blocked in step 4, return false, else true.
                 * </ol>
                 */
                gbool await(glong time, time::Temporal::ChronoUnit unit) override;

            private:
                /**
                 * Removes and transfers one or all waiters to sync queue.
                 */
                void doSignal(COND_NODE first, gbool all);

                /**
                 * Adds node to condition list and releases lock.
                 *
                 * @param node the node
                 * @return savedState to reacquire after wait
                 */
                gint enableWait(COND_NODE node);

                /**
                 * Returns true if a node that was initially placed on a condition
                 * queue is now ready to reacquire on sync queue.
                 * @param node the node
                 * @return true if is reacquiring
                 */
                gbool canReacquire(COND_NODE node) const;

                /**
                 * Unlinks the given node and other non-waiting nodes from
                 * condition queue unless already unlinked.
                 */
                void unlinkCancelledWaiters(COND_NODE node);

                /**
                 * Constructs objects needed for condition wait. On OOME,
                 * releases lock, sleeps, reacquires, and returns null.
                 */
                COND_NODE newConditionNode();

                //  support for instrumentation

                /**
                 * Returns true if this condition was created by the given
                 * synchronization object.
                 *
                 * @return @c true if owned
                 */
                gbool isOwnedBy(QueuedSynchronizer const &sync) const;

            protected:
                /**
                 * Queries whether any threads are waiting on this condition.
                 * Implements <b> QueuedSynchronizer::hasWaiters(ConditionObject) </b>.
                 *
                 * @return @c true if there are any waiting threads
                 * @throws IllegalStateException if @b isHeldExclusively
                 *         returns @c false
                 */
                gbool hasWaiters() const;

                /**
                 * Returns an estimate of the number of threads waiting on
                 * this condition.
                 * Implements <b> QueuedSynchronizer::waitQueueLength(ConditionObject)</b>.
                 *
                 * @return the estimated number of waiting threads
                 * @throws IllegalStateException if @b isHeldExclusively
                 *         returns @c false
                 */
                gint waitQueueLength() const;

                /**
                 * Returns a collection containing those threads that may be
                 * waiting on this Condition.
                 * Implements <b> QueuedSynchronizer::waitingThreads(ConditionObject)</b>.
                 *
                 * @return the collection of threads
                 * @throws IllegalStateException if @b isHeldExclusively
                 *         returns @c false
                 */
                util::ArrayList<Thread> waitingThreads() const;
            };

            /**
             * Queries whether the given ConditionObject
             * uses this synchronizer as its lock.
             *
             * @param condition the condition
             * @return @c true if owned
             * @throws NullPointerException if the condition is null
             */
            virtual gbool owns(ConditionObject const &condition) const;

            /**
             * Queries whether any threads are waiting on the given condition
             * associated with this synchronizer. Note that because timeouts
             * and interrupts may occur at any time, a @c true return
             * does not guarantee that a future @c signal will awaken
             * any threads.  This method is designed primarily for use in
             * monitoring of the system state.
             *
             * @param condition the condition
             * @return @c true if there are any waiting threads
             * @throws IllegalStateException if exclusive synchronization
             *         is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this synchronizer
             * @throws NullPointerException if the condition is null
             */
            virtual gbool hasWaiters(ConditionObject const &condition) const;

            /**
             * Returns an estimate of the number of threads waiting on the
             * given condition associated with this synchronizer. Note that
             * because timeouts and interrupts may occur at any time, the
             * estimate serves only as an upper bound on the actual number of
             * waiters.  This method is designed for use in monitoring system
             * state, not for synchronization control.
             *
             * @param condition the condition
             * @return the estimated number of waiting threads
             * @throws IllegalStateException if exclusive synchronization
             *         is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this synchronizer
             * @throws NullPointerException if the condition is null
             */
            virtual gint waiterQueueLength(ConditionObject const &condition) const;

            /**
             * Returns a collection containing those threads that may be
             * waiting on the given condition associated with this
             * synchronizer.  Because the actual set of threads may change
             * dynamically while constructing this result, the returned
             * collection is only a best-effort estimate. The elements of the
             * returned collection are in no particular order.
             *
             * @param condition the condition
             * @return the collection of threads
             * @throws IllegalStateException if exclusive synchronization
             *         is not held
             * @throws IllegalArgumentException if the given condition is
             *         not associated with this synchronizer
             * @throws NullPointerException if the condition is null
             */
            virtual util::ArrayList<Thread> waitingThreads(ConditionObject const &condition) const;

        private:
            /** CLH Nodes */
            class Node : public virtual Object {
                NODE prev = null; // initially attached via casTail
                NODE next = null; // visibly nonnull when signallable
                THREAD waiter = null; // visibly nonnull when enqueued
                gint volatile status = 0; // written by owner, atomic bit ops by others

                CORE_ADD_AS_FRIEND(QueuedSynchronizer);
                CORE_ADD_AS_FRIEND(ExclusiveNode);
                CORE_ADD_AS_FRIEND(SharedNode);
                CORE_ADD_AS_FRIEND(Condition);
                CORE_ADD_AS_FRIEND(ConditionObject);

                // methods for atomic operations
                gbool compareAndSetPrev(NODE c, NODE v);

                gbool compareAndSetNext(NODE c, NODE v);

                gint getAndUnsetStatus(gint v);

                void setPrevRelaxed(NODE p);

                void setStatusRelaxed(gint s);

                void clearStatus();
            };

            // Concrete classes tagged by type
            class ExclusiveNode final : public virtual Node {
            };

            class SharedNode final : public virtual Node {
            };

            class ConditionNode final : public virtual Node {
                COND_NODE nextWaiter = null; // link to next waiting node

                CORE_ADD_AS_FRIEND(QueuedSynchronizer);
                CORE_ADD_AS_FRIEND(ConditionObject);

            public:
                /**
                 * Allows Conditions to be used in ForkJoinPools without
                 * risking fixed pool exhaustion. This is usable only for
                 * untimed Condition waits, not timed versions.
                 */
                gbool isReleasable();

                gbool block();
            };

            // Queuing utilities

            gbool compareAndSetTail(NODE c, NODE v);

            /**
             * Tries to CAS a new dummy node for head.
             * Returns new tail, or null if OutOfMemory
             */
            NODE tryInitializeHead();

            /**
             * Enqueues the node unless null. (Currently used only for
             * ConditionNodes; other cases are interleaved with acquires.)
             */
            void enqueue(COND_NODE node);

            /** Returns true if node is found in traversal from tail */
            gbool isEnqueued(NODE node) const;

            /**
             * Wakes up the successor of given node, if one exists, and unsets its
             * WAITING status to avoid park race. This may fail to wake up an
             * eligible thread when one or more have been cancelled, but
             * cancelAcquire ensures liveness.
             */
            static void signalNext(NODE h);

            /** Wakes up the given node if in shared mode */
            static void signalNextIfShared(NODE h);

            /**
             * Main acquire method, invoked by all exported acquire methods.
             *
             * @param node null unless a reacquiring Condition
             * @param arg the acquire argument
             * @param shared true if shared mode else exclusive
             * @param interruptible if abort and return negative on interrupt
             * @param timed if true use timed waits
             * @param time if timed, the System.nanoTime value to timeout
             * @return positive if acquired, 0 if timed out, negative if interrupted
             */
            gint acquire(NODE node, gint arg, gbool shared, gbool interruptible, gbool timed, glong time);

            /**
             * Spin-waits with backoff; used only upon OOME failures during acquire.
             */
            gint acquireOnOutOfMemoryException(gbool shared, gint arg);

            /**
             * Possibly repeatedly traverses from tail, unsplicing cancelled
             * nodes until none are found. Unparks nodes that may have been
             * relinked to be next eligible acquirer.
             */
            void cleanQueue();

            /**
             * Cancels an ongoing attempt to acquire.
             *
             * @param node the node (may be null if cancelled before enqueuing)
             * @param interrupted true if thread interrupted
             * @param interruptible if should report interruption vs reset
             */
            gint cancelAcquire(NODE node, gbool interrupted, gbool interruptible);
        };
    } // concurrent
} // core

#endif //CORE24_QUEUEDSYNCHRONIZER_H
