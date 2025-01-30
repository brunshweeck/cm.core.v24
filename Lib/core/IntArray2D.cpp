//
// Created by brunshweeck on 2024/09/14.
//

#include <core/Array.h>
#include <core/IntArray2D.h>
#include <core/IllegalArgumentException.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<IntArray>::size();

    IntArray2D::IntArray2D(): IntArray2D(0) {
    }

    IntArray2D::IntArray2D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) IntArray[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D::IntArray2D(gint nbRows, IntArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) IntArray[nbRows];
            else {
                new(value) IntArray(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) IntArray(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D::IntArray2D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) IntArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) IntArray(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D::IntArray2D(gint nbRows, gint nbCols, gint initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) IntArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) IntArray(nbCols, initializer);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D::IntArray2D(IntArray2D const &array): IntArray2D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(IntArray)));
            new(value) IntArray[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D::IntArray2D(IntArray2D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    IntArray2D::IntArray2D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) IntArray[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint IntArray2D::length() const { return count; }

    gbool IntArray2D::isEmpty() const { return count == 0; }

    IntArray &IntArray2D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray const &IntArray2D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray IntArray2D::set(gint index, IntArray newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint &IntArray2D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gint const &IntArray2D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gint IntArray2D::set(gint ix, gint iy, gint newValue) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gint oldValue = value[ix].value[iy];
        value[ix].value[iy] = newValue;

        return oldValue;
    }

    IntArray2D::~IntArray2D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~IntArray();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String IntArray2D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &IntArray2D::clone() const {
        try {
            return UNSAFE::newInstance<IntArray2D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray const &IntArray2D::operator[](gint index) const { return get(index); }

    IntArray &IntArray2D::operator[](gint index) { return get(index); }

    IntArray2D &IntArray2D::operator=(IntArray2D const &array) {
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

    IntArray2D &IntArray2D::operator=(IntArray2D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    IntArray IntArray2D::of() { return {}; }

    IntArray2D IntArray2D::of(IntArray const &r) {
        try {
            return IntArray2D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1) {
        try {
            IntArray2D ia = IntArray2D(2);
            ia.value[0] = r0;
            ia.value[1] = r1;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1, IntArray const &r2) {
        try {
            IntArray2D ia = IntArray2D(3);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1, IntArray const &r2, IntArray const &r3) {
        try {
            IntArray2D ia = IntArray2D(4);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1, IntArray const &r2, IntArray const &r3,
                              IntArray const &r4) {
        try {
            IntArray2D ia = IntArray2D(5);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            ia.value[4] = r4;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1, IntArray const &r2, IntArray const &r3,
                              IntArray const &r4, IntArray const &r5) {
        try {
            IntArray2D ia = IntArray2D(6);
            ia.value[0] = r0;
            ia.value[1] = r1;
            ia.value[2] = r2;
            ia.value[3] = r3;
            ia.value[4] = r4;
            ia.value[5] = r5;
            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1, IntArray const &r2, IntArray const &r3,
                              IntArray const &r4, IntArray const &r5, IntArray const &r6) {
        try {
            IntArray2D ia = IntArray2D(10);
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

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1, IntArray const &r2, IntArray const &r3,
                              IntArray const &r4, IntArray const &r5, IntArray const &r6, IntArray const &r7) {
        try {
            IntArray2D ia = IntArray2D(6);
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

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1, IntArray const &r2, IntArray const &r3,
                              IntArray const &r4, IntArray const &r5, IntArray const &r6, IntArray const &r7,
                              IntArray const &r8) {
        try {
            IntArray2D ia = IntArray2D(10);
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

    IntArray2D IntArray2D::of(IntArray const &r0, IntArray const &r1, IntArray const &r2, IntArray const &r3,
                              IntArray const &r4, IntArray const &r5, IntArray const &r6, IntArray const &r7,
                              IntArray const &r8, IntArray const &r9) {
        try {
            IntArray2D ia = IntArray2D(10);
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

    IntArray2D IntArray2D::copyOf(Array<IntArray> const &a) {
        try {
            IntArray2D ia = IntArray2D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                ia.value[i] = a[i];
            }

            return ia;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray2D::Linear2DIterator::Linear2DIterator(IntArray2D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    IntArray2D::Linear2DIterator &IntArray2D::Linear2DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    IntArray &IntArray2D::Linear2DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool IntArray2D::Linear2DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator>::hasInstance(o)) return false;
        Linear2DIterator const &it = CORE_XCAST(Linear2DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    IntArray2D::Linear2DIterator2::Linear2DIterator2(IntArray2D const &array, gbool isEnd)
        : array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    IntArray2D::Linear2DIterator2 &IntArray2D::Linear2DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    IntArray const &IntArray2D::Linear2DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool IntArray2D::Linear2DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator2>::hasInstance(o)) return false;
        Linear2DIterator2 const &it = CORE_XCAST(Linear2DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    IntArray2D::Linear2DIterator IntArray2D::begin() { return Linear2DIterator(*this, false); }

    IntArray2D::Linear2DIterator2 IntArray2D::begin() const { return Linear2DIterator2(*this, false); }

    IntArray2D::Linear2DIterator IntArray2D::end() { return Linear2DIterator(*this, true); }

    IntArray2D::Linear2DIterator2 IntArray2D::end() const { return Linear2DIterator2(*this, true); }

    void IntArray2D::initializeFirstTens(IntArray const &r0, IntArray const &r1, IntArray const &r2,
                                         IntArray const &r3, IntArray const &r4, IntArray const &r5,
                                         IntArray const &r6, IntArray const &r7, IntArray const &r8,
                                         IntArray const &r9) {
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

    gint IntArray2D::initialize(gint &i, IntArray const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~IntArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint IntArray2D::initialize(gint &i, IntArray &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~IntArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint IntArray2D::initialize(gint &i, IntArray &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~IntArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void IntArray2D::accumulate(...) {
    }
} // core
