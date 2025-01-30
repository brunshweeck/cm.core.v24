//
// Created by brunshweeck on 2024/09/14.
//

#include <core/Array.h>
#include <core/CharArray3D.h>
#include <core/IllegalArgumentException.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    static CORE_FAST gint ARRAY_ROW_SIZE = Class<CharArray2D>::size();

    CharArray3D::CharArray3D(): CharArray3D(0) {
    }

    CharArray3D::CharArray3D(gint nbRows) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) CharArray2D[nbRows];
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D::CharArray3D(gint nbRows, CharArray2D initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (initializer.isEmpty())
                new(value) CharArray2D[nbRows];
            else {
                new(value) CharArray2D(UNSAFE::moveInstance(initializer));

                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) CharArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D::CharArray3D(gint nbRows, gint nbCols) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) CharArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) CharArray2D(nbCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D::CharArray3D(gint nbRows, gint nbCols, CharArray initializer) {
        if (nbRows < 0)
            IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

        if (nbCols < 0)
            IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

        if (nbRows == 0)
            return;

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            if (nbCols == 0)
                new(value) CharArray2D[nbRows];
            else {
                new(value) CharArray2D(nbCols, UNSAFE::moveInstance(initializer));
                for (gint i = 1; i < nbRows; i++) {
                    new(value + i) CharArray2D(value[0]);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D::CharArray3D(gint nbRows, gint nbCols, gint nbSubCols) {
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
                new(value) CharArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) CharArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D::CharArray3D(gint nbRows, gint nbCols, gint nbSubCols, gchar initializer) {
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
                new(value) CharArray2D[nbRows];
            else {
                for (gint i = 0; i < nbRows; i++) {
                    new(value + i) CharArray2D(nbCols, nbSubCols);
                }
            }
            count = nbRows;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray CharArray3D::set(gint ix, gint iy, CharArray newCol) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        value[ix].value[iy] = UNSAFE::moveInstance(newCol);

        return newCol;
    }

    gchar &CharArray3D::get(gint ix, gint iy, gint iz) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gchar const &CharArray3D::get(gint ix, gint iy, gint iz) const {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy].value[iz];
    }

    gchar CharArray3D::set(gint ix, gint iy, gint iz, gchar newValue) {
        if ((ix | iy | iz) < 0 || ix >= count || iy >= value[ix].count || iz >= value[ix].value[iy].count) {
            IndexOutOfBoundsException(
                        "Index ["_S + ix + ", "_S + iy + ", "_S + iz + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        gchar oldValue = value[ix].value[iy].value[iz];
        value[ix].value[iy].value[iz] = newValue;

        return oldValue;
    }

    CharArray3D::CharArray3D(CharArray3D const &array): CharArray3D() {
        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(CharArray2D)));
            new(value) CharArray2D[array.count];
            count = array.count;

            for (gint i = 0; i < count; i++) {
                value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D::CharArray3D(CharArray3D &&array) CORE_NOTHROW {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    CharArray3D::CharArray3D(VarArgs const &args) {
        try {
            gint const nbRows = CORE_CAST(gint, args.size());

            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
            new(value) CharArray2D[nbRows];
            count = nbRows;

            for (gint i = 0; i < count; i++) {
                value[i] = args.begin()[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gint CharArray3D::length() const { return count; }

    gbool CharArray3D::isEmpty() const { return count == 0; }

    CharArray2D &CharArray3D::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray2D const &CharArray3D::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray2D CharArray3D::set(gint index, CharArray2D newRow) {
        try {
            misc::Preconditions::checkIndex(index, length());

            value[index] = UNSAFE::moveInstance(newRow);

            return newRow;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray &CharArray3D::get(gint ix, gint iy) {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    CharArray const &CharArray3D::get(gint ix, gint iy) const {
        if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
            IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
                    .throws($ftrace());
        }

        return value[ix].value[iy];
    }

    CharArray3D::~CharArray3D() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0) {
            for (int i = n - 1; i >= 0; --i)
                a[i].~CharArray2D();

            UNSAFE::freeMemory(CORE_CAST(glong, a));
        }
    }

    String CharArray3D::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(256);
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &CharArray3D::clone() const {
        try {
            return UNSAFE::newInstance<CharArray3D>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray2D const &CharArray3D::operator[](gint index) const { return get(index); }

    CharArray2D &CharArray3D::operator[](gint index) { return get(index); }

    CharArray3D &CharArray3D::operator=(CharArray3D const &array) {
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

    CharArray3D &CharArray3D::operator=(CharArray3D &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    CharArray3D CharArray3D::of() { return {}; }

    CharArray3D CharArray3D::of(CharArray2D const &r) {
        try {
            return CharArray3D(1, r);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1) {
        try {
            CharArray3D ca = CharArray3D(2);
            ca.value[0] = r0;
            ca.value[1] = r1;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2) {
        try {
            CharArray3D ca = CharArray3D(3);
            ca.value[0] = r0;
            ca.value[1] = r1;
            ca.value[2] = r2;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2,
                                CharArray2D const &r3) {
        try {
            CharArray3D ca = CharArray3D(4);
            ca.value[0] = r0;
            ca.value[1] = r1;
            ca.value[2] = r2;
            ca.value[3] = r3;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2,
                                CharArray2D const &r3,
                                CharArray2D const &r4) {
        try {
            CharArray3D ca = CharArray3D(5);
            ca.value[0] = r0;
            ca.value[1] = r1;
            ca.value[2] = r2;
            ca.value[3] = r3;
            ca.value[4] = r4;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2,
                                CharArray2D const &r3,
                                CharArray2D const &r4, CharArray2D const &r5) {
        try {
            CharArray3D ca = CharArray3D(6);
            ca.value[0] = r0;
            ca.value[1] = r1;
            ca.value[2] = r2;
            ca.value[3] = r3;
            ca.value[4] = r4;
            ca.value[5] = r5;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2,
                                CharArray2D const &r3,
                                CharArray2D const &r4, CharArray2D const &r5, CharArray2D const &r6) {
        try {
            CharArray3D ca = CharArray3D(10);
            ca.value[0] = r0;
            ca.value[1] = r1;
            ca.value[2] = r2;
            ca.value[3] = r3;
            ca.value[4] = r4;
            ca.value[5] = r5;
            ca.value[6] = r6;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2,
                                CharArray2D const &r3,
                                CharArray2D const &r4, CharArray2D const &r5, CharArray2D const &r6,
                                CharArray2D const &r7) {
        try {
            CharArray3D ca = CharArray3D(6);
            ca.value[0] = r0;
            ca.value[1] = r1;
            ca.value[2] = r2;
            ca.value[3] = r3;
            ca.value[4] = r4;
            ca.value[5] = r5;
            ca.value[6] = r6;
            ca.value[7] = r7;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2,
                                CharArray2D const &r3,
                                CharArray2D const &r4, CharArray2D const &r5, CharArray2D const &r6,
                                CharArray2D const &r7,
                                CharArray2D const &r8) {
        try {
            CharArray3D ca = CharArray3D(10);
            ca.value[0] = r0;
            ca.value[1] = r1;
            ca.value[2] = r2;
            ca.value[3] = r3;
            ca.value[4] = r4;
            ca.value[5] = r5;
            ca.value[6] = r6;
            ca.value[7] = r7;
            ca.value[8] = r8;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::of(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2,
                                CharArray2D const &r3,
                                CharArray2D const &r4, CharArray2D const &r5, CharArray2D const &r6,
                                CharArray2D const &r7,
                                CharArray2D const &r8, CharArray2D const &r9) {
        try {
            CharArray3D ca = CharArray3D(10);
            ca.value[0] = r0;
            ca.value[1] = r1;
            ca.value[2] = r2;
            ca.value[3] = r3;
            ca.value[4] = r4;
            ca.value[5] = r5;
            ca.value[6] = r6;
            ca.value[7] = r7;
            ca.value[8] = r8;
            ca.value[9] = r9;
            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D CharArray3D::copyOf(Array<CharArray2D> const &a) {
        try {
            CharArray3D ca = CharArray3D(a.length());

            for (int i = 0; i < a.length(); ++i) {
                ca.value[i] = a[i];
            }

            return ca;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    CharArray3D::Linear3DIterator::Linear3DIterator(CharArray3D &array, gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    CharArray3D::Linear3DIterator &CharArray3D::Linear3DIterator::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    CharArray2D &CharArray3D::Linear3DIterator::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool CharArray3D::Linear3DIterator::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator>::hasInstance(o)) return false;
        Linear3DIterator const &it = CORE_XCAST(Linear3DIterator const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    CharArray3D::Linear3DIterator2::Linear3DIterator2(CharArray3D const &array,
                                                      gbool isEnd): array(array), isEnd(isEnd) {
        next = isEnd ? array.count : 0;
    }

    CharArray3D::Linear3DIterator2 &CharArray3D::Linear3DIterator2::operator++() {
        if (!isEnd) {
            next += 1;
            isEnd |= next >= array.count;
        }
        return *this;
    }

    CharArray2D const &CharArray3D::Linear3DIterator2::operator*() const {
        if (isEnd)
            util::NoSuchElementException().throws($ftrace());
        return array.value[next];
    }

    gbool CharArray3D::Linear3DIterator2::equals(const Object &o) const {
        if (this == &o) return true;
        if (!Class<Linear3DIterator2>::hasInstance(o)) return false;
        Linear3DIterator2 const &it = CORE_XCAST(Linear3DIterator2 const, o);
        if (&array != &it.array) return false;
        if (isEnd) return it.isEnd;
        return next == it.next;
    }

    CharArray3D::Linear3DIterator CharArray3D::begin() { return Linear3DIterator(*this, false); }

    CharArray3D::Linear3DIterator2 CharArray3D::begin() const { return Linear3DIterator2(*this, false); }

    CharArray3D::Linear3DIterator CharArray3D::end() { return Linear3DIterator(*this, true); }

    CharArray3D::Linear3DIterator2 CharArray3D::end() const { return Linear3DIterator2(*this, true); }

    void CharArray3D::initializeFirstTens(CharArray2D const &r0, CharArray2D const &r1, CharArray2D const &r2,
                                          CharArray2D const &r3, CharArray2D const &r4, CharArray2D const &r5,
                                          CharArray2D const &r6, CharArray2D const &r7, CharArray2D const &r8,
                                          CharArray2D const &r9) const {
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

    gint CharArray3D::initialize(gint &i, CharArray2D const &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~CharArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint CharArray3D::initialize(gint &i, CharArray2D &r) {
        try {
            value[i] = r;
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~CharArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    gint CharArray3D::initialize(gint &i, CharArray2D &&r) {
        try {
            value[i] = UNSAFE::moveInstance(r);
            return i++;
        } catch (Throwable const &ex) {
            // reset array.
            for (int j = i - 1; j >= 0; --j)
                value[j].~CharArray2D();

            value = null;
            count = 0;
            ex.throws($ftrace());
        }
    }

    void CharArray3D::accumulate(...) {
    }
} // core
