//
// Created by brunshweeck on 27 août 2024.
//

#include <core/lang/IllegalArgumentException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/LongBuffer.h>
#include <core/io/ReadOnlyBufferException.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace misc;
    using namespace util;
    using namespace charset;

    namespace io {
        LongBuffer::LongBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment)
            : Buffer(mark, position, limit, capacity, segment) {}

        Object& LongBuffer::base() const {
            return null;
        }

        LongBuffer& LongBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("Negative capacity: "_S + capacity).throws($ftrace());
            try {
                return UNSAFE::newInstance<HeapLongBuffer>(capacity, capacity, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::wrap(LongArray& array, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, array.length());
                return UNSAFE::newInstance<HeapLongBuffer>(array, offset, length, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::wrap(LongArray& array) {
            try {
                return wrap(array, 0, array.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::get(LongArray& dst, gint offset, gint length) {
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

        LongBuffer& LongBuffer::get(LongArray& dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::get(gint index, LongArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(index, length, limit());
                Preconditions::checkIndexFromRange(offset, length, dst.length());

                getArray(index, dst, offset, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        LongBuffer& LongBuffer::get(gint index, LongArray& dst) {
            try {
                return get(index, dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::put(LongBuffer& src) {
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

        LongBuffer& LongBuffer::put(gint index, LongBuffer& src, gint offset, gint length) {
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

        LongBuffer& LongBuffer::put(LongArray const& src, gint offset, gint length) {
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

        LongBuffer& LongBuffer::put(LongArray const& src) {
            try {
                return put(src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::put(gint index, LongArray const& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                putArray(index, src, offset, length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::put(gint index, LongArray const& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint LongBuffer::position() const {
            try {
                return Buffer::position();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::position(gint newPosition) {
            try {
                Buffer::position(newPosition);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint LongBuffer::limit() const {
            try {
                return Buffer::limit();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::limit(gint newLimit) {
            try {
                Buffer::limit(newLimit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& LongBuffer::mark() {
            try {
                Buffer::mark();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& LongBuffer::reset() {
            try {
                Buffer::reset();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& LongBuffer::clear() {
            try {
                Buffer::clear();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& LongBuffer::flip() {
            try {
                Buffer::flip();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& LongBuffer::rewind() {
            try {
                Buffer::rewind();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool LongBuffer::isAddressable() const {
            return false;
        }

        gint LongBuffer::hash() const {
            gint h = 1;
            gint p = position();
            try {
                for (int i = limit() - 1; i >= p; --i)
                    h = 31 * h + Long::hash(get(i));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return h;
        }

        gbool LongBuffer::equals(const Object& o) const {
            if (this == &o)
                return true;
            if (!Class<LongBuffer>::hasInstance(o))
                return false;
            LongBuffer const& that = CORE_XCAST(LongBuffer const, o);
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

        gint LongBuffer::compareTo(const LongBuffer& that) const {
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
                    return Long::compare(get(thisPos + i), that.get(thatPos + i));
                }
                return thisRem - thatRem;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint LongBuffer::mismatch(LongBuffer const& that) const {
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

        String LongBuffer::toString() const {
            try {
                return "LongBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder LongBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        void LongBuffer::getArray(gint index, LongArray& dst, gint offset, gint length) const {
            if (isAddressable() && (glong) length << 1 > 6) {
                const glong bufAddr = address + ((glong) index << 1);
                const glong dstOffset = UNSAFE::ARRAY_LONG_BASE_OFFSET + ((glong) offset << 1);
                const glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Long::BYTES);
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

        LongBuffer& LongBuffer::putArray(gint index, LongArray const& src, gint offset, gint length) {
            if (isAddressable() && (glong) length << 1 > 6) {
                const glong bufAddr = address + ((glong) index << 1);
                const glong srcOffset = UNSAFE::ARRAY_LONG_BASE_OFFSET + ((glong) offset << 1);
                const glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(src, srcOffset, base(), bufAddr, len, Long::BYTES);
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

        void LongBuffer::putBuffer(gint pos, LongBuffer& src, gint srcPos, gint n) {
            const Object& srcBase = src.base();

            Object& dstBase = base();

            CORE_ASSERT(dstBase != null || isDirect());

            const glong srcAddr = src.address + ((glong) srcPos << 1);
            const glong dstAddr = address + ((glong) pos << 1);
            const glong len = (glong) n << 1;

            try {
                if (order() != src.order())
                    UNSAFE::copySwapMemory(srcBase, srcAddr, dstBase, dstAddr, len, 2);
                else
                    UNSAFE::copyMemory(srcBase, srcAddr, dstBase, dstAddr, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongArray& LongBuffer::createHeap(gint heapSize) {
            try {
                return UNSAFE::newInstance<LongArray>(heapSize);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer::HeapLongBuffer::HeapLongBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              LongBuffer(-1, 0, lim, cap, segment), hb(createHeap(cap)) {}

        LongBuffer::HeapLongBuffer::HeapLongBuffer(LongArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              LongBuffer(-1, off, off + len, buf.length(), segment), hb(buf) {}

        LongBuffer::HeapLongBuffer::HeapLongBuffer(LongArray& buf, gint mark, gint pos, gint lim, gint cap,
                                                   gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              LongBuffer(mark, pos, lim, cap, segment), hb(buf), offset(off) {}

        LongBuffer& LongBuffer::HeapLongBuffer::slice() const {
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            return UNSAFE::newInstance<HeapLongBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                       segment == null ? null : *segment);
        }

        LongBuffer& LongBuffer::HeapLongBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<HeapLongBuffer>(hb, -1, 0, length, length, index + offset,
                                                           segment == null ? null : *segment);
            } catch (Throwable const& thr) {
                thr.throws($ftrace());
            }
        }

        LongBuffer& LongBuffer::HeapLongBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<HeapLongBuffer>(hb,
                                                           markValue(),
                                                           position(),
                                                           limit(),
                                                           capacity(),
                                                           offset,
                                                           segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::HeapLongBuffer::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapLongBuffer>(hb,
                                                                   markValue(),
                                                                   position(),
                                                                   limit(),
                                                                   capacity(),
                                                                   offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong LongBuffer::HeapLongBuffer::get() {
            try {
                return hb[ix(nextGetIndex())];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong LongBuffer::HeapLongBuffer::get(gint index) const {
            try {
                return hb[ix(checkIndex(index))];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::HeapLongBuffer::get(LongArray& dst, gint offset, gint length) {
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

        LongBuffer& LongBuffer::HeapLongBuffer::get(gint index, LongArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
                Preconditions::checkIndexFromRange(index, length, limit());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            Arrays::copy(hb, ix(index), dst, offset, length);
            return *this;
        }

        gbool LongBuffer::HeapLongBuffer::isReadOnly() const {
            return false;
        }

        gbool LongBuffer::HeapLongBuffer::isDirect() const {
            return false;
        }

        LongBuffer& LongBuffer::HeapLongBuffer::put(glong c) {
            hb[ix(nextGetIndex())] = c;
            return *this;
        }

        LongBuffer& LongBuffer::HeapLongBuffer::put(gint index, glong c) {
            hb[ix(checkIndex(index))] = c;
            return *this;
        }

        LongBuffer& LongBuffer::HeapLongBuffer::put(const LongArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = position();
            if (length > limit() - pos)
                BufferOverflowException().throws($ftrace());
            Arrays::copy(src, offset, hb, ix(pos), length);
            try {
                position(pos + length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& LongBuffer::HeapLongBuffer::put(gint index, const LongArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                Arrays::copy(src, offset, hb, ix(index), length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& LongBuffer::HeapLongBuffer::put(gint index, const LongArray& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::HeapLongBuffer::compact() {
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

        gbool LongBuffer::HeapLongBuffer::hasArray() const {
            return true;
        }

        LongArray& LongBuffer::HeapLongBuffer::array() const {
            return hb;
        }

        gint LongBuffer::HeapLongBuffer::arrayOffset() const {
            return offset;
        }

        String LongBuffer::HeapLongBuffer::toString() const {
            try {
                return "HeapLongBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder LongBuffer::HeapLongBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        gint LongBuffer::HeapLongBuffer::ix(gint i) const {
            return offset + i;
        }

        Object& LongBuffer::HeapLongBuffer::base() const {
            return hb;
        }

        LongBuffer::ReadOnlyHeapLongBuffer::ReadOnlyHeapLongBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              HeapLongBuffer(cap, lim, segment) {}

        LongBuffer::ReadOnlyHeapLongBuffer::ReadOnlyHeapLongBuffer(LongArray& buf, gint off, gint len,
                                                                   Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              HeapLongBuffer(buf, off, len, segment) {}

        LongBuffer::ReadOnlyHeapLongBuffer::ReadOnlyHeapLongBuffer(LongArray& buf, gint mark, gint pos, gint lim,
                                                                   gint cap, gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              HeapLongBuffer(buf, mark, pos, lim, cap, off, segment) {}

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos <= lim ? lim - pos : 0;
                return UNSAFE::newInstance<ReadOnlyHeapLongBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyHeapLongBuffer>(hb, -1, 0, length, length, index + offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapLongBuffer>(hb, markValue(), position(), limit(), capacity(),
                                                                   offset, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::asReadOnlyBuffer() const {
            return duplicate();
        }

        gbool LongBuffer::ReadOnlyHeapLongBuffer::isReadOnly() const {
            return true;
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::put(glong c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::put(gint index, glong c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::put(const LongArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::put(const LongArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::
        put(gint index, const LongArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::put(gint index, const LongArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& LongBuffer::ReadOnlyHeapLongBuffer::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        String LongBuffer::ReadOnlyHeapLongBuffer::toString() const {
            try {
                return "ReadOnlyHeapLongBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool LongBuffer::ReadOnlyHeapLongBuffer::hasArray() const {
            return false;
        }

        LongArray& LongBuffer::ReadOnlyHeapLongBuffer::array() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        gint LongBuffer::ReadOnlyHeapLongBuffer::arrayOffset() const {
            ReadOnlyBufferException().throws($ftrace());
        }
    } // io
} // core
