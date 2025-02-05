//
// Created by admin on 15/12/24.
//

#include "System.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#include <core/time/LocalTime.h>
#include <core/io/File.h>


namespace core {
    static LPCSTR EMPTY_STRING = "";

    LPVOID Malloc(SIZE_T size, SIZE_T alignment) {
        return ReAlloc(NULL, size, 0L, alignment);
    }

    LPVOID ReAlloc(LPVOID ptr, SIZE_T size, SIZE_T alignment) {
        return ReAlloc(ptr, size, -1L, alignment);
    }

    LPVOID ReAlloc(LPVOID ptr, SIZE_T size, SIZE_T oldSize, SIZE_T alignment) {
        LPVOID oldPtr = ptr != NULL ? ((LPPVOID) ptr)[-1] : NULL;
        if (alignment < UNSAFE::ADDRESS_SIZE) {
            LPPVOID newPtr = (LPPVOID) realloc(oldPtr, size + UNSAFE::ADDRESS_SIZE);
            if (newPtr == NULL)
                return NULL;
            if ((LPVOID) newPtr == oldPtr) {
                // ReAlloc succeeded without reallocating
                return ptr;
            }

            *newPtr = (LPVOID) newPtr;
            return (LPVOID) (newPtr + 1);
        }

        // malloc returns pointers aligned at least at sizeof(SIZE_T) boundaries
        // but usually more (8- or 16-byte boundaries).
        // So we overallocate by alignment-sizeof(SIZE_T) bytes, so we're guaranteed to find
        // somewhere within the first alignment-sizeof(SIZE_T) that is properly aligned.

        // However, we need to store the actual pointer, so we need to allocate actually size +
        // alignment anyway.
        LONG oldOffset = ptr != NULL ? (STR) ptr - (STR) oldPtr : 0L;
        LPVOID realPtr = (LPVOID) realloc(oldPtr, size + alignment);
        if (realPtr == NULL)
            return NULL;
        SIZE_T faked = (SIZE_T) realPtr + alignment;
        faked &= ~(alignment - 1);
        LPPVOID fakedPtr = (LPPVOID) faked;

        if (ptr != NULL) {
            LONG newOffset = (STR) fakedPtr - (STR) realPtr;
            if (oldOffset != newOffset)
                memmove(fakedPtr, (STR) realPtr + oldOffset, oldSize < size ? oldSize : size);
        }

        // now save the value of the real pointer at faked-sizeof(LPVOID)
        // by construction, alignment > sizeof(LPVOID) and is a power of 2, so
        // faked-sizeof(LPVOID) is properly aligned for a pointer
        fakedPtr[-1] = realPtr;

        return (LPVOID) fakedPtr;
    }

    VOID Free(LPVOID ptr) {
        if (ptr == NULL)
            return;
        LPPVOID target = (LPPVOID) ptr;
        ::free(target[-1]);
    }

    LPCSTR GetEnv(LPCSTR variable) {
        if (variable == NULL)
            return EMPTY_STRING;
        LPCSTR result = getenv(variable);
        if (result == NULL)
            return EMPTY_STRING;
        return result;
    }

    LOCALTIME GetLocalTime() {
        timespec ts = {};
        INT result = clock_gettime(CLOCK_REALTIME, &ts);
        if (result == -1)
            Error("Unable to obtain current local datetime info").throws($ftrace());

        time_t t = ts.tv_sec;
        tm ltm = {};
        localtime_r(&t, &ltm);
        LOCALTIME lt = {};
        lt.dwYear = ltm.tm_year + 1900;
        lt.dwMonth = ltm.tm_mon + 1;
        lt.dwDayOfMonth = ltm.tm_mday;
        lt.dwDayOfWeek = ltm.tm_wday + 1;
        lt.dwDayOfYear = ltm.tm_yday + 1;
        lt.dwHour = ltm.tm_hour;
        lt.dwMinute = ltm.tm_min;
        lt.dwSecond = ltm.tm_sec;
        lt.dwNano = ts.tv_nsec;
        lt.dwDaylight = ltm.tm_isdst;
        lt.dwOffset = ltm.tm_gmtoff;
        lt.lpTzName = ltm.tm_zone;

        return lt;
    }

    DWORD GetLocalTimeZone(LPSTR lpBuffer, DWORD nBufferSize) {
        CHAR tmp[1024] = {};
        DWORD len = ReadLink("/etc/localtime", tmp, 1024) - 20;
        if (len > 0 && tmp[0] == '/') {
            if (strstr(tmp, "/usr/share/zoneinfo/") == tmp) {
                if (nBufferSize < len)
                    return len;
                strncpy(lpBuffer, tmp + 20, len);
            }
            return len;
        }

        // try with "/etc/TZ"
        INT fd = open("/etc/TZ", O_RDONLY);
        if (fd != -1) {
            len = read(fd, tmp, 1024);
            close(fd);
            if (len > 0) {
                if (tmp[len - 1] == '\n')
                    len -= 1;
                if (nBufferSize < len)
                    return len;
                strncpy(lpBuffer, tmp, len);
                return len;
            }
        }

        // try with "/etc/timezone"
        fd = open("/etc/timezone", O_RDONLY);
        if (fd != -1) {
            len = read(fd, tmp, 1024);
            close(fd);
            if (len > 0) {
                if (tmp[len - 1] == '\n')
                    len -= 1;
                if (nBufferSize < len)
                    return len;
                strncpy(lpBuffer, tmp, len);
                return len;
            }
        }
        return 0;
    }

    TICK_COUNT GetTickCount() {
        timespec ts = {};
        INT result = clock_gettime(CLOCK_MONOTONIC, &ts);
        if (result == -1)
            Error("Unable to obtain current tick count").throws($ftrace());
        TICK_COUNT tick = {};
        tick.dwSeconds = ts.tv_sec;
        tick.dwNano = ts.tv_nsec;
        return tick;
    }

    LONG Seed() {
        timespec ts = {};
        gint result = clock_gettime(CLOCK_REALTIME, &ts);
        if (result == -1)
            Error("Unable to initialized current seed").throws($ftrace());
        LONG seed = ts.tv_sec * LocalTime::NANOS_PER_SECOND;
        if (seed < 0)
            seed -= ts.tv_nsec;
        else
            seed += ts.tv_nsec;
        return seed;
    }

    LPVOID GetFileHandle(INT lpFileDescriptor) {
        switch (lpFileDescriptor) {
            case -1:
                return NULL;
            case 0:
                return stdin;
            case 1:
                return stdout;
            case 2:
                return stderr;
            default:
                return fdopen(lpFileDescriptor, "r");
        }
    }

    SIZE_T GetFileAttributes(LPCSTR lpFileName) {
        struct stat s = {};
        INT result = lstat(lpFileName, &s);
        if (result != 0)
            return 0L;
        SIZE_T attributes = 0;
        if (S_ISDIR(s.st_mode)) attributes |= FA_DIRECTORY;
        if (S_ISREG(s.st_mode)) attributes |= FA_REGULAR;
        if (S_ISLNK(s.st_mode)) attributes |= FA_SYMLINK;
        if (S_ISBLK(s.st_mode)) attributes |= FA_BLOCK;
        if (S_ISFIFO(s.st_mode)) attributes |= FA_FIFO;
        if (S_ISSOCK(s.st_mode)) attributes |= FA_SOCKET;
        if ((s.st_mode & S_IRUSR) == S_IRUSR) attributes |= FA_OWNER_READ;
        if ((s.st_mode & S_IWUSR) == S_IWUSR) attributes |= FA_OWNER_WRITE;
        if ((s.st_mode & S_IXUSR) == S_IXUSR) attributes |= FA_OWNER_EXECUTE;
        if ((s.st_mode & S_IRGRP) == S_IRGRP) attributes |= FA_GROUP_READ;
        if ((s.st_mode & S_IWGRP) == S_IWGRP) attributes |= FA_GROUP_WRITE;
        if ((s.st_mode & S_IXGRP) == S_IXGRP) attributes |= FA_GROUP_EXECUTE;
        if ((s.st_mode & S_IROTH) == S_IROTH) attributes |= FA_OTHER_READ;
        if ((s.st_mode & S_IWOTH) == S_IWOTH) attributes |= FA_OTHER_WRITE;
        if ((s.st_mode & S_IXOTH) == S_IXOTH) attributes |= FA_OTHER_EXECUTE;
        {
            LPCSTR str = strchr(lpFileName, '/');
            if (str == NULL || str[1] == 0)
                str = lpFileName;
            else
                while (str != NULL) {
                    LPCSTR next = strchr(str + 1, '/');
                    if (next == NULL || next[1] == 0)
                        break;
                    str = next + 1;
                }
            if (str != NULL && str[0] != '.') attributes |= FA_HIDDEN;
        }
        return attributes | FA_EXIST;
    }

    SIZE_T GetFileSize(LPCSTR lpFileName) {
        struct stat s = {};
        INT result = stat(lpFileName, &s);
        if (result != 0)
            return 0L;
        return s.st_size < 0 ? 0L : s.st_size;
    }

    LONG GetLastWriteTime(LPCSTR lpFileName) {
        struct stat s = {};
        INT result = stat(lpFileName, &s);
        if (result != 0)
            return 0L;
        return s.st_mtime;
    }

    LONG GetCreateTime(LPCSTR lpFileName) {
        struct stat s = {};
        INT result = stat(lpFileName, &s);
        if (result != 0)
            return 0L;
        return s.st_ctime;
    }

    LONG GetAccessTime(LPCSTR lpFileName) {
        struct stat s = {};
        INT result = stat(lpFileName, &s);
        if (result != 0)
            return 0L;
        return s.st_atime;
    }

    BOOL CreateFile(LPCSTR lpFileName) {
        gint result = open(lpFileName, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
        if (result == -1)
            return FALSE;
        close(result);
        return TRUE;
    }

    BOOL CreateDirectory(LPCSTR lpFileName) {
        gint result = mkdir(lpFileName, S_IRUSR | S_IWUSR);
        if (result == -1)
            return FALSE;
        return TRUE;
    }

    BOOL DeleteFile(LPCSTR lpFileName) {
        gint result = remove(lpFileName);
        if (result == -1)
            return FALSE;
        return TRUE;
    }

    BOOL CopyFile(LPCSTR lpFileName, LPCSTR lpNewName) {
        return CopyFileEx(lpFileName, lpNewName, COPY_MOVE_DEFAULT_OPTIONS);
    }

    BOOL MoveFile(LPCSTR lpFileName, LPCSTR lpNewName) {
        return MoveFileEx(lpFileName, lpNewName, COPY_MOVE_DEFAULT_OPTIONS);
    }

    BOOL CopyFileEx(LPCSTR lpFileName, LPCSTR lpNewName, DWORD dwFlags) {
        if (strcmp(lpFileName, lpNewName) == 0)
            return FALSE;

        if (CompareFile(lpFileName, lpNewName) == 0)
            return FALSE;

        errno = 0;
        DWORD fd1 = 0;
        DWORD fd2 = 0;

        FILE *ff1 = fopen(lpFileName, "r");
        FILE *ff2 = fopen(lpNewName, "w+");

        fd1 = fileno(ff1);
        fd2 = fileno(ff2);

        CHAR lpBuffer[8192] = {};
        DWORD dwBytesRead = 0;
        DWORD n = 0;
        while ((n = read(fd1, lpBuffer, 8192)) != (DWORD) -1) {
            if (n == 0)
                break;
            gint r = 0;
            do {
                errno = 0;
                r = write(fd2, lpBuffer + r, n - r);
                if (r == -1) {
                    fclose(ff1);
                    fclose(ff2);
                    return FALSE;
                }
                dwBytesRead += r;
                n -= r;
            } while (n > 0);
        }
        close(fd1);
        close(fd2);
        return dwBytesRead > 0;
    }

    BOOL MoveFileEx(LPCSTR lpFileName, LPCSTR lpNewName, DWORD dwFlags) {
        DWORD flags = 0;
        if (dwFlags & COPY_MOVE_REPLACE_EXISTING) flags |= RENAME_EXCHANGE;
        else flags |= RENAME_NOREPLACE;
        if (dwFlags & MOVE_ATOMIC_MOVE) flags |= RENAME_WHITEOUT;
        if (strcmp(lpFileName, lpNewName) == 0)
            return FALSE;
        if (CompareFileEx(lpFileName, lpNewName, FALSE) == 0) {
            DWORD r = rename(lpFileName, lpNewName);
            DWORD err = errno;
            if (r != -1)
                return TRUE;
        }
        if (GetFileAttributes(lpFileName) & FA_DIRECTORY) {
            DWORD r = rename(lpFileName, lpNewName);
            DWORD err = errno;
            if (r != -1)
                return TRUE;
        }
        if (dwFlags & MOVE_MOVE_BY_COPY) {
            FILE * dst = fopen(lpNewName, "w");
            if (dst == null)
                return FALSE;
            FILE * src = fopen(lpFileName, "r");
            if (src == null){
                fclose(dst);
                return FALSE;
            }
            DWORD f1 = fileno(dst);
            DWORD f2 = fileno(src);
            CHAR lpBuffer[8192] = {};
            DWORD nr = 0;
            DWORD nw = 0;
            DWORD nrw = 0;
            while ((nr = read(f2, lpBuffer, 8192)) != -1) {
                nw = 0;
                do {
                    nw = write(f1, lpBuffer + nw, nr);
                    if (nw == -1) {
                        fclose(src);
                        fclose(dst);
                        return FALSE;
                    }
                    nr -= nw;
                    nrw += nw;
                } while (nr > 0);
            }
            fclose(src);
            fclose(dst);
            return nrw > 0;
        } else {
            File src = File(lpFileName).absoluteFile();
            File dst = File(lpNewName).absoluteFile();

            DWORD parentFD1 = -1, parentFD2 = -1;
            {
                File parent1 = src.parentFile();
                if (!parent1.exists())
                    return false;
                File parent2 = dst.parentFile();
                if (!parent2.exists())
                    return false;

                ByteArray path1 = parent1.path().toBytes();
                ByteArray path2 = parent2.path().toBytes();

                LPCSTR lpDir1 = (LPCSTR) &path1[0];
                LPCSTR lpDir2 = (LPCSTR) &path2[0];

                // try to open parents
                parentFD1 = open(lpDir1, O_DIRECTORY| O_RDONLY | O_NOFOLLOW, 0666);
                if (parentFD1 == -1)
                    return FALSE;
                parentFD2 = open(lpDir2, O_DIRECTORY| O_RDONLY | O_NOFOLLOW, 0666);
                if (parentFD2 == -1){
                    close(parentFD1);
                    return FALSE;
                }
            }

            DWORD r = renameat2(parentFD1, lpFileName, parentFD2, lpNewName, flags);
            if (r == -1) {
                DWORD error = errno;
                if (error == ENOENT) {
                    if (src.isDirectory()) {
                        if (!dst.exists())
                            dst.createDirectory();
                    } else {
                        if (!dst.exists())
                            dst.createFile();
                    }
                }
                r = renameat2(parentFD1, lpFileName, parentFD2, lpNewName, flags);
                error = errno;
            }
            return r == 0;
        }
    }

    LPCSTR RealPath(LPCSTR lpFileName) {
        LPCSTR result = realpath(lpFileName, NULL);
        if (result == NULL)
            return EMPTY_STRING;
        return result;
    }

    BOOL CreateLink(LPCSTR lpFileName, LPCSTR lpExistingDirectory) {
        return link(lpExistingDirectory, lpFileName) == 0;
    }

    BOOL CreateSymbolicLink(LPCSTR lpFileName, LPCSTR lpExistingFile) {
        return symlink(lpExistingFile, lpFileName) == 0;
    }

    BOOL CreateShortCut(LPCSTR lpFileName, LPCSTR lpExistingFile) {
        return FALSE;
    }

    DWORD ReadLink(LPCSTR lpFileName, LPSTR lpBuffer, DWORD nBufferSize) {
        return readlink(lpFileName, lpBuffer, nBufferSize);
    }

    DWORD FileAttributesToPermission(DWORD dwFileAttributes) {
        DWORD dwMode = 0;
        if ((dwFileAttributes & FA_OWNER_READ) == FA_OWNER_READ) dwMode |= 0400;
        if ((dwFileAttributes & FA_OWNER_WRITE) == FA_OWNER_WRITE) dwMode |= 0200;
        if ((dwFileAttributes & FA_OWNER_EXECUTE) == FA_OWNER_EXECUTE) dwMode |= 0100;

        if ((dwFileAttributes & FA_GROUP_READ) == FA_GROUP_READ) dwMode |= 0040;
        if ((dwFileAttributes & FA_GROUP_WRITE) == FA_GROUP_WRITE) dwMode |= 0020;
        if ((dwFileAttributes & FA_GROUP_EXECUTE) == FA_GROUP_EXECUTE) dwMode |= 0010;

        if ((dwFileAttributes & FA_OTHER_READ) == FA_OTHER_READ) dwMode |= 0004;
        if ((dwFileAttributes & FA_OTHER_WRITE) == FA_OTHER_WRITE) dwMode |= 0002;
        if ((dwFileAttributes & FA_OTHER_EXECUTE) == FA_OTHER_EXECUTE) dwMode |= 0001;

        return dwMode;
    }

    BOOL SetPermission(LPCSTR lpFileName, DWORD dwMode) {
        return lchmod(lpFileName, dwMode) == 0;
    }

    BOOL ReadDirectory(LPCSTR lpFileName, LPFILE_ENTRY lpFileEntry) {
        DIR *dir = opendir(lpFileName);
        if (dir == NULL)
            return FALSE;
        dirent *entry = readdir(dir);
        if (entry == NULL)
            return FALSE;
        lpFileEntry->lpFileName = entry->d_name;
        lpFileEntry->dwInode = entry->d_ino;
        lpFileEntry->dwOffset = entry->d_off;
        lpFileEntry->lpIterator = dir;
        return TRUE;
    }

    BOOL NextEntry(LPFILE_ENTRY lpFileEntry) {
        if (lpFileEntry == NULL)
            return FALSE;
        DIR *dir = (DIR *) lpFileEntry->lpIterator;
        dirent *entry = readdir(dir);
        if (entry == NULL)
            return FALSE;
        lpFileEntry->lpFileName = entry->d_name;
        lpFileEntry->dwInode = entry->d_ino;
        lpFileEntry->dwOffset = entry->d_off;
        lpFileEntry->lpIterator = dir;
        return TRUE;
    }

    VOID CloseEntry(LPFILE_ENTRY lpFileEntry) {
        if (lpFileEntry != NULL) {
            DIR *dir = (DIR *) lpFileEntry->lpIterator;
            closedir(dir);
        }
    }

    INT CompareFile(LPCSTR lpFileName1, LPCSTR lpFileName2) {
        return CompareFileEx(lpFileName1, lpFileName2, TRUE);
    }

    INT CompareFileEx(LPCSTR lpFileName1, LPCSTR lpFileName2, BOOL dwFollowLink) {
        if (strcmp(lpFileName1, lpFileName2) == 0)
            return 0;
        struct stat st1 = {};
        struct stat st2 = {};
        DWORD r1 = (dwFollowLink ? lstat : stat)(lpFileName1, &st1);
        if (r1 == -1)
            return 1;
        DWORD r2 = (dwFollowLink ? lstat : stat)(lpFileName2, &st2);
        if (r2 == -1)
            return -1;
        return st1.st_ino - st2.st_ino;
    }
} // core
