//
// Created by brunshweeck on 27 août 2024.
//

#include <core/IllegalArgumentException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/DoubleBuffer.h>
#include <core/io/ReadOnlyBufferException.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace misc;
    using namespace util;
    using namespace charset;

    namespace io {
        DoubleBuffer::DoubleBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment)
            : Buffer(mark, position, limit, capacity, segment) {}

        Object& DoubleBuffer::base() const {
            return null;
        }

        DoubleBuffer& DoubleBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("Negative capacity: "_S + capacity).throws($ftrace());
            try {
                return UNSAFE::newInstance<HeapDoubleBuffer>(capacity, capacity, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::wrap(DoubleArray& array, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, array.length());
                return UNSAFE::newInstance<HeapDoubleBuffer>(array, offset, length, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::wrap(DoubleArray& array) {
            try {
                return wrap(array, 0, array.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::get(DoubleArray& dst, gint offset, gint length) {
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

        DoubleBuffer& DoubleBuffer::get(DoubleArray& dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::get(gint index, DoubleArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(index, length, limit());
                Preconditions::checkIndexFromRange(offset, length, dst.length());

                getArray(index, dst, offset, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        DoubleBuffer& DoubleBuffer::get(gint index, DoubleArray& dst) {
            try {
                return get(index, dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::put(DoubleBuffer& src) {
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

        DoubleBuffer& DoubleBuffer::put(gint index, DoubleBuffer& src, gint offset, gint length) {
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

        DoubleBuffer& DoubleBuffer::put(DoubleArray const& src, gint offset, gint length) {
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

        DoubleBuffer& DoubleBuffer::put(DoubleArray const& src) {
            try {
                return put(src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::put(gint index, DoubleArray const& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                putArray(index, src, offset, length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::put(gint index, DoubleArray const& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint DoubleBuffer::position() const {
            try {
                return Buffer::position();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::position(gint newPosition) {
            try {
                Buffer::position(newPosition);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint DoubleBuffer::limit() const {
            try {
                return Buffer::limit();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::limit(gint newLimit) {
            try {
                Buffer::limit(newLimit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& DoubleBuffer::mark() {
            try {
                Buffer::mark();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& DoubleBuffer::reset() {
            try {
                Buffer::reset();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& DoubleBuffer::clear() {
            try {
                Buffer::clear();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& DoubleBuffer::flip() {
            try {
                Buffer::flip();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& DoubleBuffer::rewind() {
            try {
                Buffer::rewind();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool DoubleBuffer::isAddressable() const {
            return false;
        }

        gint DoubleBuffer::hash() const {
            gint h = 1;
            gint p = position();
            try {
                for (int i = limit() - 1; i >= p; --i)
                    h = 31 * h + Double::hash(get(i));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return h;
        }

        gbool DoubleBuffer::equals(const Object& o) const {
            if (this == &o)
                return true;
            if (!Class<DoubleBuffer>::hasInstance(o))
                return false;
            DoubleBuffer const& that = CORE_XCAST(DoubleBuffer const, o);
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

        gint DoubleBuffer::compareTo(const DoubleBuffer& that) const {
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
                    return Double::compare(get(thisPos + i), that.get(thatPos + i));
                }
                return thisRem - thatRem;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint DoubleBuffer::mismatch(DoubleBuffer const& that) const {
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

        String DoubleBuffer::toString() const {
            try {
                return "DoubleBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder DoubleBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        void DoubleBuffer::getArray(gint index, DoubleArray& dst, gint offset, gint length) const {
            if (isAddressable() && (glong) length << 1 > 6) {
                glong bufAddr = address + ((glong) index << 1);
                glong dstOffset = UNSAFE::ARRAY_DOUBLE_BASE_OFFSET + ((glong) offset << 1);
                glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Double::BYTES);
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

        DoubleBuffer& DoubleBuffer::putArray(gint index, DoubleArray const& src, gint offset, gint length) {
            if (isAddressable() && (glong) length << 1 > 6) {
                glong bufAddr = address + ((glong) index << 1);
                glong srcOffset = UNSAFE::ARRAY_DOUBLE_BASE_OFFSET + ((glong) offset << 1);
                glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(src, srcOffset, base(), bufAddr, len, Double::BYTES);
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

        void DoubleBuffer::putBuffer(gint pos, DoubleBuffer& src, gint srcPos, gint n) {
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

        DoubleArray& DoubleBuffer::createHeap(gint heapSize) {
            try {
                return UNSAFE::newInstance<DoubleArray>(heapSize);
            } catch (Throwable const& ex) { ex.throws($trace()); }
        }

        DoubleBuffer::HeapDoubleBuffer::HeapDoubleBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              DoubleBuffer(-1, 0, lim, cap, segment), hb(createHeap(cap)) {}

        DoubleBuffer::HeapDoubleBuffer::HeapDoubleBuffer(DoubleArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              DoubleBuffer(-1, off, off + len, buf.length(), segment), hb(buf) {}

        DoubleBuffer::HeapDoubleBuffer::HeapDoubleBuffer(DoubleArray& buf, gint mark, gint pos, gint lim, gint cap,
                                                         gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              DoubleBuffer(mark, pos, lim, cap, segment), hb(buf), offset(off) {}

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::slice() const {
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            return UNSAFE::newInstance<HeapDoubleBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                         segment == null ? null : *segment);
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<HeapDoubleBuffer>(hb, -1, 0, length, length, index + offset,
                                                             segment == null ? null : *segment);
            } catch (Throwable const& thr) {
                thr.throws($ftrace());
            }
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<HeapDoubleBuffer>(hb,
                                                             markValue(),
                                                             position(),
                                                             limit(),
                                                             capacity(),
                                                             offset,
                                                             segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapDoubleBuffer>(hb,
                                                                     markValue(),
                                                                     position(),
                                                                     limit(),
                                                                     capacity(),
                                                                     offset,
                                                                     segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gdouble DoubleBuffer::HeapDoubleBuffer::get() {
            try {
                return hb[ix(nextGetIndex())];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gdouble DoubleBuffer::HeapDoubleBuffer::get(gint index) const {
            try {
                return hb[ix(checkIndex(index))];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::get(DoubleArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = position();
            if (length > limit() - pos)
                BufferUnderflowException().throws($ftrace());
            Arrays::copy(hb, ix(pos), dst, offset, length);
            position(pos + length);
            return *this;
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::get(gint index, DoubleArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
                Preconditions::checkIndexFromRange(index, length, limit());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            Arrays::copy(hb, ix(index), dst, offset, length);
            return *this;
        }

        gbool DoubleBuffer::HeapDoubleBuffer::isReadOnly() const {
            return false;
        }

        gbool DoubleBuffer::HeapDoubleBuffer::isDirect() const {
            return false;
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::put(gdouble c) {
            hb[ix(nextGetIndex())] = c;
            return *this;
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::put(gint index, gdouble c) {
            hb[ix(checkIndex(index))] = c;
            return *this;
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::put(const DoubleArray& src, gint offset, gint length) {
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

        DoubleBuffer&
        DoubleBuffer::HeapDoubleBuffer::put(gint index, const DoubleArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                Arrays::copy(src, offset, hb, ix(index), length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::put(gint index, const DoubleArray& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::HeapDoubleBuffer::compact() {
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

        gbool DoubleBuffer::HeapDoubleBuffer::hasArray() const {
            return true;
        }

        DoubleArray& DoubleBuffer::HeapDoubleBuffer::array() const {
            return hb;
        }

        gint DoubleBuffer::HeapDoubleBuffer::arrayOffset() const {
            return offset;
        }

        String DoubleBuffer::HeapDoubleBuffer::toString() const {
            try {
                return "HeapDoubleBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder DoubleBuffer::HeapDoubleBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        gint DoubleBuffer::HeapDoubleBuffer::ix(gint i) const {
            return offset + i;
        }

        Object& DoubleBuffer::HeapDoubleBuffer::base() const {
            return hb;
        }

        DoubleBuffer::ReadOnlyHeapDoubleBuffer::ReadOnlyHeapDoubleBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              HeapDoubleBuffer(cap, lim, segment) {}

        DoubleBuffer::ReadOnlyHeapDoubleBuffer::ReadOnlyHeapDoubleBuffer(DoubleArray& buf, gint off, gint len,
                                                                         Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              HeapDoubleBuffer(buf, off, len, segment) {}

        DoubleBuffer::ReadOnlyHeapDoubleBuffer::ReadOnlyHeapDoubleBuffer(
            DoubleArray& buf, gint mark, gint pos, gint lim,
            gint cap, gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              HeapDoubleBuffer(buf, mark, pos, lim, cap, off, segment) {}

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos <= lim ? lim - pos : 0;
                return UNSAFE::newInstance<ReadOnlyHeapDoubleBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                                     segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<
                    ReadOnlyHeapDoubleBuffer>(hb, -1, 0, length, length, index + offset,
                                              segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapDoubleBuffer>(hb, markValue(), position(), limit(), capacity(),
                                                                     offset, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::asReadOnlyBuffer() const {
            return duplicate();
        }

        gbool DoubleBuffer::ReadOnlyHeapDoubleBuffer::isReadOnly() const {
            return true;
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::put(gdouble c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::put(gint index, gdouble c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::put(const DoubleArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::put(const DoubleArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::
        put(gint index, const DoubleArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::put(gint index, const DoubleArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& DoubleBuffer::ReadOnlyHeapDoubleBuffer::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        String DoubleBuffer::ReadOnlyHeapDoubleBuffer::toString() const {
            try {
                return "ReadOnlyHeapDoubleBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool DoubleBuffer::ReadOnlyHeapDoubleBuffer::hasArray() const {
            return false;
        }

        DoubleArray& DoubleBuffer::ReadOnlyHeapDoubleBuffer::array() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        gint DoubleBuffer::ReadOnlyHeapDoubleBuffer::arrayOffset() const {
            ReadOnlyBufferException().throws($ftrace());
        }
    } // io
} // core
