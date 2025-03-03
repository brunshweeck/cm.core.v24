//
// Created by brunshweeck on 15 sept. 2024.
//

#ifndef CORE24_FILE_H
#define CORE24_FILE_H

#include <core/lang/String.h>

namespace core {
  namespace io {
    /**
     * An abstract representation of file and directory pathnames.
     *
     * <p>
     * User interfaces and operating systems use system-dependent <em>pathname
     * strings</em> to name files and directories.  This class presents an
     * abstract, system-independent view of hierarchical pathnames.  An
     * <em>abstract pathname</em> has two components:
     *
     * @li An optional system-dependent <em>prefix</em> string,
     *      such as a disk-drive specifier, @c "/" for the UNIX root
     *      directory, or @c "\\\\" for a Microsoft Windows UNC pathname, and
     * @li A sequence of zero or more string <em>names</em>.
     *
     *
     * The first name in an abstract pathname may be a directory name or, in the
     * case of Microsoft Windows UNC pathnames, a hostname.  Each subsequent name
     * in an abstract pathname denotes a directory; the last name may denote
     * either a directory or a file.  The <em>empty</em> abstract pathname has no
     * prefix and an empty name sequence.
     * </p>
     * <p>
     * The conversion of a pathname string to or from an abstract pathname is
     * inherently system-dependent.  When an abstract pathname is converted into a
     * pathname string, each name is separated from the next by a single copy of
     * the default <em>separator character</em>.  The default name-separator
     * character is defined by the system property @c "file.separator", and
     * is made available in the public static field @b separator of this class.
     * When a pathname string is converted into an abstract pathname, the names
     * within it may be separated by the default name-separator character or by any
     * other name-separator character that is supported by the underlying system.
     * </p>
     * <p>
     * A pathname, whether abstract or in string form, may be either
     * <em>absolute</em> or <em>relative</em>.  An absolute pathname is complete in
     * that no other information is required in order to locate the file that it
     * denotes.  A relative pathname, in contrast, must be interpreted in terms of
     * information taken from some other pathname.  By default, the classes in the
     * @c core::io namespace always resolve relative pathnames against the
     * current user directory.  This directory is named by the system property
     * @c "user.dir".
     * </p>
     * <p>
     * The <em>parent</em> of an abstract pathname may be obtained by invoking
     * the @b parent method of this class and consists of the pathname's
     * prefix and each name in the pathname's name sequence except for the last.
     * Each directory's absolute pathname is an ancestor of any @c File
     * object with an absolute abstract pathname which begins with the directory's
     * absolute pathname.  For example, the directory denoted by the abstract
     * pathname @c "/usr" is an ancestor of the directory denoted by the
     * pathname @c "/usr/local/bin".
     * </p>
     * <p>
     * The prefix concept is used to handle root directories on UNIX platforms,
     * and drive specifiers, root directories and UNC pathnames on Microsoft Windows platforms,
     * as follows:
     *
     * @li For UNIX platforms, the prefix of an absolute pathname is always
     * @c "/".  Relative pathnames have no prefix.  The abstract pathname
     * denoting the root directory has the prefix @c "/" and an empty
     * name sequence.
     *
     * @li For Microsoft Windows platforms, the prefix of a pathname that contains a drive
     * specifier consists of the drive letter followed by @c ":" and
     * possibly followed by @c "\\" if the pathname is absolute.  The
     * prefix of a UNC pathname is @c "\\\\"; the hostname and the share
     * name are the first two names in the name sequence.  A relative pathname that
     * does not specify a drive has no prefix.
     * </p>
     * <p>
     * Instances of this class may or may not denote an actual file-system
     * object such as a file or a directory.  If it does denote such an object
     * then that object resides in a <i>partition</i>.  A partition is an
     * operating system-specific portion of storage for a file system.  A single
     * storage device (e.g. a physical disk-drive, flash memory, CD-ROM) may
     * contain multiple partitions.  The object, if any, will reside on the
     * partition <a id="partName">named</a> by some ancestor of the absolute
     * form of this pathname.
     * </p>
     * <p>
     * A file system may implement restrictions to certain operations on the
     * actual file-system object, such as reading, writing, and executing.  These
     * restrictions are collectively known as <i>access permissions</i>.  The file
     * system may have multiple sets of access permissions on a single object.
     * For example, one set may apply to the object's <i>owner</i>, and another
     * may apply to all other users.  The access permissions on an object may
     * cause some methods in this class to fail.
     * </p>
     * <p>
     * Instances of the @c File class are immutable; that is, once
     * created, the abstract pathname represented by a @c File object
     * will never change.
     * </p>
     */
    class File final : public Comparable<File> {
      CORE_ALIAS(Uri, net::URI);
      CORE_ALIAS(OptionalError, util::Optional<Throwable>);

      enum PathStatus { INVALID = -1, CHECKED = 1 };

      /**
       * This abstract pathname's normalized pathname string. A normalized
       * pathname string uses the default name-separator character and does not
       * contain any duplicate or redundant separators.
       */
      String pathname;

      /**
       * The flag indicating whether the file path is invalid.
       */
      PathStatus mutable status = { };

      /**
       * The length of this abstract pathname's prefix, or zero if it has no
       * prefix.
       */
      gint prefixLength = 0;

    public:
      /**
       * An object that configures how to copy or move a file.
       *
       * <p>
       * Objects of this type may be used with the @b File::copyTo(File, CopyOption...),
       * @b File::moveTo(File,CopyOption...) methods to configure how a file is
       * copied or moved.
       * </p>
       */
      enum CopyOption {
        /**
         * No option
         */
        DEFAULT_OPTION,

        /**
         * Replace an existing file if it exists.
         */
        REPLACE_EXISTING = 1,

        /**
         * Copy attributes to the new file.
         */
        COPY_ATTRIBUTES = 2,

        /**
         * Move the file as an atomic file system operation.
         */
        ATOMIC_MOVE = 4,

        /**
         * Do not follow symbolic links.
         */
        NOFOLLOW_LINKS = 8,

        /**
         * Force if possible the move option
         * with copy followed by deletion.
         */
        MOVE_BY_COPY = 16,
      };

      /**
       * Defines the bits for use with the @b permissions attribute.
       *
       * <p>
       * The @b FilePermissions class defines methods for manipulating
       * set of permissions.
       * </p>
       */
      enum Permission {
        /**
         * Read permission, owner.
         */
        OWNER_READ = 0400,

        /**
         * Write permission, owner.
         */
        OWNER_WRITE = 0200,

        /**
         * Execute/search permission, owner.
         */
        OWNER_EXECUTE = 0100,

        /**
         * Read permission, group.
         */
        GROUP_READ = 0040,

        /**
         * Write permission, group.
         */
        GROUP_WRITE = 0020,

        /**
         * Execute/search permission, group.
         */
        GROUP_EXECUTE = 0010,

        /**
         * Read permission, others.
         */
        OTHERS_READ = 0004,

        /**
         * Write permission, others.
         */
        OTHERS_WRITE = 0002,

        /**
         * Execute/search permission, others.
         */
        OTHERS_EXECUTE = 0001,

        // Combinations

        /**
         * Read and Write permission, owner.
         */
        OWNER_READ_WRITE = OWNER_READ | OWNER_WRITE,

        /**
         * Read and Execute/search permission, owner.
         */
        OWNER_READ_EXECUTE = OWNER_READ | OWNER_EXECUTE,

        /**
         * Write and Execute/search permission, owner.
         */
        OWNER_WRITE_EXECUTE = OWNER_WRITE | OWNER_EXECUTE,

        /**
         * All permission, owner.
         */
        OWNER_ALL = OWNER_READ | OWNER_WRITE | OWNER_EXECUTE,

        /**
         * Read and Write permission, group.
         */
        GROUP_READ_WRITE = GROUP_READ | GROUP_WRITE,

        /**
         * Read and Execute/search permission, group.
         */
        GROUP_READ_EXECUTE = GROUP_READ | GROUP_EXECUTE,

        /**
         * Write and Execute/search permission, group.
         */
        GROUP_WRITE_EXECUTE = GROUP_WRITE | GROUP_EXECUTE,

        /**
         * All permission, group.
         */
        GROUP_ALL = GROUP_READ | GROUP_WRITE | GROUP_EXECUTE,

        /**
         * Read and Write permission, others.
         */
        OTHERS_READ_WRITE = OTHERS_READ | OTHERS_WRITE,

        /**
         * Read and Execute/search permission, others.
         */
        OTHERS_READ_EXECUTE = OTHERS_READ | OTHERS_EXECUTE,

        /**
         * Write and Execute/search permission, others.
         */
        OTHERS_WRITE_EXECUTE = OTHERS_WRITE | OTHERS_EXECUTE,

        /**
         * All permission, others.
         */
        OTHERS_ALL = OTHERS_READ | OTHERS_WRITE | OTHERS_EXECUTE,

        /**
         * All permission, Everyone.
         */
        ALL = OWNER_ALL | GROUP_ALL | OTHERS_ALL,
      };

      /**
       * Defines access modes used to test the accessibility of a file.
       */
      enum AccessMode {
        /**
         * Test read access.
         */
        READ = OWNER_READ,

        /**
         * Test write access.
         */
        WRITE = OWNER_WRITE,

        /**
         * Test execute access.
         */
        EXECUTE = OWNER_EXECUTE,

        /**
         * Test read and write access.
         */
        READ_WRITE = READ | WRITE,

        /**
         * Test read and execute access.
         */
        READ_EXECUTE = READ | EXECUTE,

        /**
         * Test write and execute access.
         */
        WRITE_EXECUTE = WRITE | EXECUTE,

        /**
         * Test all access.
         */
        FULL = READ | WRITE | EXECUTE,
      };

      /**
       * The system-dependent default name-separator character, represented as a
       * string for convenience. This string contains a single character
       * and is initialized to contain the first character of the value of the system
       * property @c "file.separator".  On UNIX systems the value of this
       * field is @c '/'; on Microsoft Windows systems it is @c '\\'.
       */
      static String const fileSeparator;

      /**
       * The system-dependent path-separator character, represented as a string
       * for convenience.  This string contains a single character and
       * is initialized to contain the first character of the value of the system
       * property @c "path.separator".  This character is used to
       * separate filenames in a sequence of files given as a <em>path list</em>.
       * On UNIX systems, this character is @c ':'; on Microsoft Windows systems it
       * is @c ';'.
       */
      static String const pathSeparator;


      /* -- Constructors -- */

      /**
       * Creates a new @c File instance by converting the given
       * pathname string into an abstract pathname.  If the given string is
       * the empty string, then the result is the empty abstract pathname.
       *
       * @param   pathname  A pathname string
       */
      CORE_EXPLICIT File(String const& pathname);

      /**
       * Creates a new @c File instance from a parent pathname string
       * and a child pathname string.
       *
       * <p>
       * If @c parent is @em empty then the new
       * @c File instance is created as if by invoking the
       * single-argument @c File constructor on the given
       * @c child pathname string.
       * </p>
       * <p>
       * Otherwise, the @c parent pathname string is taken to denote
       * a directory, and the @c child pathname string is taken to
       * denote either a directory or a file.  If the @c child pathname
       * string is absolute then it is converted into a relative pathname in a
       * system-dependent way.  If @c parent is the empty string then
       * the new @c File instance is created by converting
       * @c child into an abstract pathname and resolving the result
       * against a system-dependent default directory.  Otherwise, each pathname
       * string is converted into an abstract pathname and the child abstract
       * pathname is resolved against the parent.
       * </p>
       * @param   parent  The parent pathname string
       * @param   child   The child pathname string
       */
      CORE_EXPLICIT File(String const& parent, String const& child);

      /**
       * Creates a new @c File instance from a parent abstract
       * pathname and a child pathname string.
       *
       * <p>
       * The @c parent abstract pathname is taken to
       * denote a directory, and the @c child pathname string is taken
       * to denote either a directory or a file.  If the @c child
       * pathname string is absolute then it is converted into a relative
       * pathname in a system-dependent way.  If @c parent is the empty
       * abstract pathname then the new @c File instance is created by
       * converting @c child into an abstract pathname and resolving
       * the result against a system-dependent default directory.  Otherwise, each
       * pathname string is converted into an abstract pathname and the child
       * abstract pathname is resolved against the parent.
       * </p>
       * @param   parent  The parent abstract pathname
       * @param   child   The child pathname string
       */
      CORE_EXPLICIT File(File const& parent, String const& child);

      /**
       * Creates a new @c File instance by converting the given
       * @c "file:" URI into an abstract pathname.
       *
       * <p>
       * The exact form of a @c "file:" URI is system-dependent, hence
       * the transformation performed by this constructor is also
       * system-dependent.
       * </p>
       * <p>
       * For a given abstract pathname <i>f</i> it is guaranteed that
       *
       * <code>
       *   File(f.toURI()).equals(f.absoluteFile())
       * </code>
       *
       * so long as the original abstract pathname, the URI, and the new abstract
       * pathname are all created in (possibly different invocations of) the same
       * Program.  This relationship typically does not hold,
       * however, when a @c "file:" URI that is created in a virtual machine
       * on one operating system is converted into an abstract pathname in a
       * virtual machine on a different operating system.
       * </p>
       * @param  uri
       *         An absolute, hierarchical URI with a scheme equal to
       *         @c "file", a non-empty path component, and undefined
       *         authority, query, and fragment components
       *
       * @throws  IllegalArgumentException
       *          If the preconditions on the parameter do not hold
       *
       * @see File::toURI()
       * @see net::URI
       */
      CORE_EXPLICIT File(Uri const& uri);


      /* -- Path-component accessors -- */

      /**
       * Returns the name of the file or directory denoted by this abstract
       * pathname.  This is just the last name in the pathname's name
       * sequence.  If the pathname's name sequence is empty, then the empty
       * string is returned.
       *
       * @return  The name of the file or directory denoted by this abstract
       *          pathname, or the empty string if this pathname's name sequence
       *          is empty
       */
      String name() const;

      /**
       * Returns the pathname string of this abstract pathname's parent, or
       * @c "" if this pathname does not name a parent directory.
       *
       * <p>
       * The <em>parent</em> of an abstract pathname consists of the
       * pathname's prefix, if any, and each name in the pathname's name
       * sequence except for the last.  If the name sequence is empty then
       * the pathname does not name a parent directory.
       * </p>
       * @return  The pathname string of the parent directory named by this
       *          abstract pathname, or @c "" if this pathname
       *          does not name a parent
       */
      String parent() const;

      /**
       * Returns the abstract pathname of this abstract pathname's parent,
       * or @c File("") if this pathname does not name a parent
       *
       * <p>
       * The <em>parent</em> of an abstract pathname consists of the
       * pathname's prefix, if any, and each name in the pathname's name
       * sequence except for the last.  If the name sequence is empty then
       * the pathname does not name a parent directory.
       * </p>
       * @return  The abstract pathname of the parent directory named by this
       *          abstract pathname, or @c File("") if this pathname
       *          does not name a parent
       */
      File parentFile() const;

      /**
       * Converts this abstract pathname into a pathname string.  The resulting
       * string uses the <b> default name-separator character</b> to
       * separate the names in the name sequence.
       *
       * @return  The string form of this abstract pathname
       */
      String path() const;


      /* -- Path operations -- */

      /**
       * Tests whether this abstract pathname is absolute.  The definition of
       * absolute pathname is system dependent.  On UNIX systems, a pathname is
       * absolute if its prefix is @c "/".  On Microsoft Windows systems, a
       * pathname is absolute if its prefix is a drive specifier followed by
       * @c "\\", or if its prefix is @c "\\\\".
       *
       * @return  @c true if this abstract pathname is absolute,
       *          @c false otherwise
       */
      gbool isAbsolute() const;

      /**
       * Returns the absolute pathname string of this abstract pathname.
       *
       * <p>
       * If this abstract pathname is already absolute, then the pathname
       * string is simply returned as if by the @b path method.
       * If this abstract pathname is the empty abstract pathname then
       * the pathname string of the current user directory, which is named by the
       * system property @c "user.dir", is returned.  Otherwise, this
       * pathname is resolved in a system-dependent way.  On UNIX systems, a
       * relative pathname is made absolute by resolving it against the current
       * user directory.  On Microsoft Windows systems, a relative pathname is made absolute
       * by resolving it against the current directory of the drive named by the
       * pathname, if any; is not, it is resolved against the current user
       * directory.
       * </p>
       * @return  The absolute pathname string denoting the same file or
       *          directory as this abstract pathname
       *
       * @throws  SecurityException
       *          If a required system property value cannot be accessed.
       *
       * @see     File::isAbsolute()
       */
      String absolutePath() const;

      /**
       * Returns the absolute form of this abstract pathname.  Equivalent to
       * <code>File(absolutePath())</code>.
       *
       * @return  The absolute abstract pathname denoting the same file or
       *          directory as this abstract pathname
       *
       * @throws  SecurityException
       *          If a required system property value cannot be accessed.
       */
      File absoluteFile() const;

      /**
       * Returns the canonical pathname string of this abstract pathname.
       *
       * <p>
       * A canonical pathname is both absolute and unique.  The precise
       * definition of canonical form is system-dependent.  This method first
       * converts this pathname to absolute form if necessary, as if by invoking the
       * @b absolutePath method, and then maps it to its unique form in a
       * system-dependent way.  This typically involves removing redundant names
       * such as @c "." and @c ".." from the pathname, resolving
       * symbolic links (on UNIX platforms), and converting drive letters to a
       * standard case (on Microsoft Windows platforms).
       * </p>
       * <p>
       * Every pathname that denotes an existing file or directory has a
       * unique canonical form.  Every pathname that denotes a nonexistent file
       * or directory also has a unique canonical form.  The canonical form of
       * the pathname of a nonexistent file or directory may be different from
       * the canonical form of the same pathname after the file or directory is
       * created.  Similarly, the canonical form of the pathname of an existing
       * file or directory may be different from the canonical form of the same
       * pathname after the file or directory is deleted.
       * </p>
       * @return  The canonical pathname string denoting the same file or
       *          directory as this abstract pathname
       *
       * @throws  IOException
       *          If an I/O error occurs, which is possible because the
       *          construction of the canonical pathname may require
       *          filesystem queries
       *
       * @throws  SecurityException
       *          If a required system property value cannot be accessed.
       *
       * @see     Path::toRealPath
       */
      String canonicalPath() const;

      /**
       * Returns the canonical form of this abstract pathname.  Equivalent to
       * <code> File(canonicalPath())</code>.
       *
       * @return  The canonical pathname string denoting the same file or
       *          directory as this abstract pathname
       *
       * @throws  IOException
       *          If an I/O error occurs, which is possible because the
       *          construction of the canonical pathname may require
       *          filesystem queries
       *
       * @throws  SecurityException
       *          If a required system property value cannot be accessed.
       *
       * @see     Path::toRealPath
       */
      File canonicalFile() const;

      /**
       * Constructs a @c "file:" URI that represents this abstract pathname.
       *
       * <p>
       * The exact form of the URI is system-dependent.  If it can be
       * determined that the file denoted by this abstract pathname is a
       * directory, then the resulting URI will end with a slash.
       * </p>
       * <p>
       * For a given abstract pathname <i>f</i>, it is guaranteed that
       *
       * <code>
       *  File(f.toURI()).equals(f.absoluteFile())
       * </code>
       *
       * so long as the original abstract pathname, the URI, and the new abstract
       * pathname are all created in (possibly different invocations of) the same
       * Program.  Due to the system-dependent nature of abstract
       * pathnames, however, this relationship typically does not hold when a
       * @c "file:" URI that is created in a virtual machine on one operating
       * system is converted into an abstract pathname in a virtual machine on a
       * different operating system.
       * </p>
       * <p>
       * Note that when this abstract pathname represents a UNC pathname then
       * all components of the UNC (including the server name component) are encoded
       * in the @c URI path. The authority component is undefined, meaning
       * that it is represented as @c "". The @b Path class defines the
       * @b toUri method to encode the server name in the authority
       * component of the resulting @c URI. The @b toPath method
       * may be used to obtain a @c Path representing this abstract pathname.
       * </p>
       * @return  An absolute, hierarchical URI with a scheme equal to
       *          @c "file", a path representing this abstract pathname,
       *          and undefined authority, query, and fragment components
       * @throws SecurityException If a required system property value cannot
       *                          be accessed.
       *
       * @see File(net::URI)
       * @see net::URI
       * @see net::URI::toURL()
       */
      net::URI toURI() const;


      /* -- Attribute accessors -- */

      /**
       * Tests whether the application can read the file denoted by this
       * abstract pathname.
       *
       * @return  @c true if and only if the file specified by this
       *          abstract pathname exists <em>and</em> can be read by the
       *          application; @c false otherwise
       */
      gbool isReadable() const;

      /**
       * Tests whether the application can modify the file denoted by this
       * abstract pathname.
       *
       * @return  @c true if and only if the file system actually
       *          contains a file denoted by this abstract pathname <em>and</em>
       *          the application is allowed to write to the file;
       *          @c false otherwise.
       */
      gbool isWritable() const;

      /**
       * Tests whether the application can execute the file denoted by this
       * abstract pathname.
       *
       * @return  @c true if and only if the abstract pathname exists
       *          <em>and</em> the application is allowed to execute the file
       */
      gbool isExecutable() const;

      /**
       * Tests whether the file or directory denoted by this abstract pathname
       * exists.
       *
       * @return  @c true if and only if the file or directory denoted
       *          by this abstract pathname exists; @c false otherwise or directory
       */
      gbool exists() const;

      /**
       * Tests whether the file or directory denoted by this abstract pathname
       * exists.
       *
       * <p>
       * The @c followLinks parameter may be used to indicate how symbolic links
       * are handled for the case that the file is a symbolic link. By default,
       * symbolic links are followed.
       * </p>
       * <p>
       * Note that the result of this method is immediately outdated. If this
       * method indicates the file exists then there is no guarantee that a
       * subsequent access will succeed. Care should be taken when using this
       * method in security sensitive applications.
       * </p>
       *
       * @param followLinks
       *            options indicating how symbolic links are handled
       *
       * @return  @c true if and only if the file or directory denoted
       *          by this abstract pathname exists; @c false if the file does
       *          not exist or its existence cannot be determined.
       */
      gbool exists(gbool followLinks) const;

      /**
       * Tests whether the file denoted by this abstract pathname is a
       * directory.
       *
       * @return @c true if and only if the file denoted by this
       *          abstract pathname exists <em>and</em> is a directory;
       *          @c false otherwise
       */
      gbool isDirectory() const;

      /**
       * Tests whether the file denoted by this abstract pathname is a normal
       * file.  A file is <em>normal</em> if it is not a directory and, in
       * addition, satisfies other system-dependent criteria.
       *
       * @return  @c true if and only if the file denoted by this
       *          abstract pathname exists <em>and</em> is a normal file;
       *          @c false otherwise
       */
      gbool isFile() const;

      /**
       * Tests whether the file denoted by this abstract pathname is a symbolic
       * link.
       *
       * @return  @c true if and only if the file denoted by this
       *          abstract pathname exists <em>and</em> is a normal file;
       *          @c false otherwise
       */
      gbool isSymbolicLink() const;

      /**
       * Tests whether the file named by this abstract pathname is a hidden
       * file.  The exact definition of <em>hidden</em> is system-dependent.  On
       * UNIX systems, a file is considered to be hidden if its name begins with
       * a period character (@c '.').  On Microsoft Windows systems, a file is
       * considered to be hidden if it has been marked as such in the filesystem.
       *
       * @return  @c true if and only if the file denoted by this
       *          abstract pathname is hidden according to the conventions of the
       *          underlying platform
       */
      gbool isHidden() const;

      /**
       * Returns the time that the file denoted by this abstract pathname was
       * last modified.
       *
       * @note
       * While the unit of time of the return value is milliseconds, the
       * granularity of the value depends on the underlying file system and may
       * be larger.  For example, some file systems use time stamps in units of
       * seconds.
       *
       * @return  A @c glong value representing the time the file was
       *          last modified, measured in milliseconds since the epoch
       *          (00:00:00 GMT, January 1, 1970), or @c 0LL if the
       *          file does not exist or if an I/O error occurs.  The value may
       *          be negative indicating the number of milliseconds before the
       *          epoch
       */
      glong lastModifiedTime() const;

      /**
       * Returns the time that the file denoted by this abstract pathname was
       * last accessed.
       *
       * @note
       * While the unit of time of the return value is milliseconds, the
       * granularity of the value depends on the underlying file system and may
       * be larger.  For example, some file systems use time stamps in units of
       * seconds.
       *
       * @return  A @c glong value representing the time the file was
       *          last accessed, measured in milliseconds since the epoch
       *          (00:00:00 GMT, January 1, 1970), or @c 0LL if the
       *          file does not exist or if an I/O error occurs.  The value may
       *          be negative indicating the number of milliseconds before the
       *          epoch
       */
      glong lastAccessTime() const;

      /**
       * Returns the time that the file denoted by this abstract pathname was
       * created.
       *
       * @note
       * While the unit of time of the return value is milliseconds, the
       * granularity of the value depends on the underlying file system and may
       * be larger.  For example, some file systems use time stamps in units of
       * seconds.
       *
       * @return  A @c glong value representing the time the file was
       *          created, measured in milliseconds since the epoch
       *          (00:00:00 GMT, January 1, 1970), or @c 0LL if the
       *          file does not exist or if an I/O error occurs.  The value may
       *          be negative indicating the number of milliseconds before the
       *          epoch
       */
      glong creationTime() const;

      /**
       * Returns the length of the file denoted by this abstract pathname.
       * The return value is unspecified if this pathname denotes a directory.
       *
       * @return  The length, in bytes, of the file denoted by this abstract
       *          pathname, or @c 0LL if the file does not exist.  Some
       *          operating systems may return @c 0LL for pathnames
       *          denoting system-dependent entities such as devices or pipes.
       */
      glong length() const;


      /* -- File operations -- */

      /**
       * Atomically creates a new, empty file named by this abstract pathname if
       * and only if a file with this name does not yet exist.  The check for the
       * existence of the file and the creation of the file if it does not exist
       * are a single operation that is atomic with respect to all other
       * filesystem activities that might affect the file.
       * <P>
       * Note: this method should <i>not</i> be used for file-locking, as
       * the resulting protocol cannot be made to work reliably. The
       * @b FileLock facility should be used instead.
       * </p>
       * @return  @c true if the named file does not exist and was
       *          successfully created; @c false if the named file
       *          already exists
       *
       * @throws  IOException
       *          If an I/O error occurred
       */
      gbool createFile() const;

      /**
       * Creates the directory named by this abstract pathname.
       *
       * @return  @c true if and only if the directory was
       *          created; @c false otherwise
       */
      gbool createDirectory() const;

      /**
       * Creates a symbolic link to this file <i>(optional operation)</i>.
       *
       * <p>
       * This file will be the target of the link. It may be an
       * @b absolute or relative path and may not exist. When
       * the target is a relative path then file system operations on the resulting
       * link are relative to the path of the link.
       * </p>
       *
       * @param   link
       *          the path of the symbolic link to create
       *
       * @return  @c true if operation is success, @c false in otherwise.
       *
       * @throws  UnsupportedOperationException
       *          if the implementation does not support symbolic links
       *
       * @throws  FileAlreadyExistsException
       *          if a file with the name already exists <i>(optional specific
       *          exception)</i>
       *
       * @throws  IOException
       *          if an I/O error occurs
       */
      gbool createSymbolicLink(File const& link) const;

      /**
       * Creates a new link (directory entry) for this file <i>(optional
       * operation)</i>.
       *
       * <p>
       * The @c link parameter locates the directory entry to create. This
       * method creates a new directory entry for the file so that it can be
       * accessed using @c link as the path. On some file systems this is
       * known as creating a "hard link". Whether the file attributes are
       * maintained for the file or for each directory entry is file system
       * specific and therefore not specified. Typically, a file system requires
       * that all links (directory entries) for a file be on the same file system.
       * Furthermore, on some platforms, the Java virtual machine may require to
       * be started with implementation specific privileges to create hard links
       * or to create links to directories.
       * </p>
       * @param   link
       *          the link (directory entry) to create
       *
       * @return  the path to the link (directory entry)
       *
       * @throws  UnsupportedOperationException
       *          if the implementation does not support adding an existing file
       *          to a directory
       * @throws  FileAlreadyExistsException
       *          if the entry could not otherwise be created because a file of
       *          that name already exists <i>(optional specific exception)</i>
       * @throws  IOException
       *          if an I/O error occurs
       */
      gbool createLink(File const& link) const;

      /**
       * Creates a shortcut link to this file <i>(optional operation)</i>.
       *
       * <p>
       * This file will be the target of the link. It may be an
       * @b absolute or relative path and may not exist. When
       * the target is a relative path then file system operations on the resulting
       * link are relative to the path of the link.
       * </p>
       *
       * @param   dir
       *          the path of the directory parent of link to create
       *
       * @return  @c true if operation is success, @c false in otherwise.
       *
       * @throws  UnsupportedOperationException
       *          if the implementation does not support shortcut links
       *
       * @throws  FileAlreadyExistsException
       *          if a file with the name already exists <i>(optional specific
       *          exception)</i>
       *
       * @throws  IOException
       *          if an I/O error occurs
       */
      File createShortcut(File const& dir) const;

      /**
       * Reads the target of this symbolic/shortcut link <i>(optional operation)</i>.
       *
       * <p> If the file system supports <i> symbolic or shortcut links</i>
       * then this method is used to read the target of the link, failing
       * if the file is not a symbolic or shortcut link. The target of the link need not exist.
       * The returned @c File object will be associated with the same file
       * system as @c link.
       *
       * @return  a @c Path object representing the target of the link
       *
       * @throws  UnsupportedOperationException
       *          if the implementation does not support symbolic links
       * @throws  NotLinkException
       *          if the target could otherwise not be read because the file
       *          is not a symbolic link <i>(optional specific exception)</i>
       * @throws  IOException
       *          if an I/O error occurs
       */
      File target() const;

      /**
       * Creates the directory named by this abstract pathname, including any
       * necessary but nonexistent parent directories.  Note that if this
       * operation fails it may have succeeded in creating some of the necessary
       * parent directories.
       *
       * @return  @c true if and only if the directory was created,
       *          along with all necessary parent directories; @c false
       *          otherwise
       */
      gbool createDirectories() const;

      /**
       * Deletes the file or directory denoted by this abstract pathname.  If
       * this pathname denotes a directory, then the directory must be empty in
       * order to be deleted.
       *
       * @return  @c true if and only if the file or directory is
       *          successfully deleted; @c false otherwise
       */
      gbool deleteFile() const;

      /**
       * Move the file or directory denoted by this abstract pathname to trash.
       *
       * @return  @c true if and only if the file or directory is
       *          successfully moved to trash; @c false otherwise
       */
      gbool recycleFile() const;

      /**
       * Requests that the file or directory denoted by this abstract
       * pathname be deleted when the virtual machine terminates.
       * Files (or directories) are deleted in the reverse order that
       * they are registered. Invoking this method to delete a file or
       * directory that is already registered for deletion has no effect.
       * Deletion will be attempted only for normal termination of the
       * virtual machine, as defined by the Java Language Specification.
       *
       * <p> Once deletion has been requested, it is not possible to cancel the
       * request.  This method should therefore be used with care.
       * </p>
       * <p>
       * Note: this method should <i>not</i> be used for file-locking, as
       * the resulting protocol cannot be made to work reliably. The
       * @b FileLock facility should be used instead.
       * </p>
       *
       * @see delete
       */
      void deleteOnExit() const;

      /**
       * Returns an array of strings naming the files and directories in the
       * directory denoted by this abstract pathname.
       *
       * <p>
       * If this abstract pathname does not denote a directory, then this
       * method returns empty array.  Otherwise, an array of strings is
       * returned, one for each file or directory in the directory.  Names
       * denoting the directory itself and the directory's parent directory are
       * not included in the result.  Each string is a file name rather than a
       * complete path.
       * </p>
       * <p>
       * There is no guarantee that the name strings in the resulting array
       * will appear in any specific order; they are not, in particular,
       * guaranteed to appear in alphabetical order.
       * </p>
       *
       * @return  An array of strings naming the files and directories in the
       *          directory denoted by this abstract pathname.  The array will be
       *          empty if the directory is empty.  Returns empty array if
       *          this abstract pathname does not denote a directory, or if an
       *          I/O error occurs.
       */
      StringArray childPaths() const;

      /**
       * Returns an array of strings naming the files and directories in the
       * directory denoted by this abstract pathname that satisfy the specified
       * filter.  The behavior of this method is the same as that of the
       * @b childPaths() method, except that the strings in the returned array
       * must satisfy the filter.  A name satisfies the filter if
       * and only if the value @c true results when the <b>FilenameFilter::accept(File, String)</b> method
       * of the filter is invoked on this abstract pathname and the name of a
       * file or directory in the directory that it denotes.
       *
       * @param  filter
       *         A filename filter
       *
       * @return  An array of strings naming the files and directories in the
       *          directory denoted by this abstract pathname that were accepted
       *          by the given @c filter.  The array will be empty if the
       *          directory is empty or if no names were accepted by the filter.
       *          Returns empty array if this abstract pathname does not denote
       *          a directory, or if an I/O error occurs.
       */
      StringArray childPaths(FilenameFilter const& filter) const;

      /**
       * Returns an array of abstract pathnames denoting the files in the
       * directory denoted by this abstract pathname.
       *
       * <p>
       * If this abstract pathname does not denote a directory, then this
       * method returns empty array.  Otherwise, an array of @c File objects
       * is returned, one for each file or directory in the directory.  Pathnames
       * denoting the directory itself and the directory's parent directory are
       * not included in the result.  Each resulting abstract pathname is
       * constructed from this abstract pathname using the @b File(File, String)
       * constructor.  Therefore, if this pathname is absolute then each resulting
       * pathname is absolute; if this pathname is relative then each resulting
       * pathname will be relative to the same directory.
       * </p>
       * <p>
       * There is no guarantee that the name strings in the resulting array
       * will appear in any specific order; they are not, in particular,
       * guaranteed to appear in alphabetical order.
       * </p>
       *
       * @return  An array of abstract pathnames denoting the files and
       *          directories in the directory denoted by this abstract pathname.
       *          The array will be empty if the directory is empty.  Returns
       *          empty array if this abstract pathname does not denote a
       *          directory, or if an I/O error occurs.
       */
      FileArray childFiles() const;

      /**
       * Returns an array of abstract pathnames denoting the files and
       * directories in the directory denoted by this abstract pathname that
       * satisfy the specified filter.  The behavior of this method is the same
       * as that of the @b childFiles() method, except that the pathnames in
       * the returned array must satisfy the filter.  A pathname
       * satisfies the filter if and only if the value @c true results when
       * the @b FilenameFilter::accept(File, String) method of the filter is
       * invoked on this abstract pathname and the name of a file or directory in
       * the directory that it denotes.
       *
       * @param  filter
       *         A filename filter
       *
       * @return  An array of abstract pathnames denoting the files and
       *          directories in the directory denoted by this abstract pathname.
       *          The array will be empty if the directory is empty.  Returns
       *          empty array if this abstract pathname does not denote a
       *          directory, or if an I/O error occurs.
       */
      FileArray childFiles(FilenameFilter const& filter) const;

      /**
       * Returns an array of abstract pathnames denoting the files and
       * directories in the directory denoted by this abstract pathname that
       * satisfy the specified filter.  The behavior of this method is the same
       * as that of the @b childFiles() method, except that the pathnames in
       * the returned array must satisfy the filter.  A pathname
       * satisfies the filter if and only if the value @c true results when
       * the @b FileFilter::accept(File) method of the
       * filter is invoked on the pathname.
       *
       * @param  filter
       *         A file filter
       *
       * @return  An array of abstract pathnames denoting the files and
       *          directories in the directory denoted by this abstract pathname.
       *          The array will be empty if the directory is empty.  Returns
       *          empty array if this abstract pathname does not denote a
       *          directory, or if an I/O error occurs.
       */
      FileArray childFiles(FileFilter const& filter) const;

      /**
       * Renames the file denoted by this abstract pathname.
       *
       * <p>
       * Many aspects of the behavior of this method are inherently
       * platform-dependent: The rename operation might not be able to move a
       * file from one filesystem to another, it might not be atomic, and it
       * might not succeed if a file with the destination abstract pathname
       * already exists.  The return value should always be checked to make sure
       * that the rename operation was successful.  As instances of @c File
       * are immutable, this File object is not changed to name the destination
       * file or directory.
       * </p>
       *
       * @param  dest  The new abstract pathname for the named file
       *
       * @return  @c true if and only if the renaming succeeded;
       *          @c false otherwise
       */
      gbool renameTo(File const& dest) const;

      /**
       * Copy a file to a target file.
       *
       * <p>
       * This method copies a file to the target file with the
       * @c options parameter specifying how the copy is performed. By default, the
       * copy fails if the target file already exists or is a symbolic link,
       * except if the source and target are the @b same file, in
       * which case the method completes without copying the file. File attributes
       * are not required to be copied to the target file. If symbolic links are
       * supported, and the file is a symbolic link, then the final target of the
       * link is copied. If the file is a directory then an empty directory is
       * created in the target location (entries in the directory are not
       * copied).
       * </p>
       * <p>
       * The @c options parameter may include any of the following:
       *
       * @li  REPLACE_EXISTING : <br>
       *     Replace an existing file. A non-empty directory cannot be
       *     replaced. If the target file exists and is a symbolic link, then the
       *     symbolic link itself, not the target of the link, is replaced.
       * @li COPY_ATTRIBUTES : <br>
       *     Attempts to copy the file attributes associated with this file to
       *     the target file. The exact file attributes that are copied is platform
       *     and file system dependent and therefore unspecified. Minimally, the
       *     @b last-modified-time is copied to the target file if supported by
       *     both the source and target file stores. Copying of file timestamps
       *     may result in precision loss.
       * @li  NOFOLLOW_LINKS : <br>
       *     Symbolic links are not followed. If the file is a symbolic link,
       *     then the symbolic link itself, not the target of the link, is copied.
       *     It is implementation specific if file attributes can be copied to the
       *     new link. In other words, the @c COPY_ATTRIBUTES option may be
       *     ignored when copying a symbolic link.
       * </p>
       * <p>
       * An implementation of this interface may support additional
       * implementation specific options.
       * </p>
       *
       * <p>
       * Copying a file is not an atomic operation. If an @b IOException
       * is thrown, then it is possible that the target file is incomplete or some
       * of its file attributes have not been copied from the source file. When
       * the @c REPLACE_EXISTING option is specified and the target file
       * exists, then the target file is replaced. The check for the existence of
       * the file and the creation of the new file may not be atomic with respect
       * to other file system activities.
       * </p>
       *
       * @param   target
       *          the path to the target file (maybe associated with a different
       *          provider to the source path)
       * @param   options
       *          options specifying how the copy should be done
       *
       * @return  the path to the target file
       *
       * @throws  UnsupportedOperationException
       *          if the options contains a copy option that is not supported
       *
       * @throws  IOException
       *          if an I/O error occurs
       */
      gbool copyTo(File const& target, CopyOption options) const;

      /**
       * Move or rename a file to a target file.
       *
       * <p>
       * By default, this method attempts to move the file to the target
       * file, failing if the target file exists except if the source and
       * target are the @b same file, in which case this method
       * has no effect. If the file is a symbolic link then the symbolic link
       * itself, not the target of the link, is moved. This method may be
       * invoked to move an empty directory. In some implementations a directory
       * has entries for special files or links that are created when the
       * directory is created. In such implementations a directory is considered
       * empty when only the special entries exist. When invoked to move a
       * directory that is not empty then the directory is moved if it does not
       * require moving the entries in the directory. When moving a directory requires that its
       * entries be moved then this method fails (by throwing an
       * @c IOException).
       * </p>
       * <p>
       * The @c options parameter may include any of the following:
       *
       * @li @b REPLACE_EXISTING : <br>
       *   Replace an existing file. A non-empty directory cannot be
       *     replaced. If the target file exists and is a symbolic link, then the
       *     symbolic link itself, not the target of the link, is replaced.
       * @li @b ATOMIC_MOVE : <br>
       *     The move is performed as an atomic file system operation and all
       *     other options are ignored. If the target file exists then it is
       *     implementation specific if the existing file is replaced or this method
       *     fails by throwing an @b IOException.
       * </p>
       * <p>
       * An implementation of this interface may support additional
       * implementation specific options.
       * </p>
       * @param   target
       *          the path to the target file (maybe associated with a different
       *          provider to the source path)
       * @param   options
       *          options specifying how the move should be done
       *
       * @return  @c true if and only if the renaming succeeded;
       *          @c false otherwise
       *
       * @throws  UnsupportedOperationException
       *          if the options contains a copy option that is not supported
       * @throws  IOException
       *          if an I/O error occurs
       */
      gbool moveTo(File const& target, CopyOption options) const;

      /**
       * Sets the last-modified time of the file or directory named by this
       * abstract pathname.
       *
       * <p>
       * All platforms support file-modification times to the nearest second,
       * but some provide more precision.  The argument will be truncated to fit
       * the supported precision.  If the operation succeeds and no intervening
       * operations on the file take place, then the next invocation of the
       * @b lastModified method will return the (possibly
       * truncated) @c time argument that was passed to this method.
       * </p>
       * @param  time  The new last-modified time, measured in milliseconds since
       *               the epoch (00:00:00 GMT, January 1, 1970)
       *
       * @return @c true if and only if the operation succeeded;
       *          @c false otherwise
       *
       * @throws  IllegalArgumentException  If the argument is negative
       */
      gbool setLastModifiedTime(glong time) const;

      /**
       * Sets the last-access time of the file or directory named by this
       * abstract pathname.
       *
       * <p>
       * All platforms support file-modification times to the nearest second,
       * but some provide more precision.  The argument will be truncated to fit
       * the supported precision.  If the operation succeeds and no intervening
       * operations on the file take place, then the next invocation of the
       * @b lastModified method will return the (possibly
       * truncated) @c time argument that was passed to this method.
       * </p>
       * @param  time  The new last-access time, measured in milliseconds since
       *               the epoch (00:00:00 GMT, January 1, 1970)
       *
       * @return @c true if and only if the operation succeeded;
       *          @c false otherwise
       *
       * @throws  IllegalArgumentException  If the argument is negative
       */
      gbool setLastAccessTime(glong time) const;

      /**
       * Sets the creation time of the file or directory named by this
       * abstract pathname.
       *
       * <p>
       * All platforms support file-modification times to the nearest second,
       * but some provide more precision.  The argument will be truncated to fit
       * the supported precision.  If the operation succeeds and no intervening
       * operations on the file take place, then the next invocation of the
       * @b lastModified method will return the (possibly
       * truncated) @c time argument that was passed to this method.
       * </p>
       * @param  time  The new creation time, measured in milliseconds since
       *               the epoch (00:00:00 GMT, January 1, 1970)
       *
       * @return @c true if and only if the operation succeeded;
       *          @c false otherwise
       *
       * @throws  IllegalArgumentException  If the argument is negative
       */
      gbool setCreationTime(glong time) const;

      /**
       * Marks the file or directory named by this abstract pathname so that
       * only read operations are allowed. After invoking this method the file
       * or directory will not change until it is either deleted or marked
       * to allow write access. On some platforms it may be possible to start the
       * Program with special privileges that allow it to modify
       * files that are marked read-only. Whether a read-only file or
       * directory may be deleted depends upon the underlying system.
       *
       * @return @c true if and only if the operation succeeded;
       *          @c false otherwise
       */
      gbool setReadOnly() const;

      /**
       * Sets the owner's or everybody's write permission for this abstract
       * pathname. On some platforms it may be possible to start the Java virtual
       * machine with special privileges that allow it to modify files that
       * disallow write operations.
       *
       * @param   writable
       *          If @c true, sets the access permission to allow write
       *          operations; if @c false to disallow write operations
       *
       * @param   ownerOnly
       *          If @c true, the write permission applies only to the
       *          owner's write permission; otherwise, it applies to everybody.  If
       *          the underlying file system can not distinguish the owner's write
       *          permission from that of others, then the permission will apply to
       *          everybody, regardless of this value.
       *
       * @return  @c true if and only if the operation succeeded. The
       *          operation will fail if the user does not have permission to change
       *          the access permissions of this abstract pathname.
       */
      gbool setWritable(gbool writable, gbool ownerOnly) const;

      /**
       * A convenience method to set the owner's write permission for this abstract
       * pathname. On some platforms it may be possible to start the Java virtual
       * machine with special privileges that allow it to modify files that
       * disallow write operations.
       *
       * <p>
       * An invocation of this method of the form @c file.setWritable(arg)
       * behaves in exactly the same way as the invocation
       * <code>
       *     file.setWritable(arg, true)
       * </code>
       * </p>
       *
       * @param   writable
       *          If @c true, sets the access permission to allow write
       *          operations; if @c false to disallow write operations
       *
       * @return  @c true if and only if the operation succeeded.  The
       *          operation will fail if the user does not have permission to
       *          change the access permissions of this abstract pathname.
       */
      gbool setWritable(gbool writable) const;

      /**
       * Sets the owner's or everybody's read permission for this abstract
       * pathname. On some platforms it may be possible to start the Java virtual
       * machine with special privileges that allow it to read files that are
       * marked as unreadable.
       *
       * <p>
       * If the platform supports setting a file's read permission, but
       * the user does not have permission to change the access permissions of
       * this abstract pathname, then the operation will fail. If the platform
       * does not support setting a file's read permission, this method does
       * nothing and returns the value of the @c readable parameter.
       * </p>
       * @param   readable
       *          If @c true, sets the access permission to allow read
       *          operations; if @c false to disallow read operations
       *
       * @param   ownerOnly
       *          If @c true, the read permission applies only to the
       *          owner's read permission; otherwise, it applies to everybody.  If
       *          the underlying file system can not distinguish the owner's read
       *          permission from that of others, then the permission will apply to
       *          everybody, regardless of this value.
       *
       * @return  @c true if the operation succeeds, @c false if it
       *          fails, or the value of the @c readable parameter if
       *          setting the read permission is not supported.
       */
      gbool setReadable(gbool readable, gbool ownerOnly) const;

      /**
       * A convenience method to set the owner's read permission for this abstract
       * pathname. On some platforms it may be possible to start the Java virtual
       * machine with special privileges that allow it to read files that are
       * marked as unreadable.
       *
       * <p>
       * An invocation of this method of the form @c file.setReadable(arg)
       * behaves in exactly the same way as the invocation
       *
       * <code>
       *     file.setReadable(arg, true)
       * </code>
       * </p>
       * <p>
       * If the platform supports setting a file's read permission, but
       * the user does not have permission to change the access permissions of
       * this abstract pathname, then the operation will fail. If the platform
       * does not support setting a file's read permission, this method does
       * nothing and returns the value of the @c readable parameter.
       * </p>
       *
       * @param  readable
       *          If @c true, sets the access permission to allow read
       *          operations; if @c false to disallow read operations
       *
       * @return  @c true if the operation succeeds, @c false if it
       *          fails, or the value of the @c readable parameter if
       *          setting the read permission is not supported.
       */
      gbool setReadable(gbool readable) const;

      /**
       * Sets the owner's or everybody's execute permission for this abstract
       * pathname. On some platforms it may be possible to start the Java virtual
       * machine with special privileges that allow it to execute files that are
       * not marked executable.
       *
       * <p>
       * If the platform supports setting a file's execute permission, but
       * the user does not have permission to change the access permissions of
       * this abstract pathname, then the operation will fail. If the platform
       * does not support setting a file's execute permission, this method does
       * nothing and returns the value of the @c executable parameter.
       * </p>
       *
       * @param   executable
       *          If @c true, sets the access permission to allow execute
       *          operations; if @c false to disallow execute operations
       *
       * @param   ownerOnly
       *          If @c true, the execute permission applies only to the
       *          owner's execute permission; otherwise, it applies to everybody.
       *          If the underlying file system can not distinguish the owner's
       *          execute permission from that of others, then the permission will
       *          apply to everybody, regardless of this value.
       *
       * @return  @c true if the operation succeeds, @c false if it
       *          fails, or the value of the @c executable parameter if
       *          setting the execute permission is not supported.
       */
      gbool setExecutable(gbool executable, gbool ownerOnly) const;

      /**
       * A convenience method to set the owner's execute permission for this
       * abstract pathname. On some platforms it may be possible to start the Java
       * virtual machine with special privileges that allow it to execute files
       * that are not marked executable.
       *
       * <p>
       * An invocation of this method of the form @c file.setExecutable(arg)
       * behaves in exactly the same way as the invocation
       *
       * <code>
       *     file.setExecutable(arg, true)
       * </code>
       * </p>
       *
       * <p>
       * If the platform supports setting a file's execute permission, but
       * the user does not have permission to change the access permissions of
       * this abstract pathname, then the operation will fail. If the platform
       * does not support setting a file's execute permission, this method does
       * nothing and returns the value of the @c executable parameter.
       * </p>
       * @param   executable
       *          If @c true, sets the access permission to allow execute
       *          operations; if @c false to disallow execute operations
       *
       * @return  @c true if the operation succeeds, @c false if it
       *          fails, or the value of the @c executable parameter if
       *          setting the execute permission is not supported.
       */
      gbool setExecutable(gbool executable) const;

      /**
       * Returns a file's POSIX file permissions.
       * <p>
       * The @c followLinks array may be used to indicate how symbolic links
       * are handled for the case that the file is a symbolic link. By default,
       * symbolic links are followed and the file attribute of the final target
       * of the link is read.
       * </p>
       * @param   followLinks
       *          options indicating how symbolic links are handled
       *          (@true for follow link, and @c false for otherwise).
       *
       * @return  the file permissions
       *
       * @throws  UnsupportedOperationException
       *          if the associated file system does not support the Posix
       *          file attributes.
       *
       * @throws  IOException
       *          if an I/O error occurs
       */
      Permission permissions(gbool followLinks) const;

      /**
       * Sets a file's POSIX permissions.
       *
       * <p>
       * This attribute view
       * provides access to file attributes commonly associated with files on file
       * systems used by operating systems that implement the Portable Operating
       * System Interface (POSIX) family of standards.
       * </p>
       *
       * @param   perms
       *          The new set of permissions
       *
       * @return  The given path
       *
       * @throws  UnsupportedOperationException
       *          if the associated file system does not support the
       *          Posix file attribute.
       *
       * @throws  IOException
       *          if an I/O error occurs
       */
      gbool setPermissions(Permission perms) const;


      /* -- Filesystem interface -- */

      /**
       * List the available filesystem roots.
       *
       * <p>
       * This method returns an array of @c File objects that denote the
       * root directories of the available filesystem roots.  It is guaranteed
       * that the canonical pathname of any file physically present on the local
       * machine will begin with one of the roots returned by this method.
       * There is no guarantee that a root directory can be accessed.
       * </p>
       *
       * @note
       * Windows platforms, for example, have a root directory
       * for each active drive; UNIX platforms have a single root directory,
       * namely @c "/".  The set of filesystem roots is affected
       * by various system-level operations such as the disconnecting or
       * unmounting of physical or virtual disk drives.
       *
       * <p>
       * The canonical pathname of a file that resides on some other machine
       * and is accessed via a remote-filesystem protocol such as SMB or NFS may
       * or may not begin with one of the roots returned by this method.  If the
       * pathname of a remote file is syntactically indistinguishable from the
       * pathname of a local file then it will begin with one of the roots
       * returned by this method.  Thus, for example, @c File objects
       * denoting the root directories of the mapped network drives of a Windows
       * platform will be returned by this method, while @c File objects
       * containing UNC pathnames will not be returned by this method.
       * </p>
       *
       * @return  An array of @c File objects denoting the available
       *          filesystem roots, or empty array if the set of roots could not
       *          be determined.  The array will be empty if there are no
       *          filesystem roots.
       */
      static FileArray roots();


      /* -- Disk usage -- */

      /**
       * Returns the size of the partition <a href="#partName">named</a> by this
       * abstract pathname. If the total number of bytes in the partition is
       * greater than @b Long::MAX_VALUE, then @c Long::MAX_VALUE will be
       * returned.
       *
       * @return  The size, in bytes, of the partition or @c 0LL if this
       *          abstract pathname does not name a partition or if the size
       *          cannot be obtained
       */
      glong totalSpace() const;

      /**
       * Returns the number of unallocated bytes in the partition
       * <a href="#partName">named</a> by this abstract path name.  If the
       * number of unallocated bytes in the partition is greater than
       * @b Long::MAX_VALUE, then @c Long::MAX_VALUE will be returned.
       *
       * <p>
       * The returned number of unallocated bytes is a hint, but not
       * a guarantee, that it is possible to use most or any of these
       * bytes.  The number of unallocated bytes is most likely to be
       * accurate immediately after this call.  It is likely to be made
       * inaccurate by any external I/O operations including those made
       * on the system outside of this virtual machine.  This method
       * makes no guarantee that write operations to this file system
       * will succeed.
       * </p>
       *
       * @return  The number of unallocated bytes on the partition or @c 0LL
       *          if the abstract pathname does not name a partition or if this
       *          number cannot be obtained.  This value will be less than or
       *          equal to the total file system size returned by
       *          @b totalSpace.
       */
      glong freeSpace() const;

      /**
       * Returns the number of bytes available to this virtual machine on the
       * partition <a href="#partName">named</a> by this abstract pathname.  If
       * the number of available bytes in the partition is greater than
       * @b Long::MAX_VALUE, then @c Long::MAX_VALUE will be returned.
       * When possible, this method checks for write permissions and other
       * operating system restrictions and will therefore usually provide a more
       * accurate estimate of how much new data can actually be written than
       * @b freeSpace.
       *
       * <p>
       * The returned number of available bytes is a hint, but not a
       * guarantee, that it is possible to use most or any of these bytes.  The
       * number of available bytes is most likely to be accurate immediately
       * after this call.  It is likely to be made inaccurate by any external
       * I/O operations including those made on the system outside of this
       * virtual machine.  This method makes no guarantee that write operations
       * to this file system will succeed.
       * </p>
       *
       * @return  The number of available bytes on the partition or @c 0LL
       *          if the abstract pathname does not name a partition or if this
       *          number cannot be obtained.  On systems where this information
       *          is not available, this method will be equivalent to a call to
       *          @b freeSpace.
       */
      glong usableSpace() const;

      /**
       * <p>
       * Creates a new empty file in the specified directory, using the
       * given prefix and suffix strings to generate its name.  If this method
       * returns successfully then it is guaranteed that:
       * </p>
       *
       * @li The file denoted by the returned abstract pathname did not exist
       *      before this method was invoked, and
       * @li Neither this method nor any of its variants will return the same
       *      abstract pathname again in the current invocation of the virtual
       *      machine.
       *
       * This method provides only part of a temporary-file facility.  To arrange
       * for a file created by this method to be deleted automatically, use the
       * @b deleteOnExit method.
       *
       * <p>
       * The @c prefix argument must be at least three characters
       * long.  It is recommended that the prefix be a short, meaningful string
       * such as @c "hjb" or @c "mail".
       * </p>
       * <p>
       * To create the new file, the prefix and the suffix may first be
       * adjusted to fit the limitations of the underlying platform.  If the
       * prefix is too long then it will be truncated, but its first three
       * characters will always be preserved.  If the suffix is too long then it
       * too will be truncated, but if it begins with a period character
       * (@c '.') then the period and the first three characters
       * following it will always be preserved.  Once these adjustments have been
       * made the name of the new file will be generated by concatenating the
       * prefix, five or more internally-generated characters, and the suffix.
       * </p>
       * <p>
       * If the @c directory argument is valid and denotes an existing, writable
       * directory, then the file will be created in that directory. Otherwise, the file will
       * not be created and an @c IOException will be thrown.  Under no
       * circumstances will a directory be created at the location specified by
       * the @c directory argument.
       * </p>
       * @param  prefix     The prefix string to be used in generating the file's
       *                    name; must be at least three characters long
       *
       * @param  suffix     The suffix string to be used in generating the file's
       *                    name;
       *
       * @param  directory  The directory in which the file is to be created.
       *
       * @return  An abstract pathname denoting a newly-created empty file
       *
       * @throws  IllegalArgumentException
       *          If the @c prefix argument contains fewer than three
       *          characters
       *
       * @throws  IOException
       *          If a file could not be created
       */
      static File createTempFile(String const& prefix, String const& suffix, File const& directory);

      /**
       * Creates an empty file in the default temporary-file directory, using
       * the given prefix and suffix to generate its name.
       *
       * <p>
       * The default temporary-file directory is specified by the system property
       * @c "system.tmpdir".  On UNIX systems the default value of this
       * property is typically @c "/tmp" or @c "/var/tmp"; on
       * Microsoft Windows systems it is typically @c "C:\\WINNT\\TEMP".  A different
       * value may be given to this system property when the Program
       * is invoked, but programmatic changes to this property are not guaranteed
       * to have any effect upon the temporary directory used by this method.
       * </p>
       *
       * @param  prefix     The prefix string to be used in generating the file's
       *                    name; must be at least three characters long
       *
       * @param  suffix     The suffix string to be used in generating the file's
       *                    name
       *
       * @return  An abstract pathname denoting a newly-created empty file
       *
       * @throws  IllegalArgumentException
       *          If the @c prefix argument contains fewer than three
       *          characters
       *
       * @throws  IOException  If a file could not be created
       */
      static File createTempFile(String const& prefix, String const& suffix);

      /**
       * Creates an empty file in the default temporary-file directory, using
       * the given prefix and @c ".tmp" as suffix to generate its name.
       *
       * <p>
       * The default temporary-file directory is specified by the system property
       * @c "system.tmpdir".  On UNIX systems the default value of this
       * property is typically @c "/tmp" or @c "/var/tmp"; on
       * Microsoft Windows systems it is typically @c "C:\\WINNT\\TEMP".  A different
       * value may be given to this system property when the Program
       * is invoked, but programmatic changes to this property are not guaranteed
       * to have any effect upon the temporary directory used by this method.
       * </p>
       *
       * @param  prefix     The prefix string to be used in generating the file's
       *                    name; must be at least three characters long
       *
       * @return  An abstract pathname denoting a newly-created empty file
       *
       * @throws  IllegalArgumentException
       *          If the @c prefix argument contains fewer than three
       *          characters
       *
       * @throws  IOException  If a file could not be created
       */
      static File createTempFile(String const& prefix);

      /* -- Basic infrastructure -- */

      /**
       * Compares two abstract pathnames lexicographically.  The ordering
       * defined by this method depends upon the underlying system.  On UNIX
       * systems, alphabetic case is significant in comparing pathnames; on
       * Microsoft Windows systems it is not.
       *
       * @param   pathname  The abstract pathname to be compared to this abstract
       *                    pathname
       *
       * @return  Zero if the argument is equal to this abstract pathname, a
       *          value less than zero if this abstract pathname is
       *          lexicographically less than the argument, or a value greater
       *          than zero if this abstract pathname is lexicographically
       *          greater than the argument
       */
      gint compareTo(const File& pathname) const override;

      /**
       * Tests this abstract pathname for equality with the given object.
       * Returns @c true if and only if the argument is an abstract pathname that is the same as this
       * abstract pathname.  Whether two abstract
       * pathnames are equal depends upon the underlying operating system.
       * On UNIX systems, alphabetic case is significant in comparing pathnames;
       * on Microsoft Windows systems it is not.
       *
       * @note This method only tests whether the abstract pathnames are equal;
       *          it does not access the file system and the file is not required
       *          to exist.
       *
       * @param   obj   The object to be compared with this abstract pathname
       *
       * @return  @c true if and only if the objects are the same;
       *          @c false otherwise
       *
       * @see compareTo(File)
       */
      gbool equals(const Object& obj) const override;

      /**
       * Tests if this and given paths locate the same file.
       *
       * <p>
       * If both @c File objects are @b equal
       * then this method returns @c true without checking if the file exists.
       * If the two @c File objects are associated with different providers
       * then this method returns @c false. Otherwise, this method checks if
       * both @c File objects locate the same file, and depending on the
       * implementation, may require to open or access both files.
       * </p>
       * <p>
       * If the file system and files remain static, then this method implements
       * an equivalence relation for non-null @c Files.
       * @li It is <i>reflexive</i>: for @c File @c f,
       *     @c f.isSameTo(f) should return @c true.
       * @li It is <i>symmetric</i>: for two @c Files @c f and @c g,
       *     @c f.isSameFile(g) will equal @c g.isSameTo(f).
       * @li It is <i>transitive</i>: for three @c Files
       *     @c f, @c g, and @c h, if @c f.isSameFile(g) returns
       *     @c true and @c g.isSameTo(h) returns @c true, then
       *     @c f.isSameTo(h) will return @c true.
       * </p>
       *
       * @param   other
       *          the other path
       *
       * @return  @c true if, and only if, the two paths locate the same file
       *
       * @throws  IOException
       *          if an I/O error occurs
       */
      gbool isSameTo(File const& other) const;

      /**
       * Computes a hash code for this abstract pathname.  Because equality of
       * abstract pathnames is inherently system-dependent, so is the computation
       * of their hash codes.  On UNIX systems, the hash code of an abstract
       * pathname is equal to the exclusive <em>or</em> of the hash code
       * of its pathname string and the decimal value
       * @c 1234321.  On Microsoft Windows systems, the hash
       * code is equal to the exclusive <em>or</em> of the hash code of
       * its pathname string converted to lower case and the decimal
       * value @c 1234321.  Locale is not taken into account on
       * lowercasing the pathname string.
       *
       * @return  A hash code for this abstract pathname
       */
      gint hash() const override;

      /**
       * Returns the pathname string of this abstract pathname.  This is just the
       * string returned by the @b path method.
       *
       * @return  The string form of this abstract pathname
       */
      String toString() const override;

      /**
       * The shadow copy of this instance.
       *
       * @return The shadow copy of this instance.
       */
      Object& clone() const override;

    private:
      /**
       * Check if the file has an invalid path. Currently, the inspection of
       * a file path is very limited, and it only covers Nul character check
       * unless further checking is explicitly enabled by a system property.
       * Returning true means the path is definitely invalid/garbage, but
       * returning false does not guarantee that the path is valid.
       *
       * @return true if the file path is invalid.
       */
      gbool isInvalid() const;

      /**
       * Internal constructor for already-normalized pathname strings.
       */
      CORE_EXPLICIT File(String pathname, gint prefixLength);

      /**
       * Internal constructor for already-normalized pathname strings.
       * The parameter order is used to disambiguate this method from the
       * public(File, String) constructor.
       */
      CORE_EXPLICIT File(const String& child, File const& parent);

      /**
       * The internal constructor used by @c FileArray class.
       */
      CORE_IMPLICIT File() = default;

      static String slashify(String const& path, gbool isDir);

      static File generateFile(const String& prefix, String const& suffix, File const& dir);

      static gint shortenSubName(gint subNameLength, gint excess, gint nameMin);

      CORE_ADD_AS_FRIEND(FileArray);
    };


    /**
     * Combination of two copy options in one only
     *
     * @param lhs the first option
     * @param rhs the second option
     * @return the option representing the combination of given options
     */
    extern File::CopyOption operator|(File::CopyOption lhs, File::CopyOption rhs);
  } // io
} // core

#endif //CORE24_FILE_H
