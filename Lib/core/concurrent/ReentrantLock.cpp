//
// Created by bruns on 01/10/2024.
//

#include "ReentrantLock.h"

#include <core/lang/IllegalStateException.h>
#include <core/lang/InterruptedException.h>
#include <core/time/TemporalUnit.h>
#include <core/util/ArrayList.h>

namespace core {
  using namespace time;
  using namespace util;

  static glong toNanos(glong time, TemporalUnit unit) {
    glong nanos = 0;
    switch (unit) {
      case TemporalUnit::NANOS: nanos = time;
        break;
      case TemporalUnit::MICROS: nanos = time * 1000;
        break;
      case TemporalUnit::MILLIS: nanos = time * 1000 * 1000;
        break;
      case TemporalUnit::SECONDS: nanos = time * 1000 * 1000 * 1000;
        break;
      case TemporalUnit::MINUTES: nanos = time * 1000 * 1000 * 1000 * 60;
        break;
      case TemporalUnit::HOURS: nanos = time * 1000 * 1000 * 1000 * 60 * 60;
        break;
      case TemporalUnit::HALF_DAYS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 12;
        break;
      case TemporalUnit::DAYS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24;
        break;
      case TemporalUnit::WEEKS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 7;
        break;
      case TemporalUnit::MONTHS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 30;
        break;
      case TemporalUnit::YEARS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 365;
        break;
      case TemporalUnit::DECADES: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 3652;
        break;
      case TemporalUnit::CENTURIES: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 36525;
        break;
      case TemporalUnit::MILLENNIA: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 365250;
        break;
      case TemporalUnit::ERAS:
      case TemporalUnit::FOREVER: nanos = Long::MAX_VALUE;
      default: nanos = 0;
    }
    if (nanos < 0)
      nanos = Long::MAX_VALUE;
    return nanos;
  }

  namespace concurrent {
    ReentrantLock::ReentrantLock() {
      try {
        sync = new NonFairSync();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ReentrantLock::ReentrantLock(gbool fair) {
      try {
        if (fair)
          sync = new FairSync();
        else
          sync = new NonFairSync();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ReentrantLock::lock() {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        sync->lock();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ReentrantLock::lockInterruptibly() {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        sync->lockInterruptibly();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::tryLock() {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->tryLock();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::tryLock(glong timeout, TemporalUnit unit) {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->tryLockNanos(toNanos(timeout, unit));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ReentrantLock::unlock() {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        sync->release(1);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Condition& ReentrantLock::newCondition() {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->newCondition();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint ReentrantLock::holdCount() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->holdCount();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::isHeldByCurrentThread() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->isHeldExclusively();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::isLocked() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->isLocked();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::isFair() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return CORE_DCAST(FairSync*, sync) != null;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::hasQueuedThreads() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->hasQueuedThreads();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::hasQueuedThreads(Thread const& thread) const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->isQueued(thread);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint ReentrantLock::getQueueLength() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->queueLength();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::hasWaiters(Condition const& condition) const {
      if (!Class<QueuedSynchronizer::ConditionObject>::hasInstance(condition))
        IllegalArgumentException("not owner"_S).throws($ftrace());

      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->hasWaiters(CORE_XCAST(QueuedSynchronizer::ConditionObject const, condition));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gint ReentrantLock::getWaitQueueLength(Condition const& condition) const {
      if (!Class<QueuedSynchronizer::ConditionObject>::hasInstance(condition))
        IllegalArgumentException("not owner"_S).throws($ftrace());

      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->waiterQueueLength(CORE_XCAST(QueuedSynchronizer::ConditionObject const, condition));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    String ReentrantLock::toString() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      Object& o = sync->getOwner();
      return Lock::toString() + (o == null
                                   ? "[Unlocked]"_S
                                   : "[Locked by thread "_S + CORE_XCAST(Thread, o).getName() + "]"_S);
    }

    ReentrantLock::~ReentrantLock() {
      Sync s = sync;
      sync = null;
      delete s;
    }

    Object& ReentrantLock::getOwner() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->getOwner();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ArrayList<Thread> ReentrantLock::getQueuedThreads() const {
      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->queuedThreads();
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ArrayList<Thread> ReentrantLock::getWaiters(Condition const& condition) const {
      if (!Class<QueuedSynchronizer::ConditionObject>::hasInstance(condition))
        IllegalArgumentException("not owner"_S).throws($ftrace());

      CORE_ASSERT2(sync != null, "This Object has been already destoryed."_S);
      try {
        return sync->waitingThreads(CORE_XCAST(QueuedSynchronizer::ConditionObject const, condition));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::Synchronizer::tryLock() {
      Thread& current = Thread::currentThread();
      gint c = state();
      if (c == 0) {
        if (compareAndSetState(0, 1)) {
          setExclusiveOwnerThread(current);
          return true;
        }
      } else if (exclusiveOwnerThread() == current) {
        if (++c < 0) // overflow
          Error("Maximum lock count exceeded"_S).throws($ftrace());
        setState(c);
        return true;
      }
      return false;
    }

    void ReentrantLock::Synchronizer::lock() {
      try {
        if (!initialTryLock())
          acquire(1);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ReentrantLock::Synchronizer::lockInterruptibly() {
      if (Thread::interrupted())
        InterruptedException().throws($ftrace());

      try {
        if (!initialTryLock())
          acquire(1);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::Synchronizer::tryLockNanos(glong nanos) {
      if (Thread::interrupted())
        InterruptedException().throws($ftrace());

      try {
        return initialTryLock() || tryAcquireNanos(1, nanos);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::Synchronizer::tryRelease(gint releases) {
      int c = state() - releases;
      if (exclusiveOwnerThread() != Thread::currentThread())
        IllegalStateException().throws($ftrace());

      try {
        gbool free = c == 0;
        if (free)
          setExclusiveOwnerThread(null);
        setState(c);
        return free;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    gbool ReentrantLock::Synchronizer::isHeldExclusively() const {
      // While we must in general read state before owner,
      // we don't need to do so to check if current thread is owner
      return exclusiveOwnerThread() == Thread::currentThread();
    }

    QueuedSynchronizer::ConditionObject& ReentrantLock::Synchronizer::newCondition() {
      try {
        return ReentrantLock::UNSAFE::newInstance<ConditionObject>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Object& ReentrantLock::Synchronizer::getOwner() const {
      return state() == 0 ? null : exclusiveOwnerThread();
    }

    gint ReentrantLock::Synchronizer::holdCount() const {
      return isHeldExclusively() ? state() : 0;
    }

    gbool ReentrantLock::Synchronizer::isLocked() const {
      return state() != 0;
    }

    gbool ReentrantLock::NonFairSync::initialTryLock() {
      Thread& current = Thread::currentThread();
      if (compareAndSetState(0, 1)) {
        // first attempt is unguarded
        setExclusiveOwnerThread(current);
        return true;
      }
      if (exclusiveOwnerThread() == current) {
        int c = state() + 1;
        if (c < 0) // overflow
          Error("Maximum lock count exceeded"_S).throws($ftrace());
        setState(c);
        return true;
      }
      return false;
    }

    gbool ReentrantLock::NonFairSync::tryAcquire(gint acquires) {
      if (state() == 0 && compareAndSetState(0, acquires)) {
        setExclusiveOwnerThread(Thread::currentThread());
        return true;
      }
      return false;
    }

    gbool ReentrantLock::FairSync::initialTryLock() {
      Thread& current = Thread::currentThread();
      int c = state();
      if (c == 0) {
        if (!hasQueuedThreads() && compareAndSetState(0, 1)) {
          setExclusiveOwnerThread(current);
          return true;
        }
      } else if (exclusiveOwnerThread() == current) {
        if (++c < 0) // overflow
          Error("Maximum lock count exceeded"_S).throws($ftrace());
        setState(c);
        return true;
      }
      return false;
    }

    gbool ReentrantLock::FairSync::tryAcquire(gint acquires) {
      if (state() == 0 && !hasQueuedPredecessors() &&
        compareAndSetState(0, acquires)) {
        setExclusiveOwnerThread(Thread::currentThread());
        return true;
      }
      return false;
    }
  } // concurrent
} // core
