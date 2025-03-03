//
// Created by brunshweeck on 27 août 2024.
//

#ifndef CORE24_BYTEBUFFER_H
#define CORE24_BYTEBUFFER_H

#include <core/io/Buffer.h>
#include <core/io/CharBuffer.h>
#include <core/io/DoubleBuffer.h>
#include <core/io/FloatBuffer.h>
#include <core/io/IntBuffer.h>
#include <core/io/LongBuffer.h>
#include <core/io/ShortBuffer.h>
#include <core/charset/Charset.h>

namespace core {
  namespace io {
    /**
     * A byte buffer.
     *
     * This class defines six categories of operations upon
     * byte buffers:
     *
     *  - Absolute and relative <i>get</i> and
     *   <i>put</i> methods that read and write
     *   single bytes; <br/>
     *
     *  - Absolute and relative <i>bulk get</i>
     *   methods that transfer contiguous sequences of bytes from this buffer
     *   into an array <br/>
     *
     *  - Absolute and relative <i>bulk put</i>
     *   methods that transfer contiguous sequences of bytes from a
     *   byte array or some other byte buffer into this buffer <br/>
     *
     *  - Absolute and relative <i>get</i> and <i>put</i> methods that read and
     *   write values of other primitive types, translating them to and from
     *   sequences of bytes in a particular byte order; <br/>
     *
     *  - Methods for creating <i><a href="#views">view buffers</a></i>,
     *   which allow a byte buffer to be viewed as a buffer containing values of
     *   some other primitive type; and <br/>
     *
     *
     *  - A method for @b compacting a byte buffer.  <br/>
     *
     * <p>
     * Byte buffers can be created either by
     * <i>allocation</i>, which allocates space for the buffer's
     *
     * content, or by <i>wrapping</i> an existing byte array into a buffer.
     *
     * <h2> Direct vs. non-direct buffers </h2>
     *
     * <p> A byte buffer is either <i>direct</i> or <i>non-direct</i>.  Given a
     * direct byte buffer, the Java virtual machine will make a best effort to
     * perform native I/O operations directly upon it.  That is, it will attempt to
     * avoid copying the buffer's content to (or from) an intermediate buffer
     * before (or after) each invocation of one of the underlying operating
     * system's native I/O operations.
     *
     * <p> A direct byte buffer may be created by invoking the
     * @b allocateDirect factory method of this class.  The
     * buffers returned by this method typically have somewhat higher allocation
     * and deallocation costs than non-direct buffers.  The contents of direct
     * buffers may reside outside the normal garbage-collected heap, and so
     * their impact upon the memory footprint of an application might not be
     * obvious.  It is therefore recommended that direct buffers be allocated
     * primarily for large, long-lived buffers that are subject to the underlying
     * system's native I/O operations.  In general, it is best to allocate direct
     * buffers only when they yield a measurable gain in program performance.
     *
     * <p> A direct byte buffer may also be created by
     * @b mapping a region of a file
     * directly into memory.  An implementation of the Java platform may optionally
     * support the creation of direct byte buffers from native code via JNI.  If an
     * instance of one of these kinds of buffers refers to an inaccessible region
     * of memory then an attempt to access that region will not change the buffer's
     * content and will cause an unspecified exception to be thrown either at the
     * time of the access or at some later time.
     *
     * <p> Whether a byte buffer is direct or non-direct may be determined by
     * invoking its @b isDirect method.  This method is provided so
     * that explicit buffer management can be done in performance-critical code.
     *
     *
     * <a id="bin"></a>
     * <h2> Access to binary data </h2>
     *
     * <p> This class defines methods for reading and writing values of all other
     * primitive types, except @c boolean.  Primitive values are translated
     * to (or from) sequences of bytes according to the buffer's current byte
     * order, which may be retrieved and modified via the @b order
     * methods.  Specific byte orders are represented by instances of the
     * @b ByteOrder class.  The initial order of a byte buffer is always
     * @b BIG_ENDIAN}.
     *
     * <p> For access to heterogeneous binary data, that is, sequences of values of
     * different types, this class defines a family of absolute and relative
     * <i>get</i> and <i>put</i> methods for each type.  For 32-bit floating-point
     * values, for example, this class defines:
     *
     * @code
     *     gfloat      getFloat()
     *     gfloat      getFloat(int index)
     *     ByteBuffer putFloat(gfloat f)
     *     ByteBuffer putFloat(int index, gfloat f)
     * @endcode
     *
     * <p> Corresponding methods are defined for the types @c gchar,
     * @c gshort, @c gint, @c glong, and @c gdouble.  The index
     * parameters of the absolute <i>get</i> and <i>put</i> methods are in terms of
     * bytes rather than of the type being read or written.
     *
     * <a id="views"></a>
     *
     * <p> For access to homogeneous binary data, that is, sequences of values of
     * the same type, this class defines methods that can create <i>views</i> of a
     * given byte buffer.  A <i>view buffer</i> is simply another buffer whose
     * content is backed by the byte buffer.  Changes to the byte buffer's content
     * will be visible in the view buffer, and vice versa; the two buffers'
     * position, limit, and mark values are independent.  The
     * @b asFloatBuffer method, for example, creates an instance of
     * the @b FloatBuffer class that is backed by the byte buffer upon which
     * the method is invoked.  Corresponding view-creation methods are defined for
     * the types @c gchar, @c gshort, @c gint, @c glong, and @c gdouble.
     *
     * <p>
     * View buffers have three important advantages over the families of
     * type-specific <i>get</i> and <i>put</i> methods described above: <br/>
     *
     *  - A view buffer is indexed not in terms of bytes but rather in terms
     *   of the type-specific size of its values; <br/>
     *
     *  - A view buffer provides relative bulk <i>get</i> and <i>put</i>
     *   methods that can transfer contiguous sequences of values between a buffer
     *   and an array or some other buffer of the same type; and <br/>
     *
     *  - A view buffer is potentially much more efficient because it will
     *   be direct if, and only if, its backing byte buffer is direct. <br/>
     *
     * </p>
     * <p>
     * The byte order of a view buffer is fixed to be that of its byte buffer
     * at the time that the view is created.
     * </p>
     * <h2> Invocation chaining </h2>
     *
     * <p>
     * Methods in this class that do not otherwise have a value to return are
     * specified to return the buffer upon which they are invoked.  This allows
     * method invocations to be chained.
     *
     * The sequence of statements
     *
     * @code
     *     bb.putInt(0xCAFEBABE);
     *     bb.putShort(3);
     *     bb.putShort(45);
     * @endcode
     *
     * can, for example, be replaced by the single statement
     *
     * @code  bb.putInt(0xCAFEBABE).putShort(3).putShort(45); @endcode
     *
     * <h2> Optional operations </h2>
     * <p>
     * Methods specified as <i>optional operations</i> throw a
     * @em ReadOnlyBufferException when invoked on a @em read-only ByteBuffer.
     * The @em array and @em arrayOffset throw an @em UnsupportedOperationException
     * if the ByteBuffer is not backed by an <em> accessible byte array</em>
     * (irrespective of whether the ByteBuffer is read-only).
     * </p>
     */
    class ByteBuffer : public virtual Buffer, public virtual Comparable<ByteBuffer> {
    protected:
      /**
       * Creates a new buffer with the given mark, position, limit, and capacity
       *
       * @param mark
       * @param position
       * @param limit
       * @param capacity
       * @param segment
       */
      CORE_EXPLICIT ByteBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment);

    public:
      /**
       * Allocates a new direct byte buffer.
       *
       * <p> The new buffer's position will be zero, its limit will be its
       * capacity, its mark will be undefined, each of its elements will be
       * initialized to zero, and its byte order will be
       * @b BIG_ENDIAN.  Whether it has a <b>backing array </b> is unspecified.
       *
       * @param  capacity
       *         The new buffer's capacity, in bytes
       *
       * @return  The new byte buffer
       *
       * @throws  IllegalArgumentException
       *          If the @c capacity is a negative integer
       */
      static ByteBuffer& allocateDirect(gint capacity);

      /**
       * Allocates a new byte buffer.
       *
       * <p> The new buffer's position will be zero, its limit will be its
       * capacity, its mark will be undefined, each of its elements will be
       * initialized to zero, and its byte order will be
       * @b BIG_ENDIAN.
       * It will have a <b>backing array</b>, and its
       * <b>array offset</b> will be zero.
       *
       * @param  capacity
       *         The new buffer's capacity, in bytes
       *
       * @return  The new byte buffer
       *
       * @throws  IllegalArgumentException
       *          If the @c capacity is a negative integer
       */
      static ByteBuffer& allocate(gint capacity);

      /**
       * Wraps a byte array into a buffer.
       *
       * <p> The new buffer will be backed by the given byte array;
       * that is, modifications to the buffer will cause the array to be modified
       * and vice versa.  The new buffer's capacity will be
       * @c array.length(), its position will be @c offset, its limit
       * will be @code offset + length @endcode, its mark will be undefined, and its
       * byte order will be
       * @b BIG_ENDIAN.
       * Its <b>backing array</b> will be the given array, and
       * its <b>array offset</b> will be zero.  </p>
       *
       * @param  array
       *         The array that will back the new buffer
       *
       * @param  offset
       *         The offset of the subarray to be used; must be non-negative and
       *         no larger than @c array.length().  The new buffer's position
       *         will be set to this value.
       *
       * @param  length
       *         The length of the subarray to be used;
       *         must be non-negative and no larger than
       *         @code array.length() - offset @endcode .
       *         The new buffer's limit will be set to @code offset + length @endcode.
       *
       * @return  The new byte buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c offset and @c length
       *          parameters do not hold
       */
      static ByteBuffer& wrap(ByteArray& array, gint offset, gint length);

      /**
       * Wraps a byte array into a buffer.
       *
       * <p> The new buffer will be backed by the given byte array;
       * that is, modifications to the buffer will cause the array to be modified
       * and vice versa.  The new buffer's capacity and limit will be
       * @c array.length(), its position will be zero, its mark will be
       * undefined, and its byte order will be
       * @b BIG_ENDIAN.
       * Its <b>backing array</b> will be the given array, and its
       * <b>array offset</b> will be zero.  </p>
       *
       * @param  array
       *         The array that will back this buffer
       *
       * @return  The new byte buffer
       */
      static ByteBuffer& wrap(ByteArray& array);

      /**
       * Creates a new byte buffer whose content is a shared subsequence of
       * this buffer's content.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position.  Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of bytes remaining in this buffer, its mark will be
       * undefined, and its byte order will be
       * @b BIG_ENDIAN.
       * The new buffer will be direct if, and only if, this buffer is direct, and
       * it will be read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  The new byte buffer

       *
       * @see alignedSlice(int)

       */
      ByteBuffer& slice() const override = 0;

      /**
       * Creates a new byte buffer whose content is a shared subsequence of
       * this buffer's content.
       *
       * <p> The content of the new buffer will start at position @c index
       * in this buffer, and will contain @c length elements. Changes to
       * this buffer's content will be visible in the new buffer, and vice versa;
       * the two buffers' position, limit, and mark values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be @c length, its mark will be undefined, and its byte order
       * will be

       * @b BIG_ENDIAN.



       * The new buffer will be direct if, and only if, this buffer is direct,
       * and it will be read-only if, and only if, this buffer is read-only. </p>
       *
       * @param   index
       *          The position in this buffer at which the content of the new
       *          buffer will start; must be non-negative and no larger than
       *          @b limit()
       *
       * @param   length
       *          The number of elements the new buffer will contain; must be
       *          non-negative and no larger than @code limit() - index @endcode
       *
       * @return  The new buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative or greater than @c limit(),
       *          @c length is negative, or @code length > limit() - index @endcode
       *
       * @since 13
       */
      ByteBuffer& slice(gint index, gint length) const override = 0;

      /**
       * Creates a new byte buffer that shares this buffer's content.
       *
       * <p> The content of the new buffer will be that of this buffer.  Changes
       * to this buffer's content will be visible in the new buffer, and vice
       * versa; the two buffers' position, limit, and mark values will be
       * independent.
       *
       * <p> The new buffer's capacity, limit, position,
       * and mark values will be identical to those of this buffer, and its byte
       * order will be @b BIG_ENDIAN.
       * The new buffer will be direct if, and only if, this buffer is direct, and
       * it will be read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  The new byte buffer
       */
      ByteBuffer& duplicate() const override = 0;

      /**
       * Creates a new, read-only byte buffer that shares this buffer's
       * content.
       *
       * <p> The content of the new buffer will be that of this buffer.  Changes
       * to this buffer's content will be visible in the new buffer; the new
       * buffer itself, however, will be read-only and will not allow the shared
       * content to be modified.  The two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's capacity, limit, position,

       * and mark values will be identical to those of this buffer, and its byte
       * order will be @b BIG_ENDIAN.
       *
       * <p> If this buffer is itself read-only then this method behaves in
       * exactly the same way as the @b duplicate method.  </p>
       *
       * @return  The new, read-only byte buffer
       */
      virtual ByteBuffer& asReadOnlyBuffer() const = 0;

      /**
       * Relative <i>get</i> method.  Reads the byte at this buffer's
       * current position, and then increments the position.
       *
       * @return  The byte at the buffer's current position
       *
       * @throws  BufferUnderflowException
       *          If the buffer's current position is not smaller than its limit
       */
      virtual gbyte get() = 0;

      /**
       * Relative <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes the given byte into this buffer at the current
       * position, and then increments the position. </p>
       *
       * @param  b
       *         The byte to be written
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If this buffer's current position is not smaller than its limit
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& put(gbyte b) = 0;

      /**
       * Absolute <i>get</i> method.  Reads the byte at the given
       * index.
       *
       * @param  index
       *         The index from which the byte will be read
       *
       * @return  The byte at the given index
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit
       */
      virtual gbyte get(gint index) const = 0;

      /**
       * Absolute <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes the given byte into this buffer at the given
       * index. </p>
       *
       * @param  index
       *         The index at which the byte will be written
       *
       * @param  b
       *         The byte value to be written
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& put(gint index, gbyte b) = 0;

      /**
       * Relative bulk <i>get</i> method.
       *
       * <p> This method transfers bytes from this buffer into the given
       * destination array.  If there are fewer bytes remaining in the
       * buffer than are required to satisfy the request, that is, if
       * @c length&nbsp;@c >&nbsp;@c remaining(), then no
       * bytes are transferred and a @b BufferUnderflowException is
       * thrown.
       *
       * <p> Otherwise, this method copies @c length bytes from this
       * buffer into the given array, starting at the current position of this
       * buffer and at the given offset in the array.  The position of this
       * buffer is then incremented by @c length.
       *
       * <p> In other words, an invocation of this method of the form
       * <code>src.get(dst,&nbsp;off,&nbsp;len)</code> has exactly the same effect as
       * the loop
       *
       * @code
       *     for (int i = off; i < off + len; i++)
       *         dst[i] = src.get();
       * @endcode
       *
       * except that it first checks that there are sufficient bytes in
       * this buffer and it is potentially much more efficient.
       *
       * @param  dst
       *         The array into which bytes are to be written
       *
       * @param  offset
       *         The offset within the array of the first byte to be
       *         written; must be non-negative and no larger than
       *         @c dst.length()
       *
       * @param  length
       *         The maximum number of bytes to be written to the given
       *         array; must be non-negative and no larger than
       *         @code dst.length() - offset @endcode
       *
       * @return  This buffer
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than @c length bytes
       *          remaining in this buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c offset and @c length
       *          parameters do not hold
       */
      virtual ByteBuffer& get(ByteArray& dst, gint offset, gint length);

      /**
       * Relative bulk <i>get</i> method.
       *
       * <p> This method transfers bytes from this buffer into the given
       * destination array.  An invocation of this method of the form
       * @c src.get(a) behaves in exactly the same way as the invocation
       *
       * @code
       *     src.get(a, 0, a.length)
       * @endcode
       *
       * @param   dst
       *          The destination array
       *
       * @return  This buffer
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than @c length bytes
       *          remaining in this buffer
       */
      virtual ByteBuffer& get(ByteArray& dst);

      /**
       * Absolute bulk <i>get</i> method.
       *
       * <p> This method transfers @c length bytes from this
       * buffer into the given array, starting at the given index in this
       * buffer and at the given offset in the array.  The position of this
       * buffer is unchanged.
       *
       * <p> An invocation of this method of the form
       * <code>src.get(index,&nbsp;dst,&nbsp;offset,&nbsp;length)</code>
       * has exactly the same effect as the following loop except that it first
       * checks the consistency of the supplied parameters and it is potentially
       * much more efficient:
       *
       * @code
       *     for (int i = offset, j = index; i < offset + length; i++, j++)
       *         dst[i] = src.get(j);
       * @endcode
       *
       * @param  index
       *         The index in this buffer from which the first byte will be
       *         read; must be non-negative and less than @c limit()
       *
       * @param  dst
       *         The destination array
       *
       * @param  offset
       *         The offset within the array of the first byte to be
       *         written; must be non-negative and less than
       *         @c dst.length()
       *
       * @param  length
       *         The number of bytes to be written to the given array;
       *         must be non-negative and no larger than the smaller of
       *         @code limit() - index @endcode and @code dst.length() - offset @endcode
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c index, @c offset, and
       *          @c length parameters do not hold
       *
       * @since 13
       */
      virtual ByteBuffer& get(gint index, ByteArray& dst, gint offset, gint length);

      /**
       * Absolute bulk <i>get</i> method.
       *
       * <p> This method transfers bytes from this buffer into the given
       * destination array.  The position of this buffer is unchanged.  An
       * invocation of this method of the form
       * <code>src.get(index,&nbsp;dst)</code> behaves in exactly the same
       * way as the invocation:
       *
       * @code
       *     src.get(index, dst, 0, dst.length)
       * @endcode
       *
       * @param  index
       *         The index in this buffer from which the first byte will be
       *         read; must be non-negative and less than @c limit()
       *
       * @param  dst
       *         The destination array
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative, not smaller than @c limit(),
       *          or @code limit() - index < dst.length() @endcode
       *
       * @since 13
       */
      virtual ByteBuffer& get(gint index, ByteArray& dst);

      /**
       * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> This method transfers the bytes remaining in the given source
       * buffer into this buffer.  If there are more bytes remaining in the
       * source buffer than in this buffer, that is, if
       * @c src.remaining()&nbsp;@c >&nbsp;@c remaining(),
       * then no bytes are transferred and a @b BufferOverflowException is thrown.
       *
       * <p> Otherwise, this method copies
       * <i>n</i>&nbsp;=&nbsp;@c src.remaining() bytes from the given
       * buffer into this buffer, starting at each buffer's current position.
       * The positions of both buffers are then incremented by <i>n</i>.
       *
       * <p> In other words, an invocation of this method of the form
       * @c dst.put(src) has exactly the same effect as the loop
       *
       * @code
       *     while (src.hasRemaining())
       *         dst.put(src.get());
       * @endcode
       *
       * except that it first checks that there is sufficient space in this
       * buffer and it is potentially much more efficient.  If this buffer and
       * the source buffer share the same backing array or memory, then the
       * result will be as if the source elements were first copied to an
       * intermediate location before being written into this buffer.
       *
       * @param  src
       *         The source buffer from which bytes are to be read;
       *         must not be this buffer
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *          for the remaining bytes in the source buffer
       *
       * @throws  IllegalArgumentException
       *          If the source buffer is this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& put(ByteBuffer& src);

      /**
       * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> This method transfers @c length bytes into this buffer from
       * the given source buffer, starting at the given @c offset in the
       * source buffer and the given @c index in this buffer. The positions
       * of both buffers are unchanged.
       *
       * <p> In other words, an invocation of this method of the form
       * <code>dst.put(index,&nbsp;src,&nbsp;offset,&nbsp;length)</code>
       * has exactly the same effect as the loop
       *
       * @code
       *     for (int i = offset, j = index; i < offset + length; i++, j++)
       *         dst.put(j, src.get(i));
       * @endcode
       *
       * except that it first checks the consistency of the supplied parameters
       * and it is potentially much more efficient.  If this buffer and
       * the source buffer share the same backing array or memory, then the
       * result will be as if the source elements were first copied to an
       * intermediate location before being written into this buffer.
       *
       * @param index
       *        The index in this buffer at which the first byte will be
       *        written; must be non-negative and less than @c limit()
       *
       * @param src
       *        The buffer from which bytes are to be read
       *
       * @param offset
       *        The index within the source buffer of the first byte to be
       *        read; must be non-negative and less than @c src.limit()
       *
       * @param length
       *        The number of bytes to be read from the given buffer;
       *        must be non-negative and no larger than the smaller of
       *        @code limit() - index @endcode and @code src.limit() - offset @endcode
       *
       * @return This buffer
       *
       * @throws IndexOutOfBoundsException
       *         If the preconditions on the @c index, @c offset, and
       *         @c length parameters do not hold
       *
       * @throws ReadOnlyBufferException
       *         If this buffer is read-only
       *
       * @since 16
       */
      virtual ByteBuffer& put(gint index, ByteBuffer& src, gint offset, gint length);

      /**
       * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> This method transfers bytes into this buffer from the given
       * source array.  If there are more bytes to be copied from the array
       * than remain in this buffer, that is, if
       * @c length&nbsp;@c >&nbsp;@c remaining(), then no
       * bytes are transferred and a @b BufferOverflowException is
       * thrown.
       *
       * <p> Otherwise, this method copies @c length bytes from the
       * given array into this buffer, starting at the given offset in the array
       * and at the current position of this buffer.  The position of this buffer
       * is then incremented by @c length.
       *
       * <p> In other words, an invocation of this method of the form
       * <code>dst.put(src,&nbsp;off,&nbsp;len)</code> has exactly the same effect as
       * the loop
       *
       * @code
       *     for (int i = off; i < off + len; i++)
       *         dst.put(src[i]);
       * @endcode
       *
       * except that it first checks that there is sufficient space in this
       * buffer and it is potentially much more efficient.
       *
       * @param  src
       *         The array from which bytes are to be read
       *
       * @param  offset
       *         The offset within the array of the first byte to be read;
       *         must be non-negative and no larger than @c src.length()
       *
       * @param  length
       *         The number of bytes to be read from the given array;
       *         must be non-negative and no larger than
       *         @code src.length() - offset @endcode
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c offset and @c length
       *          parameters do not hold
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& put(ByteArray const& src, gint offset, gint length);

      /**
       * Relative bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> This method transfers the entire content of the given source
       * byte array into this buffer.  An invocation of this method of the
       * form @c dst.put(a) behaves in exactly the same way as the
       * invocation
       *
       * @code
       *     dst.put(a, 0, a.length)
       * @endcode
       *
       * @param   src
       *          The source array
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& put(ByteArray const& src);

      /**
       * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> This method transfers @c length bytes from the given
       * array, starting at the given offset in the array and at the given index
       * in this buffer.  The position of this buffer is unchanged.
       *
       * <p> An invocation of this method of the form
       * <code>dst.put(index,&nbsp;src,&nbsp;offset,&nbsp;length)</code>
       * has exactly the same effect as the following loop except that it first
       * checks the consistency of the supplied parameters and it is potentially
       * much more efficient:
       *
       * @code
       *     for (int i = offset, j = index; i < offset + length; i++, j++)
       *         dst.put(j, src[i]);
       * @endcode
       *
       * @param  index
       *         The index in this buffer at which the first byte will be
       *         written; must be non-negative and less than @c limit()
       *
       * @param  src
       *         The array from which bytes are to be read
       *
       * @param  offset
       *         The offset within the array of the first byte to be read;
       *         must be non-negative and less than @c src.length()
       *
       * @param  length
       *         The number of bytes to be read from the given array;
       *         must be non-negative and no larger than the smaller of
       *         @code limit() - index @endcode and @code src.length() - offset @endcode
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c index, @c offset, and
       *          @c length parameters do not hold
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       *
       * @since 13
       */
      virtual ByteBuffer& put(gint index, ByteArray const& src, gint offset, gint length);

      /**
       * Absolute bulk <i>put</i> method&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> This method copies bytes into this buffer from the given source
       * array.  The position of this buffer is unchanged.  An invocation of this
       * method of the form <code>dst.put(index,&nbsp;src)</code>
       * behaves in exactly the same way as the invocation:
       *
       * @code
       *     dst.put(index, src, 0, src.length);
       * @endcode
       *
       * @param  index
       *         The index in this buffer at which the first byte will be
       *         written; must be non-negative and less than @c limit()
       *
       * @param  src
       *         The array from which bytes are to be read
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative, not smaller than @c limit(),
       *          or @code limit() - index < src.length() @endcode
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& put(gint index, ByteArray const& src);

      /**
       * Tells whether this buffer is backed by an accessible byte
       * array.
       *
       * <p> If this method returns @c true then the @b array
       * and @b arrayOffset methods may safely be invoked.
       * </p>
       *
       * @return  @c true if, and only if, this buffer
       *          is backed by an array and is not read-only
       */
      gbool hasArray() const override = 0;

      /**
       * Returns the byte array that backs this
       * buffer&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Modifications to this buffer's content will cause the returned
       * array's content to be modified, and vice versa.
       *
       * <p> Invoke the @b hasArray method before invoking this
       * method in order to ensure that this buffer has an accessible backing
       * array.  </p>
       *
       * @return  The array that backs this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is backed by an array but is read-only
       *
       * @throws  UnsupportedOperationException
       *          If this buffer is not backed by an accessible array
       */
      ByteArray& array() const override = 0;

      /**
       * Returns the offset within this buffer's backing array of the first
       * element of the buffer&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> If this buffer is backed by an array then buffer position <i>p</i>
       * corresponds to array index <i>p</i>&nbsp;+&nbsp;@c arrayOffset().
       *
       * <p> Invoke the @b hasArray method before invoking this
       * method in order to ensure that this buffer has an accessible backing
       * array.  </p>
       *
       * @return  The offset within this buffer's array
       *          of the first element of the buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is backed by an array but is read-only
       *
       * @throws  UnsupportedOperationException
       *          If this buffer is not backed by an accessible array
       */
      gint arrayOffset() const override = 0;

      gint position() const override;

      ByteBuffer& position(gint newPosition) override;

      gint limit() const override;

      ByteBuffer& limit(gint newLimit) override;

      ByteBuffer& mark() override;

      ByteBuffer& reset() override;

      ByteBuffer& clear() override;

      ByteBuffer& flip() override;

      ByteBuffer& rewind() override;

      /**
       * Compacts this buffer&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> The bytes between the buffer's current position and its limit,
       * if any, are copied to the beginning of the buffer.  That is, the
       * byte at index <i>p</i>&nbsp;=&nbsp;@c position() is copied
       * to index zero, the byte at index <i>p</i>&nbsp;+&nbsp;1 is copied
       * to index one, and so forth until the byte at index
       * @c limit()&nbsp;-&nbsp;1 is copied to index
       * <i>n</i>&nbsp;=&nbsp;@c limit()&nbsp;-&nbsp;@c 1&nbsp;-&nbsp;<i>p</i>.
       * The buffer's position is then set to <i>n+1</i> and its limit is set to
       * its capacity.  The mark, if defined, is discarded.
       *
       * <p> The buffer's position is set to the number of bytes copied,
       * rather than to zero, so that an invocation of this method can be
       * followed immediately by an invocation of another relative <i>put</i>
       * method. </p>
       *
       *
       * <p> Invoke this method after writing data from a buffer in case the
       * write was incomplete.  The following loop, for example, copies bytes
       * from one channel to another via the buffer @c buf:
       *
       * @code
       *     buf.clear();          // Prepare buffer for use
       *     while (in.read(buf) >= 0 || buf.position() != 0) {
       *         buf.flip();
       *         out.write(buf);
       *         buf.compact();    // In case of partial write
       *     }
       * @endcode
       *

       *
       * @return  This buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& compact() = 0;

      /**
       * Tells whether this byte buffer is direct.
       *
       * @return  @c true if, and only if, this buffer is direct
       */
      gbool isDirect() const override = 0;

      /**
       * Returns a string summarizing the state of this buffer.
       *
       * @return  A summary string
       */
      String toString() const override;

      /**
       * Returns the current hash code of this buffer.
       *
       * <p> The hash code of a byte buffer depends only upon its remaining
       * elements; that is, upon the elements from @c position() up to, and
       * including, the element at @c limit()&nbsp;-&nbsp;@c 1.
       *
       * <p> Because buffer hash codes are content-dependent, it is inadvisable
       * to use buffers as keys in hash maps or similar data structures unless it
       * is known that their contents will not change.  </p>
       *
       * @return  The current hash code of this buffer
       */
      gint hash() const override;

      /**
       * Tells whether this buffer is equal to another object.
       *
       * <p> Two byte buffers are equal if, and only if,
       *
       *  - They have the same element type, <br/>
       *
       *  - They have the same number of remaining elements, and <br/>
       *
       *  - The two sequences of remaining elements, considered
       *   independently of their starting positions, are pointwise equal. <br/>
       *
       * <p> A byte buffer is not equal to any other type of object.  </p>
       *
       * @param  o  The object to which this buffer is to be compared
       *
       * @return  @c true if, and only if, this buffer is equal to the
       *           given object
       */
      gbool equals(const Object& o) const override;

      /**
       * Compares this buffer to another.
       *
       * <p> Two byte buffers are compared by comparing their sequences of
       * remaining elements lexicographically, without regard to the starting
       * position of each sequence within its corresponding buffer.
       * Pairs of @c byte elements are compared as if by invoking
       * @b Byte::compare(byte,byte).

       *
       * <p> A byte buffer is not comparable to any other type of object.
       *
       * @return  A negative integer, zero, or a positive integer as this buffer
       *          is less than, equal to, or greater than the given buffer
       */
      gint compareTo(const ByteBuffer& that) const override;

      /**
       * Finds and returns the relative index of the first mismatch between this
       * buffer and a given buffer.  The index is relative to the
       * @b position of each buffer and will be in the range of
       * 0 (inclusive) up to the smaller of the @b remaining
       * elements in each buffer (exclusive).
       *
       * <p> If the two buffers share a common prefix then the returned index is
       * the length of the common prefix and it follows that there is a mismatch
       * between the two buffers at that index within the respective buffers.
       * If one buffer is a proper prefix of the other then the returned index is
       * the smaller of the remaining elements in each buffer, and it follows that
       * the index is only valid for the buffer with the larger number of
       * remaining elements.
       * Otherwise, there is no mismatch.
       *
       * @param  that
       *         The byte buffer to be tested for a mismatch with this buffer
       *
       * @return  The relative index of the first mismatch between this and the
       *          given buffer, otherwise -1 if no mismatch.
       */
      gint mismatch(ByteBuffer const& that) const;

      /**
       * Retrieves this buffer's byte order.
       *
       * <p> The byte order is used when reading or writing multibyte values, and
       * when creating buffers that are views of this byte buffer.  The order of
       * a newly-created byte buffer is always @b BIG_ENDIAN.  </p>
       *
       * @return  This buffer's byte order
       */
      virtual charset::Charset::ByteOrder order() const;

      /**
       * Modifies this buffer's byte order.
       *
       * @param  bo
       *         The new byte order, either @b BIG_ENDIAN or @b LITTLE_ENDIAN
       *
       * @return  This buffer
       */
      virtual ByteBuffer& order(charset::Charset::ByteOrder bo);

      /**
       * Returns the memory address, pointing to the byte at the given index,
       * modulo the given unit size.
       *
       * <p> The return value is non-negative in the range of @c 0
       * (inclusive) up to @c unitSize (exclusive), with zero indicating
       * that the address of the byte at the index is aligned for the unit size,
       * and a positive value that the address is misaligned for the unit size.
       * If the address of the byte at the index is misaligned, the return value
       * represents how much the index should be adjusted to locate a byte at an
       * aligned address.  Specifically, the index should either be decremented by
       * the return value if the latter is not greater than @c index, or be
       * incremented by the unit size minus the return value.  Therefore given
       * @code
       *     int value = alignmentOffset(index, unitSize)
       * @endcode
       * then the identities
       * @code
       *     alignmentOffset(index - value, unitSize) == 0, value <= index
       * @endcode
       * and
       * @code
       *     alignmentOffset(index + (unitSize - value), unitSize) == 0
       * @endcode
       * must hold.
       *
       * @note
       * This method may be utilized to determine if unit size bytes from an
       * index can be accessed atomically, if supported by the native platform.
       *
       * @note
       * This implementation throws @c UnsupportedOperationException for
       * non-direct buffers when the given unit size is greater then @c 8.
       *
       * @param  index
       *         The index to query for alignment offset, must be non-negative, no
       *         upper bounds check is performed
       *
       * @param  unitSize
       *         The unit size in bytes, must be a power of @c 2
       *
       * @return  The indexed byte's memory address modulo the unit size
       *
       * @throws IllegalArgumentException
       *         If the index is negative or the unit size is not a power of
       *         @c 2
       *
       * @throws UnsupportedOperationException
       *         If the native platform does not guarantee stable alignment offset
       *         values for the given unit size when managing the memory regions
       *         of buffers of the same kind as this buffer (direct or
       *         non-direct).  For example, if garbage collection would result
       *         in the moving of a memory region covered by a non-direct buffer
       *         from one location to another and both locations have different
       *         alignment characteristics.
       */
      gint alignmentOffset(gint index, gint unitSize) const;

      /**
       * Creates a new byte buffer whose content is a shared and aligned
       * subsequence of this buffer's content.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position rounded up to the index of the nearest aligned byte for the
       * given unit size, and end at this buffer's limit rounded down to the index
       * of the nearest aligned byte for the given unit size.
       * If rounding results in out-of-bound values then the new buffer's capacity
       * and limit will be zero.  If rounding is within bounds the following
       * expressions will be true for a new buffer @c nb and unit size
       * @c unitSize:
       * @code
       *     nb.alignmentOffset(0, unitSize) == 0
       *     nb.alignmentOffset(nb.limit(), unitSize) == 0
       * @endcode
       *
       * <p> Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of bytes remaining in this buffer or fewer subject to
       * alignment, its mark will be undefined, and its byte order will be
       * @b BIG_ENDIAN.
       *
       * The new buffer will be direct if, and only if, this buffer is direct, and
       * it will be read-only if, and only if, this buffer is read-only.  </p>
       *
       * @note
       * This method may be utilized to create a new buffer where unit size bytes
       * from index, that is a multiple of the unit size, may be accessed
       * atomically, if supported by the native platform.
       *
       * @details
       * This implementation throws @c UnsupportedOperationException for
       * non-direct buffers when the given unit size is greater then @c 8.
       *
       * @param  unitSize
       *         The unit size in bytes, must be a power of @c 2
       *
       * @return  The new byte buffer
       *
       * @throws IllegalArgumentException
       *         If the unit size not a power of @c 2
       *
       * @throws UnsupportedOperationException
       *         If the native platform does not guarantee stable aligned slices
       *         for the given unit size when managing the memory regions
       *         of buffers of the same kind as this buffer (direct or
       *         non-direct).  For example, if garbage collection would result
       *         in the moving of a memory region covered by a non-direct buffer
       *         from one location to another and both locations have different
       *         alignment characteristics.
       *
       * @see alignmentOffset(int, int)
       * @see slice()
       */
      ByteBuffer& alignedSlice(gint unitSize) const;

      /**
       * Relative <i>get</i> method for reading a char value.
       *
       * <p> Reads the next two bytes at this buffer's current position,
       * composing them into a char value according to the current byte order,
       * and then increments the position by two.  </p>
       *
       * @return  The char value at the buffer's current position
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than two bytes
       *          remaining in this buffer
       */
      virtual gchar getChar() = 0;

      /**
       * Relative <i>put</i> method for writing a char
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes two bytes containing the given char value, in the
       * current byte order, into this buffer at the current position, and then
       * increments the position by two.  </p>
       *
       * @param  value
       *         The char value to be written
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there are fewer than two bytes
       *          remaining in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putChar(gchar value) = 0;

      /**
       * Absolute <i>get</i> method for reading a char value.
       *
       * <p> Reads two bytes at the given index, composing them into a
       * char value according to the current byte order.  </p>
       *
       * @param  index
       *         The index from which the bytes will be read
       *
       * @return  The char value at the given index
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus one
       */
      virtual gchar getChar(gint index) const = 0;

      /**
       * Absolute <i>put</i> method for writing a char
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes two bytes containing the given char value, in the
       * current byte order, into this buffer at the given index.  </p>
       *
       * @param  index
       *         The index at which the bytes will be written
       *
       * @param  value
       *         The char value to be written
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus one
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putChar(gint index, gchar value) = 0;

      /**
       * Creates a view of this byte buffer as a char buffer.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position.  Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of bytes remaining in this buffer divided by
       * two, its mark will be undefined, and its byte order will be that
       * of the byte buffer at the moment the view is created.  The new buffer
       * will be direct if, and only if, this buffer is direct, and it will be
       * read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  A new char buffer
       */
      virtual CharBuffer& asCharBuffer() const = 0;


      /**
       * Relative <i>get</i> method for reading a short value.
       *
       * <p> Reads the next two bytes at this buffer's current position,
       * composing them into a short value according to the current byte order,
       * and then increments the position by two.  </p>
       *
       * @return  The short value at the buffer's current position
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than two bytes
       *          remaining in this buffer
       */
      virtual gchar getShort() = 0;

      /**
       * Relative <i>put</i> method for writing a short
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes two bytes containing the given short value, in the
       * current byte order, into this buffer at the current position, and then
       * increments the position by two.  </p>
       *
       * @param  value
       *         The short value to be written
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there are fewer than two bytes
       *          remaining in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putShort(gshort value) = 0;

      /**
       * Absolute <i>get</i> method for reading a short value.
       *
       * <p> Reads two bytes at the given index, composing them into a
       * short value according to the current byte order.  </p>
       *
       * @param  index
       *         The index from which the bytes will be read
       *
       * @return  The short value at the given index
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus one
       */
      virtual gshort getShort(gint index) const = 0;

      /**
       * Absolute <i>put</i> method for writing a short
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes two bytes containing the given short value, in the
       * current byte order, into this buffer at the given index.  </p>
       *
       * @param  index
       *         The index at which the bytes will be written
       *
       * @param  value
       *         The short value to be written
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus one
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putShort(gint index, gshort value) = 0;

      /**
       * Creates a view of this byte buffer as a short buffer.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position.  Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of bytes remaining in this buffer divided by
       * two, its mark will be undefined, and its byte order will be that
       * of the byte buffer at the moment the view is created.  The new buffer
       * will be direct if, and only if, this buffer is direct, and it will be
       * read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  A new short buffer
       */
      virtual ShortBuffer& asShortBuffer() const = 0;


      /**
       * Relative <i>get</i> method for reading an int value.
       *
       * <p> Reads the next four bytes at this buffer's current position,
       * composing them into an int value according to the current byte order,
       * and then increments the position by four.  </p>
       *
       * @return  The int value at the buffer's current position
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than four bytes
       *          remaining in this buffer
       */
      virtual gchar getInt() = 0;

      /**
       * Relative <i>put</i> method for writing an int
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes four bytes containing the given int value, in the
       * current byte order, into this buffer at the current position, and then
       * increments the position by four.  </p>
       *
       * @param  value
       *         The int value to be written
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there are fewer than four bytes
       *          remaining in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putInt(gint value) = 0;

      /**
       * Absolute <i>get</i> method for reading an int value.
       *
       * <p> Reads four bytes at the given index, composing them into a
       * int value according to the current byte order.  </p>
       *
       * @param  index
       *         The index from which the bytes will be read
       *
       * @return  The int value at the given index
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus three
       */
      virtual gint getInt(gint index) const = 0;

      /**
       * Absolute <i>put</i> method for writing an int
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes four bytes containing the given int value, in the
       * current byte order, into this buffer at the given index.  </p>
       *
       * @param  index
       *         The index at which the bytes will be written
       *
       * @param  value
       *         The int value to be written
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus three
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putInt(gint index, gint value) = 0;

      /**
       * Creates a view of this byte buffer as an int buffer.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position.  Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of bytes remaining in this buffer divided by
       * four, its mark will be undefined, and its byte order will be that
       * of the byte buffer at the moment the view is created.  The new buffer
       * will be direct if, and only if, this buffer is direct, and it will be
       * read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  A new int buffer
       */
      virtual IntBuffer& asIntBuffer() const = 0;


      /**
       * Relative <i>get</i> method for reading a long value.
       *
       * <p> Reads the next eight bytes at this buffer's current position,
       * composing them into a long value according to the current byte order,
       * and then increments the position by eight.  </p>
       *
       * @return  The long value at the buffer's current position
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than eight bytes
       *          remaining in this buffer
       */
      virtual gchar getLong() = 0;

      /**
       * Relative <i>put</i> method for writing a long
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes eight bytes containing the given long value, in the
       * current byte order, into this buffer at the current position, and then
       * increments the position by eight.  </p>
       *
       * @param  value
       *         The long value to be written
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there are fewer than eight bytes
       *          remaining in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putLong(glong value) = 0;

      /**
       * Absolute <i>get</i> method for reading a long value.
       *
       * <p> Reads eight bytes at the given index, composing them into a
       * long value according to the current byte order.  </p>
       *
       * @param  index
       *         The index from which the bytes will be read
       *
       * @return  The long value at the given index
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus seven
       */
      virtual glong getLong(gint index) const = 0;

      /**
       * Absolute <i>put</i> method for writing a long
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes eight bytes containing the given long value, in the
       * current byte order, into this buffer at the given index.  </p>
       *
       * @param  index
       *         The index at which the bytes will be written
       *
       * @param  value
       *         The long value to be written
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus seven
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putLong(gint index, glong value) = 0;

      /**
       * Creates a view of this byte buffer as a long buffer.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position.  Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of bytes remaining in this buffer divided by
       * eight, its mark will be undefined, and its byte order will be that
       * of the byte buffer at the moment the view is created.  The new buffer
       * will be direct if, and only if, this buffer is direct, and it will be
       * read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  A new long buffer
       */
      virtual LongBuffer& asLongBuffer() const = 0;


      /**
       * Relative <i>get</i> method for reading a gfloat value.
       *
       * <p> Reads the next four bytes at this buffer's current position,
       * composing them into a gfloat value according to the current byte order,
       * and then increments the position by four.  </p>
       *
       * @return  The gfloat value at the buffer's current position
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than four bytes
       *          remaining in this buffer
       */
      virtual gfloat getFloat() = 0;

      /**
       * Relative <i>put</i> method for writing a gfloat
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes four bytes containing the given gfloat value, in the
       * current byte order, into this buffer at the current position, and then
       * increments the position by four.  </p>
       *
       * @param  value
       *         The gfloat value to be written
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there are fewer than four bytes
       *          remaining in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putFloat(gfloat value) = 0;

      /**
       * Absolute <i>get</i> method for reading a gfloat value.
       *
       * <p> Reads four bytes at the given index, composing them into a
       * gfloat value according to the current byte order.  </p>
       *
       * @param  index
       *         The index from which the bytes will be read
       *
       * @return  The gfloat value at the given index
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus three
       */
      virtual gfloat getFloat(gint index) const = 0;

      /**
       * Absolute <i>put</i> method for writing a gfloat
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes four bytes containing the given gfloat value, in the
       * current byte order, into this buffer at the given index.  </p>
       *
       * @param  index
       *         The index at which the bytes will be written
       *
       * @param  value
       *         The gfloat value to be written
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus three
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putFloat(gint index, gfloat value) = 0;

      /**
       * Creates a view of this byte buffer as a gfloat buffer.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position.  Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of bytes remaining in this buffer divided by
       * four, its mark will be undefined, and its byte order will be that
       * of the byte buffer at the moment the view is created.  The new buffer
       * will be direct if, and only if, this buffer is direct, and it will be
       * read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  A new gfloat buffer
       */
      virtual FloatBuffer& asFloatBuffer() const = 0;


      /**
       * Relative <i>get</i> method for reading a double value.
       *
       * <p> Reads the next eight bytes at this buffer's current position,
       * composing them into a double value according to the current byte order,
       * and then increments the position by eight.  </p>
       *
       * @return  The double value at the buffer's current position
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than eight bytes
       *          remaining in this buffer
       */
      virtual gdouble getDouble() = 0;

      /**
       * Relative <i>put</i> method for writing a double
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes eight bytes containing the given double value, in the
       * current byte order, into this buffer at the current position, and then
       * increments the position by eight.  </p>
       *
       * @param  value
       *         The double value to be written
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there are fewer than eight bytes
       *          remaining in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putDouble(gdouble value) = 0;

      /**
       * Absolute <i>get</i> method for reading a double value.
       *
       * <p> Reads eight bytes at the given index, composing them into a
       * double value according to the current byte order.  </p>
       *
       * @param  index
       *         The index from which the bytes will be read
       *
       * @return  The double value at the given index
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus seven
       */
      virtual gdouble getDouble(gint index) const = 0;

      /**
       * Absolute <i>put</i> method for writing a double
       * value&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * <p> Writes eight bytes containing the given double value, in the
       * current byte order, into this buffer at the given index.  </p>
       *
       * @param  index
       *         The index at which the bytes will be written
       *
       * @param  value
       *         The double value to be written
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit,
       *          minus seven
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual ByteBuffer& putDouble(gint index, gdouble value) = 0;

      /**
       * Creates a view of this byte buffer as a double buffer.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position.  Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of bytes remaining in this buffer divided by
       * eight, its mark will be undefined, and its byte order will be that
       * of the byte buffer at the moment the view is created.  The new buffer
       * will be direct if, and only if, this buffer is direct, and it will be
       * read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  A new double buffer
       */
      virtual DoubleBuffer& asDoubleBuffer() const = 0;

    private:
      gbool bigEndian = false;
      gbool nativeByteOrder = charset::Charset::ByteOrder::NATIVE_ENDIAN ==
          charset::Charset::ByteOrder::BIG_ENDIAN;

      Object& base() const override;

      void getArray(gint index, ByteArray& dst, gint offset, gint length) const;

      virtual ByteBuffer& putArray(gint index, ByteArray const& src, gint offset, gint length);

      virtual void putBuffer(gint pos, ByteBuffer& src, gint srcPos, gint n);

      static ByteArray& createHeap(gint heapSize);

      class HeapByteBuffer;
      class ReadOnlyHeapByteBuffer;

      class CharBufferBE;
      class CharBufferLE;
      class ReadOnlyCharBufferBE;
      class ReadOnlyCharBufferLE;

      class ShortBufferBE;
      class ShortBufferLE;
      class ReadOnlyShortBufferBE;
      class ReadOnlyShortBufferLE;

      class IntBufferBE;
      class IntBufferLE;
      class ReadOnlyIntBufferBE;
      class ReadOnlyIntBufferLE;

      class LongBufferBE;
      class LongBufferLE;
      class ReadOnlyLongBufferBE;
      class ReadOnlyLongBufferLE;

      class FloatBufferBE;
      class FloatBufferLE;
      class ReadOnlyFloatBufferBE;
      class ReadOnlyFloatBufferLE;

      class DoubleBufferBE;
      class DoubleBufferLE;
      class ReadOnlyDoubleBufferBE;
      class ReadOnlyDoubleBufferLE;

      CORE_ADD_AS_FRIEND(Buffer);
      CORE_ADD_AS_FRIEND(charset::CharsetDecoder);
      CORE_ADD_AS_FRIEND(charset::CharsetEncoder);
    };

    /**
     * A read/write HeapByteBuffer.
     */
    class ByteBuffer::HeapByteBuffer : public ByteBuffer {
      ByteArray& hb;
      gint offset = 0;

      CORE_ADD_AS_FRIEND(ReadOnlyHeapByteBuffer);
      CORE_ADD_AS_FRIEND(CharBufferLE);
      CORE_ADD_AS_FRIEND(CharBufferBE);
      CORE_ADD_AS_FRIEND(ShortBufferLE);
      CORE_ADD_AS_FRIEND(ShortBufferBE);
      CORE_ADD_AS_FRIEND(IntBufferLE);
      CORE_ADD_AS_FRIEND(IntBufferBE);
      CORE_ADD_AS_FRIEND(LongBufferLE);
      CORE_ADD_AS_FRIEND(LongBufferBE);
      CORE_ADD_AS_FRIEND(FloatBufferLE);
      CORE_ADD_AS_FRIEND(FloatBufferBE);
      CORE_ADD_AS_FRIEND(DoubleBufferLE);
      CORE_ADD_AS_FRIEND(DoubleBufferBE);

    public:
      CORE_EXPLICIT HeapByteBuffer(gint cap, gint lim, Object& segment);

      CORE_EXPLICIT HeapByteBuffer(ByteArray& buf, gint off, gint len, Object& segment);

      CORE_EXPLICIT HeapByteBuffer(ByteArray& buf,
                                   gint mark, gint pos, gint lim, gint cap, gint off, Object& segment);

      ByteBuffer& slice() const override;

      ByteBuffer& slice(gint index, gint length) const override;

      ByteBuffer& duplicate() const override;

      ByteBuffer& asReadOnlyBuffer() const override;

      gbyte get() override;

      gbyte get(gint index) const override;

      ByteBuffer& get(ByteArray& dst, gint offset, gint length) override;

      ByteBuffer& get(gint index, ByteArray& dst, gint offset, gint length) override;

      gbool isReadOnly() const override;

      gbool isDirect() const override;

      ByteBuffer& put(gbyte c) override;

      ByteBuffer& put(gint index, gbyte c) override;

      ByteBuffer& put(const ByteArray& src, gint offset, gint length) override;

      ByteBuffer& put(gint index, const ByteArray& src, gint offset, gint length) override;

      ByteBuffer& put(gint index, const ByteArray& src) override;

      gbool hasArray() const override;

      ByteArray& array() const override;

      gint arrayOffset() const override;

      String toString() const override;

      ByteBuffer& compact() override;

      gchar getChar() override;

      ByteBuffer& putChar(gchar value) override;

      gchar getChar(gint index) const override;

      ByteBuffer& putChar(gint index, gchar value) override;

      CharBuffer& asCharBuffer() const override;

      gchar getShort() override;

      ByteBuffer& putShort(gshort value) override;

      gshort getShort(gint index) const override;

      ByteBuffer& putShort(gint index, gshort value) override;

      ShortBuffer& asShortBuffer() const override;

      gchar getInt() override;

      ByteBuffer& putInt(gint value) override;

      gint getInt(gint index) const override;

      ByteBuffer& putInt(gint index, gint value) override;

      IntBuffer& asIntBuffer() const override;

      gchar getLong() override;

      ByteBuffer& putLong(glong value) override;

      glong getLong(gint index) const override;

      ByteBuffer& putLong(gint index, glong value) override;

      LongBuffer& asLongBuffer() const override;

      gfloat getFloat() override;

      ByteBuffer& putFloat(gfloat value) override;

      gfloat getFloat(gint index) const override;

      ByteBuffer& putFloat(gint index, gfloat value) override;

      FloatBuffer& asFloatBuffer() const override;

      gdouble getDouble() override;

      ByteBuffer& putDouble(gdouble value) override;

      gdouble getDouble(gint index) const override;

      ByteBuffer& putDouble(gint index, gdouble value) override;

      DoubleBuffer& asDoubleBuffer() const override;

    private:
      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;

      virtual gbyte getFast(gint i) const;

      virtual void putFast(gint i, gbyte b);

      Object& base() const final;
    };

    /**
     * A read-only HeapByteBuffer.  This class extends the corresponding
     * read/write class, overriding the mutation methods to throw a
     * @b ReadOnlyBufferException and overriding the view-buffer methods to return an
     * instance of this class rather than of the superclass.
     */
    class ByteBuffer::ReadOnlyHeapByteBuffer final : public HeapByteBuffer {
    public:
      CORE_EXPLICIT ReadOnlyHeapByteBuffer(gint cap, gint lim, Object& segment);

      CORE_EXPLICIT ReadOnlyHeapByteBuffer(ByteArray& buf, gint off, gint len, Object& segment);

      CORE_EXPLICIT ReadOnlyHeapByteBuffer(ByteArray& buf,
                                           gint mark, gint pos, gint lim, gint cap, gint off, Object& segment);

      ByteBuffer& slice() const override;

      ByteBuffer& slice(gint index, gint length) const override;

      ByteBuffer& duplicate() const override;

      ByteBuffer& asReadOnlyBuffer() const override;

      gbool isReadOnly() const override;

      ByteBuffer& put(gbyte c) override;

      ByteBuffer& put(gint index, gbyte c) override;

      ByteBuffer& put(const ByteArray& src, gint offset, gint length) override;

      ByteBuffer& put(const ByteArray& src) override;

      ByteBuffer& put(gint index, const ByteArray& src, gint offset, gint length) override;

      ByteBuffer& put(gint index, const ByteArray& src) override;

      ByteBuffer& put(ByteBuffer& src) override;

      ByteBuffer& put(gint index, ByteBuffer& src, gint offset, gint length) override;

      ByteBuffer& compact() override;

      String toString() const override;

      gbool hasArray() const override;

      ByteArray& array() const override;

      gint arrayOffset() const override;

      ByteBuffer& putChar(gchar value) override;

      ByteBuffer& putChar(gint index, gchar value) override;

      CharBuffer& asCharBuffer() const override;

      ByteBuffer& putShort(gshort value) override;

      ByteBuffer& putShort(gint index, gshort value) override;

      ShortBuffer& asShortBuffer() const override;

      ByteBuffer& putInt(gint value) override;

      ByteBuffer& putInt(gint index, gint value) override;

      IntBuffer& asIntBuffer() const override;

      ByteBuffer& putLong(glong value) override;

      ByteBuffer& putLong(gint index, glong value) override;

      LongBuffer& asLongBuffer() const override;

      ByteBuffer& putFloat(gfloat value) override;

      ByteBuffer& putFloat(gint index, gfloat value) override;

      FloatBuffer& asFloatBuffer() const override;

      ByteBuffer& putDouble(gdouble value) override;

      ByteBuffer& putDouble(gint index, gdouble value) override;

      DoubleBuffer& asDoubleBuffer() const override;

    private:
      gbyte getFast(gint i) const override;

      void putFast(gint i, gbyte b) override;
    };

    class ByteBuffer::CharBufferLE : public CharBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyCharBufferLE);

    public:
      CORE_EXPLICIT CharBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT CharBufferLE(ByteBuffer& bb,
                                 gint mark, gint pos, gint lim, gint cap,
                                 glong addr, Object& segment);

      CharBuffer& slice() const override;

      CharBuffer& slice(gint index, gint length) const override;

      CharBuffer& duplicate() const override;

      CharBuffer& asReadOnlyBuffer() const override;

      gchar get() override;

      gchar get(gint index) const override;

      CharBuffer& put(gchar c) override;

      CharBuffer& put(gint index, gchar c) override;

      CharBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      CharBuffer& subSequence(gint start, gint end) const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      CharArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;

      gchar getFast(gint i) const override;

      gint charOrder() const override;
    };

    class ByteBuffer::CharBufferBE : public CharBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyCharBufferBE);

    public:
      CORE_EXPLICIT CharBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT CharBufferBE(ByteBuffer& bb,
                                 gint mark, gint pos, gint lim, gint cap,
                                 glong addr, Object& segment);

      CharBuffer& slice() const override;

      CharBuffer& slice(gint index, gint length) const override;

      CharBuffer& duplicate() const override;

      CharBuffer& asReadOnlyBuffer() const override;

      gchar get() override;

      gchar get(gint index) const override;

      CharBuffer& put(gchar c) override;

      CharBuffer& put(gint index, gchar c) override;

      CharBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      CharBuffer& subSequence(gint start, gint end) const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      CharArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;

      gchar getFast(gint i) const override;

      gint charOrder() const override;
    };

    class ByteBuffer::ReadOnlyCharBufferLE final : public CharBufferLE {
    public:
      CORE_EXPLICIT ReadOnlyCharBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyCharBufferLE(ByteBuffer& bb,
                                         gint mark, gint pos, gint lim, gint cap,
                                         glong addr, Object& segment);

      CharBuffer& slice() const override;

      CharBuffer& slice(gint index, gint length) const override;

      CharBuffer& duplicate() const override;

      CharBuffer& asReadOnlyBuffer() const override;

      CharBuffer& put(gchar c) override;

      CharBuffer& put(gint index, gchar c) override;

      CharBuffer& put(CharBuffer& src) override;

      CharBuffer& put(gint index, CharBuffer& src, gint offset, gint length) override;

      CharBuffer& put(const CharArray& src, gint offset, gint length) override;

      CharBuffer& put(const CharArray& src) override;

      CharBuffer& put(gint index, const CharArray& src, gint offset, gint length) override;

      CharBuffer& put(gint index, const CharArray& src) override;

      CharBuffer& put(const String& src, gint start, gint end) override;

      CharBuffer& put(const String& src) override;

      CharBuffer& compact() override;

      gbool isReadOnly() const override;

      CharBuffer& subSequence(gint start, gint end) const override;
    };

    class ByteBuffer::ReadOnlyCharBufferBE final : public CharBufferBE {
    public:
      CORE_EXPLICIT ReadOnlyCharBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyCharBufferBE(ByteBuffer& bb,
                                         gint mark, gint pos, gint lim, gint cap,
                                         glong addr, Object& segment);

      CharBuffer& slice() const override;

      CharBuffer& slice(gint index, gint length) const override;

      CharBuffer& duplicate() const override;

      CharBuffer& asReadOnlyBuffer() const override;

      CharBuffer& put(gchar c) override;

      CharBuffer& put(gint index, gchar c) override;

      CharBuffer& put(CharBuffer& src) override;

      CharBuffer& put(gint index, CharBuffer& src, gint offset, gint length) override;

      CharBuffer& put(const CharArray& src, gint offset, gint length) override;

      CharBuffer& put(const CharArray& src) override;

      CharBuffer& put(gint index, const CharArray& src, gint offset, gint length) override;

      CharBuffer& put(gint index, const CharArray& src) override;

      CharBuffer& put(const String& src, gint start, gint end) override;

      CharBuffer& put(const String& src) override;

      CharBuffer& compact() override;

      gbool isReadOnly() const override;

      CharBuffer& subSequence(gint start, gint end) const override;
    };

    class ByteBuffer::ShortBufferLE : public ShortBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyShortBufferLE);

    public:
      CORE_EXPLICIT ShortBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ShortBufferLE(ByteBuffer& bb,
                                  gint mark, gint pos, gint lim, gint cap,
                                  glong addr, Object& segment);

      ShortBuffer& slice() const override;

      ShortBuffer& slice(gint index, gint length) const override;

      ShortBuffer& duplicate() const override;

      ShortBuffer& asReadOnlyBuffer() const override;

      gshort get() override;

      gshort get(gint index) const override;

      ShortBuffer& put(gshort c) override;

      ShortBuffer& put(gint index, gshort c) override;

      ShortBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      ShortArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::ShortBufferBE : public ShortBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyShortBufferBE);

    public:
      CORE_EXPLICIT ShortBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ShortBufferBE(ByteBuffer& bb,
                                  gint mark, gint pos, gint lim, gint cap,
                                  glong addr, Object& segment);

      ShortBuffer& slice() const override;

      ShortBuffer& slice(gint index, gint length) const override;

      ShortBuffer& duplicate() const override;

      ShortBuffer& asReadOnlyBuffer() const override;

      gshort get() override;

      gshort get(gint index) const override;

      ShortBuffer& put(gshort c) override;

      ShortBuffer& put(gint index, gshort c) override;

      ShortBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      ShortArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::ReadOnlyShortBufferLE final : public ShortBufferLE {
    public:
      CORE_EXPLICIT ReadOnlyShortBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyShortBufferLE(ByteBuffer& bb,
                                          gint mark, gint pos, gint lim, gint cap,
                                          glong addr, Object& segment);

      ShortBuffer& slice() const override;

      ShortBuffer& slice(gint index, gint length) const override;

      ShortBuffer& duplicate() const override;

      ShortBuffer& asReadOnlyBuffer() const override;

      ShortBuffer& put(gshort c) override;

      ShortBuffer& put(gint index, gshort c) override;

      ShortBuffer& put(ShortBuffer& src) override;

      ShortBuffer& put(gint index, ShortBuffer& src, gint offset, gint length) override;

      ShortBuffer& put(const ShortArray& src, gint offset, gint length) override;

      ShortBuffer& put(const ShortArray& src) override;

      ShortBuffer& put(gint index, const ShortArray& src, gint offset, gint length) override;

      ShortBuffer& put(gint index, const ShortArray& src) override;

      ShortBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::ReadOnlyShortBufferBE final : public ShortBufferBE {
    public:
      CORE_EXPLICIT ReadOnlyShortBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyShortBufferBE(ByteBuffer& bb,
                                          gint mark, gint pos, gint lim, gint cap,
                                          glong addr, Object& segment);

      ShortBuffer& slice() const override;

      ShortBuffer& slice(gint index, gint length) const override;

      ShortBuffer& duplicate() const override;

      ShortBuffer& asReadOnlyBuffer() const override;

      ShortBuffer& put(gshort c) override;

      ShortBuffer& put(gint index, gshort c) override;

      ShortBuffer& put(ShortBuffer& src) override;

      ShortBuffer& put(gint index, ShortBuffer& src, gint offset, gint length) override;

      ShortBuffer& put(const ShortArray& src, gint offset, gint length) override;

      ShortBuffer& put(const ShortArray& src) override;

      ShortBuffer& put(gint index, const ShortArray& src, gint offset, gint length) override;

      ShortBuffer& put(gint index, const ShortArray& src) override;

      ShortBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::IntBufferLE : public IntBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyIntBufferLE);

    public:
      CORE_EXPLICIT IntBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT IntBufferLE(ByteBuffer& bb,
                                gint mark, gint pos, gint lim, gint cap,
                                glong addr, Object& segment);

      IntBuffer& slice() const override;

      IntBuffer& slice(gint index, gint length) const override;

      IntBuffer& duplicate() const override;

      IntBuffer& asReadOnlyBuffer() const override;

      gint get() override;

      gint get(gint index) const override;

      IntBuffer& put(gint c) override;

      IntBuffer& put(gint index, gint c) override;

      IntBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      IntArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::IntBufferBE : public IntBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyIntBufferBE);

    public:
      CORE_EXPLICIT IntBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT IntBufferBE(ByteBuffer& bb,
                                gint mark, gint pos, gint lim, gint cap,
                                glong addr, Object& segment);

      IntBuffer& slice() const override;

      IntBuffer& slice(gint index, gint length) const override;

      IntBuffer& duplicate() const override;

      IntBuffer& asReadOnlyBuffer() const override;

      gint get() override;

      gint get(gint index) const override;

      IntBuffer& put(gint c) override;

      IntBuffer& put(gint index, gint c) override;

      IntBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      IntArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::ReadOnlyIntBufferLE final : public IntBufferLE {
    public:
      CORE_EXPLICIT ReadOnlyIntBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyIntBufferLE(ByteBuffer& bb,
                                        gint mark, gint pos, gint lim, gint cap,
                                        glong addr, Object& segment);

      IntBuffer& slice() const override;

      IntBuffer& slice(gint index, gint length) const override;

      IntBuffer& duplicate() const override;

      IntBuffer& asReadOnlyBuffer() const override;

      IntBuffer& put(gint c) override;

      IntBuffer& put(gint index, gint c) override;

      IntBuffer& put(IntBuffer& src) override;

      IntBuffer& put(gint index, IntBuffer& src, gint offset, gint length) override;

      IntBuffer& put(const IntArray& src, gint offset, gint length) override;

      IntBuffer& put(const IntArray& src) override;

      IntBuffer& put(gint index, const IntArray& src, gint offset, gint length) override;

      IntBuffer& put(gint index, const IntArray& src) override;

      IntBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::ReadOnlyIntBufferBE final : public IntBufferBE {
    public:
      CORE_EXPLICIT ReadOnlyIntBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyIntBufferBE(ByteBuffer& bb,
                                        gint mark, gint pos, gint lim, gint cap,
                                        glong addr, Object& segment);

      IntBuffer& slice() const override;

      IntBuffer& slice(gint index, gint length) const override;

      IntBuffer& duplicate() const override;

      IntBuffer& asReadOnlyBuffer() const override;

      IntBuffer& put(gint c) override;

      IntBuffer& put(gint index, gint c) override;

      IntBuffer& put(IntBuffer& src) override;

      IntBuffer& put(gint index, IntBuffer& src, gint offset, gint length) override;

      IntBuffer& put(const IntArray& src, gint offset, gint length) override;

      IntBuffer& put(const IntArray& src) override;

      IntBuffer& put(gint index, const IntArray& src, gint offset, gint length) override;

      IntBuffer& put(gint index, const IntArray& src) override;

      IntBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::LongBufferLE : public LongBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyLongBufferLE);

    public:
      CORE_EXPLICIT LongBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT LongBufferLE(ByteBuffer& bb,
                                 gint mark, gint pos, gint lim, gint cap,
                                 glong addr, Object& segment);

      LongBuffer& slice() const override;

      LongBuffer& slice(gint index, gint length) const override;

      LongBuffer& duplicate() const override;

      LongBuffer& asReadOnlyBuffer() const override;

      glong get() override;

      glong get(gint index) const override;

      LongBuffer& put(glong c) override;

      LongBuffer& put(gint index, glong c) override;

      LongBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      LongArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::LongBufferBE : public LongBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyLongBufferBE);

    public:
      CORE_EXPLICIT LongBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT LongBufferBE(ByteBuffer& bb,
                                 gint mark, gint pos, gint lim, gint cap,
                                 glong addr, Object& segment);

      LongBuffer& slice() const override;

      LongBuffer& slice(gint index, gint length) const override;

      LongBuffer& duplicate() const override;

      LongBuffer& asReadOnlyBuffer() const override;

      glong get() override;

      glong get(gint index) const override;

      LongBuffer& put(glong c) override;

      LongBuffer& put(gint index, glong c) override;

      LongBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      LongArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::ReadOnlyLongBufferLE final : public LongBufferLE {
    public:
      CORE_EXPLICIT ReadOnlyLongBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyLongBufferLE(ByteBuffer& bb,
                                         gint mark, gint pos, gint lim, gint cap,
                                         glong addr, Object& segment);

      LongBuffer& slice() const override;

      LongBuffer& slice(gint index, gint length) const override;

      LongBuffer& duplicate() const override;

      LongBuffer& asReadOnlyBuffer() const override;

      LongBuffer& put(glong c) override;

      LongBuffer& put(gint index, glong c) override;

      LongBuffer& put(LongBuffer& src) override;

      LongBuffer& put(gint index, LongBuffer& src, gint offset, gint length) override;

      LongBuffer& put(const LongArray& src, gint offset, gint length) override;

      LongBuffer& put(const LongArray& src) override;

      LongBuffer& put(gint index, const LongArray& src, gint offset, gint length) override;

      LongBuffer& put(gint index, const LongArray& src) override;

      LongBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::ReadOnlyLongBufferBE final : public LongBufferBE {
    public:
      CORE_EXPLICIT ReadOnlyLongBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyLongBufferBE(ByteBuffer& bb,
                                         gint mark, gint pos, gint lim, gint cap,
                                         glong addr, Object& segment);

      LongBuffer& slice() const override;

      LongBuffer& slice(gint index, gint length) const override;

      LongBuffer& duplicate() const override;

      LongBuffer& asReadOnlyBuffer() const override;

      LongBuffer& put(glong c) override;

      LongBuffer& put(gint index, glong c) override;

      LongBuffer& put(LongBuffer& src) override;

      LongBuffer& put(gint index, LongBuffer& src, gint offset, gint length) override;

      LongBuffer& put(const LongArray& src, gint offset, gint length) override;

      LongBuffer& put(const LongArray& src) override;

      LongBuffer& put(gint index, const LongArray& src, gint offset, gint length) override;

      LongBuffer& put(gint index, const LongArray& src) override;

      LongBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::FloatBufferLE : public FloatBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyFloatBufferLE);

    public:
      CORE_EXPLICIT FloatBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT FloatBufferLE(ByteBuffer& bb,
                                  gint mark, gint pos, gint lim, gint cap,
                                  glong addr, Object& segment);

      FloatBuffer& slice() const override;

      FloatBuffer& slice(gint index, gint length) const override;

      FloatBuffer& duplicate() const override;

      FloatBuffer& asReadOnlyBuffer() const override;

      gfloat get() override;

      gfloat get(gint index) const override;

      FloatBuffer& put(gfloat c) override;

      FloatBuffer& put(gint index, gfloat c) override;

      FloatBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      FloatArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::FloatBufferBE : public FloatBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyFloatBufferBE);

    public:
      CORE_EXPLICIT FloatBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT FloatBufferBE(ByteBuffer& bb,
                                  gint mark, gint pos, gint lim, gint cap,
                                  glong addr, Object& segment);

      FloatBuffer& slice() const override;

      FloatBuffer& slice(gint index, gint length) const override;

      FloatBuffer& duplicate() const override;

      FloatBuffer& asReadOnlyBuffer() const override;

      gfloat get() override;

      gfloat get(gint index) const override;

      FloatBuffer& put(gfloat c) override;

      FloatBuffer& put(gint index, gfloat c) override;

      FloatBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      FloatArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::ReadOnlyFloatBufferLE final : public FloatBufferLE {
    public:
      CORE_EXPLICIT ReadOnlyFloatBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyFloatBufferLE(ByteBuffer& bb,
                                          gint mark, gint pos, gint lim, gint cap,
                                          glong addr, Object& segment);

      FloatBuffer& slice() const override;

      FloatBuffer& slice(gint index, gint length) const override;

      FloatBuffer& duplicate() const override;

      FloatBuffer& asReadOnlyBuffer() const override;

      FloatBuffer& put(gfloat c) override;

      FloatBuffer& put(gint index, gfloat c) override;

      FloatBuffer& put(FloatBuffer& src) override;

      FloatBuffer& put(gint index, FloatBuffer& src, gint offset, gint length) override;

      FloatBuffer& put(const FloatArray& src, gint offset, gint length) override;

      FloatBuffer& put(const FloatArray& src) override;

      FloatBuffer& put(gint index, const FloatArray& src, gint offset, gint length) override;

      FloatBuffer& put(gint index, const FloatArray& src) override;

      FloatBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::ReadOnlyFloatBufferBE final : public FloatBufferBE {
    public:
      CORE_EXPLICIT ReadOnlyFloatBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyFloatBufferBE(ByteBuffer& bb,
                                          gint mark, gint pos, gint lim, gint cap,
                                          glong addr, Object& segment);

      FloatBuffer& slice() const override;

      FloatBuffer& slice(gint index, gint length) const override;

      FloatBuffer& duplicate() const override;

      FloatBuffer& asReadOnlyBuffer() const override;

      FloatBuffer& put(gfloat c) override;

      FloatBuffer& put(gint index, gfloat c) override;

      FloatBuffer& put(FloatBuffer& src) override;

      FloatBuffer& put(gint index, FloatBuffer& src, gint offset, gint length) override;

      FloatBuffer& put(const FloatArray& src, gint offset, gint length) override;

      FloatBuffer& put(const FloatArray& src) override;

      FloatBuffer& put(gint index, const FloatArray& src, gint offset, gint length) override;

      FloatBuffer& put(gint index, const FloatArray& src) override;

      FloatBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::DoubleBufferLE : public DoubleBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyDoubleBufferLE);

    public:
      CORE_EXPLICIT DoubleBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT DoubleBufferLE(ByteBuffer& bb,
                                   gint mark, gint pos, gint lim, gint cap,
                                   glong addr, Object& segment);

      DoubleBuffer& slice() const override;

      DoubleBuffer& slice(gint index, gint length) const override;

      DoubleBuffer& duplicate() const override;

      DoubleBuffer& asReadOnlyBuffer() const override;

      gdouble get() override;

      gdouble get(gint index) const override;

      DoubleBuffer& put(gdouble c) override;

      DoubleBuffer& put(gint index, gdouble c) override;

      DoubleBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      DoubleArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::DoubleBufferBE : public DoubleBuffer {
      ByteBuffer& bb;

      CORE_ADD_AS_FRIEND(ReadOnlyDoubleBufferBE);

    public:
      CORE_EXPLICIT DoubleBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT DoubleBufferBE(ByteBuffer& bb,
                                   gint mark, gint pos, gint lim, gint cap,
                                   glong addr, Object& segment);

      DoubleBuffer& slice() const override;

      DoubleBuffer& slice(gint index, gint length) const override;

      DoubleBuffer& duplicate() const override;

      DoubleBuffer& asReadOnlyBuffer() const override;

      gdouble get() override;

      gdouble get(gint index) const override;

      DoubleBuffer& put(gdouble c) override;

      DoubleBuffer& put(gint index, gdouble c) override;

      DoubleBuffer& compact() override;

      gbool isDirect() const override;

      gbool isReadOnly() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

      gbool hasArray() const override;

      DoubleArray& array() const override;

      gint arrayOffset() const override;

    private:
      Object& base() const override;

      virtual gint ix(gint i) const;

      virtual glong byteOffset(glong i) const;
    };

    class ByteBuffer::ReadOnlyDoubleBufferLE final : public DoubleBufferLE {
    public:
      CORE_EXPLICIT ReadOnlyDoubleBufferLE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyDoubleBufferLE(ByteBuffer& bb,
                                           gint mark, gint pos, gint lim, gint cap,
                                           glong addr, Object& segment);

      DoubleBuffer& slice() const override;

      DoubleBuffer& slice(gint index, gint length) const override;

      DoubleBuffer& duplicate() const override;

      DoubleBuffer& asReadOnlyBuffer() const override;

      DoubleBuffer& put(gdouble c) override;

      DoubleBuffer& put(gint index, gdouble c) override;

      DoubleBuffer& put(DoubleBuffer& src) override;

      DoubleBuffer& put(gint index, DoubleBuffer& src, gint offset, gint length) override;

      DoubleBuffer& put(const DoubleArray& src, gint offset, gint length) override;

      DoubleBuffer& put(const DoubleArray& src) override;

      DoubleBuffer& put(gint index, const DoubleArray& src, gint offset, gint length) override;

      DoubleBuffer& put(gint index, const DoubleArray& src) override;

      DoubleBuffer& compact() override;

      gbool isReadOnly() const override;
    };

    class ByteBuffer::ReadOnlyDoubleBufferBE final : public DoubleBufferBE {
    public:
      CORE_EXPLICIT ReadOnlyDoubleBufferBE(ByteBuffer& bb, Object& segment);

      CORE_EXPLICIT ReadOnlyDoubleBufferBE(ByteBuffer& bb,
                                           gint mark, gint pos, gint lim, gint cap,
                                           glong addr, Object& segment);

      DoubleBuffer& slice() const override;

      DoubleBuffer& slice(gint index, gint length) const override;

      DoubleBuffer& duplicate() const override;

      DoubleBuffer& asReadOnlyBuffer() const override;

      DoubleBuffer& put(gdouble c) override;

      DoubleBuffer& put(gint index, gdouble c) override;

      DoubleBuffer& put(DoubleBuffer& src) override;

      DoubleBuffer& put(gint index, DoubleBuffer& src, gint offset, gint length) override;

      DoubleBuffer& put(const DoubleArray& src, gint offset, gint length) override;

      DoubleBuffer& put(const DoubleArray& src) override;

      DoubleBuffer& put(gint index, const DoubleArray& src, gint offset, gint length) override;

      DoubleBuffer& put(gint index, const DoubleArray& src) override;

      DoubleBuffer& compact() override;

      gbool isReadOnly() const override;
    };
  } // io
} // core

#endif //CORE24_BYTEBUFFER_H
