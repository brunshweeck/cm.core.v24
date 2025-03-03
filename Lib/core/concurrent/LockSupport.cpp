//
// Created by bruns on 01/10/2024.
//

#include "LockSupport.h"

#include <core/lang/Thread.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace concurrent {
    void LockSupport::setBlocker(Thread& t, Object& arg) {
      glong const PARKBLOCKER = CORE_CAST(glong, &t.blocker);
      UNSAFE::putReferenceOpaque(null, PARKBLOCKER, arg);
    }

    void LockSupport::setCurrentBlocker(Object& blocker) {
      Thread& t = Thread::currentThread();
      glong const PARKBLOCKER = CORE_CAST(glong, &t.blocker);
      UNSAFE::putReferenceOpaque(null, PARKBLOCKER, blocker);
    }

    void LockSupport::unpark(Thread& thread) {
      UNSAFE::unpark(thread);
    }

    void LockSupport::park(Object& blocker) {
      Thread& t = Thread::currentThread();
      setBlocker(t, blocker);
      try {
        UNSAFE::park(false, 0L);
        setBlocker(t, null);
      } catch (Throwable const& ex) {
        setBlocker(t, null);
        ex.throws($ftrace());
      }
    }

    void LockSupport::parkNanos(Object& blocker, glong nanos) {
      Thread& t = Thread::currentThread();
      setBlocker(t, blocker);
      try {
        UNSAFE::park(false, nanos);
        setBlocker(t, null);
      } catch (Throwable const& ex) {
        setBlocker(t, null);
        ex.throws($ftrace());
      }
    }

    void LockSupport::parkUntil(Object& blocker, glong deadline) {
      Thread& t = Thread::currentThread();
      setBlocker(t, blocker);
      try {
        UNSAFE::park(true, deadline);
        setBlocker(t, null);
      } catch (Throwable const& ex) {
        setBlocker(t, null);
        ex.throws($ftrace());
      }
    }

    Object& LockSupport::getCurrentBlocker(Thread const& t) {
      glong const PARKBLOCKER = CORE_CAST(glong, &t.blocker);
      return UNSAFE::getReferenceOpaque(null, PARKBLOCKER);
    }

    void LockSupport::park() {
      UNSAFE::park(false, 0L);
    }

    void LockSupport::parkNanos(glong nanos) {
      UNSAFE::park(false, nanos);
    }

    void LockSupport::parkUntil(glong deadline) {
      UNSAFE::park(true, deadline);
    }

    glong LockSupport::getThreadID(Thread const& t) {
      return t.threadId();
    }
  } // concurrent
} // core
