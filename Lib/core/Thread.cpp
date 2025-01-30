//
// Created by bruns on 20/09/2024.
//

#include <core/IllegalThreadStateException.h>
#include <core/Thread.h>
#include <core/ThreadSupport.h>
#include <core/concurrent/ReentrantLock.h>
#include <core/misc/Event.h>
#include <core/time/Chrono.h>
#include <core/time/Duration.h>
#include <core/util/ArrayList.h>
#include <core/util/HashSet.h>
#include <meta/io/Interruptible.h>

// #line CORE_LINE "Thread.c24"

namespace core {

    CORE_ALIAS(UNSAFE, misc::Unsafe);
    static gint volatile threadID = -1;

    static String GenerateThreadName() {
        // static gint volatile threadID = -1;

        gint tid = ++threadID;
        while (tid <= 0)
            tid = ++threadID;

        // if (tid == 1)
        //     return "Main"_S;

        return "Thread-"_S + tid;
    }

    Thread::Holder::Holder(Object& task, glong stackSize, gint priority, gbool daemon)
        : task(), stackSize(stackSize), priority(priority), daemon(daemon) {
        if (task != null) {
            CORE_ASSERT(Class<Runnable>::hasInstance(task));
            Holder::task = CORE_DCAST(TASK, &task);
        }
    }

    Thread::Thread(Thread const& thread) : Thread() {
        UnsupportedOperationException().throws($ftrace());
    }

    Thread::Thread()
        : eetop(0L),
          holder(null, 0L, NORM_PRIORITY, false),
          blocker(),
          event(),
          rwBlocker(),
          monitor() {}

    namespace {
        class ThreadList final : public Object {
        public:
            gint volatile mutable status = 0;
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            void tryLock() const {
                misc::Event event = { };
                while (!UNSAFE::weakCompareAndSetInt(null, CORE_CAST(glong, &status), 0, 1)) {
                    event.waitFor(5);
                }
            }

            void unlock() const {
                misc::Event event = { };
                while (!UNSAFE::weakCompareAndSetInt(null, CORE_CAST(glong, &status), 1, 0)) {
                    event.waitFor(5);
                }
            }

            util::ArrayList<Thread> threads;
            CORE_IMPLICIT ThreadList() = default;

            gbool add(Thread& thread) {
                tryLock();
                try {
                    gbool r = threads.add(thread);
                    unlock();
                    return r;
                } catch (Throwable const& _) {
                    tryLock();
                    return false;
                }
            }

            gbool contains(glong tid) {
                try {
                    for (int i = 0; i < threads.size(); ++i) {
                        Thread const& thread = threads.get(i);
                        if (thread.threadId() == tid)
                            return true;
                    }
                } catch (Throwable const& _) {}
                return false;
            }

            gbool isEmpty() const {
                return threads.isEmpty();
            }

            gint indexOf(glong tid) {
                try {
                    for (int i = 0; i < threads.size(); ++i) {
                        Thread const& thread = threads.get(i);
                        if (thread.threadId() == tid) {
                            return i;
                        }
                    }
                } catch (Throwable const& _) {}
                return -1;
            }
        };
    }

    static ThreadList& THREAD_LIST = UNSAFE::newInstance<ThreadList>();

    Thread& Thread::currentThread() {
        glong threadID = ThreadSupport::CurrentThread();

        ThreadList& threads = THREAD_LIST;
        if (UNSAFE::threadCount() == 0 && threads.isEmpty()) {
            Thread& thread = *new Thread();
            UNSAFE::storeReference(thread);
            threads.add(thread);
            // Initialize Field Holder
            thread.holder.priority = ThreadSupport::GetThreadPriority(threadID);
            thread.holder.daemon = false;
            thread.holder.status = CORE_CAST(gint, State::RUNNABLE);
            thread.holder.task = null;
            // Initialize thread
            CORE_CAST(glong&, thread.tid) = threadID;
            thread.name = ThreadSupport::GetThreadName(threadID);
            if (thread.name.isEmpty()) {
                thread.name = GenerateThreadName();
                ThreadSupport::SetThreadName(thread.tid, thread.name);
            } else {
                CORE_IGNORE(GenerateThreadName());
            }
            thread.status = false;
            return thread;
        }
        if (threads.contains(threadID)) {
            gint i = threads.indexOf(threadID);
            return threads.threads.get(i);
        }
        Thread& thread = *new Thread();
        UNSAFE::storeReference(thread);
        threads.add(thread);
        // Initialize Field Holder
        thread.holder.priority = ThreadSupport::GetThreadPriority(threadID);
        thread.holder.daemon = false;
        thread.holder.status = CORE_CAST(gint, State::RUNNABLE);
        thread.holder.task = null;
        // Initialize thread
        CORE_CAST(glong&, thread.tid) = threadID;
        thread.name = ThreadSupport::GetThreadName(threadID);
        thread.status = false;
        thread.monitor = new concurrent::ReentrantLock();
        return thread;
    }

    void Thread::yield() {
        ThreadSupport::YieldCurrentThread();
    }

    void Thread::sleep(glong millis) {
        Thread& t = currentThread();
        t.monitor->lock();
        CORE_ASSERT(t.threadId() == ThreadSupport::CurrentThread());
        gint oldState = t.holder.status;
        t.holder.status = CORE_CAST(gint, State::TIMED_WAITING);
        ThreadSupport::Sleep(millis);
        t.holder.status = oldState;
        t.monitor->unlock();
    }

    void Thread::sleep(glong millis, gint nanos) {
        if (nanos >= 500000L && millis < Long::MAX_VALUE)
            millis += 1;
        ThreadSupport::Sleep(millis);
    }

    void Thread::sleep(time::Duration const& duration) {
        try {
            glong millis = duration.toMillis();
            gint nanos = duration.nanos();
            sleep(millis, nanos);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void Thread::onSpinWait() {
        ThreadSupport::Sleep(0);
    }

    Object& Thread::clone() const {
        return CORE_CAST(Thread&, *this);
    }

    Thread& Thread::newThread(Runnable const& task) {
        try {
            return newThread(task, ""_S, 0L);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Thread& Thread::newThread(String const& name) {
        try {
            Runnable r;
            return newThread(r, name, 0);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Thread& Thread::newThread(Runnable const& task, String const& name) {
        try {
            return newThread(task, name, 0L);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Thread& Thread::newThread(Runnable const& task, String const& name, glong stackSize) {
        try {
            Thread& t = *new Thread();
            glong routine = 0;
            switch (ThreadSupport::GetRoutineForm()) {
                case ThreadSupport::RoutineForm::WIN32_DLL:
                    class DLLRoutine final : public Object {
                    public:
                        static void run(void* ptr) {
                            THREAD t = CORE_CAST(THREAD, ptr);
                            t->monitor->lock();
                            CORE_ASSERT(t->threadState() == State::NEW);
                            t->eetop = CORE_CAST(glong, run);
                            t->holder.status = CORE_CAST(gint, State::RUNNABLE);
                            t->monitor->unlock();

                            Throwable const* except = null;
                            try {
                                t->run();
                                t->monitor->lock();
                                t->eetop = 0;
                            } catch (Throwable const& ex) {
                                except = &ex;
                            } catch (...) {
                                t->monitor->lock();
                                t->eetop = 0;
                                CORE_ASSERT(t->threadState() == State::RUNNABLE);
                                t->holder.status = CORE_CAST(gint, State::TERMINATED);
                                t->monitor->unlock();
                                throw;
                            }

                            t->monitor->lock();
                            t->eetop = 0;
                            CORE_ASSERT(t->threadState() == State::RUNNABLE);
                            t->holder.status = CORE_CAST(gint, State::TERMINATED);
                            t->monitor->unlock();

                            if (except != null)
                                except->throws($ftrace());
                        }
                    };
                    routine = CORE_CAST(glong, DLLRoutine::run);
                    break;
                case ThreadSupport::RoutineForm::WIN32_STD:
                    class STDRoutine final : public Object {
                    public:
                        static void run(void* ptr) {
                            THREAD t = CORE_CAST(THREAD, ptr);
                            t->monitor->lock();
                            CORE_ASSERT(t->threadState() == State::NEW);
                            t->eetop = CORE_CAST(glong, run);
                            t->holder.status = CORE_CAST(gint, State::RUNNABLE);
                            t->monitor->unlock();

                            Throwable const* except = null;
                            try {
                                t->run();
                                t->monitor->lock();
                                t->eetop = 0;
                            } catch (Throwable const& ex) {
                                except = &ex;
                            } catch (...) {
                                t->monitor->lock();
                                t->eetop = 0;
                                CORE_ASSERT(t->threadState() == State::RUNNABLE);
                                t->holder.status = CORE_CAST(gint, State::TERMINATED);
                                t->monitor->unlock();
                                throw;
                            }

                            t->monitor->lock();
                            t->eetop = 0;
                            CORE_ASSERT(t->threadState() == State::RUNNABLE);
                            t->holder.status = CORE_CAST(gint, State::TERMINATED);
                            t->monitor->unlock();

                            if (except != null)
                                except->throws($ftrace());
                        }
                    };
                    routine = CORE_CAST(glong, STDRoutine::run);
                    break;
                case ThreadSupport::RoutineForm::UNIX_STD:
                    class UNIXRoutine final : public Object {
                    public:
                        static void run(void* arg) {
                            THREAD t = ThreadSupport::getArgsThread(arg);
                            CORE_ASSERT(t != null);
                            t->eetop = (glong)arg;
                            ThreadSupport::suspendArgs(arg);
                            t->monitor->lock();
                            CORE_ASSERT(t->threadState() == State::NEW);
                            t->eetop = CORE_CAST(glong, run);
                            t->holder.status = CORE_CAST(gint, State::RUNNABLE);
                            t->monitor->unlock();

                            Throwable const* except = null;
                            try {
                                t->run();
                            } catch (Throwable const& ex) {
                                except = &ex;
                            } catch (...) {
                                t->monitor->lock();
                                t->eetop = 0;
                                CORE_ASSERT(t->threadState() == State::RUNNABLE);
                                t->holder.status = CORE_CAST(gint, State::TERMINATED);
                                t->monitor->unlock();
                                t->stop();
                            }

                            t->monitor->lock();
                            t->eetop = 0;
                            CORE_ASSERT(t->threadState() == State::RUNNABLE);
                            t->holder.status = CORE_CAST(gint, State::TERMINATED);
                            t->monitor->unlock();

                            if (except != null)
                                except->throws($ftrace());
                        }
                    };
                    routine = CORE_CAST(glong, UNIXRoutine::run);
                    break;
            }
            glong tid = ThreadSupport::CreateNewThread(stackSize, routine, t);
            // Initialize Fields
            CORE_CAST(glong&, t.tid) = tid;
            t.status = false;
            if (!name.isEmpty()) {
                if (ThreadSupport::SetThreadName(tid, name))
                    t.name = name;
            } else {
                // Generating thread-name
                String genName = GenerateThreadName();
                if (ThreadSupport::SetThreadName(tid, genName))
                    t.name = genName;
            }
            // Initialize Fields Holder
            t.holder.priority = ThreadSupport::GetThreadPriority(tid);
            t.holder.daemon = false;
            t.holder.status = CORE_CAST(gint, State::NEW);
            Runnable r;
            t.holder.task = r == task ? null : CORE_DCAST(TASK, &task.clone());
            t.monitor = new concurrent::ReentrantLock();

            ThreadList& threads = THREAD_LIST;
            threads.add(t);
            return t;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Thread::~Thread() {
        if (Thread::isDaemon())
            ThreadSupport::StopThread(tid);
        while (CORE_DCAST(concurrent::ReentrantLock*, monitor)->isLocked())
            monitor->unlock();
        delete monitor;
    }

    void Thread::start() const {
        monitor->lock();
        if (holder.status != CORE_CAST(gint, State::NEW)) {
            monitor->unlock();
            IllegalStateException().throws($ftrace());
        }
        monitor->unlock();
        if (ThreadSupport::GetRoutineForm() == ThreadSupport::RoutineForm::UNIX_STD) {
            ThreadSupport::resumeArgs((void*)eetop);
        } else {
            CORE_CAST(glong&, eetop) = -1;
            ThreadSupport::StartThread(tid);
        }
    }

    void Thread::run() const {
        monitor->lock();
        if (holder.task != null) {
            Runnable const& runnable = *holder.task;
            monitor->unlock();
            runnable.run();
            return;
        }
        monitor->unlock();
    }

    void Thread::stop() const {
        monitor->lock();
        if (holder.status != CORE_CAST(gint, State::NEW)) {
            monitor->unlock();
            resume();
        } else
            monitor->unlock();
        ThreadSupport::StopThread(tid);
    }

    void Thread::exit() {
        ThreadSupport::ExitThreads();
    }

    void Thread::interrupt() {
        if (tid != ThreadSupport::CurrentThread()) {
            // checkAccess();
            // thread may be blocked in an I/O operation

            monitor->lock();
            io::Interruptible* b = CORE_DCAST(io::Interruptible*, rwBlocker);
            if (b != null) {
                status = true;
                monitor->unlock();
                b->interrupt(*this);
                return;
            }
            monitor->unlock();
        }
        status = true;
    }

    gbool Thread::interrupted() {
        Thread& t = currentThread();
        gbool oldValue = t.status;
        // We may have been interrupted the moment after we read the field,
        // so only clear the field if we saw that it was set and will return
        // true; otherwise we could lose an interrupt.
        if (oldValue) {
            t.status = false;
            // Clean Interrupt Event.
        }
        return oldValue;
    }

    gbool Thread::isInterrupted() const {
        return status;
    }

    gbool Thread::isAlive() const {
        return eetop != 0;
    }

    void Thread::suspend() const {
        monitor->lock();
        if (holder.status != CORE_CAST(gint, State::RUNNABLE)) {
            monitor->unlock();
            IllegalThreadStateException().throws($ftrace());
        }
        monitor->unlock();
        if (ThreadSupport::GetRoutineForm() == ThreadSupport::RoutineForm::UNIX_STD) {
            ThreadSupport::suspendArgs((void *) eetop);
        } else
            ThreadSupport::SuspendThread(tid);
    }

    void Thread::resume() const {
        monitor->lock();
        if (holder.status == CORE_CAST(gint, State::TERMINATED)) {
            monitor->unlock();
            return;
        }
        if (holder.status != CORE_CAST(gint, State::BLOCKED)) {
            monitor->unlock();
            IllegalThreadStateException().throws($ftrace());
        }
        monitor->unlock();
        if (ThreadSupport::GetRoutineForm() == ThreadSupport::RoutineForm::UNIX_STD) {
            ThreadSupport::resumeArgs((void*)eetop);
        } else
            ThreadSupport::ResumeThread(tid);
    }

    void Thread::setPriority(gint newPriority) {
        if (newPriority > MAX_PRIORITY || newPriority < MIN_PRIORITY)
            IllegalArgumentException().throws($ftrace());
        ThreadSupport::SetThreadPriority(tid, newPriority);
        holder.priority = newPriority;
    }

    gint Thread::priority() const {
        return holder.priority;
    }

    void Thread::setName(String const& newName) {
        monitor->lock();
        ThreadSupport::SetThreadName(tid, newName);
        name = newName;
    }

    String Thread::getName() const {
        return name;
    }

    using time::Chrono;
    using time::Duration;
    using time::Temporal;

    void Thread::join(glong millis) const {
        monitor->lock();
        if (millis > 0) {
            if (isAlive()) {
                Chrono chronometer = Chrono();
                glong delay = millis;
                chronometer.start();
                monitor->unlock();
                do {
                    ThreadSupport::WaitForThread(tid, delay);
                } while (isAlive() && (delay = millis - chronometer.elapsedTime()) > 0);
            }
        } else {
            monitor->unlock();
            while (isAlive()) {
                ThreadSupport::WaitForThread(tid, 0);
            }
        }
    }

    void Thread::join(glong millis, gint nanos) const {
        if (millis < 0)
            IllegalArgumentException("Negative timeout value"_S).throws($ftrace());

        if (nanos < 0 || nanos > 999999)
            IllegalArgumentException("Nanosecond of timeout value out of range."_S).throws($ftrace());

        if (nanos > 499999 & millis < Long::MAX_VALUE)
            millis += 1;

        join(millis);
    }

    gbool Thread::join(Duration const& duration) const {
        glong millis = duration.toMillis();
        State state = threadState();
        if (state == State::NEW)
            IllegalArgumentException().throws($ftrace());
        if (state == State::TERMINATED)
            return true;
        if (millis < 0)
            return false;
        gint ns = duration.nanos();
        if (millis == 0 && ns == 0)
            return false;
        join(millis, ns);
        return threadState() == State::TERMINATED;
    }

    void Thread::join() const {
        join(0);
    }

    void Thread::setDaemon(gbool on) {
        if (isAlive())
            IllegalThreadStateException().throws($ftrace());
    }

    gbool Thread::isDaemon() const {
        return holder.daemon;
    }

    String Thread::toString() const {
        XString xs = XString("Thread[#"_S);
        xs.append(threadId());
        xs.append(",");
        xs.append(getName());
        xs.append(",");
        xs.append(priority());
        // xs.append(",");
        // ThreadGroup group = getThreadGroup();
        // if (group != null)
        //     xs.append(group.getName());
        xs.append("]");
        return xs.toString();
    }

    glong Thread::threadId() const {
        return tid;
    }

    Thread::State Thread::threadState() const {
        return CORE_CAST(State, holder.status);
    }

    gbool Thread::equals(const Object& o) const {
        if (this == &o)
            return true;
        return Class<Thread>::hasInstance(o) && CORE_XCAST(Thread const, o).tid == tid;
    }

    gint Thread::hash() const {
        return Long::hash(tid);
    }

    static Thread& MAIN = Thread::currentThread();
} // core
