//
// Created by brunshweeck on 2024/09/14.
//

#include <core/lang/Array.h>
#include "FloatArray2D.h"
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<FloatArray>::size();

    FloatArray2D::FloatArray2D(): FloatArray2D(0) {
    }

    FloatArray2D::FloatArray2D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) FloatArray[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D::FloatArray2D(gint nbRows, FloatArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) FloatArray[nbRows];
            else {
                new(value) FloatArray(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) FloatArray(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D::FloatArray2D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) FloatArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) FloatArray(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D::FloatArray2D(gint nbRows, gint nbCols, gfloat initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) FloatArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) FloatArray(nbCols, initializer);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D::FloatArray2D(FloatArray2D const &array): FloatArray2D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(FloatArray)));
            new(value) FloatArray[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D::FloatArray2D(FloatArray2D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    FloatArray2D::FloatArray2D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) FloatArray[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint FloatArray2D::length() const { return count; }

    gbool FloatArray2D::isEmpty() const { return count == 0; }

    FloatArray &FloatArray2D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray const &FloatArray2D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray FloatArray2D::set(gint index, FloatArray newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gfloat &FloatArray2D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gfloat const &FloatArray2D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gfloat FloatArray2D::set(gint ix, gint iy, gfloat newValue) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gfloat oldValue = value[ix].value[iy];
        value[ix].value[iy] = newValue;

        return oldValue;
    }

    FloatArray2D::~FloatArray2D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~FloatArray();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String FloatArray2D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &FloatArray2D::clone() const {
        try {
            return UNSAFE::newInstance<FloatArray2D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray const &FloatArray2D::operator[](gint index) const { return get(index); }

    FloatArray &FloatArray2D::operator[](gint index) { return get(index); }

    FloatArray2D &FloatArray2D::operator=(FloatArray2D const &array) {
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

    FloatArray2D &FloatArray2D::operator=(FloatArray2D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    FloatArray FloatArray2D::of() { return {}; }

    FloatArray2D FloatArray2D::of(FloatArray const &r) {
        try {
            return FloatArray2D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1) {
        try {
            FloatArray2D fa = FloatArray2D(2);
            fa.value[0] = r0;
            fa.value[1] = r1;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2) {
        try {
            FloatArray2D fa = FloatArray2D(3);
            fa.value[0] = r0;
            fa.value[1] = r1;
            fa.value[2] = r2;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2,
                                  FloatArray const &r3) {
        try {
            FloatArray2D fa = FloatArray2D(4);
            fa.value[0] = r0;
            fa.value[1] = r1;
            fa.value[2] = r2;
            fa.value[3] = r3;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2,
                                  FloatArray const &r3,
                                  FloatArray const &r4) {
        try {
            FloatArray2D fa = FloatArray2D(5);
            fa.value[0] = r0;
            fa.value[1] = r1;
            fa.value[2] = r2;
            fa.value[3] = r3;
            fa.value[4] = r4;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2,
                                  FloatArray const &r3,
                                  FloatArray const &r4, FloatArray const &r5) {
        try {
            FloatArray2D fa = FloatArray2D(6);
            fa.value[0] = r0;
            fa.value[1] = r1;
            fa.value[2] = r2;
            fa.value[3] = r3;
            fa.value[4] = r4;
            fa.value[5] = r5;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2,
                                  FloatArray const &r3,
                                  FloatArray const &r4, FloatArray const &r5, FloatArray const &r6) {
        try {
            FloatArray2D fa = FloatArray2D(10);
            fa.value[0] = r0;
            fa.value[1] = r1;
            fa.value[2] = r2;
            fa.value[3] = r3;
            fa.value[4] = r4;
            fa.value[5] = r5;
            fa.value[6] = r6;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2,
                                  FloatArray const &r3,
                                  FloatArray const &r4, FloatArray const &r5, FloatArray const &r6,
                                  FloatArray const &r7) {
        try {
            FloatArray2D fa = FloatArray2D(6);
            fa.value[0] = r0;
            fa.value[1] = r1;
            fa.value[2] = r2;
            fa.value[3] = r3;
            fa.value[4] = r4;
            fa.value[5] = r5;
            fa.value[6] = r6;
            fa.value[7] = r7;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2,
                                  FloatArray const &r3,
                                  FloatArray const &r4, FloatArray const &r5, FloatArray const &r6,
                                  FloatArray const &r7,
                                  FloatArray const &r8) {
        try {
            FloatArray2D fa = FloatArray2D(10);
            fa.value[0] = r0;
            fa.value[1] = r1;
            fa.value[2] = r2;
            fa.value[3] = r3;
            fa.value[4] = r4;
            fa.value[5] = r5;
            fa.value[6] = r6;
            fa.value[7] = r7;
            fa.value[8] = r8;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::of(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2,
                                  FloatArray const &r3,
                                  FloatArray const &r4, FloatArray const &r5, FloatArray const &r6,
                                  FloatArray const &r7,
                                  FloatArray const &r8, FloatArray const &r9) {
        try {
            FloatArray2D fa = FloatArray2D(10);
            fa.value[0] = r0;
            fa.value[1] = r1;
            fa.value[2] = r2;
            fa.value[3] = r3;
            fa.value[4] = r4;
            fa.value[5] = r5;
            fa.value[6] = r6;
            fa.value[7] = r7;
            fa.value[8] = r8;
            fa.value[9] = r9;
            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D FloatArray2D::copyOf(Array<FloatArray> const &a) {
        try {
            FloatArray2D fa = FloatArray2D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                fa.value[i] = a[i];
            }

            return fa;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    FloatArray2D::Linear2DIterator::Linear2DIterator(FloatArray2D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    FloatArray2D::Linear2DIterator &FloatArray2D::Linear2DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    FloatArray &FloatArray2D::Linear2DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool FloatArray2D::Linear2DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator>::hasInstance(o)) return false;
        Linear2DIterator const &it = CORE_XCAST(Linear2DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    FloatArray2D::Linear2DIterator2::Linear2DIterator2(FloatArray2D const &array, gbool isEnd)
        : array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    FloatArray2D::Linear2DIterator2 &FloatArray2D::Linear2DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    FloatArray const &FloatArray2D::Linear2DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool FloatArray2D::Linear2DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator2>::hasInstance(o)) return false;
        Linear2DIterator2 const &it = CORE_XCAST(Linear2DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    FloatArray2D::Linear2DIterator FloatArray2D::begin() { return Linear2DIterator(*this, false); }

    FloatArray2D::Linear2DIterator2 FloatArray2D::begin() const { return Linear2DIterator2(*this, false); }

    FloatArray2D::Linear2DIterator FloatArray2D::end() { return Linear2DIterator(*this, true); }

    FloatArray2D::Linear2DIterator2 FloatArray2D::end() const { return Linear2DIterator2(*this, true); }

    void FloatArray2D::initializeFirstTens(FloatArray const &r0, FloatArray const &r1, FloatArray const &r2,
                                           FloatArray const &r3, FloatArray const &r4, FloatArray const &r5,
                                           FloatArray const &r6, FloatArray const &r7, FloatArray const &r8,
                                           FloatArray const &r9) {
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

    gint FloatArray2D::initialize(gint &i, FloatArray const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~FloatArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint FloatArray2D::initialize(gint &i, FloatArray &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~FloatArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint FloatArray2D::initialize(gint &i, FloatArray &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~FloatArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void FloatArray2D::accumulate(...) {
    }
} // core
