//
// Created by bruns on 12/10/2024.
//

#ifndef CORE24_FILEDESCRIPTOR_H
#define CORE24_FILEDESCRIPTOR_H

#include <core/io/IOException.h>
#include <core/io/Closeable.h>

namespace core {
    namespace io {
        /**
         * Instances of the file descriptor class serve as an opaque handle
         * to the underlying machine-specific structure representing an open
         * file, an open socket, or another source or sink of bytes.
         * The main practical use for a file descriptor is to create a
         * @b FileInputStream or @b FileOutputStream to contain it.
         * <p>
         * Applications should not create their own file descriptors.
         * </p>
         */
        class FileDescriptor final : public Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ALIAS(PARENT, Class<Closeable>::Pointer);
            CORE_ALIAS(PARENTS, Class<util::List<Closeable>>::Pointer);
            CORE_ALIAS(MONITOR, Class<Object>::Pointer);

            gint fd;

            glong handle;

            PARENT firstParent;
            PARENTS otherParents;
            gbool closed;

            /**
             * true, if file is opened for appending.
             */
            gbool append;

            /**
             * Used for standard input, output, and error only.
             * For Windows the corresponding handle is initialized.
             * For Unix the append mode is cached.
             * @param fd the raw fd number (0, 1, 2)
             */
            CORE_EXPLICIT FileDescriptor(gint fd);

        public:
            /**
             * Constructs an (invalid) FileDescriptor object.
             * The fd or handle is set later.
             */
            CORE_IMPLICIT FileDescriptor();

            FileDescriptor(FileDescriptor const &) = delete;

            FileDescriptor(FileDescriptor &&fd) CORE_NOTHROW;

            FileDescriptor &operator=(FileDescriptor const &) = delete;

            FileDescriptor &operator=(FileDescriptor const &&fd) CORE_NOTHROW = delete;

            /**
             * A handle to the standard input stream. Usually, this file
             * descriptor is not used directly, but rather via the input stream
             * known as @c Console::in.
             */
            static const FileDescriptor in;

            /**
             * A handle to the standard output stream. Usually, this file
             * descriptor is not used directly, but rather via the output stream
             * known as @c Console::out.
             */
            static const FileDescriptor out;

            /**
             * A handle to the standard error stream. Usually, this file
             * descriptor is not used directly, but rather via the output stream
             * known as @c Console::err.
             */
            static const FileDescriptor err;

            /**
             * Tests if this file descriptor object is valid.
             *
             * @return  @c true if the file descriptor object represents a
             *          valid, open file, socket, or other active I/O connection;
             *          @c false otherwise.
             */
            gbool isValid() const;

            /**
             * Force all system buffers to synchronize with the underlying
             * device.  This method returns after all modified data and
             * attributes of this FileDescriptor have been written to the
             * relevant device(s).  In particular, if this FileDescriptor
             * refers to a physical storage medium, such as a file in a file
             * system, sync will not return until all in-memory modified copies
             * of buffers associated with this FileDescriptor have been
             * written to the physical medium.
             *
             * sync is meant to be used by code that requires physical
             * storage (such as a file) to be in a known state  For
             * example, a class that provided a simple transaction facility
             * might use sync to ensure that all changes to a file caused
             * by a given transaction were recorded on a storage medium.
             *
             * sync only affects buffers downstream of this FileDescriptor.  If
             * any in-memory buffering is being done by the application (for
             * example, by a BufferedOutputStream object), those buffers must
             * be flushed into the FileDescriptor (for example, by invoking
             * OutputStream::flush) before that data will be affected by sync.
             *
             * @throws  SyncFailedException
             *        Thrown when the buffers cannot be flushed,
             *        or because the system cannot guarantee that all the
             *        buffers have been synchronized with physical media.
             */
            void sync() const;

        private:
            /* fsync/equivalent this file descriptor */
            void synchronize() const;

            /*
             * On Windows return the handle for the standard streams.
             */
            static glong getHandle(gint fd);

            /**
             * Returns true, if the file was opened for appending.
             */
            static glong getAppendStatus(gint fd);

            MONITOR monitor = {};
        };
    } // io
} // core

#endif //CORE24_FILEDESCRIPTOR_H
