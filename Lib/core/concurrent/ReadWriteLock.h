//
// Created by bruns on 06/10/2024.
//

#ifndef CORE24_READWRITELOCK_H
#define CORE24_READWRITELOCK_H
#include <core/lang/Object.h>

namespace core {
  namespace concurrent {
    /**
     * A @c ReadWriteLock maintains a pair of associated
     * @b concurrent, one for read-only operations and one for writing.
     * The <em> read lock</em> may be held simultaneously
     * by multiple reader threads, so long as there are no writers.
     * The <em> write lock</em> is exclusive.
     *
     * <p>
     * All @c ReadWriteLock implementations must guarantee that
     * the memory synchronization effects of @c writeLock operations
     * (as specified in the @b Lock class) also hold with respect
     * to the associated @c readLock. That is, a thread successfully
     * acquiring the read lock will see all updates made upon previous
     * release of the write lock.
     * </p>
     * <p>
     * A read-write lock allows for a greater level of concurrency in
     * accessing shared data than that permitted by a mutual exclusion lock.
     * It exploits the fact that while only a single thread at a time (a
     * <em>writer</em> thread) can modify the shared data, in many cases any
     * number of threads can concurrently read the data (hence <em>reader</em>
     * threads).
     * In theory, the increase in concurrency permitted by the use of a read-write
     * lock will lead to performance improvements over the use of a mutual
     * exclusion lock. In practice this increase in concurrency will only be fully
     * realized on a multiprocessor, and then only if the access patterns for
     * the shared data are suitable.
     * </p>
     * <p>
     * Whether or not a read-write lock will improve performance over the use
     * of a mutual exclusion lock depends on the frequency that the data is
     * read compared to being modified, the duration of the read and write
     * operations, and the contention for the data - that is, the number of
     * threads that will try to read or write the data at the same time.
     * For example, a collection that is initially populated with data and
     * thereafter infrequently modified, while being frequently searched
     * (such as a directory of some kind) is an ideal candidate for the use of
     * a read-write lock. However, if updates become frequent then the data
     * spends most of its time being exclusively locked and there is little, if any
     * increase in concurrency. Further, if the read operations are too short
     * the overhead of the read-write lock implementation (which is inherently
     * more complex than a mutual exclusion lock) can dominate the execution
     * cost, particularly as many read-write lock implementations still serialize
     * all threads through a small section of code. Ultimately, only profiling
     * and measurement will establish whether the use of a read-write lock is
     * suitable for your application.
     * </p>
     * <p>
     * Although the basic operation of a read-write lock is straight-forward,
     * there are many policy decisions that an implementation must make, which
     * may affect the effectiveness of the read-write lock in a given application.
     * Examples of these policies include:
     *
     * @li Determining whether to grant the read lock or the write lock, when
     * both readers and writers are waiting, at the time that a writer releases
     * the write lock. Writer preference is common, as writes are expected to be
     * short and infrequent. Reader preference is less common as it can lead to
     * lengthy delays for a write if the readers are frequent and long-lived as
     * expected. Fair, or &quot;in-order&quot; implementations are also possible.
     *
     * @li Determining whether readers that request the read lock while a
     * reader is active and a writer is waiting, are granted the read lock.
     * Preference to the reader can delay the writer indefinitely, while
     * preference to the writer can reduce the potential for concurrency.
     *
     * @li Determining whether the concurrent are reentrant: can a thread with the
     * write lock reacquire it? Can it acquire a read lock while holding the
     * write lock? Is the read lock itself reentrant?
     *
     * @li Can the write lock be downgraded to a read lock without allowing
     * an intervening writer? Can a read lock be upgraded to a write lock,
     * in preference to other waiting readers or writers?
     * </p>
     * You should consider all of these things when evaluating the suitability
     * of a given implementation for your application.
     *
     * @see ReentrantReadWriteLock
     * @see Lock
     * @see ReentrantLock
     */
    class ReadWriteLock : public virtual Object {
    public:
      /**
       * Returns the lock used for reading.
       *
       * @return the lock used for reading
       */
      virtual Lock& readLock() const = 0;

      /**
       * Returns the lock used for writing.
       *
       * @return the lock used for writing
       */
      virtual Lock& writeLock() const = 0;
    };
  } // concurrent
} // core

#endif //CORE24_READWRITELOCK_H
