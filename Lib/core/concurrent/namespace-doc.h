//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {
    /**
     * Utility classes commonly useful in concurrent programming.  This
     * package includes a few small standardized extensible frameworks, as
     * well as some classes that provide useful functionality and are
     * otherwise tedious or difficult to implement.  Here are brief
     * descriptions of the main components.  See also the
     * @b concurrent::concurrent and @b concurrent::atomic packages.
     *
     * <h3>Executors</h3>
     *
     * <b>Interfaces</b>
     *
     * @b concurrent::Executor is a simple standardized
     * interface for defining custom thread-like subsystems, including
     * thread pools, asynchronous I/O, and lightweight task frameworks.
     * Depending on which concrete Executor class is being used, tasks may
     * execute in a newly created thread, an existing task-execution thread,
     * or the thread calling @b execute, and may execute sequentially or concurrently.
     *
     * @b concurrent::ExecutorService provides a more
     * complete asynchronous task execution framework.  An
     * ExecutorService manages queuing and scheduling of tasks,
     * and allows controlled shutdown.
     *
     * The @b concurrent::ScheduledExecutorService
     * sub-interface and associated interfaces add support for
     * delayed and periodic task execution.  ExecutorServices
     * provide methods arranging asynchronous execution of any
     * function expressed as @b concurrent::Callable,
     * the result-bearing analog of @b Runnable.
     *
     * A @b concurrent::Future returns the results of
     * a function, allows determination of whether execution has
     * completed, and provides a means to cancel execution.
     *
     * A @b concurrent::RunnableFuture is a @c Future
     * that possesses a @c run method that upon execution,
     * sets its results.
     *
     * <p>
     *
     * <b>Implementations.</b>
     *
     * Classes @b concurrent::ThreadPoolExecutor and
     * @b concurrent::ScheduledThreadPoolExecutor
     * provide tunable, flexible thread pools.
     *
     * The @b concurrent::Executors class provides
     * factory methods for the most common kinds and configurations
     * of Executors, as well as a few utility methods for using
     * them.  Other utilities based on @c Executors include the
     * concrete class @b concurrent::FutureTask
     * providing a common extensible implementation of Futures, and
     * @b concurrent::ExecutorCompletionService, that
     * assists in coordinating the processing of groups of
     * asynchronous tasks.
     * </p>
     * <p>
     * Class @b concurrent::ForkJoinPool provides an
     * Executor primarily designed for processing instances of
     * @b concurrent::ForkJoinTask and its subclasses.  These
     * classes employ a work-stealing scheduler that attains high
     * throughput for tasks conforming to restrictions that often hold in
     * computation-intensive parallel processing.
     * </p>
     *
     * <h3>Queues</h3>
     *
     * The @b concurrent::ConcurrentLinkedQueue class
     * supplies an efficient scalable thread-safe non-blocking FIFO queue.
     * The @b concurrent::ConcurrentLinkedDeque class is
     * similar, but additionally supports the @b Deque
     * interface.
     * <p>
     * Five implementations in @c core::concurrent support
     * the extended @b concurrent::BlockingQueue
     * interface, that defines blocking versions of put and take:
     * @b concurrent::LinkedBlockingQueue,
     * @b concurrent::ArrayBlockingQueue,
     * @b concurrent::SynchronousQueue,
     * @b concurrent::PriorityBlockingQueue, and
     * @b concurrent::DelayQueue.
     * The different classes cover the most common usage contexts
     * for producer-consumer, messaging, parallel tasking, and
     * related concurrent designs.
     * </p>
     * <p>
     * Extended interface @b concurrent::TransferQueue,
     * and implementation @b concurrent::LinkedTransferQueue
     * introduce a synchronous @c transfer method (along with related
     * features) in which a producer may optionally block awaiting its
     * consumer.
     * </p>
     * <p>
     * The @b concurrent::BlockingDeque interface
     * extends @c BlockingQueue to support both FIFO and LIFO
     * (stack-based) operations.
     * Class @b concurrent::LinkedBlockingDeque
     * provides an implementation.
     * </p>
     * <h3>Timing</h3>
     *
     * The @b concurrent::TimeUnit class provides
     * multiple granularities (including nanoseconds) for
     * specifying and controlling time-out based operations.  Most
     * classes in the package contain operations based on time-outs
     * in addition to indefinite waits.  In all cases that
     * time-outs are used, the time-out specifies the minimum time
     * that the method should wait before indicating that it
     * timed-out.  Implementations make a &quot;best effort&quot;
     * to detect time-outs as soon as possible after they occur.
     * However, an indefinite amount of time may elapse between a
     * time-out being detected and a thread actually executing
     * again after that time-out.  All methods that accept timeout
     * parameters treat values less than or equal to zero to mean
     * not to wait at all.  To wait "forever", you can use a value
     * of @c Long::MAX_VALUE.
     *
     * <h3>Synchronizers</h3>
     *
     * Five classes aid common special-purpose synchronization idioms. <br>
     *
     * - @b concurrent::Semaphore is a classic concurrency tool. <br/>
     *
     * - @b concurrent::CountDownLatch is a very simple yet
     * very common utility for blocking until a given number of signals,
     * events, or conditions hold. <br/>
     *
     * - A @b concurrent::CyclicBarrier is a resettable
     * multiway synchronization point useful in some styles of parallel
     * programming. <br/>
     *
     * - A @b concurrent::Phaser provides
     * a more flexible form of barrier that may be used to control phased
     * computation among multiple threads. <br/>
     *
     * - A @b concurrent::Exchanger allows two threads to
     * exchange objects at a rendezvous point, and is useful in several
     * pipeline designs. <br/>
     *
     * <h3>Concurrent Collections</h3>
     *
     * Besides Queues, this package supplies Collection implementations
     * designed for use in multithreaded contexts:
     * @b concurrent::ConcurrentHashMap,
     * @b concurrent::ConcurrentSkipListMap,
     * @b concurrent::ConcurrentSkipListSet,
     * @b concurrent::CopyOnWriteArrayList, and
     * @b concurrent::CopyOnWriteArraySet.
     * When many threads are expected to access a given collection, a
     * @c ConcurrentHashMap is normally preferable to a synchronized
     * @c HashMap, and a @c ConcurrentSkipListMap is normally
     * preferable to a synchronized @c TreeMap.
     * A @c CopyOnWriteArrayList is preferable to a synchronized
     * @c ArrayList when the expected number of reads and traversals
     * greatly outnumber the number of updates to a list.
     * 
     * <p>
     * The "Concurrent" prefix used with some classes in this package
     * is a shorthand indicating several differences from similar
     * "synchronized" classes.  For example @c java.util.Hashtable and
     * @c Collections::synchronizedMap(*new HashMap()) are
     * synchronized.  But @b concurrent::ConcurrentHashMap is "concurrent".  A
     * concurrent collection is thread-safe, but not governed by a
     * single exclusion lock.  In the particular case of
     * ConcurrentHashMap, it safely permits any number of
     * concurrent reads as well as a large number of concurrent
     * writes.  "Synchronized" classes can be useful when you need
     * to prevent all access to a collection via a single lock, at
     * the expense of poorer scalability.  In other cases in which
     * multiple threads are expected to access a common collection,
     * "concurrent" versions are normally preferable.  And
     * unsynchronized collections are preferable when either
     * collections are unshared, or are accessible only when
     * holding other concurrent.
     * </p>
     * <p>
     * Most concurrent Collection implementations
     * (including most Queues) also differ from the usual @c core::util
     * conventions in that their @em Iterators and @em Spliterators provide
     * <em>weakly consistent</em> rather than fast-fail traversal:
     * </p>
     *
     * - they may proceed concurrently with other operations <br/>
     * - they will never throw @b ConcurrentModificationException <br/>
     * - they are guaranteed to traverse elements as they existed upon
     * construction exactly once, and may (but are not guaranteed to)
     * reflect any modifications after construction. <br/>
     *
     * <h3>Memory Consistency Properties</h3>
     *
     * Chapter 17 of
     * <cite>The Java Language Specification</cite> defines the
     * <i>happens-before</i> relation on memory operations such as reads and
     * writes of shared variables.  The results of a write by one thread are
     * guaranteed to be visible to a read by another thread only if the write
     * operation <i>happens-before</i> the read operation.  The
     * @c synchronized and @c volatile constructs, as well as the
     * @c Thread.start() and @c Thread.join() methods, can form
     * <i>happens-before</i> relationships.  In particular: <br>
     *
     *
     *   - Each action in a thread <i>happens-before</i> every action in that
     *   thread that comes later in the program's order. <br/>
     *
     *   - An unlock (@c synchronized block or method exit) of a
     *   monitor <i>happens-before</i> every subsequent lock (@c synchronized
     *   block or method entry) of that same monitor.  And because
     *   the <i>happens-before</i> relation is transitive, all actions
     *   of a thread prior to unlocking <i>happen-before</i> all actions
     *   after any thread locking that monitor. <br/>
     *
     *   - A writing to a @c volatile field <i>happens-before</i> every
     *   subsequent read of that same field.  Writes and reads of
     *   @c volatile fields have similar memory consistency effects
     *   as entering and exiting monitors, but do <em>not</em> entail
     *   mutual exclusion locking. <br/>
     *
     *   - A call to @c start on a thread <i>happens-before</i> any
     *   action in the started thread. <br/>
     *
     *   - All actions in a thread <i>happen-before</i> any other thread
     *   successfully returns from a @c join on that thread. <br/>
     *
     *
     *
     * The methods of all classes in @c core::concurrent and its
     * subpackages extend these guarantees to higher-level
     * synchronization.  In particular: <br>
     *
     *   - Actions in a thread prior to placing an object into any concurrent
     *   collection <i>happen-before</i> actions after the access or
     *   removal of that element from the collection in another thread. <br/>
     *
     *   - Actions in a thread prior to the submission of a @c Runnable
     *   to an @c Executor <i>happen-before</i> its execution begins.
     *   Similarly, for @c Callables submitted to an @c ExecutorService. <br/>
     *
     *   - Actions taken by the asynchronous computation represented by a
     *   @c Future <i>happen-before</i> actions after the
     *   retrieval of the result via @c Future::get() in another thread. <br/>
     *
     *   - Actions prior to "releasing" synchronizer methods such as
     *   @c Lock::unlock, @c Semaphore::release, and
     *   @c CountDownLatch.countDown <i>happen-before</i> actions
     *   after a successful "acquiring" method such as
     *   @c Lock::lock, @c Semaphore::acquire,
     *   @c Condition::await, and @c CountDownLatch::await on the
     *   same synchronizer object in another thread. <br/>
     *
     *   - For each pair of threads that successfully exchange objects via
     *   an @c Exchanger, actions prior to the @c exchange()
     *   in each thread <i>happen-before</i> those after the
     *   corresponding @c exchange() in another thread. <br/>
     *
     *   - Actions prior to calling @c CyclicBarrier::await and
     *   @c Phaser::awaitAdvance (as well as its variants)
     *   <i>happen-before</i> actions performed by the barrier action, and
     *   actions performed by the barrier action <i>happen-before</i> actions
     *   after a successful return from the corresponding to @c await
     *   in other threads. <br/>
     */
    namespace concurrent {}
}

#endif //NAMESPACE_DOC_H
