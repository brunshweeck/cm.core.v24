//
// Created by brunshweeck on 2024/09/14.
//

#include <core/lang/Array.h>
#include "LongArray2D.h"
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<LongArray>::size();

    LongArray2D::LongArray2D(): LongArray2D(0) {
    }

    LongArray2D::LongArray2D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) LongArray[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D::LongArray2D(gint nbRows, LongArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) LongArray[nbRows];
            else {
                new(value) LongArray(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) LongArray(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D::LongArray2D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) LongArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) LongArray(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D::LongArray2D(gint nbRows, gint nbCols, glong initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) LongArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) LongArray(nbCols, initializer);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D::LongArray2D(LongArray2D const &array): LongArray2D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(LongArray)));
            new(value) LongArray[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D::LongArray2D(LongArray2D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    LongArray2D::LongArray2D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) LongArray[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint LongArray2D::length() const { return count; }

    gbool LongArray2D::isEmpty() const { return count == 0; }

    LongArray &LongArray2D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray const &LongArray2D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray LongArray2D::set(gint index, LongArray newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    glong &LongArray2D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    glong const &LongArray2D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    glong LongArray2D::set(gint ix, gint iy, glong newValue) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        glong oldValue = value[ix].value[iy];
        value[ix].value[iy] = newValue;

        return oldValue;
    }

    LongArray2D::~LongArray2D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~LongArray();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String LongArray2D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &LongArray2D::clone() const {
        try {
            return UNSAFE::newInstance<LongArray2D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray const &LongArray2D::operator[](gint index) const { return get(index); }

    LongArray &LongArray2D::operator[](gint index) { return get(index); }

    LongArray2D &LongArray2D::operator=(LongArray2D const &array) {
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

    LongArray2D &LongArray2D::operator=(LongArray2D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    LongArray LongArray2D::of() { return {}; }

    LongArray2D LongArray2D::of(LongArray const &r) {
        try {
            return LongArray2D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1) {
        try {
            LongArray2D la = LongArray2D(2);
            la.value[0] = r0;
            la.value[1] = r1;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1, LongArray const &r2) {
        try {
            LongArray2D la = LongArray2D(3);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1, LongArray const &r2, LongArray const &r3) {
        try {
            LongArray2D la = LongArray2D(4);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1, LongArray const &r2, LongArray const &r3,
                                LongArray const &r4) {
        try {
            LongArray2D la = LongArray2D(5);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            la.value[4] = r4;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1, LongArray const &r2, LongArray const &r3,
                                LongArray const &r4, LongArray const &r5) {
        try {
            LongArray2D la = LongArray2D(6);
            la.value[0] = r0;
            la.value[1] = r1;
            la.value[2] = r2;
            la.value[3] = r3;
            la.value[4] = r4;
            la.value[5] = r5;
            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1, LongArray const &r2, LongArray const &r3,
                                LongArray const &r4, LongArray const &r5, LongArray const &r6) {
        try {
            LongArray2D la = LongArray2D(10);
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

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1, LongArray const &r2, LongArray const &r3,
                                LongArray const &r4, LongArray const &r5, LongArray const &r6, LongArray const &r7) {
        try {
            LongArray2D la = LongArray2D(6);
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

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1, LongArray const &r2, LongArray const &r3,
                                LongArray const &r4, LongArray const &r5, LongArray const &r6, LongArray const &r7,
                                LongArray const &r8) {
        try {
            LongArray2D la = LongArray2D(10);
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

    LongArray2D LongArray2D::of(LongArray const &r0, LongArray const &r1, LongArray const &r2, LongArray const &r3,
                                LongArray const &r4, LongArray const &r5, LongArray const &r6, LongArray const &r7,
                                LongArray const &r8, LongArray const &r9) {
        try {
            LongArray2D la = LongArray2D(10);
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

    LongArray2D LongArray2D::copyOf(Array<LongArray> const &a) {
        try {
            LongArray2D la = LongArray2D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                la.value[i] = a[i];
            }

            return la;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray2D::Linear2DIterator::Linear2DIterator(LongArray2D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    LongArray2D::Linear2DIterator &LongArray2D::Linear2DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    LongArray &LongArray2D::Linear2DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool LongArray2D::Linear2DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator>::hasInstance(o)) return false;
        Linear2DIterator const &it = CORE_XCAST(Linear2DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    LongArray2D::Linear2DIterator2::Linear2DIterator2(LongArray2D const &array, gbool isEnd)
        : array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    LongArray2D::Linear2DIterator2 &LongArray2D::Linear2DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    LongArray const &LongArray2D::Linear2DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool LongArray2D::Linear2DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator2>::hasInstance(o)) return false;
        Linear2DIterator2 const &it = CORE_XCAST(Linear2DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    LongArray2D::Linear2DIterator LongArray2D::begin() { return Linear2DIterator(*this, false); }

    LongArray2D::Linear2DIterator2 LongArray2D::begin() const { return Linear2DIterator2(*this, false); }

    LongArray2D::Linear2DIterator LongArray2D::end() { return Linear2DIterator(*this, true); }

    LongArray2D::Linear2DIterator2 LongArray2D::end() const { return Linear2DIterator2(*this, true); }

    void LongArray2D::initializeFirstTens(LongArray const &r0, LongArray const &r1, LongArray const &r2,
                                          LongArray const &r3, LongArray const &r4, LongArray const &r5,
                                          LongArray const &r6, LongArray const &r7, LongArray const &r8,
                                          LongArray const &r9) {
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

    gint LongArray2D::initialize(gint &i, LongArray const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~LongArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint LongArray2D::initialize(gint &i, LongArray &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~LongArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint LongArray2D::initialize(gint &i, LongArray &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~LongArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void LongArray2D::accumulate(...) {
    }
} // core
