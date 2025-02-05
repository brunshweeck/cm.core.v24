//
// Created by bruns on 01/10/2024.
//

#ifndef CORE24_UNIX_THREADSUPPORT_H
#define CORE24_UNIX_THREADSUPPORT_H

#include <core/Thread.h>

namespace core {
    /**
     * The little utility for threads management.
     */
    class ThreadSupport final : public Object {
        CORE_ALIAS(UNSAFE, misc::Unsafe);

        CORE_IMPLICIT ThreadSupport() = default;

    public:

        CORE_ALIAS(THREAD, Class<Thread>::Pointer);

        static void initArgs(void *arg, Thread &t);

        // Empeche le thread en cours de s'executer directement apres creation
        static void suspendArgs(void* arg);

        static void resumeArgs(void* arg);

        static THREAD getArgsThread(void* arg);

        static void destroyArgs(void* arg);

        enum class RoutineForm {
            WIN32_DLL, // using CreateThread
            WIN32_STD, // using _beginThreadEx
            UNIX_STD // using pthread lib
        };

        static RoutineForm GetRoutineForm();

        static glong CreateNewThread(glong stackSize, glong task, Thread &thread);

        static String GetThreadName(glong tid);

        static gbool SetThreadName(glong tid, String const &newName);

        static glong CurrentThread();

        static void YieldCurrentThread();

        static gbool Sleep(glong millis);

        static gbool StartThread(glong tid);

        static gbool ResumeThread(glong tid);

        static gbool StopThread(glong tid);

        static gbool SuspendThread(glong tid);

        static gbool SetThreadPriority(glong tid, gint priority);

        static gint GetThreadPriority(glong tid);

        static gbool WaitForThread(glong tid, glong millis);

        static void LoadLibraries();

        static LongArray GetProcessThreads();

        static glong CreateIndex();

        static void ExitThreads();
    };
} // core

#endif //CORE24_UNIX_THREADSUPPORT_H
