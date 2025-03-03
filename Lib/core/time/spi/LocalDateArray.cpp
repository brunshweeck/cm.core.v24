//
// Created by brunshweeck on 01/03/25.
//

#include "LocalDateArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    LocalDateArray::LocalDateArray() CORE_NOTHROW: LocalDateArray(0) {}

    LocalDateArray::LocalDateArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalDate)));
        for (gint i = 0; i < length; i++)
          new(value + i) LocalDate(LocalDate::EPOCH);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray::LocalDateArray(gint length, LocalDate const& initialValue): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalDate)));
        for (gint i = 0; i < length; i++)
          new(value + i) LocalDate(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray::LocalDateArray(LocalDateArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(LocalDate)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) LocalDate(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray::LocalDateArray(LocalDateArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    LocalDateArray::LocalDateArray(VarArgs const& args): LocalDateArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalDate)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) LocalDate(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray& LocalDateArray::operator=(const LocalDateArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(LocalDate)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) LocalDate(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    LocalDateArray& LocalDateArray::operator=(LocalDateArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint LocalDateArray::length() const { return count; }

    gbool LocalDateArray::isEmpty() const { return count == 0; }

    LocalDate& LocalDateArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate const& LocalDateArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate LocalDateArray::set(gint index, LocalDate newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        LocalDate oldValue = value[index];
        new(value + index) LocalDate(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray::~LocalDateArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    LocalDateArray LocalDateArray::copyOf(Array<LocalDate> const& a) {
      gint n = a.length();
      LocalDateArray sa = LocalDateArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) LocalDate(a[i]);
      return sa;
    }

    LocalDateArray LocalDateArray::of() { return { }; }

    LocalDateArray LocalDateArray::of(LocalDate const& s0) {
      return LocalDateArray(1, UNSAFE::moveInstance(s0));
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1) {
      try {
        LocalDateArray sa = LocalDateArray(2);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2) {
      try {
        LocalDateArray sa = LocalDateArray(3);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        new(sa.value + 2) LocalDate(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                        LocalDate const& s3) {
      try {
        LocalDateArray sa = LocalDateArray(4);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        new(sa.value + 2) LocalDate(s2);
        new(sa.value + 3) LocalDate(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                        LocalDate const& s3, LocalDate const& s4) {
      try {
        LocalDateArray sa = LocalDateArray(5);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        new(sa.value + 2) LocalDate(s2);
        new(sa.value + 3) LocalDate(s3);
        new(sa.value + 4) LocalDate(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                        LocalDate const& s3, LocalDate const& s4, LocalDate const& s5) {
      try {
        LocalDateArray sa = LocalDateArray(6);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        new(sa.value + 2) LocalDate(s2);
        new(sa.value + 3) LocalDate(s3);
        new(sa.value + 4) LocalDate(s4);
        new(sa.value + 5) LocalDate(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                        LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                        LocalDate const& s6) {
      try {
        LocalDateArray sa = LocalDateArray(7);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        new(sa.value + 2) LocalDate(s2);
        new(sa.value + 3) LocalDate(s3);
        new(sa.value + 4) LocalDate(s4);
        new(sa.value + 5) LocalDate(s5);
        new(sa.value + 6) LocalDate(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                        LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                        LocalDate const& s6, LocalDate const& s7) {
      try {
        LocalDateArray sa = LocalDateArray(8);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        new(sa.value + 2) LocalDate(s2);
        new(sa.value + 3) LocalDate(s3);
        new(sa.value + 4) LocalDate(s4);
        new(sa.value + 5) LocalDate(s5);
        new(sa.value + 6) LocalDate(s6);
        new(sa.value + 7) LocalDate(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                        LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                        LocalDate const& s6, LocalDate const& s7, LocalDate const& s8) {
      try {
        LocalDateArray sa = LocalDateArray(9);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        new(sa.value + 2) LocalDate(s2);
        new(sa.value + 3) LocalDate(s3);
        new(sa.value + 4) LocalDate(s4);
        new(sa.value + 5) LocalDate(s5);
        new(sa.value + 6) LocalDate(s6);
        new(sa.value + 7) LocalDate(s7);
        new(sa.value + 8) LocalDate(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateArray LocalDateArray::of(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                        LocalDate const& s3,
                                        LocalDate const& s4, LocalDate const& s5, LocalDate const& s6,
                                        LocalDate const& s7, LocalDate const& s8, LocalDate const& s9) {
      try {
        LocalDateArray sa = LocalDateArray(10);
        new(sa.value + 0) LocalDate(s0);
        new(sa.value + 1) LocalDate(s1);
        new(sa.value + 2) LocalDate(s2);
        new(sa.value + 3) LocalDate(s3);
        new(sa.value + 4) LocalDate(s4);
        new(sa.value + 5) LocalDate(s5);
        new(sa.value + 6) LocalDate(s6);
        new(sa.value + 7) LocalDate(s7);
        new(sa.value + 8) LocalDate(s8);
        new(sa.value + 9) LocalDate(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDate& LocalDateArray::operator[](gint index) { return get(index); }

    LocalDate const& LocalDateArray::operator[](gint index) const { return get(index); }

    LocalDateArray::LinearIterator::LinearIterator(LocalDateArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    LocalDateArray::LinearIterator& LocalDateArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    LocalDate& LocalDateArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool LocalDateArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool LocalDateArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    LocalDateArray::LinearIterator2::LinearIterator2(LocalDateArray const& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    LocalDateArray::LinearIterator2& LocalDateArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    LocalDate const& LocalDateArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool LocalDateArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool LocalDateArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    LocalDateArray::LinearIterator LocalDateArray::begin() { return LinearIterator(*this, false); }

    LocalDateArray::LinearIterator2 LocalDateArray::begin() const { return LinearIterator2(*this, false); }

    LocalDateArray::LinearIterator LocalDateArray::end() { return LinearIterator(*this, true); }

    LocalDateArray::LinearIterator2 LocalDateArray::end() const { return LinearIterator2(*this, true); }

    gbool LocalDateArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<LocalDateArray>::hasInstance(obj)) {
        return false;
      }

      LocalDateArray const& array = CORE_XCAST(LocalDateArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint LocalDateArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String LocalDateArray::toString() const {
      gint const count = length();

      XString str = XString(Math::max(count * (1 + 2) + 2, 16));
      str.append(u'[');
      for (int i = 0; i < count; ++i) {
        str.append(value[i]);
        if (i < count - 1) {
          str.append(", "_S);
        }
      }
      str.append(u']');
      return str.toString();
    }

    Object& LocalDateArray::clone() const {
      try {
        return UNSAFE::newInstance<LocalDateArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void LocalDateArray::initializeFirstTeens(LocalDate const& s0, LocalDate const& s1, LocalDate const& s2,
                                               LocalDate const& s3, LocalDate const& s4, LocalDate const& s5,
                                               LocalDate const& s6, LocalDate const& s7, LocalDate const& s8,
                                               LocalDate const& s9) {
      new(value + 0) LocalDate(s0);
      new(value + 1) LocalDate(s1);
      new(value + 2) LocalDate(s2);
      new(value + 3) LocalDate(s3);
      new(value + 4) LocalDate(s4);
      new(value + 5) LocalDate(s5);
      new(value + 6) LocalDate(s6);
      new(value + 7) LocalDate(s7);
      new(value + 8) LocalDate(s8);
      new(value + 9) LocalDate(s9);
    }

    void LocalDateArray::writeAsLocalDate(gint idx, LocalDate const& s) {
      new(value + idx) LocalDate(s);
    }

    void LocalDateArray::writeLocalDates(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
