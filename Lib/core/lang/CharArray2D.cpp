//
// Created by brunshweeck on 8 sept. 2024.
//

#include "CharArray2D.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IndexOutOfBoundsException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  static CORE_FAST gint ARRAY_ROW_SIZE = Class<CharArray>::size();

  CharArray2D::CharArray2D(): CharArray2D(0) {}

  CharArray2D::CharArray2D(gint nbRows) {
    if (nbRows < 0)
      IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

    if (nbRows == 0)
      return;

    try {
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
      new(value) CharArray[nbRows];
      count = nbRows;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D::CharArray2D(gint nbRows, CharArray initializer) {
    if (nbRows < 0)
      IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

    if (nbRows == 0)
      return;

    try {
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
      if (initializer.isEmpty())
        new(value) CharArray[nbRows];
      else {
        new(value) CharArray(UNSAFE::moveInstance(initializer));

        for (gint i = 1; i < nbRows; i++) {
          new(value + i) CharArray(value[0]);
        }
      }
      count = nbRows;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D::CharArray2D(gint nbRows, gint nbCols) {
    if (nbRows < 0)
      IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

    if (nbCols < 0)
      IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

    if (nbRows == 0)
      return;

    try {
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
      if (nbCols == 0)
        new(value) CharArray[nbRows];
      else {
        for (gint i = 0; i < nbRows; i++) {
          new(value + i) CharArray(nbCols);
        }
      }
      count = nbRows;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D::CharArray2D(gint nbRows, gint nbCols, gchar initializer) {
    if (nbRows < 0)
      IllegalArgumentException("Negative number of rows: "_S + nbRows).throws($ftrace());

    if (nbCols < 0)
      IllegalArgumentException("Negative number of columns: "_S + nbCols).throws($ftrace());

    if (nbRows == 0)
      return;

    try {
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
      if (nbCols == 0)
        new(value) CharArray[nbRows];
      else {
        for (gint i = 0; i < nbRows; i++) {
          new(value + i) CharArray(nbCols, initializer);
        }
      }
      count = nbRows;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D::CharArray2D(CharArray2D const& array): CharArray2D() {
    try {
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * sizeof(CharArray)));
      new(value) CharArray[array.count];
      count = array.count;

      for (gint i = 0; i < count; i++) {
        value[i] = array.value[i];
      }
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D::CharArray2D(CharArray2D&& array) CORE_NOTHROW {
    UNSAFE::swapValues(value, array.value);
    UNSAFE::swapValues(count, array.count);
  }

  CharArray2D::CharArray2D(VarArgs const& args) {
    try {
      gint const nbRows = CORE_CAST(gint, args.size());

      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(nbRows * ARRAY_ROW_SIZE));
      new(value) CharArray[nbRows];
      count = nbRows;

      for (gint i = 0; i < count; i++) {
        value[i] = args.begin()[i];
      }
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  gint CharArray2D::length() const { return count; }

  gbool CharArray2D::isEmpty() const { return count == 0; }

  CharArray& CharArray2D::get(gint index) {
    try {
      misc::Preconditions::checkIndex(index, length());

      return value[index];
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray const& CharArray2D::get(gint index) const {
    try {
      misc::Preconditions::checkIndex(index, length());

      return value[index];
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray CharArray2D::set(gint index, CharArray newRow) {
    try {
      misc::Preconditions::checkIndex(index, length());

      value[index] = UNSAFE::moveInstance(newRow);

      return newRow;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  gchar& CharArray2D::get(gint ix, gint iy) {
    if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
      IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
          .throws($ftrace());
    }

    return value[ix].value[iy];
  }

  gchar const& CharArray2D::get(gint ix, gint iy) const {
    if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
      IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
          .throws($ftrace());
    }

    return value[ix].value[iy];
  }

  gchar CharArray2D::set(gint ix, gint iy, gchar newValue) {
    if ((ix | iy) < 0 || ix >= count || iy >= value[ix].count) {
      IndexOutOfBoundsException("Index ["_S + ix + ", "_S + iy + "] out of bounds for length "_S + count)
          .throws($ftrace());
    }

    gchar oldValue = value[ix].value[iy];
    value[ix].value[iy] = newValue;

    return oldValue;
  }

  CharArray2D::~CharArray2D() {
    ARRAY a = value;
    const gint n = count;

    value = null;
    count = 0;

    if (n > 0) {
      for (int i = n - 1; i >= 0; --i)
        a[i].~CharArray();

      UNSAFE::freeMemory(CORE_CAST(glong, a));
    }
  }

  String CharArray2D::toString() const {
    if (count == 0)
      return "[]"_S;

    XString str = XString(256);
    str.append(u'[');
    for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

    return str.append(value[count - 1]).append(']').toString();
  }

  Object& CharArray2D::clone() const {
    try {
      return UNSAFE::newInstance<CharArray2D>(*this);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray const& CharArray2D::operator[](gint index) const { return get(index); }

  CharArray& CharArray2D::operator[](gint index) { return get(index); }

  CharArray2D& CharArray2D::operator=(CharArray2D const& array) {
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
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
    return *this;
  }

  CharArray2D& CharArray2D::operator=(CharArray2D&& array) CORE_NOTHROW {
    if (this != &array) {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }
    return *this;
  }

  CharArray CharArray2D::of() { return { }; }

  CharArray2D CharArray2D::of(CharArray const& r) {
    try {
      return CharArray2D(1, r);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1) {
    try {
      CharArray2D ca = CharArray2D(2);
      ca.value[0] = r0;
      ca.value[1] = r1;
      return ca;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1, CharArray const& r2) {
    try {
      CharArray2D ca = CharArray2D(3);
      ca.value[0] = r0;
      ca.value[1] = r1;
      ca.value[2] = r2;
      return ca;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3) {
    try {
      CharArray2D ca = CharArray2D(4);
      ca.value[0] = r0;
      ca.value[1] = r1;
      ca.value[2] = r2;
      ca.value[3] = r3;
      return ca;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                              CharArray const& r4) {
    try {
      CharArray2D ca = CharArray2D(5);
      ca.value[0] = r0;
      ca.value[1] = r1;
      ca.value[2] = r2;
      ca.value[3] = r3;
      ca.value[4] = r4;
      return ca;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                              CharArray const& r4, CharArray const& r5) {
    try {
      CharArray2D ca = CharArray2D(6);
      ca.value[0] = r0;
      ca.value[1] = r1;
      ca.value[2] = r2;
      ca.value[3] = r3;
      ca.value[4] = r4;
      ca.value[5] = r5;
      return ca;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                              CharArray const& r4, CharArray const& r5, CharArray const& r6) {
    try {
      CharArray2D ca = CharArray2D(10);
      ca.value[0] = r0;
      ca.value[1] = r1;
      ca.value[2] = r2;
      ca.value[3] = r3;
      ca.value[4] = r4;
      ca.value[5] = r5;
      ca.value[6] = r6;
      return ca;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                              CharArray const& r4, CharArray const& r5, CharArray const& r6, CharArray const& r7) {
    try {
      CharArray2D ca = CharArray2D(6);
      ca.value[0] = r0;
      ca.value[1] = r1;
      ca.value[2] = r2;
      ca.value[3] = r3;
      ca.value[4] = r4;
      ca.value[5] = r5;
      ca.value[6] = r6;
      ca.value[7] = r7;
      return ca;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                              CharArray const& r4, CharArray const& r5, CharArray const& r6, CharArray const& r7,
                              CharArray const& r8) {
    try {
      CharArray2D ca = CharArray2D(10);
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
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::of(CharArray const& r0, CharArray const& r1, CharArray const& r2, CharArray const& r3,
                              CharArray const& r4, CharArray const& r5, CharArray const& r6, CharArray const& r7,
                              CharArray const& r8, CharArray const& r9) {
    try {
      CharArray2D ca = CharArray2D(10);
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
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D CharArray2D::copyOf(Array<CharArray> const& a) {
    try {
      CharArray2D ca = CharArray2D(a.length());

      for (int i = 0; i < a.length(); ++i) {
        ca.value[i] = a[i];
      }

      return ca;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  CharArray2D::Linear2DIterator::Linear2DIterator(CharArray2D& array, gbool isEnd): array(array), isEnd(isEnd) {
    next = isEnd ? array.count : 0;
  }

  CharArray2D::Linear2DIterator& CharArray2D::Linear2DIterator::operator++() {
    if (!isEnd) {
      next += 1;
      isEnd |= next >= array.count;
    }
    return *this;
  }

  CharArray& CharArray2D::Linear2DIterator::operator*() const {
    if (isEnd)
      util::NoSuchElementException().throws($ftrace());
    return array.value[next];
  }

  gbool CharArray2D::Linear2DIterator::equals(const Object& o) const {
    if (this == &o) return true;
    if (!Class<Linear2DIterator>::hasInstance(o)) return false;
    Linear2DIterator const& it = CORE_XCAST(Linear2DIterator const, o);
    if (&array != &it.array) return false;
    if (isEnd) return it.isEnd;
    return next == it.next;
  }

  CharArray2D::Linear2DIterator2::Linear2DIterator2(CharArray2D const& array, gbool isEnd)
    : array(array), isEnd(isEnd) {
    next = isEnd ? array.count : 0;
  }

  CharArray2D::Linear2DIterator2& CharArray2D::Linear2DIterator2::operator++() {
    if (!isEnd) {
      next += 1;
      isEnd |= next >= array.count;
    }
    return *this;
  }

  CharArray const& CharArray2D::Linear2DIterator2::operator*() const {
    if (isEnd)
      util::NoSuchElementException().throws($ftrace());
    return array.value[next];
  }

  gbool CharArray2D::Linear2DIterator2::equals(const Object& o) const {
    if (this == &o) return true;
    if (!Class<Linear2DIterator2>::hasInstance(o)) return false;
    Linear2DIterator2 const& it = CORE_XCAST(Linear2DIterator2 const, o);
    if (&array != &it.array) return false;
    if (isEnd) return it.isEnd;
    return next == it.next;
  }

  CharArray2D::Linear2DIterator CharArray2D::begin() { return Linear2DIterator(*this, false); }

  CharArray2D::Linear2DIterator2 CharArray2D::begin() const { return Linear2DIterator2(*this, false); }

  CharArray2D::Linear2DIterator CharArray2D::end() { return Linear2DIterator(*this, true); }

  CharArray2D::Linear2DIterator2 CharArray2D::end() const { return Linear2DIterator2(*this, true); }

  void CharArray2D::initializeFirstTens(CharArray const& r0, CharArray const& r1, CharArray const& r2,
                                        CharArray const& r3, CharArray const& r4, CharArray const& r5,
                                        CharArray const& r6, CharArray const& r7, CharArray const& r8,
                                        CharArray const& r9) {
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
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  gint CharArray2D::initialize(gint& i, CharArray const& r) {
    try {
      value[i] = r;
      return i++;
    } catch (Throwable const& ex) {
      // reset array.
      for (int j = i - 1; j >= 0; --j)
        value[j].~CharArray();

      value = null;
      count = 0;
      ex.throws($ftrace());
    }
  }

  gint CharArray2D::initialize(gint& i, CharArray& r) {
    try {
      value[i] = r;
      return i++;
    } catch (Throwable const& ex) {
      // reset array.
      for (int j = i - 1; j >= 0; --j)
        value[j].~CharArray();

      value = null;
      count = 0;
      ex.throws($ftrace());
    }
  }

  gint CharArray2D::initialize(gint& i, CharArray&& r) {
    try {
      value[i] = UNSAFE::moveInstance(r);
      return i++;
    } catch (Throwable const& ex) {
      // reset array.
      for (int j = i - 1; j >= 0; --j)
        value[j].~CharArray();

      value = null;
      count = 0;
      ex.throws($ftrace());
    }
  }

  void CharArray2D::accumulate(...) {}
} // core
