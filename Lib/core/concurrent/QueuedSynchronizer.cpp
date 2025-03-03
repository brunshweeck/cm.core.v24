//
// Created by bruns on 30/09/2024.
//

#include "QueuedSynchronizer.h"

#include <core/concurrent/LockSupport.h>
#include <core/concurrent/RejectedExecutionException.h>
#include <core/lang/InterruptedException.h>
#include <core/lang/OutOfMemoryError.h>
#include <core/time/Chrono.h>
#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>
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
    gint QueuedSynchronizer::state() const {
      return state_;
    }

    void QueuedSynchronizer::setState(gint newState) {
      state_ = newState;
    }

    template <class T, ClassOf(1)::OnlyIf<Class<T>::isPrimitive()>  = 1>
    static glong addressOf(T const& o) { return CORE_CAST(glong, &o); }

    static glong addressOf(Object const& o) { return o == null ? 0LL : CORE_CAST(glong, &o); }

    template <class T>
    static glong ptr2Address(T ptr) { return CORE_CAST(glong, ptr); }

    template <class T>
    static Object& ptr2Object(T& ptr) {
      if (ptr2Address(ptr) == 0LL)
        return null;

      return *ptr;
    }

    template <class T>
    static Object const& ptr2Object(T const& ptr) {
      if (ptr2Address(ptr) == 0LL)
        return null;

      return *ptr;
    }

    template <class T>
    static T const* object2Ptr(Object const& o) {
      if (o == null)
        return { };

      return CORE_DCAST(T const*, &o);
    }

    template <class T>
    static T* object2Ptr(Object& o) {
      if (o == null)
        return null;

      return CORE_DCAST(T*, &o);
    }

    gbool QueuedSynchronizer::compareAndSetState(gint expected, gint update) {
      return UNSAFE::compareAndSetInt(null, addressOf(state_), expected, update);
    }

    gbool QueuedSynchronizer::tryAcquire(gint arg) {
      UnsupportedOperationException().throws($ftrace());
    }

    gbool QueuedSynchronizer::tryRelease(gint arg) {
      UnsupportedOperationException().throws($ftrace());
    }

    gint QueuedSynchronizer::tryAcquireShared(gint arg) {
      UnsupportedOperationException().throws($ftrace());
    }

    gbool QueuedSynchronizer::tryReleaseShared(gint arg) {
      UnsupportedOperationException().throws($ftrace());
    }

    gbool QueuedSynchronizer::isHeldExclusively() const {
      UnsupportedOperationException().throws($ftrace());
    }

    void QueuedSynchronizer::acquire(gint arg) {
      if (!tryAcquire(arg))
        acquire(null, arg, false, false, false, 0L);
    }

    void QueuedSynchronizer::acquireInterruptibly(gint arg) {
      if (Thread::interrupted() ||
        (!tryAcquire(arg) && acquire(null, arg, false, true, false, 0L) < 0))
        InterruptedException().throws($ftrace());
    }

    gbool QueuedSynchronizer::tryAcquireNanos(gint arg, glong nanosTimeout) {
      if (!Thread::interrupted()) {
        if (tryAcquire(arg))
          return true;
        if (nanosTimeout <= 0L)
          return false;
        int stat = acquire(null, arg, false, true, true, nanosTimeout);
        if (stat > 0)
          return true;
        if (stat == 0)
          return false;
      }
      InterruptedException().throws($ftrace());
    }

    gbool QueuedSynchronizer::release(gint arg) {
      if (tryRelease(arg)) {
        signalNext(head);
        return true;
      }
      return false;
    }

    void QueuedSynchronizer::acquireShared(gint arg) {
      if (tryAcquireShared(arg) < 0)
        acquire(null, arg, true, false, false, 0L);
    }

    void QueuedSynchronizer::acquireSharedInterruptibly(gint arg) {
      if (Thread::interrupted() || (tryAcquireShared(arg) < 0 &&
        acquire(null, arg, true, true, false, 0L) < 0))
        InterruptedException().throws($ftrace());
    }

    gbool QueuedSynchronizer::tryAcquireSharedNanos(gint arg, glong nanosTimeout) {
      if (!Thread::interrupted()) {
        if (tryAcquireShared(arg) >= 0)
          return true;
        if (nanosTimeout <= 0L)
          return false;
        int stat = acquire(null, arg, true, true, true, nanosTimeout);
        if (stat > 0)
          return true;
        if (stat == 0)
          return false;
      }
      InterruptedException().throws($ftrace());
    }

    gbool QueuedSynchronizer::releaseShared(gint arg) {
      if (tryReleaseShared(arg)) {
        signalNext(head);
        return true;
      }
      return false;
    }

    gbool QueuedSynchronizer::hasQueuedThreads() const {
      for (NODE p = tail, h = head; p != h && p; p = p->prev)
        if (p->status >= 0)
          return true;

      return false;
    }

    gbool QueuedSynchronizer::hasContended() const {
      return head;
    }

    Object& QueuedSynchronizer::firstQueuedThread() {
      THREAD first = null, w;
      NODE h, s;
      if (((h = head)) && (!((s = h->next)) || !((first = s->waiter)) || !s->prev)) {
        // traverse from tail on stale reads
        for (NODE p = tail, q; p && ((q = p->prev)); p = q)
          if ((w = p->waiter))
            first = w;
      }
      return ptr2Object(first);
    }

    gbool QueuedSynchronizer::isQueued(Thread const& thread) {
      for (NODE p = tail; p; p = p->prev)
        if (p->waiter == &thread)
          return true;

      return false;
    }

    gbool QueuedSynchronizer::apparentlyFirstQueuedIsExclusive() {
      NODE h = { }, s = { };
      return ((h = head)) && ((s = h->next)) && !CORE_DCAST(SHARED_NODE, s) && s->waiter;
    }

    gbool QueuedSynchronizer::hasQueuedPredecessors() {
      THREAD first = { };
      NODE h, s;
      if (((h = head)) && (!((s = h->next)) || !((first = s->waiter)) || !s->prev))
        first = object2Ptr<Thread>(firstQueuedThread()); // retry via firstQueuedThread
      return first && first != &Thread::currentThread();
    }

    gint QueuedSynchronizer::queueLength() {
      int n = 0;
      for (NODE p = tail; p; p = p->prev) {
        if (p->waiter)
          ++n;
      }
      return n;
    }

    ArrayList<Thread> QueuedSynchronizer::queuedThreads() const {
      ArrayList<Thread> list = { };
      for (NODE p = tail; p; p = p->prev) {
        THREAD t = p->waiter;
        if (t)
          list.add(*t);
      }
      return list;
    }

    ArrayList<Thread> QueuedSynchronizer::exclusiveQueuedThreads() const {
      ArrayList<Thread> list = { };
      for (NODE p = tail; p; p = p->prev) {
        if (!CORE_DCAST(SHARED_NODE, p)) {
          THREAD t = p->waiter;
          if (t)
            list.add(*t);
        }
      }
      return list;
    }

    ArrayList<Thread> QueuedSynchronizer::sharedQueuedThreads() const {
      ArrayList<Thread> list = { };
      for (NODE p = tail; p; p = p->prev) {
        if (CORE_DCAST(SHARED_NODE, p)) {
          THREAD t = p->waiter;
          if (t)
            list.add(*t);
        }
      }
      return list;
    }

    String QueuedSynchronizer::toString() const {
      return OwnableSynchronizer::toString()
          + "[State = " + state() + ", "
          + (hasQueuedThreads() ? "non" : "") + "empty queue]";
    }

    void QueuedSynchronizer::ConditionObject::notify() {
      COND_NODE first = firstWaiter;
      if (!owner.isHeldExclusively())
        IllegalStateException().throws($ftrace());
      else if (first)
        doSignal(first, false);
    }

    void QueuedSynchronizer::ConditionObject::notifyAll() {
      COND_NODE first = firstWaiter;
      if (!owner.isHeldExclusively())
        IllegalStateException().throws($ftrace());
      else if (first)
        doSignal(first, true);
    }

    void QueuedSynchronizer::ConditionObject::awaitUninterruptibly() {
      COND_NODE node = newConditionNode();
      if (!node)
        return;
      int savedState = enableWait(node);
      LockSupport::setCurrentBlocker(*this); // for back-compatibility
      gbool interrupted = false, rejected = false;
      while (!canReacquire(node)) {
        if (Thread::interrupted())
          interrupted = true;
        else if ((node->status & COND) != 0) {
          try {
            if (rejected)
              node->block();
            else {
              /** ManagedBlock for external threads */
              do {} while (!node->isReleasable() && !node->block());
            }
          } catch (RejectedExecutionException const&) {
            rejected = true;
          } catch (InterruptedException const&) {
            interrupted = true;
          }
        } else
          Thread::onSpinWait(); // awoke while enqueuing
      }
      LockSupport::setCurrentBlocker(null);
      node->clearStatus();
      owner.acquire(node, savedState, false, false, false, 0L);
      if (interrupted)
        Thread::currentThread().interrupt();
    }

    void QueuedSynchronizer::ConditionObject::await() {
      if (Thread::interrupted())
        InterruptedException().throws($ftrace());
      COND_NODE node = newConditionNode();
      if (!node)
        return;
      int savedState = enableWait(node);
      LockSupport::setCurrentBlocker(*this); // for back-compatibility
      gbool interrupted = false, cancelled = false, rejected = false;
      while (!canReacquire(node)) {
        if (interrupted |= Thread::interrupted()) {
          if ((cancelled = (node->getAndUnsetStatus(COND) & COND) != 0))
            break; // else interrupted after signal
        } else if ((node->status & COND) != 0) {
          try {
            if (rejected)
              node->block();
            else {
              /** ManagedBlock for external threads */
              do {} while (!node->isReleasable() && !node->block());
            }
          } catch (RejectedExecutionException const&) {
            rejected = true;
          } catch (InterruptedException const&) {
            interrupted = true;
          }
        } else
          Thread::onSpinWait(); // awoke while enqueuing
      }
      LockSupport::setCurrentBlocker(null);
      node->clearStatus();
      owner.acquire(node, savedState, false, false, false, 0L);
      if (interrupted) {
        if (cancelled) {
          unlinkCancelledWaiters(node);
          InterruptedException().throws($ftrace());
        }
        Thread::currentThread().interrupt();
      }
    }

    glong QueuedSynchronizer::ConditionObject::awaitNanos(glong nanosTimeout) {
      if (Thread::interrupted())
        InterruptedException().throws($ftrace());
      COND_NODE node = newConditionNode();
      if (!node)
        return nanosTimeout - OOME_COND_WAIT_DELAY;
      int savedState = enableWait(node);
      glong nanos = (nanosTimeout < 0L) ? 0L : nanosTimeout;
      gbool cancelled = false, interrupted = false;
      Chronometer chrono = Chronometer();
      chrono.start();
      while (!canReacquire(node)) {
        if ((interrupted |= Thread::interrupted()) ||
          (nanos = nanos - chrono.elapsedTime(TemporalUnit::NANOS)) <= 0L) {
          if ((cancelled = (node->getAndUnsetStatus(COND) & COND) != 0))
            break;
        } else
          LockSupport::parkNanos(*this, nanos);
      }
      node->clearStatus();
      owner.acquire(node, savedState, false, false, false, 0L);
      if (cancelled) {
        unlinkCancelledWaiters(node);
        if (interrupted)
          InterruptedException().throws($ftrace());
      } else if (interrupted)
        Thread::currentThread().interrupt();
      glong remaining = nanos - chrono.elapsedTime(TemporalUnit::NANOS); // avoid overflow
      return (remaining <= nanosTimeout) ? remaining : Long::MIN_VALUE;
    }

    gbool QueuedSynchronizer::ConditionObject::awaitUntil(const LocalDateTime& deadline) {
      glong abstime = deadline.toEpochSecond(ZoneOffset::UTC) * 1000 + deadline.toLocalTime().nano() / 1000000;
      if (Thread::interrupted())
        InterruptedException().throws($ftrace());
      COND_NODE node = newConditionNode();
      if (!node)
        return false;
      int savedState = enableWait(node);
      gbool cancelled = false, interrupted = false;
      Chronometer chrono = Chronometer();
      chrono.start();
      while (!canReacquire(node)) {
        LocalDateTime dt = LocalDateTime::now();
        glong current = dt.toEpochSecond(ZoneOffset::UTC) * 1000 + dt.toLocalTime().nano() / 1000000;
        if ((interrupted |= Thread::interrupted()) || current >= abstime) {
          if ((cancelled = (node->getAndUnsetStatus(COND) & COND) != 0))
            break;
        } else
          LockSupport::parkUntil(*this, abstime);
      }
      node->clearStatus();
      owner.acquire(node, savedState, false, false, false, 0L);
      if (cancelled) {
        unlinkCancelledWaiters(node);
        if (interrupted)
          InterruptedException().throws($ftrace());
      } else if (interrupted)
        Thread::currentThread().interrupt();
      return !cancelled;
    }

    gbool QueuedSynchronizer::ConditionObject::await(glong time, TemporalUnit unit) {
      if (Thread::interrupted())
        InterruptedException().throws($ftrace());
      COND_NODE node = newConditionNode();
      if (!node)
        return false;
      int savedState = enableWait(node);
      if (time < 0) time = 0LL;
      gbool cancelled = false, interrupted = false;
      glong nanos = toNanos(time, unit);
      Chronometer chrono = Chronometer();
      chrono.start();
      while (!canReacquire(node)) {
        if ((interrupted |= Thread::interrupted()) ||
          (time = time - chrono.elapsedTime(unit)) <= 0L) {
          if ((cancelled = (node->getAndUnsetStatus(COND) & COND) != 0))
            break;
        } else
          LockSupport::parkNanos(*this, nanos);
      }
      node->clearStatus();
      owner.acquire(node, savedState, false, false, false, 0L);
      if (cancelled) {
        unlinkCancelledWaiters(node);
        if (interrupted)
          InterruptedException().throws($ftrace());
      } else if (interrupted)
        Thread::currentThread().interrupt();
      return !cancelled;
    }

    void QueuedSynchronizer::ConditionObject::doSignal(COND_NODE first, gbool all) {
      while (first) {
        COND_NODE next = first->nextWaiter;
        if (!((firstWaiter = next)))
          lastWaiter = null;
        if ((first->getAndUnsetStatus(COND) & COND) != 0) {
          owner.enqueue(first);
          if (!all)
            break;
        }
        first = next;
      }
    }

    gint QueuedSynchronizer::ConditionObject::enableWait(COND_NODE node) {
      if (owner.isHeldExclusively()) {
        node->waiter = &Thread::currentThread();
        node->setStatusRelaxed(COND | WAITING);
        COND_NODE last = lastWaiter;
        if (!last)
          firstWaiter = node;
        else
          last->nextWaiter = node;
        lastWaiter = node;
        int savedState = owner.state();
        if (owner.release(savedState))
          return savedState;
      }
      node->status = CANCELLED; // lock not held or inconsistent
      IllegalStateException().throws($ftrace());
    }

    gbool QueuedSynchronizer::ConditionObject::canReacquire(COND_NODE node) const {
      // check links, not status to avoid enqueue race
      NODE p; // traverse unless known to be bidirectionally linked
      return node && ((p = node->prev)) &&
          (p->next == node || owner.isEnqueued(node));
    }

    void QueuedSynchronizer::ConditionObject::unlinkCancelledWaiters(COND_NODE node) {
      if (!node || node->nextWaiter || node == lastWaiter) {
        COND_NODE w = firstWaiter, trail = null;
        while (w) {
          COND_NODE next = w->nextWaiter;
          if ((w->status & COND) == 0) {
            w->nextWaiter = null;
            if (!trail)
              firstWaiter = next;
            else
              trail->nextWaiter = next;
            if (!next)
              lastWaiter = trail;
          } else
            trail = w;
          w = next;
        }
      }
    }

    QueuedSynchronizer::COND_NODE QueuedSynchronizer::ConditionObject::newConditionNode() {
      int savedState = 0;
      if (owner.tryInitializeHead()) {
        try { return new ConditionNode(); } catch (OutOfMemoryError const&) { $() }
      }
      // fall through if encountered OutOfMemoryError
      if (!owner.isHeldExclusively() || !owner.release(savedState = owner.state()))
        IllegalStateException().throws($ftrace());
      UNSAFE::park(false, OOME_COND_WAIT_DELAY);
      owner.acquireOnOutOfMemoryException(false, savedState);
      return null;
    }

    gbool QueuedSynchronizer::ConditionObject::isOwnedBy(QueuedSynchronizer const& sync) const {
      return owner == sync;
    }

    gbool QueuedSynchronizer::ConditionObject::hasWaiters() const {
      if (!owner.isHeldExclusively())
        IllegalStateException().throws($ftrace());
      for (COND_NODE w = firstWaiter; w; w = w->nextWaiter) {
        if ((w->status & COND) != 0)
          return true;
      }
      return false;
    }

    gint QueuedSynchronizer::ConditionObject::waitQueueLength() const {
      if (!owner.isHeldExclusively())
        IllegalStateException().throws($ftrace());
      int n = 0;
      for (COND_NODE w = firstWaiter; w; w = w->nextWaiter) {
        if ((w->status & COND) != 0)
          ++n;
      }
      return n;
    }

    ArrayList<Thread> QueuedSynchronizer::ConditionObject::waitingThreads() const {
      if (!owner.isHeldExclusively())
        IllegalStateException().throws($ftrace());
      ArrayList<Thread> list;
      for (COND_NODE w = firstWaiter; w; w = w->nextWaiter) {
        if ((w->status & COND) != 0) {
          THREAD t = w->waiter;
          if (t)
            list.add(*t);
        }
      }
      return list;
    }

    gbool QueuedSynchronizer::owns(ConditionObject const& condition) const {
      return condition.isOwnedBy(*this);
    }

    gbool QueuedSynchronizer::hasWaiters(ConditionObject const& condition) const {
      if (!owns(condition))
        IllegalArgumentException("Not owner"_S).throws($ftrace());
      return condition.hasWaiters();
    }

    gint QueuedSynchronizer::waiterQueueLength(ConditionObject const& condition) const {
      if (!owns(condition))
        IllegalArgumentException("Not owner"_S).throws($ftrace());
      return condition.waitQueueLength();
    }

    ArrayList<Thread> QueuedSynchronizer::waitingThreads(ConditionObject const& condition) const {
      if (!owns(condition))
        IllegalArgumentException("Not owner"_S).throws($ftrace());
      return condition.waitingThreads();
    }

    gbool QueuedSynchronizer::Node::compareAndSetPrev(NODE c, NODE v) {
      // for cleanQueue
      return UNSAFE::compareAndSetReference(null, ptr2Address(&prev), ptr2Object(c), ptr2Object(v));
    }

    gbool QueuedSynchronizer::Node::compareAndSetNext(NODE c, NODE v) {
      // for cleanQueue
      return UNSAFE::compareAndSetReference(null, ptr2Address(&next), ptr2Object(c), ptr2Object(v));
    }

    gint QueuedSynchronizer::Node::getAndUnsetStatus(gint v) {
      // for signalling
      return UNSAFE::getAndBitwiseAndInt(null, ptr2Address(&status), ~v);
    }

    void QueuedSynchronizer::Node::setPrevRelaxed(NODE p) {
      // for off-queue assignment
      UNSAFE::putReference(null, ptr2Address(&prev), ptr2Object(p));
    }

    void QueuedSynchronizer::Node::setStatusRelaxed(gint s) {
      // for off-queue assignment
      UNSAFE::putInt(null, ptr2Address(&status), s);
    }

    void QueuedSynchronizer::Node::clearStatus() {
      // for reducing unneeded signals
      UNSAFE::putIntOpaque(null, ptr2Address(&status), 0);
    }

    gbool QueuedSynchronizer::ConditionNode::isReleasable() {
      return status <= 1 || Thread::currentThread().isInterrupted();
    }

    gbool QueuedSynchronizer::ConditionNode::block() {
      while (!isReleasable())
        LockSupport::park();
      return true;
    }

    gbool QueuedSynchronizer::compareAndSetTail(NODE c, NODE v) {
      return UNSAFE::compareAndSetReference(null, ptr2Address(&tail), ptr2Object(c), ptr2Object(v));
    }

    QueuedSynchronizer::NODE QueuedSynchronizer::tryInitializeHead() {
      for (NODE h = null, t;;) {
        if ((t = tail))
          return t;

        if (head)
          Thread::onSpinWait();
        else {
          if (!h) {
            try {
              h = new ExclusiveNode();
            } catch (OutOfMemoryError const&) { return { }; }
          }

          if (UNSAFE::compareAndSetReference(null, ptr2Address(&head), null, ptr2Object(h)))
            return tail = h;
        }
      }
    }

    void QueuedSynchronizer::enqueue(COND_NODE node) {
      if (node) {
        gbool unpark = false;
        for (NODE t;;) {
          if (!((t = tail)) && !((t = tryInitializeHead()))) {
            unpark = true;
            break;
          }

          node->setPrevRelaxed(t);
          if (compareAndSetTail(t, node)) {
            t->next = node;
            if (t->status < 0)
              unpark = true;
            break;
          }
        }
        if (unpark)
          UNSAFE::unpark(ptr2Object(node->waiter));
      }
    }

    gbool QueuedSynchronizer::isEnqueued(NODE node) const {
      for (NODE t = tail; t; t = t->prev)
        if (t == node)
          return true;

      return false;
    }

    void QueuedSynchronizer::signalNext(NODE h) {
      NODE s = { };
      if (h && ((s = h->next)) && s->status != 0) {
        s->getAndUnsetStatus(WAITING);
        UNSAFE::unpark(ptr2Object(s->waiter));
      }
    }

    void QueuedSynchronizer::signalNextIfShared(NODE h) {
      NODE s = { };
      if (h && ((s = h->next)) && CORE_DCAST(SHARED_NODE, s) && s->status != 0) {
        s->getAndUnsetStatus(WAITING);
        UNSAFE::unpark(ptr2Object(s->waiter));
      }
    }

    gint QueuedSynchronizer::acquire(NODE node, gint arg, gbool shared,
                                     gbool interruptible, gbool timed, glong time) {
      Thread& current = Thread::currentThread();
      gbyte spins = 0, postSpins = 0; // retries upon unpark of first thread
      gbool interrupted = false, first = false;
      NODE pred = null; // predecessor of node when enqueued
      Chronometer chrono;

      /*
       * Repeatedly:
       *  Check if node now first
       *    if so, ensure head stable, else ensure valid predecessor
       *  if node is first or not yet enqueued, try acquiring
       *  else if queue is not initialized, do so by attaching new header node
       *     resort to spinwait on OOME trying to create node
       *  else if node not yet created, create it
       *     resort to spinwait on OOME trying to create node
       *  else if not yet enqueued, try once to enqueue
       *  else if woken from park, retry (up to postSpins times)
       *  else if WAITING status not set, set and retry
       *  else park and clear WAITING status, and check cancellation
       */

      chrono.start();
      for (;;) {
        if (!first && ((pred = (!node) ? null : node->prev)) && !((first = (head == pred)))) {
          if (pred->status < 0) {
            cleanQueue(); // predecessor cancelled
            continue;
          }
          if (!pred->prev) {
            Thread::onSpinWait(); // ensure serialization
            continue;
          }
        }
        if (first || !pred) {
          gbool acquired;
          try {
            if (shared)
              acquired = (tryAcquireShared(arg) >= 0);
            else
              acquired = tryAcquire(arg);
          } catch (Throwable const& ex) {
            cancelAcquire(node, interrupted, false);
            ex.throws($ftrace());
          }
          if (acquired) {
            if (first) {
              node->prev = null;
              head = node;
              pred->next = null;
              node->waiter = null;
              if (shared)
                signalNextIfShared(node);
              if (interrupted)
                current.interrupt();
            }
            return 1;
          }
        }
        NODE t = null;
        if (!((t = tail))) {
          // initialize queue
          if (!tryInitializeHead())
            return acquireOnOutOfMemoryException(shared, arg);
        } else if (!node) {
          // allocate; retry before enqueue
          try {
            node = (shared)
                     ? CORE_DCAST(NODE, new SharedNode())
                     : CORE_DCAST(NODE, new ExclusiveNode());
          } catch (OutOfMemoryError const&) {
            return acquireOnOutOfMemoryException(shared, arg);
          }
        } else if (!pred) {
          // try to enqueue
          node->waiter = &current;
          node->setPrevRelaxed(t); // avoid unnecessary fence
          if (!compareAndSetTail(t, node))
            node->setPrevRelaxed(null); // back out
          else
            t->next = node;
        } else if (first && spins != 0) {
          --spins; // reduce unfairness on rewaits
          Thread::onSpinWait();
        } else if (node->status == 0) {
          node->status = WAITING; // enable signal and recheck
        } else {
          glong nanos;
          spins = postSpins = CORE_CAST(gbyte, (postSpins << 1) | 1);
          if (!timed)
            LockSupport::park(*this);
          else if ((nanos = time - chrono.elapsedTime(TemporalUnit::NANOS)) > 0L)
            LockSupport::parkNanos(*this, nanos);
          else
            break;
          node->clearStatus();
          if ((interrupted |= Thread::interrupted()) && interruptible)
            break;
        }
      }
      return cancelAcquire(node, interrupted, interruptible);
    }

    gint QueuedSynchronizer::acquireOnOutOfMemoryException(gbool shared, gint arg) {
      for (glong nanos = 1L;;) {
        if (shared ? (tryAcquireShared(arg) >= 0) : tryAcquire(arg))
          return 1;
        UNSAFE::park(false, nanos); // must use Unsafe park to sleep
        if (nanos < 1L << 30) // max about 1 second
          nanos <<= 1;
      }
    }

    void QueuedSynchronizer::cleanQueue() {
      for (;;) {
        // restart point
        for (NODE q = tail, s = null, p, n;;) {
          // (p, q, s) triples
          if (!q || !((p = q->prev)))
            return; // end of list
          if (!s ? tail != q : (s->prev != q || s->status < 0))
            break; // inconsistent
          if (q->status < 0) {
            // cancelled
            if ((!s ? compareAndSetTail(q, p) : s->compareAndSetPrev(q, p)) &&
              q->prev == p) {
              p->compareAndSetNext(q, s); // OK if fails
              if (!p->prev)
                signalNext(p);
            }
            break;
          }
          if ((n = p->next) != q) {
            // help finish
            if (n && q->prev == p) {
              p->compareAndSetNext(n, q);
              if (!p->prev)
                signalNext(p);
            }
            break;
          }
          s = q;
          q = q->prev;
        }
      }
    }

    gint QueuedSynchronizer::cancelAcquire(NODE node, gbool interrupted, gbool interruptible) {
      if (node) {
        node->waiter = null;
        node->status = CANCELLED;
        if (node->prev)
          cleanQueue();
      }
      if (interrupted) {
        if (interruptible)
          return CANCELLED;

        Thread::currentThread().interrupt();
      }
      return 0;
    }
  } // concurrent
} // core
