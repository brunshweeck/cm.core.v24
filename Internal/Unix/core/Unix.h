//
// Created by admin on 15/12/24.
//

#ifndef CORE24_UNIX_H
#define CORE24_UNIX_H

#include <core/misc/Unsafe.h>

namespace core {
    using namespace util;
    using namespace text;
    using namespace time;
    using namespace io;

    CORE_ALIAS(UNSAFE, misc::Unsafe);
    CORE_ALIAS(CHAR, char);
    CORE_ALIAS(CCHAR, Class<CHAR>::Const);
    CORE_ALIAS(STR, Class<CHAR>::Pointer);
    CORE_ALIAS(CSTR, Class<CCHAR>::Pointer);
    CORE_ALIAS(INT, gint);
    CORE_ALIAS(UINT, unsigned int);
    CORE_ALIAS(LONG, long);
    CORE_ALIAS(ULONG, unsigned long);
    CORE_ALIAS(VOID, void);
    CORE_ALIAS(BOOL, gbool);
    CORE_ALIAS(PVOID, Class<void>::Pointer);
    CORE_ALIAS(LPVOID, PVOID);
    CORE_ALIAS(LPCVOID, Class<PVOID>::Const);
    CORE_ALIAS(LPSTR, STR);
    CORE_ALIAS(LPCSTR, CSTR);
    CORE_ALIAS(SIZE_T, size_t);
    CORE_ALIAS(LPPVOID, Class<LPVOID>::Pointer);
    CORE_ALIAS(LPPCVOID, Class<LPCVOID>::Pointer);

    CORE_ALIAS(DWORD, UINT);
    CORE_ALIAS(CDWORD, Class<DWORD>::Const);
    CORE_ALIAS(LPDWORD, Class<DWORD>::Pointer);
    CORE_ALIAS(LCPDWORD, Class<CDWORD>::Pointer);

    CORE_FAST gbool TRUE = true;
    CORE_FAST gbool FALSE = false;

    CORE_ALIAS(TCHAR, CHAR);

    struct LOCALTIME;
    struct TICK_COUNT;

    extern LPVOID Malloc(SIZE_T size, SIZE_T alignment) CORE_ALLOC_SIZE(1);

    extern LPVOID ReAlloc(LPVOID ptr, SIZE_T size, SIZE_T alignment);

    extern LPVOID ReAlloc(LPVOID ptr, SIZE_T size, SIZE_T oldSize, SIZE_T alignment);

    extern VOID Free(LPVOID ptr);

    extern LPCSTR GetEnv(LPCSTR variable);

    extern LOCALTIME GetLocalTime();

    extern DWORD GetLocalTimeZone(LPSTR lpBuffer, DWORD nBufferSize);

    extern TICK_COUNT GetTickCount();

    extern LONG Seed();

    extern LPVOID GetFileHandle(INT lpFileDescriptor);

    enum {
        FA_DIRECTORY     = 0x00001,
        FA_REGULAR       = 0x00002,
        FA_SYMLINK       = 0x00004,
        FA_BLOCK         = 0x00008,
        FA_FIFO          = 0x00010,
        FA_SOCKET        = 0x00020,
        FA_OWNER_READ    = 0x00040,
        FA_OWNER_WRITE   = 0x00080,
        FA_OWNER_EXECUTE = 0x00100,
        FA_GROUP_READ    = 0x00200,
        FA_GROUP_WRITE   = 0x00400,
        FA_GROUP_EXECUTE = 0x00800,
        FA_OTHER_READ    = 0x01000,
        FA_OTHER_WRITE   = 0x02000,
        FA_OTHER_EXECUTE = 0x04000,
        FA_HIDDEN        = 0x08000,
        FA_EXIST         = 0x10000,
    };

    extern SIZE_T GetFileAttributes(LPCSTR lpFileName);

    extern SIZE_T GetFileSize(LPCSTR lpFileName);

    extern LONG GetLastWriteTime(LPCSTR lpFileName);

    extern LONG GetCreateTime(LPCSTR lpFileName);

    extern LONG GetAccessTime(LPCSTR lpFileName);

    extern BOOL CreateFile(LPCSTR lpFileName);

    extern BOOL CreateDirectory(LPCSTR lpFileName);

    extern BOOL DeleteFile(LPCSTR lpFileName);

    enum {
        COPY_MOVE_DEFAULT_OPTIONS = 0,
        COPY_MOVE_REPLACE_EXISTING = 1,
        COPY_COPY_FILE_ATTRIBUTE = 2,
        MOVE_ATOMIC_MOVE = 4,
        COPY_MOVE_NOFOLLOW_LINK = 8,
        MOVE_MOVE_BY_COPY = 16,
    };

    extern BOOL CopyFile(LPCSTR lpFileName, LPCSTR lpNewName);

    extern BOOL MoveFile(LPCSTR lpFileName, LPCSTR lpNewName);

    extern BOOL CopyFileEx(LPCSTR lpFileName, LPCSTR lpNewName, DWORD dwFlags);

    extern BOOL MoveFileEx(LPCSTR lpFileName, LPCSTR lpNewName, DWORD dwFlags);

    extern LPCSTR RealPath(LPCSTR lpFileName);

    extern BOOL CreateLink(LPCSTR lpFileName, LPCSTR lpExistingDirectory);

    extern BOOL CreateSymbolicLink(LPCSTR lpFileName, LPCSTR lpExistingFile);

    extern BOOL CreateShortCut(LPCSTR lpFileName, LPCSTR lpExistingFile);

    extern DWORD ReadLink(LPCSTR lpFileName, LPSTR lpBuffer, DWORD nBufferSize);

    extern DWORD FileAttributesToPermission(DWORD dwFileAttributes);

    extern BOOL SetPermission(LPCSTR lpFileName, DWORD dwMode);

    struct FILE_ENTRY {
        LPCSTR lpFileName;
        DWORD dwInode;
        DWORD dwOffset;
        LPVOID lpIterator;
    };

    CORE_ALIAS(LPFILE_ENTRY, FILE_ENTRY*);

    extern BOOL ReadDirectory(LPCSTR lpFileName, LPFILE_ENTRY lpFileEntry);

    extern BOOL NextEntry(LPFILE_ENTRY lpFileEntry);

    extern VOID CloseEntry(LPFILE_ENTRY lpFileEntry);

    extern INT CompareFile(LPCSTR lpFileName1, LPCSTR lpFileName2);

    extern INT CompareFileEx(LPCSTR lpFileName1, LPCSTR lpFileName2, BOOL dwFollowLink);

#ifdef NULL
#undef NULL
#endif

#define NULL null

    struct LOCALTIME {
        INT dwYear;
        INT dwMonth; // 01 - 12
        INT dwDayOfYear; // 01 - 366
        INT dwDayOfMonth; // 01 - 31
        INT dwDayOfWeek; // 01 - 07
        INT dwHour; // 00 - 23
        INT dwMinute; // 00 - 50
        INT dwSecond; // 00 - 60
        INT dwNano; // 000000000 - 999999999
        INT dwDaylight; // -01 / 00 / 01
        INT dwOffset; // -18*3600 - +18*3600
        LPCSTR lpTzName; //
    };

    struct TICK_COUNT {
        LONG dwSeconds;
        INT dwNano;
    };
} // core

#endif //CORE24_UNIX_H
