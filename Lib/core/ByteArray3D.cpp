//
// Created by brunshweeck on 2024/09/14.
//


#include <core/Array.h>
#include <core/ByteArray3D.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/OutOfMemoryError.h>
#include <core/XString.h>
#include <core/function/IntSupplier.h>
#include <core/function/IntUnaryOperator.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<ByteArray2D>::size();

    ByteArray3D::ByteArray3D(): ByteArray3D(0) {
    }

    ByteArray3D::ByteArray3D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) ByteArray2D[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D::ByteArray3D(gint nbRows, ByteArray2D initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) ByteArray2D[nbRows];
            else {
                new(value) ByteArray2D(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) ByteArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D::ByteArray3D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ByteArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ByteArray2D(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D::ByteArray3D(gint nbRows, gint nbCols, ByteArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) ByteArray2D[nbRows];
            else {
                new(value) ByteArray2D(nbCols, UNSAFE::moveInstance(initializer));
                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) ByteArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D::ByteArray3D(gint nbRows, gint nbCols, gint nbSubCols) {
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
                new(value) ByteArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ByteArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D::ByteArray3D(gint nbRows, gint nbCols, gint nbSubCols, gbyte initializer) {
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
                new(value) ByteArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) ByteArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray ByteArray3D::set(gint ix, gint iy, ByteArray newCol) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        value[ix].value[iy] = UNSAFE::moveInstance(newCol);

        return newCol;
    }

    gbyte &ByteArray3D::get(gint ix, gint iy, gint iz) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gbyte const &ByteArray3D::get(gint ix, gint iy, gint iz) const {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gbyte ByteArray3D::set(gint ix, gint iy, gint iz, gbyte newValue) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gbyte oldValue = value[ix].value[iy].value[iz];
        value[ix].value[iy].value[iz] = newValue;

        return oldValue;
    }

    ByteArray3D::ByteArray3D(ByteArray3D const &array): ByteArray3D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(ByteArray2D)));
            new(value) ByteArray2D[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D::ByteArray3D(ByteArray3D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    ByteArray3D::ByteArray3D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) ByteArray2D[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint ByteArray3D::length() const { return count; }

    gbool ByteArray3D::isEmpty() const { return count == 0; }

    ByteArray2D &ByteArray3D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D const &ByteArray3D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D ByteArray3D::set(gint index, ByteArray2D newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray &ByteArray3D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    ByteArray const &ByteArray3D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    ByteArray3D::~ByteArray3D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~ByteArray2D();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String ByteArray3D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &ByteArray3D::clone() const {
        try {
            return UNSAFE::newInstance<ByteArray3D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray2D const &ByteArray3D::operator[](gint index) const { return get(index); }

    ByteArray2D &ByteArray3D::operator[](gint index) { return get(index); }

    ByteArray3D &ByteArray3D::operator=(ByteArray3D const &array) {
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

    ByteArray3D &ByteArray3D::operator=(ByteArray3D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    ByteArray3D ByteArray3D::of() { return {}; }

    ByteArray3D ByteArray3D::of(ByteArray2D const &r) {
        try {
            return ByteArray3D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1) {
        try {
            ByteArray3D ba = ByteArray3D(2);
            ba.value[0] = r0;
            ba.value[1] = r1;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2) {
        try {
            ByteArray3D ba = ByteArray3D(3);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2,
                                ByteArray2D const &r3) {
        try {
            ByteArray3D ba = ByteArray3D(4);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2,
                                ByteArray2D const &r3,
                                ByteArray2D const &r4) {
        try {
            ByteArray3D ba = ByteArray3D(5);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2,
                                ByteArray2D const &r3,
                                ByteArray2D const &r4, ByteArray2D const &r5) {
        try {
            ByteArray3D ba = ByteArray3D(6);
            ba.value[0] = r0;
            ba.value[1] = r1;
            ba.value[2] = r2;
            ba.value[3] = r3;
            ba.value[4] = r4;
            ba.value[5] = r5;
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2,
                                ByteArray2D const &r3,
                                ByteArray2D const &r4, ByteArray2D const &r5, ByteArray2D const &r6) {
        try {
            ByteArray3D ba = ByteArray3D(10);
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

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2,
                                ByteArray2D const &r3,
                                ByteArray2D const &r4, ByteArray2D const &r5, ByteArray2D const &r6,
                                ByteArray2D const &r7) {
        try {
            ByteArray3D ba = ByteArray3D(6);
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

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2,
                                ByteArray2D const &r3,
                                ByteArray2D const &r4, ByteArray2D const &r5, ByteArray2D const &r6,
                                ByteArray2D const &r7,
                                ByteArray2D const &r8) {
        try {
            ByteArray3D ba = ByteArray3D(10);
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

    ByteArray3D ByteArray3D::of(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2,
                                ByteArray2D const &r3,
                                ByteArray2D const &r4, ByteArray2D const &r5, ByteArray2D const &r6,
                                ByteArray2D const &r7,
                                ByteArray2D const &r8, ByteArray2D const &r9) {
        try {
            ByteArray3D ba = ByteArray3D(10);
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

    ByteArray3D ByteArray3D::copyOf(Array<ByteArray2D> const &a) {
        try {
            ByteArray3D ba = ByteArray3D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                ba.value[i] = a[i];
            }

            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray3D::Linear3DIterator::Linear3DIterator(ByteArray3D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    ByteArray3D::Linear3DIterator &ByteArray3D::Linear3DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    ByteArray2D &ByteArray3D::Linear3DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool ByteArray3D::Linear3DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator>::hasInstance(o)) return false;
        Linear3DIterator const &it = CORE_XCAST(Linear3DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    ByteArray3D::Linear3DIterator2::Linear3DIterator2(ByteArray3D const &array,
                                                      gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    ByteArray3D::Linear3DIterator2 &ByteArray3D::Linear3DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    ByteArray2D const &ByteArray3D::Linear3DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool ByteArray3D::Linear3DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator2>::hasInstance(o)) return false;
        Linear3DIterator2 const &it = CORE_XCAST(Linear3DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    ByteArray3D::Linear3DIterator ByteArray3D::begin() { return Linear3DIterator(*this, false); }

    ByteArray3D::Linear3DIterator2 ByteArray3D::begin() const { return Linear3DIterator2(*this, false); }

    ByteArray3D::Linear3DIterator ByteArray3D::end() { return Linear3DIterator(*this, true); }

    ByteArray3D::Linear3DIterator2 ByteArray3D::end() const { return Linear3DIterator2(*this, true); }

    void ByteArray3D::initializeFirstTens(ByteArray2D const &r0, ByteArray2D const &r1, ByteArray2D const &r2,
                                          ByteArray2D const &r3, ByteArray2D const &r4, ByteArray2D const &r5,
                                          ByteArray2D const &r6, ByteArray2D const &r7, ByteArray2D const &r8,
                                          ByteArray2D const &r9) const {
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

    gint ByteArray3D::initialize(gint &i, ByteArray2D const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ByteArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint ByteArray3D::initialize(gint &i, ByteArray2D &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ByteArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint ByteArray3D::initialize(gint &i, ByteArray2D &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~ByteArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void ByteArray3D::accumulate(...) {
    }
} // core
