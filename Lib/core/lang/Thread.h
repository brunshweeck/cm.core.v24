//
// Created by bruns on 20/09/2024.
//

#ifndef CORE24_THREAD_H
#define CORE24_THREAD_H

#include <core/lang/Runnable.h>
#include <core/lang/String.h>

#undef MAX_PRIORITY
#undef MIN_PRIORITY

namespace core {
  /**
   * A <i>thread</i> is a thread of execution in a program. The Core24
   * lib allows an application to have multiple threads of
   * execution running concurrently.
   *
   * <p>
   * @c Thread defines constructors to create threads.
   * @em Starting a thread schedules it to execute its @b run
   * method. The newly started thread executes concurrently with the thread that caused
   * it to start.
   * </p>
   * <p>
   * A thread <i>terminates</i> if either its @c run method completes normally,
   * or if its @c run method completes abruptly and the appropriate
   * <em> uncaught exception handler</em> completes normally or
   * abruptly. With no code left to run, the thread has completed execution. The
   * @b join method can be used to wait for a thread to terminate.
   * </p>
   * <p>
   * Threads have a unique @em identifier and a @em name.
   * The identifier is generated when a @c Thread is created
   * and cannot be changed. The thread name can be specified when creating a thread
   * or can be @em changed at a later time.
   * </p>
   * <h3> Platform threads </h3>
   * <p>
   * @c Thread supports the creation of <i>platform threads</i> that are
   * typically mapped 1:1 to kernel threads scheduled by the operating system.
   * Platform threads will usually have a large stack and other resources that are
   * maintained by the operating system. Platforms threads are suitable for executing
   * all types of tasks but may be a limited resource.
   * </p>
   * <p>
   * Platform threads get an automatically generated thread name by default.
   * </p>
   * <p>
   * Platform threads are designated <i>daemon</i> or <i>non-daemon</i> threads.
   * When it's internally starts up, there is usually one non-daemon
   * thread (the thread that typically calls the application's @em main method).
   * The shutdown sequence begins when all started
   * non-daemon threads have terminated. Unstarted non-daemon threads do not prevent
   * the shutdown sequence from beginning.
   * </p>
   * <p>
   * In addition to the daemon status, platform threads have a
   * <em> thread priority</em> and are members of a
   * <em>thread group</em>.
   * </p>
   *
   * <h3>Creating and starting threads</h3>
   *
   * <p>
   * @c Thread defines public constructors for creating platform threads and
   * the @b start method to schedule threads to execute. @c Thread
   * may be extended for customization and other advanced reasons although most
   * applications should have little need to do this.
   * </p>
   *
   * <h3> Inheritance when creating threads </h3>
   * <p>
   * A @c Thread inherits its initial values of
   * <em>inheritable-thread-local</em> variables (including the context class loader) from
   * the parent thread values at the time that the child @c Thread is created.
   * The 5-param @em constructor can be used to create a thread that does not inherit its initial
   * values from the constructing thread.
   * </p>
   * <p>
   * Platform threads inherit the daemon status, thread priority, and when not
   * provided (or not selected by a security manager), the thread group.
   * </p>
   * <p>
   * Creating a platform thread @em captures the <em> caller context</em> to limit the
   * @em permissions of the new thread when it executes code that performs a
   * <em> privileged action</em>. The captured caller context is the new thread's
   * @b "Inherited AccessControlContext".
   * </p>
   */
  class Thread : public virtual Runnable {
    CORE_ALIAS(TASK, Class<Runnable const>::Pointer);
    CORE_ALIAS(BLOCKER, Class<Object>::Pointer);
    CORE_ALIAS(CACHE, Class<Object>::Pointer);
    CORE_ALIAS(THREAD, Class<Thread>::Pointer);
    CORE_ALIAS(MONITOR, Class<concurrent::Lock>::Pointer);
    CORE_ADD_GLOBAL_FRIENDS();

    CORE_ADD_AS_FRIEND(concurrent::LockSupport);

    /*
     * Reserved for exclusive use. Cannot be moved to the FieldHolder
     * as it needs to be set by the VM for JNI attaching threads, before executing
     * the constructor that will create the FieldHolder. The historically named
     * `eetop` holds the address, and is set to
     * non-zero when the thread is started, and reset to zero when the thread terminates.
     * A non-zero value indicates this thread isAlive().
     */
    glong volatile eetop;

    // The thread id
    glong const tid = -1;

    // The thread name
    String name;

    // interrupt status
    gbool volatile status = false;

    // Additional fields for platform threads.
    // All fields, except task, are accessed directly internally.
    class Holder final : public Object {
      CORE_ADD_AS_FRIEND(Thread);

      TASK task;
      glong stackSize = 0LL;
      gint volatile priority = 5;
      gbool volatile daemon = false;
      gint volatile status = 0;

    public:
      CORE_EXPLICIT Holder(Object& task,
                           glong stackSize,
                           gint priority,
                           gbool daemon);
    };

    Holder holder;

    /**
     * The argument supplied to the current call to
     * concurrent::LockSupport::park.
     * Set by (private) concurrent::LockSupport::setBlocker
     * Accessed using concurrent.LockSupport::getBlocker
     */
    BLOCKER volatile blocker;
    CACHE volatile event;

    /*
     * The object in which this thread is blocked in an interruptible I/O
     * operation, if any.  The blocker's interrupt method should be invoked
     * after setting this thread's interrupt status.
     */
    BLOCKER volatile rwBlocker;

    /**
     * The Internal monitor for this thread
     */
    MONITOR monitor;

    Thread(Thread const& thread);

  protected:
    /**
     * Initializes a new platform @c Thread. This constructor has the same
     * effect as <em> Thread(Runnable,String) </em> where the name is a
     * newly generated name.
     *
     * <p>
     * This constructor is only useful when extending @c Thread to
     * override the @b run() method.
     * </p>
     * @see Inheritance when creating threads
     */
    CORE_IMPLICIT Thread();

  public:
    /**
     * A thread state.  A thread can be in one of the following states:
     *
     * <li>@b NEW<br>
     *     A thread that has not yet started is in this state.
     *     </li>
     * @li @b RUNNABLE<br>
     *     A thread executing is in this state.
     *
     * @li @b BLOCKED <br>
     *     A thread that is blocked waiting for a monitor lock
     *     is in this state.
     *
     * @li @b WAITING <br>
     *     A thread that is waiting indefinitely for another thread to
     *     perform a particular action is in this state.
     *
     * @li @b TIMED_WAITING <br>
     *     A thread that is waiting for another thread to perform an action
     *     for up to a specified waiting time is in this state.
     *
     * @li @b TERMINATED <br>
     *     A thread that has exited is in this state.
     *
     *
     *
     * <p>
     * A thread can be in only one state at a given point in time.
     * These states are virtual machine states which do not reflect
     * any operating system thread states.
     *
     * @see Thread::state
     */
    enum class State {
      /**
       * Thread state for a thread which has not yet started.
       */
      NEW,

      /**
       * Thread state for a runnable thread.  A thread in the runnable
       * state is executing, but it may
       * be waiting for other resources from the operating system
       * such as processor.
       */
      RUNNABLE,

      /**
       * Thread state for a thread blocked waiting for a monitor lock.
       * A thread in the blocked state is waiting for a monitor lock
       * to enter a synchronized block/method or
       * reenter a synchronized block/method after calling
       * @b Object::wait.
       */
      BLOCKED,

      /**
       * Thread state for a waiting thread.
       * A thread is in the waiting state due to calling one of the
       * following methods:
       *
       *   @li @b Object::wait with no timeout
       *   @li @b Thread::join with no timeout
       *   @li @b LockSupport::park
       *
       *
       * <p>
       * A thread in the waiting state is waiting for another thread to
       * perform a particular action.
       *
       * For example, a thread that has called @c Object.wait()
       * on an object is waiting for another thread to call
       * @c Object::notify() or @c Object::notifyAll() on
       * that object. A thread that has called @c Thread.join()
       * is waiting for a specified thread to terminate.
       * </p>
       */
      WAITING,

      /**
       * Thread state for a waiting thread with a specified waiting time.
       * A thread is in the timed waiting state due to calling one of
       * the following methods with a specified positive waiting time:
       *
       *   @li @b Thread::sleep
       *   @li @b Object::wait  with timeout
       *   @li @b Thread::join  with timeout
       *   @li @b LockSupport::parkNanos
       *   @li @b LockSupport::parkUntil
       *
       */
      TIMED_WAITING,

      /**
       * Thread state for a terminated thread.
       * The thread has completed execution.
       */
      TERMINATED
    };

    /* -------------------- Priority ---------------------------------- */

    /**
     * The priority of the parent thread.
     */
    static CORE_FAST gint INHERIT_PRIORITY = 0;

    /**
     * The minimum priority that a thread can have.
     */
    static CORE_FAST gint MIN_PRIORITY = 1;

    /**
     * scheduled only when no other threads are running.
     */
    static CORE_FAST gint IDLE_PRIORITY = 2;

    /**
     * scheduled less often than LOW_PRIORITY.
     */
    static CORE_FAST gint LOWEST_PRIORITY = 3;

    /**
     * scheduled less often than NORM_PRIORITY.
     */
    static CORE_FAST gint LOW_PRIORITY = 4;

    /**
     * The default priority that is assigned to a thread.
     */
    static CORE_FAST gint NORM_PRIORITY = 5;

    /**
     * scheduled more often than NORM_PRIORITY.
     */
    static CORE_FAST gint HIGH_PRIORITY = 6;

    /**
     * scheduled more often than HIGH_PRIORITY.
     */
    static CORE_FAST gint HIGHEST_PRIORITY = 7;

    /**
     * scheduled as often as possible.
     */
    static CORE_FAST gint TIME_CRITICAL_PRIORITY = 9;

    /**
     * The maximum priority that a thread can have.
     */
    static CORE_FAST gint MAX_PRIORITY = 10;

    /* ----------------------- Methods ----------------------------- */

    /**
     * Returns the Thread object for the current thread.
     * @return  the current thread
     */
    static Thread& currentThread();

    /**
     * A hint to the scheduler that the current thread is willing to yield
     * its current use of a processor. The scheduler is free to ignore this
     * hint.
     *
     * <p>
     * Yield is a heuristic attempt to improve relative progression
     * between threads that would otherwise over-utilise a CPU. Its use
     * should be combined with detailed profiling and benchmarking to
     * ensure that it actually has the desired effect.
     * </p>
     * <p>
     * It is rarely appropriate to use this method. It may be useful
     * for debugging or testing purposes, where it may help to reproduce
     * bugs due to race conditions. It may also be useful when designing
     * concurrency control constructs such as the ones in the
     * @c core::concurrent namespace.
     * </p>
     */
    static void yield();

    /**
     * Causes the currently executing thread to sleep (temporarily cease
     * execution) for the specified number of milliseconds, subject to
     * the precision and accuracy of system timers and schedulers. The thread
     * does not lose ownership of any monitors.
     *
     * @param  millis
     *         the length of time to sleep in milliseconds
     *
     * @throws  IllegalArgumentException
     *          if the value of @c millis is negative
     *
     * @throws  InterruptedException
     *          if any thread has interrupted the current thread. The
     *          <i>interrupted status</i> of the current thread is
     *          cleared when this exception is thrown.
     */
    static void sleep(glong millis);

    /**
     * Causes the currently executing thread to sleep (temporarily cease
     * execution) for the specified number of milliseconds plus the specified
     * number of nanoseconds, subject to the precision and accuracy of system
     * timers and schedulers. The thread does not lose ownership of any
     * monitors.
     *
     * @param  millis
     *         the length of time to sleep in milliseconds
     *
     * @param  nanos
     *         @c 0-999999 additional nanoseconds to sleep
     *
     * @throws  IllegalArgumentException
     *          if the value of @c millis is negative, or the value of
     *          @c nanos is not in the range @c 0-999999
     *
     * @throws  InterruptedException
     *          if any thread has interrupted the current thread. The
     *          <i>interrupted status</i> of the current thread is
     *          cleared when this exception is thrown.
     */
    static void sleep(glong millis, gint nanos);

    /**
     * Causes the currently executing thread to sleep (temporarily cease
     * execution) for the specified duration, subject to the precision and
     * accuracy of system timers and schedulers. This method is a no-op if
     * the duration is {@linkplain Duration#isNegative() negative}.
     *
     * @param  duration
     *         the duration to sleep
     *
     * @throws  InterruptedException
     *          if the current thread is interrupted while sleeping. The
     *          <i>interrupted status</i> of the current thread is
     *          cleared when this exception is thrown.
     */
    static void sleep(time::Duration const& duration);

    /**
     * Indicates that the caller is momentarily unable to progress, until the
     * occurrence of one or more actions on the part of other activities. By
     * invoking this method within each iteration of a spin-wait loop construct,
     * the calling thread indicates to the runtime that it is busy-waiting.
     * The runtime may take action to improve the performance of invoking
     * spin-wait loop constructions.
     *
     * @apiNote
     * As an example consider a method in a class that spins in a loop until
     * some flag is set outside of that method. A call to the @c onSpinWait
     * method should be placed inside the spin loop.
     * @code
     *     class EventHandler {
     *         volatile gbool eventNotificationNotReceived;
     *         void waitForEventAndHandleIt() {
     *             while ( eventNotificationNotReceived ) {
     *                 Thread.onSpinWait();
     *             }
     *             readAndProcessEvent();
     *         }
     *
     *         void readAndProcessEvent() {
     *             // Read event from some source and process it...
     *         }
     *     };
     * @endcode
     * <p>
     * The code above would remain correct even if the @c onSpinWait
     * method was not called at all.
     * </p>
     */
    static void onSpinWait();

    /**
     * Throws CloneNotSupportedException as a Thread can not be meaningfully
     * cloned. Construct a new Thread instead.
     *
     * @throws  CloneNotSupportedException
     *          always
     */
    Object& clone() const override;

    /**
     * Initializes a new platform @c Thread. This constructor has the same
     * effect as <em> Thread(Runnable,String) </em>, where name is a newly generated
     * name.
     *
     * @param  task
     *         the object whose @c run method is invoked when this thread
     *         is started.
     *
     * @see Inheritance when creating threads
     */
    static Thread& newThread(Runnable const& task);

    /**
     * Initializes a new platform @c Thread.
     *
     * <p>
     * This constructor is only useful when extending @c Thread to
     * override the @b run() method.
     * </p>
     *
     * @param   name
     *          the name of the new thread
     *
     * @see Inheritance when creating threads
     */
    static Thread& newThread(String const& name);

    /**
     * Initializes a new platform @c Thread.
     *
     * @param  task
     *         the object whose @c run method is invoked when this thread
     *         is started.
     *
     * @param  name
     *         the name of the new thread
     *
     * @see Inheritance when creating threads
     */
    static Thread& newThread(Runnable const& task, String const& name);

    /**
     * Initializes a new platform @c Thread so that it has @c task
     * as its run object, has the specified @c name as its name,
     * and belongs to the thread group referred to by @c group, and has
     * the specified <i>stack size</i>.
     *
     * <p>
     * This constructor is identical to <b> Thread(Runnable, String) </b>
     * except for the fact that it allows the thread stack size to be specified.
     * The stack size
     * is the approximate number of bytes of address space that the virtual
     * machine is to allocate for this thread's stack.  <b>The effect of the
     * @c stackSize parameter, if any, is highly platform dependent.</b>
     * </p>
     * <p>
     * On some platforms, specifying a higher value for the
     * @c stackSize parameter may allow a thread to achieve greater
     * recursion depth before throwing a @b StackOverflowError.
     * Similarly, specifying a lower value may allow a greater number of
     * threads to exist concurrently without throwing an
     * @b OutOfMemoryError (or other internal error).  The details of
     * the relationship between the value of the @c stackSize parameter
     * and the maximum recursion depth and concurrency level are
     * platform-dependent.  <b>On some platforms, the value of the
     * @c stackSize parameter may have no effect whatsoever.</b>
     * </p>
     * <p>
     * The virtual machine is free to treat the @c stackSize
     * parameter as a suggestion.  If the specified value is unreasonably low
     * for the platform, the virtual machine may instead use some
     * platform-specific minimum value; if the specified value is unreasonably
     * high, the virtual machine may instead use some platform-specific
     * maximum.  Likewise, the virtual machine is free to round the specified
     * value up or down as it sees fit (or to ignore it completely).
     * </p>
     * <p>
     * Specifying a value of zero for the @c stackSize parameter will
     * cause this constructor to behave exactly like the
     * @code Thread(ThreadGroup, Runnable, String) @endcode constructor.
     * </p>
     * <p>
     * <i>Due to the platform-dependent nature of the behavior of this
     * constructor, extreme care should be exercised in its use.
     * The thread stack size necessary to perform a given computation will
     * likely vary from one JRE implementation to another.  In light of this
     * variation, careful tuning of the stack size parameter may be required,
     * and the tuning may need to be repeated for each JRE implementation on
     * which an application is to run.</i>
     * </p>
     *
     * @param  task
     *         the object whose @c run method is invoked when this thread
     *         is started.
     *
     * @param  name
     *         the name of the new thread
     *
     * @param  stackSize
     *         the desired stack size for the new thread, or zero to indicate
     *         that this parameter is to be ignored.
     *
     * @throws  SecurityException
     *          if the current thread cannot create a thread in the specified
     *          thread group
     *
     * @see Inheritance when creating threads
     */
    static Thread& newThread(Runnable const& task, String const& name, glong stackSize);

    ~Thread() override;

    /**
     * Schedules this thread to begin execution. The thread will execute
     * independently of the current thread.
     *
     * <p>
     * A thread can be started at most once. In particular, a thread can not
     * be restarted after it has terminated.
     * </p>
     * @throws IllegalStateException if the thread was already started
     */
    void start() const;

    /**
     * This method is run by the thread when it executes. Subclasses of
     * @c Thread may override this method.
     *
     * <p>
     * This method is not intended to be invoked directly. If this thread is a
     * platform thread created with a @b Runnable task then invoking this method
     * will invoke the task's @c run method. If this thread is a virtual thread
     * then invoking this method directly does nothing.
     * </p>
     *
     * @note The default implementation executes the @b Runnable task that
     * the @c Thread was created with. If the thread was created without a task
     * then this method does nothing.
     */
    void run() const override;

    /**
     * Forces the thread to stop executing.
     */
    void stop() const;

    /**
     * Terminate current thread.
     */
    static void exit();

    /**
     * Interrupts this thread.
     *
     * <p>
     * If this thread is blocked in an invocation of the
     * @b  wait(), @b wait(long), or <b> wait(long, int)</b> methods,
     * or of the @b join(), @b join(long), <b>join(long, int)</b>, @b sleep(long),
     * or <b>sleep(long, int)</b> methods of this class, then its interrupt status
     * will be cleared, and it will receive an @b InterruptedException.
     * </p>
     *
     * <p>
     * If none of the previous conditions hold then this thread's interrupt
     * status will be set.
     * </p>
     *
     * <p>
     * Interrupting a thread that is not alive need not have any effect.
     * </p>
     *
     * @throws  SecurityException
     *          if the current thread cannot modify this thread
     */
    void interrupt();

    /**
     * Tests whether the current thread has been interrupted.  The
     * <i>interrupted status</i> of the thread is cleared by this method.  In
     * other words, if this method were to be called twice in succession, the
     * second call would return false (unless the current thread were
     * interrupted again, after the first call had cleared its interrupted
     * status and before the second call had examined it).
     *
     * @return  @c true if the current thread has been interrupted;
     *          @c false otherwise.
     * @see isInterrupted()
     */
    static gbool interrupted();

    /**
     * Tests whether this thread has been interrupted.  The <i>interrupted
     * status</i> of the thread is unaffected by this method.
     *
     * @return  @c true if this thread has been interrupted;
     *          @c false otherwise.
     * @see interrupted()
     */
    gbool isInterrupted() const;

    /**
     * Tests if this thread is alive. A thread is alive if it has
     * been started and has not yet terminated.
     *
     * @return  @c true if this thread is alive;
     *          @c false otherwise.
     */
    gbool isAlive() const;

    /**
     * Suspends this thread.
     */
    void suspend() const;

    /**
     * Resumes a suspended thread.
     */
    void resume() const;

    /**
     * Changes the priority of this thread.
     *
     * For platform threads, the priority is set to the smaller of the specified
     * @c newPriority and the maximum permitted priority of the thread's
     * <em> thread group </em>.
     *
     * @param newPriority the new thread priority
     * @throws  IllegalArgumentException if the priority is not in the
     *          range @c MIN_PRIORITY to @c MAX_PRIORITY.
     * @throws  SecurityException
     *          if the current thread cannot modify this thread
     *
     * @see setPriority(int)
     */
    void setPriority(gint newPriority);

    /**
     * Returns this thread's priority.
     *
     * @return  this thread's priority.
     * @see setPriority
     */
    gint priority() const;

    /**
     * Changes the name of this thread to be equal to the argument @c name.
     *
     * @param      name   the new name for this thread.
     * @throws     SecurityException  if the current thread cannot modify this
     *             thread.
     *
     * @see getName
     * @see checkAccess()
     */
    void setName(String const& name);

    /**
     * Returns this thread's name.
     *
     * @return  this thread's name.
     * @see setName(String)
     */
    String getName() const;

    /**
     * Waits at most @c millis milliseconds for this thread to terminate.
     * A timeout of @c 0 means to wait forever.
     * This method returns immediately, without waiting, if the thread has not
     * been @b started.
     *
     * @note
     * For platform threads, the implementation uses a loop of @c this.wait
     * calls conditioned on @c this.isAlive. As a thread terminates the
     * @c this.notifyAll method is invoked. It is recommended that
     * applications not use @c wait, @c notify, or
     * @c notifyAll on @c Thread instances.
     *
     * @param  millis
     *         the time to wait in milliseconds
     *
     * @throws  IllegalArgumentException
     *          if the value of @c millis is negative
     *
     * @throws  InterruptedException
     *          if any thread has interrupted the current thread. The
     *          <i>interrupted status</i> of the current thread is
     *          cleared when this exception is thrown.
     */
    void join(glong millis) const;

    /**
     * Waits at most @c millis milliseconds plus
     * @c nanos nanoseconds for this thread to terminate.
     * If both arguments are @c 0, it means to wait forever.
     * This method returns immediately, without waiting, if the thread has not
     * been @b started.
     *
     * @note
     * For platform threads, the implementation uses a loop of @c this.wait
     * calls conditioned on @c this.isAlive. As a thread terminates the
     * @c this.notifyAll method is invoked. It is recommended that
     * applications not use @c wait, @c notify, or
     * @c notifyAll on @c Thread instances.
     *
     * @param  millis
     *         the time to wait in milliseconds
     *
     * @param  nanos
     *         @c 0-999999 additional nanoseconds to wait
     *
     * @throws  IllegalArgumentException
     *          if the value of @c millis is negative, or the value
     *          of @c nanos is not in the range @c 0-999999
     *
     * @throws  InterruptedException
     *          if any thread has interrupted the current thread. The
     *          <i>interrupted status</i> of the current thread is
     *          cleared when this exception is thrown.
     */
    void join(glong millis, gint nanos) const;

    /**
     * Waits for this thread to terminate for up to the given waiting duration.
     *
     * <p> This method does not wait if the duration to wait is less than or
     * equal to zero. In this case, the method just tests if the thread has
     * terminated.
     *
     * @param   duration
     *          the maximum duration to wait
     *
     * @return  @c true if the thread has terminated, @c false if the
     *          thread has not terminated
     *
     * @throws  InterruptedException
     *          if the current thread is interrupted while waiting.
     *          The <i>interrupted status</i> of the current thread is cleared
     *          when this exception is thrown.
     *
     * @throws  IllegalStateException
     *          if this thread has not been started.
     */
    gbool join(time::Duration const& duration) const;

    /**
     * Waits for this thread to terminate.
     *
     * <p>
     * An invocation of this method behaves in exactly the same
     * way as the invocation
     *
     * @code
     *    join(0)
     * @endcode
     *
     * @throws  InterruptedException
     *          if any thread has interrupted the current thread. The
     *          <i>interrupted status</i> of the current thread is
     *          cleared when this exception is thrown.
     */
    void join() const;

    /**
     * Marks this thread as either a <i>daemon</i> or <i>non-daemon</i> thread.
     * The shutdown sequence begins when all started non-daemon threads have terminated.
     *
     * <p>
     * The daemon status of a virtual thread is always @c true and cannot be
     * changed by this method to @c false.
     * </p>
     * <p>
     * This method must be invoked before the thread is started. The behavior
     * of this method when the thread has terminated is not specified.
     * </p>
     * @param  on
     *         if @c true, marks this thread as a daemon thread
     *
     * @throws  IllegalArgumentException
     *          if this is a virtual thread and @c on is false
     * @throws  IllegalStateException
     *          if this thread is @em alive
     * @throws  SecurityException
     *          if the current thread cannot modify this thread
     */
    virtual void setDaemon(gbool on);

    /**
     * Tests if this thread is a daemon thread.
     * The daemon status of a virtual thread is always @c true.
     *
     * @return  @c true if this thread is a daemon thread;
     *          @c false otherwise.
     * @see setDaemon(boolean)
     */
    virtual gbool isDaemon() const;

    /**
     * Returns a string representation of this thread. The string representation
     * will usually include the thread's @em identifier and
     * name. The default implementation for platform threads includes the thread's
     * identifier, name, priority, and the name of the thread group.
     *
     * @return  a string representation of this thread.
     */
    String toString() const override;

    /**
     * Returns the identifier of this Thread.  The thread ID is a positive
     * @c long number generated when this thread was created.
     * The thread ID is unique and remains unchanged during its lifetime.
     *
     * @return this thread's ID
     */
    glong threadId() const;

    /**
     * Returns the state of this thread.
     * This method is designed for use in monitoring of the system state,
     * not for synchronization control.
     *
     * @return this thread's state.
     */
    State threadState() const;

    gbool equals(const Object& o) const override;

    gint hash() const override;
  };
} // core

#endif //CORE24_THREAD_H
