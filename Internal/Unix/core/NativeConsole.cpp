//
// Created by bruns on 19/10/2024.
//

#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <core/Array.h>
#include <core/ByteArray.h>
#include <core/NativeConsole.h>
#include <core/misc/Event.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>


namespace core {

CORE_ALIAS(VOID, Class<void>::Target);
CORE_ALIAS(PVOID, Class<VOID>::Pointer);
CORE_ALIAS(LPVOID, Class<VOID>::Pointer);
CORE_ALIAS(CHAR, Class<char>::Target);
CORE_ALIAS(CCHAR, Class<CHAR>::Const);
CORE_ALIAS(STR, Class<CHAR>::Pointer);
CORE_ALIAS(CSTR, Class<CCHAR>::Pointer);
CORE_ALIAS(LPSTR, Class<CHAR>::Pointer);
CORE_ALIAS(LPCSTR, Class<CCHAR>::Pointer);


    static gint volatile status = 0;
    CORE_ALIAS(UNSAFE, misc::Unsafe);

    static void tryLock() {
        misc::Event event = {};
        while (!UNSAFE::weakCompareAndSetInt(null, CORE_CAST(glong, &status), 0, 1)) {
            event.waitFor(1);
        }
    }

    static void unlock() {
        misc::Event event = {};
        while (!UNSAFE::weakCompareAndSetInt(null, CORE_CAST(glong, &status), 1, 0)) {
            event.waitFor(1);
        }
    }

    glong NativeConsole::WriteError(ByteArray const &text) {
        if (text.length() < 1)
            return 0;
        tryLock();
        static FILE *handle = 0;
        if (!handle)
            handle = fdopen(2, "w");
        if (!handle) {
            unlock();
            Error().throws($ftrace());
        }
        gint bytes = fwrite(&text[0], 1, text.length(), handle);
        fflush(handle);
        unlock();
        return bytes;
    }

    glong NativeConsole::Write(ByteArray const &text) {
        if (text.length() < 1)
            return 0;
        tryLock();
        static FILE *handle = 0;
        if (!handle)
            handle = fdopen(1, "w");
        if (!handle) {
            unlock();
            Error().throws($ftrace());
        }
        gint bytes = fwrite(&text[0], 1, text.length(), handle);
        fflush(handle);
        unlock();
        return bytes;
    }

    glong NativeConsole::WriteLn(ByteArray const &text) {
        if (text.length() < 1)
            return Write(ByteArray::of('\n'));
        
        tryLock();
        static FILE *handle = 0;
        if (!handle)
            handle = fdopen(1, "w");
        if (!handle) {
            unlock();
            Error().throws($ftrace());
        }
        gint bytes = fwrite(&text[0], 1, text.length(), handle);
        fputc('\n', handle);
        fflush(handle);
        unlock();
        return bytes;
    }

    gint NativeConsole::ReadByte() {
        static FILE *handle = 0;
        if (!handle)
            handle = fdopen(0, "r");
        if (!handle)
            Error().throws($ftrace());
        return fgetc(handle);
    }

    // Fonction pour afficher la pile d'appels
    gbool NativeConsole::ReadStacktrace(Array<Trace> &trace) {
        tryLock();
        trace = {};
        PVOID tmp[256] = {};
        gint n = backtrace(tmp, ClassOf(tmp)::count());
        if (n < 0) {
            unlock();
            return false;
        }
        CORE_IGNORE_DEPRECATIONS(trace = Array<Trace>(n);)
        static char out[8132] = {};
        gint j = 0;
        for (int i = 1; i < n; i++) {
            Dl_info info = {};
            gint r = dladdr(tmp[i], &info);
            if (r == 0)
                continue;
            if (!info.dli_fname)
                info.dli_fname = "";
            if (!info.dli_saddr || !info.dli_sname)
                continue;
            size_t size = 8192;
            int state = 0;
            char * ca = __cxxabiv1::__cxa_demangle(info.dli_sname, out, &size, &state);
            if (state != 0)
                continue;
            Trace t = Trace(ca, String(info.dli_fname), -2);
            trace.set(j++, t);
        }
        trace = util::Arrays::copyOf(trace, j);
        unlock();

        return true;

    }
} // core

