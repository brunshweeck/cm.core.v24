//
// Created by bruns on 01/10/2024.
//

#include <errno.h>
#include <pthread.h>
#include <core/LongArray.h>
#include <core/SecurityException.h>
#include <core/ThreadSupport.h>
#include <core/util/Arrays.h>

#include "Unix.h"

namespace core {
    static size_t AlignWordSize(glong byte) {
        if (byte < 0)
            return 0L;

        if (byte < 16)
            return 16;

        size_t ADDRESS_SIZE = misc::Unsafe::addressSize();
        return byte + ADDRESS_SIZE - 1 & ~(ADDRESS_SIZE - 1);
    }

    gint UnixPriorityToThreadPriority(gint p) {
        return Thread::IDLE_PRIORITY;
    }

    gint ThreadPriorityToUnixPriority(gint priority) {
        return 0;
    }

    namespace {
        CORE_ALIAS(THREAD, Class<Thread>::Pointer);
        class Args final : public Object {
        public:


            gint volatile state = 0;
            glong threadId = 0;
            THREAD thread = NULL;
            pthread_mutex_t mutex = { };
            pthread_cond_t cond = { };

            Args() = default;
        };
        CORE_ALIAS(ARGS, Class<Args>::Pointer);
    }

    void ThreadSupport::initArgs(void* arg, Thread& t) {
        ARGS args = (ARGS)arg;
        args->thread = &t;
        pthread_mutex_init(&args->mutex, NULL);
        pthread_cond_init(&args->cond, NULL);
    }

    void ThreadSupport::suspendArgs(void* arg) {
        ARGS args = (ARGS)arg;
        pthread_mutex_lock(&args->mutex);
        args->state++;
        pthread_cond_wait(&args->cond, &args->mutex);
        pthread_mutex_unlock(&args->mutex);
    }

    void ThreadSupport::resumeArgs(void* arg) {
        ARGS args = (ARGS)arg;
        pthread_mutex_lock(&args->mutex);
        args->state--;
        pthread_cond_signal(&args->cond);
        pthread_mutex_unlock(&args->mutex);
    }

    ThreadSupport::THREAD ThreadSupport::getArgsThread(void* arg) {
        ARGS args = (ARGS)arg;
        if (args->threadId == 0)
            args->threadId = pthread_self();
        return args->thread;
    }

    void ThreadSupport::destroyArgs(void* arg) {
        ARGS args = (ARGS)arg;
        pthread_cond_destroy(&args->cond);
        pthread_mutex_destroy(&args->mutex);
        UNSAFE::deleteInstance(*args);
    }

    ThreadSupport::RoutineForm ThreadSupport::GetRoutineForm() {
        return RoutineForm::UNIX_STD;
    }

    glong ThreadSupport::CreateNewThread(glong stackSize, glong task, Thread& thread) {
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        gint code = 0;

        code = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        if (stackSize > 0) {
            size_t StackSize = AlignWordSize(stackSize);
            code = pthread_attr_setguardsize(&attr, StackSize);
        }

        pthread_t ID = { };

        Args& args = UNSAFE::newInstance<Args>();
        initArgs(&args, thread);
        code = pthread_create(&ID, &attr, CORE_CAST(void*(*)(void*), task), &args);

        pthread_attr_destroy(&attr);

        if (code != 0) {
            Error("Unable to create new Thread."_S).throws($ftrace());
        }

        return CORE_CAST(glong, ID);
    }

    String ThreadSupport::GetThreadName(glong tid) {
        char name[1024] = { };
        pthread_getname_np(tid, name, 1024);
        return String(name + 0);
    }

    gbool ThreadSupport::SetThreadName(glong tid, String const& newName) {
        ByteArray str = newName.toBytes();
        gint code = pthread_setname_np(tid, (char const*)&str[0]);
        return code == 0;
    }

    glong ThreadSupport::CurrentThread() {
        return CORE_CAST(glong, pthread_self());
    }

    void ThreadSupport::YieldCurrentThread() {
        sched_yield();
    }

    gbool ThreadSupport::Sleep(glong millis) {
        timespec ts = { };
        ts.tv_sec = millis / 1000;
        ts.tv_nsec = (millis % 1000) * 1000000;
        gint code;
        do {
            code = nanosleep(&ts, &ts);
        } while (code == -1 && errno == EINTR);
        return code == 0;
    }

    gbool ThreadSupport::StartThread(glong tid) {
        return true;
    }

    gbool ThreadSupport::ResumeThread(glong tid) {
        return true;
    }

    gbool ThreadSupport::StopThread(glong tid) {
        gint code = pthread_cancel(tid);
        if (code != 0) {
            Error("Unable to terminate Thread."_S).throws($ftrace());
            return false;
        }
        return true;
    }

    gbool ThreadSupport::SuspendThread(glong tid) {
        return true;
    }

    gbool ThreadSupport::SetThreadPriority(glong tid, gint priority) {
        gint policy = 0;
        sched_param param = { };

        if (pthread_getschedparam(tid, &policy, &param) != 0) {
            return false;
        }

        gint prio = param.sched_priority;

        if (prio == priority)
            return true;

        param.sched_priority = prio;
        return pthread_setschedparam(tid, policy, &param) == 0;
    }

    gint ThreadSupport::GetThreadPriority(glong tid) {
        gint policy = 0;
        sched_param param = { };

        if (pthread_getschedparam(tid, &policy, &param) != 0) {
            return param.sched_priority == 0 ? Thread::NORM_PRIORITY : param.sched_priority;
        }

        return Thread::NORM_PRIORITY;
    }

    gbool ThreadSupport::WaitForThread(glong tid, glong millis) {
        if (tid == pthread_self()) {
            return false;
        }

        if (millis < 0)
            millis = 0;

        if (millis == 0)
            return pthread_join(tid, NULL);

        timespec ts = { };
        ts.tv_sec = millis / 1000;
        ts.tv_nsec = (millis % 1000) * 1000000;
        LPPVOID ret = NULL;

        gint r = pthread_timedjoin_np(tid, ret, &ts);
        return r == 0;
    }

    void ThreadSupport::LoadLibraries() {
        //
    }

    LongArray ThreadSupport::GetProcessThreads() {
        return { };
    }

    glong ThreadSupport::CreateIndex() {
        return 0;
    }

    void ThreadSupport::ExitThreads() {
        pthread_exit(NULL);
    }
} // core
