//
// Created by brunshweeck on 16 sept. 2024.
//

#ifndef CORE24_FILESYSTEM_H
#define CORE24_FILESYSTEM_H

#include <core/io/File.h>


namespace meta {
    using namespace core;

    namespace io {
        using namespace core::io;

        /**
         * abstract class for the local filesystem abstraction.
         */
        class FileSystem : public Object {
        protected:
            CORE_ALIAS(UNSAFE, misc::Unsafe);

        public:
            /* -- Normalization and construction -- */

            /**
             * Return the local filesystem's name-separator character.
             */
            virtual gchar fileSeparator() = 0;

            /**
             * Return the local filesystem's path-separator character.
             */
            virtual gchar pathSeparator() = 0;

            /**
             * Convert the given pathname string to normal form.  If the string is
             * already in normal form then it is simply returned.
             */
            virtual String normalize(String const &path) = 0;

            /**
             * Compute the length of this pathname string's prefix.  The pathname
             * string must be in normal form.
             */
            virtual gint prefixLength(String const &path) = 0;

            /**
             * Resolve the child pathname string against the parent.
             * Both strings must be in normal form, and the result
             * will be in normal form.
             */
            virtual String resolve(String const &parent, String const &child) = 0;

            /**
             * Return the parent pathname string to be used when the parent-directory
             * argument in one of the two-argument File constructors is the empty
             * pathname.
             */
            virtual String defaultParent() = 0;

            /**
             * Post-process the given URI path string if necessary.  This is used on
             * win32, e.g., to transform "/c:/foo" into "c:/foo".  The path string
             * still has slash separators; code in the File class will translate them
             * after this method returns.
             */
            virtual String fromURIPath(String const &path) = 0;


            /* -- Path operations -- */

            /**
             * Tell whether the given abstract pathname is absolute.
             */
            virtual gbool isAbsolute(File const &f) = 0;

            /**
             * Tell whether the given abstract pathname is invalid.
             */
            virtual gbool isInvalid(File const &f) = 0;

            /**
             * Resolve the given abstract pathname into absolute form.  Invoked by the
             * absolutePath and canonicalPath methods in the File class.
             */
            virtual String resolve(File const &f) = 0;

            virtual String canonicalize(String const &path) = 0;

            enum LinkType {
                SYMBOLIC_LINK,
                HARD_LINK,
                SHORTCUT_LINK,
            };

            virtual String createLink(String const &link, String const &target, LinkType type) = 0;

            virtual String readLink(String const &link) = 0;


            /* -- Attribute accessors -- */

            /* Constants for simple boolean attributes */
            enum {
                BA_EXISTS = 1,
                BA_REGULAR = 2,
                BA_DIRECTORY = 4,
                BA_HIDDEN = 8,
                BA_SYMLINK = 16,
            };

            /**
             * Return the simple boolean attributes for the file or directory denoted
             * by the given abstract pathname, or zero if it does not exist or some
             * other I/O error occurs.
             */
            virtual gint booleanAttributes(File const &f) = 0;

            /**
             * Checks if all the given boolean attributes are true for the file or
             * directory denoted by the given abstract pathname. False if it does not
             * exist or some other I/O error occurs.
             */
            gbool hasBooleanAttributes(File const &f, gint attributes) {
                return (booleanAttributes(f) & attributes) == attributes;
            }

            enum {
                ACCESS_READ = 004,
                ACCESS_WRITE = 002,
                ACCESS_EXECUTE = 001,
            };

            /**
             * Check whether the file or directory denoted by the given abstract
             * pathname may be accessed by this process.  The second argument specifies
             * which access, ACCESS_READ, ACCESS_WRITE or ACCESS_EXECUTE, to check.
             * Return false if access is denied or an I/O error occurs
             */
            virtual gbool checkAccess(File const &f, gint access) = 0;

            /**
             * Set on or off the access permission (to owner only or to all) to the file
             * or directory denoted by the given abstract pathname, based on the parameters
             * enable, access and ownerOnly.
             */
            virtual gbool setPermission(File const &f, gint access, gbool enable, gbool ownerOnly) = 0;

            /**
             * Set the posix access permission to the file
             * or directory denoted by the given abstract pathname.
             */
            virtual gint posixPermission(File const &f) = 0;

            /**
             * Set the posix access permission to the file
             * or directory denoted by the given abstract pathname.
             */
            virtual gbool setPosixPermission(File const &f, gint perms) = 0;

            /**
             * Return the time at which the file or directory denoted by the given
             * abstract pathname was last modified, or zero if it does not exist or
             * some other I/O error occurs.
             */
            virtual glong lastModifiedTime(File const &f) = 0;

            /**
             * Return the time at which the file or directory denoted by the given
             * abstract pathname was last access, or zero if it does not exist or
             * some other I/O error occurs.
             */
            virtual glong lastAccessTime(File const &f) = 0;

            /**
             * Return the time at which the file or directory denoted by the given
             * abstract pathname was created, or zero if it does not exist or
             * some other I/O error occurs.
             */
            virtual glong creationTime(File const &f) = 0;

            /**
             * Return the length in bytes of the file denoted by the given abstract
             * pathname, or zero if it does not exist, is a directory, or some other
             * I/O error occurs.
             */
            virtual glong length(File const &f) = 0;


            /* -- File operations -- */

            /**
             * Create a new empty file with the given pathname.  Return
             * @c true if the file was created and @c false if a
             * file or directory with the given pathname already exists.  Throw an
             * IOException if an I/O error occurs.
             */
            virtual gbool createFileExclusively(String const &pathname) = 0;

            virtual gbool createDirectory(String const &pathname) = 0;

            virtual gbool markAsTemporary(String const &pathname) = 0;

            /**
             * Delete the file or directory denoted by the given abstract pathname,
             * returning @c true if and only if the operation succeeds.
             */
            virtual gbool deleteFile(File const &f) = 0;

            virtual gbool recycleFile(File const &f) = 0;

            /**
             * List the elements of the directory denoted by the given abstract
             * pathname.  Return an array of strings naming the elements of the
             * directory if successful; otherwise, return empty array
             */
            virtual StringArray children(File const &f) = 0;

            /**
             * Rename the file or directory denoted by the first abstract pathname to
             * the second abstract pathname, returning @c true if and only if
             * the operation succeeds.
             */
            virtual gbool renameFile(File const &source, File const &target) = 0;

            /**
             * Copy the file or directory denoted by the first abstract pathname to
             * the second abstract pathname, returning @c true if and only if
             * the operation succeeds.
             */
            virtual gbool copyFile(File const &source, File const &target, File::CopyOption options) = 0;

            /**
             * Move or Rename the file or directory denoted by the first abstract pathname to
             * the second abstract pathname, returning @c true if and only if
             * the operation succeeds.
             */
            virtual gbool moveFile(File const &source, File const &target, File::CopyOption options) = 0;

            /**
             * Set the last-modified time of the file or directory denoted by the
             * given abstract pathname, returning @c true if and only if the
             * operation succeeds.
             */
            virtual gbool setLastModifiedTime(File const &f, glong time) = 0;

            /**
             * Set the last-access time of the file or directory denoted by the
             * given abstract pathname, returning @c true if and only if the
             * operation succeeds.
             */
            virtual gbool setLastAccessTime(File const &f, glong time) = 0;

            /**
             * Set the creation time of the file or directory denoted by the
             * given abstract pathname, returning @c true if and only if the
             * operation succeeds.
             */
            virtual gbool setCreationTime(File const &f, glong time) = 0;

            /**
             * Mark the file or directory denoted by the given abstract pathname as
             * read-only, returning @c true if and only if the operation
             * succeeds.
             */
            virtual gbool setReadOnly(File const &f) = 0;


            /* -- Filesystem interface -- */

            /**
             * List the available filesystem roots.
             */
            virtual FileArray roots() = 0;

            /* -- Disk usage -- */
            enum {
                SPACE_TOTAL,
                SPACE_FREE,
                SPACE_USABLE,
            };

            virtual glong diskSpace(File const &f, gint type) = 0;

            /* -- Basic infrastructure -- */

            /**
             * Retrieve the maximum length of a component of a file path.
             *
             * @return The maximum length of a file path component.
             */
            virtual gint maxNameLength(String const &path) = 0;

            /**
             * Compare two abstract pathnames lexicographically.
             */
            virtual gbool compare(File const &f1, File const &f2) = 0;

            virtual gbool isSameFile(File const &f1, File const &f2) = 0;

            /**
             * Compute the hash code of an abstract pathname.
             */
            virtual gint hash(File const &f) = 0;

            virtual String userDirectory() = 0;

            virtual String tempDirectory() = 0;

            virtual gint lastError() = 0;

            /**
             * Return an instance of local filesystem.
             *
             * @return the instance of filesystem.
             */
            static FileSystem &ofPlatform();
        };
    } // io
} // core

#endif //CORE24_FILESYSTEM_H
