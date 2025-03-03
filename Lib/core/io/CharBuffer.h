//
// Created by brunshweeck on 27 août 2024.
//

#ifndef CORE24_CHARBUFFER_H
#define CORE24_CHARBUFFER_H

#include <core/lang/Appendable.h>
#include <core/charset/Charset.h>
#include <core/io/Buffer.h>

namespace core {
  namespace io {
    /**
     * A char buffer.
     *
     * <p> This class defines four categories of operations upon
     * char buffers: <br/>
     *
     *  - Absolute and relative <i>get</i> and <i>put</i> methods that read and write
     *   single chars <br/>
     *
     *  - Absolute and relative <i>bulk get</i>
     *   methods that transfer contiguous sequences of chars from this buffer
     *   into an arra <br/>
     *
     *  - Absolute and relative <i>bulk put</i> methods that transfer contiguous sequences of chars from a
     *   char array, a string, or some other char buffer into this buffe <br/>
     *
     *  - A method for @b compacting a char buffer. <br/>
     *
     * <p> Char buffers can be created either by
     * <i>allocation</i>, which allocates space for the buffer's
     *
     * content, by <i>wrapping</i> an existing
     * char array or string into a buffer, or by creating a
     * <a href="ByteBuffer.html#views"><i>view</i></a> of an existing byte buffer.
     *
     *
     * <p> Like a byte buffer, a char buffer is either
     * <a href="ByteBuffer.html#direct"><i>direct</i> or <i>non-direct</i></a>.  A
     * char buffer created via the @c wrap methods of this class will
     * be non-direct.  A char buffer created as a view of a byte buffer will
     * be direct if, and only if, the byte buffer itself is direct.  Whether
     * a char buffer is direct may be determined by invoking the
     * @b isDirect method.  </p>
     *
     * <p> This class implements the @b CharSequence interface so that
     * character buffers may be used wherever character sequences are accepted, for
     * example in the regular-expression namespace @b core::regex.
     * The methods defined by @c CharSequence operate relative to the current
     * position of the buffer when they are invoked.
     * </p>
     *
     * <p> Methods in this class that do not otherwise have a value to return are
     * specified to return the buffer upon which they are invoked.  This allows
     * method invocations to be chained.
     *
     * The sequence of statements
     *
     * @code
     *     cb.put("text/"_S);
     *     cb.put(subtype);
     *     cb.put("; charset="_S);
     *     cb.put(enc);
     * @endcode
     *
     * can, for example, be replaced by the single statement
     *
     * @code
     *     cb.put("text/"_S).put(subtype).put("; charset="_S).put(enc);
     * @endcode
     *
     * <h2> Optional operations </h2>
     * Methods specified as
     * <i>optional operations</i> throw a @em ReadOnlyBufferException when invoked
     * on a @em read-only CharBuffer. The methods @em array and @em arrayOffset
     * throw an @em UnsupportedOperationException if the CharBuffer is
     * not backed by an <em>accessible char array </em>
     * (irrespective of whether the CharBuffer is read-only).
     *
     */
    class CharBuffer : public virtual Buffer,
                       public virtual Comparable<CharBuffer>,
                       public virtual Appendable,
                       public virtual CharSequence {
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
      CORE_EXPLICIT CharBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment);

    public:
      /**
       * Allocates a new char buffer.
       *
       * <p> The new buffer's position will be zero, its limit will be its
       * capacity, its mark will be undefined, each of its elements will be
       * initialized to zero, and its byte order will be
       * the <b> native order</b> of the underlying hardware.
       * It will have a <b> backing array</b>, and its
       * <b> array offset</b> will be zero.
       *
       * @param  capacity
       *         The new buffer's capacity, in chars
       *
       * @return  The new char buffer
       *
       * @throws  IllegalArgumentException
       *          If the @c capacity is a negative integer
       */
      static CharBuffer& allocate(gint capacity);

      /**
       * Wraps a char array into a buffer.
       *
       * <p> The new buffer will be backed by the given char array;
       * that is, modifications to the buffer will cause the array to be modified
       * and vice versa.  The new buffer's capacity will be
       * @c array.length(), its position will be @c offset, its limit
       * will be @code offset + length @endcode , its mark will be undefined, and its
       * byte order will be the <b> native order</b> of the underlying hardware.

       * Its <b> backing array</b> will be the given array, and
       * its <b> array offset</b> will be zero.  </p>
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
       *         The new buffer's limit will be set to @code offset + length @endcode .
       *
       * @return  The new char buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c offset and @c length
       *          parameters do not hold
       */
      static CharBuffer& wrap(CharArray& array, gint offset, gint length);

      /**
       * Wraps a char array into a buffer.
       *
       * <p> The new buffer will be backed by the given char array;
       * that is, modifications to the buffer will cause the array to be modified
       * and vice versa.  The new buffer's capacity and limit will be
       * @c array.length(), its position will be zero, its mark will be
       * undefined, and its byte order will be the <b> native order</b> of the underlying
       * hardware.

       * Its <b>backing array</b> will be the given array, and its
       * <b> array offset</b> will be zero.  </p>
       *
       * @param  array
       *         The array that will back this buffer
       *
       * @return  The new char buffer
       */
      static CharBuffer& wrap(CharArray& array);


      /**
       * Attempts to read characters into the specified character buffer.
       * The buffer is used as a repository of characters as-is: the only
       * changes made are the results of a put operation. No flipping or
       * rewinding of the buffer is performed. If the
       * @em length of the specified character buffer is zero, then no characters
       * will be read and zero will be returned.
       *
       * @param target the buffer to read characters into
       * @return The number of characters added to the buffer,
       *         possibly zero, or -1 if this source of characters is at its end
       * @throws IOException if an I/O error occurs
       * @throws ReadOnlyBufferException if target is a read only buffer,
       *         even if its length is zero
       */
      gint read(CharBuffer& target);

      /**
       * Wraps a character sequence into a buffer.
       *
       * <p> The content of the new, read-only buffer will be the content of the
       * given character sequence.  The buffer's capacity will be
       * @c csq.length(), its position will be @c start, its limit
       * will be @c end, and its mark will be undefined.  </p>
       *
       * @param  csq
       *         The character sequence from which the new character buffer is to
       *         be created
       *
       * @param  start
       *         The index of the first character to be used;
       *         must be non-negative and no larger than @c csq.length().
       *         The new buffer's position will be set to this value.
       *
       * @param  end
       *         The index of the character following the last character to be
       *         used; must be no smaller than @c start and no larger
       *         than @c csq.length().
       *         The new buffer's limit will be set to this value.
       *
       * @return  The new character buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c start and @c end
       *          parameters do not hold
       */
      static CharBuffer& wrap(CharSequence const& csq, gint start, gint end);

      /**
       * Wraps a character sequence into a buffer.
       *
       * <p> The content of the new, read-only buffer will be the content of the
       * given character sequence.  The new buffer's capacity and limit will be
       * @c csq.length(), its position will be zero, and its mark will be
       * undefined.  </p>
       *
       * @param  csq
       *         The character sequence from which the new character buffer is to
       *         be created
       *
       * @return  The new character buffer
       */
      static CharBuffer& wrap(CharSequence const& csq);


      /**
       * Creates a new char buffer whose content is a shared subsequence of
       * this buffer's content.
       *
       * <p> The content of the new buffer will start at this buffer's current
       * position.  Changes to this buffer's content will be visible in the new
       * buffer, and vice versa; the two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's position will be zero, its capacity and its limit
       * will be the number of chars remaining in this buffer, its mark will be
       * undefined, and its byte order will be
       * identical to that of this buffer.

       * The new buffer will be direct if, and only if, this buffer is direct, and
       * it will be read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  The new char buffer
       */
      CharBuffer& slice() const override = 0;

      /**
       * Creates a new char buffer whose content is a shared subsequence of
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
       * identical to that of this buffer.
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
       */
      CharBuffer& slice(gint index, gint length) const override = 0;

      /**
       * Creates a new char buffer that shares this buffer's content.
       *
       * <p> The content of the new buffer will be that of this buffer.  Changes
       * to this buffer's content will be visible in the new buffer, and vice
       * versa; the two buffers' position, limit, and mark values will be
       * independent.
       *
       * <p> The new buffer's capacity, limit, position,
       * mark values, and byte order will be identical to those of this buffer.
       * The new buffer will be direct if, and only if, this buffer is direct, and
       * it will be read-only if, and only if, this buffer is read-only.  </p>
       *
       * @return  The new char buffer
       */
      CharBuffer& duplicate() const override = 0;

      /**
       * Creates a new, read-only char buffer that shares this buffer's
       * content.
       *
       * <p> The content of the new buffer will be that of this buffer.  Changes
       * to this buffer's content will be visible in the new buffer; the new
       * buffer itself, however, will be read-only and will not allow the shared
       * content to be modified.  The two buffers' position, limit, and mark
       * values will be independent.
       *
       * <p> The new buffer's capacity, limit, position,
       * mark values, and byte order will be identical to those of this buffer.
       *
       * <p> If this buffer is itself read-only then this method behaves in
       * exactly the same way as the @b duplicate method.  </p>
       *
       * @return  The new, read-only char buffer
       */
      virtual CharBuffer& asReadOnlyBuffer() const = 0;

      /**
       * Relative <i>get</i> method.  Reads the char at this buffer's
       * current position, and then increments the position.
       *
       * @return  The char at the buffer's current position
       *
       * @throws  BufferUnderflowException
       *          If the buffer's current position is not smaller than its limit
       */
      virtual gchar get() = 0;

      /**
       * Relative <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> Writes the given char into this buffer at the current
       * position, and then increments the position. </p>
       *
       * @param  c
       *         The char to be written
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If this buffer's current position is not smaller than its limit
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual CharBuffer& put(gchar c) = 0;

      /**
       * Absolute <i>get</i> method.  Reads the char at the given
       * index.
       *
       * @param  index
       *         The index from which the char will be read
       *
       * @return  The char at the given index
       *
       * @throws  IndexOutOfBoundsException
       *          If @c index is negative
       *          or not smaller than the buffer's limit
       */
      virtual gchar get(gint index) const = 0;


      /**
       * Absolute <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> Writes the given char into this buffer at the given
       * index. </p>
       *
       * @param  index
       *         The index at which the char will be written
       *
       * @param  c
       *         The char value to be written
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
      virtual CharBuffer& put(gint index, gchar c) = 0;

      /**
       * Relative bulk <i>get</i> method.
       *
       * <p> This method transfers chars from this buffer into the given
       * destination array.  If there are fewer chars remaining in the
       * buffer than are required to satisfy the request, that is, if
       * @c length @c > @c remaining(), then no
       * chars are transferred and a @b BufferUnderflowException is
       * thrown.
       *
       * <p> Otherwise, this method copies @c length chars from this
       * buffer into the given array, starting at the current position of this
       * buffer and at the given offset in the array.  The position of this
       * buffer is then incremented by @c length.
       *
       * <p> In other words, an invocation of this method of the form
       * <code>src.get(dst, off, len)</code> has exactly the same effect as
       * the loop
       *
       * @code
       *     for (int i = off; i < off + len; i++)
       *         dst[i] = src.get();
       * @endcode
       *
       * except that it first checks that there are sufficient chars in
       * this buffer and it is potentially much more efficient.
       *
       * @param  dst
       *         The array into which chars are to be written
       *
       * @param  offset
       *         The offset within the array of the first char to be
       *         written; must be non-negative and no larger than
       *         @c dst.length()
       *
       * @param  length
       *         The maximum number of chars to be written to the given
       *         array; must be non-negative and no larger than
       *         @code dst.length() - offset @endcode
       *
       * @return  This buffer
       *
       * @throws  BufferUnderflowException
       *          If there are fewer than @c length chars
       *          remaining in this buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c offset and @c length
       *          parameters do not hold
       */
      virtual CharBuffer& get(CharArray& dst, gint offset, gint length);

      /**
       * Relative bulk <i>get</i> method.
       *
       * <p> This method transfers chars from this buffer into the given
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
       *          If there are fewer than @c length chars
       *          remaining in this buffer
       */
      virtual CharBuffer& get(CharArray& dst);

      /**
       * Absolute bulk <i>get</i> method.
       *
       * <p> This method transfers @c length chars from this
       * buffer into the given array, starting at the given index in this
       * buffer and at the given offset in the array.  The position of this
       * buffer is unchanged.
       *
       * <p> An invocation of this method of the form
       * <code>src.get(index, dst, offset, length)</code>
       * has exactly the same effect as the following loop except that it first
       * checks the consistency of the supplied parameters and it is potentially
       * much more efficient:
       *
       * @code
       *     for (gint i = offset, j = index; i < offset + length; i++, j++)
       *         dst[i] = src.get(j);
       * @endcode
       *
       * @param  index
       *         The index in this buffer from which the first char will be
       *         read; must be non-negative and less than @c limit()
       *
       * @param  dst
       *         The destination array
       *
       * @param  offset
       *         The offset within the array of the first char to be
       *         written; must be non-negative and less than
       *         @c dst.length()
       *
       * @param  length
       *         The number of chars to be written to the given array;
       *         must be non-negative and no larger than the smaller of
       *         @code limit() - index @endcode and @code dst.length() - offset @endcode
       *
       * @return  This buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c index, @c offset, and
       *          @c length parameters do not hold
       */
      virtual CharBuffer& get(gint index, CharArray& dst, gint offset, gint length);

      /**
       * Absolute bulk <i>get</i> method.
       *
       * <p> This method transfers chars from this buffer into the given
       * destination array.  The position of this buffer is unchanged.  An
       * invocation of this method of the form
       * <code>src.get(index, dst)</code> behaves in exactly the same
       * way as the invocation:
       *
       * @code
       *     src.get(index, dst, 0, dst.length)
       * @endcode
       *
       * @param  index
       *         The index in this buffer from which the first char will be
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
       */
      virtual CharBuffer& get(gint index, CharArray& dst);

      /**
       * Relative bulk <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> This method transfers the chars remaining in the given source
       * buffer into this buffer.  If there are more chars remaining in the
       * source buffer than in this buffer, that is, if
       * @c src.remaining() @c > @c remaining(),
       * then no chars are transferred and a @b BufferOverflowException is thrown.
       *
       * <p> Otherwise, this method copies
       * <i>n</i> = @c src.remaining() chars from the given
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
       *         The source buffer from which chars are to be read;
       *         must not be this buffer
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *          for the remaining chars in the source buffer
       *
       * @throws  IllegalArgumentException
       *          If the source buffer is this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual CharBuffer& put(CharBuffer& src);

      /**
       * Absolute bulk <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> This method transfers @c length chars into this buffer from
       * the given source buffer, starting at the given @c offset in the
       * source buffer and the given @c index in this buffer. The positions
       * of both buffers are unchanged.
       *
       * <p> In other words, an invocation of this method of the form
       * <code>dst.put(index, src, offset, length)</code>
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
       *        The index in this buffer at which the first char will be
       *        written; must be non-negative and less than @c limit()
       *
       * @param src
       *        The buffer from which chars are to be read
       *
       * @param offset
       *        The index within the source buffer of the first char to be
       *        read; must be non-negative and less than @c src.limit()
       *
       * @param length
       *        The number of chars to be read from the given buffer;
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
       */
      virtual CharBuffer& put(gint index, CharBuffer& src, gint offset, gint length);

      /**
       * Relative bulk <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> This method transfers chars into this buffer from the given
       * source array.  If there are more chars to be copied from the array
       * than remain in this buffer, that is, if
       * @c length @c > @c remaining(), then no
       * chars are transferred and a @b BufferOverflowException is
       * thrown.
       *
       * <p> Otherwise, this method copies @c length chars from the
       * given array into this buffer, starting at the given offset in the array
       * and at the current position of this buffer.  The position of this buffer
       * is then incremented by @c length.
       *
       * <p> In other words, an invocation of this method of the form
       * <code>dst.put(src, off, len)</code> has exactly the same effect as
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
       *         The array from which chars are to be read
       *
       * @param  offset
       *         The offset within the array of the first char to be read;
       *         must be non-negative and no larger than @c src.length()
       *
       * @param  length
       *         The number of chars to be read from the given array;
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
      virtual CharBuffer& put(CharArray const& src, gint offset, gint length);

      /**
       * Relative bulk <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> This method transfers the entire content of the given source
       * char array into this buffer.  An invocation of this method of the
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
      virtual CharBuffer& put(CharArray const& src);

      /**
       * Absolute bulk <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> This method transfers @c length chars from the given
       * array, starting at the given offset in the array and at the given index
       * in this buffer.  The position of this buffer is unchanged.
       *
       * <p> An invocation of this method of the form
       * <code>dst.put(index, src, offset, length)</code>
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
       *         The index in this buffer at which the first char will be
       *         written; must be non-negative and less than @c limit()
       *
       * @param  src
       *         The array from which chars are to be read
       *
       * @param  offset
       *         The offset within the array of the first char to be read;
       *         must be non-negative and less than @c src.length()
       *
       * @param  length
       *         The number of chars to be read from the given array;
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
       */
      virtual CharBuffer& put(gint index, CharArray const& src, gint offset, gint length);

      /**
       * Absolute bulk <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> This method copies chars into this buffer from the given source
       * array.  The position of this buffer is unchanged.  An invocation of this
       * method of the form <code>dst.put(index, src)</code>
       * behaves in exactly the same way as the invocation:
       *
       * @code
       *     dst.put(index, src, 0, src.length);
       * @endcode
       *
       * @param  index
       *         The index in this buffer at which the first char will be
       *         written; must be non-negative and less than @c limit()
       *
       * @param  src
       *         The array from which chars are to be read
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
      virtual CharBuffer& put(gint index, CharArray const& src);


      /**
       * Relative bulk <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> This method transfers chars from the given string into this
       * buffer.  If there are more chars to be copied from the string than
       * remain in this buffer, that is, if
       * <code>end - start</code> @c > @c remaining(),
       * then no chars are transferred and a @b BufferOverflowException is thrown.
       *
       * <p> Otherwise, this method copies
       * <i>n</i> = @c end - @c start chars
       * from the given string into this buffer, starting at the given
       * @c start index and at the current position of this buffer.  The
       * position of this buffer is then incremented by <i>n</i>.
       *
       * <p> In other words, an invocation of this method of the form
       * <code>dst.put(src, start, end)</code> has exactly the same effect
       * as the loop
       *
       * @code
       *     for (int i = start; i < end; i++)
       *         dst.put(src.charAt(i));
       * @endcode
       *
       * except that it first checks that there is sufficient space in this
       * buffer and it is potentially much more efficient.
       *
       * @param  src
       *         The string from which chars are to be read
       *
       * @param  start
       *         The offset within the string of the first char to be read;
       *         must be non-negative and no larger than
       *         @c string.length()
       *
       * @param  end
       *         The offset within the string of the last char to be read,
       *         plus one; must be non-negative and no larger than
       *         @c string.length()
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on the @c start and @c end
       *          parameters do not hold
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual CharBuffer& put(String const& src, gint start, gint end);

      /**
       * Relative bulk <i>put</i> method  <i>(optional operation)</i>.
       *
       * <p> This method transfers the entire content of the given source string
       * into this buffer.  An invocation of this method of the form
       * @c dst.put(s) behaves in exactly the same way as the invocation
       *
       * @code
       *     dst.put(s, 0, s.length())
       * @endcode
       *
       * @param   src
       *          The source string
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual CharBuffer& put(String const& src);

      /**
       * Tells whether or not this buffer is backed by an accessible char
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
       * Returns the char array that backs this
       * buffer  <i>(optional operation)</i>.
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
      CharArray& array() const override = 0;

      /**
       * Returns the offset within this buffer's backing array of the first
       * element of the buffer  <i>(optional operation)</i>.
       *
       * <p> If this buffer is backed by an array then buffer position <i>p</i>
       * corresponds to array index <i>p</i> + @c arrayOffset().
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

      CharBuffer& position(gint newPosition) override;

      gint limit() const override;

      CharBuffer& limit(gint newLimit) override;

      CharBuffer& mark() override;

      CharBuffer& reset() override;

      CharBuffer& clear() override;

      CharBuffer& flip() override;

      CharBuffer& rewind() override;

      /**
       * Compacts this buffer  <i>(optional operation)</i>.
       *
       * <p> The chars between the buffer's current position and its limit,
       * if any, are copied to the beginning of the buffer.  That is, the
       * char at index <i>p</i> = @c position() is copied
       * to index zero, the char at index <i>p</i> + 1 is copied
       * to index one, and so forth until the char at index
       * @c limit() - 1 is copied to index
       * <i>n</i> = @c limit() - @c 1 - <i>p</i>.
       * The buffer's position is then set to <i>n+1</i> and its limit is set to
       * its capacity.  The mark, if defined, is discarded.
       *
       * <p> The buffer's position is set to the number of chars copied,
       * rather than to zero, so that an invocation of this method can be
       * followed immediately by an invocation of another relative <i>put</i>
       * method. </p>
       *
       * @return  This buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      virtual CharBuffer& compact() = 0;

      /**
       * Tells whether or not this char buffer is direct.
       *
       * @return  @c true if, and only if, this buffer is direct
       */
      gbool isDirect() const override = 0;


      /**
       * Tells whether this buffer has addressable memory, e.g., a Java array or
       * a native address.  This method returns @c true.  Subclasses such as
       * @c StringCharBuffer, which wraps a @c CharSequence, should
       * override this method to return @c false.
       *
       * @return @c true if, and only, this buffer has addressable memory
       */
      virtual gbool isAddressable() const;

      /**
       * Returns the current hash code of this buffer.
       *
       * <p> The hash code of a char buffer depends only upon its remaining
       * elements; that is, upon the elements from @c position() up to, and
       * including, the element at @c limit() - @c 1.
       *
       * <p> Because buffer hash codes are content-dependent, it is inadvisable
       * to use buffers as keys in hash maps or similar data structures unless it
       * is known that their contents will not change.  </p>
       *
       * @return  The current hash code of this buffer
       */
      gint hash() const override;

      /**
       * Tells whether or not this buffer is equal to another object.
       *
       * <p> Two char buffers are equal if, and only if,
       *
       *  - They have the same element type,   <br/>
       *
       *  - They have the same number of remaining elements, and <br/>
       *
       *  - The two sequences of remaining elements, considered
       *   independently of their starting positions, are point-wise equal. <br/>
       *
       * <p> A char buffer is not equal to any other type of object.  </p>
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
       * <p> Two char buffers are compared by comparing their sequences of
       * remaining elements lexicographically, without regard to the starting
       * position of each sequence within its corresponding buffer.








       * Pairs of @c gchar elements are compared as if by invoking
       * @code Character::compare(gchar,gchar) @endcode .

       *
       * <p> A char buffer is not comparable to any other type of object.
       *
       * @return  A negative integer, zero, or a positive integer as this buffer
       *          is less than, equal to, or greater than the given buffer
       */
      gint compareTo(const CharBuffer& that) const override;

      /**
       * Finds and returns the relative index of the first mismatch between this
       * buffer and a given buffer.  The index is relative to the
       * @b position of each buffer and will be in the range of
       * 0 (inclusive) up to the smaller of the @b remaining
       * elements in each buffer (exclusive).
       *
       * <p> If the two buffers share a common prefix then the returned index is
       * the length of the common prefix, and it follows that there is a mismatch
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
      gint mismatch(CharBuffer const& that) const;


      /**
       * Returns a string containing the characters in this buffer.
       *
       * <p> The first character of the resulting string will be the character at
       * this buffer's position, while the last character will be the character
       * at index @c limit() - 1.  Invoking this method does not
       * change the buffer's position. </p>
       *
       * @return  The specified string
       */
      String toString() const override = 0;

      /**
       * Returns the length of this character buffer.
       *
       * <p> When viewed as a character sequence, the length of a character
       * buffer is simply the number of characters between the position
       * (inclusive) and the limit (exclusive); that is, it is equivalent to
       * @c remaining(). </p>
       *
       * @return  The length of this character buffer
       */
      gint length() const override;

      /**
       * Returns @c true if this character buffer is empty.
       *
       * @return @c true if there are @c 0 remaining characters,
       *         otherwise @c false
       */
      gbool isEmpty() const override;

      /**
       * Reads the character at the given index relative to the current
       * position.
       *
       * @param  index
       *         The index of the character to be read, relative to the position;
       *         must be non-negative and smaller than @c remaining()
       *
       * @return  The character at index
       *          <code>position() + index</code>
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on @c index do not hold
       */
      gchar charAt(gint index) const override;

      /**
       * Creates a new character buffer that represents the specified subsequence
       * of this buffer, relative to the current position.
       *
       * <p> The new buffer will share this buffer's content; that is, if the
       * content of this buffer is mutable then modifications to one buffer will
       * cause the other to be modified.  The new buffer's capacity will be that
       * of this buffer, its position will be
       * @c position() + @c start, its limit will be
       * @c position() + @c end, and its byte order
       * will be identical to that of this buffer. The new buffer will be direct
       * if, and only if, this buffer is direct, and it will be read-only
       * if, and only if, this buffer is read-only.  </p>
       *
       * @param  start
       *         The index, relative to the current position, of the first
       *         character in the subsequence; must be non-negative and no larger
       *         than @c remaining()
       *
       * @param  end
       *         The index, relative to the current position, of the character
       *         following the last character in the subsequence; must be no
       *         smaller than @c start and no larger than
       *         @c remaining()
       *
       * @return  The new character buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If the preconditions on @c start and @c end
       *          do not hold
       */
      CharBuffer& subSequence(gint start, gint end) const override = 0;

      /**
       * Appends the specified character sequence  to this
       * buffer  <i>(optional operation)</i>.
       *
       * <p> An invocation of this method of the form @c dst.append(csq)
       * behaves in exactly the same way as the invocation
       *
       * @code
       *     dst.put(csq.toString())
       * @endcode
       *
       * <p> Depending on the specification of @c toString for the
       * character sequence @c csq, the entire sequence may not be
       * appended.  For instance, invoking the
       * @b toString method of a character buffer will return a subsequence whose
       * content depends upon the buffer's position and limit.
       *
       * @param  csq
       *         The character sequence to append.
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      CharBuffer& append(const CharSequence& csq) override;

      /**
       * Appends a subsequence of the  specified character sequence  to this
       * buffer  <i>(optional operation)</i>.
       *
       * <p> An invocation of this method of the form @code dst.append(csq, start,
       * end) @endcode when @c csq is not @c null, behaves in exactly the
       * same way as the invocation
       *
       * @code
       *     dst.put(csq.subSequence(start, end).toString())
       * @endcode
       *
       * @param  csq
       *         The character sequence from which a subsequence will be
       *         appended.
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *
       * @throws  IndexOutOfBoundsException
       *          If @c start or @c end are negative, @c start
       *          is greater than @c end, or @c end is greater than
       *          @c csq.length()
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      CharBuffer& append(const CharSequence& csq, gint start, gint end) override;

      /**
       * Appends the specified char  to this
       * buffer  <i>(optional operation)</i>.
       *
       * <p> An invocation of this method of the form @c dst.append(c)
       * behaves in exactly the same way as the invocation
       *
       * @code
       *     dst.put(c)
       * @endcode
       *
       * @param  c
       *         The 16-bit char to append
       *
       * @return  This buffer
       *
       * @throws  BufferOverflowException
       *          If there is insufficient space in this buffer
       *
       * @throws  ReadOnlyBufferException
       *          If this buffer is read-only
       */
      CharBuffer& append(gchar c) override;


      /**
       * Retrieves this buffer's byte order.
       *
       * <p> The byte order of a char buffer created by allocation or by
       * wrapping an existing @c gchar array is the
       * <b> native order</b> of the underlying
       * hardware.  The byte order of a char buffer created as a
       * <a href="ByteBuffer.html#views">view</a> of a byte buffer is that of the
       * byte buffer at the moment that the view is created.  </p>
       *
       * @return  This buffer's byte order
       */
      virtual charset::Charset::ByteOrder order() const;

    private:
      Object& base() const override;

      virtual gchar getFast(gint i) const = 0;

      void getArray(gint index, CharArray& dst, gint offset, gint length) const;

      virtual CharBuffer& putArray(gint index, CharArray const& src, gint offset, gint length);

      virtual void putBuffer(gint pos, CharBuffer& src, gint srcPos, gint n);

      virtual gint charOrder() const = 0;

      static CharArray& createHeap(gint heapSize);

      class HeapCharBuffer;
      class ReadOnlyHeapCharBuffer;
      class StringCharBuffer;

      CORE_ADD_AS_FRIEND(Buffer);
      CORE_ADD_AS_FRIEND(charset::CharsetDecoder);
      CORE_ADD_AS_FRIEND(charset::CharsetEncoder);
    };

    /**
     * A read/write HeapCharBuffer.
     */
    class CharBuffer::HeapCharBuffer : public CharBuffer {
      CharArray& hb;
      gint offset = 0;

      CORE_ADD_AS_FRIEND(ReadOnlyHeapCharBuffer);

    public:
      CORE_EXPLICIT HeapCharBuffer(gint cap, gint lim, Object& segment);

      CORE_EXPLICIT HeapCharBuffer(CharArray& buf, gint off, gint len, Object& segment);

      CORE_EXPLICIT HeapCharBuffer(CharArray& buf,
                                   gint mark, gint pos, gint lim, gint cap, gint off, Object& segment);

      CharBuffer& slice() const override;

      CharBuffer& slice(gint index, gint length) const override;

      CharBuffer& duplicate() const override;

      CharBuffer& asReadOnlyBuffer() const override;

      gchar get() override;

      gchar get(gint index) const override;

      CharBuffer& get(CharArray& dst, gint offset, gint length) override;

      CharBuffer& get(gint index, CharArray& dst, gint offset, gint length) override;

      gbool isReadOnly() const override;

      gbool isDirect() const override;

      CharBuffer& put(gchar c) override;

      CharBuffer& put(gint index, gchar c) override;

      CharBuffer& put(const CharArray& src, gint offset, gint length) override;

      CharBuffer& put(gint index, const CharArray& src, gint offset, gint length) override;

      CharBuffer& put(gint index, const CharArray& src) override;

      CharBuffer& append(const CharSequence& csq) override;

      CharBuffer& append(const CharSequence& csq, gint start, gint end) override;

      CharBuffer& put(const String& src, gint start, gint end) override;

      CharBuffer& compact() override;

      CharBuffer& subSequence(gint start, gint end) const override;

      gbool hasArray() const override;

      CharArray& array() const override;

      gint arrayOffset() const override;

      String toString() const override;

      charset::Charset::ByteOrder order() const override;

    private:
      virtual gint ix(gint i) const;

      gchar getFast(gint i) const override;

      Object& base() const final;


      //
      // Use getChars() to load chars directly into the heap buffer array.
      // For a String or StringBuffer source this improves performance if
      // a proper subsequence is being appended as copying to a new intermediate
      // String object is avoided. For a StringBuilder where either a subsequence
      // or the full sequence of chars is being appended, copying the chars to
      // an intermediate String in StringBuilder::toString is avoided.
      //
      virtual CharBuffer& appendChars(CharSequence const& csq, gint start, gint end);

      gint charOrder() const override;
    };

    /**
     * A read-only HeapCharBuffer.  This class extends the corresponding
     * read/write class, overriding the mutation methods to throw a
     * @b ReadOnlyBufferException and overriding the view-buffer methods to return an
     * instance of this class rather than of the superclass.
     */
    class CharBuffer::ReadOnlyHeapCharBuffer final : public HeapCharBuffer {
    public:
      CORE_EXPLICIT ReadOnlyHeapCharBuffer(gint cap, gint lim, Object& segment);

      CORE_EXPLICIT ReadOnlyHeapCharBuffer(CharArray& buf, gint off, gint len, Object& segment);

      CORE_EXPLICIT ReadOnlyHeapCharBuffer(CharArray& buf,
                                           gint mark, gint pos, gint lim, gint cap, gint off, Object& segment);

      CharBuffer& slice() const override;

      CharBuffer& slice(gint index, gint length) const override;

      CharBuffer& duplicate() const override;

      CharBuffer& asReadOnlyBuffer() const override;

      gbool isReadOnly() const override;

      CharBuffer& put(gchar c) override;

      CharBuffer& put(gint index, gchar c) override;

      CharBuffer& put(const CharArray& src, gint offset, gint length) override;

      CharBuffer& put(const CharArray& src) override;

      CharBuffer& put(gint index, const CharArray& src, gint offset, gint length) override;

      CharBuffer& put(gint index, const CharArray& src) override;

      CharBuffer& append(gchar c) override;

      CharBuffer& append(const CharSequence& csq) override;

      CharBuffer& append(const CharSequence& csq, gint start, gint end) override;

      CharBuffer& put(const String& src) override;

      CharBuffer& put(const String& src, gint start, gint end) override;

      CharBuffer& compact() override;

      String toString() const override;

      CharBuffer& subSequence(gint start, gint end) const override;

      gbool hasArray() const override;

      CharArray& array() const override;

      gint arrayOffset() const override;

    private:
      CharBuffer& appendChars(const CharSequence& csq, gint start, gint end) override;
    };

    class CharBuffer::StringCharBuffer final : public CharBuffer {
      CharSequence const& str;
      gint offset = 0;

    public:
      CORE_EXPLICIT StringCharBuffer(CharSequence const& csq, gint start, gint end);

      CORE_EXPLICIT StringCharBuffer(CharSequence const& csq, gint mark, gint pos, gint lim, gint cap, gint off);

      CharBuffer& slice() const override;

      CharBuffer& slice(gint index, gint length) const override;

      gbool isReadOnly() const override;

      CharBuffer& duplicate() const override;

      CharBuffer& asReadOnlyBuffer() const override;

      gchar get() override;

      CharBuffer& put(gchar c) override;

      gchar get(gint index) const override;

      CharBuffer& put(gint index, gchar c) override;

      CharBuffer& compact() override;

      gbool isDirect() const override;

      CharBuffer& subSequence(gint start, gint end) const override;

      gbool hasArray() const override;

      CharArray& array() const override;

      gint arrayOffset() const override;

      charset::Charset::ByteOrder order() const override;

      String toString() const override;

    private:
      gchar getFast(gint i) const override;

      gint charOrder() const override;
    };
  } // io
} // core

#endif //CORE24_CHARBUFFER_H
