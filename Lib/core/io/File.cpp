//
// Created by brunshweeck on 15 sept. 2024.
//

#include "File.h"
#include "NotLinkException.h"
#include "NotDirectoryException.h"
#include "AtomicMoveNotSupportedException.h"

#include <core/IllegalArgumentException.h>
#include <core/StringArray.h>
#include <core/io/FileArray.h>
#include <core/io/FileFilter.h>
#include <core/io/FilenameFilter.h>
#include <core/io/IOException.h>
#include <core/io/FileAlreadyExistsException.h>
#include <core/io/NoSuchFileException.h>
#include <core/misc/Unsafe.h>
#include <core/net/URI.h>
#include <core/net/URISyntaxException.h>
#include <core/random/Random.h>
#include <core/util/Arrays.h>
#include <meta/io/FileSystem.h>


namespace core {
    using namespace random;
    using namespace meta::io;

    CORE_ALIAS(DefaultFileSystem, Class<FileSystem>::Pointer);

    namespace io {
        namespace {
            /**
             * The FileSystem object representing the platform's local file system.
             */
            DefaultFileSystem defaultFileSystem = { };

            FileSystem& fs() {
                if (defaultFileSystem == null) {
                    FileSystem& FS = FileSystem::ofPlatform();
                    if (defaultFileSystem == null) {
                        defaultFileSystem = &FS;
                    } else if (defaultFileSystem != &FS)
                        misc::Unsafe::deleteRegInstance(FS);
                }
                return *defaultFileSystem;
            }
        }

        // Initialize File and Path separators.
        String const File::fileSeparator = String::valueOf(fs().fileSeparator());
        String const File::pathSeparator = String::valueOf(fs().pathSeparator());

        File::File(String const& pathname) {
            File::pathname = fs().normalize(pathname);
            prefixLength = fs().prefixLength(File::pathname);
        }

        File::File(String const& parent, String const& child) {
            pathname = parent.isEmpty()
                           ? fs().normalize(child)
                           : fs().resolve(fs().normalize(parent), fs().normalize(child));
            prefixLength = fs().prefixLength(pathname);
        }

        File::File(File const& parent, String const& child) {
            pathname = parent.pathname.isEmpty()
                           ? fs().normalize(child)
                           : fs().resolve(fs().normalize(parent.pathname), fs().normalize(child));
            prefixLength = fs().prefixLength(pathname);
        }

        File::File(Uri const& uri) {
            // Check our many preconditions
            if (!uri.isAbsolute())
                IllegalArgumentException("URI is not absolute"_S).throws($ftrace());

            if (uri.isOpaque())
                IllegalArgumentException("URI is not hierarchical"_S).throws($ftrace());

            String scheme = uri.scheme();
            if ((scheme.isEmpty()) || !scheme.equalsIgnoreCase("file"_S))
                IllegalArgumentException(R"(URI scheme is not "file")"_S).throws($ftrace());

            if (!uri.rawAuthority().isEmpty())
                IllegalArgumentException("URI has an authority component"_S).throws($ftrace());

            if (!uri.rawFragment().isEmpty())
                IllegalArgumentException("URI has a fragment component"_S).throws($ftrace());

            if (!uri.rawQuery().isEmpty())
                IllegalArgumentException("URI has a query component"_S).throws($ftrace());

            String path = uri.path();
            if (path.isEmpty())
                IllegalArgumentException("URI path component is empty"_S).throws($ftrace());

            // Okay, now initialize
            path = fs().fromURIPath(path);
            CORE_ASSERT(!fileSeparator.isEmpty());
            if (fileSeparator.charAt(0) != '/')
                path = path.replace('/', fileSeparator.charAt(0));

            pathname = fs().normalize(path);
            prefixLength = fs().prefixLength(pathname);
        }

        String File::name() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            CORE_ASSERT(!fileSeparator.isEmpty());
            gint lastSlashIndex = pathname.lastIndexOf(fileSeparator.charAt(0));

            if (lastSlashIndex < prefixLength)
                return pathname.subString(prefixLength);

            return pathname.subString(lastSlashIndex + 1);
        }

        String File::parent() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            CORE_ASSERT(!fileSeparator.isEmpty());
            gint lastSlashIndex = pathname.lastIndexOf(fileSeparator.charAt(0));

            if (lastSlashIndex < prefixLength) {
                if (prefixLength > 0 && pathname.length() >= prefixLength)
                    return pathname.subString(0, prefixLength);

                // Try with absolute path.
                String path = absolutePath();
                gint prefLen = fs().prefixLength(path);

                lastSlashIndex = path.lastIndexOf(fileSeparator.charAt(0));

                if (lastSlashIndex < prefLen)
                    return path.subString(0, prefLen);

                return path.subString(0, lastSlashIndex);
            }

            return pathname.subString(0, lastSlashIndex);
        }

        File File::parentFile() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            CORE_ASSERT(!fileSeparator.isEmpty());
            String parent = File::parent();

            if (prefixLength > 0)
                return File(parent, prefixLength);

            return File(parent, fs().prefixLength(parent));
        }

        String File::path() const {
            return pathname;
        }

        gbool File::isAbsolute() const {
            if (isInvalid())
                return false;

            return fs().isAbsolute(*this);
        }

        String File::absolutePath() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            String absolute = fs().resolve(*this);

            return absolute;
        }

        File File::absoluteFile() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            String path = absolutePath();
            path = fs().normalize(path);

            return File(path, fs().prefixLength(path));
        }

        String File::canonicalPath() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            return fs().canonicalize(pathname);
        }

        File File::canonicalFile() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            String path = canonicalPath();
            path = fs().normalize(path);

            return File(path, fs().prefixLength(path));
        }

        net::URI File::toURI() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            File f = absoluteFile();
            String path = slashify(f.pathname, f.isDirectory());
            if (path.length() >= 2 && path.charAt(0) == '/' && path.charAt(1) == '/')
                path = "//"_S + path;

            try {
                return net::URI("file"_S, ""_S, path, ""_S);
            } catch (net::URISyntaxException const& cse) {
                Error(cse).throws($ftrace()); // Can't happen
            }
        }

        gbool File::isReadable() const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().checkAccess(*this, FileSystem::ACCESS_READ);
            return r;
        }

        gbool File::isWritable() const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().checkAccess(*this, FileSystem::ACCESS_WRITE);
            return r;
        }

        gbool File::isExecutable() const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().checkAccess(*this, FileSystem::ACCESS_EXECUTE);
            return r;
        }

        gbool File::exists() const {
            if (isInvalid())
                return false;

            gbool r = fs().hasBooleanAttributes(*this, FileSystem::BA_EXISTS);
            return r;
        }

        gbool File::exists(gbool followLinks) const {
            if (isInvalid())
                return false;

            if (!followLinks) {
                gbool r = fs().hasBooleanAttributes(*this, FileSystem::BA_EXISTS);
                return r;
            }

            try {
                File f = canonicalFile();
                gbool r = f.exists();

                return r;
            } catch (Throwable const&) { return false; }
        }

        gbool File::isDirectory() const {
            if (isInvalid())
                return false;

            gbool r = fs().hasBooleanAttributes(*this, FileSystem::BA_DIRECTORY);
            return r;
        }

        gbool File::isFile() const {
            if (isInvalid())
                return false;

            gbool r = fs().hasBooleanAttributes(*this, FileSystem::BA_REGULAR);
            return r;
        }

        gbool File::isSymbolicLink() const {
            if (isInvalid())
                return false;

            gbool r = fs().hasBooleanAttributes(*this, FileSystem::BA_SYMLINK);
            return r;
        }

        gbool File::isHidden() const {
            if (isInvalid())
                return false;

            gbool r = fs().hasBooleanAttributes(*this, FileSystem::BA_HIDDEN);
            return r;
        }

        glong File::lastModifiedTime() const {
            if (isInvalid())
                return 0LL;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            glong r = fs().lastModifiedTime(*this);
            return r < 0 ? 0LL : r;
        }

        glong File::lastAccessTime() const {
            if (isInvalid())
                return 0LL;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            glong r = fs().lastAccessTime(*this);
            return r < 0 ? 0LL : r;
        }

        glong File::creationTime() const {
            if (isInvalid())
                return 0LL;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            glong r = fs().creationTime(*this);
            return r < 0 ? 0LL : r;
        }

        glong File::length() const {
            if (isInvalid())
                return 0LL;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            glong r = fs().length(*this);
            return r < 0 ? Long::MAX_VALUE : r;
        }

        gbool File::createFile() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (exists())
                FileAlreadyExistsException(pathname).throws($ftrace());

            gbool r = fs().createFileExclusively(pathname);
            return r;
        }

        gbool File::createDirectory() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (exists())
                FileAlreadyExistsException(pathname).throws($ftrace());

            gbool r = fs().createDirectory(pathname);
            return r;
        }

        gbool File::createSymbolicLink(File const& link) const {
            if (isInvalid() || link.isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
//            if (exists())
//                FileAlreadyExistsException(pathname).throws($ftrace());

            String s = fs().createLink(link.pathname, pathname, FileSystem::SYMBOLIC_LINK);
            return !s.isEmpty();
        }

        gbool File::createLink(File const& link) const {
            if (isInvalid() || link.isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (exists())
                FileAlreadyExistsException(pathname).throws($ftrace());

            String s = fs().createLink(link.pathname, pathname, FileSystem::HARD_LINK);
            return !s.isEmpty();
        }

        File File::createShortcut(File const& dir) const {
            if (isInvalid() || dir.isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            String t = fs().createLink(dir.pathname, pathname, FileSystem::SHORTCUT_LINK);
            return File(t);
        }

        File File::target() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());
            if (!isSymbolicLink())
                NotLinkException(pathname).throws($ftrace());

            String t = fs().readLink(pathname);
            return File(t);
        }

        gbool File::createDirectories() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            gbool r = fs().createDirectory(pathname);

            if (r) return r;

            try {
                File f = canonicalFile();
                File parent = f.parentFile();

                if (parent.exists()) return false;

                return parent.createDirectories();
            } catch (Throwable const&) { return false; }
        }

        gbool File::deleteFile() const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().deleteFile(*this);
            return r;
        }

        gbool File::recycleFile() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            return fs().recycleFile(*this);
        }

        void File::deleteOnExit() const {
            if (isInvalid())
                return;

            fs().markAsTemporary(pathname);
        }

        StringArray File::childPaths() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());
            if (!isDirectory())
                NotDirectoryException(pathname).throws($ftrace());

            StringArray children = fs().children(*this);

            for (String& child : children) {
                child = fs().normalize(child);
            }

            return children;
        }

        StringArray File::childPaths(FilenameFilter const& filter) const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());
            if (!isDirectory())
                NotDirectoryException(pathname).throws($ftrace());

            StringArray children = fs().children(*this);
            StringArray selectedChildren = StringArray(children.length());
            gint i = 0;

            for (String& child : children) {
                if (!filter.accept(*this, child))
                    continue;

                selectedChildren[i] = fs().normalize(child);
                i += 1;
            }

            if (i == selectedChildren.length())
                return selectedChildren;

            StringArray childs = StringArray(i);
            for (int j = 0; j < i; ++j) childs[i] = UNSAFE::moveInstance(selectedChildren[i]);

            return childs;
        }

        FileArray File::childFiles() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());
            if (!isDirectory())
                NotDirectoryException(pathname).throws($ftrace());

            StringArray children = childPaths();
            FileArray childrenFiles = FileArray(children.length());

            for (int i = 0; i < children.length(); ++i) childrenFiles[i] = File(children[i], *this);

            return childrenFiles;
        }

        FileArray File::childFiles(FilenameFilter const& filter) const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());
            if (!isDirectory())
                NotDirectoryException(pathname).throws($ftrace());

            StringArray children = childPaths();
            FileArray childrenFiles = FileArray(children.length());
            gint i = 0;

            for (String& child : children) {
                if (!filter.accept(*this, child))
                    continue;

                childrenFiles[i] = File(child, *this);
                i += 1;
            }

            if (i == childrenFiles.length())
                return childrenFiles;

            FileArray childs = FileArray(i);
            for (int j = 0; j < i; ++j) childs[j] = UNSAFE::moveInstance(childrenFiles[j]);

            return childs;
        }

        FileArray File::childFiles(FileFilter const& filter) const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());
            if (!isDirectory())
                NotDirectoryException(pathname).throws($ftrace());

            StringArray children = childPaths();
            FileArray childrenFiles = FileArray(children.length());
            gint i = 0;

            for (String& child : children) {
                File f = File(child, *this);
                if (!filter.accept(f))
                    continue;

                childrenFiles[i] = UNSAFE::moveInstance(f);
                i += 1;
            }

            if (i == childrenFiles.length())
                return childrenFiles;

            FileArray childs = FileArray(i);
            for (int j = 0; j < i; ++j) childs[j] = UNSAFE::moveInstance(childrenFiles[j]);

            return childs;
        }

        gbool File::renameTo(File const& dest) const {
            if (isInvalid() || dest.isInvalid()) return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().renameFile(*this, dest);

            return r;
        }

        gbool File::copyTo(File const& target, CopyOption options) const {
            if (isInvalid() || target.isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());
            if (isDirectory())
                return false;
            if ((ATOMIC_MOVE & options) == ATOMIC_MOVE)
                AtomicMoveNotSupportedException(pathname,
                                                target.pathname,
                                                "Atomic move not supported for copy").throws($ftrace());
            if ((MOVE_BY_COPY & options) == MOVE_BY_COPY)
                IllegalArgumentException("MOVE_BY_COPY option is not supported for copy").throws($ftrace());
            if ((REPLACE_EXISTING & options) != REPLACE_EXISTING) {
                if (target.exists())
                    FileAlreadyExistsException(target.pathname).throws($ftrace());
            }

            gbool r = fs().copyFile(*this, target, options);

            return r;
        }

        gbool File::moveTo(File const& target, CopyOption options) const {
            if (isInvalid() || target.isInvalid())
                return false;
            if ((REPLACE_EXISTING & options) != REPLACE_EXISTING) {
                if (target.exists())
                    FileAlreadyExistsException(target.pathname).throws($ftrace());
            }

            gbool r = fs().moveFile(*this, target, options);

            return r;
        }

        gbool File::setLastModifiedTime(glong time) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setLastModifiedTime(*this, time);

            return r;
        }

        gbool File::setLastAccessTime(glong time) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setLastAccessTime(*this, time);

            return r;
        }

        gbool File::setCreationTime(glong time) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setCreationTime(*this, time);

            return r;
        }

        gbool File::setReadOnly() const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setReadOnly(*this);

            return r;
        }

        gbool File::setWritable(gbool writable, gbool ownerOnly) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setPermission(*this, FileSystem::ACCESS_WRITE, writable, ownerOnly);

            return r;
        }

        gbool File::setWritable(gbool writable) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setPermission(*this, FileSystem::ACCESS_WRITE, writable, true);

            return r;
        }

        gbool File::setReadable(gbool readable, gbool ownerOnly) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setPermission(*this, FileSystem::ACCESS_READ, readable, ownerOnly);

            return r;
        }

        gbool File::setReadable(gbool readable) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setPermission(*this, FileSystem::ACCESS_READ, readable, true);

            return r;
        }

        gbool File::setExecutable(gbool executable, gbool ownerOnly) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setPermission(*this, FileSystem::ACCESS_EXECUTE, executable, ownerOnly);

            return r;
        }

        gbool File::setExecutable(gbool executable) const {
            if (isInvalid())
                return false;
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setPermission(*this, FileSystem::ACCESS_EXECUTE, executable, true);

            return r;
        }

        File::Permission File::permissions(gbool followLinks) const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gint perms = fs().posixPermission(*this);

            return (Permission) (perms & Permission::ALL);
        }

        gbool File::setPermissions(Permission perms) const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());
            if (!exists())
                NoSuchFileException(pathname).throws($ftrace());

            gbool r = fs().setPosixPermission(*this, CORE_CAST(gint, perms));

            return r;
        }

        FileArray File::roots() {
            return fs().roots();
        }

        glong File::totalSpace() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            glong r = fs().diskSpace(*this, FileSystem::SPACE_TOTAL);

            return r < 0 ? Long::MAX_VALUE : r;
        }

        glong File::freeSpace() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            glong r = fs().diskSpace(*this, FileSystem::SPACE_FREE);

            return r < 0 ? Long::MAX_VALUE : r;
        }

        glong File::usableSpace() const {
            if (isInvalid())
                IOException("Invalid file path."_S).throws($ftrace());

            glong r = fs().diskSpace(*this, FileSystem::SPACE_USABLE);

            return r < 0 ? Long::MAX_VALUE : r;
        }

        File File::createTempFile(String const& prefix, String const& suffix, File const& directory) {
            if (prefix.length() < 3) {
                IllegalArgumentException("Prefix string \""_S + prefix +
                    "\" too short: length must be at least 3").throws($ftrace());
            }

            File tmpDir = File(fs().tempDirectory());

            File f;
            do {
                f = generateFile(prefix, suffix, directory);
            } while (fs().hasBooleanAttributes(f, FileSystem::BA_EXISTS));

            if (!fs().createFileExclusively(f.pathname))
                IOException("Unable to create temporary file"_S).throws($ftrace());

            return f;
        }

        File File::createTempFile(String const& prefix, String const& suffix) {
            return createTempFile(prefix, suffix, File(fs().tempDirectory()));
        }

        File File::createTempFile(String const& prefix) {
            return createTempFile(prefix, ".tmp"_S);
        }

        gint File::compareTo(const File& pathname) const {
            return fs().compare(*this, pathname);
        }

        gbool File::equals(const Object& obj) const {
            return this == &obj || Class<File>::hasInstance(obj) && compareTo(CORE_XCAST(File const, obj)) == 0;
        }

        gbool File::isSameTo(File const& other) const {
            return fs().isSameFile(*this, other);
        }

        gint File::hash() const {
            return fs().hash(*this);
        }

        String File::toString() const { return pathname; }

        Object& File::clone() const {
            try {
                return UNSAFE::newInstance<File>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gbool File::isInvalid() const {
            PathStatus s = status;
            if (s != INVALID && s != CHECKED) {
                s = fs().isInvalid(*this) ? INVALID : CHECKED;
                status = s;
            }

            return s == INVALID;
        }

        File::File(String pathname, gint prefixLength)
            : pathname(UNSAFE::moveInstance(pathname)), prefixLength(prefixLength) {}

        File::File(const String& child, File const& parent) {
            CORE_ASSERT(!parent.pathname.isEmpty());
            pathname = fs().resolve(parent.pathname, child);
            prefixLength = parent.prefixLength;
        }

        String File::slashify(String const& path, gbool isDir) {
            String p = path;
            if (fileSeparator.charAt(0) != '/')
                p = p.replace(File::fileSeparator.charAt(0), '/');
            if (!p.startsWith("/"_S))
                p = "/" + p;
            if (!p.endsWith("/"_S) && isDir)
                p = p + "/";
            return p;
        }

        File File::generateFile(const String& prefix, String const& suffix, File const& dir) {
            static Random rnd = Random();
            CORE_IGNORE(rnd.nextInt());

            glong r = rnd.nextLong();
            while (r == 0)
                r = rnd.nextLong();

            String rs = Long::toUnsignedString(r, Character::MAX_RADIX);

            // Use only the file name from the supplied prefix
            String p = File(prefix).name();

            gint prefixLength = p.length();
            gint rsLength = rs.length();
            gint suffixLength = suffix.length();

            String name;
            gint nameMax = fs().maxNameLength(dir.pathname);
            int excess = prefixLength + rsLength + suffixLength - nameMax;
            if (excess <= 0) {
                name = prefix + rs + suffix;
            } else {
                // Name exceeds the maximum path component length: shorten it

                // Attempt to shorten the prefix length to no less than 3
                prefixLength = shortenSubName(prefixLength, excess, 3);
                excess = prefixLength + rsLength + suffixLength - nameMax;

                if (excess > 0) {
                    // Attempt to shorten the suffix length to no less than
                    // 0 or 4 depending on whether it begins with a dot ('.')
                    suffixLength = shortenSubName(suffixLength, excess,
                                                  suffix.indexOf('.') == 0 ? 4 : 0);
                    suffixLength = shortenSubName(suffixLength, excess, 3);
                    excess = prefixLength + rsLength + suffixLength - nameMax;
                }

                if (excess > 0 && excess <= rsLength - 5) {
                    // Attempt to shorten the random character string length
                    // to no less than 5
                    rsLength = shortenSubName(rsLength, excess, 5);
                }

                XString sb = XString(prefixLength + rsLength + suffixLength);
                sb.append(prefixLength < prefix.length() ? prefix.subString(0, prefixLength) : prefix);
                sb.append(rsLength < rs.length() ? rs.subString(0, rsLength) : rs);
                sb.append(suffixLength < suffix.length() ? suffix.subString(0, suffixLength) : suffix);
                name = sb.toString();
            }

            // Normalize the path component
            name = fs().normalize(name);

            File f = File(dir, name);
            if (!name.equals(f.name()) || f.isInvalid())
                IOException("Unable to create temporary file"_S).throws($ftrace());

            return f;
        }

        gint File::shortenSubName(gint subNameLength, gint excess, gint nameMin) {
            int newLength = Math::max(nameMin, subNameLength - excess);
            if (newLength < subNameLength) {
                return newLength;
            }
            return subNameLength;
        }

        File::CopyOption operator|(File::CopyOption lhs, File::CopyOption rhs) {
            return (File::CopyOption) ((gint) lhs | (gint) rhs);
        }
    } // io
} // core
