//
// Created by brunshweeck on 24/02/25.
//

#include "LocalDateTimeArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    LocalDateTimeArray::LocalDateTimeArray() CORE_NOTHROW: LocalDateTimeArray(0) {}

    LocalDateTimeArray::LocalDateTimeArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalDateTime)));
        for (gint i = 0; i < length; i++)
          new(value + i) LocalDateTime(LocalDateTime::EPOCH);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray::LocalDateTimeArray(gint length, LocalDateTime const& initialValue): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalDateTime)));
        for (gint i = 0; i < length; i++)
          new(value + i) LocalDateTime(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray::LocalDateTimeArray(LocalDateTimeArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(LocalDateTime)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) LocalDateTime(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray::LocalDateTimeArray(LocalDateTimeArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    LocalDateTimeArray::LocalDateTimeArray(VarArgs const& args): LocalDateTimeArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalDateTime)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) LocalDateTime(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray& LocalDateTimeArray::operator=(const LocalDateTimeArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(LocalDateTime)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) LocalDateTime(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    LocalDateTimeArray& LocalDateTimeArray::operator=(LocalDateTimeArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint LocalDateTimeArray::length() const { return count; }

    gbool LocalDateTimeArray::isEmpty() const { return count == 0; }

    LocalDateTime& LocalDateTimeArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime const& LocalDateTimeArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime LocalDateTimeArray::set(gint index, LocalDateTime newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        LocalDateTime oldValue = value[index];
        new(value + index) LocalDateTime(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray::~LocalDateTimeArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    LocalDateTimeArray LocalDateTimeArray::copyOf(Array<LocalDateTime> const& a) {
      gint n = a.length();
      LocalDateTimeArray sa = LocalDateTimeArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) LocalDateTime(a[i]);
      return sa;
    }

    LocalDateTimeArray LocalDateTimeArray::of() { return { }; }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0) {
      return LocalDateTimeArray(1, UNSAFE::moveInstance(s0));
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(2);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(3);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        new(sa.value + 2) LocalDateTime(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                        LocalDateTime const& s3) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(4);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        new(sa.value + 2) LocalDateTime(s2);
        new(sa.value + 3) LocalDateTime(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                        LocalDateTime const& s3, LocalDateTime const& s4) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(5);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        new(sa.value + 2) LocalDateTime(s2);
        new(sa.value + 3) LocalDateTime(s3);
        new(sa.value + 4) LocalDateTime(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                        LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(6);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        new(sa.value + 2) LocalDateTime(s2);
        new(sa.value + 3) LocalDateTime(s3);
        new(sa.value + 4) LocalDateTime(s4);
        new(sa.value + 5) LocalDateTime(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                        LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                        LocalDateTime const& s6) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(7);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        new(sa.value + 2) LocalDateTime(s2);
        new(sa.value + 3) LocalDateTime(s3);
        new(sa.value + 4) LocalDateTime(s4);
        new(sa.value + 5) LocalDateTime(s5);
        new(sa.value + 6) LocalDateTime(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                        LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                        LocalDateTime const& s6, LocalDateTime const& s7) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(8);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        new(sa.value + 2) LocalDateTime(s2);
        new(sa.value + 3) LocalDateTime(s3);
        new(sa.value + 4) LocalDateTime(s4);
        new(sa.value + 5) LocalDateTime(s5);
        new(sa.value + 6) LocalDateTime(s6);
        new(sa.value + 7) LocalDateTime(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                        LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                        LocalDateTime const& s6, LocalDateTime const& s7, LocalDateTime const& s8) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(9);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        new(sa.value + 2) LocalDateTime(s2);
        new(sa.value + 3) LocalDateTime(s3);
        new(sa.value + 4) LocalDateTime(s4);
        new(sa.value + 5) LocalDateTime(s5);
        new(sa.value + 6) LocalDateTime(s6);
        new(sa.value + 7) LocalDateTime(s7);
        new(sa.value + 8) LocalDateTime(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTimeArray LocalDateTimeArray::of(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                        LocalDateTime const& s3,
                                        LocalDateTime const& s4, LocalDateTime const& s5, LocalDateTime const& s6,
                                        LocalDateTime const& s7, LocalDateTime const& s8, LocalDateTime const& s9) {
      try {
        LocalDateTimeArray sa = LocalDateTimeArray(10);
        new(sa.value + 0) LocalDateTime(s0);
        new(sa.value + 1) LocalDateTime(s1);
        new(sa.value + 2) LocalDateTime(s2);
        new(sa.value + 3) LocalDateTime(s3);
        new(sa.value + 4) LocalDateTime(s4);
        new(sa.value + 5) LocalDateTime(s5);
        new(sa.value + 6) LocalDateTime(s6);
        new(sa.value + 7) LocalDateTime(s7);
        new(sa.value + 8) LocalDateTime(s8);
        new(sa.value + 9) LocalDateTime(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalDateTime& LocalDateTimeArray::operator[](gint index) { return get(index); }

    LocalDateTime const& LocalDateTimeArray::operator[](gint index) const { return get(index); }

    LocalDateTimeArray::LinearIterator::LinearIterator(LocalDateTimeArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    LocalDateTimeArray::LinearIterator& LocalDateTimeArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    LocalDateTime& LocalDateTimeArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool LocalDateTimeArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool LocalDateTimeArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    LocalDateTimeArray::LinearIterator2::LinearIterator2(LocalDateTimeArray const& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    LocalDateTimeArray::LinearIterator2& LocalDateTimeArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    LocalDateTime const& LocalDateTimeArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool LocalDateTimeArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool LocalDateTimeArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    LocalDateTimeArray::LinearIterator LocalDateTimeArray::begin() { return LinearIterator(*this, false); }

    LocalDateTimeArray::LinearIterator2 LocalDateTimeArray::begin() const { return LinearIterator2(*this, false); }

    LocalDateTimeArray::LinearIterator LocalDateTimeArray::end() { return LinearIterator(*this, true); }

    LocalDateTimeArray::LinearIterator2 LocalDateTimeArray::end() const { return LinearIterator2(*this, true); }

    gbool LocalDateTimeArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<LocalDateTimeArray>::hasInstance(obj)) {
        return false;
      }

      LocalDateTimeArray const& array = CORE_XCAST(LocalDateTimeArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint LocalDateTimeArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String LocalDateTimeArray::toString() const {
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

    Object& LocalDateTimeArray::clone() const {
      try {
        return UNSAFE::newInstance<LocalDateTimeArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void LocalDateTimeArray::initializeFirstTeens(LocalDateTime const& s0, LocalDateTime const& s1, LocalDateTime const& s2,
                                               LocalDateTime const& s3, LocalDateTime const& s4, LocalDateTime const& s5,
                                               LocalDateTime const& s6, LocalDateTime const& s7, LocalDateTime const& s8,
                                               LocalDateTime const& s9) {
      new(value + 0) LocalDateTime(s0);
      new(value + 1) LocalDateTime(s1);
      new(value + 2) LocalDateTime(s2);
      new(value + 3) LocalDateTime(s3);
      new(value + 4) LocalDateTime(s4);
      new(value + 5) LocalDateTime(s5);
      new(value + 6) LocalDateTime(s6);
      new(value + 7) LocalDateTime(s7);
      new(value + 8) LocalDateTime(s8);
      new(value + 9) LocalDateTime(s9);
    }

    void LocalDateTimeArray::writeAsLocalDateTime(gint idx, LocalDateTime const& s) {
      new(value + idx) LocalDateTime(s);
    }

    void LocalDateTimeArray::writeLocalDateTimes(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
