//
// Created by brunshweeck on 27 août 2024.
//

#include <core/lang/IllegalArgumentException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/IntBuffer.h>
#include <core/io/ReadOnlyBufferException.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace misc;
    using namespace util;
    using namespace charset;

    namespace io {
        IntBuffer::IntBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment)
            : Buffer(mark, position, limit, capacity, segment) {}

        Object& IntBuffer::base() const {
            return null;
        }

        IntBuffer& IntBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("Negative capacity: "_S + capacity).throws($ftrace());
            try {
                return UNSAFE::newInstance<HeapIntBuffer>(capacity, capacity, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::wrap(IntArray& array, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, array.length());
                return UNSAFE::newInstance<HeapIntBuffer>(array, offset, length, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::wrap(IntArray& array) {
            try {
                return wrap(array, 0, array.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::get(IntArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
                gint pos = position();
                if (length > limit() - pos)
                    BufferOverflowException().throws($ftrace());

                getArray(pos, dst, offset, length);
                position(pos + length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        IntBuffer& IntBuffer::get(IntArray& dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::get(gint index, IntArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(index, length, limit());
                Preconditions::checkIndexFromRange(offset, length, dst.length());

                getArray(index, dst, offset, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        IntBuffer& IntBuffer::get(gint index, IntArray& dst) {
            try {
                return get(index, dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::put(IntBuffer& src) {
            if (&src == this)
                IllegalArgumentException("The source buffer is this buffer"_S).throws(
                    $ftrace());
            if (isReadOnly())
                ReadOnlyBufferException().throws($ftrace());

            gint srcPos = src.position();
            gint srcLim = src.limit();
            gint srcRem = srcPos <= srcLim ? srcLim - srcPos : 0;
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;

            if (srcRem > rem)
                BufferOverflowException().throws($ftrace());

            try {
                putBuffer(pos, src, srcPos, srcRem);

                position(pos + srcRem);
                src.position(srcPos + srcRem);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& IntBuffer::put(gint index, IntBuffer& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(index, length, limit());
                Preconditions::checkIndexFromRange(offset, length, src.limit());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (isReadOnly())
                ReadOnlyBufferException().throws($ftrace());
            try {
                putBuffer(index, src, offset, length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::put(IntArray const& src, gint offset, gint length) {
            if (isReadOnly())
                ReadOnlyBufferException().throws($ftrace());
            try {
                Preconditions::checkIndexFromSize(offset, length, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws($ftrace());
            try {
                putArray(pos, src, offset, length);
                position(pos + length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::put(IntArray const& src) {
            try {
                return put(src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::put(gint index, IntArray const& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                putArray(index, src, offset, length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::put(gint index, IntArray const& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint IntBuffer::position() const {
            try {
                return Buffer::position();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::position(gint newPosition) {
            try {
                Buffer::position(newPosition);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint IntBuffer::limit() const {
            try {
                return Buffer::limit();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::limit(gint newLimit) {
            try {
                Buffer::limit(newLimit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& IntBuffer::mark() {
            try {
                Buffer::mark();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& IntBuffer::reset() {
            try {
                Buffer::reset();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& IntBuffer::clear() {
            try {
                Buffer::clear();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& IntBuffer::flip() {
            try {
                Buffer::flip();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& IntBuffer::rewind() {
            try {
                Buffer::rewind();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool IntBuffer::isAddressable() const {
            return false;
        }

        gint IntBuffer::hash() const {
            gint h = 1;
            gint p = position();
            try {
                for (int i = limit() - 1; i >= p; --i)
                    h = 31 * h + get(i);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return h;
        }

        gbool IntBuffer::equals(const Object& o) const {
            if (this == &o)
                return true;
            if (!Class<IntBuffer>::hasInstance(o))
                return false;
            IntBuffer const& that = CORE_XCAST(IntBuffer const, o);
            try {
                gint thisPos = position();
                gint thisRem = limit() - thisPos;
                gint thatPos = that.position();
                gint thatRem = that.limit() - thatPos;
                if (thisRem < 0 || thisRem != thatRem)
                    return false;
                return Buffer::mismatch(*this, thisPos,
                                        that, thatPos,
                                        thisRem) < 0;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint IntBuffer::compareTo(const IntBuffer& that) const {
            if (this == &that)
                return 0;
            try {
                gint thisPos = position();
                gint thisRem = limit() - thisPos;
                gint thatPos = that.position();
                gint thatRem = that.limit() - thatPos;
                gint length = Math::min(thisRem, thatRem);
                if (length < 0)
                    return -1;
                gint i = Buffer::mismatch(*this, thisPos,
                                          that, thatPos,
                                          length);
                if (i >= 0) {
                    return Integer::compare(get(thisPos + i), that.get(thatPos + i));
                }
                return thisRem - thatRem;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint IntBuffer::mismatch(IntBuffer const& that) const {
            try {
                gint thisPos = position();
                gint thisRem = limit() - thisPos;
                gint thatPos = that.position();
                gint thatRem = that.limit() - thatPos;
                gint length = Math::min(thisRem, thatRem);
                if (length < 0)
                    return -1;
                gint r = Buffer::mismatch(*this, thisPos,
                                          that, thatPos,
                                          length);
                return r == -1 && thisRem != thatRem ? length : r;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String IntBuffer::toString() const {
            try {
                return "IntBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder IntBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        void IntBuffer::getArray(gint index, IntArray& dst, gint offset, gint length) const {
            if (isAddressable() && (glong) length << 1 > 6) {
                glong bufAddr = address + ((glong) index << 1);
                glong dstOffset = UNSAFE::ARRAY_INT_BASE_OFFSET + ((glong) offset << 1);
                glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Integer::BYTES);
                    else
                        UNSAFE::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            } else {
                try {
                    for (int i = offset, j = index, end = offset + length; i < end; i++, j++)
                        dst[i] = get(j);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        }

        IntBuffer& IntBuffer::putArray(gint index, IntArray const& src, gint offset, gint length) {
            if (isAddressable() && (glong) length << 1 > 6) {
                glong bufAddr = address + ((glong) index << 1);
                glong srcOffset = UNSAFE::ARRAY_INT_BASE_OFFSET + ((glong) offset << 1);
                glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(src, srcOffset, base(), bufAddr, len, Integer::BYTES);
                    else
                        UNSAFE::copyMemory(src, srcOffset, base(), bufAddr, len);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            } else {
                try {
                    for (int i = offset, j = index, end = offset + length; i < end; i++, j++)
                        put(j, src[i]);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
            return *this;
        }

        void IntBuffer::putBuffer(gint pos, IntBuffer& src, gint srcPos, gint n) {
            const Object& srcBase = src.base();

            Object& dstBase = base();

            CORE_ASSERT(dstBase != null || isDirect());

            glong srcAddr = src.address + ((glong) srcPos << 1);
            glong dstAddr = address + ((glong) pos << 1);
            glong len = (glong) n << 1;

            try {
                if (order() != src.order())
                    UNSAFE::copySwapMemory(srcBase, srcAddr, dstBase, dstAddr, len, 2);
                else
                    UNSAFE::copyMemory(srcBase, srcAddr, dstBase, dstAddr, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntArray& IntBuffer::createHeap(gint heapSize) {
            try {
                return UNSAFE::newInstance<IntArray>(heapSize);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer::HeapIntBuffer::HeapIntBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              IntBuffer(-1, 0, lim, cap, segment), hb(createHeap(cap)) {}

        IntBuffer::HeapIntBuffer::HeapIntBuffer(IntArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              IntBuffer(-1, off, off + len, buf.length(), segment), hb(buf) {}

        IntBuffer::HeapIntBuffer::HeapIntBuffer(IntArray& buf, gint mark, gint pos, gint lim, gint cap,
                                                gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              IntBuffer(mark, pos, lim, cap, segment), hb(buf), offset(off) {}

        IntBuffer& IntBuffer::HeapIntBuffer::slice() const {
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            return UNSAFE::newInstance<HeapIntBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                      segment == null ? null : *segment);
        }

        IntBuffer& IntBuffer::HeapIntBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<HeapIntBuffer>(hb, -1, 0, length, length, index + offset,
                                                          segment == null ? null : *segment);
            } catch (Throwable const& thr) {
                thr.throws($ftrace());
            }
        }

        IntBuffer& IntBuffer::HeapIntBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<HeapIntBuffer>(hb,
                                                          markValue(),
                                                          position(),
                                                          limit(),
                                                          capacity(),
                                                          offset,
                                                          segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::HeapIntBuffer::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapIntBuffer>(hb,
                                                                  markValue(),
                                                                  position(),
                                                                  limit(),
                                                                  capacity(),
                                                                  offset,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint IntBuffer::HeapIntBuffer::get() {
            try {
                return hb[ix(nextGetIndex())];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint IntBuffer::HeapIntBuffer::get(gint index) const {
            try {
                return hb[ix(checkIndex(index))];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::HeapIntBuffer::get(IntArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws($ftrace());
            Arrays::copy(hb, ix(pos), dst, offset, length);
            try {
                position(pos + length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& IntBuffer::HeapIntBuffer::get(gint index, IntArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
                Preconditions::checkIndexFromRange(index, length, limit());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            Arrays::copy(hb, ix(index), dst, offset, length);
            return *this;
        }

        gbool IntBuffer::HeapIntBuffer::isReadOnly() const {
            return false;
        }

        gbool IntBuffer::HeapIntBuffer::isDirect() const {
            return false;
        }

        IntBuffer& IntBuffer::HeapIntBuffer::put(gint c) {
            hb[ix(nextGetIndex())] = c;
            return *this;
        }

        IntBuffer& IntBuffer::HeapIntBuffer::put(gint index, gint c) {
            hb[ix(checkIndex(index))] = c;
            return *this;
        }

        IntBuffer& IntBuffer::HeapIntBuffer::put(const IntArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws($ftrace());
            Arrays::copy(src, offset, hb, ix(pos), length);
            position(pos + length);
            return *this;
        }

        IntBuffer& IntBuffer::HeapIntBuffer::put(gint index, const IntArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                Arrays::copy(src, offset, hb, ix(index), length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& IntBuffer::HeapIntBuffer::put(gint index, const IntArray& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::HeapIntBuffer::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;
            try {
                Arrays::copy(hb, ix(pos), hb, ix(0), rem);
                position(rem);
                limit(capacity());
                discardMark();
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool IntBuffer::HeapIntBuffer::hasArray() const {
            return true;
        }

        IntArray& IntBuffer::HeapIntBuffer::array() const {
            return hb;
        }

        gint IntBuffer::HeapIntBuffer::arrayOffset() const {
            return offset;
        }

        String IntBuffer::HeapIntBuffer::toString() const {
            try {
                return "HeapIntBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder IntBuffer::HeapIntBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        gint IntBuffer::HeapIntBuffer::ix(gint i) const {
            return offset + i;
        }

        Object& IntBuffer::HeapIntBuffer::base() const {
            return hb;
        }

        IntBuffer::ReadOnlyHeapIntBuffer::ReadOnlyHeapIntBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              HeapIntBuffer(cap, lim, segment) {}

        IntBuffer::ReadOnlyHeapIntBuffer::ReadOnlyHeapIntBuffer(IntArray& buf, gint off, gint len,
                                                                Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              HeapIntBuffer(buf, off, len, segment) {}

        IntBuffer::ReadOnlyHeapIntBuffer::ReadOnlyHeapIntBuffer(IntArray& buf, gint mark, gint pos, gint lim,
                                                                gint cap, gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              HeapIntBuffer(buf, mark, pos, lim, cap, off, segment) {}

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos <= lim ? lim - pos : 0;
                return UNSAFE::newInstance<ReadOnlyHeapIntBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyHeapIntBuffer>(hb, -1, 0, length, length, index + offset,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapIntBuffer>(hb, markValue(), position(), limit(), capacity(),
                                                                  offset, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::asReadOnlyBuffer() const {
            return duplicate();
        }

        gbool IntBuffer::ReadOnlyHeapIntBuffer::isReadOnly() const {
            return true;
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::put(gint c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::put(gint index, gint c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::put(const IntArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::put(const IntArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::
        put(gint index, const IntArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::put(gint index, const IntArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& IntBuffer::ReadOnlyHeapIntBuffer::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        String IntBuffer::ReadOnlyHeapIntBuffer::toString() const {
            try {
                return "ReadOnlyHeapIntBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool IntBuffer::ReadOnlyHeapIntBuffer::hasArray() const {
            return false;
        }

        IntArray& IntBuffer::ReadOnlyHeapIntBuffer::array() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        gint IntBuffer::ReadOnlyHeapIntBuffer::arrayOffset() const {
            ReadOnlyBufferException().throws($ftrace());
        }
    } // io
} // core
