//
// Created by bruns on 01/10/2024.
//

#ifndef UNICODE
#define UNICODE
#endif


#include <core/lang/ProcessThreadsApi.h>
#include <core/lang/TlHelp32.h>
#include <core/lang/Windows.h>
#include <core/lang/SecurityException.h>
#include <core/util/Arrays.h>
#include <core/misc/Event.h>

#ifndef CORE_COMPILER_MSVC

CORE_ALIAS(GET_THREAD_DESCRIPTION, HRESULT(*)(HANDLE hThread, PWSTR* pszThreadDescription));
CORE_ALIAS(SET_THREAD_DESCRIPTION, HRESULT(*)(HANDLE hThread, PCWSTR lpThreadDescription));

static SET_THREAD_DESCRIPTION SetThreadDescription = NULL;
static GET_THREAD_DESCRIPTION GetThreadDescription = NULL;
#endif

static DWORD volatile LoadLibraryState = 123456789;

#line CORE_LINE "ThreadSupport.c24"

namespace core {


    static gint volatile status = 0;
    CORE_ALIAS(UNSAFE, misc::Unsafe);

    static void tryLock() {
        misc::Event event = {};
        while (!UNSAFE::weakCompareAndSetInt(null, CORE_CAST(glong, &status), 0, 1)) {
            event.waitFor(5);
        }
    }

    static void unlock() {
        misc::Event event = {};
        while (!UNSAFE::weakCompareAndSetInt(null, CORE_CAST(glong, &status), 1, 0)) {
            event.waitFor(5);
        }
    }

    static SIZE_T AlignWordSize(glong byte) {
        if (byte < 0)
            return 0L;

        if (byte > 0 && byte < 16)
            return 16;

        SIZE_T ADDRESS_SIZE = misc::Unsafe::addressSize();
        return byte + ADDRESS_SIZE - 1 & ~(ADDRESS_SIZE - 1);
    }

    static gint ThreadPriorityToWin32Priority(gint priority) {
        switch (priority) {
            case Thread::INHERIT_PRIORITY: return GetThreadPriority(GetCurrentThread());
            case Thread::MIN_PRIORITY:
            case Thread::IDLE_PRIORITY: return THREAD_PRIORITY_IDLE;
            case Thread::LOWEST_PRIORITY: return THREAD_PRIORITY_LOWEST;
            case Thread::LOW_PRIORITY: return THREAD_PRIORITY_BELOW_NORMAL;
            case Thread::NORM_PRIORITY: return THREAD_PRIORITY_NORMAL;
            case Thread::HIGH_PRIORITY: return THREAD_PRIORITY_ABOVE_NORMAL;
            case Thread::HIGHEST_PRIORITY: return THREAD_PRIORITY_HIGHEST;
            case Thread::TIME_CRITICAL_PRIORITY:
            case Thread::MAX_PRIORITY: return THREAD_PRIORITY_TIME_CRITICAL;
            default: return THREAD_PRIORITY_NORMAL;
        }
    }

    static gint Win32PriorityToThreadPriority(DWORD win32Priority) {
        switch (CORE_CAST(gint, win32Priority)) {
            case THREAD_PRIORITY_IDLE: return Thread::IDLE_PRIORITY;
            case THREAD_PRIORITY_LOWEST: return Thread::LOWEST_PRIORITY;
            case THREAD_PRIORITY_BELOW_NORMAL: return Thread::LOW_PRIORITY;
            case THREAD_PRIORITY_NORMAL: return Thread::NORM_PRIORITY;
            case THREAD_PRIORITY_ABOVE_NORMAL: return Thread::HIGH_PRIORITY;
            case THREAD_PRIORITY_HIGHEST: return Thread::HIGHEST_PRIORITY;
            case THREAD_PRIORITY_TIME_CRITICAL: return Thread::MAX_PRIORITY;
            default: return Thread::NORM_PRIORITY;
        }
    }

    ThreadSupport::RoutineForm ThreadSupport::GetRoutineForm() {
#if defined(CORE_COMPILER_MSVC) && !defined(_DLL)
        return RoutineForm::WIN32_STD;
#endif
        return RoutineForm::WIN32_DLL;
    }

    glong ThreadSupport::CreateNewThread(glong stackSize, glong task, Thread &thread) {
        LoadLibraries();
        DWORD StackSize = AlignWordSize(stackSize);
#if defined(CORE_COMPILER_MSVC) && !defined(_DLL)
        unsigned ID = 0;
        HANDLE handle = (HANDLE) _beginthreadex(NULL,
                                                StackSize,
                                                CORE_CAST(_beginthreadex_proc_type, task),
                                                &thread,
                                                CREATE_SUSPENDED,
                                                &ID);
#else
        DWORD ID = 0;
        HANDLE handle = CreateThread(NULL,
                                     StackSize,
                                     CORE_CAST(LPTHREAD_START_ROUTINE, task),
                                     &thread,
                                     CREATE_SUSPENDED,
                                     &ID);
#endif

        if (handle == NULL) {
            // DWORD error = GetLastError();
            Error("Unable to create new thread"_S).throws($ftrace());
        }

        CloseHandle(handle);

        return ID;
    }

    String ThreadSupport::GetThreadName(glong tid) {
        LoadLibraries();
        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());

        CharArray ca = CharArray(MAX_CLASS_NAME);

        if (GetThreadDescription != NULL) {
            LPWSTR desc = NULL;
            HRESULT r = GetThreadDescription(handle, &desc);
            if (SUCCEEDED(r)) {
                CloseHandle(handle);
                return desc;
            }
        }

        CloseHandle(handle);
        return ""_S;
    }

    gbool ThreadSupport::SetThreadName(glong tid, String const &newName) {
        LoadLibraries();
        CORE_ASSERT(!newName.isEmpty());
        CharArray ca = newName.toChars();

        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());

        if (SetThreadDescription != NULL) {
            HRESULT r = SetThreadDescription(handle, CORE_CAST(LPWSTR, &ca[0]));
            CloseHandle(handle);
            if (SUCCEEDED(r))
                return true;
        }
        CloseHandle(handle);
        return false;
    }

    glong ThreadSupport::CurrentThread() {
        DWORD tid = GetCurrentThreadId();
        if (tid == 0)
            Error("Unable to get current thread"_S).throws($ftrace());
        return tid;
    }

    void ThreadSupport::YieldCurrentThread() {
        SwitchToThread();
    }

    gbool ThreadSupport::Sleep(glong millis) {
        DWORD timeout;
        if (millis < 0)
            timeout = 0;
        else if (millis == 0 || millis > CORE_CAST(DWORD, -1))
            timeout = INFINITE;
        else
            timeout = CORE_CAST(DWORD, millis);

        auto r = SleepEx(timeout, 1);
        return r == 0;
    }

    gbool ThreadSupport::StartThread(glong tid) {
        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());
        HANDLE hToken = NULL;
        if (OpenThreadToken(handle, GENERIC_READ | GENERIC_WRITE, 0, &hToken) != 0) {
            if (AccessCheck(NULL, hToken, THREAD_RESUME, NULL, NULL, NULL, NULL, NULL) == 0) {
                CloseHandle(hToken);
                CloseHandle(handle);
                SecurityException().throws($ftrace());
            }
        }
        DWORD r = ::ResumeThread(handle);
        CloseHandle(handle);
        return r != -1;
    }

    gbool ThreadSupport::ResumeThread(glong tid) {
        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());
        HANDLE hToken = NULL;
        if (OpenThreadToken(handle, GENERIC_READ | GENERIC_WRITE, 0, &hToken) != 0) {
            if (AccessCheck(NULL, hToken, THREAD_RESUME, NULL, NULL, NULL, NULL, NULL) == 0) {
                CloseHandle(hToken);
                CloseHandle(handle);
                SecurityException().throws($ftrace());
            }
        }
        DWORD r = ::ResumeThread(handle);
        CloseHandle(handle);
        return r != -1;
    }

    gbool ThreadSupport::StopThread(glong tid) {
        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());
        HANDLE hToken = NULL;
        if (OpenThreadToken(handle, GENERIC_READ | GENERIC_WRITE, 0, &hToken) != 0) {
            if (AccessCheck(NULL, hToken, THREAD_TERMINATE, NULL, NULL, NULL, NULL, NULL) == 0) {
                CloseHandle(hToken);
                CloseHandle(handle);
                SecurityException().throws($ftrace());
            }
        }
        DWORD r = ::TerminateThread(handle, EXIT_SUCCESS);
        CloseHandle(handle);
        return r != 0;
    }

    gbool ThreadSupport::SuspendThread(glong tid) {
        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());
        HANDLE hToken = NULL;
        if (OpenThreadToken(handle, GENERIC_READ | GENERIC_WRITE, 0, &hToken) != 0) {
            if (AccessCheck(NULL, hToken, THREAD_SUSPEND_RESUME, NULL, NULL, NULL, NULL, NULL) == 0) {
                CloseHandle(hToken);
                CloseHandle(handle);
                SecurityException().throws($ftrace());
            }
        }
        DWORD r = ::SuspendThread(handle);
        CloseHandle(handle);
        return r != -1;
    }

    gbool ThreadSupport::SetThreadPriority(glong tid, gint priority) {
        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());
        DWORD r = ::SetThreadPriority(handle, ThreadPriorityToWin32Priority(priority));
        CloseHandle(handle);
        return r != 0;
    }

    gint ThreadSupport::GetThreadPriority(glong tid) {
        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());
        DWORD r = ::GetThreadPriority(handle);
        CloseHandle(handle);
        return Win32PriorityToThreadPriority(r);
    }

    gbool ThreadSupport::WaitForThread(glong tid, glong millis) {
        DWORD timeout = 0;
        if (millis < 0)
            timeout = 0;
        else if (millis == 0 or millis > CORE_CAST(DWORD, -1))
            timeout = INFINITE;
        else
            timeout = CORE_CAST(DWORD, millis);
        HANDLE handle = OpenThread(GENERIC_READ | GENERIC_WRITE, 0, tid);
        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            Error("Unable to open thread"_S).throws($ftrace());

        DWORD r = WaitForSingleObjectEx(handle, timeout, 0);
        switch (r) {
            case WAIT_OBJECT_0:
            case WAIT_TIMEOUT:
                return true;
            case WAIT_FAILED:
                // Error("Wait operation failed"_S).throws($ftrace());
            default:
                return false;
        }
    }

    void ThreadSupport::LoadLibraries() {
        tryLock();
        if (LoadLibraryState != 123454321) {
            HMODULE module = LoadLibrary(TEXT("Kernel32.dll"));
#ifndef CORE_COMPILER_MSVC
            FARPROC proc1 = GetProcAddress(module, "GetThreadDescription");
            FARPROC proc2 = GetProcAddress(module, "SetThreadDescription");
            if (proc1 != NULL)
                GetThreadDescription = CORE_CAST(GET_THREAD_DESCRIPTION, proc1);
            if (proc2 != NULL)
                SetThreadDescription = CORE_CAST(SET_THREAD_DESCRIPTION, proc2);
#endif
            FreeLibrary(module);
            LoadLibraryState = 123454321;
        }
        unlock();
        CORE_ASSERT(LoadLibraryState == 123454321);
    }

    LongArray ThreadSupport::GetProcessThreads() {
        DWORD pid = GetCurrentProcessId();
        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, pid);

        if (handle == NULL || handle == INVALID_HANDLE_VALUE)
            return {};

        THREADENTRY32 threadEntry;

        // Fill in the size of the structure before using i
        threadEntry.dwSize = sizeof(THREADENTRY32);

        // Retrieve information about the first thread,
        // and exit if unsuccessful
        if (Thread32First(handle, &threadEntry) == 0) {
            CloseHandle(handle);
            return {};
        }

        LongArray threads = LongArray(256);
        gint i = 0;
        // Now walk the thread list of the system,
        // and display information about each thread
        // associated with the specified process
        do {
            if (threadEntry.th32OwnerProcessID == pid) {
                if (i == threads.length()) {
                    gint newLength = util::Arrays::newLength(i, i + 5, i >> 1);
                    threads = util::Arrays::copyOf(threads, newLength);
                }
                threads[i++] = threadEntry.th32ThreadID;
            }
        } while (Thread32Next(handle, &threadEntry) != 0);

        CloseHandle(handle);

        if (i < threads.length())
            return util::Arrays::copyOf(threads, i);

        return threads;
    }

    glong ThreadSupport::CreateIndex() {
        DWORD r = TlsAlloc();
        return r;
    }
} // core
