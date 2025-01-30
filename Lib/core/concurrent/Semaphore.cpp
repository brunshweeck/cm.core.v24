//
// Created by bruns on 06/10/2024.
//

#include "Semaphore.h"

#include <core/util/ArrayList.h>

namespace core {
    using namespace util;
    using namespace time;

    static glong toNanos(glong time, Temporal::ChronoUnit unit) {
        glong nanos = 0;
        switch (unit) {
            case Temporal::NANOS: nanos = time;
            break;
            case Temporal::MICROS: nanos = time * 1000;
            break;
            case Temporal::MILLIS: nanos = time * 1000 * 1000;
            break;
            case Temporal::SECONDS: nanos = time * 1000 * 1000 * 1000;
            break;
            case Temporal::MINUTES: nanos = time * 1000 * 1000 * 1000 * 60;
            break;
            case Temporal::HOURS: nanos = time * 1000 * 1000 * 1000 * 60 * 60;
            break;
            case Temporal::HALF_DAYS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 12;
            break;
            case Temporal::DAYS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24;
            break;
            case Temporal::WEEKS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 7;
            break;
            case Temporal::MONTHS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 30;
            break;
            case Temporal::YEARS: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 365;
            break;
            case Temporal::DECADES: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 3652;
            break;
            case Temporal::CENTURIES: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 36525;
            break;
            case Temporal::MILLENNIA: nanos = time * 1000 * 1000 * 1000 * 60 * 60 * 24 * 365250;
            break;
            case Temporal::ERAS:
            case Temporal::FOREVER: nanos = Long::MAX_VALUE;
            default: nanos = 0;
        }
        if (nanos < 0)
            nanos = Long::MAX_VALUE;
        return nanos;
    }

    namespace concurrent {
        Semaphore::Semaphore(gint permits) {
            try {
                sync = new NonFairSync(permits);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        Semaphore::Semaphore(gint permits, gbool fair) {
            try {
                if (fair)
                    sync = new FairSync(permits);
                else
                    sync = new NonFairSync(permits);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void Semaphore::acquire() {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                sync->acquireSharedInterruptibly(1);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void Semaphore::acquireUninterruptibly() {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                sync->acquireShared(1);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gbool Semaphore::tryAcquire() {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->nonFairTryAcquireShared(1) >= 0;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gbool Semaphore::tryAcquire(glong timeout, Temporal::ChronoUnit unit) {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->tryAcquireSharedNanos(1, toNanos(timeout, unit));
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void Semaphore::release() {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                sync->releaseShared(1);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void Semaphore::acquire(gint permits) {
            if (permits < 0)
                IllegalArgumentException().throws($ftrace());

            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                sync->acquireSharedInterruptibly(permits);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void Semaphore::acquireUninterruptibly(gint permits) {
            if (permits < 0)
                IllegalArgumentException().throws($ftrace());

            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                sync->acquireShared(permits);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gbool Semaphore::tryAcquire(gint permits) {
            if (permits < 0)
                IllegalArgumentException().throws($ftrace());

            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->nonFairTryAcquireShared(permits) >= 0;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gbool Semaphore::tryAcquire(gint permits, glong timeout, Temporal::ChronoUnit unit) {
            if (permits < 0)
                IllegalArgumentException().throws($ftrace());

            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->tryAcquireSharedNanos(permits, toNanos(timeout, unit));
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void Semaphore::release(gint permits) {
            if (permits < 0)
                IllegalArgumentException().throws($ftrace());

            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                sync->releaseShared(permits);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Semaphore::availablePermits() const {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->getPermits();
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Semaphore::drainPermits() const {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->drainPermits();
            }catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool Semaphore::isFair() const {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            return CORE_DCAST(FairSync const *, sync) != null;
        }

        gbool Semaphore::hasQueueThreads() const {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->hasQueuedThreads();
            }catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint Semaphore::getQueueLength() const {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->queueLength();
            }catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String Semaphore::toString() const {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            return Object::toString() + "[Permits = " + sync->getPermits() + "]";
        }

        Semaphore::~Semaphore() {
            Sync s = sync;
            sync = null;
            delete s;
        }

        void Semaphore::reducePermits(gint reduction) {
            if (reduction < 0)
                IllegalArgumentException().throws($ftrace());

            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                sync->reducePermits(reduction);
            }catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ArrayList<Thread> Semaphore::getQueueThreads() const {
            CORE_ASSERT2(sync != null, "This Object has been already destoryed."_Sl);
            try {
                return sync->queuedThreads();
            }catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Semaphore::Synchronizer::Synchronizer(gint permits) {
            setState(permits);
        }

        gint Semaphore::Synchronizer::getPermits() const {
            return state();
        }

        gint Semaphore::Synchronizer::nonFairTryAcquireShared(gint acquires) {
            for (;;) {
                int available = state();
                int remaining = available - acquires;
                if (remaining < 0 || compareAndSetState(available, remaining))
                    return remaining;
            }
        }

        void Semaphore::Synchronizer::reducePermits(gint reductions) {
            for (;;) {
                int current = state();
                int next = current - reductions;
                if (next > current) // underflow
                    Error("Permit count underflow"_Sl).throws($ftrace());
                if (compareAndSetState(current, next))
                    return;
            }
        }

        gint Semaphore::Synchronizer::drainPermits() {
            for (;;) {
                int current = state();
                if (current == 0 || compareAndSetState(current, 0))
                    return current;
            }
        }

        gbool Semaphore::Synchronizer::tryReleaseShared(gint releases) {
            for (;;) {
                int current = state();
                int next = current + releases;
                if (next < current) // overflow
                    Error("Maximum permit count exceeded"_Sl).throws($ftrace());
                if (compareAndSetState(current, next))
                    return true;
            }
        }

        Semaphore::NonFairSync::NonFairSync(gint permits): Synchronizer(permits) {
        }

        gint Semaphore::NonFairSync::tryAcquireShared(gint acquires) {
            return nonFairTryAcquireShared(acquires);
        }

        Semaphore::FairSync::FairSync(gint permits): Synchronizer(permits) {
        }

        gint Semaphore::FairSync::tryAcquireShared(gint acquires) {
            for (;;) {
                if (hasQueuedPredecessors())
                    return -1;
                int available = state();
                int remaining = available - acquires;
                if (remaining < 0 || compareAndSetState(available, remaining))
                    return remaining;
            }
        }
    } // concurrent
} // core
