//
// Created by brunshweeck on 2024/09/14.
//

#include <core/Array.h>
#include <core/ByteArray2D.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/OutOfMemoryError.h>
#include <core/XString.h>
#include <core/function/IntSupplier.h>
#include <core/function/IntUnaryOperator.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<ByteArray>::size();

    ByteArray2D::ByteArray2D(): ByteArray2D(0) {
    }

    ByteArray2D::ByteArray2D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) ByteArray[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D::ByteArray2D(gint nbRows, ByteArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) ByteArray[nbRows];
            else {
                new(value) ByteArray(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) ByteArray(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D::ByteArray2D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ByteArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ByteArray(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D::ByteArray2D(gint nbRows, gint nbCols, gbyte initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ByteArray[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ByteArray(nbCols, initializer);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D::ByteArray2D(ByteArray2D const &array): ByteArray2D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(ByteArray)));
            new(value) ByteArray[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D::ByteArray2D(ByteArray2D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    ByteArray2D::ByteArray2D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) ByteArray[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint ByteArray2D::length() const { return count; }

    gbool ByteArray2D::isEmpty() const { return count == 0; }

    ByteArray &ByteArray2D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray const &ByteArray2D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray ByteArray2D::set(gint index, ByteArray newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gbyte &ByteArray2D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gbyte const &ByteArray2D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    gbyte ByteArray2D::set(gint ix, gint iy, gbyte newValue) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gbyte oldValue = value[ix].value[iy];
        value[ix].value[iy] = newValue;

        return oldValue;
    }

    ByteArray2D::~ByteArray2D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~ByteArray();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String ByteArray2D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &ByteArray2D::clone() const {
        try {
            return UNSAFE::newInstance<ByteArray2D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray const &ByteArray2D::operator[](gint index) const { return get(index); }

    ByteArray &ByteArray2D::operator[](gint index) { return get(index); }

    ByteArray2D &ByteArray2D::operator=(ByteArray2D const &array) {
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

    ByteArray2D &ByteArray2D::operator=(ByteArray2D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    ByteArray ByteArray2D::of() { return {}; }

    ByteArray2D ByteArray2D::of(ByteArray const &r) {
        try {
            return ByteArray2D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1) {
        try {
            ByteArray2D ba = ByteArray2D(2);
            ba.value[0] = r0;
            ba.value[1] = r1;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2) {
        try {
            ByteArray2D ba = ByteArray2D(3);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2, ByteArray const &r3) {
        try {
            ByteArray2D ba = ByteArray2D(4);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2, ByteArray const &r3,
                                ByteArray const &r4) {
        try {
            ByteArray2D ba = ByteArray2D(5);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2, ByteArray const &r3,
                                ByteArray const &r4, ByteArray const &r5) {
        try {
            ByteArray2D ba = ByteArray2D(6);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            ba.value[5] = r5;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2, ByteArray const &r3,
                                ByteArray const &r4, ByteArray const &r5, ByteArray const &r6) {
        try {
            ByteArray2D ba = ByteArray2D(10);
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

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2, ByteArray const &r3,
                                ByteArray const &r4, ByteArray const &r5, ByteArray const &r6, ByteArray const &r7) {
        try {
            ByteArray2D ba = ByteArray2D(6);
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

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2, ByteArray const &r3,
                                ByteArray const &r4, ByteArray const &r5, ByteArray const &r6, ByteArray const &r7,
                                ByteArray const &r8) {
        try {
            ByteArray2D ba = ByteArray2D(10);
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

    ByteArray2D ByteArray2D::of(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2, ByteArray const &r3,
                                ByteArray const &r4, ByteArray const &r5, ByteArray const &r6, ByteArray const &r7,
                                ByteArray const &r8, ByteArray const &r9) {
        try {
            ByteArray2D ba = ByteArray2D(10);
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

    ByteArray2D ByteArray2D::copyOf(Array<ByteArray> const &a) {
        try {
            ByteArray2D ba = ByteArray2D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                ba.value[i] = a[i];
            }

            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D::Linear2DIterator::Linear2DIterator(ByteArray2D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    ByteArray2D::Linear2DIterator &ByteArray2D::Linear2DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    ByteArray &ByteArray2D::Linear2DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool ByteArray2D::Linear2DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator>::hasInstance(o)) return false;
        Linear2DIterator const &it = CORE_XCAST(Linear2DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    ByteArray2D::Linear2DIterator2::Linear2DIterator2(ByteArray2D const &array, gbool isEnd)
        : array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    ByteArray2D::Linear2DIterator2 &ByteArray2D::Linear2DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    ByteArray const &ByteArray2D::Linear2DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool ByteArray2D::Linear2DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear2DIterator2>::hasInstance(o)) return false;
        Linear2DIterator2 const &it = CORE_XCAST(Linear2DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    ByteArray2D::Linear2DIterator ByteArray2D::begin() { return Linear2DIterator(*this, false); }

    ByteArray2D::Linear2DIterator2 ByteArray2D::begin() const { return Linear2DIterator2(*this, false); }

    ByteArray2D::Linear2DIterator ByteArray2D::end() { return Linear2DIterator(*this, true); }

    ByteArray2D::Linear2DIterator2 ByteArray2D::end() const { return Linear2DIterator2(*this, true); }

    void ByteArray2D::initializeFirstTens(ByteArray const &r0, ByteArray const &r1, ByteArray const &r2,
                                          ByteArray const &r3, ByteArray const &r4, ByteArray const &r5,
                                          ByteArray const &r6, ByteArray const &r7, ByteArray const &r8,
                                          ByteArray const &r9) {
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

    gint ByteArray2D::initialize(gint &i, ByteArray const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ByteArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint ByteArray2D::initialize(gint &i, ByteArray &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ByteArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint ByteArray2D::initialize(gint &i, ByteArray &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ByteArray();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void ByteArray2D::accumulate(...) {
    }
} // core
