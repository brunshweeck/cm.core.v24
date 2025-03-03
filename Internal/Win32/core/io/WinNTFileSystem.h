//
// Created by brunshweeck on 16 sept. 2024.
//

#ifndef CORE24_WINNTFILESYSTEM_H
#define CORE24_WINNTFILESYSTEM_H

#include <core/atomic/AtomicLong.h>
#include <core/io/File.h>
#include <meta/io/FileSystem.h>

namespace core {
    namespace io {
        using meta::io::FileSystem;

        /**
         * Unicode-aware FileSystem for Windows NT/2000.
         */
        class WinNTFileSystem final : public FileSystem {
            static String const LONG_PATH_PREFIX;

            gchar slash;
            gchar altSlash;
            gchar semicolon;
            String userDir;

            atomic::AtomicLong err;

        public:
            CORE_IMPLICIT WinNTFileSystem();

            /* -- Normalization and construction -- */

            gchar fileSeparator() override;

            gchar pathSeparator() override;

            /* Check that the given pathname is normal.  If not, invoke the real
               normalizer on the part of the pathname that requires normalization.
               This way we iterate through the whole pathname string only once. */
            String normalize(const String& path) override;

            gint prefixLength(String const& path) override;

            String resolve(const String& parent, const String& child) override;

            String defaultParent() override;

            String fromURIPath(const String& path) override;

            /* -- Path operations -- */

            gbool isAbsolute(const File& f) override;

            gbool isInvalid(const File& f) override;

            String resolve(const File& f) override;

            String canonicalize(const String& path) override;


            /* -- Attribute accessors -- */

            enum {
                WA_REPARSE_POINT = 0x20,
                WA_SYSTEM = 0x40,
                WA_ARCHIVE = 0x80,
                WA_DEVICE = 0x100,
                WA_READONLY = 0x200,
                WA_TEMPORARY = 0x400,
                WA_SPARSE_FILE = 0x800,
                WA_COMPRESSED = 0x1000,
                WA_OFFLINE = 0x2000,
                WA_NOT_CONTENT_INDEXED = 0x4000,
                WA_ENCRYPTED = 0x8000,
                WA_INTEGRITY_STREAM = 0x10000,
                WA_VIRTUAL = 0x20000,
                WA_EA = 0x40000,
                WA_PINNED = 0x80000,
                WA_RECALL_ON_OPEN = 0x100000,
                WA_RECALL_ON_DATA_ACCESS = 0x200000,
                WA_STRICTLY_SEQUENTIAL = 0x400000,
            };

            gint booleanAttributes(const File& f) override;

            gbool checkAccess(const File& f, gint access) override;

            glong lastModifiedTime(const File& f) override;

            glong lastAccessTime(const File& f) override;

            glong creationTime(const File& f) override;

            glong length(const File& f) override;

            gbool setPermission(const File& f, gint access, gbool enable, gbool ownerOnly) override;

            gint posixPermission(const File &f) override;

            gbool setPosixPermission(const File& f, gint perms) override;

            /* -- File operations -- */

            gbool createFileExclusively(const String& pathname) override;

            gbool createDirectory(const String& pathname) override;

            gbool markAsTemporary(const String& pathname) override;

            StringArray children(const File& f) override;

            gbool setLastModifiedTime(const File& f, glong time) override;

            gbool setLastAccessTime(const File& f, glong time) override;

            gbool setCreationTime(const File& f, glong time) override;

            gbool setReadOnly(const File& f) override;

            gbool deleteFile(const File& f) override;

            gbool recycleFile(const File& f) override;

            gbool renameFile(const File& source, const File& target) override;

            gbool copyFile(const File& source, const File& target, File::CopyOption options) override;

            gbool moveFile(const File& source, const File& target, File::CopyOption options) override;

            FileArray roots() override;

            /* -- Disk usage -- */
            glong diskSpace(const File& f, gint type) override;

            /* -- Basic infrastructure -- */

            // Obtain maximum file component length from GetVolumeInformation which
            // expects the path to be null or a root component ending in a backslash
            gint maxNameLength(const String& path) override;

            gbool compare(const File& f1, const File& f2) override;

            gbool isSameFile(const File& f1, const File& f2) override;

            gint hash(const File& f) override;

            String userDirectory() override;

            String tempDirectory() override;

            gint lastError() override;

            ~WinNTFileSystem() override;

        private:
            // Strip a long path or UNC prefix and return the result.
            // If there is no such prefix, return the parameter passed in.
            static String stripLongOrUNCPrefix(String const& path);

            static gbool isSlash(gchar c);

            static gbool isLetter(gchar c);

            String slashify(String const& p) const;

            /* Normalize the given pathname, whose length is len, starting at the given
               offset; everything before this offset is already normal. */
            String normalize(String const& path, gint len, gint off) const;

            /* A normal Win32 pathname contains no duplicate slashes, except possibly
               for a UNC prefix, and does not end with a slash.  It may be the empty
               string.  Normalized Win32 pathnames have the convenient property that
               the length of the prefix almost uniquely identifies the type of the path
               and whether it is absolute or relative:

                   0  relative to both drive and directory
                   1  drive-relative (begins with '\\')
                   2  absolute UNC (if first char is '\\'),
                        else directory-relative (has form "z:foo")
                   3  absolute local pathname (begins with "z:\\")
             */
            gint normalizePrefix(String const& path, gint len, XString &sb) const;

            String userPath();

            String drive(String const& path);

            static StringArray DRIVE_DIR_CACHE;

            static gint driveIndex(gchar d);

            String driveDirectory(gint drive);

            String driveDirectory(gchar drive);

            String realPath(String const& path);

            String readLink(String const& path);

            static gbool setEveryoneAccess(File const& f, gint access, gbool enable);

            static gbool setOwnerAccess(File const& f, gint access, gbool enable);

            static gbool checkOwnerAccess(File const& f, gint access);

            static gint getUnixAccess(const String& pathname);

            static gbool setUnixAccess(const String& pathname, gint access);
        };
    } // io
} // core

#endif //CORE24_WINNTFILESYSTEM_H
