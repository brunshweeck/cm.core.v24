//
// Created by brunshweeck on 16 sept. 2024.
//

#ifndef UNICODE
#define UNICODE
#endif

#include <AclAPI.h> // GetNamedSecurityInfo
#include <Windows.h>
#include <ShlObj.h>
#include <core/StringArray.h>
#include <core/XString.h>
#include <core/io/FileArray.h>
#include <core/io/WinNTFileSystem.h>
#include <core/misc/Unsafe.h>
#include <core/time/LocalDateTime.h>
#include <core/util/ArrayList.h>

namespace core {
    namespace {
        typedef struct _REPARSE_DATA_BUFFER {
            ULONG ReparseTag;
            USHORT ReparseDataLength;
            USHORT Reserved;

            union {
                struct {
                    USHORT SubstituteNameOffset;
                    USHORT SubstituteNameLength;
                    USHORT PrintNameOffset;
                    USHORT PrintNameLength;
                    ULONG Flags;
                    WCHAR PathBuffer[1];
                } SymbolicLinkReparseBuffer;

                struct {
                    USHORT SubstituteNameOffset;
                    USHORT SubstituteNameLength;
                    USHORT PrintNameOffset;
                    USHORT PrintNameLength;
                    WCHAR PathBuffer[1];
                } MountPointReparseBuffer;

                struct {
                    UCHAR DataBuffer[1];
                } GenericReparseBuffer;
            };
        } REPARSE_DATA_BUFFER, *PREPARSE_DATA_BUFFER;
    }

    namespace io {
        String const WinNTFileSystem::LONG_PATH_PREFIX = "\\?\\"_S;

        WinNTFileSystem::WinNTFileSystem() {
            slash = UNSAFE::DEFAULT_FILE_SEPARATOR;
            altSlash = (slash == '\\') ? '/' : '\\';
            semicolon = UNSAFE::DEFAULT_PATH_SEPARATOR;
            userDir = normalize(userPath());
        }

        gchar WinNTFileSystem::fileSeparator() {
            return slash;
        }

        gchar WinNTFileSystem::pathSeparator() {
            return semicolon;
        }

        String WinNTFileSystem::normalize(const String &path) {
            String p = stripLongOrUNCPrefix(path);
            gint n = p.length();
            gchar slash = WinNTFileSystem::slash;
            gchar altSlash = WinNTFileSystem::altSlash;
            gchar prev = 0;
            for (gint i = 0; i < n; i++) {
                gchar c = p.charAt(i);
                if (c == altSlash)
                    return normalize(p, n, (prev == slash) ? i - 1 : i);
                if ((c == slash) && (prev == slash) && (i > 1))
                    return normalize(p, n, i - 1);
                if ((c == ':') && (i > 1))
                    return normalize(p, n, 0);
                prev = c;
            }
            if (prev == slash) return normalize(p, n, n - 1);
            return p;
        }

        gint WinNTFileSystem::prefixLength(String const &path) {
            CORE_ASSERT(!path.startsWith(LONG_PATH_PREFIX));

            gchar slash = WinNTFileSystem::slash;
            gint n = path.length();
            if (n == 0) return 0;
            gchar c0 = path.charAt(0);
            gchar c1 = (n > 1) ? path.charAt(1) : 0;
            if (c0 == slash) {
                if (c1 == slash) return 2; /* Absolute UNC pathname "\\\\foo" */
                return 1; /* Drive-relative "\\foo" */
            }
            if (isLetter(c0) && (c1 == ':')) {
                if ((n > 2) && (path.charAt(2) == slash))
                    return 3; /* Absolute local pathname "z:\\foo" */
                return 2; /* Directory-relative "z:foo" */
            }
            return 0; /* Completely relative */
        }

        String WinNTFileSystem::resolve(const String &parent, const String &child) {
            CORE_ASSERT(!child.startsWith(LONG_PATH_PREFIX));

            gint pn = parent.length();
            if (pn == 0) return child;
            gint cn = child.length();
            if (cn == 0) return parent;

            const String &c = child;
            gint childStart = 0;
            gint parentEnd = pn;

            gbool isDirectoryRelative =
                    pn == 2 && isLetter(parent.charAt(0)) && parent.charAt(1) == ':';

            if ((cn > 1) && (c.charAt(0) == slash)) {
                if (c.charAt(1) == slash) {
                    /* Drop prefix when child is a UNC pathname */
                    childStart = 2;
                } else if (!isDirectoryRelative) {
                    /* Drop prefix when child is drive-relative */
                    childStart = 1;
                }
                if (cn == childStart) {
                    // Child is double slash
                    if (parent.charAt(pn - 1) == slash)
                        return parent.subString(0, pn - 1);
                    return parent;
                }
            }

            if (parent.charAt(pn - 1) == slash)
                parentEnd--;

            gint strlen = parentEnd + cn - childStart;
            CharArray theChars;
            if (child.charAt(childStart) == slash || isDirectoryRelative) {
                theChars = CharArray(strlen);
                parent.toChars(0, parentEnd, theChars, 0);
                child.toChars(childStart, cn, theChars, parentEnd);
            } else {
                theChars = CharArray(strlen + 1);
                parent.toChars(0, parentEnd, theChars, 0);
                theChars[parentEnd] = slash;
                child.toChars(childStart, cn, theChars, parentEnd + 1);
            }

            // if present, strip trailing name separator unless after a ':'
            if (theChars.length() > 1
                && theChars[theChars.length() - 1] == slash
                && theChars[theChars.length() - 2] != ':')
                return String(theChars, 0, theChars.length() - 1);

            return String(theChars);
        }

        String WinNTFileSystem::defaultParent() { return ""_S + slash; }

        String WinNTFileSystem::fromURIPath(const String &path) {
            String p = path;
            if ((p.length() > 2) && (p.charAt(2) == ':')) {
                // "/c:/foo" --> "c:/foo"
                p = p.subString(1);
                // "c:/foo/" --> "c:/foo", but "c:/" --> "c:/"
                if ((p.length() > 3) && p.endsWith("/"_S))
                    p = p.subString(0, p.length() - 1);
            } else if ((p.length() > 1) && p.endsWith("/"_S)) {
                // "/foo/" --> "/foo"
                p = p.subString(0, p.length() - 1);
            }
            return p;
        }

        gbool WinNTFileSystem::isAbsolute(const File &f) {
            String path = f.path();
            CORE_ASSERT(!path.startsWith(LONG_PATH_PREFIX));

            gint pl = prefixLength(path);
            return (((pl == 2) && (f.path().charAt(0) == slash))
                    || (pl == 3));
        }

        namespace {
            enum class PathType : gbyte {
                PT_ABSOLUTE, //  C:\foo
                PT_UNC, //  \\server\share\foo
                PT_RELATIVE, //  foo
                PT_DIRECTORY_RELATIVE, //  \foo
                PT_DRIVE_RELATIVE, //  C:foo
                PT_INVALID
            };

            CORE_FAST gbool isSlash(gchar ch) {
                return (ch == '\\') || (ch == '/');
            }

            CORE_FAST gbool isLetter(gchar ch) {
                return ('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z');
            }

            int nextNonSlash(String const &path, int off, int end) {
                while (off < end && isSlash(path.charAt(off))) {
                    off++;
                }
                return off;
            }

            gbool isInvalidPathChar(gchar ch) {
                return ch < u'\u0020' ||
                       ch == '<' || ch == '>' ||
                       ch == ':' || ch == '\"' || ch == '|' || ch == '*' || ch == '?';
            }

            int nextSlash(String const &path, int off, int end) {
                gchar c = 0;
                while (off < end && !isSlash(c = path.charAt(off))) {
                    if (isInvalidPathChar(c)) {
                        return -1;
                    }
                    off++;
                }
                return off;
            }

            class ParseResult CORE_FINAL : public Object {
            public:
                PathType type = {};
                String root;
                String path;

                ParseResult() : type(PathType::PT_INVALID) {
                }

                CORE_EXPLICIT ParseResult(PathType type, String root, String path) : type(type),
                    root(misc::Unsafe::moveInstance(root)), path(misc::Unsafe::moveInstance(path)) {
                }

                Object &clone() const override {
                    return misc::Unsafe::newInstance<ParseResult>(*this);
                }

                gbool equals(Object const &obj) const override {
                    if (this == &obj) {
                        return true;
                    }
                    if (!Class<ParseResult>::hasInstance(obj))
                        return false;
                    ParseResult const &result = CORE_XCAST(ParseResult const, obj);
                    if (result.type == PathType::PT_INVALID)
                        return type == PathType::PT_INVALID;
                    return result.type == type && root == result.root && path == result.path;
                }

                String toString() const override {
                    if (type == PathType::PT_INVALID)
                        return "ParseResult[Error]"_S;
                    return "ParseResult[root="_S + root + ", path="_S + path + "]"_S;
                }
            };

            String normalizePath(XString &sb, String const &path, gint off) {
                int const len = path.length();
                off = nextNonSlash(path, off, len);
                int start = off;
                gchar lastC = 0;
                while (off < len) {
                    gchar const c = path.charAt(off);
                    if (isSlash(c)) {
                        if (lastC == ' ') {
                            // Trailing char is diskSpace character
                            return {};
                        }
                        sb.append(path, start, off);
                        off = nextNonSlash(path, off, len);
                        if (off != len) //no slash at the end of normalized path
                            sb.append('\\');
                        start = off;
                    } else {
                        if (isInvalidPathChar(c)) {
                            // Invalid char in pathname
                            return {};
                        }
                        lastC = c;
                        off++;
                    }
                }
                if (start != off) {
                    if (lastC == ' ') {
                        // Trailing char is diskSpace character
                        return {};
                    }
                    sb.append(path, start, off);
                }
                return sb.toString();
            }

            ParseResult parsePath(String const &path, gbool requireNormalization) {
                String root = {};
                PathType type = PathType::PT_INVALID;
                String const &input = path;

                int const len = input.length();
                int off = 0;
                if (len > 1) {
                    gchar const c0 = input.charAt(0);
                    gchar const c1 = input.charAt(1);
                    // gchar const c = 0;
                    if (isSlash(c0) && isSlash(c1)) {
                        int next = 2;
                        // UNC: We keep the first two slashes, collapse all the
                        // following, then take the hostname and share name out,
                        // meanwhile collapsing all the redundant slashes.
                        type = PathType::PT_UNC;
                        off = nextNonSlash(input, next, len);
                        next = nextSlash(input, off, len);
                        if (off == next || next == -1) {
                            // UNC path is missing hostname
                            return {};
                        }
                        String const host = input.subString(off, next); //host
                        off = nextNonSlash(input, next, len);
                        next = nextSlash(input, off, len);
                        if (off == next || next == -1) {
                            // UNC path is missing share-name
                            return {};
                        }
                        root = R"(\\)"_S + host + R"(\)"_S + input.subString(off, next) + R"(\)"_S;
                        off = next;
                    } else {
                        if (isLetter(c0) && c1 == ':') {
                            gchar c2 = 0;
                            if (len > 2 && isSlash(c2 = input.charAt(2))) {
                                // avoid concatenation when root is "D:\"
                                if (c2 == '\\') {
                                    root = input.subString(0, 3);
                                } else {
                                    root = input.subString(0, 2) + R"(\)"_S;
                                }
                                off = 3;
                                type = PathType::PT_ABSOLUTE;
                            } else {
                                root = input.subString(0, 2);
                                off = 2;
                                type = PathType::PT_DRIVE_RELATIVE;
                            }
                        }
                    }
                }
                if (off == 0) {
                    if (len > 0 && isSlash(input.charAt(0))) {
                        type = PathType::PT_DIRECTORY_RELATIVE;
                        root = R"(\)"_S;
                    } else {
                        type = PathType::PT_RELATIVE;
                    }
                }

                if (requireNormalization) {
                    XString sb = XString(input.length());
                    sb.append(root);
                    const String normalized = normalizePath(sb, input, off);
                    if (normalized.isEmpty() && !root.isEmpty()) {
                        return {};
                    } else
                        return ParseResult(type, root, normalized);
                }
                return ParseResult(type, root, input);
            }
        }

        gbool WinNTFileSystem::isInvalid(const File &f) {
            if (f.path().indexOf(u'\u0000') >= 0)
                return true;

            // Invalid if there is a ":" at a position greater than 1, or if there
            // is a ":" at position 1 and the first character is not a letter
            String pathname = f.path();
            gint lastColon = pathname.lastIndexOf(":"_S);

            // Valid if there is no ":" present or if the last ":" present is
            // at index 1 and the first character is a latter
            if (lastColon < 0 ||
                (lastColon == 1 && isLetter(pathname.charAt(0))))
                return false;

            // Invalid if path creation fails
            ParseResult pr = parsePath(pathname, pathname.indexOf(altSlash) >= 0);

            return pr.type == PathType::PT_INVALID;
        }

        String WinNTFileSystem::resolve(const File &f) {
            String path = f.path();
            CORE_ASSERT(!path.startsWith(LONG_PATH_PREFIX));

            gint pl = prefixLength(path);
            if ((pl == 2) && (path.charAt(0) == slash))
                return path; /* UNC */
            if (pl == 3)
                return path; /* Absolute local */
            if (pl == 0)
                return userPath() + slashify(path); /* Completely relative */
            if (pl == 1) {
                /* Drive-relative */
                String up = userPath();
                String ud = drive(up);
                if (!ud.isEmpty()) return ud + path;
                return up + path; /* User dir is a UNC path */
            }
            if (pl == 2) {
                /* Directory-relative */
                String up = userPath();
                String ud = drive(up);
                if ((!ud.isEmpty()) && path.startsWith(ud))
                    return up + slashify(path.subString(2));
                gchar drive = path.charAt(0);
                String dir = driveDirectory(drive);
                if (!dir.isEmpty()) {
                    /* When resolving a directory-relative path that refers to a
                       drive other than the current drive, insist that the caller
                       have read permission on the result */
                    String p = drive + (':' + dir + slashify(path.subString(2)));
                    return p;
                }
                return drive + ":" + slashify(path.subString(2)); /* fake it */
            }
            throw Error("Unresolvable path: " + path);
        }

        String WinNTFileSystem::canonicalize(const String &path) {
            CORE_ASSERT(!path.startsWith(LONG_PATH_PREFIX));

            // If path is a drive letter only then skip canonicalization
            gint len = path.length();
            if ((len == 2) &&
                (isLetter(path.charAt(0))) &&
                (path.charAt(1) == ':')) {
                gchar c = path.charAt(0);
                if ((c >= 'A') && (c <= 'Z'))
                    return path;
                return String(CharArray::of(CORE_CAST(gchar, c - 32), u':'));
            } else if ((len == 3) &&
                       (isLetter(path.charAt(0))) &&
                       (path.charAt(1) == ':') &&
                       (path.charAt(2) == '\\')) {
                gchar c = path.charAt(0);
                if ((c >= 'A') && (c <= 'Z'))
                    return path;
                return String(CharArray::of(CORE_CAST(gchar, c - 32), u':', u'\\'));
            }
            return realPath(path);
        }

        gint WinNTFileSystem::booleanAttributes(const File &f) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = f.path().toChars();
            if (ca.isEmpty())
                return 0;
            glong attr = GetFileAttributes(CORE_CAST(LPWSTR, &ca[0]));
            err.lazySet(GetLastError());
            if (attr == INVALID_FILE_ATTRIBUTES) return 0;

            gint r = BA_EXISTS;
            if (attr & FILE_ATTRIBUTE_NORMAL) r |= BA_REGULAR;
            if (attr & FILE_ATTRIBUTE_DIRECTORY) r |= BA_DIRECTORY;
            if (attr & FILE_ATTRIBUTE_HIDDEN) r |= BA_HIDDEN;
            if (attr & FILE_ATTRIBUTE_REPARSE_POINT) r |= WA_REPARSE_POINT;
            if (attr & FILE_ATTRIBUTE_SYSTEM) r |= WA_SYSTEM;
            if (attr & FILE_ATTRIBUTE_ARCHIVE) r |= WA_ARCHIVE;
            if (attr & FILE_ATTRIBUTE_DEVICE) r |= WA_DEVICE;
            if (attr & FILE_ATTRIBUTE_READONLY) r |= WA_READONLY;
            if (attr & FILE_ATTRIBUTE_TEMPORARY) r |= WA_TEMPORARY;
            if (attr & FILE_ATTRIBUTE_SPARSE_FILE) r |= WA_SPARSE_FILE;
            if (attr & FILE_ATTRIBUTE_COMPRESSED) r |= WA_COMPRESSED;
            if (attr & FILE_ATTRIBUTE_OFFLINE) r |= WA_OFFLINE;
            if (attr & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) r |= WA_NOT_CONTENT_INDEXED;
            if (attr & FILE_ATTRIBUTE_ENCRYPTED) r |= WA_ENCRYPTED;
            if (attr & FILE_ATTRIBUTE_INTEGRITY_STREAM) r |= WA_INTEGRITY_STREAM;
            if (attr & FILE_ATTRIBUTE_VIRTUAL) r |= WA_VIRTUAL;
            if (attr & FILE_ATTRIBUTE_EA) r |= WA_EA;
            if (attr & FILE_ATTRIBUTE_PINNED) r |= WA_PINNED;
            if (attr & FILE_ATTRIBUTE_RECALL_ON_OPEN) r |= WA_RECALL_ON_OPEN;
            if (attr & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS) r |= WA_RECALL_ON_DATA_ACCESS;
            if (attr & FILE_ATTRIBUTE_STRICTLY_SEQUENTIAL) r |= WA_STRICTLY_SEQUENTIAL;

            if (r & WA_REPARSE_POINT) {
                // Check if is symbolic link
                HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]),
                                           GENERIC_READ,
                                           FILE_SHARE_READ,
                                           NULL,
                                           OPEN_EXISTING,
                                           FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
                                           NULL);
                if (handle == INVALID_HANDLE_VALUE) return {};

                ByteArray ba = ByteArray(MAXIMUM_REPARSE_DATA_BUFFER_SIZE);
                DWORD nBytes = 0;
                DWORD r2 = DeviceIoControl(handle,
                                           FSCTL_GET_REPARSE_POINT,
                                           NULL, 0,
                                           CORE_CAST(LPVOID, &ba[0]),
                                           ba.length(),
                                           &nBytes,
                                           NULL);
                CloseHandle(handle);
                if (r2 == 0) return r;

                PREPARSE_DATA_BUFFER rdb = CORE_CAST(PREPARSE_DATA_BUFFER, &ba[0]);
                if (rdb->ReparseTag == IO_REPARSE_TAG_SYMLINK) r |= BA_SYMLINK;
            }

            return r;
        }

        gbool WinNTFileSystem::checkAccess(const File &f, gint access) {
            err.lazySet(ERROR_SUCCESS);

            if (!checkOwnerAccess(f, access)) {
                DWORD r = 0;

                if (access & ACCESS_READ) r |= GENERIC_READ;
                if (access & ACCESS_WRITE) r |= GENERIC_WRITE;
                if (access & ACCESS_EXECUTE) r |= GENERIC_EXECUTE;

                CharArray ca = f.path().toChars();
                if (ca.isEmpty()) return false;

                HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]), r, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                                           FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED,
                                           NULL);
                DWORD r2 = GetLastError();
                err.lazySet(r2);
                if (handle == INVALID_HANDLE_VALUE) {
                    if (r2 == ERROR_FILE_NOT_FOUND || r2 == ERROR_NOT_FOUND)
                        return false;
                } else {
                    CloseHandle(handle);
                    return true;
                }
                return false;
            }

            return true;
        }

        glong WinNTFileSystem::lastModifiedTime(const File &f) {
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return 0;

            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]), GENERIC_READ, FILE_SHARE_READ, NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED, NULL);
            err.lazySet(GetLastError());
            if (handle == INVALID_HANDLE_VALUE) {
                return 0LL;
            }

            FILETIME ft;
            gint r = GetFileTime(handle, NULL, NULL, &ft);
            err.lazySet(GetLastError());
            CloseHandle(handle);
            if (r != 0) return 0LL;

            SYSTEMTIME st;
            FileTimeToSystemTime(&ft, &st);

            time::LocalDateTime ldt = time::LocalDateTime(
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond,
                CORE_CAST(gint, st.wMilliseconds * time::LocalTime::NANOS_PER_MILLI));

            return ldt.toLocalDate().toEpochDay() * time::LocalTime::MILLIS_PER_DAY
                   + ldt.toLocalTime().toNanoOfDay() / time::LocalTime::NANOS_PER_MILLI;
        }

        glong WinNTFileSystem::lastAccessTime(const File &f) {
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return 0;

            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]), GENERIC_READ, FILE_SHARE_READ, NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED, NULL);
            err.lazySet(GetLastError());
            if (handle == INVALID_HANDLE_VALUE) {
                return 0LL;
            }

            FILETIME ft;
            gint r = GetFileTime(handle, NULL, &ft, NULL);
            err.lazySet(GetLastError());
            CloseHandle(handle);
            if (r != 0) return 0LL;

            SYSTEMTIME st;
            FileTimeToSystemTime(&ft, &st);

            time::LocalDateTime ldt = time::LocalDateTime(
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond,
                CORE_CAST(gint, st.wMilliseconds * time::LocalTime::NANOS_PER_MILLI));

            return ldt.toLocalDate().toEpochDay() * time::LocalTime::MILLIS_PER_DAY
                   + ldt.toLocalTime().toNanoOfDay() / time::LocalTime::NANOS_PER_MILLI;
        }

        glong WinNTFileSystem::creationTime(const File &f) {
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return 0;

            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]), GENERIC_READ, FILE_SHARE_READ, NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED, NULL);
            err.lazySet(GetLastError());
            if (handle == INVALID_HANDLE_VALUE) {
                return 0LL;
            }

            FILETIME ft;
            gint r = GetFileTime(handle, &ft, NULL, NULL);
            err.lazySet(GetLastError());
            CloseHandle(handle);
            if (r != 0) return 0LL;

            SYSTEMTIME st;
            FileTimeToSystemTime(&ft, &st);

            time::LocalDateTime ldt = time::LocalDateTime(
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond,
                CORE_CAST(gint, st.wMilliseconds * time::LocalTime::NANOS_PER_MILLI));

            return ldt.toLocalDate().toEpochDay() * time::LocalTime::MILLIS_PER_DAY
                   + ldt.toLocalTime().toNanoOfDay() / time::LocalTime::NANOS_PER_MILLI;
        }

        glong WinNTFileSystem::length(const File &f) {
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return 0;

            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]), GENERIC_READ, FILE_SHARE_READ, NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED, NULL);
            err.lazySet(GetLastError());
            if (handle == INVALID_HANDLE_VALUE) {
                return 0LL;
            }

            LARGE_INTEGER li;
            gint r = GetFileSizeEx(CORE_CAST(LPWSTR, &ca[0]), &li);
            err.lazySet(GetLastError());
            CloseHandle(handle);

            if (r == 0) return 0LL;

            return li.QuadPart;
        }

        gbool WinNTFileSystem::setPermission(const File &f, gint access, gbool enable, gbool ownerOnly) {
            if (ownerOnly) return setOwnerAccess(f, access, enable);

            return setEveryoneAccess(f, access, enable);
        }

        gint WinNTFileSystem::posixPermission(const File &f) {
            return getUnixAccess(f.path());
        }

        gbool WinNTFileSystem::setPosixPermission(const File &f, gint perms) {
            return setUnixAccess(f.path(), perms & 0777);
        }

        gbool WinNTFileSystem::createFileExclusively(const String &pathname) {
            err.lazySet(ERROR_SUCCESS);
            if (pathname.isEmpty()) return false;

            CharArray ca = pathname.toChars();
            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]),
                                       GENERIC_READ | GENERIC_WRITE,
                                       FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                                       NULL,
                                       CREATE_NEW,
                                       FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
                                       NULL);

            err.lazySet(GetLastError());

            if (handle != INVALID_HANDLE_VALUE) {
                CloseHandle(handle);
                return true;
            }

            return false;
        }

        gbool WinNTFileSystem::createDirectory(const String &pathname) {
            err.lazySet(ERROR_SUCCESS);
            if (pathname.isEmpty()) return false;

            CharArray ca = pathname.toChars();

            gint r = CreateDirectory(CORE_CAST(LPWSTR, &ca[0]), NULL);
            err.lazySet(GetLastError());

            return r != 0;
        }

        gbool WinNTFileSystem::markAsTemporary(const String &pathname) {
            err.lazySet(ERROR_SUCCESS);
            if (pathname.isEmpty()) return false;

            CharArray ca = pathname.toChars();
            glong attr = GetFileAttributes(CORE_CAST(LPWSTR, &ca[0]));
            err.lazySet(GetLastError());
            if (attr == INVALID_FILE_ATTRIBUTES) return false;

            if ((attr & FILE_ATTRIBUTE_TEMPORARY) == FILE_ATTRIBUTE_TEMPORARY) return true;

            attr |= FILE_ATTRIBUTE_TEMPORARY;

            gint r = SetFileAttributes(CORE_CAST(LPWSTR, &ca[0]), attr);
            err.lazySet(GetLastError());

            return r != 0;
        }

        StringArray WinNTFileSystem::children(const File &f) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return {};

            WIN32_FIND_DATA fd;
            HANDLE handle = FindFirstFile(CORE_CAST(LPWSTR, &ca[0]), &fd);
            err.lazySet(GetLastError());

            if (handle == INVALID_HANDLE_VALUE) return {};
            util::ArrayList<String> al = {};
            do {
                if (fd.cFileName != NULL && fd.cFileName[0] != 0)
                    al.add(fd.cFileName);
                else
                    al.add(fd.cAlternateFileName);
            } while (FindNextFile(handle, &fd));

            StringArray r = StringArray::copyOf(al.toArray());
            for (int i = 0, n = al.size(); i < n; ++i) {
                UNSAFE::deleteInstance(al.get(i));
            }
            return r;
        }

        gbool WinNTFileSystem::setLastModifiedTime(const File &f, glong time) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;

            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]), GENERIC_READ, FILE_SHARE_READ, NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED, NULL);
            err.lazySet(GetLastError());
            if (handle == INVALID_HANDLE_VALUE) return false;

            SYSTEMTIME st;
            FILETIME ft;

            try {
                time::LocalDateTime ldt = time::LocalDateTime::ofEpochSecond(
                    Math::floorDiv(time, time::LocalTime::NANOS_PER_SECOND),
                    CORE_CAST(gint, Math::floorMod(time, time::LocalTime::NANOS_PER_SECOND)));
                st.wYear = ldt.year();
                st.wMonth = ldt.month();
                st.wDay = ldt.dayOfMonth();
                st.wHour = ldt.hour();
                st.wMinute = ldt.minute();
                st.wSecond = ldt.second();
                st.wMilliseconds = ldt.nano() / time::LocalTime::NANOS_PER_MILLI;

                if (!SystemTimeToFileTime(&st, &ft)) {
                    err.lazySet(GetLastError());
                    CloseHandle(handle);
                    return false;
                }
            } catch (Throwable const &) {
                ft.dwLowDateTime = time & 0xFFFFFFFF;
                ft.dwHighDateTime = time >> 32;
            }

            if (!SetFileTime(handle, NULL, NULL, &ft)) {
                err.lazySet(GetLastError());
                return false;
            }

            CloseHandle(handle);
            return true;
        }

        gbool WinNTFileSystem::setLastAccessTime(const File &f, glong time) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;

            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]), GENERIC_READ, FILE_SHARE_READ, NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED, NULL);
            err.lazySet(GetLastError());
            if (handle == INVALID_HANDLE_VALUE) return false;

            SYSTEMTIME st;
            FILETIME ft;

            try {
                time::LocalDateTime ldt = time::LocalDateTime::ofEpochSecond(
                    Math::floorDiv(time, time::LocalTime::NANOS_PER_SECOND),
                    CORE_CAST(gint, Math::floorMod(time, time::LocalTime::NANOS_PER_SECOND)));
                st.wYear = ldt.year();
                st.wMonth = ldt.month();
                st.wDay = ldt.dayOfMonth();
                st.wHour = ldt.hour();
                st.wMinute = ldt.minute();
                st.wSecond = ldt.second();
                st.wMilliseconds = ldt.nano() / time::LocalTime::NANOS_PER_MILLI;

                if (!SystemTimeToFileTime(&st, &ft)) {
                    err.lazySet(GetLastError());
                    CloseHandle(handle);
                    return false;
                }
            } catch (Throwable const &) {
                ft.dwLowDateTime = time & 0xFFFFFFFF;
                ft.dwHighDateTime = time >> 32;
            }

            if (!SetFileTime(handle, NULL, &ft, NULL)) {
                err.lazySet(GetLastError());
                return false;
            }

            CloseHandle(handle);
            return true;
        }

        gbool WinNTFileSystem::setCreationTime(const File &f, glong time) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;

            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]), GENERIC_READ, FILE_SHARE_READ, NULL,
                                       OPEN_EXISTING,
                                       FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY | FILE_FLAG_OVERLAPPED, NULL);
            err.lazySet(GetLastError());
            if (handle == INVALID_HANDLE_VALUE) return false;

            SYSTEMTIME st;
            FILETIME ft;

            try {
                time::LocalDateTime ldt = time::LocalDateTime::ofEpochSecond(
                    Math::floorDiv(time, time::LocalTime::NANOS_PER_SECOND),
                    CORE_CAST(gint, Math::floorMod(time, time::LocalTime::NANOS_PER_SECOND)));
                st.wYear = ldt.year();
                st.wMonth = ldt.month();
                st.wDay = ldt.dayOfMonth();
                st.wHour = ldt.hour();
                st.wMinute = ldt.minute();
                st.wSecond = ldt.second();
                st.wMilliseconds = ldt.nano() / time::LocalTime::NANOS_PER_MILLI;

                if (!SystemTimeToFileTime(&st, &ft)) {
                    err.lazySet(GetLastError());
                    CloseHandle(handle);
                    return false;
                }
            } catch (Throwable const &) {
                ft.dwLowDateTime = time & 0xFFFFFFFF;
                ft.dwHighDateTime = time >> 32;
            }

            if (!SetFileTime(handle, &ft, NULL, NULL)) {
                err.lazySet(GetLastError());
                return false;
            }

            CloseHandle(handle);
            return true;
        }

        gbool WinNTFileSystem::setReadOnly(const File &f) {
            err.lazySet(ERROR_SUCCESS);

            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;
            glong attr = GetFileAttributes(CORE_CAST(LPWSTR, &ca[0]));
            err.lazySet(GetLastError());
            if (attr == INVALID_FILE_ATTRIBUTES) return false;

            if ((attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY) return true;

            attr |= FILE_ATTRIBUTE_READONLY;

            gint r = SetFileAttributes(CORE_CAST(LPWSTR, &ca[0]), attr);
            err.lazySet(GetLastError());

            return r != 0;
        }

        gbool WinNTFileSystem::deleteFile(const File &f) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;

            glong attr = GetFileAttributes(CORE_CAST(LPWSTR, &ca[0]));
            err.lazySet(GetLastError());
            if (attr == INVALID_FILE_ATTRIBUTES) return false;

            if ((attr & FILE_ATTRIBUTE_DIRECTORY)) {
                gint r = RemoveDirectory(CORE_CAST(LPWSTR, &ca[0]));
                err.lazySet(GetLastError());
                if (r == 0) return false;
            } else {
                gint r = DeleteFile(CORE_CAST(LPWSTR, &ca[0]));
                err.lazySet(GetLastError());
                if (r == 0) return false;
            }

            return true;
        }

        gbool WinNTFileSystem::recycleFile(const File &f) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;

            SHFILEOPSTRUCT fileOp = {NULL};
            fileOp.wFunc = FO_DELETE;
            fileOp.pFrom = CORE_CAST(LPWSTR, &ca[0]);
            fileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;

            gint r = SHFileOperation(&fileOp);
            err.lazySet(GetLastError());

            return r == 0;
        }

        gbool WinNTFileSystem::renameFile(const File &source, const File &target) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca1 = source.path().toChars();
            CharArray ca2 = target.path().toChars();
            if (ca1.isEmpty()) return false;
            if (ca2.isEmpty()) return false;

            DWORD r = MoveFile(CORE_CAST(LPWSTR, &ca1[0]), CORE_CAST(LPWSTR, &ca2[0]));
            err.lazySet(GetLastError());

            return r != 0;
        }

        CORE_ALIAS(CopyOption, File::CopyOption);

        gbool WinNTFileSystem::copyFile(const File &source, const File &target, File::CopyOption options) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca1 = source.path().toChars();
            CharArray ca2 = target.path().toChars();
            if (ca1.isEmpty()) return false;
            if (ca2.isEmpty()) return false;
            DWORD r = 0;

            DWORD flags = COPY_FILE_NO_BUFFERING;

            if ((options & CopyOption::REPLACE_EXISTING) != CopyOption::REPLACE_EXISTING)
                flags |= COPY_FILE_FAIL_IF_EXISTS;
            if ((options & CopyOption::NOFOLLOW_LINKS) == CopyOption::NOFOLLOW_LINKS)
                flags |= COPY_FILE_COPY_SYMLINK;

            r = CopyFileEx(CORE_CAST(LPWSTR, &ca1[0]),CORE_CAST(LPWSTR, &ca2[0]), NULL, NULL, NULL, flags);
            // ERROR_FILE_NOT_FOUND.
            err.lazySet(GetLastError());

            return r != 0;
        }

        gbool WinNTFileSystem::moveFile(const File &source, const File &target, File::CopyOption options) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca1 = source.path().toChars();
            CharArray ca2 = target.path().toChars();
            if (ca1.isEmpty()) return false;
            if (ca2.isEmpty()) return false;
            DWORD r = 0;

            DWORD flag = 0;
            if ((options & CopyOption::REPLACE_EXISTING) == CopyOption::REPLACE_EXISTING)
                flag |= MOVEFILE_REPLACE_EXISTING;
            if ((options & CopyOption::MOVE_BY_COPY) == CopyOption::MOVE_BY_COPY) flag |= MOVEFILE_COPY_ALLOWED;
            if ((options & CopyOption::ATOMIC_MOVE) == CopyOption::MOVE_BY_COPY) flag |= MOVEFILE_FAIL_IF_NOT_TRACKABLE;

            r = MoveFileEx(CORE_CAST(LPWSTR, &ca1[0]), CORE_CAST(LPWSTR, &ca2[0]), flag);
            err.lazySet(GetLastError());

            return r != 0;
        }

        FileArray WinNTFileSystem::roots() {
            err.lazySet(ERROR_SUCCESS);
            DWORD drivesMask = GetLogicalDrives();
            gint nbDrives = 0;
            DWORD r = GetLastError();
            err.lazySet(CORE_CAST(gint, r));

            if (r != ERROR_SUCCESS) {
                for (int i = 0; i < 26; ++i) {
                    if (!DRIVE_DIR_CACHE[i].isEmpty())
                        nbDrives += 1;
                }
            } else {
                for (int i = 0; i < 26; ++i) {
                    if (drivesMask & (1 << i)) {
                        nbDrives += 1;
                        if (DRIVE_DIR_CACHE[i].isEmpty()) {
                            DRIVE_DIR_CACHE[i] = String(CharArray::of('A' + i, ':', slash));
                        }
                    } else {
                        // Drive has been removed.
                        if (!DRIVE_DIR_CACHE[i].isEmpty()) {
                            DRIVE_DIR_CACHE[i] = {};
                        }
                    }
                }
            }

        COPY:
            FileArray files = FileArray(nbDrives);
            for (int i = 0, j = 0; i < 26 && j < nbDrives; ++i) {
                if (!DRIVE_DIR_CACHE[i].isEmpty()) {
                    files[j++] = File(DRIVE_DIR_CACHE[i]);
                }
            }

            return files;
        }

        glong WinNTFileSystem::diskSpace(const File &f, gint type) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return 0LL;

            DWORD r = 0;

            ULARGE_INTEGER total = {}, available = {}, free = {};
            r = GetDiskFreeSpaceEx(CORE_CAST(LPWSTR, &ca[0]), &available, &total, &free);
            err.lazySet(GetLastError());

            if (r != 0) {
                if (type == SPACE_TOTAL)
                    return CORE_CAST(glong, total.QuadPart);
                if (type == SPACE_FREE)
                    return CORE_CAST(glong, free.QuadPart);
                if (type == SPACE_USABLE)
                    return CORE_CAST(glong, available.QuadPart);
            }

            return 0LL;
        }

        gint WinNTFileSystem::maxNameLength(const String &path) {
            err.lazySet(ERROR_SUCCESS);

            gint n = prefixLength(path);

            CharArray ca = CharArray(n);
            path.toChars(0, n, ca, 0);

            CharArray volume = CharArray(MAX_PATH + 1);
            CharArray fs = CharArray(MAX_PATH + 1);
            DWORD serial = 0;
            DWORD maxComponentLength = 0;
            DWORD fsFlag = 0;

            gint r = GetVolumeInformation(CORE_CAST(LPWSTR, &ca[0]),
                                          CORE_CAST(LPWSTR, &volume[0]),
                                          volume.length(),
                                          &serial,
                                          &maxComponentLength,
                                          &fsFlag,
                                          CORE_CAST(LPWSTR, &fs[0]),
                                          fs.length());
            err.lazySet(GetLastError());
            if (r == 0) return 0;

            return CORE_CAST(gint, maxComponentLength);
        }

        gbool WinNTFileSystem::compare(const File &f1, const File &f2) {
            return f1.path().compareToIgnoreCase(f2.path());
        }

        gbool WinNTFileSystem::isSameFile(const File &f1, const File &f2) {
            err.lazySet(ERROR_SUCCESS);
            if (&f1 == &f2) return true;

            CharArray ca1 = f1.path().toChars();
            if (ca1.isEmpty()) return false;

            CharArray ca2 = f2.path().toChars();
            if (ca2.isEmpty()) return false;

            HANDLE handle1 = CreateFile(CORE_CAST(LPWSTR, &ca1[0]),
                                        GENERIC_READ,
                                        FILE_SHARE_READ,
                                        NULL,
                                        OPEN_EXISTING,
                                        FILE_ATTRIBUTE_NORMAL,
                                        NULL);
            err.lazySet(GetLastError());
            if (handle1 == INVALID_HANDLE_VALUE) return false;

            HANDLE handle2 = CreateFile(CORE_CAST(LPWSTR, &ca2[0]),
                                        GENERIC_READ,
                                        FILE_SHARE_READ,
                                        NULL,
                                        OPEN_EXISTING,
                                        FILE_ATTRIBUTE_NORMAL,
                                        NULL);
            err.lazySet(GetLastError());
            if (handle2 == INVALID_HANDLE_VALUE) {
                CloseHandle(handle1);
                return false;
            }

            BY_HANDLE_FILE_INFORMATION fileInfo1 = {};
            gint r1 = GetFileInformationByHandle(handle1, &fileInfo1);
            err.lazySet(GetLastError());
            if (r1 == 0) {
                CloseHandle(handle1);
                CloseHandle(handle2);
                return false;
            }

            BY_HANDLE_FILE_INFORMATION fileInfo2 = {};
            gint r2 = GetFileInformationByHandle(handle2, &fileInfo2);
            err.lazySet(GetLastError());
            if (r2 == 0) {
                CloseHandle(handle1);
                CloseHandle(handle2);
                return false;
            }

            CloseHandle(handle1);
            CloseHandle(handle2);

            return (fileInfo1.dwVolumeSerialNumber == fileInfo2.dwVolumeSerialNumber) &&
                   (fileInfo1.nFileIndexHigh == fileInfo2.nFileIndexHigh) &&
                   (fileInfo1.nFileIndexLow == fileInfo2.nFileIndexLow);
        }

        gint WinNTFileSystem::hash(const File &f) {
            /* Could make this more efficient: String.hashCodeIgnoreCase */
            return f.path().toLowerCase().hash() ^ 1234321;
        }

        String WinNTFileSystem::userDirectory() {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = CharArray(MAX_PATH + 1);
            gint r = SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROFILE, NULL, 0, CORE_CAST(LPWSTR, &ca[0])));
            err.lazySet(GetLastError());
            if (r == 0) return {};

            for (r = 0; ca[r] != 0; r += 1) {
                //
            }

            return String(ca, 0, r);
        }

        String WinNTFileSystem::tempDirectory() {
            err.lazySet(ERROR_SUCCESS);

            CharArray ca = CharArray(MAX_PATH + 1);
            gint r = GetTempPath(ca.length(), CORE_CAST(LPWSTR, &ca[0]));
            err.lazySet(GetLastError());

            return String(ca, 0, r);
        }

        gint WinNTFileSystem::lastError() {
            gint error = CORE_CAST(gint, err.get());
            err.lazySet(ERROR_SUCCESS);
            return error;
        }

        WinNTFileSystem::~WinNTFileSystem() {
            err.lazySet(ERROR_SUCCESS);
        }

        String WinNTFileSystem::stripLongOrUNCPrefix(String const &path) {
            // if a prefix is present, remove it
            if (path.startsWith(LONG_PATH_PREFIX)) {
                if (path.startsWith("UNC\\"_S, 4)) {
                    return "\\\\" + path.subString(8);
                } else {
                    return path.subString(4);
                    // if only "UNC" remains, a trailing "\\" was likely removed
                    if (path.equals("UNC"_S)) {
                        return "\\\\"_S;
                    }
                }
            }

            return path;
        }

        gbool WinNTFileSystem::isSlash(gchar c) { return (c == '\\') || (c == '/'); }

        gbool WinNTFileSystem::isLetter(gchar c) { return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')); }

        String WinNTFileSystem::slashify(String const &p) const {
            if (!p.isEmpty() && p.charAt(0) != slash)
                return slash + p;

            return p;
        }

        String WinNTFileSystem::normalize(String const &path, gint len, gint off) const {
            if (len == 0) return path;
            if (off < 3) off = 0; /* Avoid fencepost cases with UNC pathnames */
            gint src;
            gchar slash = WinNTFileSystem::slash;
            XString sb = XString(len);

            if (off == 0) {
                /* Complete normalization, including prefix */
                src = normalizePrefix(path, len, sb);
            } else {
                /* Partial normalization */
                src = off;
                sb.append(path, 0, off);
            }

            /* Remove redundant slashes from the remainder of the path, forcing all
               slashes into the preferred slash */
            while (src < len) {
                gchar c = path.charAt(src++);
                if (isSlash(c)) {
                    while ((src < len) && isSlash(path.charAt(src))) src++;
                    if (src == len) {
                        /* Check for trailing separator */
                        gint sn = sb.length();
                        if ((sn == 2) && (sb.charAt(1) == ':')) {
                            /* "z:\\" */
                            sb.append(slash);
                            break;
                        }
                        if (sn == 0) {
                            /* "\\" */
                            sb.append(slash);
                            break;
                        }
                        if ((sn == 1) && (isSlash(sb.charAt(0)))) {
                            /* "\\\\" is not collapsed to "\\" because "\\\\" marks
                               the beginning of a UNC pathname.  Even though it is
                               not, by itself, a valid UNC pathname, we leave it as
                               is in order to be consistent with the win32 APIs,
                               which treat this case as an invalid UNC pathname
                               rather than as an alias for the root directory of
                               the current drive. */
                            sb.append(slash);
                            break;
                        }
                        /* Path does not denote a root directory, so do not append
                           trailing slash */
                        break;
                    } else {
                        sb.append(slash);
                    }
                } else {
                    sb.append(c);
                }
            }

            return sb.toString();
        }

        gint WinNTFileSystem::normalizePrefix(String const &path, gint len, XString &sb) const {
            gint src = 0;
            while ((src < len) && isSlash(path.charAt(src))) src++;
            gchar c;
            if ((len - src >= 2)
                && isLetter(c = path.charAt(src))
                && path.charAt(src + 1) == ':') {
                /* Remove leading slashes if followed by drive specifier.
                   This hack is necessary to support file URLs containing drive
                   specifiers (e.g., "file://c:/path").  As a side effect,
                   "/c:/path" can be used as an alternative to "c:/path". */
                sb.append(c);
                sb.append(':');
                src += 2;
            } else {
                src = 0;
                if ((len >= 2)
                    && isSlash(path.charAt(0))
                    && isSlash(path.charAt(1))) {
                    /* UNC pathname: Retain first slash; leave src pointed at
                       second slash so that further slashes will be collapsed
                       into the second slash.  The result will be a pathname
                       beginning with "\\\\" followed (most likely) by a host
                       name. */
                    src = 1;
                    sb.append(slash);
                }
            }
            return src;
        }

        String WinNTFileSystem::userPath() {
            if (!userDir.isEmpty()) {
                return userDir;
            } else {
                String s = userDirectory();
                if (!s.isEmpty())
                    return userDir = s;
            }

            gint r = 0;
            CharArray ca = CharArray(MAX_PATH);
            r = GetEnvironmentVariable(L"USERPROFILE", CORE_CAST(LPWSTR, &ca[0]), ca.length());
            if (r > 0)
                return userDir = String(ca, 0, r);
            // user root without '\' in trailing (C:)
            r = GetEnvironmentVariable(L"HOMEDRIVE", CORE_CAST(LPWSTR, &ca[0]), ca.length());
            if (r > 0) {
                gint r2 = 0;
                // user path without a root path starting with '\' (\Users\<user>)
                r2 = GetEnvironmentVariable(L"HOMEPATH", CORE_CAST(LPWSTR, &ca[0]), ca.length() - r);
                if (r2 > 0)
                    return userDir = String(ca, 0, r + r2);
            }
            r = GetEnvironmentVariable(L"HOME", CORE_CAST(LPWSTR, &ca[0]), ca.length());
            if (r > 0)
                return userDir = String(ca, 0, r);
            Error("Couldn't read property 'user.dir'"_S).throws($ftrace());
        }

        String WinNTFileSystem::drive(String const &path) {
            gint pl = prefixLength(path);
            return (pl == 3) ? path.subString(0, 2) : ""_S;
        }

        StringArray WinNTFileSystem::DRIVE_DIR_CACHE = StringArray(26);

        gint WinNTFileSystem::driveIndex(gchar d) {
            if ((d >= 'a') && (d <= 'z')) return d - 'a';
            if ((d >= 'A') && (d <= 'Z')) return d - 'A';
            return -1;
        }

        String WinNTFileSystem::driveDirectory(gint drive) {
            if (drive > 0) {
                glong drives = GetLogicalDrives();
                err.lazySet(GetLastError());
                if (drives > 0) {
                    if (drives & (1 << drive))
                        return String(CharArray::of(CORE_CAST(gchar, u'A' + (drive - 1)) /* Drive letter*/,
                                                    u':', u'\\'));
                }
            }

            return ""_S;
        }

        String WinNTFileSystem::driveDirectory(gchar drive) {
            CORE_ASSERT(DRIVE_DIR_CACHE.length() == 26);
            gint i = driveIndex(drive);
            if (i < 0) return ""_S;
            // Updates might not be visible to other threads so there
            // is no guarantee getDriveDirectory(i+1) is called just once
            // for any given value of i.
            String s = DRIVE_DIR_CACHE[i];
            if (!s.isEmpty()) return s;
            s = driveDirectory(i + 1);
            DRIVE_DIR_CACHE[i] = s;
            return s;
        }

        String WinNTFileSystem::realPath(String const &path) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = path.toChars();

            CharArray absolute = CharArray(MAX_PATH + 1);

            gint r = GetFullPathName(CORE_CAST(LPWSTR, &ca[0]), ca.length(), CORE_CAST(LPWSTR, &absolute[0]), NULL);
            err.lazySet(GetLastError());
            if (r == 0) return {};

            // CharArray canonical = CharArray(MAX_PATH + 1);
            // // Normalize the path to get the canonical form
            // gint r2 = PathCanonicalize(CORE_CAST(LPWSTR, &canonical[0]), CORE_CAST(LPWSTR, &absolute[0]));
            // err.lazySet(GetLastError());
            // if (r2 == 0) return {};
            //
            // return String(canonical, 0, r2);
            return String(absolute);
        }

        String WinNTFileSystem::readLink(String const &path) {
            err.lazySet(ERROR_SUCCESS);
            CharArray ca = path.toChars();
            if (ca.isEmpty()) return {};

            HANDLE handle = CreateFile(CORE_CAST(LPWSTR, &ca[0]),
                                       GENERIC_READ,
                                       FILE_SHARE_READ,
                                       NULL,
                                       OPEN_EXISTING,
                                       FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
                                       NULL);
            err.lazySet(GetLastError());
            if (handle == INVALID_HANDLE_VALUE) return {};

            ByteArray ba = ByteArray(MAXIMUM_REPARSE_DATA_BUFFER_SIZE);
            DWORD nBytes = 0;
            gint r = DeviceIoControl(handle,
                                     FSCTL_GET_REPARSE_POINT,
                                     NULL, 0,
                                     CORE_CAST(LPVOID, &ba[0]),
                                     ba.length(),
                                     &nBytes,
                                     NULL);
            err.lazySet(GetLastError());
            if (r == 0) {
                CloseHandle(handle);
                return {};
            }

            CloseHandle(handle);
            PREPARSE_DATA_BUFFER rdb = CORE_CAST(PREPARSE_DATA_BUFFER, &ba[0]);
            if (rdb->ReparseTag != IO_REPARSE_TAG_SYMLINK)
                return {};

            LPWSTR target = CORE_CAST(LPWSTR, rdb->SymbolicLinkReparseBuffer.PathBuffer);
            glong offset = rdb->SymbolicLinkReparseBuffer.PrintNameOffset >> 1;
            glong count = rdb->SymbolicLinkReparseBuffer.PrintNameLength >> 1;

            ca = CharArray(count);
            for (int i = 0; i < count; ++i) ca[i] = target[offset++];

            return String(ca, 0, count);
        }

        gbool WinNTFileSystem::setEveryoneAccess(File const &f, gint access, gbool enable) {
            DWORD everyoneAccess = 0;
            if (access == ACCESS_READ) everyoneAccess = GENERIC_READ;
            else if (access == ACCESS_WRITE) everyoneAccess = GENERIC_WRITE;
            else if (access == ACCESS_EXECUTE) everyoneAccess = GENERIC_EXECUTE;
            else return false;

            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;

            PSECURITY_DESCRIPTOR pSD = NULL;
            PACL pOldDACL = NULL;
            PACL pNewDACL = NULL;
            EXPLICIT_ACCESS ea;
            PSID pEveryoneSID = NULL;
            SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
            DWORD r = 0;

            // Create SID for Everyone Group
            r = AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID);
            if (r == 0) return false;

            // Get Current DACL
            r = GetNamedSecurityInfo(CORE_CAST(LPWSTR, &ca[0]), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
                                     &pOldDACL, NULL, &pSD);
            if (r != ERROR_SUCCESS) goto CLEAN_UP;

            // Initialize an EXPLICIT_ACCESS structure for new DACL
            ZeroMemory(&ea, ClassOf(ea)::size());
            ea.grfAccessPermissions = everyoneAccess;
            ea.grfAccessMode = enable ? GRANT_ACCESS : REVOKE_ACCESS;
            ea.grfInheritance = NO_INHERITANCE;
            ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
            ea.Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
            ea.Trustee.ptstrName = CORE_CAST(LPWSTR, pEveryoneSID);

            // Create new DACL to file
            r = SetEntriesInAcl(1, &ea, pOldDACL, &pNewDACL);
            if (r != ERROR_SUCCESS) goto CLEAN_UP;

            // Apply the new DACL
            r = SetNamedSecurityInfo(CORE_CAST(LPWSTR, &ca[0]), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
                                     pNewDACL, NULL);
            if (r != ERROR_SUCCESS) goto CLEAN_UP;

        CLEAN_UP:
            // Clean Up
            if (pSD != NULL) LocalFree(CORE_CAST(HLOCAL, pSD));
            if (pNewDACL != NULL) LocalFree(CORE_CAST(HLOCAL, pNewDACL));
            if (pEveryoneSID != NULL) LocalFree(CORE_CAST(HLOCAL, pEveryoneSID));

            return r == ERROR_SUCCESS;
        }

        gbool WinNTFileSystem::setOwnerAccess(File const &f, gint access, gbool enable) {
            DWORD ownerAccess = 0;
            if (access == ACCESS_READ) ownerAccess = GENERIC_READ;
            else if (access == ACCESS_WRITE) ownerAccess = GENERIC_WRITE;
            else if (access == ACCESS_EXECUTE) ownerAccess = GENERIC_EXECUTE;
            else return false;

            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;

            PSECURITY_DESCRIPTOR pSD = NULL;
            PACL pOldDACL = NULL;
            PACL pNewDACL = NULL;
            EXPLICIT_ACCESS ea;
            PSID pOwnerSID = NULL;
            HANDLE hToken = NULL;
            DWORD dwSize = 0;
            PTOKEN_OWNER pTokenOwner = NULL;
            DWORD r = 0;

            // Open an access token associated to current process.
            r = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
            if (r == 0) return false;

            // Get the size of token information.
            GetTokenInformation(hToken, TokenOwner, NULL, 0, &dwSize);
            pTokenOwner = CORE_CAST(PTOKEN_OWNER, LocalAlloc(LPTR, dwSize));

            // Get the token information
            r = GetTokenInformation(hToken, TokenOwner, pTokenOwner, dwSize, &dwSize);
            if (r == 0) goto CLEAN_UP;

            pOwnerSID = pTokenOwner->Owner;

            // Get current DACL
            r = GetNamedSecurityInfo(CORE_CAST(LPWSTR, &ca[0]), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
                                     &pOldDACL, NULL, &pSD);

            if (r != ERROR_SUCCESS) goto CLEAN_UP;

            // Initialize an EXPLICIT_ACCESS structure for new ACE
            ZeroMemory(&ea, ClassOf(ea)::size());
            ea.grfAccessPermissions = ownerAccess;
            ea.grfAccessMode = enable ? GRANT_ACCESS : REVOKE_ACCESS;
            ea.grfInheritance = NO_INHERITANCE;
            ea.Trustee.TrusteeForm = TRUSTEE_IS_SID;
            ea.Trustee.TrusteeType = TRUSTEE_IS_USER;
            ea.Trustee.ptstrName = CORE_CAST(LPWSTR, pOwnerSID);

            // Create the DACL to merge new ACE into the existing DACL
            r = SetEntriesInAcl(1, &ea, pOldDACL, &pNewDACL);
            if (r != ERROR_SUCCESS) goto CLEAN_UP;

            // Create new DACL to the file
            r = SetNamedSecurityInfo(CORE_CAST(LPWSTR, &ca[0]), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
                                     pNewDACL, NULL);
            if (r != ERROR_SUCCESS) goto CLEAN_UP;

        CLEAN_UP:
            // Clean Up
            if (pSD != NULL) LocalFree(CORE_CAST(HLOCAL, pSD));
            if (pNewDACL != NULL) LocalFree(CORE_CAST(HLOCAL, pNewDACL));
            if (pTokenOwner != NULL) LocalFree(CORE_CAST(HLOCAL, pTokenOwner));
            if (hToken != NULL) CloseHandle(hToken);

            return r == ERROR_SUCCESS;
        }

        gbool WinNTFileSystem::checkOwnerAccess(File const &f, gint access) {
            DWORD ownerAccess = 0;
            if (access == ACCESS_READ) ownerAccess = GENERIC_READ;
            else if (access == ACCESS_WRITE) ownerAccess = GENERIC_WRITE;
            else if (access == ACCESS_EXECUTE) ownerAccess = GENERIC_EXECUTE;
            else return false;

            CharArray ca = f.path().toChars();
            if (ca.isEmpty()) return false;

            PSECURITY_DESCRIPTOR pSD = NULL;
            PACL pDACL = NULL;
            PSID pOwnerSID = NULL;
            HANDLE hToken = NULL;
            DWORD dwSize = 0;
            PTOKEN_OWNER pTokenOwner = NULL;
            // SID_IDENTIFIER_AUTHORITY sia = SECURITY_NT_AUTHORITY;
            DWORD r = 0;

            r = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
            if (r == 0) return false;

            // Get the size of Token Information// Get the size of token information.
            GetTokenInformation(hToken, TokenOwner, NULL, 0, &dwSize);
            pTokenOwner = CORE_CAST(PTOKEN_OWNER, LocalAlloc(LPTR, dwSize));

            // Get the token information
            r = GetTokenInformation(hToken, TokenOwner, pTokenOwner, dwSize, &dwSize);
            if (r == 0) {
                LocalFree(pTokenOwner);
                CloseHandle(hToken);
                return false;
            }

            pOwnerSID = pTokenOwner->Owner;

            // Get current DACL
            r = GetNamedSecurityInfo(CORE_CAST(LPWSTR, &ca[0]), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
                                     &pDACL, NULL, &pSD);

            if (r != ERROR_SUCCESS) {
                LocalFree(pTokenOwner);
                CloseHandle(hToken);
                return false;
            }

            // Check If the owner has Write Access
            gbool hasDesiredAccess = false;
            for (int i = 0; i < pDACL->AceCount; ++i) {
                PACCESS_ALLOWED_ACE pACE = NULL;
                r = GetAce(pDACL, i, CORE_CAST(LPVOID*, &pACE));
                if (r == 0) continue;

                PSID pSID = &pACE->SidStart;
                if (!EqualSid(pOwnerSID, pSID)) continue;

                if (pACE->Mask & ownerAccess) {
                    hasDesiredAccess = true;
                    break;
                }
            }

            // Clean Up
            if (pSD != NULL) LocalFree(CORE_CAST(HLOCAL, pSD));
            if (pDACL != NULL) LocalFree(CORE_CAST(HLOCAL, pDACL));
            if (pTokenOwner != NULL) LocalFree(CORE_CAST(HLOCAL, pTokenOwner));
            if (hToken != NULL) CloseHandle(hToken);

            return hasDesiredAccess;
        }

        gint WinNTFileSystem::getUnixAccess(const String &pathname) {
            CharArray ca = pathname.toChars();
            if (ca.isEmpty()) return -1;

            PSECURITY_DESCRIPTOR pSD = NULL;
            PACL pDACL = NULL;
            DWORD r = 0;
            gint user = 0, group = 0, other = 0;

            r = GetNamedSecurityInfo(CORE_CAST(LPWSTR, &ca), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
                                     &pDACL, NULL, &pSD);
            if (r != ERROR_SUCCESS) return -1;

            // Start the Searching
            ACL_SIZE_INFORMATION aclSizeInfo;
            r = GetAclInformation(pDACL, &aclSizeInfo, Class<ACL_SIZE_INFORMATION>::size(), AclSizeInformation);
            if (r == 0) goto CLEAN_UP;


            for (DWORD i = 0; i < aclSizeInfo.AceCount; ++i) {
                PACCESS_ALLOWED_ACE pACE = NULL;
                r = GetAce(pDACL, i, CORE_CAST(LPVOID *, &pACE));
                if (r == 0) continue;

                PSID pSID = &pACE->SidStart;
                SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
                PSID pEveryoneSID = NULL;
                AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID);

                if (EqualSid(pSID, pEveryoneSID)) {
                    // Other Permissions
                    if (pACE->Mask & GENERIC_READ) other |= ACCESS_READ;
                    if (pACE->Mask & GENERIC_WRITE) other |= ACCESS_WRITE;
                    if (pACE->Mask & GENERIC_EXECUTE) other |= ACCESS_EXECUTE;
                } else {
                    // Check If it is Owner/ Group
                    if (i == 0) {
                        // Assuming the first as Owner
                        if (pACE->Mask & GENERIC_READ) user |= ACCESS_READ;
                        if (pACE->Mask & GENERIC_WRITE) user |= ACCESS_WRITE;
                        if (pACE->Mask & GENERIC_EXECUTE) user |= ACCESS_EXECUTE;
                    } else {
                        // Assuming the second as Group
                        if (pACE->Mask & GENERIC_READ) group |= ACCESS_READ;
                        if (pACE->Mask & GENERIC_WRITE) group |= ACCESS_WRITE;
                        if (pACE->Mask & GENERIC_EXECUTE) group |= ACCESS_EXECUTE;
                    }
                }

                FreeSid(pEveryoneSID);
            }

            r = user << 6 | group << 3 | other;

        CLEAN_UP:
            // Clean Up
            if (pSD != NULL) LocalFree(CORE_CAST(HLOCAL, pSD));
            return CORE_CAST(gint, r);
        }

        gbool WinNTFileSystem::setUnixAccess(const String &pathname, gint access) {
            CharArray ca = pathname.toChars();
            if (ca.isEmpty()) return false;

            // Convert POSIX access to Windows Access

            // On windows, user is Owner
            DWORD ownerAccess = 0;
            if (access & 0400) ownerAccess |= GENERIC_READ;
            if (access & 0200) ownerAccess |= GENERIC_WRITE;
            if (access & 0100) ownerAccess |= GENERIC_EXECUTE;

            // On windows, other is Everyone and EveryOne is a group => EveryOne = group << 3 | other
            DWORD everyoneAccess = 0;
            if (access & 0044) everyoneAccess |= GENERIC_READ;
            if (access & 0022) everyoneAccess |= GENERIC_WRITE;
            if (access & 0011) everyoneAccess |= GENERIC_EXECUTE;

            // Fields
            PSECURITY_DESCRIPTOR pSD = NULL;
            PACL pOldDACL = NULL;
            PACL pNewDACL = NULL;
            EXPLICIT_ACCESS ea[2];
            PSID pOwnerSID = NULL;
            PSID pEveryoneSID = NULL;
            HANDLE hToken = NULL;
            DWORD dwSize = 0;
            PTOKEN_OWNER pTokenOwner = NULL;
            SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
            DWORD r = 0;

            // Open access token associated with the current process
            r = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
            if (r == 0) return false;

            // Get the size of the token information.
            GetTokenInformation(hToken, TokenOwner, NULL, 0, &dwSize);
            pTokenOwner = CORE_CAST(PTOKEN_OWNER, LocalAlloc(LPTR, dwSize));

            // Get Token Information
            r = GetTokenInformation(hToken, TokenOwner, pTokenOwner, dwSize, &dwSize);
            if (r == 0) {
                if (hToken != NULL) CloseHandle(hToken);
                return false;
            }

            pOwnerSID = pTokenOwner->Owner;

            // Create SID for EveryOne Group
            AllocateAndInitializeSid(&SIDAuthWorld, 1, SECURITY_WORLD_RID, 0, 0, 0, 0, 0, 0, 0, &pEveryoneSID);

            // Get current DACL
            r = GetNamedSecurityInfo(CORE_CAST(LPWSTR, &ca[0]), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
                                     &pOldDACL, NULL, &pSD);
            if (r != ERROR_SUCCESS) goto CLEAN_UP;

            // Initialize an EXPLICIT_ACCESS for new ACEs
            ZeroMemory(ea, 2 * Class<EXPLICIT_ACCESS>::size());

            // Define Owner access
            ea[0].grfAccessPermissions = ownerAccess;
            ea[0].grfAccessMode = SET_ACCESS;
            ea[0].grfInheritance = NO_INHERITANCE;
            ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
            ea[0].Trustee.TrusteeType = TRUSTEE_IS_USER;
            ea[0].Trustee.ptstrName = CORE_CAST(LPWSTR, pOwnerSID);

            ea[1].grfAccessPermissions = everyoneAccess;
            ea[1].grfAccessMode = SET_ACCESS;
            ea[1].grfInheritance = NO_INHERITANCE;
            ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
            ea[1].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
            ea[1].Trustee.ptstrName = CORE_CAST(LPWSTR, pEveryoneSID);

            // Create the new DACL that merge the new ACEs into existing DACL
            r = SetEntriesInAcl(2, ea, pOldDACL, &pNewDACL);

            // Apply the new DACL to the file
            r = SetNamedSecurityInfo(CORE_CAST(LPWSTR, &ca[0]), SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL,
                                     pNewDACL, NULL);
            if (r != ERROR_SUCCESS) goto CLEAN_UP;

        CLEAN_UP:
            // Clean Up
            if (pSD != NULL) LocalFree(CORE_CAST(HLOCAL, pSD));
            if (pNewDACL != NULL) LocalFree(CORE_CAST(HLOCAL, pNewDACL));
            if (pTokenOwner != NULL) LocalFree(CORE_CAST(HLOCAL, pTokenOwner));
            if (pEveryoneSID != NULL) FreeSid(pEveryoneSID);
            if (hToken != NULL) CloseHandle(hToken);

            return r == ERROR_SUCCESS;
        }
    } // io
} // core

namespace meta {
    namespace io {
        FileSystem &FileSystem::ofPlatform() {
            try {
                return UNSAFE::newInstance<WinNTFileSystem>();
            } catch (Throwable const &ex) { ex.throws($ftrace(meta::io::FileSystem)); }
        }
    }
}
