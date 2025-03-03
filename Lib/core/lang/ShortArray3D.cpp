//
// Created by brunshweeck on 2024/09/14.
//

#include <core/lang/Array.h>
#include "ShortArray3D.h"
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<ShortArray2D>::size();

    ShortArray3D::ShortArray3D(): ShortArray3D(0) {
    }

    ShortArray3D::ShortArray3D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) ShortArray2D[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D::ShortArray3D(gint nbRows, ShortArray2D initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) ShortArray2D[nbRows];
            else {
                new(value) ShortArray2D(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) ShortArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D::ShortArray3D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ShortArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ShortArray2D(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D::ShortArray3D(gint nbRows, gint nbCols, ShortArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ShortArray2D[nbRows];
            else {
                new(value) ShortArray2D(nbCols, UNSAFE::moveInstance(initializer));
                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) ShortArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D::ShortArray3D(gint nbRows, gint nbCols, gint nbSubCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbSubCols < 0)
            IllegalArgumentException("Negative number of sub-columns: "_S + nbSubCols).throws(
                $ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ShortArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ShortArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D::ShortArray3D(gint nbRows, gint nbCols, gint nbSubCols, gshort initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbSubCols < 0)
            IllegalArgumentException("Negative number of sub-columns: "_S + nbSubCols).throws(
                $ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ShortArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ShortArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray ShortArray3D::set(gint ix, gint iy, ShortArray newCol) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        value[ix].value[iy] = UNSAFE::moveInstance(newCol);

        return newCol;
    }

    gshort &ShortArray3D::get(gint ix, gint iy, gint iz) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gshort const &ShortArray3D::get(gint ix, gint iy, gint iz) const {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gshort ShortArray3D::set(gint ix, gint iy, gint iz, gshort newValue) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gshort oldValue = value[ix].value[iy].value[iz];
        value[ix].value[iy].value[iz] = newValue;

        return oldValue;
    }

    ShortArray3D::ShortArray3D(ShortArray3D const &array): ShortArray3D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(ShortArray2D)));
            new(value) ShortArray2D[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D::ShortArray3D(ShortArray3D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    ShortArray3D::ShortArray3D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) ShortArray2D[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint ShortArray3D::length() const { return count; }

    gbool ShortArray3D::isEmpty() const { return count == 0; }

    ShortArray2D &ShortArray3D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D const &ShortArray3D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D ShortArray3D::set(gint index, ShortArray2D newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray &ShortArray3D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    ShortArray const &ShortArray3D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    ShortArray3D::~ShortArray3D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~ShortArray2D();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String ShortArray3D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &ShortArray3D::clone() const {
        try {
            return UNSAFE::newInstance<ShortArray3D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray2D const &ShortArray3D::operator[](gint index) const { return get(index); }

    ShortArray2D &ShortArray3D::operator[](gint index) { return get(index); }

    ShortArray3D &ShortArray3D::operator=(ShortArray3D const &array) {
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

    ShortArray3D &ShortArray3D::operator=(ShortArray3D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    ShortArray3D ShortArray3D::of() { return {}; }

    ShortArray3D ShortArray3D::of(ShortArray2D const &r) {
        try {
            return ShortArray3D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1) {
        try {
            ShortArray3D sa = ShortArray3D(2);
            sa.value[0] = r0;
            sa.value[1] = r1;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2) {
        try {
            ShortArray3D sa = ShortArray3D(3);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2,
                                  ShortArray2D const &r3) {
        try {
            ShortArray3D sa = ShortArray3D(4);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2,
                                  ShortArray2D const &r3,
                                  ShortArray2D const &r4) {
        try {
            ShortArray3D sa = ShortArray3D(5);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            sa.value[4] = r4;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2,
                                  ShortArray2D const &r3,
                                  ShortArray2D const &r4, ShortArray2D const &r5) {
        try {
            ShortArray3D sa = ShortArray3D(6);
            sa.value[0] = r0;
            sa.value[1] = r1;
            sa.value[2] = r2;
            sa.value[3] = r3;
            sa.value[4] = r4;
            sa.value[5] = r5;
            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2,
                                  ShortArray2D const &r3,
                                  ShortArray2D const &r4, ShortArray2D const &r5, ShortArray2D const &r6) {
        try {
            ShortArray3D sa = ShortArray3D(10);
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

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2,
                                  ShortArray2D const &r3,
                                  ShortArray2D const &r4, ShortArray2D const &r5, ShortArray2D const &r6,
                                  ShortArray2D const &r7) {
        try {
            ShortArray3D sa = ShortArray3D(6);
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

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2,
                                  ShortArray2D const &r3,
                                  ShortArray2D const &r4, ShortArray2D const &r5, ShortArray2D const &r6,
                                  ShortArray2D const &r7,
                                  ShortArray2D const &r8) {
        try {
            ShortArray3D sa = ShortArray3D(10);
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

    ShortArray3D ShortArray3D::of(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2,
                                  ShortArray2D const &r3,
                                  ShortArray2D const &r4, ShortArray2D const &r5, ShortArray2D const &r6,
                                  ShortArray2D const &r7,
                                  ShortArray2D const &r8, ShortArray2D const &r9) {
        try {
            ShortArray3D sa = ShortArray3D(10);
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

    ShortArray3D ShortArray3D::copyOf(Array<ShortArray2D> const &a) {
        try {
            ShortArray3D sa = ShortArray3D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                sa.value[i] = a[i];
            }

            return sa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray3D::Linear3DIterator::Linear3DIterator(ShortArray3D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    ShortArray3D::Linear3DIterator &ShortArray3D::Linear3DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    ShortArray2D &ShortArray3D::Linear3DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool ShortArray3D::Linear3DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator>::hasInstance(o)) return false;
        Linear3DIterator const &it = CORE_XCAST(Linear3DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    ShortArray3D::Linear3DIterator2::Linear3DIterator2(ShortArray3D const &array,
                                                       gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    ShortArray3D::Linear3DIterator2 &ShortArray3D::Linear3DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    ShortArray2D const &ShortArray3D::Linear3DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool ShortArray3D::Linear3DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator2>::hasInstance(o)) return false;
        Linear3DIterator2 const &it = CORE_XCAST(Linear3DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    ShortArray3D::Linear3DIterator ShortArray3D::begin() { return Linear3DIterator(*this, false); }

    ShortArray3D::Linear3DIterator2 ShortArray3D::begin() const { return Linear3DIterator2(*this, false); }

    ShortArray3D::Linear3DIterator ShortArray3D::end() { return Linear3DIterator(*this, true); }

    ShortArray3D::Linear3DIterator2 ShortArray3D::end() const { return Linear3DIterator2(*this, true); }

    void ShortArray3D::initializeFirstTens(ShortArray2D const &r0, ShortArray2D const &r1, ShortArray2D const &r2,
                                           ShortArray2D const &r3, ShortArray2D const &r4, ShortArray2D const &r5,
                                           ShortArray2D const &r6, ShortArray2D const &r7, ShortArray2D const &r8,
                                           ShortArray2D const &r9) const {
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

    gint ShortArray3D::initialize(gint &i, ShortArray2D const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ShortArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint ShortArray3D::initialize(gint &i, ShortArray2D &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ShortArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint ShortArray3D::initialize(gint &i, ShortArray2D &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ShortArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void ShortArray3D::accumulate(...) {
    }
} // core
