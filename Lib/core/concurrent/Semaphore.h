//
// Created by bruns on 06/10/2024.
//

#ifndef CORE24_SEMAPHORE_H
#define CORE24_SEMAPHORE_H

#include <core/concurrent/QueuedSynchronizer.h>

namespace core {
  namespace concurrent {
    /**
     * A counting semaphore.  Conceptually, a semaphore maintains a set of
     * permits.  Each @b acquire blocks if necessary until a permit is
     * available, and then takes it.  Each @b release adds a permit,
     * potentially releasing a blocking acquirer.
     * However, no actual permit objects are used; the @c Semaphore just
     * keeps a count of the number available and acts accordingly.
     *
     * <p>
     * Semaphores are often used to restrict the number of threads than can
     * access some (physical or logical) resource. For example, here is
     * a class that uses a semaphore to control access to a pool of items:
     * @code
     * class Pool {
     *   static CORE_FAST int MAX_AVAILABLE = 100;
     *   Semaphore available = Semaphore(MAX_AVAILABLE, true);
     *
     *   public:
     *   Object& getItem() {
     *     available.acquire();
     *     return getNextAvailableItem();
     *   }
     *
     *   void putItem(Object const &x) {
     *     if (markAsUnused(x))
     *       available.release();
     *   }
     *
     *   // Not a particularly efficient data structure; just for demo
     *
     *   protected:
     *   Array<> items = ...; // whatever kinds of items being managed
     *   BooleanArray used = BooleanArray(MAX_AVAILABLE);
     *
     *   Object& getNextAvailableItem() {
     *     for (int i = 0; i < MAX_AVAILABLE; ++i) {
     *       if (!used[i]) {
     *         used[i] = true;
     *         return items[i];
     *       }
     *     }
     *     return null; // not reached
     *   }
     *
     *   gbool markAsUnused(Object const &item) {
     *     for (int i = 0; i < MAX_AVAILABLE; ++i) {
     *       if (item == items[i]) {
     *         if (used[i]) {
     *           used[i] = false;
     *           return true;
     *         } else
     *           return false;
     *       }
     *     }
     *     return false;
     *   }
     * };
     * @endcode
     * </p>
     * <p>
     * Before obtaining an item each thread must acquire a permit from
     * the semaphore, guaranteeing that an item is available for use. When
     * the thread has finished with the item it is returned back to the
     * pool and a permit is returned to the semaphore, allowing another
     * thread to acquire that item.  Note that no synchronization lock is
     * held when @b acquire is called as that would prevent an item
     * from being returned to the pool.  The semaphore encapsulates the
     * synchronization needed to restrict access to the pool, separately
     * from any synchronization needed to maintain the consistency of the
     * pool itself.
     * </p>
     * <p>
     * A semaphore initialized to one, and which is used such that it
     * only has at most one permit available, can serve as a mutual
     * exclusion lock.  This is more commonly known as a <em>binary
     * semaphore</em>, because it only has two states: one permit
     * available, or zero permits available.  When used in this way, the
     * binary semaphore has the property (unlike many @b concurrent::Lock
     * implementations), that the ¨lock¨ can be released by a
     * thread other than the owner (as semaphores have no notion of
     * ownership).  This can be useful in some specialized contexts, such
     * as deadlock recovery.
     * </p>
     * <p>
     * The constructor for this class optionally accepts a
     * <em>fairness</em> parameter. When set false, this class makes no
     * guarantees about the order in which threads acquire permits. In
     * particular, <em>barging</em> is permitted, that is, a thread
     * invoking @b acquire can be allocated a permit ahead of a
     * thread that has been waiting - logically the new thread places itself at
     * the head of the queue of waiting threads. When fairness is set true, the
     * semaphore guarantees that threads invoking any of the
     * @b acquire methods are selected to obtain permits in the order in
     * which their invocation of those methods was processed
     * (first-in-first-out; FIFO). Note that FIFO ordering necessarily
     * applies to specific internal points of execution within these
     * methods.  So, it is possible for one thread to invoke
     * @c acquire before another, but reach the ordering point after
     * the other, and similarly upon return from the method.
     * Also note that the untimed @b tryAcquire methods do not
     * honor the fairness setting, but will take any permits that are
     * available.
     * </p>
     * <p>
     * Generally, semaphores used to control resource access should be
     * initialized as fair, to ensure that no thread is starved out from
     * accessing a resource. When using semaphores for other kinds of
     * synchronization control, the throughput advantages of non-fair
     * ordering often outweigh fairness considerations.
     * </p>
     * <p>
     * This class also provides convenience methods to
     * @b acquire and @b release multiple
     * permits at a time. These methods are generally more efficient and
     * effective than loops. However, they do not establish any preference
     * order. For example, if thread A invokes @c s.acquire(3) and
     * thread B invokes @c s.acquire(2), and two permits become
     * available, then there is no guarantee that thread B will obtain
     * them unless its acquire came first and Semaphore @c s is in
     * fair mode.
     * </p>
     * <p>
     * Memory consistency effects: Actions in a thread prior to calling
     * a "release" method such as @c release() <i>happen-before</i>
     * actions following a successful "acquire" method such as @c acquire()
     * in another thread.
     * </p>
     */
    class Semaphore : public virtual Object {
      class Synchronizer;
      class FairSync;
      class NonFairSync;

      CORE_ALIAS(UNSAFE, misc::Unsafe);
      CORE_ALIAS(Sync, Class<Synchronizer>::Pointer);

      /** All mechanics via QS subclass */
      Sync sync = { };

    public:
      /**
       * Creates a @c Semaphore with the given number of
       * permits and nonfair fairness setting.
       *
       * @param permits the initial number of permits available.
       *        This value may be negative, in which case releases
       *        must occur before any acquires will be granted.
       */
      CORE_EXPLICIT Semaphore(gint permits);

      /**
       * Creates a @c Semaphore with the given number of
       * permits and the given fairness setting.
       *
       * @param permits the initial number of permits available.
       *        This value may be negative, in which case releases
       *        must occur before any acquires will be granted.
       * @param fair @c true if this semaphore will guarantee
       *        first-in first-out granting of permits under contention,
       *        else @c false
       */
      CORE_EXPLICIT Semaphore(gint permits, gbool fair);

      /**
       * Acquires a permit from this semaphore, blocking until one is
       * available, or the thread is @em interrupted.
       *
       * <p>
       * Acquires a permit, if one is available and returns immediately,
       * reducing the number of available permits by one.
       * </p>
       * <p>
       * If no permit is available then the current thread becomes
       * disabled for thread scheduling purposes and lies dormant until
       * one of two things happens:
       *
       * @li Some other thread invokes the @b release method for this
       * semaphore and the current thread is next to be assigned a permit; or
       * @li Some other thread @em interrupts
       * the current thread.
       *
       * </p>
       * <p>
       * If the current thread:
       *
       * @li has its interrupted status set on entry to this method; or
       * @li is @em interrupted while waiting
       * for a permit,
       *
       * then @b InterruptedException is thrown and the current thread's
       * interrupted status is cleared.
       * </p>
       * @throws InterruptedException if the current thread is interrupted
       */
      virtual void acquire();

      /**
       * Acquires a permit from this semaphore, blocking until one is
       * available.
       *
       * <p>
       * Acquires a permit, if one is available and returns immediately,
       * reducing the number of available permits by one.
       * </p>
       * <p>
       * If no permit is available then the current thread becomes
       * disabled for thread scheduling purposes and lies dormant until
       * some other thread invokes the @b release method for this
       * semaphore and the current thread is next to be assigned a permit.
       * </p>
       * <p>
       * If the current thread is @em interrupted
       * while waiting for a permit then it will continue to wait, but the
       * time at which the thread is assigned a permit may change compared to
       * the time it would have received the permit had no interruption
       * occurred.  When the thread does return from this method its interrupt
       * status will be set.
       * </p>
       */
      virtual void acquireUninterruptibly();

      /**
       * Acquires a permit from this semaphore, only if one is available at the
       * time of invocation.
       *
       * <p>
       * Acquires a permit, if one is available and returns immediately,
       * with the value @c true,
       * reducing the number of available permits by one.
       * </p>
       * <p>
       * If no permit is available then this method will return
       * immediately with the value @c false.
       * </p>
       * <p>
       * Even when this semaphore has been set to use a
       * fair ordering policy, a call to @c tryAcquire() <em>will</em>
       * immediately acquire a permit if one is available, whether
       * other threads are currently waiting.
       * This &quot;barging&quot; behavior can be useful in certain
       * circumstances, even though it breaks fairness. If you want to honor
       * the fairness setting, then use
       * <b> tryAcquire(0, TemporalUnit::SECONDS) </b>
       * which is almost equivalent (it also detects interruption).
       * </p>
       * @return @c true if a permit was acquired and @c false
       *         otherwise
       */
      virtual gbool tryAcquire();

      /**
       * Acquires a permit from this semaphore, if one becomes available
       * within the given waiting time and the current thread has not
       * been @em interrupted.
       *
       * <p>
       * Acquires a permit, if one is available and returns immediately,
       * with the value @c true,
       * reducing the number of available permits by one.
       * </p>
       * <p>
       * If no permit is available then the current thread becomes
       * disabled for thread scheduling purposes and lies dormant until
       * one of three things happens:
       *
       * @li Some other thread invokes the @b release method for this
       * semaphore and the current thread is next to be assigned a permit; or
       * @li Some other thread @em interrupts
       * the current thread; or
       * @li The specified waiting time elapses.
       *
       * </p>
       * <p>
       * If a permit is acquired then the value @c true is returned.
       * </p>
       * <p>
       * If the current thread:
       *
       * @li has its interrupted status set on entry to this method; or
       * @li is @em interrupted while waiting
       * to acquire a permit,
       *
       * then @b InterruptedException is thrown and the current thread's
       * interrupted status is cleared.
       * </p>
       * <p>
       * If the specified waiting time elapses then the value @c false
       * is returned.  If the time is less than or equal to zero, the method
       * will not wait at all.
       * </p>
       * @param timeout the maximum time to wait for a permit
       * @param unit the time unit of the @c timeout argument
       * @return @c true if a permit was acquired and @c false
       *         if the waiting time elapsed before a permit was acquired
       * @throws InterruptedException if the current thread is interrupted
       */
      virtual gbool tryAcquire(glong timeout, time::TemporalUnit unit);

      /**
       * Releases a permit, returning it to the semaphore.
       *
       * <p>
       * Releases a permit, increasing the number of available permits by
       * one.  If any threads are trying to acquire a permit, then one is
       * selected and given the permit that was just released.  That thread
       * is (re)enabled for thread scheduling purposes.
       * </p>
       * <p>
       * There is no requirement that a thread that releases a permit must
       * have acquired that permit by calling @b acquire.
       * Correct usage of a semaphore is established by programming convention
       * in the application.
       * </p>
       */
      virtual void release();

      /**
       * Acquires the given number of permits from this semaphore,
       * blocking until all are available,
       * or the thread is @em interrupted.
       *
       * <p>
       * Acquires the given number of permits, if they are available,
       * and returns immediately, reducing the number of available permits
       * by the given amount. This method has the same effect as the
       * loop @code for (int i = 0; i < permits; ++i) acquire(); @endcode except
       * that it atomically acquires the permits all at once:
       * </p>
       * <p>
       * If insufficient permits are available then the current thread becomes
       * disabled for thread scheduling purposes and lies dormant until
       * one of two things happens:
       *
       * @li Some other thread invokes one of the @b release
       * methods for this semaphore and the current thread is next to be assigned
       * permits and the number of available permits satisfies this request; or
       * @li Some other thread @em interrupts
       * the current thread.
       *
       * </p>
       * <p>
       * If the current thread:
       *
       * @li has its interrupted status set on entry to this method; or
       * @li is @em interrupted while waiting
       * for a permit,
       *
       * then @b InterruptedException is thrown and the current thread's
       * interrupted status is cleared.
       * Any permits that were to be assigned to this thread are instead
       * assigned to other threads trying to acquire permits, as if
       * permits had been made available by a call to @b release().
       * </p>
       * @param permits the number of permits to acquire
       * @throws InterruptedException if the current thread is interrupted
       * @throws IllegalArgumentException if @c permits is negative
       */
      virtual void acquire(gint permits);

      /**
       * Acquires the given number of permits from this semaphore,
       * blocking until all are available.
       *
       * <p>
       * Acquires the given number of permits, if they are available,
       * and returns immediately, reducing the number of available permits
       * by the given amount. This method has the same effect as the
       * loop @code for (int i = 0; i < permits; ++i) acquireUninterruptibly(); @endcode
       * except that it atomically acquires the permits all at once:
       * </p>
       * <p>
       * If insufficient permits are available then the current thread becomes
       * disabled for thread scheduling purposes and lies dormant until
       * some other thread invokes one of the @b release
       * methods for this semaphore and the current thread is next to be assigned
       * permits and the number of available permits satisfies this request.
       * </p>
       * <p>
       * If the current thread is @em interrupted
       * while waiting for permits then it will continue to wait and its
       * position in the queue is not affected.  When the thread does return
       * from this method its interrupt status will be set.
       * </p>
       * @param permits the number of permits to acquire
       * @throws IllegalArgumentException if @c permits is negative
       */
      virtual void acquireUninterruptibly(gint permits);

      /**
       * Acquires the given number of permits from this semaphore, only
       * if all are available at the time of invocation.
       *
       * <p>
       * Acquires the given number of permits, if they are available, and
       * returns immediately, with the value @c true,
       * reducing the number of available permits by the given amount.
       * </p>
       * <p>
       * If insufficient permits are available then this method will return
       * immediately with the value @c false and the number of available
       * permits is unchanged.
       * </p>
       * <p>
       * Even when this semaphore has been set to use a fair ordering
       * policy, a call to @c tryAcquire <em>will</em>
       * immediately acquire a permit if one is available, whether
       *  other threads are currently waiting.  This
       * &quot;barging&quot; behavior can be useful in certain
       * circumstances, even though it breaks fairness. If you want to
       * honor the fairness setting, then use <b>
       * tryAcquire(permits, 0, TemporalUnit::SECONDS) </b>
       * which is almost equivalent (it also detects interruption).
       * </p>
       * @param permits the number of permits to acquire
       * @return @c true if the permits were acquired and
       *         @c false otherwise
       * @throws IllegalArgumentException if @c permits is negative
       */
      virtual gbool tryAcquire(gint permits);

      /**
       * Acquires the given number of permits from this semaphore, if all
       * become available within the given waiting time and the current
       * thread has not been @em interrupted.
       *
       * <p>
       * Acquires the given number of permits, if they are available and
       * returns immediately, with the value @c true,
       * reducing the number of available permits by the given amount.
       * </p>
       * <p>
       * If insufficient permits are available then
       * the current thread becomes disabled for thread scheduling
       * purposes and lies dormant until one of three things happens:
       *
       * @li Some other thread invokes one of the @b release
       * methods for this semaphore and the current thread is next to be assigned
       * permits and the number of available permits satisfies this request; or
       * @li Some other thread @em interrupts
       * the current thread; or
       * @li The specified waiting time elapses.
       *
       * </p>
       * <p>
       * If the permits are acquired then the value @c true is returned.
       * </p>
       * <p>
       * If the current thread:
       *
       * @li has its interrupted status set on entry to this method; or
       * @li is @em interrupted while waiting
       * to acquire the permits,
       *
       * then @b InterruptedException is thrown and the current thread's
       * interrupted status is cleared.
       * Any permits that were to be assigned to this thread, are instead
       * assigned to other threads trying to acquire permits, as if
       * the permits had been made available by a call to @b release().
       * </p>
       * <p>
       * If the specified waiting time elapses then the value @c false
       * is returned.  If the time is less than or equal to zero, the method
       * will not wait at all.  Any permits that were to be assigned to this
       * thread, are instead assigned to other threads trying to acquire
       * permits, as if the permits had been made available by a call to
       * @b release().
       * </p>
       * @param permits the number of permits to acquire
       * @param timeout the maximum time to wait for the permits
       * @param unit the time unit of the @c timeout argument
       * @return @c true if all permits were acquired and @c false
       *         if the waiting time elapsed before all permits were acquired
       * @throws InterruptedException if the current thread is interrupted
       * @throws IllegalArgumentException if @c permits is negative
       */
      virtual gbool tryAcquire(gint permits, glong timeout, time::TemporalUnit unit);

      /**
       * Releases the given number of permits, returning them to the semaphore.
       *
       * <p>
       * Releases the given number of permits, increasing the number of
       * available permits by that amount.
       * If any threads are trying to acquire permits, then one thread
       * is selected and given the permits that were just released.
       * If the number of available permits satisfies that thread's request
       * then that thread is (re)enabled for thread scheduling purposes;
       * otherwise the thread will wait until sufficient permits are available.
       * If there are still permits available
       * after this thread's request has been satisfied, then those permits
       * are assigned in turn to other threads trying to acquire permits.
       * </p>
       * <p>
       * There is no requirement that a thread that releases a permit must
       * have acquired that permit by calling @b acquire.
       * Correct usage of a semaphore is established by programming convention
       * in the application.
       * </p>
       * @param permits the number of permits to release
       * @throws IllegalArgumentException if @c permits is negative
       */
      virtual void release(gint permits);

      /**
       * Returns the current number of permits available in this semaphore.
       *
       * <p>
       * This method is typically used for debugging and testing purposes.
       * </p>
       * @return the number of permits available in this semaphore
       */
      virtual gint availablePermits() const;

      /**
       * Acquires and returns all permits that are immediately
       * available, or if negative permits are available, releases them.
       * Upon return, zero permits are available.
       *
       * @return the number of permits acquired or, if negative, the
       * number released
       */
      virtual gint drainPermits() const;

      /**
       * Returns @c true if this semaphore has fairness set true.
       *
       * @return @c true if this semaphore has fairness set true
       */
      virtual gbool isFair() const;

      /**
       * Queries whether any threads are waiting to acquire. Note that
       * because cancellations may occur at any time, a @c true
       * return does not guarantee that any other thread will ever
       * acquire.  This method is designed primarily for use in
       * monitoring of the system state.
       *
       * @return @c true if there may be other threads waiting to
       *         acquire the lock
       */
      virtual gbool hasQueueThreads() const;

      /**
       * Returns an estimate of the number of threads waiting to acquire.
       * The value is only an estimate because the number of threads may
       * change dynamically while this method traverses internal data
       * structures.  This method is designed for use in monitoring
       * system state, not for synchronization control.
       *
       * @return the estimated number of threads waiting for this lock
       */
      virtual gint getQueueLength() const;

      /**
       * Returns a string identifying this semaphore, as well as its state.
       * The state, in brackets, includes the String @c "Permits ="
       * followed by the number of permits.
       *
       * @return a string identifying this semaphore, as well as its state
       */
      String toString() const override;

      ~Semaphore() override;

    protected:
      /**
       * Shrinks the number of available permits by the indicated
       * reduction. This method can be useful in subclasses that use
       * semaphores to track resources that become unavailable. This
       * method differs from @c acquire in that it does not block
       * waiting for permits to become available.
       *
       * @param reduction the number of permits to remove
       * @throws IllegalArgumentException if @c reduction is negative
       */
      virtual void reducePermits(gint reduction);

      /**
       * Returns a collection containing threads that may be waiting to acquire.
       * Because the actual set of threads may change dynamically while
       * constructing this result, the returned collection is only a best-effort
       * estimate.  The elements of the returned collection are in no particular
       * order.  This method is designed to facilitate construction of
       * subclasses that provide more extensive monitoring facilities.
       *
       * @return the collection of threads
       */
      virtual util::ArrayList<Thread> getQueueThreads() const;
    };

    /**
     * Synchronization implementation for semaphore.  Uses QS state
     * to represent permits. Subclassed into fair and non-fair
     * versions.
     */
    class Semaphore::Synchronizer : public QueuedSynchronizer {
    public:
      CORE_EXPLICIT Synchronizer(gint permits);

      gint getPermits() const;

      gint nonFairTryAcquireShared(gint acquires);

      void reducePermits(gint reductions);

      gint drainPermits();

    protected:
      gbool tryReleaseShared(gint releases) override;
    };

    /**
     * NonFair version
     */
    class Semaphore::NonFairSync final : public Synchronizer {
    public:
      CORE_EXPLICIT NonFairSync(gint permits);

    protected:
      gint tryAcquireShared(gint acquires) override;
    };

    /**
     * Fair version
     */
    class Semaphore::FairSync final : public Synchronizer {
    public:
      CORE_EXPLICIT FairSync(gint permits);

    protected:
      gint tryAcquireShared(gint acquires) override;
    };
  } // concurrent
} // core

#endif //CORE24_SEMAPHORE_H
