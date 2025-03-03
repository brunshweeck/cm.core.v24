//
// Created by brunshweeck on 01/03/25.
//

#include "ZonedDateTimeArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    ZonedDateTimeArray::ZonedDateTimeArray() CORE_NOTHROW: ZonedDateTimeArray(0) {}

    ZonedDateTimeArray::ZonedDateTimeArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZonedDateTime)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZonedDateTime(ZonedDateTime::of(LocalDateTime::EPOCH, ZoneOffset::UTC));
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray::ZonedDateTimeArray(gint length, ZonedDateTime const& initialValue): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZonedDateTime)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZonedDateTime(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray::ZonedDateTimeArray(ZonedDateTimeArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(ZonedDateTime)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) ZonedDateTime(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray::ZonedDateTimeArray(ZonedDateTimeArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    ZonedDateTimeArray::ZonedDateTimeArray(VarArgs const& args): ZonedDateTimeArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZonedDateTime)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) ZonedDateTime(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray& ZonedDateTimeArray::operator=(const ZonedDateTimeArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(ZonedDateTime)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) ZonedDateTime(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    ZonedDateTimeArray& ZonedDateTimeArray::operator=(ZonedDateTimeArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint ZonedDateTimeArray::length() const { return count; }

    gbool ZonedDateTimeArray::isEmpty() const { return count == 0; }

    ZonedDateTime& ZonedDateTimeArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime const& ZonedDateTimeArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime ZonedDateTimeArray::set(gint index, ZonedDateTime newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        ZonedDateTime oldValue = value[index];
        new(value + index) ZonedDateTime(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray::~ZonedDateTimeArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    ZonedDateTimeArray ZonedDateTimeArray::copyOf(Array<ZonedDateTime> const& a) {
      gint n = a.length();
      ZonedDateTimeArray sa = ZonedDateTimeArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) ZonedDateTime(a[i]);
      return sa;
    }

    ZonedDateTimeArray ZonedDateTimeArray::of() { return { }; }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0) {
      return ZonedDateTimeArray(1, UNSAFE::moveInstance(s0));
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(2);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(3);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        new(sa.value + 2) ZonedDateTime(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                        ZonedDateTime const& s3) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(4);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        new(sa.value + 2) ZonedDateTime(s2);
        new(sa.value + 3) ZonedDateTime(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                        ZonedDateTime const& s3, ZonedDateTime const& s4) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(5);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        new(sa.value + 2) ZonedDateTime(s2);
        new(sa.value + 3) ZonedDateTime(s3);
        new(sa.value + 4) ZonedDateTime(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                        ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(6);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        new(sa.value + 2) ZonedDateTime(s2);
        new(sa.value + 3) ZonedDateTime(s3);
        new(sa.value + 4) ZonedDateTime(s4);
        new(sa.value + 5) ZonedDateTime(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                        ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                        ZonedDateTime const& s6) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(7);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        new(sa.value + 2) ZonedDateTime(s2);
        new(sa.value + 3) ZonedDateTime(s3);
        new(sa.value + 4) ZonedDateTime(s4);
        new(sa.value + 5) ZonedDateTime(s5);
        new(sa.value + 6) ZonedDateTime(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                        ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                        ZonedDateTime const& s6, ZonedDateTime const& s7) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(8);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        new(sa.value + 2) ZonedDateTime(s2);
        new(sa.value + 3) ZonedDateTime(s3);
        new(sa.value + 4) ZonedDateTime(s4);
        new(sa.value + 5) ZonedDateTime(s5);
        new(sa.value + 6) ZonedDateTime(s6);
        new(sa.value + 7) ZonedDateTime(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                        ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                        ZonedDateTime const& s6, ZonedDateTime const& s7, ZonedDateTime const& s8) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(9);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        new(sa.value + 2) ZonedDateTime(s2);
        new(sa.value + 3) ZonedDateTime(s3);
        new(sa.value + 4) ZonedDateTime(s4);
        new(sa.value + 5) ZonedDateTime(s5);
        new(sa.value + 6) ZonedDateTime(s6);
        new(sa.value + 7) ZonedDateTime(s7);
        new(sa.value + 8) ZonedDateTime(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTimeArray ZonedDateTimeArray::of(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                        ZonedDateTime const& s3,
                                        ZonedDateTime const& s4, ZonedDateTime const& s5, ZonedDateTime const& s6,
                                        ZonedDateTime const& s7, ZonedDateTime const& s8, ZonedDateTime const& s9) {
      try {
        ZonedDateTimeArray sa = ZonedDateTimeArray(10);
        new(sa.value + 0) ZonedDateTime(s0);
        new(sa.value + 1) ZonedDateTime(s1);
        new(sa.value + 2) ZonedDateTime(s2);
        new(sa.value + 3) ZonedDateTime(s3);
        new(sa.value + 4) ZonedDateTime(s4);
        new(sa.value + 5) ZonedDateTime(s5);
        new(sa.value + 6) ZonedDateTime(s6);
        new(sa.value + 7) ZonedDateTime(s7);
        new(sa.value + 8) ZonedDateTime(s8);
        new(sa.value + 9) ZonedDateTime(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZonedDateTime& ZonedDateTimeArray::operator[](gint index) { return get(index); }

    ZonedDateTime const& ZonedDateTimeArray::operator[](gint index) const { return get(index); }

    ZonedDateTimeArray::LinearIterator::LinearIterator(ZonedDateTimeArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZonedDateTimeArray::LinearIterator& ZonedDateTimeArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZonedDateTime& ZonedDateTimeArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZonedDateTimeArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZonedDateTimeArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    ZonedDateTimeArray::LinearIterator2::LinearIterator2(ZonedDateTimeArray const& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZonedDateTimeArray::LinearIterator2& ZonedDateTimeArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZonedDateTime const& ZonedDateTimeArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZonedDateTimeArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZonedDateTimeArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    ZonedDateTimeArray::LinearIterator ZonedDateTimeArray::begin() { return LinearIterator(*this, false); }

    ZonedDateTimeArray::LinearIterator2 ZonedDateTimeArray::begin() const { return LinearIterator2(*this, false); }

    ZonedDateTimeArray::LinearIterator ZonedDateTimeArray::end() { return LinearIterator(*this, true); }

    ZonedDateTimeArray::LinearIterator2 ZonedDateTimeArray::end() const { return LinearIterator2(*this, true); }

    gbool ZonedDateTimeArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<ZonedDateTimeArray>::hasInstance(obj)) {
        return false;
      }

      ZonedDateTimeArray const& array = CORE_XCAST(ZonedDateTimeArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint ZonedDateTimeArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String ZonedDateTimeArray::toString() const {
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

    Object& ZonedDateTimeArray::clone() const {
      try {
        return UNSAFE::newInstance<ZonedDateTimeArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ZonedDateTimeArray::initializeFirstTeens(ZonedDateTime const& s0, ZonedDateTime const& s1, ZonedDateTime const& s2,
                                               ZonedDateTime const& s3, ZonedDateTime const& s4, ZonedDateTime const& s5,
                                               ZonedDateTime const& s6, ZonedDateTime const& s7, ZonedDateTime const& s8,
                                               ZonedDateTime const& s9) {
      new(value + 0) ZonedDateTime(s0);
      new(value + 1) ZonedDateTime(s1);
      new(value + 2) ZonedDateTime(s2);
      new(value + 3) ZonedDateTime(s3);
      new(value + 4) ZonedDateTime(s4);
      new(value + 5) ZonedDateTime(s5);
      new(value + 6) ZonedDateTime(s6);
      new(value + 7) ZonedDateTime(s7);
      new(value + 8) ZonedDateTime(s8);
      new(value + 9) ZonedDateTime(s9);
    }

    void ZonedDateTimeArray::writeAsZonedDateTime(gint idx, ZonedDateTime const& s) {
      new(value + idx) ZonedDateTime(s);
    }

    void ZonedDateTimeArray::writeZonedDateTimes(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
