//
// Created by brunshweeck on 27 août 2024.
//

#ifndef CORE24_SHORTBUFFER_H
#define CORE24_SHORTBUFFER_H

#include <core/io/Buffer.h>
#include <core/charset/Charset.h>
#include <core/ShortArray.h>

namespace core {
    namespace io {
        /**
         * A short buffer.
         * This class defines four categories of operations upon
         * short buffers:
         *  @li <p> Absolute and relative <i>get</i> and
         *   <i>put</i> methods that read and write
         *   single shorts; </p>
         *
         *  @li <p> Absolute and relative <i>bulk get</i>
         *   methods that transfer contiguous sequences of shorts from this buffer
         *   into an array;</p>
         *
         *  @li <p> Absolute and relative <i>bulk put</i>
         *   methods that transfer contiguous sequences of shorts from a
         *   short array or some other short
         *   buffer into this buffer;</p>
         *
         *
         *  @li <p> A method for compacting a short buffer.  </p>
         *
         * <p>
         * Short buffers can be created either by
         * <i>allocation</i>, which allocates space for the buffer's
         * content, by <i>wrapping</i> an existing
         * short array  into a buffer, or by creating a
         * <a href="ByteBuffer.html#views"><i>view</i></a> of an existing byte buffer.
         * <p> Like a byte buffer, a short buffer is either
         * <a href="ByteBuffer.html#direct"><i>direct</i> or <i>non-direct</i></a>.  A
         * short buffer created via the @c wrap methods of this class will
         * be non-direct.  A short buffer created as a view of a byte buffer will
         * be direct if, and only if, the byte buffer itself is direct.  Whether
         * a short buffer is direct may be determined by invoking the
         * @b isDirect method.
         * </p>
         * <p>
         * Methods in this class that do not otherwise have a value to return are
         * specified to return the buffer upon which they are invoked.  This allows
         * method invocations to be chained.
         * </p>
         * <h2> Optional operations </h2>
         * <p>
         * Methods specified as
         * <i>operations</i> throw a @em ReadOnlyBufferException when invoked
         * on a @em read-only ShortBuffer. The methods @em array and @em arrayOffset
         * throw an @em UnsupportedOperationException if the ShortBuffer is
         * not backed by an <em> accessible short array</em>
         * (irrespective of whether the ShortBuffer is read-only).
         * </p>
         */
        class ShortBuffer : public virtual Buffer,
                            public virtual Comparable<ShortBuffer> {
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
            CORE_EXPLICIT ShortBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment);

        public:
            /**
             * Allocates a new short buffer.
             *
             * <p> The new buffer's position will be zero, its limit will be its
             * capacity, its mark will be undefined, each of its elements will be
             * initialized to zero, and its byte order will be
             * the <b> native order</b> of the underlying hardware.
             * It will have a <b> backing array</b>, and its
             * <b> array offset</b> will be zero.
             *
             * @param  capacity
             *         The new buffer's capacity, in shorts
             *
             * @return  The new short buffer
             *
             * @throws  IllegalArgumentException
             *          If the @c capacity is a negative integer
             */
            static ShortBuffer& allocate(gint capacity);

            /**
             * Wraps a short array into a buffer.
             *
             * <p> The new buffer will be backed by the given short array;
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
             * @return  The new short buffer
             *
             * @throws  IndexOutOfBoundsException
             *          If the preconditions on the @c offset and @c length
             *          parameters do not hold
             */
            static ShortBuffer& wrap(ShortArray& array, gint offset, gint length);

            /**
             * Wraps a short array into a buffer.
             *
             * <p> The new buffer will be backed by the given short array;
             * that is, modifications to the buffer will cause the array to be modified
             * and vice versa.  The new buffer's capacity and limit will be
             * @c array.length(), its position will be zero, its mark will be
             * undefined, and its byte order will be
             * the <b> native order</b> of the underlying
             * hardware.
             * Its <b>backing array</b> will be the given array, and its
             * <b> array offset</b> will be zero.  </p>
             *
             * @param  array
             *         The array that will back this buffer
             *
             * @return  The new short buffer
             */
            static ShortBuffer& wrap(ShortArray& array);

            /**
             * Creates a new short buffer whose content is a shared subsequence of
             * this buffer's content.
             *
             * <p> The content of the new buffer will start at this buffer's current
             * position.  Changes to this buffer's content will be visible in the new
             * buffer, and vice versa; the two buffers' position, limit, and mark
             * values will be independent.
             *
             * <p> The new buffer's position will be zero, its capacity and its limit
             * will be the number of shorts remaining in this buffer, its mark will be
             * undefined, and its byte order will be
             * identical to that of this buffer.

             * The new buffer will be direct if, and only if, this buffer is direct, and
             * it will be read-only if, and only if, this buffer is read-only.  </p>
             *
             * @return  The new short buffer
             */
            ShortBuffer& slice() const override = 0;

            /**
             * Creates a new short buffer whose content is a shared subsequence of
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
            ShortBuffer& slice(gint index, gint length) const override = 0;

            /**
             * Creates a new short buffer that shares this buffer's content.
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
             * @return  The new short buffer
             */
            ShortBuffer& duplicate() const override = 0;

            /**
             * Creates a new, read-only short buffer that shares this buffer's
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
             * @return  The new, read-only short buffer
             */
            virtual ShortBuffer& asReadOnlyBuffer() const = 0;

            /**
             * Relative <i>get</i> method.  Reads the short at this buffer's
             * current position, and then increments the position.
             *
             * @return  The short at the buffer's current position
             *
             * @throws  BufferUnderflowException
             *          If the buffer's current position is not smaller than its limit
             */
            virtual gshort get() = 0;

            /**
             * Relative <i>put</i> method  <i>(optional operation)</i>.
             *
             * <p> Writes the given short into this buffer at the current
             * position, and then increments the position. </p>
             *
             * @param  c
             *         The short to be written
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If this buffer's current position is not smaller than its limit
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer& put(gshort c) = 0;

            /**
             * Absolute <i>get</i> method.  Reads the short at the given
             * index.
             *
             * @param  index
             *         The index from which the short will be read
             *
             * @return  The short at the given index
             *
             * @throws  IndexOutOfBoundsException
             *          If @c index is negative
             *          or not smaller than the buffer's limit
             */
            virtual gshort get(gint index) const = 0;

            /**
             * Absolute <i>put</i> method  <i>(optional operation)</i>.
             *
             * <p> Writes the given short into this buffer at the given
             * index. </p>
             *
             * @param  index
             *         The index at which the short will be written
             *
             * @param  c
             *         The short value to be written
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
            virtual ShortBuffer& put(gint index, gshort c) = 0;

            /**
             * Relative bulk <i>get</i> method.
             *
             * <p> This method transfers shorts from this buffer into the given
             * destination array.  If there are fewer shorts remaining in the
             * buffer than are required to satisfy the request, that is, if
             * @c length @c > @c remaining(), then no
             * shorts are transferred and a @b BufferUnderflowException is
             * thrown.
             *
             * <p> Otherwise, this method copies @c length shorts from this
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
             * except that it first checks that there are sufficient shorts in
             * this buffer, and it is potentially much more efficient.
             *
             * @param  dst
             *         The array into which shorts are to be written
             *
             * @param  offset
             *         The offset within the array of the first short to be
             *         written; must be non-negative and no larger than
             *         @c dst.length()
             *
             * @param  length
             *         The maximum number of shorts to be written to the given
             *         array; must be non-negative and no larger than
             *         @code dst.length() - offset @endcode
             *
             * @return  This buffer
             *
             * @throws  BufferUnderflowException
             *          If there are fewer than @c length shorts
             *          remaining in this buffer
             *
             * @throws  IndexOutOfBoundsException
             *          If the preconditions on the @c offset and @c length
             *          parameters do not hold
             */
            virtual ShortBuffer& get(ShortArray& dst, gint offset, gint length);

            /**
             * Relative bulk <i>get</i> method.
             *
             * <p> This method transfers shorts from this buffer into the given
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
             *          If there are fewer than @c length shorts
             *          remaining in this buffer
             */
            virtual ShortBuffer& get(ShortArray& dst);

            /**
             * Absolute bulk <i>get</i> method.
             *
             * <p> This method transfers @c length shorts from this
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
             *         The index in this buffer from which the first short will be
             *         read; must be non-negative and less than @c limit()
             *
             * @param  dst
             *         The destination array
             *
             * @param  offset
             *         The offset within the array of the first short to be
             *         written; must be non-negative and less than
             *         @c dst.length()
             *
             * @param  length
             *         The number of shorts to be written to the given array;
             *         must be non-negative and no larger than the smaller of
             *         @code limit() - index @endcode and @code dst.length() - offset @endcode
             *
             * @return  This buffer
             *
             * @throws  IndexOutOfBoundsException
             *          If the preconditions on the @c index, @c offset, and
             *          @c length parameters do not hold
             */
            virtual ShortBuffer& get(gint index, ShortArray& dst, gint offset, gint length);

            /**
             * Absolute bulk <i>get</i> method.
             *
             * <p> This method transfers shorts from this buffer into the given
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
             *         The index in this buffer from which the first short will be
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
            virtual ShortBuffer& get(gint index, ShortArray& dst);

            /**
             * Relative bulk <i>put</i> method  <i>(optional operation)</i>.
             *
             * <p> This method transfers the shorts remaining in the given source
             * buffer into this buffer.  If there are more shorts remaining in the
             * source buffer than in this buffer, that is, if
             * @c src.remaining() @c > @c remaining(),
             * then no shorts are transferred and a @b BufferOverflowException is thrown.
             *
             * <p> Otherwise, this method copies
             * <i>n</i> = @c src.remaining() shorts from the given
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
             *         The source buffer from which shorts are to be read;
             *         must not be this buffer
             *
             * @return  This buffer
             *
             * @throws  BufferOverflowException
             *          If there is insufficient space in this buffer
             *          for the remaining shorts in the source buffer
             *
             * @throws  IllegalArgumentException
             *          If the source buffer is this buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer& put(ShortBuffer& src);

            /**
             * Absolute bulk <i>put</i> method  <i>(optional operation)</i>.
             *
             * <p> This method transfers @c length shorts into this buffer from
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
             *        The index in this buffer at which the first short will be
             *        written; must be non-negative and less than @c limit()
             *
             * @param src
             *        The buffer from which shorts are to be read
             *
             * @param offset
             *        The index within the source buffer of the first short to be
             *        read; must be non-negative and less than @c src.limit()
             *
             * @param length
             *        The number of shorts to be read from the given buffer;
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
            virtual ShortBuffer& put(gint index, ShortBuffer& src, gint offset, gint length);

            /**
             * Relative bulk <i>put</i> method  <i>(optional operation)</i>.
             *
             * <p> This method transfers shorts into this buffer from the given
             * source array.  If there are more shorts to be copied from the array
             * than remain in this buffer, that is, if
             * @c length @c > @c remaining(), then no
             * shorts are transferred and a @b BufferOverflowException is
             * thrown.
             *
             * <p> Otherwise, this method copies @c length shorts from the
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
             *         The array from which shorts are to be read
             *
             * @param  offset
             *         The offset within the array of the first short to be read;
             *         must be non-negative and no larger than @c src.length()
             *
             * @param  length
             *         The number of shorts to be read from the given array;
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
            virtual ShortBuffer& put(ShortArray const& src, gint offset, gint length);

            /**
             * Relative bulk <i>put</i> method  <i>(optional operation)</i>.
             *
             * <p> This method transfers the entire content of the given source
             * short array into this buffer.  An invocation of this method of the
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
            virtual ShortBuffer& put(ShortArray const& src);

            /**
             * Absolute bulk <i>put</i> method  <i>(optional operation)</i>.
             *
             * <p> This method transfers @c length shorts from the given
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
             *         The index in this buffer at which the first short will be
             *         written; must be non-negative and less than @c limit()
             *
             * @param  src
             *         The array from which shorts are to be read
             *
             * @param  offset
             *         The offset within the array of the first short to be read;
             *         must be non-negative and less than @c src.length()
             *
             * @param  length
             *         The number of shorts to be read from the given array;
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
            virtual ShortBuffer& put(gint index, ShortArray const& src, gint offset, gint length);

            /**
             * Absolute bulk <i>put</i> method  <i>(optional operation)</i>.
             *
             * <p> This method copies shorts into this buffer from the given source
             * array.  The position of this buffer is unchanged.  An invocation of this
             * method of the form <code>dst.put(index, src)</code>
             * behaves in exactly the same way as the invocation:
             *
             * @code
             *     dst.put(index, src, 0, src.length);
             * @endcode
             *
             * @param  index
             *         The index in this buffer at which the first short will be
             *         written; must be non-negative and less than @c limit()
             *
             * @param  src
             *         The array from which shorts are to be read
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
            virtual ShortBuffer& put(gint index, ShortArray const& src);

            /**
             * Tells whether or not this buffer is backed by an accessible short
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
             * Returns the short array that backs this
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
            ShortArray& array() const override = 0;

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

            ShortBuffer& position(gint newPosition) override;

            gint limit() const override;

            ShortBuffer& limit(gint newLimit) override;

            ShortBuffer& mark() override;

            ShortBuffer& reset() override;

            ShortBuffer& clear() override;

            ShortBuffer& flip() override;

            ShortBuffer& rewind() override;

            /**
             * Compacts this buffer  <i>(optional operation)</i>.
             *
             * <p> The shorts between the buffer's current position and its limit,
             * if any, are copied to the beginning of the buffer.  That is, the
             * short at index <i>p</i> = @c position() is copied
             * to index zero, the short at index <i>p</i> + 1 is copied
             * to index one, and so forth until the short at index
             * @c limit() - 1 is copied to index
             * <i>n</i> = @c limit() - @c 1 - <i>p</i>.
             * The buffer's position is then set to <i>n+1</i> and its limit is set to
             * its capacity.  The mark, if defined, is discarded.
             *
             * <p> The buffer's position is set to the number of shorts copied,
             * rather than to zero, so that an invocation of this method can be
             * followed immediately by an invocation of another relative <i>put</i>
             * method. </p>
             *
             * @return  This buffer
             *
             * @throws  ReadOnlyBufferException
             *          If this buffer is read-only
             */
            virtual ShortBuffer& compact() = 0;

            /**
             * Tells whether or not this short buffer is direct.
             *
             * @return  @c true if, and only if, this buffer is direct
             */
            gbool isDirect() const override = 0;


            /**
             * Tells whether this buffer has addressable memory, e.g., a Java array or
             * a native address.  This method returns @c true.  Subclasses such as
             * @c StringShortBuffer, which wraps a @c ShortSequence, should
             * override this method to return @c false.
             *
             * @return @c true if, and only, this buffer has addressable memory
             */
            virtual gbool isAddressable() const;

            /**
             * Returns the current hash code of this buffer.
             *
             * <p> The hash code of a short buffer depends only upon its remaining
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
             * <p> Two short buffers are equal if, and only if,
             *
             *  @li <p> They have the same element type,  </p>
             *
             *  @li <p> They have the same number of remaining elements, and
             *   </p>
             *
             *  @li <p> The two sequences of remaining elements, considered
             *   independently of their starting positions, are point-wise equal.
             *   </p>
             *
             * <p> A short buffer is not equal to any other type of object.  </p>
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
             * <p> Two short buffers are compared by comparing their sequences of
             * remaining elements lexicographically, without regard to the starting
             * position of each sequence within its corresponding buffer.








             * Pairs of @c gshort elements are compared as if by invoking
             * @code Shortacter::compare(gshort,gshort) @endcode .

             *
             * <p> A short buffer is not comparable to any other type of object.
             *
             * @return  A negative integer, zero, or a positive integer as this buffer
             *          is less than, equal to, or greater than the given buffer
             */
            gint compareTo(const ShortBuffer& o) const override;

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
            gint mismatch(ShortBuffer const& that) const;

            /**
             * Returns a string containing the shortacters in this buffer.
             *
             * <p> The first shortacter of the resulting string will be the shortacter at
             * this buffer's position, while the last shortacter will be the shortacter
             * at index @c limit() - 1.  Invoking this method does not
             * change the buffer's position. </p>
             *
             * @return  The specified string
             */
            String toString() const override;

            virtual charset::Charset::ByteOrder order() const = 0;

        private:
            Object& base() const override;

            void getArray(gint index, ShortArray& dst, gint offset, gint length) const;

            virtual ShortBuffer& putArray(gint index, ShortArray const& src, gint offset, gint length);

            virtual void putBuffer(gint pos, ShortBuffer& src, gint srcPos, gint n);

            static ShortArray &createHeap(gint heapSize);

            class HeapShortBuffer;
            class ReadOnlyHeapShortBuffer;

            CORE_ADD_AS_FRIEND(Buffer);
        };

        /**
         * A read/write HeapShortBuffer.
         */
        class ShortBuffer::HeapShortBuffer : public ShortBuffer {
            ShortArray& hb;
            gint offset = 0;

            CORE_ADD_AS_FRIEND(ReadOnlyHeapShortBuffer);

        public:
            CORE_EXPLICIT HeapShortBuffer(gint cap, gint lim, Object& segment);

            CORE_EXPLICIT HeapShortBuffer(ShortArray& buf, gint off, gint len, Object& segment);

            CORE_EXPLICIT HeapShortBuffer(ShortArray& buf,
                                          gint mark, gint pos, gint lim, gint cap, gint off, Object& segment);

            ShortBuffer& slice() const override;

            ShortBuffer& slice(gint index, gint length) const override;

            ShortBuffer& duplicate() const override;

            ShortBuffer& asReadOnlyBuffer() const override;

            gshort get() override;

            gshort get(gint index) const override;

            ShortBuffer& get(ShortArray& dst, gint offset, gint length) override;

            ShortBuffer& get(gint index, ShortArray& dst, gint offset, gint length) override;

            gbool isReadOnly() const override;

            gbool isDirect() const override;

            ShortBuffer& put(gshort c) override;

            ShortBuffer& put(gint index, gshort c) override;

            ShortBuffer& put(const ShortArray& src, gint offset, gint length) override;

            ShortBuffer& put(gint index, const ShortArray& src, gint offset, gint length) override;

            ShortBuffer& put(gint index, const ShortArray& src) override;

            ShortBuffer& compact() override;

            gbool hasArray() const override;

            ShortArray& array() const override;

            gint arrayOffset() const override;

            String toString() const override;

            charset::Charset::ByteOrder order() const override;

        private:
            virtual gint ix(gint i) const;

            Object& base() const override;
        };

        /**
         * A read-only HeapShortBuffer.  This class extends the corresponding
         * read/write class, overriding the mutation methods to throw a
         * @b ReadOnlyBufferException and overriding the view-buffer methods to return an
         * instance of this class rather than of the superclass.
         */
        class ShortBuffer::ReadOnlyHeapShortBuffer final : public HeapShortBuffer {
        public:
            CORE_EXPLICIT ReadOnlyHeapShortBuffer(gint cap, gint lim, Object& segment);

            CORE_EXPLICIT ReadOnlyHeapShortBuffer(ShortArray& buf, gint off, gint len, Object& segment);

            CORE_EXPLICIT ReadOnlyHeapShortBuffer(ShortArray& buf,
                                                  gint mark, gint pos, gint lim, gint cap, gint off, Object& segment);

            ShortBuffer& slice() const override;

            ShortBuffer& slice(gint index, gint length) const override;

            ShortBuffer& duplicate() const override;

            ShortBuffer& asReadOnlyBuffer() const override;

            gbool isReadOnly() const override;

            ShortBuffer& put(gshort c) override;

            ShortBuffer& put(gint index, gshort c) override;

            ShortBuffer& put(const ShortArray& src, gint offset, gint length) override;

            ShortBuffer& put(const ShortArray& src) override;

            ShortBuffer& put(gint index, const ShortArray& src, gint offset, gint length) override;

            ShortBuffer& put(gint index, const ShortArray& src) override;

            ShortBuffer& compact() override;

            String toString() const override;

            gbool hasArray() const override;

            ShortArray& array() const override;

            gint arrayOffset() const override;
        };
    } // io
} // core

#endif //CORE24_SHORTBUFFER_H
