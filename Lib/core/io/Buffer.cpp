//
// Created by brunshweeck on 25 août 2024.
//

#include "Buffer.h"

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/ByteBuffer.h>
#include <core/io/InvalidMarkException.h>
#include <core/misc/Preconditions.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace util;
    using namespace misc;
    using namespace charset;

    namespace io {

        Buffer::Buffer(glong address, gint capacity, Object &segment)
            : limit_(capacity), capacity_(capacity), address(address) {
            if (segment != null)
                Buffer::segment = &segment;
        }

        Buffer::Buffer(gint mark, gint pos, gint lim, gint cap, Object &segment) {
            if (segment != null)
                Buffer::segment = &segment;
            if (cap < 0)
                IllegalStateException("Negative capacity: "_S + cap).throws($ftrace());
            capacity_ = cap;
            try {
                Buffer::limit(lim);
                Buffer::position(pos);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
            if (mark >= 0) {
                if (mark > pos)
                    IllegalArgumentException("mark larger than position: "_S + mark)
                            .throws($ftrace());
                mark_ = mark;
            }
        }

        gint Buffer::capacity() const {
            return capacity_;
        }

        gint Buffer::position() const {
            return position_;
        }

        Buffer &Buffer::position(gint newPosition) {
            if (newPosition < 0)
                IllegalStateException("Negative position: "_S + newPosition).throws($ftrace());
            if (newPosition > limit_)
                IllegalStateException("Position larger than limit: "_S + newPosition).throws($ftrace());
            if (mark_ > newPosition) mark_ = -1;
            position_ = newPosition;
            return *this;
        }

        gint Buffer::limit() const {
            return limit_;
        }

        Buffer &Buffer::limit(gint newLimit) {
            if (newLimit < 0)
                IllegalStateException("Negative limit: "_S + newLimit).throws($ftrace());
            if (newLimit > capacity_)
                IllegalArgumentException("Limit larger than capacity: "_S + newLimit).throws($ftrace());
            if (position_ > newLimit) position_ = newLimit;
            limit_ = newLimit;
            return *this;
        }

        Buffer &Buffer::mark() {
            mark_ = position_;
            return *this;
        }

        Buffer &Buffer::reset() {
            gint m = mark_;
            if (m < 0)
                InvalidMarkException().throws($ftrace());
            position_ = m;
            return *this;
        }

        Buffer &Buffer::clear() {
            position_ = 0;
            limit_ = capacity_;
            mark_ = -1;
            return *this;
        }

        Buffer &Buffer::flip() {
            limit_ = position_;
            position_ = 0;
            mark_ = -1;
            return *this;
        }

        Buffer &Buffer::rewind() {
            position_ = 0;
            mark_ = -1;
            return *this;
        }

        gint Buffer::remaining() const {
            gint r = limit_ - position_;
            return r < 0 ? 0 : r;
        }

        gbool Buffer::hasRemaining() const {
            return position_ < limit_;
        }

        gint Buffer::mismatch(ByteBuffer const &a, gint aOff, ByteBuffer const &b, gint bOff, gint length) {
            try {
                gint i = 0;
                if (length > 7) {
                    if (a.get(aOff) != b.get(bOff))
                        return 0;
                    i = Arrays::vectorizedMismatch(
                        a.base(), a.address + aOff,
                        b.base(), b.address + bOff,
                        length,
                        Arrays::LOG2_BYTE_ARRAY_INDEX_SCALE);
                    if (i >= 0) return i;
                    i = length - ~i;
                }
                for (; i < length; i++) {
                    if (a.get(aOff + i) != b.get(bOff + i))
                        return i;
                }
                return -1;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::mismatch(CharBuffer const &a, gint aOff, CharBuffer const &b, gint bOff, gint length) {
            try {
                gint i = 0;
                // Ensure only heap or off-heap buffer instances use the
                // vectorized mismatch. If either buffer is a StringCharBuffer
                // (order is null) then the slow path is taken
                if (length > 3 && a.charOrder() == b.charOrder() && a.charOrder() != 0 && b.charOrder() != 0) {
                    if (a.get(aOff) != b.get(bOff))
                        return 0;
                    i = Arrays::vectorizedMismatch(
                        a.base(), a.address + (aOff << Arrays::LOG2_CHAR_ARRAY_INDEX_SCALE),
                        b.base(), b.address + (bOff << Arrays::LOG2_CHAR_ARRAY_INDEX_SCALE),
                        length,
                        Arrays::LOG2_CHAR_ARRAY_INDEX_SCALE);
                    if (i >= 0) return i;
                    i = length - ~i;
                }
                for (; i < length; i++) {
                    if (a.get(aOff + i) != b.get(bOff + i))
                        return i;
                }
                return -1;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::mismatch(ShortBuffer const &a, gint aOff, ShortBuffer const &b, gint bOff, gint length) {
            try {
                gint i = 0;
                if (length > 3 && a.order() == b.order()) {
                    if (a.get(aOff) != b.get(bOff))
                        return 0;
                    i = Arrays::vectorizedMismatch(
                            a.base(), a.address + (aOff << Arrays::LOG2_SHORT_ARRAY_INDEX_SCALE),
                            b.base(), b.address + (bOff << Arrays::LOG2_SHORT_ARRAY_INDEX_SCALE),
                            length,
                            Arrays::LOG2_SHORT_ARRAY_INDEX_SCALE);
                    if (i >= 0) return i;
                    i = length - ~i;
                }
                for (; i < length; i++) {
                    if (a.get(aOff + i) != b.get(bOff + i))
                        return i;
                }
                return -1;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::mismatch(IntBuffer const &a, gint aOff, IntBuffer const &b, gint bOff, gint length) {
            try {
                gint i = 0;
                if (length > 1 && a.order() == b.order()) {
                    if (a.get(aOff) != b.get(bOff))
                        return 0;
                    i = Arrays::vectorizedMismatch(
                            a.base(), a.address + (aOff << Arrays::LOG2_INT_ARRAY_INDEX_SCALE),
                            b.base(), b.address + (bOff << Arrays::LOG2_INT_ARRAY_INDEX_SCALE),
                            length,
                            Arrays::LOG2_INT_ARRAY_INDEX_SCALE);
                    if (i >= 0) return i;
                    i = length - ~i;
                }
                for (; i < length; i++) {
                    if (a.get(aOff + i) != b.get(bOff + i))
                        return i;
                }
                return -1;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::mismatch(FloatBuffer const &a, gint aOff, FloatBuffer const &b, gint bOff, gint length) {
            try {
                gint i = 0;
                if (length > 1 && a.order() == b.order()) {
                    if (Float::toRawIntBits(a.get(aOff)) == Float::toRawIntBits(b.get(bOff))) {
                        i = Arrays::vectorizedMismatch(
                                a.base(), a.address + (aOff << Arrays::LOG2_FLOAT_ARRAY_INDEX_SCALE),
                                b.base(), b.address + (bOff << Arrays::LOG2_FLOAT_ARRAY_INDEX_SCALE),
                                length,
                                Arrays::LOG2_FLOAT_ARRAY_INDEX_SCALE);
                    }
                    // Mismatched
                    if (i >= 0) {
                        // Check if mismatch is not associated with two NaN values; and
                        // is not associated with +0 and -0
                        gfloat av = a.get(aOff + i);
                        gfloat bv = b.get(bOff + i);
                        if (av != bv && (!Float::isNaN(av) || !Float::isNaN(bv)))
                            return i;

                        // Fall back to slow mechanism
                        // ISSUE: Consider looping over vectorizedMismatch adjusting ranges
                        // However, requires that returned value be relative to input ranges
                        i++;
                    }
                    // Matched
                    else {
                        i = length - ~i;
                    }
                }
                for (; i < length; i++) {
                    gfloat av = a.get(aOff + i);
                    gfloat bv = b.get(bOff + i);
                    if (av != bv && (!Float::isNaN(av) || !Float::isNaN(bv)))
                        return i;
                }
                return -1;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::mismatch(LongBuffer const &a, gint aOff, LongBuffer const &b, gint bOff, gint length) {
            try {
                gint i = 0;
                if (length > 0 && a.order() == b.order()) {
                    if (a.get(aOff) != b.get(bOff))
                        return 0;
                    i = Arrays::vectorizedMismatch(
                            a.base(), a.address + (aOff << Arrays::LOG2_LONG_ARRAY_INDEX_SCALE),
                            b.base(), b.address + (bOff << Arrays::LOG2_LONG_ARRAY_INDEX_SCALE),
                            length,
                            Arrays::LOG2_LONG_ARRAY_INDEX_SCALE);
                    return i >= 0 ? i : -1;
                }
                for (; i < length; i++) {
                    if (a.get(aOff + i) != b.get(bOff + i))
                        return i;
                }
                return -1;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::mismatch(DoubleBuffer const &a, gint aOff, DoubleBuffer const &b, gint bOff, gint length) {
            try {
                gint i = 0;
                if (length > 0 && a.order() == b.order()) {
                    if (Double::toRawLongBits(a.get(aOff)) == Double::toRawLongBits(b.get(bOff))) {
                        i = Arrays::vectorizedMismatch(
                                a.base(), a.address + (aOff << Arrays::LOG2_DOUBLE_ARRAY_INDEX_SCALE),
                                b.base(), b.address + (bOff << Arrays::LOG2_DOUBLE_ARRAY_INDEX_SCALE),
                                length,
                                Arrays::LOG2_DOUBLE_ARRAY_INDEX_SCALE);
                    }
                    // Mismatched
                    if (i >= 0) {
                        // Check if mismatch is not associated with two NaN values; and
                        // is not associated with +0 and -0
                        gdouble av = a.get(aOff + i);
                        gdouble bv = b.get(bOff + i);
                        if (av != bv && (!Double::isNaN(av) || !Double::isNaN(bv)))
                            return i;

                        // Fall back to slow mechanism
                        // ISSUE: Consider looping over vectorizedMismatch adjusting ranges
                        // However, requires that returned value be relative to input ranges
                        i++;
                    }
                    // Matched
                    else {
                        return -1;
                    }
                }
                for (; i < length; i++) {
                    gdouble av = a.get(aOff + i);
                    gdouble bv = b.get(bOff + i);
                    if (av != bv && (!Double::isNaN(av) || !Double::isNaN(bv)))
                        return i;
                }
                return -1;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::nextGetIndex() {
            gint p = position_;
            if (p >= limit_)
                BufferUnderflowException().throws($ftrace());
            position_ = p + 1;
            return p;
        }

        gint Buffer::nextGetIndex(gint nb) {
            gint p = position_;
            if (limit_ - p < nb)
                BufferUnderflowException().throws($ftrace());
            position_ = p + nb;
            return p;
        }

        gint Buffer::nextPutIndex() {
            gint p = position_;
            if (p >= limit_)
                BufferOverflowException().throws($ftrace());
            position_ = p + 1;
            return p;
        }

        gint Buffer::nextPutIndex(gint nb) {
            gint p = position_;
            if (limit_ - p < nb)
                BufferOverflowException().throws($ftrace());
            position_ = p + nb;
            return p;
        }

        gint Buffer::checkIndex(gint i) const {
            try {
                return Preconditions::checkIndex(i, limit_);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::checkIndex(gint i, gint nb) const {
            try {
                return Preconditions::checkIndex(i, limit_ - nb + 1);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Buffer::markValue() const {
            return mark_;
        }

        void Buffer::discardMark() {
            mark_ = -1;
        }
    } // io
} // core
