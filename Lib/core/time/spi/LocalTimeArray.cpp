//
// Created by brunshweeck on 01/03/25.
//

#include "LocalTimeArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    LocalTimeArray::LocalTimeArray() CORE_NOTHROW: LocalTimeArray(0) {}

    LocalTimeArray::LocalTimeArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalTime)));
        for (gint i = 0; i < length; i++)
          new(value + i) LocalTime(LocalTime::MIDNIGHT);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray::LocalTimeArray(gint length, LocalTime const& initialValue): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalTime)));
        for (gint i = 0; i < length; i++)
          new(value + i) LocalTime(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray::LocalTimeArray(LocalTimeArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(LocalTime)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) LocalTime(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray::LocalTimeArray(LocalTimeArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    LocalTimeArray::LocalTimeArray(VarArgs const& args): LocalTimeArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(LocalTime)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) LocalTime(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray& LocalTimeArray::operator=(const LocalTimeArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(LocalTime)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) LocalTime(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    LocalTimeArray& LocalTimeArray::operator=(LocalTimeArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint LocalTimeArray::length() const { return count; }

    gbool LocalTimeArray::isEmpty() const { return count == 0; }

    LocalTime& LocalTimeArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime const& LocalTimeArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime LocalTimeArray::set(gint index, LocalTime newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        LocalTime oldValue = value[index];
        new(value + index) LocalTime(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray::~LocalTimeArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    LocalTimeArray LocalTimeArray::copyOf(Array<LocalTime> const& a) {
      gint n = a.length();
      LocalTimeArray sa = LocalTimeArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) LocalTime(a[i]);
      return sa;
    }

    LocalTimeArray LocalTimeArray::of() { return { }; }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0) {
      return LocalTimeArray(1, UNSAFE::moveInstance(s0));
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1) {
      try {
        LocalTimeArray sa = LocalTimeArray(2);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2) {
      try {
        LocalTimeArray sa = LocalTimeArray(3);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        new(sa.value + 2) LocalTime(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                        LocalTime const& s3) {
      try {
        LocalTimeArray sa = LocalTimeArray(4);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        new(sa.value + 2) LocalTime(s2);
        new(sa.value + 3) LocalTime(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                        LocalTime const& s3, LocalTime const& s4) {
      try {
        LocalTimeArray sa = LocalTimeArray(5);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        new(sa.value + 2) LocalTime(s2);
        new(sa.value + 3) LocalTime(s3);
        new(sa.value + 4) LocalTime(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                        LocalTime const& s3, LocalTime const& s4, LocalTime const& s5) {
      try {
        LocalTimeArray sa = LocalTimeArray(6);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        new(sa.value + 2) LocalTime(s2);
        new(sa.value + 3) LocalTime(s3);
        new(sa.value + 4) LocalTime(s4);
        new(sa.value + 5) LocalTime(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                        LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                        LocalTime const& s6) {
      try {
        LocalTimeArray sa = LocalTimeArray(7);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        new(sa.value + 2) LocalTime(s2);
        new(sa.value + 3) LocalTime(s3);
        new(sa.value + 4) LocalTime(s4);
        new(sa.value + 5) LocalTime(s5);
        new(sa.value + 6) LocalTime(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                        LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                        LocalTime const& s6, LocalTime const& s7) {
      try {
        LocalTimeArray sa = LocalTimeArray(8);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        new(sa.value + 2) LocalTime(s2);
        new(sa.value + 3) LocalTime(s3);
        new(sa.value + 4) LocalTime(s4);
        new(sa.value + 5) LocalTime(s5);
        new(sa.value + 6) LocalTime(s6);
        new(sa.value + 7) LocalTime(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                        LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                        LocalTime const& s6, LocalTime const& s7, LocalTime const& s8) {
      try {
        LocalTimeArray sa = LocalTimeArray(9);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        new(sa.value + 2) LocalTime(s2);
        new(sa.value + 3) LocalTime(s3);
        new(sa.value + 4) LocalTime(s4);
        new(sa.value + 5) LocalTime(s5);
        new(sa.value + 6) LocalTime(s6);
        new(sa.value + 7) LocalTime(s7);
        new(sa.value + 8) LocalTime(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTimeArray LocalTimeArray::of(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                        LocalTime const& s3,
                                        LocalTime const& s4, LocalTime const& s5, LocalTime const& s6,
                                        LocalTime const& s7, LocalTime const& s8, LocalTime const& s9) {
      try {
        LocalTimeArray sa = LocalTimeArray(10);
        new(sa.value + 0) LocalTime(s0);
        new(sa.value + 1) LocalTime(s1);
        new(sa.value + 2) LocalTime(s2);
        new(sa.value + 3) LocalTime(s3);
        new(sa.value + 4) LocalTime(s4);
        new(sa.value + 5) LocalTime(s5);
        new(sa.value + 6) LocalTime(s6);
        new(sa.value + 7) LocalTime(s7);
        new(sa.value + 8) LocalTime(s8);
        new(sa.value + 9) LocalTime(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    LocalTime& LocalTimeArray::operator[](gint index) { return get(index); }

    LocalTime const& LocalTimeArray::operator[](gint index) const { return get(index); }

    LocalTimeArray::LinearIterator::LinearIterator(LocalTimeArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    LocalTimeArray::LinearIterator& LocalTimeArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    LocalTime& LocalTimeArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool LocalTimeArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool LocalTimeArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    LocalTimeArray::LinearIterator2::LinearIterator2(LocalTimeArray const& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    LocalTimeArray::LinearIterator2& LocalTimeArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    LocalTime const& LocalTimeArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool LocalTimeArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool LocalTimeArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    LocalTimeArray::LinearIterator LocalTimeArray::begin() { return LinearIterator(*this, false); }

    LocalTimeArray::LinearIterator2 LocalTimeArray::begin() const { return LinearIterator2(*this, false); }

    LocalTimeArray::LinearIterator LocalTimeArray::end() { return LinearIterator(*this, true); }

    LocalTimeArray::LinearIterator2 LocalTimeArray::end() const { return LinearIterator2(*this, true); }

    gbool LocalTimeArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<LocalTimeArray>::hasInstance(obj)) {
        return false;
      }

      LocalTimeArray const& array = CORE_XCAST(LocalTimeArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint LocalTimeArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String LocalTimeArray::toString() const {
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

    Object& LocalTimeArray::clone() const {
      try {
        return UNSAFE::newInstance<LocalTimeArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void LocalTimeArray::initializeFirstTeens(LocalTime const& s0, LocalTime const& s1, LocalTime const& s2,
                                               LocalTime const& s3, LocalTime const& s4, LocalTime const& s5,
                                               LocalTime const& s6, LocalTime const& s7, LocalTime const& s8,
                                               LocalTime const& s9) {
      new(value + 0) LocalTime(s0);
      new(value + 1) LocalTime(s1);
      new(value + 2) LocalTime(s2);
      new(value + 3) LocalTime(s3);
      new(value + 4) LocalTime(s4);
      new(value + 5) LocalTime(s5);
      new(value + 6) LocalTime(s6);
      new(value + 7) LocalTime(s7);
      new(value + 8) LocalTime(s8);
      new(value + 9) LocalTime(s9);
    }

    void LocalTimeArray::writeAsLocalTime(gint idx, LocalTime const& s) {
      new(value + idx) LocalTime(s);
    }

    void LocalTimeArray::writeLocalTimes(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
