//
// Created by brunshweeck on 2024/09/14.
//

#include <core/lang/Array.h>
#include "ShortArray2D.h"
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<ShortArray>::size();

    ShortArray2D::ShortArray2D(): ShortArray2D(0) {
    }

    ShortArray2D::ShortArray2D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) ShortArray[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D::ShortArray2D(gint nbRows, ShortArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) ShortArray[nbRows];
            else {
                new(value) ShortArray(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) ShortArray(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D::ShortArray2D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ShortArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ShortArray(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D::ShortArray2D(gint nbRows, gint nbCols, gshort initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ShortArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ShortArray(nbCols, initializer);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D::ShortArray2D(ShortArray2D const &array): ShortArray2D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(ShortArray)));
            new(value) ShortArray[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D::ShortArray2D(ShortArray2D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    ShortArray2D::ShortArray2D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) ShortArray[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint ShortArray2D::length() const { return count; }

    gbool ShortArray2D::isEmpty() const { return count == 0; }

    ShortArray &ShortArray2D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray const &ShortArray2D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray ShortArray2D::set(gint index, ShortArray newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gshort &ShortArray2D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gshort const &ShortArray2D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gshort ShortArray2D::set(gint ix, gint iy, gshort newValue) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gshort oldValue = value[ix].value[iy];
        value[ix].value[iy] = newValue;

        return oldValue;
    }

    ShortArray2D::~ShortArray2D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~ShortArray();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String ShortArray2D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &ShortArray2D::clone() const {
        try {
            return UNSAFE::newInstance<ShortArray2D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray const &ShortArray2D::operator[](gint index) const { return get(index); }

    ShortArray &ShortArray2D::operator[](gint index) { return get(index); }

    ShortArray2D &ShortArray2D::operator=(ShortArray2D const &array) {
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

    ShortArray2D &ShortArray2D::operator=(ShortArray2D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    ShortArray ShortArray2D::of() { return {}; }

    ShortArray2D ShortArray2D::of(ShortArray const &r) {
        try {
            return ShortArray2D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1) {
        try {
            ShortArray2D sa = ShortArray2D(2);
            sa.value[0] = r0;
            sa.value[1] = r1;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2) {
        try {
            ShortArray2D sa = ShortArray2D(3);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2,
                                  ShortArray const &r3) {
        try {
            ShortArray2D sa = ShortArray2D(4);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2,
                                  ShortArray const &r3,
                                  ShortArray const &r4) {
        try {
            ShortArray2D sa = ShortArray2D(5);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            sa.value[4] = r4;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2,
                                  ShortArray const &r3,
                                  ShortArray const &r4, ShortArray const &r5) {
        try {
            ShortArray2D sa = ShortArray2D(6);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            sa.value[4] = r4;
            sa.value[5] = r5;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2,
                                  ShortArray const &r3,
                                  ShortArray const &r4, ShortArray const &r5, ShortArray const &r6) {
        try {
            ShortArray2D sa = ShortArray2D(10);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            sa.value[4] = r4;
            sa.value[5] = r5;
            sa.value[6] = r6;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2,
                                  ShortArray const &r3,
                                  ShortArray const &r4, ShortArray const &r5, ShortArray const &r6,
                                  ShortArray const &r7) {
        try {
            ShortArray2D sa = ShortArray2D(6);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            sa.value[4] = r4;
            sa.value[5] = r5;
            sa.value[6] = r6;
            sa.value[7] = r7;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2,
                                  ShortArray const &r3,
                                  ShortArray const &r4, ShortArray const &r5, ShortArray const &r6,
                                  ShortArray const &r7,
                                  ShortArray const &r8) {
        try {
            ShortArray2D sa = ShortArray2D(10);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            sa.value[4] = r4;
            sa.value[5] = r5;
            sa.value[6] = r6;
            sa.value[7] = r7;
            sa.value[8] = r8;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::of(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2,
                                  ShortArray const &r3,
                                  ShortArray const &r4, ShortArray const &r5, ShortArray const &r6,
                                  ShortArray const &r7,
                                  ShortArray const &r8, ShortArray const &r9) {
        try {
            ShortArray2D sa = ShortArray2D(10);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            sa.value[4] = r4;
            sa.value[5] = r5;
            sa.value[6] = r6;
            sa.value[7] = r7;
            sa.value[8] = r8;
            sa.value[9] = r9;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray2D::copyOf(Array<ShortArray> const &a) {
        try {
            ShortArray2D sa = ShortArray2D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                sa.value[i] = a[i];
            }

            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D::Linear2DIterator::Linear2DIterator(ShortArray2D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    ShortArray2D::Linear2DIterator &ShortArray2D::Linear2DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    ShortArray &ShortArray2D::Linear2DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool ShortArray2D::Linear2DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator>::hasInstance(o)) return false;
        Linear2DIterator const &it = CORE_XCAST(Linear2DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    ShortArray2D::Linear2DIterator2::Linear2DIterator2(ShortArray2D const &array, gbool isEnd)
        : array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    ShortArray2D::Linear2DIterator2 &ShortArray2D::Linear2DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    ShortArray const &ShortArray2D::Linear2DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool ShortArray2D::Linear2DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator2>::hasInstance(o)) return false;
        Linear2DIterator2 const &it = CORE_XCAST(Linear2DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    ShortArray2D::Linear2DIterator ShortArray2D::begin() { return Linear2DIterator(*this, false); }

    ShortArray2D::Linear2DIterator2 ShortArray2D::begin() const { return Linear2DIterator2(*this, false); }

    ShortArray2D::Linear2DIterator ShortArray2D::end() { return Linear2DIterator(*this, true); }

    ShortArray2D::Linear2DIterator2 ShortArray2D::end() const { return Linear2DIterator2(*this, true); }

    void ShortArray2D::initializeFirstTens(ShortArray const &r0, ShortArray const &r1, ShortArray const &r2,
                                           ShortArray const &r3, ShortArray const &r4, ShortArray const &r5,
                                           ShortArray const &r6, ShortArray const &r7, ShortArray const &r8,
                                           ShortArray const &r9) {
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

    gint ShortArray2D::initialize(gint &i, ShortArray const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ShortArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint ShortArray2D::initialize(gint &i, ShortArray &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ShortArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint ShortArray2D::initialize(gint &i, ShortArray &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ShortArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void ShortArray2D::accumulate(...) {
    }
} // core
