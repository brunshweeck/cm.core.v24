//
// Created by brunshweeck on 2024/09/14.
//

#include <core/lang/Array.h>
#include "IntArray3D.h"
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<IntArray2D>::size();

    IntArray3D::IntArray3D(): IntArray3D(0) {
    }

    IntArray3D::IntArray3D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) IntArray2D[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D::IntArray3D(gint nbRows, IntArray2D initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) IntArray2D[nbRows];
            else {
                new(value) IntArray2D(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) IntArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D::IntArray3D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) IntArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) IntArray2D(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D::IntArray3D(gint nbRows, gint nbCols, IntArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) IntArray2D[nbRows];
            else {
                new(value) IntArray2D(nbCols, UNSAFE::moveInstance(initializer));
                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) IntArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D::IntArray3D(gint nbRows, gint nbCols, gint nbSubCols) {
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
                new(value) IntArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) IntArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D::IntArray3D(gint nbRows, gint nbCols, gint nbSubCols, gint initializer) {
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
                new(value) IntArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) IntArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray IntArray3D::set(gint ix, gint iy, IntArray newCol) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        value[ix].value[iy] = UNSAFE::moveInstance(newCol);

        return newCol;
    }

    gint &IntArray3D::get(gint ix, gint iy, gint iz) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gint const &IntArray3D::get(gint ix, gint iy, gint iz) const {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gint IntArray3D::set(gint ix, gint iy, gint iz, gint newValue) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gint oldValue = value[ix].value[iy].value[iz];
        value[ix].value[iy].value[iz] = newValue;

        return oldValue;
    }

    IntArray3D::IntArray3D(IntArray3D const &array): IntArray3D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(IntArray2D)));
            new(value) IntArray2D[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D::IntArray3D(IntArray3D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    IntArray3D::IntArray3D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) IntArray2D[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint IntArray3D::length() const { return count; }

    gbool IntArray3D::isEmpty() const { return count == 0; }

    IntArray2D &IntArray3D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D const &IntArray3D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D IntArray3D::set(gint index, IntArray2D newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray &IntArray3D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    IntArray const &IntArray3D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    IntArray3D::~IntArray3D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~IntArray2D();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String IntArray3D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &IntArray3D::clone() const {
        try {
            return UNSAFE::newInstance<IntArray3D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D const &IntArray3D::operator[](gint index) const { return get(index); }

    IntArray2D &IntArray3D::operator[](gint index) { return get(index); }

    IntArray3D &IntArray3D::operator=(IntArray3D const &array) {
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

    IntArray3D &IntArray3D::operator=(IntArray3D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    IntArray3D IntArray3D::of() { return {}; }

    IntArray3D IntArray3D::of(IntArray2D const &r) {
        try {
            return IntArray3D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1) {
        try {
            IntArray3D ia = IntArray3D(2);
            ia.value[0] = r0;
            ia.value[1] = r1;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2) {
        try {
            IntArray3D ia = IntArray3D(3);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2,
                              IntArray2D const &r3) {
        try {
            IntArray3D ia = IntArray3D(4);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2,
                              IntArray2D const &r3,
                              IntArray2D const &r4) {
        try {
            IntArray3D ia = IntArray3D(5);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            ia.value[4] = r4;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2,
                              IntArray2D const &r3,
                              IntArray2D const &r4, IntArray2D const &r5) {
        try {
            IntArray3D ia = IntArray3D(6);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            ia.value[4] = r4;
            ia.value[5] = r5;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2,
                              IntArray2D const &r3,
                              IntArray2D const &r4, IntArray2D const &r5, IntArray2D const &r6) {
        try {
            IntArray3D ia = IntArray3D(10);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            ia.value[4] = r4;
            ia.value[5] = r5;
            ia.value[6] = r6;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2,
                              IntArray2D const &r3,
                              IntArray2D const &r4, IntArray2D const &r5, IntArray2D const &r6,
                              IntArray2D const &r7) {
        try {
            IntArray3D ia = IntArray3D(6);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            ia.value[4] = r4;
            ia.value[5] = r5;
            ia.value[6] = r6;
            ia.value[7] = r7;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2,
                              IntArray2D const &r3,
                              IntArray2D const &r4, IntArray2D const &r5, IntArray2D const &r6,
                              IntArray2D const &r7,
                              IntArray2D const &r8) {
        try {
            IntArray3D ia = IntArray3D(10);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            ia.value[4] = r4;
            ia.value[5] = r5;
            ia.value[6] = r6;
            ia.value[7] = r7;
            ia.value[8] = r8;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::of(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2,
                              IntArray2D const &r3,
                              IntArray2D const &r4, IntArray2D const &r5, IntArray2D const &r6,
                              IntArray2D const &r7,
                              IntArray2D const &r8, IntArray2D const &r9) {
        try {
            IntArray3D ia = IntArray3D(10);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            ia.value[4] = r4;
            ia.value[5] = r5;
            ia.value[6] = r6;
            ia.value[7] = r7;
            ia.value[8] = r8;
            ia.value[9] = r9;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D IntArray3D::copyOf(Array<IntArray2D> const &a) {
        try {
            IntArray3D ia = IntArray3D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                ia.value[i] = a[i];
            }

            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray3D::Linear3DIterator::Linear3DIterator(IntArray3D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    IntArray3D::Linear3DIterator &IntArray3D::Linear3DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    IntArray2D &IntArray3D::Linear3DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool IntArray3D::Linear3DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator>::hasInstance(o)) return false;
        Linear3DIterator const &it = CORE_XCAST(Linear3DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    IntArray3D::Linear3DIterator2::Linear3DIterator2(IntArray3D const &array,
                                                     gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    IntArray3D::Linear3DIterator2 &IntArray3D::Linear3DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    IntArray2D const &IntArray3D::Linear3DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool IntArray3D::Linear3DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator2>::hasInstance(o)) return false;
        Linear3DIterator2 const &it = CORE_XCAST(Linear3DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    IntArray3D::Linear3DIterator IntArray3D::begin() { return Linear3DIterator(*this, false); }

    IntArray3D::Linear3DIterator2 IntArray3D::begin() const { return Linear3DIterator2(*this, false); }

    IntArray3D::Linear3DIterator IntArray3D::end() { return Linear3DIterator(*this, true); }

    IntArray3D::Linear3DIterator2 IntArray3D::end() const { return Linear3DIterator2(*this, true); }

    void IntArray3D::initializeFirstTens(IntArray2D const &r0, IntArray2D const &r1, IntArray2D const &r2,
                                         IntArray2D const &r3, IntArray2D const &r4, IntArray2D const &r5,
                                         IntArray2D const &r6, IntArray2D const &r7, IntArray2D const &r8,
                                         IntArray2D const &r9) const {
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

    gint IntArray3D::initialize(gint &i, IntArray2D const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~IntArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint IntArray3D::initialize(gint &i, IntArray2D &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~IntArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint IntArray3D::initialize(gint &i, IntArray2D &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~IntArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void IntArray3D::accumulate(...) {
    }
} // core
