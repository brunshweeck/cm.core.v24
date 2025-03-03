//
// Created by brunshweeck on 2024/09/14.
//

#include <core/lang/Array.h>
#include "LongArray3D.h"
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<LongArray2D>::size();

    LongArray3D::LongArray3D(): LongArray3D(0) {
    }

    LongArray3D::LongArray3D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) LongArray2D[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D::LongArray3D(gint nbRows, LongArray2D initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) LongArray2D[nbRows];
            else {
                new(value) LongArray2D(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) LongArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D::LongArray3D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) LongArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) LongArray2D(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D::LongArray3D(gint nbRows, gint nbCols, LongArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) LongArray2D[nbRows];
            else {
                new(value) LongArray2D(nbCols, UNSAFE::moveInstance(initializer));
                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) LongArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D::LongArray3D(gint nbRows, gint nbCols, gint nbSubCols) {
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
                new(value) LongArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) LongArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D::LongArray3D(gint nbRows, gint nbCols, gint nbSubCols, glong initializer) {
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
                new(value) LongArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) LongArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray LongArray3D::set(gint ix, gint iy, LongArray newCol) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        value[ix].value[iy] = UNSAFE::moveInstance(newCol);

        return newCol;
    }

    glong &LongArray3D::get(gint ix, gint iy, gint iz) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    glong const &LongArray3D::get(gint ix, gint iy, gint iz) const {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    glong LongArray3D::set(gint ix, gint iy, gint iz, glong newValue) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        glong oldValue = value[ix].value[iy].value[iz];
        value[ix].value[iy].value[iz] = newValue;

        return oldValue;
    }

    LongArray3D::LongArray3D(LongArray3D const &array): LongArray3D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(LongArray2D)));
            new(value) LongArray2D[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D::LongArray3D(LongArray3D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    LongArray3D::LongArray3D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) LongArray2D[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint LongArray3D::length() const { return count; }

    gbool LongArray3D::isEmpty() const { return count == 0; }

    LongArray2D &LongArray3D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D const &LongArray3D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D LongArray3D::set(gint index, LongArray2D newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray &LongArray3D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    LongArray const &LongArray3D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    LongArray3D::~LongArray3D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~LongArray2D();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String LongArray3D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &LongArray3D::clone() const {
        try {
            return UNSAFE::newInstance<LongArray3D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D const &LongArray3D::operator[](gint index) const { return get(index); }

    LongArray2D &LongArray3D::operator[](gint index) { return get(index); }

    LongArray3D &LongArray3D::operator=(LongArray3D const &array) {
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

    LongArray3D &LongArray3D::operator=(LongArray3D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    LongArray3D LongArray3D::of() { return {}; }

    LongArray3D LongArray3D::of(LongArray2D const &r) {
        try {
            return LongArray3D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1) {
        try {
            LongArray3D la = LongArray3D(2);
            la.value[0] = r0;
            la.value[1] = r1;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2) {
        try {
            LongArray3D la = LongArray3D(3);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2,
                                LongArray2D const &r3) {
        try {
            LongArray3D la = LongArray3D(4);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2,
                                LongArray2D const &r3,
                                LongArray2D const &r4) {
        try {
            LongArray3D la = LongArray3D(5);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            la.value[4] = r4;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2,
                                LongArray2D const &r3,
                                LongArray2D const &r4, LongArray2D const &r5) {
        try {
            LongArray3D la = LongArray3D(6);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            la.value[4] = r4;
            la.value[5] = r5;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2,
                                LongArray2D const &r3,
                                LongArray2D const &r4, LongArray2D const &r5, LongArray2D const &r6) {
        try {
            LongArray3D la = LongArray3D(10);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            la.value[4] = r4;
            la.value[5] = r5;
            la.value[6] = r6;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2,
                                LongArray2D const &r3,
                                LongArray2D const &r4, LongArray2D const &r5, LongArray2D const &r6,
                                LongArray2D const &r7) {
        try {
            LongArray3D la = LongArray3D(6);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            la.value[4] = r4;
            la.value[5] = r5;
            la.value[6] = r6;
            la.value[7] = r7;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2,
                                LongArray2D const &r3,
                                LongArray2D const &r4, LongArray2D const &r5, LongArray2D const &r6,
                                LongArray2D const &r7,
                                LongArray2D const &r8) {
        try {
            LongArray3D la = LongArray3D(10);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            la.value[4] = r4;
            la.value[5] = r5;
            la.value[6] = r6;
            la.value[7] = r7;
            la.value[8] = r8;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::of(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2,
                                LongArray2D const &r3,
                                LongArray2D const &r4, LongArray2D const &r5, LongArray2D const &r6,
                                LongArray2D const &r7,
                                LongArray2D const &r8, LongArray2D const &r9) {
        try {
            LongArray3D la = LongArray3D(10);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            la.value[4] = r4;
            la.value[5] = r5;
            la.value[6] = r6;
            la.value[7] = r7;
            la.value[8] = r8;
            la.value[9] = r9;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D LongArray3D::copyOf(Array<LongArray2D> const &a) {
        try {
            LongArray3D la = LongArray3D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                la.value[i] = a[i];
            }

            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray3D::Linear3DIterator::Linear3DIterator(LongArray3D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    LongArray3D::Linear3DIterator &LongArray3D::Linear3DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    LongArray2D &LongArray3D::Linear3DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool LongArray3D::Linear3DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator>::hasInstance(o)) return false;
        Linear3DIterator const &it = CORE_XCAST(Linear3DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    LongArray3D::Linear3DIterator2::Linear3DIterator2(LongArray3D const &array,
                                                      gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    LongArray3D::Linear3DIterator2 &LongArray3D::Linear3DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    LongArray2D const &LongArray3D::Linear3DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool LongArray3D::Linear3DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator2>::hasInstance(o)) return false;
        Linear3DIterator2 const &it = CORE_XCAST(Linear3DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    LongArray3D::Linear3DIterator LongArray3D::begin() { return Linear3DIterator(*this, false); }

    LongArray3D::Linear3DIterator2 LongArray3D::begin() const { return Linear3DIterator2(*this, false); }

    LongArray3D::Linear3DIterator LongArray3D::end() { return Linear3DIterator(*this, true); }

    LongArray3D::Linear3DIterator2 LongArray3D::end() const { return Linear3DIterator2(*this, true); }

    void LongArray3D::initializeFirstTens(LongArray2D const &r0, LongArray2D const &r1, LongArray2D const &r2,
                                          LongArray2D const &r3, LongArray2D const &r4, LongArray2D const &r5,
                                          LongArray2D const &r6, LongArray2D const &r7, LongArray2D const &r8,
                                          LongArray2D const &r9) const {
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

    gint LongArray3D::initialize(gint &i, LongArray2D const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~LongArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint LongArray3D::initialize(gint &i, LongArray2D &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~LongArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint LongArray3D::initialize(gint &i, LongArray2D &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~LongArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void LongArray3D::accumulate(...) {
    }
} // core
