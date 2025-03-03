//
// Created by bruns on 19/10/2024.
//

#include <core/lang/Windows.h>
#include <core/lang/io.h>
#include <core/lang/ByteArray.h>
#include <core/misc/Event.h>
#include <core/misc/Unsafe.h>
#include <core/lang/Array.h>
#include <core/lang/dbghelp.h>

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

    glong NativeConsole::WriteError(ByteArray const &text) {
        if (text.length() < 1)
            return 0;
        static HANDLE handle = 0;
        if (!handle)
            handle = CORE_CAST(HANDLE, _get_osfhandle(2));
        DWORD bytes;
        WriteFile(handle, &text[0], text.length(), &bytes, NULL);
        FlushFileBuffers(handle);
        return bytes;
    }

    glong NativeConsole::Write(ByteArray const &text) {
        if (text.length() < 1)
            return 0;
        static HANDLE handle = 0;
        if (!handle)
            handle = CORE_CAST(HANDLE, _get_osfhandle(1));
        DWORD bytes = 0;
        WriteFile(handle, &text[0], text.length(), &bytes, NULL);
        FlushFileBuffers(handle);
        return bytes;
    }

    glong NativeConsole::WriteLn(ByteArray const &text) {
        if (text.length() < 1)
            return 0;
        static HANDLE handle = 0;
        if (!handle)
            handle = CORE_CAST(HANDLE, _get_osfhandle(1));
        DWORD bytes = 0;
        glong n = 0;
        while (n < text.length()) {
            WriteFile(handle, &text[0] + n, text.length() - n, &bytes, NULL);
            n += bytes;
            if (bytes == 0)
                break;
        }
        WriteFile(handle, "\n", 1, &bytes, NULL);
        FlushFileBuffers(handle);
        return n + bytes;
    }

    gint NativeConsole::ReadByte() {
        static HANDLE handle = 0;
        if (!handle)
            handle = CORE_CAST(HANDLE, _get_osfhandle(0));
        BYTE bytes[2];
        DWORD n = 0;
        ReadFile(handle, bytes, 1, &n, NULL);
        if (n < 1)
            return -1;
        return bytes[0];
    }

    // Fonction pour afficher la pile d'appels
    gbool NativeConsole::ReadStacktrace(Array<Trace> &trace) {
        tryLock();
        static HMODULE library = NULL;
        if (!library)
            library = LoadLibrary(TEXT("dbghelp"));
        if (!library)
            library = LoadLibrary(TEXT("dbghelp.dll"));
        if (!library)
            library = LoadLibrary(TEXT("dbghelp.lib"));
        if (!library) {
            unlock();
            return false;
        }


        static FARPROC SIP = NULL;
        if (!SIP)
            SIP = GetProcAddress(library, "SymInitializeW");
        static FARPROC SFAP = NULL;
        if (!SFAP)
            SFAP = GetProcAddress(library, "SymFromAddrW");
        FARPROC SCP = NULL;
        if (!SCP)
            SCP = GetProcAddress(library, "SymCleanup");

        CORE_ALIAS(SIT, BOOL(*)(HANDLE hProcess, PCWSTR UserSearchPath, WINBOOL fInvadeProcess));
        CORE_ALIAS(SFAT, BOOL(*)(HANDLE hProcess, DWORD64 Address, PDWORD64 Displacement, PSYMBOL_INFOW Symbol));
        CORE_ALIAS(SCT, BOOL(*)(HANDLE hProcess));

        if (!SIP || !SFAP || !SCP) {
            FreeLibrary(library);
            unlock();
            return false;
        }

        SIT InitializeSymbol = CORE_CAST(SIT, SIP);
        SFAT SymbolFromAddress = CORE_CAST(SFAT, SFAP);
        SCT SymbolCleanup = CORE_CAST(SCT, SCP);

        // Snapshot of frames
        PVOID buffer[256] = {};
        gint nbFrames = CaptureStackBackTrace(0, 256, buffer, NULL);

        // Start function
        InitializeSymbol(GetCurrentProcess(), NULL, true);

        CORE_IGNORE_DEPRECATIONS(trace = Array<Trace>(nbFrames);)
        SYMBOL_INFOW *symbol = (PSYMBOL_INFOW) LocalAlloc(LPTR, Class<SYMBOL_INFOW>::count() + 1024);
        for (gint i = 0; i < nbFrames; i++) {
            ZeroMemory(symbol, Class<SYMBOL_INFOW>::count() + 1024);
            symbol->MaxNameLen = MAX_SYM_NAME;
            if (SymbolFromAddress(GetCurrentProcess(), (DWORD64) buffer[i], NULL, symbol)) {
                String s1 = symbol->Name;
                DWORD len = symbol->NameLen;
//                String s2 = symbol->LineNumber;
            }
        }
        LocalFree(symbol);
        SymbolCleanup(GetCurrentProcess());

        unlock();
        return true;

    }
} // core
