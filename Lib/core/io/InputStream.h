//
// Created by bruns on 12/10/2024.
//

#ifndef CORE24_INPUTSTREAM_H
#define CORE24_INPUTSTREAM_H

#include <core/io/IOError.h>
#include <core/io/IOException.h>
#include <core/io/Closeable.h>

namespace core {
    namespace io {
        /**
         * This abstract class is the superclass of all classes representing
         * an input stream of bytes.
         *
         * <p>
         * Applications that need to define a subclass of @c InputStream
         * must always provide a method that returns the next byte of input.
         * </p>
         * @see  io::BufferedInputStream
         * @see  io::ByteArrayInputStream
         * @see  io::DataInputStream
         * @see  io::FilterInputStream
         * @see  io::InputStream::read()
         * @see  io::OutputStream
         * @see  io::PushbackInputStream
         */
        class InputStream : public virtual Closeable {
            // MAX_SKIP_BUFFER_SIZE is used to determine the maximum buffer size to
            // use when skipping.
            static CORE_FAST glong MAX_SKIP_BUFFER_SIZE = 2048;

            static CORE_FAST gint DEFAULT_BUFFER_SIZE = 2048;

            /**
             * The maximum size of array to allocate.
             * Some VMs reserve some header words in an array.
             * Attempts to allocate larger arrays may result in
             * OutOfMemoryError: Requested array size exceeds implementation limit
             */
            static CORE_FAST gint MAX_BUFFER_SIZE = Integer::MAX_VALUE - 8;

        public:
            /**
             * Constructor for subclasses to call.
             */
            CORE_IMPLICIT InputStream() = default;

            /**
             * Reads the next byte of data from the input stream. The value byte is
             * returned as an @c int in the range @c 0 to
             * @c 255. If no byte is available because the end of the stream
             * has been reached, the value @c -1 is returned. This method
             * blocks until input data is available, the end of the stream is detected,
             * or an exception is thrown.
             *
             * @return     the next byte of data, or @c -1 if the end of the
             *             stream is reached.
             * @throws     IOException  if an I/O error occurs.
             */
            virtual gint read() = 0;

            /**
             * Reads some number of bytes from the input stream and stores them into
             * the buffer array @c b. The number of bytes actually read is
             * returned as an integer.  This method blocks until input data is
             * available, end of file is detected, or an exception is thrown.
             *
             * <p>
             * If the length of @c b is zero, then no bytes are read and
             * @c 0 is returned; otherwise, there is an attempt to read at
             * least one byte. If no byte is available because the stream is at the
             * end of the file, the value @c -1 is returned; otherwise, at
             * least one byte is read and stored into @c b.
             * </p>
             * <p>
             * The first byte read is stored into element @c b[0], the
             * next one into @c b[1], and so on. The number of bytes read is,
             * at most, equal to the length of @c b. Let <i>k</i> be the
             * number of bytes actually read; these bytes will be stored in elements
             * @c b[0] through @code b[k-1] @endcode ,
             * leaving elements @c b[k] through
             * @code b[b.length()-1] @endcode unaffected.
             * </p>
             * @note
             * The @c read(b) method for class @c InputStream
             * has the same effect as:
             * @code
             *     read(b, 0, b.length)
             * @endcode
             *
             * @param      b   the buffer into which the data is read.
             * @return     the total number of bytes read into the buffer, or
             *             @c -1 if there is no more data because the end of
             *             the stream has been reached.
             * @throws     IOException  If the first byte cannot be read for any reason
             *             other than the end of the file, if the input stream has been
             *             closed, or if some other I/O error occurs.
             * @see        InputStream::read(ByteArray, int, int)
             */
            virtual gint read(ByteArray &b);

            /**
             * Reads up to @c len bytes of data from the input stream into
             * an array of bytes.  An attempt is made to read as many as
             * @c len bytes, but a smaller number may be read.
             * The number of bytes actually read is returned as an integer.
             *
             * <p> This method blocks until input data is available, end of file is
             * detected, or an exception is thrown.
             *
             * <p> If @c len is zero, then no bytes are read and
             * @c 0 is returned; otherwise, there is an attempt to read at
             * least one byte. If no byte is available because the stream is at end of
             * file, the value @c -1 is returned; otherwise, at least one
             * byte is read and stored into @c b.
             *
             * <p> The first byte read is stored into element @c b[off], the
             * next one into @code b[off+1] @endcode , and so on. The number of bytes read
             * is, at most, equal to @c len. Let <i>k</i> be the number of
             * bytes actually read; these bytes will be stored in elements
             * @c b[off] through @code b[off+k -1] @endcode ,
             * leaving elements @code b[off+k] @endcode through
             *@code b[off+len-1] @endcode unaffected.
             *
             * <p>
             * In every case, elements @c b[0] through
             * @code b[off-1] @endcode and elements @code b[off+len] @endcode through
             * @code b[b.length()-1] @endcode are unaffected.
             * </p>
             * @implSpec
             * The @code read(b, off, len) @endcode method
             * for class @c InputStream simply calls the method
             * @c read() repeatedly. If the first such call results in an
             * @c IOException, that exception is returned from the call to
             * the @c read(b, @c off, @c len) method.  If
             * any subsequent call to @c read() results in a
             * @c IOException, the exception is caught and treated as if it
             * were end of file; the bytes read up to that point are stored into
             * @c b and the number of bytes read before the exception
             * occurred is returned. The default implementation of this method blocks
             * until the requested amount of input data @c len has been read,
             * end of file is detected, or an exception is thrown. Subclasses are
             * encouraged to provide a more efficient implementation of this method.
             *
             * @param      b     the buffer into which the data is read.
             * @param      off   the start offset in array @c b
             *                   at which the data is written.
             * @param      len   the maximum number of bytes to read.
             * @return     the total number of bytes read into the buffer, or
             *             @c -1 if there is no more data because the end of
             *             the stream has been reached.
             * @throws     IOException If the first byte cannot be read for any reason
             *             other than end of file, or if the input stream has been closed,
             *             or if some other I/O error occurs.
             * @throws     IndexOutOfBoundsException If @c off is negative,
             *             @c len is negative, or @c len is greater than
             *             @code b.length() - off @endcode
             * @see        io::InputStream::read()
             */
            virtual gint read(ByteArray &b, gint off, gint len);

            /**
             * Reads all remaining bytes from the input stream. This method blocks until
             * all remaining bytes have been read and end of stream is detected, or an
             * exception is thrown. This method does not close the input stream.
             *
             * <p>
             * When this stream reaches end of stream, further invocations of this
             * method will return an empty byte array.
             * </p>
             * <p>
             * Note that this method is intended for simple cases where it is
             * convenient to read all bytes into a byte array. It is not intended for
             * reading input streams with large amounts of data.
             * </p>
             * <p>
             * The behavior for the case where the input stream is <i>asynchronously
             * closed</i>, or the thread interrupted during the read, is highly input
             * stream specific, and therefore not specified.
             * </p>
             * <p>
             * If an I/O error occurs reading from the input stream, then it may do
             * so after some, but not all, bytes have been read. Consequently, the input
             * stream may not be at end of stream and may be in an inconsistent state.
             * It is strongly recommended that the stream be promptly closed if an I/O
             * error occurs.
             * </p>
             * @note
             * This method invokes @b readNBytes(gint) with a length of
             * @b Integer::MAX_VALUE.
             *
             * @return a byte array containing the bytes read from this input stream
             * @throws IOException if an I/O error occurs
             * @throws OutOfMemoryError if an array of the required size cannot be
             *         allocated.
             */
            virtual ByteArray readAllBytes();

            /**
             * Reads up to a specified number of bytes from the input stream. This
             * method blocks until the requested number of bytes has been read, end
             * of stream is detected, or an exception is thrown. This method does not
             * close the input stream.
             *
             * <p> The length of the returned array equals the number of bytes read
             * from the stream. If @c len is zero, then no bytes are read and
             * an empty byte array is returned. Otherwise, up to @c len bytes
             * are read from the stream. Fewer than @c len bytes may be read if
             * end of stream is encountered.
             *
             * <p> When this stream reaches end of stream, further invocations of this
             * method will return an empty byte array.
             *
             * <p> Note that this method is intended for simple cases where it is
             * convenient to read the specified number of bytes into a byte array. The
             * total amount of memory allocated by this method is proportional to the
             * number of bytes read from the stream which is bounded by @c len.
             * Therefore, the method may be safely called with very large values of
             * @c len provided sufficient memory is available.
             *
             * <p> The behavior for the case where the input stream is <i>asynchronously
             * closed</i>, or the thread interrupted during the read, is highly input
             * stream specific, and therefore not specified.
             *
             * <p> If an I/O error occurs reading from the input stream, then it may do
             * so after some, but not all, bytes have been read. Consequently the input
             * stream may not be at end of stream and may be in an inconsistent state.
             * It is strongly recommended that the stream be promptly closed if an I/O
             * error occurs.
             *
             * @implNote
             * The number of bytes allocated to read data from this stream and return
             * the result is bounded by @code 2*(glong)len @endcode , inclusive.
             *
             * @param len the maximum number of bytes to read
             * @return a byte array containing the bytes read from this input stream
             * @throws IllegalArgumentException if @c length is negative
             * @throws IOException if an I/O error occurs
             * @throws OutOfMemoryError if an array of the required size cannot be
             *         allocated.
             */
            virtual ByteArray readNBytes(gint len);

            /**
             * Reads the requested number of bytes from the input stream into the given
             * byte array. This method blocks until @c len bytes of input data have
             * been read, end of stream is detected, or an exception is thrown. The
             * number of bytes actually read, possibly zero, is returned. This method
             * does not close the input stream.
             *
             * <p> In the case where end of stream is reached before @c len bytes
             * have been read, then the actual number of bytes read will be returned.
             * When this stream reaches end of stream, further invocations of this
             * method will return zero.
             *
             * <p> If @c len is zero, then no bytes are read and @c 0 is
             * returned; otherwise, there is an attempt to read up to @c len bytes.
             *
             * <p> The first byte read is stored into element @c b[off], the next
             * one in to @code b[off+1] @endcode , and so on. The number of bytes read is, at
             * most, equal to @c len. Let <i>k</i> be the number of bytes actually
             * read; these bytes will be stored in elements @c b[off] through
             * @code b[off+ k -1] @endcode , leaving elements @code b[off+ k] @endcode
             * through @code b[off+len-1] @endcode unaffected.
             *
             * <p> The behavior for the case where the input stream is <i>asynchronously
             * closed</i>, or the thread interrupted during the read, is highly input
             * stream specific, and therefore not specified.
             *
             * <p> If an I/O error occurs reading from the input stream, then it may do
             * so after some, but not all, bytes of @c b have been updated with
             * data from the input stream. Consequently the input stream and @c b
             * may be in an inconsistent state. It is strongly recommended that the
             * stream be promptly closed if an I/O error occurs.
             *
             * @param  b the byte array into which the data is read
             * @param  off the start offset in @c b at which the data is written
             * @param  len the maximum number of bytes to read
             * @return the actual number of bytes read into the buffer
             * @throws IOException if an I/O error occurs
             * @throws IndexOutOfBoundsException If @c off is negative, @c len
             *         is negative, or @c len is greater than @code b.length() - off @endcode
             *
             * @since 9
             */
            virtual gint readNBytes(ByteArray &b, gint off, gint len);

            /**
             * Skips over and discards @c n bytes of data from this input
             * stream. The @c skip method may, for a variety of reasons, end
             * up skipping over some smaller number of bytes, possibly @c 0.
             * This may result from any of a number of conditions; reaching end of file
             * before @c n bytes have been skipped is only one possibility.
             * The actual number of bytes skipped is returned. If @c n is
             * negative, the @c skip method for class @c InputStream always
             * returns 0, and no bytes are skipped. Subclasses may handle the negative
             * value differently.
             *
             * @note
             * The @c skip method implementation of this class creates a
             * byte array and then repeatedly reads into it until @c n bytes
             * have been read or the end of the stream has been reached. Subclasses are
             * encouraged to provide a more efficient implementation of this method.
             * For instance, the implementation may depend on the ability to seek.
             *
             * @param      n   the number of bytes to be skipped.
             * @return     the actual number of bytes skipped which might be zero.
             * @throws     IOException  if an I/O error occurs.
             * @see        io::InputStream#skipNBytes(long)
             */
            virtual glong skip(glong n);

            /**
             * Skips over and discards exactly @c n bytes of data from this input
             * stream.  If @c n is zero, then no bytes are skipped.
             * If @c n is negative, then no bytes are skipped.
             * Subclasses may handle the negative value differently.
             *
             * <p>
             * This method blocks until the requested number of bytes has been
             * skipped, end of file is reached, or an exception is thrown.
             * </p>
             * <p>
             * If end of stream is reached before the stream is at the desired
             * position, then an @c EOFException is thrown.
             * </p>
             * <p>
             * If an I/O error occurs, then the input stream may be
             * in an inconsistent state. It is strongly recommended that the
             * stream be promptly closed if an I/O error occurs.
             * </p>
             * @note
             * Subclasses are encouraged to provide a more efficient implementation
             * of this method.
             *
             * @note
             * If @c n is zero or negative, then no bytes are skipped.
             * If @c n is positive, the default implementation of this method
             * invokes @b skip() repeatedly with its parameter equal
             * to the remaining number of bytes to skip until the requested number
             * of bytes has been skipped or an error condition occurs.  If at any
             * point the return value of @c skip() is negative or greater than the
             * remaining number of bytes to be skipped, then an @c IOException is
             * thrown.  If @c skip() ever returns zero, then @b read() is
             * invoked to read a single byte, and if it returns @c -1, then an
             * @c EOFException is thrown.  Any exception thrown by @c skip()
             * or @c read() will be propagated.
             *
             * @param      n   the number of bytes to be skipped.
             * @throws     EOFException if end of stream is encountered before the
             *             stream can be positioned @c n bytes beyond its position
             *             when this method was invoked.
             * @throws     IOException  if the stream cannot be positioned properly or
             *             if an I/O error occurs.
             *
             * @see        io::InputStream::skip(glong)
             */
            virtual void skipNBytes(glong n);

            /**
             * Returns an estimate of the number of bytes that can be read (or skipped
             * over) from this input stream without blocking, which may be 0, or 0 when
             * end of stream is detected.  The read might be on the same thread or
             * another thread.  A single read or skip of this many bytes will not block,
             * but may read or skip fewer bytes.
             *
             * <p>
             * Note that while some implementations of @c InputStream will
             * return the total number of bytes in the stream, many will not.  It is
             * never correct to use the return value of this method to allocate
             * a buffer intended to hold all data in this stream.
             * </p>
             * <p>
             * A subclass's implementation of this method may choose to throw an
             * @b IOException if this input stream has been closed by invoking the
             * @b close() method.
             * </p>
             * @note
             * The @c available method of @c InputStream always returns
             * @c 0.
             *
             * @note
             * This method should be overridden by subclasses.
             *
             * @return     an estimate of the number of bytes that can be read (or
             *             skipped over) from this input stream without blocking or
             *             @c 0 when it reaches the end of the input stream.
             * @throws     IOException if an I/O error occurs.
             */
            virtual gint available() const;

            /**
             * Closes this input stream and releases any system resources associated
             * with the stream.
             *
             * @note
             * The @c close method of @c InputStream does
             * nothing.
             *
             * @throws     IOException  if an I/O error occurs.
             */
            void close() override;

            /**
             * Marks the current position in this input stream. A subsequent call to
             * the @c reset method repositions this stream at the last marked
             * position so that subsequent reads re-read the same bytes.
             *
             * <p>
             * The @c readlimit arguments tells this input stream to
             * allow that many bytes to be read before the mark position gets
             * invalidated.
             * </p>
             * <p>
             * The general contract of @c mark is that, if the method
             * @c markSupported returns @c true, the stream somehow
             * remembers all the bytes read after the call to @c mark and
             * stands ready to supply those same bytes again if and whenever the method
             * @c reset is called.  However, the stream is not required to
             * remember any data at all if more than @c readlimit bytes are
             * read from the stream before @c reset is called.
             * </p>
             * <p>
             * Marking a closed stream should not have any effect on the stream.
             * </p>
             * @note
             * The @c mark method of @c InputStream does nothing.
             *
             * @param   readlimit   the maximum limit of bytes that can be read before
             *                      the mark position becomes invalid.
             * @see     io::InputStream#reset()
             */
            virtual void mark(gint readlimit);

            /**
             * Repositions this stream to the position at the time the
             * @c mark method was last called on this input stream.
             *
             * <p> The general contract of @c reset is:
             *
             *
             * @li If the method @c markSupported returns
             * @c true, then:
             *
             *     @li If the method @c mark has not been called since
             *     the stream was created, or the number of bytes read from the stream
             *     since @c mark was last called is larger than the argument
             *     to @c mark at that last call, then an
             *     @c IOException might be thrown.
             *
             *     @li If such an @c IOException is not thrown, then the
             *     stream is reset to a state such that all the bytes read since the
             *     most recent call to @c mark (or since the start of the
             *     file, if @c mark has not been called) will be resupplied
             *     to subsequent callers of the @c read method, followed by
             *     any bytes that otherwise would have been the next input data as of
             *     the time of the call to @c reset.
             *
             * @li If the method @c markSupported returns
             * @c false, then:
             *
             *     @li The call to @c reset may throw an
             *     @c IOException.
             *
             *     @li If an @c IOException is not thrown, then the stream
             *     is reset to a fixed state that depends on the particular type of the
             *     input stream and how it was created. The bytes that will be supplied
             *     to subsequent callers of the @c read method depend on the
             *     particular type of the input stream.
             * </p>
             * @note
             * The method @c reset for class @c InputStream
             * does nothing except throw an @c IOException.
             *
             * @throws  IOException  if this stream has not been marked or if the
             *          mark has been invalidated.
             * @see     io::InputStream::mark(gint)
             * @see     io::IOException
             */
            virtual void reset();

            /**
             * Tests if this input stream supports the @c mark and
             * @c reset methods. Whether @c mark and
             * @c reset are supported is an invariant property of a
             * particular input stream instance.
             *
             * @note
             * The @c markSupported method
             * of @c InputStream returns @c false.
             *
             * @return  @c true if this stream instance supports the mark
             *          and reset methods; @c false otherwise.
             * @see     io::InputStream::mark(gint)
             * @see     io::InputStream::reset()
             */
            virtual gbool supportMark() const;

            /**
             * Reads all bytes from this input stream and writes the bytes to the
             * given output stream in the order that they are read. On return, this
             * input stream will be at end of stream. This method does not close either
             * stream.
             * <p>
             * This method may block indefinitely reading from the input stream, or
             * writing to the output stream. The behavior for the case where the input
             * and/or output stream is <i>asynchronously closed</i>, or the thread
             * interrupted during the transfer, is highly input and output stream
             * specific, and therefore not specified.
             * </p>
             * <p>
             * If the total number of bytes transferred is greater than
             * @em Long::MAX_VALUE, then @c Long::MAX_VALUE will be returned.
             * </p>
             * <p>
             * If an I/O error occurs reading from the input stream or writing to the
             * output stream, then it may do so after some bytes have been read or
             * written. Consequently, the input stream may not be at end of stream and
             * one, or both, streams may be in an inconsistent state. It is strongly
             * recommended that both streams be promptly closed if an I/O error occurs.
             * </p>
             * @param  out the output stream, non-null
             * @return the number of bytes transferred
             * @throws IOException if an I/O error occurs when reading or writing
             */
            virtual glong transferTo(OutputStream &out);

            /**
             * Returns a new @c InputStream that reads no bytes. The returned
             * stream is initially open.  The stream is closed by calling the
             * @c close() method.  Subsequent calls to @c close() have no
             * effect.
             *
             * <p>
             * While the stream is open, the @c available(), @c read(),
             * @code read(ByteArray) @endcode , @code read(ByteArray, gint, gint) @endcode ,
             * @c readAllBytes(), @code readNBytes(ByteArray, gint, gint) @endcode ,
             * @c readNBytes(gint), @c skip(glong), @c skipNBytes(glong),
             * and @c transferTo() methods all behave as if end of stream has been
             * reached.  After the stream has been closed, these methods all throw
             * @c IOException.
             * </p>
             * <p>
             * The @c markSupported() method returns @c false.  The
             * @c mark() method does nothing, and the @c reset() method
             * throws @c IOException.
             * </p>
             * @return an @c InputStream which contains no bytes
             */
            static InputStream &nullStream();
        };
    } // io
} // core

#endif //CORE24_INPUTSTREAM_H
