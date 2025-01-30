//
// Created by brunshweeck on 27 août 2024.
//

#include <core/IllegalArgumentException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/ShortBuffer.h>
#include <core/io/ReadOnlyBufferException.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace misc;
    using namespace util;
    using namespace charset;

    namespace io {
        ShortBuffer::ShortBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment)
            : Buffer(mark, position, limit, capacity, segment) {}

        Object& ShortBuffer::base() const {
            return null;
        }

        ShortBuffer& ShortBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("Negative capacity: "_S + capacity).throws($ftrace());
            try {
                return UNSAFE::newInstance<HeapShortBuffer>(capacity, capacity, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::wrap(ShortArray& array, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, array.length());
                return UNSAFE::newInstance<HeapShortBuffer>(array, offset, length, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::wrap(ShortArray& array) {
            try {
                return wrap(array, 0, array.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::get(ShortArray& dst, gint offset, gint length) {
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

        ShortBuffer& ShortBuffer::get(ShortArray& dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::get(gint index, ShortArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(index, length, limit());
                Preconditions::checkIndexFromRange(offset, length, dst.length());

                getArray(index, dst, offset, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        ShortBuffer& ShortBuffer::get(gint index, ShortArray& dst) {
            try {
                return get(index, dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::put(ShortBuffer& src) {
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

        ShortBuffer& ShortBuffer::put(gint index, ShortBuffer& src, gint offset, gint length) {
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

        ShortBuffer& ShortBuffer::put(ShortArray const& src, gint offset, gint length) {
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

        ShortBuffer& ShortBuffer::put(ShortArray const& src) {
            try {
                return put(src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::put(gint index, ShortArray const& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                putArray(index, src, offset, length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::put(gint index, ShortArray const& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ShortBuffer::position() const {
            try {
                return Buffer::position();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::position(gint newPosition) {
            try {
                Buffer::position(newPosition);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint ShortBuffer::limit() const {
            try {
                return Buffer::limit();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::limit(gint newLimit) {
            try {
                Buffer::limit(newLimit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ShortBuffer::mark() {
            try {
                Buffer::mark();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ShortBuffer::reset() {
            try {
                Buffer::reset();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ShortBuffer::clear() {
            try {
                Buffer::clear();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ShortBuffer::flip() {
            try {
                Buffer::flip();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ShortBuffer::rewind() {
            try {
                Buffer::rewind();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ShortBuffer::isAddressable() const {
            return false;
        }

        gint ShortBuffer::hash() const {
            gint h = 1;
            gint p = position();
            try {
                for (int i = limit() - 1; i >= p; --i)
                    h = 31 * h + get(i);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return h;
        }

        gbool ShortBuffer::equals(const Object& o) const {
            if (this == &o)
                return true;
            if (!Class<ShortBuffer>::hasInstance(o))
                return false;
            ShortBuffer const& that = CORE_XCAST(ShortBuffer const, o);
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

        gint ShortBuffer::compareTo(const ShortBuffer& that) const {
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
                    return Short::compare(get(thisPos + i), that.get(thatPos + i));
                }
                return thisRem - thatRem;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ShortBuffer::mismatch(ShortBuffer const& that) const {
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

        String ShortBuffer::toString() const {
            try {
                return "ShortBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ShortBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        void ShortBuffer::getArray(gint index, ShortArray& dst, gint offset, gint length) const {
            if (isAddressable() && CORE_CAST(glong, length) << 1 > 6) {
                glong bufAddr = address + (CORE_CAST(glong, index) << 1);
                glong dstOffset = UNSAFE::ARRAY_SHORT_BASE_OFFSET + (CORE_CAST(glong, offset) << 1);
                glong len = CORE_CAST(glong, length) << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Short::BYTES);
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

        ShortBuffer& ShortBuffer::putArray(gint index, ShortArray const& src, gint offset, gint length) {
            if (isAddressable() && (glong) length << 1 > 6) {
                glong bufAddr = address + ((glong) index << 1);
                glong srcOffset = UNSAFE::ARRAY_SHORT_BASE_OFFSET + ((glong) offset << 1);
                glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(src, srcOffset, base(), bufAddr, len, Short::BYTES);
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

        void ShortBuffer::putBuffer(gint pos, ShortBuffer& src, gint srcPos, gint n) {
            const Object& srcBase = src.base();

            Object& dstBase = base();

            CORE_ASSERT(dstBase != null || isDirect());

            glong srcAddr = src.address + (CORE_CAST(glong, srcPos) << 1);
            glong dstAddr = address + (CORE_CAST(glong, pos) << 1);
            glong len = CORE_CAST(glong, n) << 1;

            try {
                if (order() != src.order())
                    UNSAFE::copySwapMemory(srcBase, srcAddr, dstBase, dstAddr, len, 2);
                else
                    UNSAFE::copyMemory(srcBase, srcAddr, dstBase, dstAddr, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortArray& ShortBuffer::createHeap(gint heapSize) {
            try {
                return UNSAFE::newInstance<ShortArray>(heapSize);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer::HeapShortBuffer::HeapShortBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              ShortBuffer(-1, 0, lim, cap, segment), hb(createHeap(cap)) {}

        ShortBuffer::HeapShortBuffer::HeapShortBuffer(ShortArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              ShortBuffer(-1, off, off + len, buf.length(), segment), hb(buf) {}

        ShortBuffer::HeapShortBuffer::HeapShortBuffer(ShortArray& buf, gint mark, gint pos, gint lim, gint cap,
                                                      gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              ShortBuffer(mark, pos, lim, cap, segment), hb(buf), offset(off) {}

        ShortBuffer& ShortBuffer::HeapShortBuffer::slice() const {
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            return UNSAFE::newInstance<HeapShortBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                        segment == null ? null : *segment);
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<HeapShortBuffer>(hb, -1, 0, length, length, index + offset,
                                                            segment == null ? null : *segment);
            } catch (Throwable const& thr) {
                thr.throws($ftrace());
            }
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<HeapShortBuffer>(hb,
                                                            markValue(),
                                                            position(),
                                                            limit(),
                                                            capacity(),
                                                            offset,
                                                            segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapShortBuffer>(hb,
                                                                    markValue(),
                                                                    position(),
                                                                    limit(),
                                                                    capacity(),
                                                                    offset,
                                                                    segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gshort ShortBuffer::HeapShortBuffer::get() {
            try {
                return hb[ix(nextGetIndex())];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gshort ShortBuffer::HeapShortBuffer::get(gint index) const {
            try {
                return hb[ix(checkIndex(index))];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::get(ShortArray& dst, gint offset, gint length) {
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

        ShortBuffer& ShortBuffer::HeapShortBuffer::get(gint index, ShortArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
                Preconditions::checkIndexFromRange(index, length, limit());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            Arrays::copy(hb, ix(index), dst, offset, length);
            return *this;
        }

        gbool ShortBuffer::HeapShortBuffer::isReadOnly() const {
            return false;
        }

        gbool ShortBuffer::HeapShortBuffer::isDirect() const {
            return false;
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::put(gshort c) {
            try {
                hb[ix(nextGetIndex())] = c;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::put(gint index, gshort c) {
            try {
                hb[ix(checkIndex(index))] = c;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::put(const ShortArray& src, gint offset, gint length) {
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

        ShortBuffer& ShortBuffer::HeapShortBuffer::put(gint index, const ShortArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                Arrays::copy(src, offset, hb, ix(index), length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::put(gint index, const ShortArray& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::HeapShortBuffer::compact() {
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

        gbool ShortBuffer::HeapShortBuffer::hasArray() const {
            return true;
        }

        ShortArray& ShortBuffer::HeapShortBuffer::array() const {
            return hb;
        }

        gint ShortBuffer::HeapShortBuffer::arrayOffset() const {
            return offset;
        }

        String ShortBuffer::HeapShortBuffer::toString() const {
            try {
                return "HeapShortBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ShortBuffer::HeapShortBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        gint ShortBuffer::HeapShortBuffer::ix(gint i) const {
            return offset + i;
        }

        Object& ShortBuffer::HeapShortBuffer::base() const {
            return hb;
        }

        ShortBuffer::ReadOnlyHeapShortBuffer::ReadOnlyHeapShortBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              HeapShortBuffer(cap, lim, segment) {}

        ShortBuffer::ReadOnlyHeapShortBuffer::ReadOnlyHeapShortBuffer(ShortArray& buf, gint off, gint len,
                                                                      Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              HeapShortBuffer(buf, off, len, segment) {}

        ShortBuffer::ReadOnlyHeapShortBuffer::ReadOnlyHeapShortBuffer(ShortArray& buf, gint mark, gint pos, gint lim,
                                                                      gint cap, gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              HeapShortBuffer(buf, mark, pos, lim, cap, off, segment) {}

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos <= lim ? lim - pos : 0;
                return UNSAFE::newInstance<ReadOnlyHeapShortBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                                    segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyHeapShortBuffer>(hb, -1, 0, length, length, index + offset,
                                                                    segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapShortBuffer>(hb, markValue(), position(), limit(), capacity(),
                                                                    offset, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::asReadOnlyBuffer() const {
            return duplicate();
        }

        gbool ShortBuffer::ReadOnlyHeapShortBuffer::isReadOnly() const {
            return true;
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::put(gshort c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::put(gint index, gshort c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::put(const ShortArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::put(const ShortArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::
        put(gint index, const ShortArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::put(gint index, const ShortArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ShortBuffer::ReadOnlyHeapShortBuffer::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        String ShortBuffer::ReadOnlyHeapShortBuffer::toString() const {
            try {
                return "ReadOnlyHeapShortBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool ShortBuffer::ReadOnlyHeapShortBuffer::hasArray() const {
            return false;
        }

        ShortArray& ShortBuffer::ReadOnlyHeapShortBuffer::array() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        gint ShortBuffer::ReadOnlyHeapShortBuffer::arrayOffset() const {
            ReadOnlyBufferException().throws($ftrace());
        }
    } // io
} // core
