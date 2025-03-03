//
// Created by bruns on 13/10/2024.
//

#ifndef CORE24_OUTPUTSTREAM_H
#define CORE24_OUTPUTSTREAM_H

#include <core/io/IOError.h>
#include <core/io/Closeable.h>
#include <core/io/Flushable.h>

namespace core {
    namespace io {
        /**
         * This abstract class is the superclass of all classes representing
         * an output stream of bytes. An output stream accepts output bytes
         * and sends them to some sink.
         * <p>
         * Applications that need to define a subclass of
         * @c OutputStream must always provide at least a method
         * that writes one byte of output.
         * </p>
         *
         * @see  io::BufferedOutputStream
         * @see  io::ByteArrayOutputStream
         * @see  io::DataOutputStream
         * @see  io::FilterOutputStream
         * @see  io::InputStream
         * @see  io::OutputStream::write(int)
         */
        class OutputStream : public virtual Closeable, public virtual Flushable {
        public:
            /**
             * Constructor for subclasses to call.
             */
            CORE_IMPLICIT OutputStream() = default;

            /**
             * Writes the specified byte to this output stream. The general
             * contract for @c write is that one byte is written
             * to the output stream. The byte to be written is the eight
             * low-order bits of the argument @c b. The 24
             * high-order bits of @c b are ignored.
             *
             * @param      b   the @c byte.
             * @throws     IOException  if an I/O error occurs. In particular,
             *             an @c IOException may be thrown if the
             *             output stream has been closed.
             */
            virtual void write(gint b) = 0;

            /**
             * Writes @c b.length() bytes from the specified byte array
             * to this output stream. The general contract for @c write(b)
             * is that it should have exactly the same effect as the call
             * @code write(b, 0, b.length()) @endcode .
             *
             * @param      b   the data.
             * @throws     IOException  if an I/O error occurs.
             * @see        java.io.OutputStream#write(byte[], int, int)
             */
            virtual void write(ByteArray const &b);

            /**
             * Writes @c len bytes from the specified byte array
             * starting at offset @c off to this output stream.
             * The general contract for @code write(b, off, len) @endcode is that
             * some of the bytes in the array @c b are written to the
             * output stream in order; element @c b[off] is the first
             * byte written and @code b[off+len-1] @endcode is the last byte written
             * by this operation.
             *
             *
             * <p>
             * If @c off is negative, or @c len is negative, or
             * @c off+len is greater than the length of the array
             * @c b, then an @c IndexOutOfBoundsException is thrown.
             * </p>
             * @note
             * The @c write method of @c OutputStream calls
             * the write method of one argument on each of the bytes to be
             * written out.
             *
             * @apiNote
             * Subclasses are encouraged to override this method and
             * provide a more efficient implementation.
             *
             * @param      b     the data.
             * @param      off   the start offset in the data.
             * @param      len   the number of bytes to write.
             * @throws     IOException  if an I/O error occurs. In particular,
             *             an @c IOException is thrown if the output
             *             stream is closed.
             * @throws     IndexOutOfBoundsException If @c off is negative,
             *             @c len is negative, or @c len is greater than
             *             @code b.length() - off @endcode
             */
            virtual void write(ByteArray const &b, gint off, gint len);

            /**
             * Flushes this output stream and forces any buffered output bytes
             * to be written out. The general contract of @c flush is
             * that calling it is an indication that, if any bytes previously
             * written have been buffered by the implementation of the output
             * stream, such bytes should immediately be written to their
             * intended destination.
             * <p>
             * If the intended destination of this stream is an abstraction provided by
             * the underlying operating system, for example a file, then flushing the
             * stream guarantees only that bytes previously written to the stream are
             * passed to the operating system for writing; it does not guarantee that
             * they are actually written to a physical device such as a disk drive.
             * </p>
             * @note
             * The @c flush method of @c OutputStream does nothing.
             *
             * @throws     IOException  if an I/O error occurs.
             */
            void flush() override;

            /**
             * Closes this output stream and releases any system resources
             * associated with this stream. The general contract of @c close
             * is that it closes the output stream. A closed stream cannot perform
             * output operations and cannot be reopened.
             *
             * @note
             * The @c close method of @c OutputStream does nothing.
             *
             * @throws     IOException  if an I/O error occurs.
             */
            void close() override;

            /**
             * Returns a new @c OutputStream which discards all bytes.  The
             * returned stream is initially open.  The stream is closed by calling
             * the @c close() method.  Subsequent calls to @c close() have
             * no effect.
             *
             * <p>
             * While the stream is open, the @c write(int),
             * @code write(byte[]) @endcode , and @code write(byte[], int, int) @endcode methods do nothing.
             * After the stream has been closed, these methods all throw
             * @c IOException.
             * </p>
             * <p>
             * The @c flush() method does nothing.
             * </p>
             * @return an @c OutputStream which discards all bytes
             */
            static OutputStream &nullStream();
        };
    } // io
} // core

#endif //CORE24_OUTPUTSTREAM_H
