//
// Created by brunshweeck on 27 août 2024.
//

#include <core/IllegalArgumentException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/FloatBuffer.h>
#include <core/io/ReadOnlyBufferException.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace misc;
    using namespace util;
    using namespace charset;

    namespace io {
        FloatBuffer::FloatBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment)
            : Buffer(mark, position, limit, capacity, segment) {}

        Object& FloatBuffer::base() const {
            return null;
        }

        FloatBuffer& FloatBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("Negative capacity: "_S + capacity).throws($ftrace());
            try {
                return UNSAFE::newInstance<HeapFloatBuffer>(capacity, capacity, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::wrap(FloatArray& array, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, array.length());
                return UNSAFE::newInstance<HeapFloatBuffer>(array, offset, length, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::wrap(FloatArray& array) {
            try {
                return wrap(array, 0, array.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::get(FloatArray& dst, gint offset, gint length) {
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

        FloatBuffer& FloatBuffer::get(FloatArray& dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::get(gint index, FloatArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(index, length, limit());
                Preconditions::checkIndexFromRange(offset, length, dst.length());

                getArray(index, dst, offset, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        FloatBuffer& FloatBuffer::get(gint index, FloatArray& dst) {
            try {
                return get(index, dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::put(FloatBuffer& src) {
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

        FloatBuffer& FloatBuffer::put(gint index, FloatBuffer& src, gint offset, gint length) {
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

        FloatBuffer& FloatBuffer::put(FloatArray const& src, gint offset, gint length) {
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

        FloatBuffer& FloatBuffer::put(FloatArray const& src) {
            try {
                return put(src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::put(gint index, FloatArray const& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                putArray(index, src, offset, length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::put(gint index, FloatArray const& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint FloatBuffer::position() const {
            try {
                return Buffer::position();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::position(gint newPosition) {
            try {
                Buffer::position(newPosition);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint FloatBuffer::limit() const {
            try {
                return Buffer::limit();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::limit(gint newLimit) {
            try {
                Buffer::limit(newLimit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& FloatBuffer::mark() {
            try {
                Buffer::mark();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& FloatBuffer::reset() {
            try {
                Buffer::reset();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& FloatBuffer::clear() {
            try {
                Buffer::clear();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& FloatBuffer::flip() {
            try {
                Buffer::flip();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& FloatBuffer::rewind() {
            try {
                Buffer::rewind();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool FloatBuffer::isAddressable() const {
            return false;
        }

        gint FloatBuffer::hash() const {
            gint h = 1;
            gint p = position();
            try {
                for (int i = limit() - 1; i >= p; --i)
                    h = 31 * h + Float::hash(get(i));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return h;
        }

        gbool FloatBuffer::equals(const Object& o) const {
            if (this == &o)
                return true;
            if (!Class<FloatBuffer>::hasInstance(o))
                return false;
            FloatBuffer const& that = CORE_XCAST(FloatBuffer const, o);
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

        gint FloatBuffer::compareTo(const FloatBuffer& that) const {
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
                    return Float::compare(get(thisPos + i), that.get(thatPos + i));
                }
                return thisRem - thatRem;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint FloatBuffer::mismatch(FloatBuffer const& that) const {
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

        String FloatBuffer::toString() const {
            try {
                return "FloatBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder FloatBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        void FloatBuffer::getArray(gint index, FloatArray& dst, gint offset, gint length) const {
            if (isAddressable() && (glong) length << 1 > 6) {
                const glong bufAddr = address + ((glong) index << 1);
                const glong dstOffset = UNSAFE::ARRAY_FLOAT_BASE_OFFSET + ((glong) offset << 1);
                const glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Float::BYTES);
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

        FloatBuffer& FloatBuffer::putArray(gint index, FloatArray const& src, gint offset, gint length) {
            if (isAddressable() && (glong) length << 1 > 6) {
                const glong bufAddr = address + ((glong) index << 1);
                const glong srcOffset = UNSAFE::ARRAY_FLOAT_BASE_OFFSET + ((glong) offset << 1);
                const glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(src, srcOffset, base(), bufAddr, len, Float::BYTES);
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

        void FloatBuffer::putBuffer(gint pos, FloatBuffer& src, gint srcPos, gint n) {
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

        FloatArray& FloatBuffer::createHeap(gint heapSize) {
            try {
                return UNSAFE::newInstance<FloatArray>(heapSize);
            } catch (Throwable const& ex) { ex.throws($trace()); }
        }

        FloatBuffer::HeapFloatBuffer::HeapFloatBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              FloatBuffer(-1, 0, lim, cap, segment), hb(createHeap(cap)) {}

        FloatBuffer::HeapFloatBuffer::HeapFloatBuffer(FloatArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              FloatBuffer(-1, off, off + len, buf.length(), segment), hb(buf) {}

        FloatBuffer::HeapFloatBuffer::HeapFloatBuffer(FloatArray& buf, gint mark, gint pos, gint lim, gint cap,
                                                      gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              FloatBuffer(mark, pos, lim, cap, segment), hb(buf), offset(off) {}

        FloatBuffer& FloatBuffer::HeapFloatBuffer::slice() const {
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            return UNSAFE::newInstance<HeapFloatBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                        segment == null ? null : *segment);
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<HeapFloatBuffer>(hb, -1, 0, length, length, index + offset,
                                                            segment == null ? null : *segment);
            } catch (Throwable const& thr) {
                thr.throws($ftrace());
            }
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<HeapFloatBuffer>(hb,
                                                            markValue(),
                                                            position(),
                                                            limit(),
                                                            capacity(),
                                                            offset,
                                                            segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapFloatBuffer>(hb,
                                                                    markValue(),
                                                                    position(),
                                                                    limit(),
                                                                    capacity(),
                                                                    offset,
                                                                    segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gfloat FloatBuffer::HeapFloatBuffer::get() {
            try {
                return hb[ix(nextGetIndex())];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gfloat FloatBuffer::HeapFloatBuffer::get(gint index) const {
            try {
                return hb[ix(checkIndex(index))];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::get(FloatArray& dst, gint offset, gint length) {
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

        FloatBuffer& FloatBuffer::HeapFloatBuffer::get(gint index, FloatArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
                Preconditions::checkIndexFromRange(index, length, limit());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            Arrays::copy(hb, ix(index), dst, offset, length);
            return *this;
        }

        gbool FloatBuffer::HeapFloatBuffer::isReadOnly() const {
            return false;
        }

        gbool FloatBuffer::HeapFloatBuffer::isDirect() const {
            return false;
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::put(gfloat c) {
            hb[ix(nextGetIndex())] = c;
            return *this;
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::put(gint index, gfloat c) {
            hb[ix(checkIndex(index))] = c;
            return *this;
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::put(const FloatArray& src, gint offset, gint length) {
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

        FloatBuffer& FloatBuffer::HeapFloatBuffer::put(gint index, const FloatArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                Arrays::copy(src, offset, hb, ix(index), length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::put(gint index, const FloatArray& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::HeapFloatBuffer::compact() {
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

        gbool FloatBuffer::HeapFloatBuffer::hasArray() const {
            return true;
        }

        FloatArray& FloatBuffer::HeapFloatBuffer::array() const {
            return hb;
        }

        gint FloatBuffer::HeapFloatBuffer::arrayOffset() const {
            return offset;
        }

        String FloatBuffer::HeapFloatBuffer::toString() const {
            try {
                return "HeapFloatBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder FloatBuffer::HeapFloatBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        gint FloatBuffer::HeapFloatBuffer::ix(gint i) const {
            return offset + i;
        }

        Object& FloatBuffer::HeapFloatBuffer::base() const {
            return hb;
        }

        FloatBuffer::ReadOnlyHeapFloatBuffer::ReadOnlyHeapFloatBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              HeapFloatBuffer(cap, lim, segment) {}

        FloatBuffer::ReadOnlyHeapFloatBuffer::ReadOnlyHeapFloatBuffer(FloatArray& buf, gint off, gint len,
                                                                      Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              HeapFloatBuffer(buf, off, len, segment) {}

        FloatBuffer::ReadOnlyHeapFloatBuffer::ReadOnlyHeapFloatBuffer(FloatArray& buf, gint mark, gint pos, gint lim,
                                                                      gint cap, gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              HeapFloatBuffer(buf, mark, pos, lim, cap, off, segment) {}

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos <= lim ? lim - pos : 0;
                return UNSAFE::newInstance<ReadOnlyHeapFloatBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                                    segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyHeapFloatBuffer>(hb, -1, 0, length, length, index + offset,
                                                                    segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapFloatBuffer>(hb, markValue(), position(), limit(), capacity(),
                                                                    offset, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::asReadOnlyBuffer() const {
            return duplicate();
        }

        gbool FloatBuffer::ReadOnlyHeapFloatBuffer::isReadOnly() const {
            return true;
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::put(gfloat c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::put(gint index, gfloat c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::put(const FloatArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::put(const FloatArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::
        put(gint index, const FloatArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::put(gint index, const FloatArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& FloatBuffer::ReadOnlyHeapFloatBuffer::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        String FloatBuffer::ReadOnlyHeapFloatBuffer::toString() const {
            try {
                return "ReadOnlyHeapFloatBuffer[pos="_S + position()
                        + ", lim = "_S + limit()
                        + ", cap="_S + capacity()
                        + "]"_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool FloatBuffer::ReadOnlyHeapFloatBuffer::hasArray() const {
            return false;
        }

        FloatArray& FloatBuffer::ReadOnlyHeapFloatBuffer::array() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        gint FloatBuffer::ReadOnlyHeapFloatBuffer::arrayOffset() const {
            ReadOnlyBufferException().throws($ftrace());
        }
    } // io
} // core
