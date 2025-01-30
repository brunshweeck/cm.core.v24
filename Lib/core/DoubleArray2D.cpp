//
// Created by brunshweeck on 2024/09/14.
//

#include <core/Array.h>
#include <core/DoubleArray2D.h>
#include <core/IllegalArgumentException.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<DoubleArray>::size();

    DoubleArray2D::DoubleArray2D(): DoubleArray2D(0) {
    }

    DoubleArray2D::DoubleArray2D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) DoubleArray[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D::DoubleArray2D(gint nbRows, DoubleArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) DoubleArray[nbRows];
            else {
                new(value) DoubleArray(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) DoubleArray(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D::DoubleArray2D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) DoubleArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) DoubleArray(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D::DoubleArray2D(gint nbRows, gint nbCols, gdouble initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) DoubleArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) DoubleArray(nbCols, initializer);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D::DoubleArray2D(DoubleArray2D const &array): DoubleArray2D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(DoubleArray)));
            new(value) DoubleArray[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D::DoubleArray2D(DoubleArray2D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    DoubleArray2D::DoubleArray2D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) DoubleArray[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint DoubleArray2D::length() const { return count; }

    gbool DoubleArray2D::isEmpty() const { return count == 0; }

    DoubleArray &DoubleArray2D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray const &DoubleArray2D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray DoubleArray2D::set(gint index, DoubleArray newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gdouble &DoubleArray2D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gdouble const &DoubleArray2D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gdouble DoubleArray2D::set(gint ix, gint iy, gdouble newValue) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gdouble oldValue = value[ix].value[iy];
        value[ix].value[iy] = newValue;

        return oldValue;
    }

    DoubleArray2D::~DoubleArray2D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~DoubleArray();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String DoubleArray2D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &DoubleArray2D::clone() const {
        try {
            return UNSAFE::newInstance<DoubleArray2D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray const &DoubleArray2D::operator[](gint index) const { return get(index); }

    DoubleArray &DoubleArray2D::operator[](gint index) { return get(index); }

    DoubleArray2D &DoubleArray2D::operator=(DoubleArray2D const &array) {
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

    DoubleArray2D &DoubleArray2D::operator=(DoubleArray2D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    DoubleArray DoubleArray2D::of() { return {}; }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r) {
        try {
            return DoubleArray2D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1) {
        try {
            DoubleArray2D da = DoubleArray2D(2);
            da.value[0] = r0;
            da.value[1] = r1;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2) {
        try {
            DoubleArray2D da = DoubleArray2D(3);
            da.value[0] = r0;
            da.value[1] = r1;
            da.value[2] = r2;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2,
                                    DoubleArray const &r3) {
        try {
            DoubleArray2D da = DoubleArray2D(4);
            da.value[0] = r0;
            da.value[1] = r1;
            da.value[2] = r2;
            da.value[3] = r3;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2,
                                    DoubleArray const &r3,
                                    DoubleArray const &r4) {
        try {
            DoubleArray2D da = DoubleArray2D(5);
            da.value[0] = r0;
            da.value[1] = r1;
            da.value[2] = r2;
            da.value[3] = r3;
            da.value[4] = r4;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2,
                                    DoubleArray const &r3,
                                    DoubleArray const &r4, DoubleArray const &r5) {
        try {
            DoubleArray2D da = DoubleArray2D(6);
            da.value[0] = r0;
            da.value[1] = r1;
            da.value[2] = r2;
            da.value[3] = r3;
            da.value[4] = r4;
            da.value[5] = r5;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2,
                                    DoubleArray const &r3,
                                    DoubleArray const &r4, DoubleArray const &r5, DoubleArray const &r6) {
        try {
            DoubleArray2D da = DoubleArray2D(10);
            da.value[0] = r0;
            da.value[1] = r1;
            da.value[2] = r2;
            da.value[3] = r3;
            da.value[4] = r4;
            da.value[5] = r5;
            da.value[6] = r6;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2,
                                    DoubleArray const &r3,
                                    DoubleArray const &r4, DoubleArray const &r5, DoubleArray const &r6,
                                    DoubleArray const &r7) {
        try {
            DoubleArray2D da = DoubleArray2D(6);
            da.value[0] = r0;
            da.value[1] = r1;
            da.value[2] = r2;
            da.value[3] = r3;
            da.value[4] = r4;
            da.value[5] = r5;
            da.value[6] = r6;
            da.value[7] = r7;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2,
                                    DoubleArray const &r3,
                                    DoubleArray const &r4, DoubleArray const &r5, DoubleArray const &r6,
                                    DoubleArray const &r7,
                                    DoubleArray const &r8) {
        try {
            DoubleArray2D da = DoubleArray2D(10);
            da.value[0] = r0;
            da.value[1] = r1;
            da.value[2] = r2;
            da.value[3] = r3;
            da.value[4] = r4;
            da.value[5] = r5;
            da.value[6] = r6;
            da.value[7] = r7;
            da.value[8] = r8;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::of(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2,
                                    DoubleArray const &r3,
                                    DoubleArray const &r4, DoubleArray const &r5, DoubleArray const &r6,
                                    DoubleArray const &r7,
                                    DoubleArray const &r8, DoubleArray const &r9) {
        try {
            DoubleArray2D da = DoubleArray2D(10);
            da.value[0] = r0;
            da.value[1] = r1;
            da.value[2] = r2;
            da.value[3] = r3;
            da.value[4] = r4;
            da.value[5] = r5;
            da.value[6] = r6;
            da.value[7] = r7;
            da.value[8] = r8;
            da.value[9] = r9;
            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D DoubleArray2D::copyOf(Array<DoubleArray> const &a) {
        try {
            DoubleArray2D da = DoubleArray2D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                da.value[i] = a[i];
            }

            return da;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray2D::Linear2DIterator::Linear2DIterator(DoubleArray2D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    DoubleArray2D::Linear2DIterator &DoubleArray2D::Linear2DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    DoubleArray &DoubleArray2D::Linear2DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool DoubleArray2D::Linear2DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator>::hasInstance(o)) return false;
        Linear2DIterator const &it = CORE_XCAST(Linear2DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    DoubleArray2D::Linear2DIterator2::Linear2DIterator2(DoubleArray2D const &array, gbool isEnd)
        : array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    DoubleArray2D::Linear2DIterator2 &DoubleArray2D::Linear2DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    DoubleArray const &DoubleArray2D::Linear2DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool DoubleArray2D::Linear2DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator2>::hasInstance(o)) return false;
        Linear2DIterator2 const &it = CORE_XCAST(Linear2DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    DoubleArray2D::Linear2DIterator DoubleArray2D::begin() { return Linear2DIterator(*this, false); }

    DoubleArray2D::Linear2DIterator2 DoubleArray2D::begin() const { return Linear2DIterator2(*this, false); }

    DoubleArray2D::Linear2DIterator DoubleArray2D::end() { return Linear2DIterator(*this, true); }

    DoubleArray2D::Linear2DIterator2 DoubleArray2D::end() const { return Linear2DIterator2(*this, true); }

    void DoubleArray2D::initializeFirstTens(DoubleArray const &r0, DoubleArray const &r1, DoubleArray const &r2,
                                            DoubleArray const &r3, DoubleArray const &r4, DoubleArray const &r5,
                                            DoubleArray const &r6, DoubleArray const &r7, DoubleArray const &r8,
                                            DoubleArray const &r9) {
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

    gint DoubleArray2D::initialize(gint &i, DoubleArray const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~DoubleArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint DoubleArray2D::initialize(gint &i, DoubleArray &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~DoubleArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint DoubleArray2D::initialize(gint &i, DoubleArray &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~DoubleArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void DoubleArray2D::accumulate(...) {
    }
} // core
