//
// Created by brunshweeck on 07/10/24.
//

#ifndef CORE_UNIXFILESYSTEM_H
#define CORE_UNIXFILESYSTEM_H
#include "meta/io/FileSystem.h"

namespace core {
    namespace io {
        using meta::io::FileSystem;

        class UnixFileSystem final : public FileSystem {
            gchar slash;
            gchar colon;
            String userDir;

        public:
            CORE_IMPLICIT UnixFileSystem();

            gchar fileSeparator() override;

            gchar pathSeparator() override;

            String normalize(const String &path) override;

            gint prefixLength(const String &path) override;

            String resolve(const String &parent, const String &child) override;

            String defaultParent() override;

            String fromURIPath(const String &path) override;

            gbool isAbsolute(const File &f) override;

            gbool isInvalid(const File &f) override;

            String resolve(const File &f) override;

            String canonicalize(const String &path) override;

            String createLink(String const& link, String const& target, LinkType type) override;

            String readLink(String const& link) override;

            static CORE_FAST gint UA_BLK = 1 << 5;
            static CORE_FAST gint UA_CHR = 1 << 6;
            static CORE_FAST gint UA_FIFO = 1 << 7;
            static CORE_FAST gint UA_SOCK = 1 << 8;

            gint booleanAttributes(const File &f) override;

            gbool checkAccess(const File &f, gint access) override;

            gbool setPermission(const File &f, gint access, gbool enable, gbool ownerOnly) override;

            gint posixPermission(const File &f) override;

            gbool setPosixPermission(const File &f, gint perms) override;

            glong lastModifiedTime(const File &f) override;

            glong lastAccessTime(const File &f) override;

            glong creationTime(const File &f) override;

            glong length(const File &f) override;

            gbool createFileExclusively(const String &pathname) override;

            gbool createDirectory(const String &pathname) override;

            gbool markAsTemporary(const String &pathname) override;

            gbool deleteFile(const File &f) override;

            gbool recycleFile(const File &f) override;

            StringArray children(const File &f) override;

            gbool renameFile(const File &source, const File &target) override;

            gbool copyFile(const File &source, const File &target, File::CopyOption options) override;

            gbool moveFile(const File &source, const File &target, File::CopyOption options) override;

            gbool setLastModifiedTime(const File &f, glong time) override;

            gbool setLastAccessTime(const File &f, glong time) override;

            gbool setCreationTime(const File &f, glong time) override;

            gbool setReadOnly(const File &f) override;

            FileArray roots() override;

            glong diskSpace(const File &f, gint type) override;

            gint maxNameLength(const String &path) override;

            gint hash(const File &f) override;

            String userDirectory() override;

            String tempDirectory() override;

            gbool isSameFile(const File &f1, const File &f2) override;

            gbool compare(const File &f1, const File &f2) override;

            gint lastError() override;

        private:
            static String normalize(String const& pathname, gint off);

            static String trimSeparator(String const& s);
        };
    } // io
} // core

#endif //CORE_UNIXFILESYSTEM_H
