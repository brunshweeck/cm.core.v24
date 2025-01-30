//
// Created by brunshweeck on 2024/09/14.
//

#include <core/BooleanArray3D.h>
#include <core/IllegalArgumentException.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<BooleanArray2D>::size();

    BooleanArray3D::BooleanArray3D(): BooleanArray3D(0) {
    }

    BooleanArray3D::BooleanArray3D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) BooleanArray2D[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D::BooleanArray3D(gint nbRows, BooleanArray2D initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) BooleanArray2D[nbRows];
            else {
                new(value) BooleanArray2D(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) BooleanArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D::BooleanArray3D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) BooleanArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) BooleanArray2D(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D::BooleanArray3D(gint nbRows, gint nbCols, BooleanArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) BooleanArray2D[nbRows];
            else {
                new(value) BooleanArray2D(nbCols, UNSAFE::moveInstance(initializer));
                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) BooleanArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D::BooleanArray3D(gint nbRows, gint nbCols, gint nbSubCols) {
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
                new(value) BooleanArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) BooleanArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D::BooleanArray3D(gint nbRows, gint nbCols, gint nbSubCols, gbool initializer) {
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
                new(value) BooleanArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) BooleanArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray BooleanArray3D::set(gint ix, gint iy, BooleanArray newCol) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        value[ix].value[iy] = UNSAFE::moveInstance(newCol);

        return newCol;
    }

    gbool &BooleanArray3D::get(gint ix, gint iy, gint iz) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gbool const &BooleanArray3D::get(gint ix, gint iy, gint iz) const {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gbool BooleanArray3D::set(gint ix, gint iy, gint iz, gbool newValue) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gbool oldValue = value[ix].value[iy].value[iz];
        value[ix].value[iy].value[iz] = newValue;

        return oldValue;
    }

    BooleanArray3D::BooleanArray3D(BooleanArray3D const &array): BooleanArray3D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(BooleanArray2D)));
            new(value) BooleanArray2D[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D::BooleanArray3D(BooleanArray3D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    BooleanArray3D::BooleanArray3D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) BooleanArray2D[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint BooleanArray3D::length() const { return count; }

    gbool BooleanArray3D::isEmpty() const { return count == 0; }

    BooleanArray2D &BooleanArray3D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D const &BooleanArray3D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D BooleanArray3D::set(gint index, BooleanArray2D newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray &BooleanArray3D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    BooleanArray const &BooleanArray3D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    BooleanArray3D::~BooleanArray3D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~BooleanArray2D();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String BooleanArray3D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &BooleanArray3D::clone() const {
        try {
            return UNSAFE::newInstance<BooleanArray3D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray2D const &BooleanArray3D::operator[](gint index) const { return get(index); }

    BooleanArray2D &BooleanArray3D::operator[](gint index) { return get(index); }

    BooleanArray3D &BooleanArray3D::operator=(BooleanArray3D const &array) {
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

    BooleanArray3D &BooleanArray3D::operator=(BooleanArray3D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    BooleanArray3D BooleanArray3D::of() { return {}; }

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r) {
        try {
            return BooleanArray3D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1) {
        try {
            BooleanArray3D ba = BooleanArray3D(2);
            ba.value[0] = r0;
            ba.value[1] = r1;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1, BooleanArray2D const &r2) {
        try {
            BooleanArray3D ba = BooleanArray3D(3);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1, BooleanArray2D const &r2,
                                      BooleanArray2D const &r3) {
        try {
            BooleanArray3D ba = BooleanArray3D(4);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1, BooleanArray2D const &r2,
                                      BooleanArray2D const &r3,
                                      BooleanArray2D const &r4) {
        try {
            BooleanArray3D ba = BooleanArray3D(5);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1, BooleanArray2D const &r2,
                                      BooleanArray2D const &r3,
                                      BooleanArray2D const &r4, BooleanArray2D const &r5) {
        try {
            BooleanArray3D ba = BooleanArray3D(6);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            ba.value[5] = r5;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1, BooleanArray2D const &r2,
                                      BooleanArray2D const &r3,
                                      BooleanArray2D const &r4, BooleanArray2D const &r5, BooleanArray2D const &r6) {
        try {
            BooleanArray3D ba = BooleanArray3D(10);
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

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1, BooleanArray2D const &r2,
                                      BooleanArray2D const &r3,
                                      BooleanArray2D const &r4, BooleanArray2D const &r5, BooleanArray2D const &r6,
                                      BooleanArray2D const &r7) {
        try {
            BooleanArray3D ba = BooleanArray3D(6);
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

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1, BooleanArray2D const &r2,
                                      BooleanArray2D const &r3,
                                      BooleanArray2D const &r4, BooleanArray2D const &r5, BooleanArray2D const &r6,
                                      BooleanArray2D const &r7,
                                      BooleanArray2D const &r8) {
        try {
            BooleanArray3D ba = BooleanArray3D(10);
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

    BooleanArray3D BooleanArray3D::of(BooleanArray2D const &r0, BooleanArray2D const &r1, BooleanArray2D const &r2,
                                      BooleanArray2D const &r3,
                                      BooleanArray2D const &r4, BooleanArray2D const &r5, BooleanArray2D const &r6,
                                      BooleanArray2D const &r7,
                                      BooleanArray2D const &r8, BooleanArray2D const &r9) {
        try {
            BooleanArray3D ba = BooleanArray3D(10);
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

    BooleanArray3D BooleanArray3D::copyOf(Array<BooleanArray2D> const &a) {
        try {
            BooleanArray3D ba = BooleanArray3D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                ba.value[i] = a[i];
            }

            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray3D::Linear3DIterator::Linear3DIterator(BooleanArray3D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    BooleanArray3D::Linear3DIterator &BooleanArray3D::Linear3DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    BooleanArray2D &BooleanArray3D::Linear3DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool BooleanArray3D::Linear3DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator>::hasInstance(o)) return false;
        Linear3DIterator const &it = CORE_XCAST(Linear3DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    BooleanArray3D::Linear3DIterator2::Linear3DIterator2(BooleanArray3D const &array,
                                                         gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    BooleanArray3D::Linear3DIterator2 &BooleanArray3D::Linear3DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    BooleanArray2D const &BooleanArray3D::Linear3DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool BooleanArray3D::Linear3DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator2>::hasInstance(o)) return false;
        Linear3DIterator2 const &it = CORE_XCAST(Linear3DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    BooleanArray3D::Linear3DIterator BooleanArray3D::begin() { return Linear3DIterator(*this, false); }

    BooleanArray3D::Linear3DIterator2 BooleanArray3D::begin() const { return Linear3DIterator2(*this, false); }

    BooleanArray3D::Linear3DIterator BooleanArray3D::end() { return Linear3DIterator(*this, true); }

    BooleanArray3D::Linear3DIterator2 BooleanArray3D::end() const { return Linear3DIterator2(*this, true); }

    void BooleanArray3D::initializeFirstTens(BooleanArray2D const &r0, BooleanArray2D const &r1,
                                             BooleanArray2D const &r2,
                                             BooleanArray2D const &r3, BooleanArray2D const &r4,
                                             BooleanArray2D const &r5,
                                             BooleanArray2D const &r6, BooleanArray2D const &r7,
                                             BooleanArray2D const &r8,
                                             BooleanArray2D const &r9) const {
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

    gint BooleanArray3D::initialize(gint &i, BooleanArray2D const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~BooleanArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint BooleanArray3D::initialize(gint &i, BooleanArray2D &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~BooleanArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint BooleanArray3D::initialize(gint &i, BooleanArray2D &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~BooleanArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void BooleanArray3D::accumulate(...) {
    }
} // core
