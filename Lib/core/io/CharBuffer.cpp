//
// Created by brunshweeck on 27 août 2024.
//

#include "CharBuffer.h"

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/UnsupportedOperationException.h>
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
        CharBuffer::CharBuffer(gint mark, gint position, gint limit, gint capacity, Object& segment)
            : Buffer(mark, position, limit, capacity, segment) {}

        Object& CharBuffer::base() const {
            return null;
        }

        CharBuffer& CharBuffer::allocate(gint capacity) {
            if (capacity < 0)
                IllegalArgumentException("Negative capacity: "_S + capacity).throws($ftrace());
            try {
                return UNSAFE::newInstance<HeapCharBuffer>(capacity, capacity, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::wrap(CharArray& array, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(offset, length, array.length());
                return UNSAFE::newInstance<HeapCharBuffer>(array, offset, length, null);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::wrap(CharArray& array) {
            try {
                return wrap(array, 0, array.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint CharBuffer::read(CharBuffer& target) {
            // Determine the number of bytes n that can be transferred
            gint limit = this->limit();
            gint pos = position();
            gint remaining = limit - pos;
            CORE_ASSERT(remaining >= 0);
            if (remaining <= 0) // include equality condition when remaining == 0
                return -1;

            gint targetRemaining = target.remaining();
            CORE_ASSERT(targetRemaining >= 0);
            if (targetRemaining <= 0) // include condition targetRemaining == 0
                return 0;

            gint n = Math::min(remaining, targetRemaining);

            try {
                // Set source limit to prevent target overflow
                if (targetRemaining < remaining)
                    CharBuffer::limit(pos + n);
                if (n > 0)
                    target.put(*this);
                this->limit(limit); // restore real limit
            } catch (Throwable const& ex) {
                try {
                    this->limit(limit); // restore real limit
                } catch (Throwable const& ex2) { ex2.throws($ftrace()); }
                ex.throws($ftrace());
            }
            return n;
        }

        CharBuffer& CharBuffer::wrap(CharSequence const& csq, gint start, gint end) {
            try {
                return UNSAFE::newInstance<StringCharBuffer>(csq, start, end);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::wrap(CharSequence const& csq) {
            try {
                return wrap(csq, 0, csq.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::get(CharArray& dst, gint offset, gint length) {
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

        CharBuffer& CharBuffer::get(CharArray& dst) {
            try {
                return get(dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::get(gint index, CharArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(index, length, limit());
                Preconditions::checkIndexFromRange(offset, length, dst.length());

                getArray(index, dst, offset, length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        CharBuffer& CharBuffer::get(gint index, CharArray& dst) {
            try {
                return get(index, dst, 0, dst.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::put(CharBuffer& src) {
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

        CharBuffer& CharBuffer::put(gint index, CharBuffer& src, gint offset, gint length) {
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

        CharBuffer& CharBuffer::put(CharArray const& src, gint offset, gint length) {
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

        CharBuffer& CharBuffer::put(CharArray const& src) {
            try {
                return put(src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::put(gint index, CharArray const& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                putArray(index, src, offset, length);
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::put(gint index, CharArray const& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::put(String const& src, gint start, gint end) {
            try {
                Preconditions::checkIndexFromRange(start, end, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (isReadOnly())
                ReadOnlyBufferException().throws($ftrace());
            if (end - start > remaining())
                BufferOverflowException().throws($ftrace());
            try {
                for (gint pos = start; pos <= end; ++pos)
                    put(src.charAt(pos));
                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::put(String const& src) {
            try {
                return put(src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint CharBuffer::position() const {
            try {
                return Buffer::position();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::position(gint newPosition) {
            try {
                Buffer::position(newPosition);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint CharBuffer::limit() const {
            try {
                return Buffer::limit();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::limit(gint newLimit) {
            try {
                Buffer::limit(newLimit);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& CharBuffer::mark() {
            try {
                Buffer::mark();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& CharBuffer::reset() {
            try {
                Buffer::reset();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& CharBuffer::clear() {
            try {
                Buffer::clear();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& CharBuffer::flip() {
            try {
                Buffer::flip();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& CharBuffer::rewind() {
            try {
                Buffer::rewind();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gbool CharBuffer::isAddressable() const {
            return false;
        }

        gint CharBuffer::hash() const {
            gint h = 1;
            gint p = position();
            try {
                for (int i = limit() - 1; i >= p; --i)
                    h = 31 * h + get(i);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return h;
        }

        gbool CharBuffer::equals(const Object& o) const {
            if (this == &o)
                return true;
            if (!Class<CharBuffer>::hasInstance(o))
                return false;
            try {
                CharBuffer const& that = CORE_XCAST(CharBuffer const, o);
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

        gint CharBuffer::compareTo(const CharBuffer& that) const {
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
                    return Character::compare(get(thisPos + i), that.get(thatPos + i));
                }
                return thisRem - thatRem;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint CharBuffer::mismatch(CharBuffer const& that) const {
            try {
                gint thisPos = position();
                gint thisRem = limit() - thisPos;
                gint thatPos = that.position();
                gint thatRem = that.limit() - thatPos;
                gint length = Math::min(thisRem, thatRem);
                if (length < 0)
                    return -1;
                gint r = Buffer::mismatch(*this, thisPos, that, thatPos, length);
                return r == -1 && thisRem != thatRem ? length : r;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint CharBuffer::length() const {
            return remaining();
        }

        gbool CharBuffer::isEmpty() const {
            return !hasRemaining();
        }

        gchar CharBuffer::charAt(gint index) const {
            CORE_TRY_RETHROW_AT(core::io::CharBuffer, return get(position() + checkIndex(index)));
        }

        CharBuffer& CharBuffer::append(const CharSequence& csq) {
            try {
                if (Class<CharBuffer>::hasInstance(csq)) {
                    CharBuffer const& cb = CORE_XCAST(CharBuffer const, csq);
                    return put((CharBuffer&) cb);
                }
                return put(csq.toString());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::append(const CharSequence& csq, gint start, gint end) {
            if (Class<CharBuffer>::hasInstance(csq)) {
                CharBuffer const& cb = CORE_XCAST(CharBuffer const, csq);

                try {
                    Preconditions::checkIndexFromRange(start, end, csq.length());
                } catch (Throwable const& ex) { ex.throws($ftrace()); }

                //
                // the append method throws BufferOverflowException when
                // there is insufficient space in the buffer
                //
                gint length = end - start;
                gint pos = position();
                gint lim = limit();
                gint rem = pos <= lim ? lim - pos : 0;
                if (length > rem)
                    BufferOverflowException().throws($ftrace());

                try {
                    put(pos, (CharBuffer&) cb, start, length);
                    position(pos + length);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            } else {
                try {
                    CharSequence& cs = csq.subSequence(start, end);
                    put(cs.toString());
                    if (&cs != &csq)
                        UNSAFE::deleteInstance(cs);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
            return *this;
        }

        CharBuffer& CharBuffer::append(gchar c) {
            try {
                return put(c);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder CharBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        void CharBuffer::getArray(gint index, CharArray& dst, gint offset, gint length) const {
            if (isAddressable() && (glong) length << 1 > 6) {
                glong bufAddr = address + ((glong) index << 1);
                glong dstOffset = UNSAFE::ARRAY_CHAR_BASE_OFFSET + ((glong) offset << 1);
                glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(base(), bufAddr, dst, dstOffset, len, Character::BYTES);
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

        CharBuffer& CharBuffer::putArray(gint index, CharArray const& src, gint offset, gint length) {
            if (isAddressable() && (glong) length << 1 > 6) {
                glong bufAddr = address + ((glong) index << 1);
                glong srcOffset = UNSAFE::ARRAY_CHAR_BASE_OFFSET + ((glong) offset << 1);
                glong len = (glong) length << 1;
                try {
                    if (order() != Charset::ByteOrder::NATIVE_ENDIAN)
                        UNSAFE::copySwapMemory(src, srcOffset, base(), bufAddr, len, Character::BYTES);
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

        void CharBuffer::putBuffer(gint pos, CharBuffer& src, gint srcPos, gint n) {
            const Object& srcBase = src.base();
            if (src.isAddressable()) {
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
            } else {
                CORE_ASSERT(Class<StringCharBuffer>::hasInstance(src));
                try {
                    for (int i = pos, j = srcPos, posMax = pos + n; i < posMax; i++, j++)
                        put(i, src.get(j));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        }

        CharArray& CharBuffer::createHeap(gint heapSize) {
            try {
                return UNSAFE::newInstance<CharArray>(heapSize);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer::HeapCharBuffer::HeapCharBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              CharBuffer(-1, 0, lim, cap, segment), hb(createHeap(cap)) {
            address = UNSAFE::ARRAY_CHAR_BASE_OFFSET;
        }

        CharBuffer::HeapCharBuffer::HeapCharBuffer(CharArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              CharBuffer(-1, off, off + len, buf.length(), segment),
              hb(buf) {
            address = UNSAFE::ARRAY_CHAR_BASE_OFFSET;
        }

        CharBuffer::HeapCharBuffer::HeapCharBuffer(CharArray& buf, gint mark, gint pos, gint lim, gint cap,
                                                   gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              CharBuffer(mark, pos, lim, cap, segment), hb(buf), offset(off) {
            address = UNSAFE::ARRAY_CHAR_BASE_OFFSET;
        }

        CharBuffer& CharBuffer::HeapCharBuffer::slice() const {
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            return UNSAFE::newInstance<HeapCharBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                       segment == null ? null : *segment);
        }

        CharBuffer& CharBuffer::HeapCharBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<HeapCharBuffer>(hb, -1, 0, length, length, index + offset,
                                                           segment == null ? null : *segment);
            } catch (Throwable const& thr) {
                thr.throws($ftrace());
            }
        }

        CharBuffer& CharBuffer::HeapCharBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<HeapCharBuffer>(hb,
                                                           markValue(),
                                                           position(),
                                                           limit(),
                                                           capacity(),
                                                           offset,
                                                           segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::HeapCharBuffer::asReadOnlyBuffer() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapCharBuffer>(hb,
                                                                   markValue(),
                                                                   position(),
                                                                   limit(),
                                                                   capacity(),
                                                                   offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar CharBuffer::HeapCharBuffer::get() {
            try {
                return hb[ix(nextGetIndex())];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar CharBuffer::HeapCharBuffer::get(gint index) const {
            try {
                return hb[ix(checkIndex(index))];
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::HeapCharBuffer::get(CharArray& dst, gint offset, gint length) {
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

        CharBuffer& CharBuffer::HeapCharBuffer::get(gint index, CharArray& dst, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromRange(offset, length, dst.length());
                Preconditions::checkIndexFromRange(index, length, limit());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            Arrays::copy(hb, ix(index), dst, offset, length);
            return *this;
        }

        gbool CharBuffer::HeapCharBuffer::isReadOnly() const {
            return false;
        }

        gbool CharBuffer::HeapCharBuffer::isDirect() const {
            return false;
        }

        CharBuffer& CharBuffer::HeapCharBuffer::put(gchar c) {
            hb[ix(nextGetIndex())] = c;
            return *this;
        }

        CharBuffer& CharBuffer::HeapCharBuffer::put(gint index, gchar c) {
            hb[ix(checkIndex(index))] = c;
            return *this;
        }

        CharBuffer& CharBuffer::HeapCharBuffer::put(const CharArray& src, gint offset, gint length) {
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

        CharBuffer& CharBuffer::HeapCharBuffer::put(gint index, const CharArray& src, gint offset, gint length) {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                Preconditions::checkIndexFromSize(offset, length, src.length());
                Arrays::copy(src, offset, hb, ix(index), length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& CharBuffer::HeapCharBuffer::put(gint index, const CharArray& src) {
            try {
                return put(index, src, 0, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::HeapCharBuffer::append(const CharSequence& csq) {
            try {
                if (Class<XString>::hasInstance(csq))
                    return appendChars(csq, 0, csq.length());
                return CharBuffer::append(csq);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::HeapCharBuffer::append(const CharSequence& csq, gint start, gint end) {
            try {
                if (Class<String>::hasInstance(csq) || Class<XString>::hasInstance(csq))
                    return appendChars(csq, start, end);
                return CharBuffer::append(csq, start, end);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::HeapCharBuffer::put(const String& src, gint start, gint end) {
            gint length = end - start;
            try {
                Preconditions::checkIndexFromSize(start, length, src.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            if (length > rem)
                BufferOverflowException().throws($ftrace());
            try {
                src.toChars(start, end, hb, ix(pos));
                position(pos + length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        CharBuffer& CharBuffer::HeapCharBuffer::compact() {
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

        CharBuffer& CharBuffer::HeapCharBuffer::subSequence(gint start, gint end) const {
            try {
                gint pos = position();
                Preconditions::checkIndexFromRange(start, end, limit() - pos);
                return UNSAFE::newInstance<HeapCharBuffer>(hb, -1, pos + start, pos + end, capacity(), offset,
                                                           segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool CharBuffer::HeapCharBuffer::hasArray() const {
            return true;
        }

        CharArray& CharBuffer::HeapCharBuffer::array() const {
            return hb;
        }

        gint CharBuffer::HeapCharBuffer::arrayOffset() const {
            return offset;
        }

        String CharBuffer::HeapCharBuffer::toString() const {
            try {
                return String(hb, offset + position(), remaining());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Charset::ByteOrder CharBuffer::HeapCharBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        gint CharBuffer::HeapCharBuffer::ix(gint i) const {
            return offset + i;
        }

        gchar CharBuffer::HeapCharBuffer::getFast(gint i) const {
            return hb[ix(i)];
        }

        Object& CharBuffer::HeapCharBuffer::base() const {
            return hb;
        }

        CharBuffer& CharBuffer::HeapCharBuffer::appendChars(CharSequence const& csq, gint start, gint end) {
            try {
                Preconditions::checkIndexFromRange(start, end, csq.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint length = end - start;
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            if (length > rem)
                BufferOverflowException().throws($ftrace());

            try {
                if (Class<String>::hasInstance(csq)) {
                    String const& str = CORE_XCAST(String const, csq);
                    str.toChars(start, end, hb, ix(pos));
                } else if (Class<XString>::hasInstance(csq)) {
                    XString const& xs = CORE_XCAST(XString const, csq);
                    xs.toChars(start, end, hb, ix(pos));
                }
                position(pos + length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        gint CharBuffer::HeapCharBuffer::charOrder() const {
            return order();
        }

        CharBuffer::ReadOnlyHeapCharBuffer::ReadOnlyHeapCharBuffer(gint cap, gint lim, Object& segment)
            : Buffer(-1, 0, lim, cap, segment),
              HeapCharBuffer(cap, lim, segment) {}

        CharBuffer::ReadOnlyHeapCharBuffer::ReadOnlyHeapCharBuffer(CharArray& buf, gint off, gint len, Object& segment)
            : Buffer(-1, off, off + len, buf.length(), segment),
              HeapCharBuffer(buf, off, len, segment) {}

        CharBuffer::ReadOnlyHeapCharBuffer::ReadOnlyHeapCharBuffer(CharArray& buf, gint mark, gint pos, gint lim,
                                                                   gint cap, gint off, Object& segment)
            : Buffer(mark, pos, lim, cap, segment),
              HeapCharBuffer(buf, mark, pos, lim, cap, off, segment) {}

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos <= lim ? lim - pos : 0;
                return UNSAFE::newInstance<ReadOnlyHeapCharBuffer>(hb, -1, 0, rem, rem, pos + offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<ReadOnlyHeapCharBuffer>(hb, -1, 0, length, length, index + offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<ReadOnlyHeapCharBuffer>(hb, markValue(), position(), limit(), capacity(),
                                                                   offset, segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::asReadOnlyBuffer() const {
            return duplicate();
        }

        gbool CharBuffer::ReadOnlyHeapCharBuffer::isReadOnly() const {
            return true;
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::put(gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::put(gint index, gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::put(const CharArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::put(const CharArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::
        put(gint index, const CharArray& src, gint offset, gint length) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::put(gint index, const CharArray& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::append(gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::append(const CharSequence& csq) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::append(const CharSequence& csq, gint start, gint end) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::put(const String& src) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::put(const String& src, gint start, gint end) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        String CharBuffer::ReadOnlyHeapCharBuffer::toString() const {
            try {
                return String(hb, offset + position(), remaining());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::subSequence(gint start, gint end) const {
            try {
                gint pos = position();
                Preconditions::checkIndexFromRange(start, end, limit() - pos);
                return UNSAFE::newInstance<ReadOnlyHeapCharBuffer>(hb, -1, pos + start, pos + end, capacity(), offset,
                                                                   segment == null ? null : *segment);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool CharBuffer::ReadOnlyHeapCharBuffer::hasArray() const {
            return false;
        }

        CharArray& CharBuffer::ReadOnlyHeapCharBuffer::array() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        gint CharBuffer::ReadOnlyHeapCharBuffer::arrayOffset() const {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::ReadOnlyHeapCharBuffer::appendChars(const CharSequence& csq, gint start, gint end) {
            try {
                Preconditions::checkIndexFromRange(start, end, csq.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint length = end - start;
            gint pos = position();
            gint lim = limit();
            gint rem = pos <= lim ? lim - pos : 0;
            if (length > rem)
                BufferOverflowException().throws($ftrace());

            try {
                if (Class<String>::hasInstance(csq)) {
                    String const& str = CORE_XCAST(String const, csq);
                    str.toChars(start, end, hb, ix(pos));
                } else if (Class<XString>::hasInstance(csq)) {
                    XString const& xs = CORE_XCAST(XString const, csq);
                    xs.toChars(start, end, hb, ix(pos));
                }
                position(pos + length);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return *this;
        }

        CharBuffer::StringCharBuffer::StringCharBuffer(CharSequence const& csq, gint start, gint end)
            : Buffer(-1, start, end, csq.length(), null),
              CharBuffer(-1, start, end, csq.length(), null), str(csq) {}

        CharBuffer::StringCharBuffer::StringCharBuffer(CharSequence const& csq,
                                                       gint mark, gint pos, gint lim, gint cap, gint off)
            : Buffer(mark, pos, lim, cap, null),
              CharBuffer(mark, pos, lim, cap, null), str(csq), offset(off) {}

        CharBuffer& CharBuffer::StringCharBuffer::slice() const {
            try {
                gint pos = position();
                gint lim = limit();
                gint rem = pos < lim ? lim - pos : 0;
                return UNSAFE::newInstance<StringCharBuffer>(str, -1, 0, rem, rem, offset + pos);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::StringCharBuffer::slice(gint index, gint length) const {
            try {
                Preconditions::checkIndexFromSize(index, length, limit());
                return UNSAFE::newInstance<StringCharBuffer>(str, -1, 0, length, length, offset + index);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool CharBuffer::StringCharBuffer::isReadOnly() const {
            return true;
        }

        CharBuffer& CharBuffer::StringCharBuffer::duplicate() const {
            try {
                return UNSAFE::newInstance<StringCharBuffer>(str, markValue(), position(), limit(), capacity(), offset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::StringCharBuffer::asReadOnlyBuffer() const {
            return duplicate();
        }

        gchar CharBuffer::StringCharBuffer::get() {
            try {
                return str.charAt(nextGetIndex() + offset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::StringCharBuffer::put(gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        gchar CharBuffer::StringCharBuffer::get(gint index) const {
            try {
                return str.charAt(checkIndex(index) + offset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        CharBuffer& CharBuffer::StringCharBuffer::put(gint index, gchar c) {
            ReadOnlyBufferException().throws($ftrace());
        }

        CharBuffer& CharBuffer::StringCharBuffer::compact() {
            ReadOnlyBufferException().throws($ftrace());
        }

        gbool CharBuffer::StringCharBuffer::isDirect() const {
            return false;
        }

        CharBuffer& CharBuffer::StringCharBuffer::subSequence(gint start, gint end) const {
            try {
                gint pos = position();
                return UNSAFE::newInstance<StringCharBuffer>(str, -1, pos + checkIndex(start, pos),
                                                             pos + checkIndex(end, pos), capacity(), offset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool CharBuffer::StringCharBuffer::hasArray() const {
            return false;
        }

        CharArray& CharBuffer::StringCharBuffer::array() const {
            UnsupportedOperationException().throws($ftrace());
        }

        gint CharBuffer::StringCharBuffer::arrayOffset() const {
            UnsupportedOperationException().throws($ftrace());
        }

        Charset::ByteOrder CharBuffer::StringCharBuffer::order() const {
            return Charset::ByteOrder::NATIVE_ENDIAN;
        }

        String CharBuffer::StringCharBuffer::toString() const {
            try {
                if (Class<String>::hasInstance(str))
                    return CORE_XCAST(String const, str).subString(offset, remaining() + offset);
                if (Class<XString>::hasInstance(str))
                    return CORE_XCAST(XString const, str).subString(offset, remaining() + offset);
                CharSequence& cs = str.subSequence(position(), limit());
                String s = cs.toString();
                if (&cs != &str)
                    UNSAFE::deleteInstance(cs);
                return UNSAFE::moveInstance(s);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gchar CharBuffer::StringCharBuffer::getFast(gint i) const {
            try {
                return str.charAt(i + offset);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint CharBuffer::StringCharBuffer::charOrder() const {
            return 0;
        }
    } // io
} // core
