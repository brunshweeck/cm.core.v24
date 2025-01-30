//
// Created by brunshweeck on 07/10/24.
//

#include <core/Unix.h>

#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif

#include <core/StringArray.h>
#include <core/XString.h>
#include <core/io/FileArray.h>
#include <core/io/UnixFileSystem.h>
#include <core/util/Arrays.h>
#include "Trash.h"

#ifndef PATH_MAX
#define PATH_MAX 1024 * 1024
#endif

namespace core {
    namespace io {
        UnixFileSystem::UnixFileSystem() {
            slash = '/';
            colon = ':';
            userDir = userDirectory();
        }

        gchar UnixFileSystem::fileSeparator() {
            return slash;
        }

        gchar UnixFileSystem::pathSeparator() {
            return colon;
        }

        String UnixFileSystem::normalize(const String& pathname) {
            int doubleSlash = pathname.indexOf("//"_S);
            if (doubleSlash >= 0) {
                return normalize(pathname, doubleSlash);
            }

            return pathname.endsWith("/"_S) ? normalize(pathname, pathname.length() - 1) : pathname;
        }

        gint UnixFileSystem::prefixLength(const String& pathname) {
            return pathname.startsWith("/"_S) ? 1 : 0;
        }

        String UnixFileSystem::resolve(const String& parent, const String& child) {
            if (child.isEmpty()) {
                return parent;
            } else if (child.charAt(0) == '/') {
                return parent.equals("/"_S) ? child : trimSeparator(parent + child);
            } else {
                return parent.equals("/"_S) ? trimSeparator(parent + child) : trimSeparator(parent + '/' + child);
            }
        }

        String UnixFileSystem::defaultParent() {
            return "/"_S;
        }

        String UnixFileSystem::fromURIPath(const String& path) {
            String p = path;
            if (path.endsWith("/"_S) && path.length() > 1) {
                p = path.subString(0, path.length() - 1);
            }

            return p;
        }

        gbool UnixFileSystem::isAbsolute(const File& f) {
            return prefixLength(f.path()) != 0;
        }

        gbool UnixFileSystem::isInvalid(const File& f) {
            return f.path().indexOf(0) >= 0;
        }

        String UnixFileSystem::resolve(const File& f) {
            if (isAbsolute(f)) {
                return f.path();
            }

            return resolve(userDir, f.path());
        }

        String UnixFileSystem::canonicalize(const String& path) {
            ByteArray str = path.toBytes();
            if (str.length() < 1)
                return String();

            return RealPath((LPCSTR) &str[0]);
        }

        String UnixFileSystem::createLink(String const& link, String const& target, LinkType type) {
            ByteArray lba = link.toBytes();
            ByteArray tba = target.toBytes();

            if (lba.length() < 1 || tba.length() < 1)
                return String();

            LPCSTR path1 = (LPCSTR) &lba[0];
            LPCSTR path2 = (LPCSTR) &tba[0];
            BOOL res = FALSE;
            switch (type) {
                case SYMBOLIC_LINK:
                    res = CreateSymbolicLink(path1, path2);
                    break;
                case HARD_LINK:
                    res = CreateLink(path1, path2);
                    break;
                case SHORTCUT_LINK:
                    res = CreateShortCut(path1, path2);
                    break;
            }

            if (!res)
                return String();

            return link;
        }

        String UnixFileSystem::readLink(String const& link) {
            ByteArray lba = link.toBytes();
            if (lba.length() < 1)
                return String();
            ByteArray tba = ByteArray(PATH_MAX);
            LPCSTR path1 = (LPCSTR) &lba[0];
            LPSTR path2 = (LPSTR) &tba[0];
            DWORD size = tba.length();

            size = ReadLink(path1, path2, size);
            if (size == 0)
                return String();

            return String(tba, 0, size);
        }

        gint UnixFileSystem::booleanAttributes(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return 0;
            LPCSTR path = (LPCSTR) &fba[0];
            DWORD attributes = GetFileAttributes(path);

            gint result = 0;
            if ((attributes & FA_DIRECTORY) == FA_DIRECTORY) result |= BA_DIRECTORY;
            if ((attributes & FA_REGULAR) == FA_REGULAR) result |= BA_REGULAR;
            if ((attributes & FA_HIDDEN) == FA_HIDDEN) result |= BA_HIDDEN;
            if ((attributes & FA_SYMLINK) == FA_SYMLINK) result |= BA_SYMLINK;
            if ((attributes & FA_EXIST) == FA_EXIST) result |= BA_EXISTS;

            return result;
        }

        gbool UnixFileSystem::checkAccess(const File& f, gint access) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            DWORD attributes = GetFileAttributes(path);
            if ((access & ACCESS_READ) == ACCESS_READ && (attributes & FA_OWNER_READ) != FA_OWNER_READ)
                return FALSE;
            if ((access & ACCESS_WRITE) == ACCESS_WRITE && (attributes & FA_OWNER_WRITE) != FA_OWNER_WRITE)
                return FALSE;
            if ((access & ACCESS_EXECUTE) == ACCESS_EXECUTE && (attributes & FA_OWNER_EXECUTE) != FA_OWNER_EXECUTE)
                return FALSE;
            return TRUE;
        }

        gbool UnixFileSystem::setPermission(const File& f, gint access, gbool enable, gbool ownerOnly) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            gint attributes = GetFileAttributes(path);
            if ((access & ACCESS_READ) == ACCESS_READ) {
                if ((attributes & FA_OWNER_READ) == FA_OWNER_READ) {
                    if (!enable)
                        attributes &= ~FA_OWNER_READ;
                } else {
                    if (enable)
                        attributes |= FA_OWNER_READ;
                }
            }
            if ((access & ACCESS_WRITE) == ACCESS_WRITE) {
                if ((attributes & FA_OWNER_WRITE) == FA_OWNER_WRITE) {
                    if (!enable)
                        attributes &= ~FA_OWNER_WRITE;
                } else {
                    if (enable)
                        attributes |= FA_OWNER_WRITE;
                }
            }
            if ((access & ACCESS_EXECUTE) == ACCESS_EXECUTE) {
                if ((attributes & FA_OWNER_EXECUTE) == FA_OWNER_EXECUTE) {
                    if (!enable)
                        attributes &= ~FA_OWNER_EXECUTE;
                } else {
                    if (enable)
                        attributes |= FA_OWNER_EXECUTE;
                }
            }

            DWORD dwMode = FileAttributesToPermission(attributes);
            return SetPermission(path, dwMode);
        }

        gint UnixFileSystem::posixPermission(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            DWORD attributes = GetFileAttributes(path);

            return FileAttributesToPermission(attributes);
        }

        gbool UnixFileSystem::setPosixPermission(const File& f, gint perms) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            perms &= 0777;
            return SetPermission(path, perms);
        }

        glong UnixFileSystem::lastModifiedTime(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            return GetLastWriteTime(path);
        }

        glong UnixFileSystem::lastAccessTime(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            return GetAccessTime(path);
        }

        glong UnixFileSystem::creationTime(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            return GetCreateTime(path);
        }

        glong UnixFileSystem::length(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            return GetFileSize(path);
        }

        gbool UnixFileSystem::createFileExclusively(const String& pathname) {
            ByteArray fba = pathname.toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            return CreateFile(path);
        }

        gbool UnixFileSystem::createDirectory(const String& pathname) {
            ByteArray fba = pathname.toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            return CreateDirectory(path);
        }

        gbool UnixFileSystem::markAsTemporary(const String& pathname) {
            return false;
        }

        gbool UnixFileSystem::deleteFile(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            return DeleteFile(path);
        }

        gbool UnixFileSystem::recycleFile(const File& f) {
            return Trash(f).commit();
        }

        StringArray UnixFileSystem::children(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return StringArray();
            LPCSTR path = (LPCSTR) &fba[0];

            FILE_ENTRY entry = { };
            if (!ReadDirectory(path, &entry))
                return StringArray();

            StringArray result = StringArray();
            DWORD i = 0;
            do {
                if (i == result.length()) {
                    gint n = Arrays::newLength(i, i + 16, i >> 1);
                    result = Arrays::copyOf(result, n);
                }
                result[i++] = entry.lpFileName;
            } while (NextEntry(&entry));
            CloseEntry(&entry);

            if (i == result.length())
                return result;

            return Arrays::copyOf(result, i);
        }

        gbool UnixFileSystem::renameFile(const File& source, const File& target) {
            ByteArray sba = source.path().toBytes();
            ByteArray tba = target.path().toBytes();
            if (sba.length() < 1 || tba.length() < 1)
                return FALSE;
            LPCSTR path1 = (LPCSTR) &sba[0];
            LPCSTR path2 = (LPCSTR) &tba[0];
            return MoveFile(path1, path2);
        }

        gbool UnixFileSystem::copyFile(const File& source, const File& target, File::CopyOption options) {
            ByteArray sba = source.path().toBytes();
            ByteArray tba = target.path().toBytes();
            if (sba.length() < 1 || tba.length() < 1)
                return FALSE;
            LPCSTR path1 = (LPCSTR) &sba[0];
            LPCSTR path2 = (LPCSTR) &tba[0];
            return CopyFileEx(path1, path2, options);
        }

        gbool UnixFileSystem::moveFile(const File& source, const File& target, File::CopyOption options) {
            ByteArray sba = source.path().toBytes();
            ByteArray tba = target.path().toBytes();
            if (sba.length() < 1 || tba.length() < 1)
                return FALSE;
            LPCSTR path1 = (LPCSTR) &sba[0];
            LPCSTR path2 = (LPCSTR) &tba[0];
            return MoveFileEx(path1, path2, options);
        }

        gbool UnixFileSystem::setLastModifiedTime(const File& f, glong time) {
            return false;
        }

        gbool UnixFileSystem::setLastAccessTime(const File& f, glong time) {
            return false;
        }

        gbool UnixFileSystem::setCreationTime(const File& f, glong time) {
            return false;
        }

        gbool UnixFileSystem::setReadOnly(const File& f) {
            ByteArray fba = f.path().toBytes();
            if (fba.length() < 1)
                return FALSE;
            LPCSTR path = (LPCSTR) &fba[0];
            gint attributes = GetFileAttributes(path);
            attributes |= FA_OWNER_READ;
            attributes &= ~FA_OWNER_WRITE;

            DWORD dwMode = FileAttributesToPermission(attributes);
            return SetPermission(path, dwMode);
        }

        FileArray UnixFileSystem::roots() {
            return {File("/"_S)};
        }

        glong UnixFileSystem::diskSpace(const File& f, gint type) {
            return 0;
        }

        gint UnixFileSystem::maxNameLength(const String& path) {
            return PATH_MAX;
        }

        gint UnixFileSystem::hash(const File& f) {
            return f.path().hash();
        }

        String UnixFileSystem::userDirectory() {
            char const* user = GetEnv("HOME");
            if (!user) {
                user = GetEnv("USER");
                if (user)
                    return "/home/"_S + String(user);
            }
            return String(user);
        }

        String UnixFileSystem::tempDirectory() {
            return "/tmp"_Sl;
        }

        gbool UnixFileSystem::isSameFile(const File& f1, const File& f2) {
            ByteArray fba1 = f1.path().toBytes();
            ByteArray fba2 = f2.path().toBytes();
            if (fba1.length() < 1 || fba2.length() < 1)
                return FALSE;
            LPCSTR path1 = (LPCSTR) &fba1[0];
            LPCSTR path2 = (LPCSTR) &fba2[0];
            return CompareFile(path1, path2) == 0;
        }

        gbool UnixFileSystem::compare(const File& f1, const File& f2) {
            return f1.path().compareTo(f2.path());
        }

        gint UnixFileSystem::lastError() {
            return 0;
        }

        String UnixFileSystem::normalize(String const& pathname, gint off) {
            int n;
            for (n = pathname.length(); n > off && pathname.charAt(n - 1) == '/'; --n) {
                //
            }

            if (n == 0) {
                return "/"_S;
            } else if (n == off) {
                return pathname.subString(0, off);
            } else {
                XString sb = XString(n);
                if (off > 0) {
                    sb.append(pathname, 0, off);
                }

                gchar prevChar = 0;

                for (int i = off; i < n; ++i) {
                    gchar c = pathname.charAt(i);
                    if (prevChar != '/' || c != '/') {
                        sb.append(c);
                        prevChar = c;
                    }
                }

                return sb.toString();
            }
        }

        String UnixFileSystem::trimSeparator(String const& s) {
            int len = s.length();
            return len > 1 && s.charAt(len - 1) == '/' ? s.subString(0, len - 1) : s;
        }
    } // io
} // core

namespace meta {
    namespace io {
        using namespace core::io;

        FileSystem& FileSystem::ofPlatform() {
            try {
                return UNSAFE::newInstance<UnixFileSystem>();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    }
}
