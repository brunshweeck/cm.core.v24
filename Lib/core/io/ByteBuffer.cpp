//
// Created by brunshweeck on 27 août 2024.
//

#include "ByteBuffer.h"

#include <core/IllegalArgumentException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/ReadOnlyBufferException.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace misc;
    using namespace util;
    using namespace charset;

    namespace io {
        ByteBuffer::ByteBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment)
            : Buffer(mark, position, limit, capacity, segment) {}

        ByteBuffer& ByteBuffer::allocateDirect(gint capacity) {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::base() const {
            return null;
        }

        void ByteBuffer::getArray(gint index, ByteArray& dst, gint offset, gint length) const {
            if ((glong) length << 0 > 6) {
                glong bufAddr = address + ((glong) index << 0);
                glong dstOffset = UNSAFE::ARRAY_CHAR_BASE_OFFSET + ((glong) offset << 0);
                glong len = (glong) length << 0;
                try {
                    UNSAFE::copyMemory(base(), bufAddr, dst, dstOffset, len);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            } else {
                try {
                    for (int i = offset, j = index, end = offset + length; i < end; i++, j++)
                        dst[i] = get(j);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        }

        ByteBuffer& ByteBuffer::putArray(gint index, ByteArray const& src, gint offset, gint length) {
            if ((glong) length << 0 > 6) {
                glong bufAddr = address + ((glong) index << 0);
                glong srcOffset = UNSAFE::ARRAY_CHAR_BASE_OFFSET + ((glong) offset << 0);
                glong len = (glong) length << 0;
                try {
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

        void ByteBuffer::putBuffer(gint pos, ByteBuffer& src, gint srcPos, gint n) {
            const Object& srcBase = src.base();
            Object& dstBase = base();

            CORE_ASSERT(dstBase != null || isDirect());

            glong srcAddr = src.address + ((glong) srcPos << 0);
            glong dstAddr = address + ((glong) pos << 0);
            glong len = (glong) n << 0;

            try {
                if (order() != src.order())
                    UNSAFE::copySwapMemory(srcBase, srcAddr, dstBase, dstAddr, len, 2);
                else
                    UNSAFE::copyMemory(srcBase, srcAddr, dstBase, dstAddr, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteArray& ByteBuffer::createHeap(gint heapSize) {
            try {
                return UNSAFE::newInstance<ByteArray>(heapSize);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("Negative capacity: "_S + capacity).throws($ftrace());
            try {
                return UNSAFE::newInstance<HeapByteBuffer>(capacity, capacity, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::wrap(ByteArray& array, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, array.length());
                return UNSAFE::newInstance<HeapByteBuffer>(array, offset, length, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::wrap(ByteArray& array) {
            try {
                return wrap(array, 0, array.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::get(ByteArray& dst, gint offset, gint length) {
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

        ByteBuffer& ByteBuffer::get(ByteArray& dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::get(gint index, ByteArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(index, length, limit());
                Preconditions::checkIndexFromRange(offset, length, dst.length());

                getArray(index, dst, offset, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        ByteBuffer& ByteBuffer::get(gint index, ByteArray& dst) {
            try {
                return get(index, dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::put(ByteBuffer& src) {
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

        ByteBuffer& ByteBuffer::put(gint index, ByteBuffer& src, gint offset, gint length) {
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

        ByteBuffer& ByteBuffer::put(ByteArray const& src, gint offset, gint length) {
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

        ByteBuffer& ByteBuffer::put(ByteArray const& src) {
            try {
                return put(src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::put(gint index, ByteArray const& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                putArray(index, src, offset, length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::put(gint index, ByteArray const& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::position() const {
            try {
                return Buffer::position();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::position(gint newPosition) {
            try {
                Buffer::position(newPosition);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint ByteBuffer::limit() const {
            try {
                return Buffer::limit();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::limit(gint newLimit) {
            try {
                Buffer::limit(newLimit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ByteBuffer& ByteBuffer::mark() {
            try {
                Buffer::mark();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ByteBuffer& ByteBuffer::reset() {
            try {
                Buffer::reset();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ByteBuffer& ByteBuffer::clear() {
            try {
                Buffer::clear();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ByteBuffer& ByteBuffer::flip() {
            try {
                Buffer::flip();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ByteBuffer& ByteBuffer::rewind() {
            try {
                Buffer::rewind();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        String ByteBuffer::toString() const {
            try {
                return "ByteBuffer[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::hash() const {
            gint h = 1;
            gint const p = position();
            try {
                for (int i = limit() - 1; i >= p; --i)
                    h = 31 * h + get(i);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return h;
        }

        gbool ByteBuffer::equals(const Object& o) const {
            if (this == &o)
                return true;
            if (!Class<ByteBuffer>::hasInstance(o))
                return false;
            ByteBuffer const& that = CORE_XCAST(ByteBuffer const, o);
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

        gint ByteBuffer::compareTo(const ByteBuffer& that) const {
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
                    return Byte::compare(get(thisPos + i), that.get(thatPos + i));
                }
                return thisRem - thatRem;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::mismatch(ByteBuffer const& that) const {
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

        Charset::ByteOrder ByteBuffer::order() const {
            return bigEndian ? Charset::ByteOrder::BIG_ENDIAN : Charset::ByteOrder::LITTLE_ENDIAN;
        }

        ByteBuffer& ByteBuffer::order(Charset::ByteOrder bo) {
            bigEndian = bo == Charset::ByteOrder::BIG_ENDIAN;
            nativeByteOrder = bigEndian == (Charset::ByteOrder::NATIVE_ENDIAN ==
                Charset::ByteOrder::BIG_ENDIAN);
            return *this;
        }

        gint ByteBuffer::alignmentOffset(gint index, gint unitSize) const {
            if (index < 0)
                IllegalArgumentException("Index less than zero: "_S + index)
                        .throws($ftrace());
            if (unitSize < 1 || (unitSize & unitSize - 1) != 0)
                IllegalArgumentException("Unit size not a power of two: "_S + unitSize)
                        .throws($ftrace());
            if (unitSize > 8 && !isDirect())
                UnsupportedOperationException("Unit size unsupported for non-direct buffers: "_S + unitSize)
                        .throws($ftrace());

            return (gint) (address + index & unitSize - 1);
        }

        ByteBuffer& ByteBuffer::alignedSlice(gint unitSize) const {
            try {
                gint pos = position();
                gint lim = limit();

                gint posMod = alignmentOffset(pos, unitSize);
                gint limMod = alignmentOffset(lim, unitSize);

                // Round up the position to align with unit size
                gint alignedPos = posMod > 0
                                      ? pos + (unitSize - posMod)
                                      : pos;

                // Round down the limit to align with unit size
                gint alignedLim = lim - limMod;

                if (alignedPos > lim || alignedLim < pos) {
                    alignedPos = alignedLim = pos;
                }

                return slice(alignedPos, alignedLim - alignedPos);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer::HeapByteBuffer::HeapByteBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              ByteBuffer(-1, 0, lim, cap, segment), hb(createHeap(cap)) {
            address = UNSAFE::ARRAY_BYTE_BASE_OFFSET;
        }

        ByteBuffer::HeapByteBuffer::HeapByteBuffer(ByteArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              ByteBuffer(-1, off, off + len, buf.length(), segment), hb(buf) {
            address = UNSAFE::ARRAY_BYTE_BASE_OFFSET;
        }

        ByteBuffer::HeapByteBuffer::HeapByteBuffer(ByteArray& buf, gint mark, gint pos, gint lim, gint cap,
                                                   gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              ByteBuffer(mark, pos, lim, cap, segment), hb(buf), offset(off) {
            address = UNSAFE::ARRAY_BYTE_BASE_OFFSET;
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::slice() const {
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            return UNSAFE::newInstance<HeapByteBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                       segment == null ? null : *segment);
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<HeapByteBuffer>(hb, -1, 0, length, length, index + offset,
                                                           segment == null ? null : *segment);
            } catch (Throwable const& thr) {
                thr.throws($ftrace());
            }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<HeapByteBuffer>(hb,
                                                           markValue(),
                                                           position(),
                                                           limit(),
                                                           capacity(),
                                                           offset,
                                                           segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapByteBuffer>(hb,
                                                                   markValue(),
                                                                   position(),
                                                                   limit(),
                                                                   capacity(),
                                                                   offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbyte ByteBuffer::HeapByteBuffer::get() {
            try {
                return hb[ix(nextGetIndex())];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbyte ByteBuffer::HeapByteBuffer::get(gint index) const {
            try {
                return hb[ix(checkIndex(index))];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::get(ByteArray& dst, gint offset, gint length) {
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

        ByteBuffer& ByteBuffer::HeapByteBuffer::get(gint index, ByteArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
                Preconditions::checkIndexFromRange(index, length, limit());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            Arrays::copy(hb, ix(index), dst, offset, length);
            return *this;
        }

        gbool ByteBuffer::HeapByteBuffer::isReadOnly() const {
            return false;
        }

        gbool ByteBuffer::HeapByteBuffer::isDirect() const {
            return false;
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::put(gbyte c) {
            try {
                hb[ix(nextGetIndex())] = c;
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::put(gint index, gbyte c) {
            try {
                hb[ix(checkIndex(index))] = c;
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::put(const ByteArray& src, gint offset, gint length) {
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

        ByteBuffer& ByteBuffer::HeapByteBuffer::put(gint index, const ByteArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                Arrays::copy(src, offset, hb, ix(index), length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::put(gint index, const ByteArray& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::compact() {
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

        gchar ByteBuffer::HeapByteBuffer::getChar() {
            try {
                return UNSAFE::getCharUnaligned(hb, byteOffset(nextGetIndex(2)), bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putChar(gchar value) {
            try {
                UNSAFE::putCharUnaligned(hb, byteOffset(nextGetIndex(2)), value, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gchar ByteBuffer::HeapByteBuffer::getChar(gint index) const {
            try {
                return UNSAFE::getCharUnaligned(hb, byteOffset(checkIndex(index, 2)), bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putChar(gint index, gchar value) {
            try {
                UNSAFE::putCharUnaligned(hb, byteOffset(checkIndex(index, 2)), value, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& ByteBuffer::HeapByteBuffer::asCharBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 1;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<CharBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                             segment == null ? null : *segment);
                return UNSAFE::newInstance<CharBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                         segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar ByteBuffer::HeapByteBuffer::getShort() {
            try {
                return UNSAFE::getShortUnaligned(hb, byteOffset(nextGetIndex(2)), bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putShort(gshort value) {
            try {
                UNSAFE::putShortUnaligned(hb, byteOffset(nextGetIndex(2)), value, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gshort ByteBuffer::HeapByteBuffer::getShort(gint index) const {
            try {
                return UNSAFE::getShortUnaligned(hb, byteOffset(checkIndex(index, 2)), bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putShort(gint index, gshort value) {
            try {
                UNSAFE::putCharUnaligned(hb, byteOffset(checkIndex(index, 2)), value, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ByteBuffer::HeapByteBuffer::asShortBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 1;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<ShortBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                              segment == null ? null : *segment);
                return UNSAFE::newInstance<ShortBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                          segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar ByteBuffer::HeapByteBuffer::getInt() {
            try {
                return UNSAFE::getShortUnaligned(hb, byteOffset(nextGetIndex(4)), bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putInt(gint value) {
            try {
                UNSAFE::putIntUnaligned(hb, byteOffset(nextGetIndex(4)), value, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint ByteBuffer::HeapByteBuffer::getInt(gint index) const {
            try {
                return UNSAFE::getIntUnaligned(hb, byteOffset(checkIndex(index, 4)), bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putInt(gint index, gint value) {
            try {
                UNSAFE::putIntUnaligned(hb, byteOffset(checkIndex(index, 2)), value, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& ByteBuffer::HeapByteBuffer::asIntBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 2;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<IntBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                            segment == null ? null : *segment);
                return UNSAFE::newInstance<IntBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                        segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar ByteBuffer::HeapByteBuffer::getLong() {
            try {
                return UNSAFE::getShortUnaligned(hb, byteOffset(nextGetIndex(8)), bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putLong(glong value) {
            try {
                UNSAFE::putLongUnaligned(hb, byteOffset(nextGetIndex(8)), value, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        glong ByteBuffer::HeapByteBuffer::getLong(gint index) const {
            try {
                return UNSAFE::getLongUnaligned(hb, byteOffset(checkIndex(index, 8)), bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putLong(gint index, glong value) {
            try {
                UNSAFE::putLongUnaligned(hb, byteOffset(checkIndex(index, 8)), value, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& ByteBuffer::HeapByteBuffer::asLongBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 3;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<LongBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                             segment == null ? null : *segment);
                return UNSAFE::newInstance<LongBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                         segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gfloat ByteBuffer::HeapByteBuffer::getFloat() {
            try {
                gint raw = UNSAFE::getIntUnaligned(hb, byteOffset(nextGetIndex(4)), bigEndian);
                return Float::fromIntBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putFloat(gfloat value) {
            gint raw = Float::toRawIntBits(value);
            try {
                UNSAFE::putIntUnaligned(hb, byteOffset(nextGetIndex(4)), raw, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gfloat ByteBuffer::HeapByteBuffer::getFloat(gint index) const {
            try {
                gint raw = UNSAFE::getIntUnaligned(hb, byteOffset(checkIndex(index, 4)), bigEndian);
                return Float::fromIntBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putFloat(gint index, gfloat value) {
            gint raw = Float::toRawIntBits(value);
            try {
                UNSAFE::putIntUnaligned(hb, byteOffset(checkIndex(index, 4)), raw, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& ByteBuffer::HeapByteBuffer::asFloatBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 2;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<FloatBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                              segment == null ? null : *segment);
                return UNSAFE::newInstance<FloatBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                          segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gdouble ByteBuffer::HeapByteBuffer::getDouble() {
            try {
                glong raw = UNSAFE::getLongUnaligned(hb, byteOffset(nextGetIndex(8)), bigEndian);
                return Double::fromLongBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putDouble(gdouble value) {
            glong raw = Double::toRawLongBits(value);
            try {
                UNSAFE::putLongUnaligned(hb, byteOffset(nextGetIndex(8)), raw, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gdouble ByteBuffer::HeapByteBuffer::getDouble(gint index) const {
            try {
                glong raw = UNSAFE::getLongUnaligned(hb, byteOffset(checkIndex(index, 8)), bigEndian);
                return Double::fromLongBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::HeapByteBuffer::putDouble(gint index, gdouble value) {
            glong raw = Double::toRawLongBits(value);
            try {
                UNSAFE::putLongUnaligned(hb, byteOffset(checkIndex(index, 8)), raw, bigEndian);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::HeapByteBuffer::hasArray() const {
            return true;
        }

        ByteArray& ByteBuffer::HeapByteBuffer::array() const {
            return hb;
        }

        gint ByteBuffer::HeapByteBuffer::arrayOffset() const {
            return offset;
        }

        String ByteBuffer::HeapByteBuffer::toString() const {
            try {
                return ""_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::HeapByteBuffer::asDoubleBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 3;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<DoubleBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                               segment == null ? null : *segment);
                return UNSAFE::newInstance<DoubleBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                           segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::HeapByteBuffer::ix(gint i) const {
            return offset + i;
        }

        glong ByteBuffer::HeapByteBuffer::byteOffset(glong i) const {
            return address + i;
        }

        gbyte ByteBuffer::HeapByteBuffer::getFast(gint i) const {
            try {
                return hb[ix(i)];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void ByteBuffer::HeapByteBuffer::putFast(gint i, gbyte b) {
            try {
                hb[i] = b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Object& ByteBuffer::HeapByteBuffer::base() const {
            return hb;
        }

        ByteBuffer::ReadOnlyHeapByteBuffer::ReadOnlyHeapByteBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              HeapByteBuffer(cap, lim, segment) {}

        ByteBuffer::ReadOnlyHeapByteBuffer::ReadOnlyHeapByteBuffer(ByteArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              HeapByteBuffer(buf, off, len, segment) {}

        ByteBuffer::ReadOnlyHeapByteBuffer::ReadOnlyHeapByteBuffer(ByteArray& buf, gint mark, gint pos, gint lim,
                                                                   gint cap, gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              HeapByteBuffer(buf, mark, pos, lim, cap, off, segment) {}

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos <= lim ? lim - pos : 0;
                return UNSAFE::newInstance<ReadOnlyHeapByteBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyHeapByteBuffer>(hb, -1, 0, length, length, index + offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapByteBuffer>(hb, markValue(), position(), limit(), capacity(),
                                                                   offset, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::asReadOnlyBuffer() const {
            return duplicate();
        }

        gbool ByteBuffer::ReadOnlyHeapByteBuffer::isReadOnly() const {
            return true;
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::put(gbyte c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::put(gint index, gbyte c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::put(const ByteArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::put(const ByteArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::
        put(gint index, const ByteArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::put(gint index, const ByteArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::put(ByteBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::put(gint index, ByteBuffer& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        String ByteBuffer::ReadOnlyHeapByteBuffer::toString() const {
            try {
                return ""_S;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool ByteBuffer::ReadOnlyHeapByteBuffer::hasArray() const {
            return false;
        }

        ByteArray& ByteBuffer::ReadOnlyHeapByteBuffer::array() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        gint ByteBuffer::ReadOnlyHeapByteBuffer::arrayOffset() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putChar(gchar value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putChar(gint index, gchar value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::asCharBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 1;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<ReadOnlyCharBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                     segment == null ? null : *segment);
                return UNSAFE::newInstance<ReadOnlyCharBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putShort(gshort value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putShort(gint index, gshort value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::asShortBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 1;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<ReadOnlyShortBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                      segment == null ? null : *segment);
                return UNSAFE::newInstance<ReadOnlyShortBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putInt(gint value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putInt(gint index, gint value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::asIntBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 2;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<ReadOnlyIntBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                    segment == null ? null : *segment);
                return UNSAFE::newInstance<ReadOnlyIntBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putLong(glong value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putLong(gint index, glong value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::asLongBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 3;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<ReadOnlyLongBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                     segment == null ? null : *segment);
                return UNSAFE::newInstance<ReadOnlyLongBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putFloat(gfloat value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putFloat(gint index, gfloat value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::asFloatBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 2;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<ReadOnlyFloatBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                      segment == null ? null : *segment);
                return UNSAFE::newInstance<ReadOnlyFloatBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putDouble(gdouble value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ByteBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::putDouble(gint index, gdouble value) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyHeapByteBuffer::asDoubleBuffer() const {
            try {
                gint pos = position();
                gint size = limit() - pos >> 3;
                glong addr = address + pos;
                if (bigEndian)
                    return UNSAFE::newInstance<ReadOnlyDoubleBufferBE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                       segment == null ? null : *segment);
                return UNSAFE::newInstance<ReadOnlyDoubleBufferLE>((ByteBuffer&) *this, -1, 0, size, size, addr,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbyte ByteBuffer::ReadOnlyHeapByteBuffer::getFast(gint i) const {
            try {
                return hb[i];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void ByteBuffer::ReadOnlyHeapByteBuffer::putFast(gint i, gbyte b) {
            ReadOnlyBufferException().throws($ftrace());
        }

        // ------------------------------------- ByteBuffer as CharBuffer ---------------------------------------------
        // #1 Read/Write LITTLE_ENDIAN

        ByteBuffer::CharBufferLE::CharBufferLE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment),
              CharBuffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                CharBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = CharBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::CharBufferLE::CharBufferLE(ByteBuffer& bb,
                                               gint mark, gint pos, gint lim, gint cap,
                                               glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              CharBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        CharBuffer& ByteBuffer::CharBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<CharBufferLE>(bb, -1, 0, rem, rem, addr, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<CharBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<CharBufferLE>(bb, markValue(), position(), limit(),
                                                         capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferLE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyCharBufferLE>(bb, markValue(), position(), limit(),
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar ByteBuffer::CharBufferLE::get() {
            try {
                return UNSAFE::getCharUnaligned(base(), byteOffset(nextGetIndex()), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar ByteBuffer::CharBufferLE::get(gint index) const {
            try {
                return UNSAFE::getCharUnaligned(base(), byteOffset(checkIndex(index)), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferLE::put(gchar c) {
            try {
                UNSAFE::putCharUnaligned(base(), byteOffset(nextGetIndex()), c, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& ByteBuffer::CharBufferLE::put(gint index, gchar c) {
            try {
                UNSAFE::putCharUnaligned(base(), byteOffset(checkIndex(index)), c, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& ByteBuffer::CharBufferLE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::CharBufferLE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::CharBufferLE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::CharBufferLE::toString() const {
            try {
                gint start = position();
                gint end = limit();
                gint len = end - start;
                CharArray ca = CharArray(len);
                CharBuffer& cb = wrap(ca);
                CharBuffer& db = duplicate();
                db.position(start);
                db.limit(end);
                cb.put(db);
                String str = String(ca);
                UNSAFE::deleteInstance(cb);
                UNSAFE::deleteInstance(db);
                return str;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferLE::subSequence(gint start, gint end) const {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            pos = pos <= lim ? pos : lim;
            gint len = lim - pos;

            try {
                Preconditions::checkIndexFromRange(start, end, len);
                return UNSAFE::newInstance<CharBufferLE>(bb, -1, pos + start, pos + end,
                                                         capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::CharBufferLE::order() const {
            return Charset::ByteOrder::LITTLE_ENDIAN;
        }

        gbool ByteBuffer::CharBufferLE::hasArray() const {
            return false;
        }

        CharArray& ByteBuffer::CharBufferLE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::CharBufferLE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::CharBufferLE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::CharBufferLE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::CharBufferLE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        gchar ByteBuffer::CharBufferLE::getFast(gint i) const {
            try {
                return UNSAFE::getCharUnaligned(base(), byteOffset(i), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::CharBufferLE::charOrder() const {
            return order();
        }

        // #2 Read/Write BIG_ENDIAN

        ByteBuffer::CharBufferBE::CharBufferBE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment),
              CharBuffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                CharBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = CharBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::CharBufferBE::CharBufferBE(ByteBuffer& bb,
                                               gint mark, gint pos, gint lim, gint cap,
                                               glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              CharBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        CharBuffer& ByteBuffer::CharBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyCharBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyCharBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<CharBufferBE>(bb, markValue(), position(), limit(),
                                                         capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferBE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyCharBufferBE>(bb, markValue(), position(), limit(),
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar ByteBuffer::CharBufferBE::get() {
            try {
                return UNSAFE::getCharUnaligned(base(), byteOffset(nextGetIndex()), true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar ByteBuffer::CharBufferBE::get(gint index) const {
            try {
                return UNSAFE::getCharUnaligned(base(), byteOffset(checkIndex(index)), true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferBE::put(gchar c) {
            try {
                UNSAFE::putCharUnaligned(base(), byteOffset(nextGetIndex()), c, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& ByteBuffer::CharBufferBE::put(gint index, gchar c) {
            try {
                UNSAFE::putCharUnaligned(base(), byteOffset(checkIndex(index)), c, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& ByteBuffer::CharBufferBE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::CharBufferBE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::CharBufferBE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::CharBufferBE::toString() const {
            try {
                gint start = position();
                gint end = limit();
                gint len = end - start;
                CharArray ca = CharArray(len);
                CharBuffer& cb = wrap(ca);
                CharBuffer& db = duplicate();
                db.position(start);
                db.limit(end);
                cb.put(db);
                String str = String(ca);
                UNSAFE::deleteInstance(cb);
                UNSAFE::deleteInstance(db);
                return str;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::CharBufferBE::subSequence(gint start, gint end) const {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            pos = pos <= lim ? pos : lim;
            gint len = lim - pos;

            try {
                Preconditions::checkIndexFromRange(start, end, len);
                return UNSAFE::newInstance<CharBufferBE>(bb, -1, pos + start, pos + end,
                                                         capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::CharBufferBE::order() const {
            return Charset::ByteOrder::BIG_ENDIAN;
        }

        gbool ByteBuffer::CharBufferBE::hasArray() const {
            return false;
        }

        CharArray& ByteBuffer::CharBufferBE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::CharBufferBE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::CharBufferBE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::CharBufferBE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::CharBufferBE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        gchar ByteBuffer::CharBufferBE::getFast(gint i) const {
            try {
                return UNSAFE::getCharUnaligned(base(), byteOffset(i), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::CharBufferBE::charOrder() const {
            return order();
        }

        // #3 Read-Only LITTLE ENDIAN

        ByteBuffer::ReadOnlyCharBufferLE::ReadOnlyCharBufferLE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment),
              CharBufferLE(bb, segment) {}

        ByteBuffer::ReadOnlyCharBufferLE::ReadOnlyCharBufferLE(ByteBuffer& bb,
                                                               gint mark, gint pos, gint lim, gint cap, glong addr,
                                                               Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              CharBufferLE(bb, mark, pos, lim, cap, addr, segment) {}

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyCharBufferLE>(bb, -1, 0, rem, rem, addr,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyCharBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyCharBufferLE>(bb, markValue(), position(), limit(),
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(gint index, gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(CharBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(gint index, CharBuffer& src, gint offset,
                                                          gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(const CharArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(const CharArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(gint index, const CharArray& src, gint offset,
                                                          gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(gint index, const CharArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(const String& src, gint start, gint end) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::put(const String& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyCharBufferLE::isReadOnly() const {
            return true;
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferLE::subSequence(gint start, gint end) const {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            pos = pos <= lim ? pos : lim;
            gint len = lim - pos;

            try {
                Preconditions::checkIndexFromRange(start, end, len);
                return UNSAFE::newInstance<ReadOnlyCharBufferLE>(bb, -1, pos + start, pos + end,
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        // #4 Read-Only BIG ENDIAN

        ByteBuffer::ReadOnlyCharBufferBE::ReadOnlyCharBufferBE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment),
              CharBufferBE(bb, segment) {}

        ByteBuffer::ReadOnlyCharBufferBE::ReadOnlyCharBufferBE(ByteBuffer& bb,
                                                               gint mark, gint pos, gint lim, gint cap, glong addr,
                                                               Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              CharBufferBE(bb, mark, pos, lim, cap, addr, segment) {}

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyCharBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyCharBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyCharBufferBE>(bb, markValue(), position(), limit(),
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(gint index, gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(CharBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(gint index, CharBuffer& src, gint offset,
                                                          gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(const CharArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(const CharArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(gint index, const CharArray& src, gint offset,
                                                          gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(gint index, const CharArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(const String& src, gint start, gint end) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::put(const String& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyCharBufferBE::isReadOnly() const {
            return true;
        }

        CharBuffer& ByteBuffer::ReadOnlyCharBufferBE::subSequence(gint start, gint end) const {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            pos = pos <= lim ? pos : lim;
            gint len = lim - pos;

            try {
                Preconditions::checkIndexFromRange(start, end, len);
                return UNSAFE::newInstance<ReadOnlyCharBufferBE>(bb, -1, pos + start, pos + end,
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        // ------------------------------------- ByteBuffer as ShortBuffer --------------------------------------------
        // #5 Read/Write LITTLE ENDIAN

        ByteBuffer::ShortBufferLE::ShortBufferLE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment),
              ShortBuffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                ShortBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = ShortBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::ShortBufferLE::ShortBufferLE(ByteBuffer& bb,
                                                 gint mark, gint pos, gint lim, gint cap,
                                                 glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              ShortBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        ShortBuffer& ByteBuffer::ShortBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ShortBufferLE>(bb, -1, 0, rem, rem, addr, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ShortBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ShortBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ShortBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<ShortBufferLE>(bb, markValue(), position(), limit(),
                                                          capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ShortBufferLE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyShortBufferLE>(bb, markValue(), position(), limit(),
                                                                  capacity(), address,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gshort ByteBuffer::ShortBufferLE::get() {
            try {
                return UNSAFE::getShortUnaligned(base(), byteOffset(nextGetIndex()), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gshort ByteBuffer::ShortBufferLE::get(gint index) const {
            try {
                return UNSAFE::getShortUnaligned(base(), byteOffset(checkIndex(index)), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ShortBufferLE::put(gshort c) {
            try {
                UNSAFE::putShortUnaligned(base(), byteOffset(nextGetIndex()), c, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ByteBuffer::ShortBufferLE::put(gint index, gshort c) {
            try {
                UNSAFE::putShortUnaligned(base(), byteOffset(checkIndex(index)), c, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ByteBuffer::ShortBufferLE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::ShortBufferLE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::ShortBufferLE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::ShortBufferLE::toString() const {
            try {
                return "ShortBufferLE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::ShortBufferLE::order() const {
            return Charset::ByteOrder::LITTLE_ENDIAN;
        }

        gbool ByteBuffer::ShortBufferLE::hasArray() const {
            return false;
        }

        ShortArray& ByteBuffer::ShortBufferLE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::ShortBufferLE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::ShortBufferLE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::ShortBufferLE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::ShortBufferLE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #6 Read/Write BIG_ENDIAN

        ByteBuffer::ShortBufferBE::ShortBufferBE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment),
              ShortBuffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                ShortBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = ShortBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::ShortBufferBE::ShortBufferBE(ByteBuffer& bb,
                                                 gint mark, gint pos, gint lim, gint cap,
                                                 glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              ShortBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        ShortBuffer& ByteBuffer::ShortBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyShortBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ShortBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyShortBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ShortBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<ShortBufferBE>(bb, markValue(), position(), limit(),
                                                          capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ShortBufferBE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyShortBufferBE>(bb, markValue(), position(), limit(),
                                                                  capacity(), address,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gshort ByteBuffer::ShortBufferBE::get() {
            try {
                return UNSAFE::getShortUnaligned(base(), byteOffset(nextGetIndex()), true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gshort ByteBuffer::ShortBufferBE::get(gint index) const {
            try {
                return UNSAFE::getShortUnaligned(base(), byteOffset(checkIndex(index)), true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ShortBufferBE::put(gshort c) {
            try {
                UNSAFE::putShortUnaligned(base(), byteOffset(nextGetIndex()), c, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ByteBuffer::ShortBufferBE::put(gint index, gshort c) {
            try {
                UNSAFE::putShortUnaligned(base(), byteOffset(checkIndex(index)), c, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        ShortBuffer& ByteBuffer::ShortBufferBE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::ShortBufferBE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::ShortBufferBE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::ShortBufferBE::toString() const {
            try {
                return "ShortBufferBE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::ShortBufferBE::order() const {
            return Charset::ByteOrder::BIG_ENDIAN;
        }

        gbool ByteBuffer::ShortBufferBE::hasArray() const {
            return false;
        }

        ShortArray& ByteBuffer::ShortBufferBE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::ShortBufferBE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::ShortBufferBE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::ShortBufferBE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::ShortBufferBE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #7 Read-Only LITTLE ENDIAN

        ByteBuffer::ReadOnlyShortBufferLE::ReadOnlyShortBufferLE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment),
              ShortBufferLE(bb, segment) {}

        ByteBuffer::ReadOnlyShortBufferLE::ReadOnlyShortBufferLE(ByteBuffer& bb,
                                                                 gint mark, gint pos, gint lim, gint cap, glong addr,
                                                                 Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              ShortBufferLE(bb, mark, pos, lim, cap, addr, segment) {}

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyShortBufferLE>(bb, -1, 0, rem, rem, addr,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyShortBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyShortBufferLE>(bb, markValue(), position(), limit(),
                                                                  capacity(), address,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::put(gshort c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::put(gint index, gshort c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::put(ShortBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::put(gint index, ShortBuffer& src, gint offset,
                                                            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::put(const ShortArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::put(const ShortArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::put(gint index, const ShortArray& src, gint offset,
                                                            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::put(gint index, const ShortArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferLE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyShortBufferLE::isReadOnly() const {
            return true;
        }

        // #8 Read-Only BIG ENDIAN

        ByteBuffer::ReadOnlyShortBufferBE::ReadOnlyShortBufferBE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 1, bb.remaining() >> 1, segment),
              ShortBufferBE(bb, segment) {}

        ByteBuffer::ReadOnlyShortBufferBE::ReadOnlyShortBufferBE(ByteBuffer& bb,
                                                                 gint mark, gint pos, gint lim, gint cap, glong addr,
                                                                 Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              ShortBufferBE(bb, mark, pos, lim, cap, addr, segment) {}

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyShortBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyShortBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyShortBufferBE>(bb, markValue(), position(), limit(),
                                                                  capacity(), address,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::put(gshort c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::put(gint index, gshort c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::put(ShortBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::put(gint index, ShortBuffer& src, gint offset,
                                                            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::put(const ShortArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::put(const ShortArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::put(gint index, const ShortArray& src, gint offset,
                                                            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::put(gint index, const ShortArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        ShortBuffer& ByteBuffer::ReadOnlyShortBufferBE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyShortBufferBE::isReadOnly() const {
            return true;
        }


        // ------------------------------------- ByteBuffer as IntBuffer --------------------------------------------
        // #5 Read/Write LITTLE ENDIAN

        ByteBuffer::IntBufferLE::IntBufferLE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              IntBuffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                IntBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = IntBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::IntBufferLE::IntBufferLE(ByteBuffer& bb,
                                             gint mark, gint pos, gint lim, gint cap,
                                             glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              IntBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        IntBuffer& ByteBuffer::IntBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<IntBufferLE>(bb, -1, 0, rem, rem, addr, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::IntBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<IntBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::IntBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<IntBufferLE>(bb, markValue(), position(), limit(),
                                                        capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::IntBufferLE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyIntBufferLE>(bb, markValue(), position(), limit(),
                                                                capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::IntBufferLE::get() {
            try {
                return UNSAFE::getIntUnaligned(base(), byteOffset(nextGetIndex()), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::IntBufferLE::get(gint index) const {
            try {
                return UNSAFE::getIntUnaligned(base(), byteOffset(checkIndex(index)), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::IntBufferLE::put(gint c) {
            try {
                UNSAFE::putIntUnaligned(base(), byteOffset(nextGetIndex()), c, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& ByteBuffer::IntBufferLE::put(gint index, gint c) {
            try {
                UNSAFE::putIntUnaligned(base(), byteOffset(checkIndex(index)), c, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& ByteBuffer::IntBufferLE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::IntBufferLE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::IntBufferLE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::IntBufferLE::toString() const {
            try {
                return "IntBufferLE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::IntBufferLE::order() const {
            return Charset::ByteOrder::LITTLE_ENDIAN;
        }

        gbool ByteBuffer::IntBufferLE::hasArray() const {
            return false;
        }

        IntArray& ByteBuffer::IntBufferLE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::IntBufferLE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::IntBufferLE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::IntBufferLE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::IntBufferLE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #6 Read/Write BIG_ENDIAN

        ByteBuffer::IntBufferBE::IntBufferBE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              IntBuffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                IntBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = IntBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::IntBufferBE::IntBufferBE(ByteBuffer& bb,
                                             gint mark, gint pos, gint lim, gint cap,
                                             glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              IntBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        IntBuffer& ByteBuffer::IntBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyIntBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::IntBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyIntBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::IntBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<IntBufferBE>(bb, markValue(), position(), limit(),
                                                        capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::IntBufferBE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyIntBufferBE>(bb, markValue(), position(), limit(),
                                                                capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::IntBufferBE::get() {
            try {
                return UNSAFE::getIntUnaligned(base(), byteOffset(nextGetIndex()), true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint ByteBuffer::IntBufferBE::get(gint index) const {
            try {
                return UNSAFE::getIntUnaligned(base(), byteOffset(checkIndex(index)), true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::IntBufferBE::put(gint c) {
            try {
                UNSAFE::putIntUnaligned(base(), byteOffset(nextGetIndex()), c, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& ByteBuffer::IntBufferBE::put(gint index, gint c) {
            try {
                UNSAFE::putIntUnaligned(base(), byteOffset(checkIndex(index)), c, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        IntBuffer& ByteBuffer::IntBufferBE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::IntBufferBE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::IntBufferBE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::IntBufferBE::toString() const {
            try {
                return "IntBufferBE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::IntBufferBE::order() const {
            return Charset::ByteOrder::BIG_ENDIAN;
        }

        gbool ByteBuffer::IntBufferBE::hasArray() const {
            return false;
        }

        IntArray& ByteBuffer::IntBufferBE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::IntBufferBE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::IntBufferBE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::IntBufferBE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::IntBufferBE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #7 Read-Only LITTLE ENDIAN

        ByteBuffer::ReadOnlyIntBufferLE::ReadOnlyIntBufferLE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              IntBufferLE(bb, segment) {}

        ByteBuffer::ReadOnlyIntBufferLE::ReadOnlyIntBufferLE(ByteBuffer& bb,
                                                             gint mark, gint pos, gint lim, gint cap, glong addr,
                                                             Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              IntBufferLE(bb, mark, pos, lim, cap, addr, segment) {}

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyIntBufferLE>(bb, -1, 0, rem, rem, addr,
                                                                segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyIntBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyIntBufferLE>(bb, markValue(), position(), limit(),
                                                                capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::put(gint c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::put(gint index, gint c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::put(IntBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::put(gint index, IntBuffer& src, gint offset,
                                                        gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::put(const IntArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::put(const IntArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::put(gint index, const IntArray& src, gint offset,
                                                        gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::put(gint index, const IntArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferLE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyIntBufferLE::isReadOnly() const {
            return true;
        }

        // #8 Read-Only BIG ENDIAN

        ByteBuffer::ReadOnlyIntBufferBE::ReadOnlyIntBufferBE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              IntBufferBE(bb, segment) {}

        ByteBuffer::ReadOnlyIntBufferBE::ReadOnlyIntBufferBE(ByteBuffer& bb,
                                                             gint mark, gint pos, gint lim, gint cap, glong addr,
                                                             Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              IntBufferBE(bb, mark, pos, lim, cap, addr, segment) {}

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyIntBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyIntBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyIntBufferBE>(bb, markValue(), position(), limit(),
                                                                capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::put(gint c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::put(gint index, gint c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::put(IntBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::put(gint index, IntBuffer& src, gint offset,
                                                        gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::put(const IntArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::put(const IntArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::put(gint index, const IntArray& src, gint offset,
                                                        gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::put(gint index, const IntArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        IntBuffer& ByteBuffer::ReadOnlyIntBufferBE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyIntBufferBE::isReadOnly() const {
            return true;
        }

        // ------------------------------------- ByteBuffer as LongBuffer --------------------------------------------
        // #5 Read/Write LITTLE ENDIAN

        ByteBuffer::LongBufferLE::LongBufferLE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment),
              LongBuffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                LongBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = LongBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::LongBufferLE::LongBufferLE(ByteBuffer& bb,
                                               gint mark, gint pos, gint lim, gint cap,
                                               glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              LongBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        LongBuffer& ByteBuffer::LongBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<LongBufferLE>(bb, -1, 0, rem, rem, addr, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::LongBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<LongBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::LongBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<LongBufferLE>(bb, markValue(), position(), limit(),
                                                         capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::LongBufferLE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyLongBufferLE>(bb, markValue(), position(), limit(),
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong ByteBuffer::LongBufferLE::get() {
            try {
                return UNSAFE::getLongUnaligned(base(), byteOffset(nextGetIndex()), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong ByteBuffer::LongBufferLE::get(gint index) const {
            try {
                return UNSAFE::getLongUnaligned(base(), byteOffset(checkIndex(index)), false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::LongBufferLE::put(glong c) {
            try {
                UNSAFE::putLongUnaligned(base(), byteOffset(nextGetIndex()), c, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& ByteBuffer::LongBufferLE::put(gint index, glong c) {
            try {
                UNSAFE::putLongUnaligned(base(), byteOffset(checkIndex(index)), c, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& ByteBuffer::LongBufferLE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::LongBufferLE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::LongBufferLE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::LongBufferLE::toString() const {
            try {
                return "LongBufferLE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::LongBufferLE::order() const {
            return Charset::ByteOrder::LITTLE_ENDIAN;
        }

        gbool ByteBuffer::LongBufferLE::hasArray() const {
            return false;
        }

        LongArray& ByteBuffer::LongBufferLE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::LongBufferLE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::LongBufferLE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::LongBufferLE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::LongBufferLE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #6 Read/Write BIG_ENDIAN

        ByteBuffer::LongBufferBE::LongBufferBE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment),
              LongBuffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                LongBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = LongBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::LongBufferBE::LongBufferBE(ByteBuffer& bb,
                                               gint mark, gint pos, gint lim, gint cap,
                                               glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              LongBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        LongBuffer& ByteBuffer::LongBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyLongBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::LongBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyLongBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::LongBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<LongBufferBE>(bb, markValue(), position(), limit(),
                                                         capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::LongBufferBE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyLongBufferBE>(bb, markValue(), position(), limit(),
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong ByteBuffer::LongBufferBE::get() {
            try {
                return UNSAFE::getLongUnaligned(base(), byteOffset(nextGetIndex()), true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong ByteBuffer::LongBufferBE::get(gint index) const {
            try {
                return UNSAFE::getLongUnaligned(base(), byteOffset(checkIndex(index)), true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::LongBufferBE::put(glong c) {
            try {
                UNSAFE::putLongUnaligned(base(), byteOffset(nextGetIndex()), c, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& ByteBuffer::LongBufferBE::put(gint index, glong c) {
            try {
                UNSAFE::putLongUnaligned(base(), byteOffset(checkIndex(index)), c, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        LongBuffer& ByteBuffer::LongBufferBE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::LongBufferBE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::LongBufferBE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::LongBufferBE::toString() const {
            try {
                return "LongBufferBE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::LongBufferBE::order() const {
            return Charset::ByteOrder::BIG_ENDIAN;
        }

        gbool ByteBuffer::LongBufferBE::hasArray() const {
            return false;
        }

        LongArray& ByteBuffer::LongBufferBE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::LongBufferBE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::LongBufferBE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::LongBufferBE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::LongBufferBE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #7 Read-Only LITTLE ENDIAN

        ByteBuffer::ReadOnlyLongBufferLE::ReadOnlyLongBufferLE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment),
              LongBufferLE(bb, segment) {}

        ByteBuffer::ReadOnlyLongBufferLE::ReadOnlyLongBufferLE(ByteBuffer& bb,
                                                               gint mark, gint pos, gint lim, gint cap, glong addr,
                                                               Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              LongBufferLE(bb, mark, pos, lim, cap, addr, segment) {}

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyLongBufferLE>(bb, -1, 0, rem, rem, addr,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyLongBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyLongBufferLE>(bb, markValue(), position(), limit(),
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::put(glong c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::put(gint index, glong c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::put(LongBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::put(gint index, LongBuffer& src, gint offset,
                                                          gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::put(const LongArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::put(const LongArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::put(gint index, const LongArray& src, gint offset,
                                                          gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::put(gint index, const LongArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferLE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyLongBufferLE::isReadOnly() const {
            return true;
        }

        // #8 Read-Only BIG ENDIAN

        ByteBuffer::ReadOnlyLongBufferBE::ReadOnlyLongBufferBE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment),
              LongBufferBE(bb, segment) {}

        ByteBuffer::ReadOnlyLongBufferBE::ReadOnlyLongBufferBE(ByteBuffer& bb,
                                                               gint mark, gint pos, gint lim, gint cap, glong addr,
                                                               Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              LongBufferBE(bb, mark, pos, lim, cap, addr, segment) {}

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyLongBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyLongBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyLongBufferBE>(bb, markValue(), position(), limit(),
                                                                 capacity(), address,
                                                                 segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::put(glong c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::put(gint index, glong c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::put(LongBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::put(gint index, LongBuffer& src, gint offset,
                                                          gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::put(const LongArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::put(const LongArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::put(gint index, const LongArray& src, gint offset,
                                                          gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::put(gint index, const LongArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        LongBuffer& ByteBuffer::ReadOnlyLongBufferBE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyLongBufferBE::isReadOnly() const {
            return true;
        }

        // ------------------------------------- ByteBuffer as FloatBuffer --------------------------------------------
        // #5 Read/Write LITTLE ENDIAN

        ByteBuffer::FloatBufferLE::FloatBufferLE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              FloatBuffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                FloatBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = FloatBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::FloatBufferLE::FloatBufferLE(ByteBuffer& bb,
                                                 gint mark, gint pos, gint lim, gint cap,
                                                 glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              FloatBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        FloatBuffer& ByteBuffer::FloatBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<FloatBufferLE>(bb, -1, 0, rem, rem, addr, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::FloatBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<FloatBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::FloatBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<FloatBufferLE>(bb, markValue(), position(), limit(),
                                                          capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::FloatBufferLE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyFloatBufferLE>(bb, markValue(), position(), limit(),
                                                                  capacity(), address,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gfloat ByteBuffer::FloatBufferLE::get() {
            try {
                gint raw = UNSAFE::getIntUnaligned(base(), byteOffset(nextGetIndex()), false);
                return Float::fromIntBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gfloat ByteBuffer::FloatBufferLE::get(gint index) const {
            try {
                gint raw = UNSAFE::getIntUnaligned(base(), byteOffset(checkIndex(index)), false);
                return Float::fromIntBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::FloatBufferLE::put(gfloat c) {
            gint raw = Float::toRawIntBits(c);
            try {
                UNSAFE::putIntUnaligned(base(), byteOffset(nextGetIndex()), raw, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& ByteBuffer::FloatBufferLE::put(gint index, gfloat c) {
            gint raw = Float::toRawIntBits(c);
            try {
                UNSAFE::putIntUnaligned(base(), byteOffset(checkIndex(index)), raw, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& ByteBuffer::FloatBufferLE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::FloatBufferLE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::FloatBufferLE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::FloatBufferLE::toString() const {
            try {
                return "FloatBufferLE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::FloatBufferLE::order() const {
            return Charset::ByteOrder::LITTLE_ENDIAN;
        }

        gbool ByteBuffer::FloatBufferLE::hasArray() const {
            return false;
        }

        FloatArray& ByteBuffer::FloatBufferLE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::FloatBufferLE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::FloatBufferLE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::FloatBufferLE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::FloatBufferLE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #6 Read/Write BIG_ENDIAN

        ByteBuffer::FloatBufferBE::FloatBufferBE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              FloatBuffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                FloatBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = FloatBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::FloatBufferBE::FloatBufferBE(ByteBuffer& bb,
                                                 gint mark, gint pos, gint lim, gint cap,
                                                 glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              FloatBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        FloatBuffer& ByteBuffer::FloatBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyFloatBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::FloatBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyFloatBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::FloatBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<FloatBufferBE>(bb, markValue(), position(), limit(),
                                                          capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::FloatBufferBE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyFloatBufferBE>(bb, markValue(), position(), limit(),
                                                                  capacity(), address,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gfloat ByteBuffer::FloatBufferBE::get() {
            try {
                gint const raw = UNSAFE::getIntUnaligned(base(), byteOffset(nextGetIndex()), true);
                return Float::fromIntBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gfloat ByteBuffer::FloatBufferBE::get(gint index) const {
            try {
                gint const raw = UNSAFE::getIntUnaligned(base(), byteOffset(checkIndex(index)), true);
                return Float::fromIntBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::FloatBufferBE::put(gfloat c) {
            try {
                gint raw = Float::toRawIntBits(c);
                UNSAFE::putIntUnaligned(base(), byteOffset(nextGetIndex()), raw, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& ByteBuffer::FloatBufferBE::put(gint index, gfloat c) {
            try {
                gint raw = Float::toRawIntBits(c);
                UNSAFE::putIntUnaligned(base(), byteOffset(checkIndex(index)), raw, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        FloatBuffer& ByteBuffer::FloatBufferBE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::FloatBufferBE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::FloatBufferBE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::FloatBufferBE::toString() const {
            try {
                return "FloatBufferBE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::FloatBufferBE::order() const {
            return Charset::ByteOrder::BIG_ENDIAN;
        }

        gbool ByteBuffer::FloatBufferBE::hasArray() const {
            return false;
        }

        FloatArray& ByteBuffer::FloatBufferBE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::FloatBufferBE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::FloatBufferBE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::FloatBufferBE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::FloatBufferBE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #7 Read-Only LITTLE ENDIAN

        ByteBuffer::ReadOnlyFloatBufferLE::ReadOnlyFloatBufferLE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              FloatBufferLE(bb, segment) {}

        ByteBuffer::ReadOnlyFloatBufferLE::ReadOnlyFloatBufferLE(ByteBuffer& bb,
                                                                 gint mark, gint pos, gint lim, gint cap, glong addr,
                                                                 Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              FloatBufferLE(bb, mark, pos, lim, cap, addr, segment) {}

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyFloatBufferLE>(bb, -1, 0, rem, rem, addr,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyFloatBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyFloatBufferLE>(bb, markValue(), position(), limit(),
                                                                  capacity(), address,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::put(gfloat c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::put(gint index, gfloat c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::put(FloatBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::put(gint index, FloatBuffer& src, gint offset,
                                                            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::put(const FloatArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::put(const FloatArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::put(gint index, const FloatArray& src, gint offset,
                                                            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::put(gint index, const FloatArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferLE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyFloatBufferLE::isReadOnly() const {
            return true;
        }

        // #8 Read-Only BIG ENDIAN

        ByteBuffer::ReadOnlyFloatBufferBE::ReadOnlyFloatBufferBE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              FloatBufferBE(bb, segment) {}

        ByteBuffer::ReadOnlyFloatBufferBE::ReadOnlyFloatBufferBE(ByteBuffer& bb,
                                                                 gint mark, gint pos, gint lim, gint cap, glong addr,
                                                                 Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              FloatBufferBE(bb, mark, pos, lim, cap, addr, segment) {}

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyFloatBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyFloatBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyFloatBufferBE>(bb, markValue(), position(), limit(),
                                                                  capacity(), address,
                                                                  segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::put(gfloat c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::put(gint index, gfloat c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::put(FloatBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::put(gint index, FloatBuffer& src, gint offset,
                                                            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::put(const FloatArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::put(const FloatArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::put(gint index, const FloatArray& src, gint offset,
                                                            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::put(gint index, const FloatArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        FloatBuffer& ByteBuffer::ReadOnlyFloatBufferBE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyFloatBufferBE::isReadOnly() const {
            return true;
        }

        // ------------------------------------- ByteBuffer as DoubleBuffer --------------------------------------------
        // #5 Read/Write LITTLE ENDIAN

        ByteBuffer::DoubleBufferLE::DoubleBufferLE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment),
              DoubleBuffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                DoubleBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = DoubleBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::DoubleBufferLE::DoubleBufferLE(ByteBuffer& bb,
                                                   gint mark, gint pos, gint lim, gint cap,
                                                   glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              DoubleBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        DoubleBuffer& ByteBuffer::DoubleBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<
                    DoubleBufferLE>(bb, -1, 0, rem, rem, addr, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::DoubleBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<DoubleBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::DoubleBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<DoubleBufferLE>(bb, markValue(), position(), limit(),
                                                           capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::DoubleBufferLE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyDoubleBufferLE>(bb, markValue(), position(), limit(),
                                                                   capacity(), address,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gdouble ByteBuffer::DoubleBufferLE::get() {
            try {
                glong const raw = UNSAFE::getLongUnaligned(base(), byteOffset(nextGetIndex()), false);
                return Double::fromLongBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gdouble ByteBuffer::DoubleBufferLE::get(gint index) const {
            try {
                glong const raw = UNSAFE::getLongUnaligned(base(), byteOffset(checkIndex(index)), false);
                return Double::fromLongBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::DoubleBufferLE::put(gdouble c) {
            try {
                glong const raw = Double::toRawLongBits(c);
                UNSAFE::putLongUnaligned(base(), byteOffset(nextGetIndex()), raw, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& ByteBuffer::DoubleBufferLE::put(gint index, gdouble c) {
            try {
                glong const raw = Double::toRawLongBits(c);
                UNSAFE::putLongUnaligned(base(), byteOffset(checkIndex(index)), raw, false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& ByteBuffer::DoubleBufferLE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::DoubleBufferLE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::DoubleBufferLE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::DoubleBufferLE::toString() const {
            try {
                return "DoubleBufferLE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::DoubleBufferLE::order() const {
            return Charset::ByteOrder::LITTLE_ENDIAN;
        }

        gbool ByteBuffer::DoubleBufferLE::hasArray() const {
            return false;
        }

        DoubleArray& ByteBuffer::DoubleBufferLE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::DoubleBufferLE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::DoubleBufferLE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::DoubleBufferLE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::DoubleBufferLE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #6 Read/Write BIG_ENDIAN

        ByteBuffer::DoubleBufferBE::DoubleBufferBE(ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment),
              DoubleBuffer(-1, 0, bb.remaining() >> 3, bb.remaining() >> 3, segment), bb(bb) {
            // enforce limit == capacity
            gint cap = capacity();
            try {
                DoubleBuffer::limit(cap);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = DoubleBuffer::position();
            CORE_ASSERT(pos <= cap);
            address = bb.address;
        }

        ByteBuffer::DoubleBufferBE::DoubleBufferBE(ByteBuffer& bb,
                                                   gint mark, gint pos, gint lim, gint cap,
                                                   glong addr, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              DoubleBuffer(mark, pos, lim, cap, segment), bb(bb) {
            address = addr;
            CORE_ASSERT(address >= bb.address);
        }

        DoubleBuffer& ByteBuffer::DoubleBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyDoubleBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::DoubleBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyDoubleBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::DoubleBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<DoubleBufferBE>(bb, markValue(), position(), limit(),
                                                           capacity(), address, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::DoubleBufferBE::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyDoubleBufferBE>(bb, markValue(), position(), limit(),
                                                                   capacity(), address,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gdouble ByteBuffer::DoubleBufferBE::get() {
            try {
                glong const raw = UNSAFE::getLongUnaligned(base(), byteOffset(nextGetIndex()), true);
                return Double::fromLongBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gdouble ByteBuffer::DoubleBufferBE::get(gint index) const {
            try {
                glong const raw = UNSAFE::getLongUnaligned(base(), byteOffset(checkIndex(index)), true);
                return Double::fromLongBits(raw);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::DoubleBufferBE::put(gdouble c) {
            try {
                glong const raw = Double::toRawLongBits(c);
                UNSAFE::putLongUnaligned(base(), byteOffset(nextGetIndex()), raw, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& ByteBuffer::DoubleBufferBE::put(gint index, gdouble c) {
            try {
                glong const raw = Double::toRawLongBits(c);
                UNSAFE::putLongUnaligned(base(), byteOffset(checkIndex(index)), raw, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        DoubleBuffer& ByteBuffer::DoubleBufferBE::compact() {
            gint pos = position();
            gint lim = limit();
            CORE_ASSERT(pos <= lim);
            gint rem = pos <= lim ? lim - pos : 0;

            try {
                ByteBuffer& db = bb.duplicate();
                db.limit(ix(lim));
                db.position(ix(0));
                ByteBuffer& sb = db.slice();
                sb.position(pos << 1);
                sb.compact();
                position(rem);
                limit(capacity());
                discardMark();
                UNSAFE::deleteInstance(db);
                UNSAFE::deleteInstance(sb);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool ByteBuffer::DoubleBufferBE::isDirect() const {
            return bb.isDirect();
        }

        gbool ByteBuffer::DoubleBufferBE::isReadOnly() const {
            return false;
        }

        String ByteBuffer::DoubleBufferBE::toString() const {
            try {
                return "DoubleBufferBE[pos="_S + position()
                        + ", lim=" + limit()
                        + ", cap=" + capacity()
                        + "]";
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder ByteBuffer::DoubleBufferBE::order() const {
            return Charset::ByteOrder::BIG_ENDIAN;
        }

        gbool ByteBuffer::DoubleBufferBE::hasArray() const {
            return false;
        }

        DoubleArray& ByteBuffer::DoubleBufferBE::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint ByteBuffer::DoubleBufferBE::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Object& ByteBuffer::DoubleBufferBE::base() const {
            if (Class<HeapByteBuffer>::hasInstance(bb))
                return CORE_XCAST(HeapByteBuffer, bb).hb;
            return null;
        }

        gint ByteBuffer::DoubleBufferBE::ix(gint i) const {
            gint off = (gint) (address - bb.address);
            return (i << 1) + off;
        }

        glong ByteBuffer::DoubleBufferBE::byteOffset(glong i) const {
            return (i << 1) + address;
        }

        // #7 Read-Only LITTLE ENDIAN

        ByteBuffer::ReadOnlyDoubleBufferLE::ReadOnlyDoubleBufferLE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              DoubleBufferLE(bb, segment) {}

        ByteBuffer::ReadOnlyDoubleBufferLE::ReadOnlyDoubleBufferLE(ByteBuffer& bb,
                                                                   gint mark, gint pos, gint lim, gint cap, glong addr,
                                                                   Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              DoubleBufferLE(bb, mark, pos, lim, cap, addr, segment) {}

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyDoubleBufferLE>(bb, -1, 0, rem, rem, addr,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyDoubleBufferLE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyDoubleBufferLE>(bb, markValue(), position(), limit(),
                                                                   capacity(), address,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::put(gdouble c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::put(gint index, gdouble c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::put(DoubleBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::put(gint index, DoubleBuffer& src, gint offset,
                                                              gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::put(
            const DoubleArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::put(const DoubleArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::put(
            gint index, const DoubleArray& src, gint offset,
            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::put(gint index, const DoubleArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferLE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyDoubleBufferLE::isReadOnly() const {
            return true;
        }

        // #8 Read-Only BIG ENDIAN

        ByteBuffer::ReadOnlyDoubleBufferBE::ReadOnlyDoubleBufferBE(
            ByteBuffer& bb, Object& segment)
            : Buffer(-1, 0, bb.remaining() >> 2, bb.remaining() >> 2, segment),
              DoubleBufferBE(bb, segment) {}

        ByteBuffer::ReadOnlyDoubleBufferBE::ReadOnlyDoubleBufferBE(ByteBuffer& bb,
                                                                   gint mark, gint pos, gint lim, gint cap, glong addr,
                                                                   Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              DoubleBufferBE(bb, mark, pos, lim, cap, addr, segment) {}

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                glong addr = byteOffset(pos);
                return UNSAFE::newInstance<ReadOnlyDoubleBufferBE>(bb, -1, 0, rem, rem, addr,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyDoubleBufferBE>(
                    bb, -1, 0, length, length, byteOffset(index), segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyDoubleBufferBE>(bb, markValue(), position(), limit(),
                                                                   capacity(), address,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::asReadOnlyBuffer() const {
            try {
                return duplicate();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::put(gdouble c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::put(gint index, gdouble c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::put(DoubleBuffer& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::put(gint index, DoubleBuffer& src, gint offset,
                                                              gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::put(
            const DoubleArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::put(const DoubleArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::put(
            gint index, const DoubleArray& src, gint offset,
            gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::put(gint index, const DoubleArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        DoubleBuffer& ByteBuffer::ReadOnlyDoubleBufferBE::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool ByteBuffer::ReadOnlyDoubleBufferBE::isReadOnly() const {
            return true;
        }
    } // io
} // core
