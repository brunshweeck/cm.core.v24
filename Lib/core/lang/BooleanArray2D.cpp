//
// Created by brunshweeck on 2024/09/14.
//

#include "BooleanArray2D.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<BooleanArray>::size();

    BooleanArray2D::BooleanArray2D(): BooleanArray2D(0) {
    }

    BooleanArray2D::BooleanArray2D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) BooleanArray[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D::BooleanArray2D(gint nbRows, BooleanArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) BooleanArray[nbRows];
            else {
                new(value) BooleanArray(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) BooleanArray(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D::BooleanArray2D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) BooleanArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) BooleanArray(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D::BooleanArray2D(gint nbRows, gint nbCols, gbool initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) BooleanArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) BooleanArray(nbCols, initializer);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D::BooleanArray2D(BooleanArray2D const &array): BooleanArray2D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(BooleanArray)));
            new(value) BooleanArray[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D::BooleanArray2D(BooleanArray2D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    BooleanArray2D::BooleanArray2D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) BooleanArray[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint BooleanArray2D::length() const { return count; }

    gbool BooleanArray2D::isEmpty() const { return count == 0; }

    BooleanArray &BooleanArray2D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray const &BooleanArray2D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray BooleanArray2D::set(gint index, BooleanArray newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gbool &BooleanArray2D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gbool const &BooleanArray2D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gbool BooleanArray2D::set(gint ix, gint iy, gbool newValue) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gbool oldValue = value[ix].value[iy];
        value[ix].value[iy] = newValue;

        return oldValue;
    }

    BooleanArray2D::~BooleanArray2D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~BooleanArray();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String BooleanArray2D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &BooleanArray2D::clone() const {
        try {
            return UNSAFE::newInstance<BooleanArray2D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray const &BooleanArray2D::operator[](gint index) const { return get(index); }

    BooleanArray &BooleanArray2D::operator[](gint index) { return get(index); }

    BooleanArray2D &BooleanArray2D::operator=(BooleanArray2D const &array) {
        try {
            if (this != &array) {
                if (count < array.count)
                    value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                          array.count * ARRAY_ROW_SIZE));
                else if (array.count == 0) {
                    UNSAFE::freeMemory(CORE_CAST(glong, value));
                    value = null;
                }

                count = array.count;
                for (int i = 0; i < count; i++) value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
        return *this;
    }

    BooleanArray2D &BooleanArray2D::operator=(BooleanArray2D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    BooleanArray BooleanArray2D::of() { return {}; }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r) {
        try {
            return BooleanArray2D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1) {
        try {
            BooleanArray2D ba = BooleanArray2D(2);
            ba.value[0] = r0;
            ba.value[1] = r1;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2) {
        try {
            BooleanArray2D ba = BooleanArray2D(3);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2,
                                      BooleanArray const &r3) {
        try {
            BooleanArray2D ba = BooleanArray2D(4);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2,
                                      BooleanArray const &r3,
                                      BooleanArray const &r4) {
        try {
            BooleanArray2D ba = BooleanArray2D(5);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2,
                                      BooleanArray const &r3,
                                      BooleanArray const &r4, BooleanArray const &r5) {
        try {
            BooleanArray2D ba = BooleanArray2D(6);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            ba.value[5] = r5;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2,
                                      BooleanArray const &r3,
                                      BooleanArray const &r4, BooleanArray const &r5, BooleanArray const &r6) {
        try {
            BooleanArray2D ba = BooleanArray2D(10);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            ba.value[5] = r5;
            ba.value[6] = r6;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2,
                                      BooleanArray const &r3,
                                      BooleanArray const &r4, BooleanArray const &r5, BooleanArray const &r6,
                                      BooleanArray const &r7) {
        try {
            BooleanArray2D ba = BooleanArray2D(6);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            ba.value[5] = r5;
            ba.value[6] = r6;
            ba.value[7] = r7;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2,
                                      BooleanArray const &r3,
                                      BooleanArray const &r4, BooleanArray const &r5, BooleanArray const &r6,
                                      BooleanArray const &r7,
                                      BooleanArray const &r8) {
        try {
            BooleanArray2D ba = BooleanArray2D(10);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            ba.value[5] = r5;
            ba.value[6] = r6;
            ba.value[7] = r7;
            ba.value[8] = r8;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::of(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2,
                                      BooleanArray const &r3,
                                      BooleanArray const &r4, BooleanArray const &r5, BooleanArray const &r6,
                                      BooleanArray const &r7,
                                      BooleanArray const &r8, BooleanArray const &r9) {
        try {
            BooleanArray2D ba = BooleanArray2D(10);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            ba.value[5] = r5;
            ba.value[6] = r6;
            ba.value[7] = r7;
            ba.value[8] = r8;
            ba.value[9] = r9;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray2D::copyOf(Array<BooleanArray> const &a) {
        try {
            BooleanArray2D ba = BooleanArray2D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                ba.value[i] = a[i];
            }

            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D::Linear2DIterator::Linear2DIterator(BooleanArray2D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    BooleanArray2D::Linear2DIterator &BooleanArray2D::Linear2DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    BooleanArray &BooleanArray2D::Linear2DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool BooleanArray2D::Linear2DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator>::hasInstance(o)) return false;
        Linear2DIterator const &it = CORE_XCAST(Linear2DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    BooleanArray2D::Linear2DIterator2::Linear2DIterator2(BooleanArray2D const &array, gbool isEnd)
        : array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    BooleanArray2D::Linear2DIterator2 &BooleanArray2D::Linear2DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    BooleanArray const &BooleanArray2D::Linear2DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool BooleanArray2D::Linear2DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator2>::hasInstance(o)) return false;
        Linear2DIterator2 const &it = CORE_XCAST(Linear2DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    BooleanArray2D::Linear2DIterator BooleanArray2D::begin() { return Linear2DIterator(*this, false); }

    BooleanArray2D::Linear2DIterator2 BooleanArray2D::begin() const { return Linear2DIterator2(*this, false); }

    BooleanArray2D::Linear2DIterator BooleanArray2D::end() { return Linear2DIterator(*this, true); }

    BooleanArray2D::Linear2DIterator2 BooleanArray2D::end() const { return Linear2DIterator2(*this, true); }

    void BooleanArray2D::initializeFirstTens(BooleanArray const &r0, BooleanArray const &r1, BooleanArray const &r2,
                                             BooleanArray const &r3, BooleanArray const &r4, BooleanArray const &r5,
                                             BooleanArray const &r6, BooleanArray const &r7, BooleanArray const &r8,
                                             BooleanArray const &r9) {
        try {
            value[0] = r0;
            value[1] = r1;
            value[2] = r2;
            value[3] = r3;
            value[4] = r4;
            value[5] = r5;
            value[6] = r6;
            value[7] = r7;
            value[8] = r8;
            value[9] = r9;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint BooleanArray2D::initialize(gint &i, BooleanArray const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~BooleanArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint BooleanArray2D::initialize(gint &i, BooleanArray &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~BooleanArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint BooleanArray2D::initialize(gint &i, BooleanArray &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~BooleanArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void BooleanArray2D::accumulate(...) {
    }
} // core
