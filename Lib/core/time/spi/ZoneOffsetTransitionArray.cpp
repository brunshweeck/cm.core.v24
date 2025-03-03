//
// Created by brunshweeck on 01/03/25.
//

#include "ZoneOffsetTransitionArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    ZoneOffsetTransitionArray::ZoneOffsetTransitionArray() CORE_NOTHROW: ZoneOffsetTransitionArray(0) {}

    ZoneOffsetTransitionArray::ZoneOffsetTransitionArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffsetTransition)));
        // Wednesday, 1st January 1970
        for (gint i = 0; i < length; i++)
          new(value + i) ZoneOffsetTransition(0, ZoneOffset::UTC, ZoneOffset::UTC);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray::ZoneOffsetTransitionArray(gint length, ZoneOffsetTransition const& initialValue): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffsetTransition)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZoneOffsetTransition(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray::ZoneOffsetTransitionArray(ZoneOffsetTransitionArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(ZoneOffsetTransition)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) ZoneOffsetTransition(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray::ZoneOffsetTransitionArray(ZoneOffsetTransitionArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    ZoneOffsetTransitionArray::ZoneOffsetTransitionArray(VarArgs const& args): ZoneOffsetTransitionArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffsetTransition)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) ZoneOffsetTransition(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray& ZoneOffsetTransitionArray::operator=(const ZoneOffsetTransitionArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(ZoneOffsetTransition)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) ZoneOffsetTransition(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    ZoneOffsetTransitionArray& ZoneOffsetTransitionArray::operator=(ZoneOffsetTransitionArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint ZoneOffsetTransitionArray::length() const { return count; }

    gbool ZoneOffsetTransitionArray::isEmpty() const { return count == 0; }

    ZoneOffsetTransition& ZoneOffsetTransitionArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransition const& ZoneOffsetTransitionArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransition ZoneOffsetTransitionArray::set(gint index, ZoneOffsetTransition newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        ZoneOffsetTransition oldValue = value[index];
        new(value + index) ZoneOffsetTransition(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray::~ZoneOffsetTransitionArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::copyOf(Array<ZoneOffsetTransition> const& a) {
      gint n = a.length();
      ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) ZoneOffsetTransition(a[i]);
      return sa;
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of() { return { }; }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0) {
      return ZoneOffsetTransitionArray(1, UNSAFE::moveInstance(s0));
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(2);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(3);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        new(sa.value + 2) ZoneOffsetTransition(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2,
                                        ZoneOffsetTransition const& s3) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(4);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        new(sa.value + 2) ZoneOffsetTransition(s2);
        new(sa.value + 3) ZoneOffsetTransition(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2,
                                        ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(5);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        new(sa.value + 2) ZoneOffsetTransition(s2);
        new(sa.value + 3) ZoneOffsetTransition(s3);
        new(sa.value + 4) ZoneOffsetTransition(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2,
                                        ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4, ZoneOffsetTransition const& s5) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(6);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        new(sa.value + 2) ZoneOffsetTransition(s2);
        new(sa.value + 3) ZoneOffsetTransition(s3);
        new(sa.value + 4) ZoneOffsetTransition(s4);
        new(sa.value + 5) ZoneOffsetTransition(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2,
                                        ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4, ZoneOffsetTransition const& s5,
                                        ZoneOffsetTransition const& s6) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(7);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        new(sa.value + 2) ZoneOffsetTransition(s2);
        new(sa.value + 3) ZoneOffsetTransition(s3);
        new(sa.value + 4) ZoneOffsetTransition(s4);
        new(sa.value + 5) ZoneOffsetTransition(s5);
        new(sa.value + 6) ZoneOffsetTransition(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2,
                                        ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4, ZoneOffsetTransition const& s5,
                                        ZoneOffsetTransition const& s6, ZoneOffsetTransition const& s7) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(8);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        new(sa.value + 2) ZoneOffsetTransition(s2);
        new(sa.value + 3) ZoneOffsetTransition(s3);
        new(sa.value + 4) ZoneOffsetTransition(s4);
        new(sa.value + 5) ZoneOffsetTransition(s5);
        new(sa.value + 6) ZoneOffsetTransition(s6);
        new(sa.value + 7) ZoneOffsetTransition(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2,
                                        ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4, ZoneOffsetTransition const& s5,
                                        ZoneOffsetTransition const& s6, ZoneOffsetTransition const& s7, ZoneOffsetTransition const& s8) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(9);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        new(sa.value + 2) ZoneOffsetTransition(s2);
        new(sa.value + 3) ZoneOffsetTransition(s3);
        new(sa.value + 4) ZoneOffsetTransition(s4);
        new(sa.value + 5) ZoneOffsetTransition(s5);
        new(sa.value + 6) ZoneOffsetTransition(s6);
        new(sa.value + 7) ZoneOffsetTransition(s7);
        new(sa.value + 8) ZoneOffsetTransition(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionArray ZoneOffsetTransitionArray::of(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2,
                                        ZoneOffsetTransition const& s3,
                                        ZoneOffsetTransition const& s4, ZoneOffsetTransition const& s5, ZoneOffsetTransition const& s6,
                                        ZoneOffsetTransition const& s7, ZoneOffsetTransition const& s8, ZoneOffsetTransition const& s9) {
      try {
        ZoneOffsetTransitionArray sa = ZoneOffsetTransitionArray(10);
        new(sa.value + 0) ZoneOffsetTransition(s0);
        new(sa.value + 1) ZoneOffsetTransition(s1);
        new(sa.value + 2) ZoneOffsetTransition(s2);
        new(sa.value + 3) ZoneOffsetTransition(s3);
        new(sa.value + 4) ZoneOffsetTransition(s4);
        new(sa.value + 5) ZoneOffsetTransition(s5);
        new(sa.value + 6) ZoneOffsetTransition(s6);
        new(sa.value + 7) ZoneOffsetTransition(s7);
        new(sa.value + 8) ZoneOffsetTransition(s8);
        new(sa.value + 9) ZoneOffsetTransition(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransition& ZoneOffsetTransitionArray::operator[](gint index) { return get(index); }

    ZoneOffsetTransition const& ZoneOffsetTransitionArray::operator[](gint index) const { return get(index); }

    ZoneOffsetTransitionArray::LinearIterator::LinearIterator(ZoneOffsetTransitionArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZoneOffsetTransitionArray::LinearIterator& ZoneOffsetTransitionArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZoneOffsetTransition& ZoneOffsetTransitionArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZoneOffsetTransitionArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZoneOffsetTransitionArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    ZoneOffsetTransitionArray::LinearIterator2::LinearIterator2(ZoneOffsetTransitionArray const& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZoneOffsetTransitionArray::LinearIterator2& ZoneOffsetTransitionArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZoneOffsetTransition const& ZoneOffsetTransitionArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZoneOffsetTransitionArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZoneOffsetTransitionArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    ZoneOffsetTransitionArray::LinearIterator ZoneOffsetTransitionArray::begin() { return LinearIterator(*this, false); }

    ZoneOffsetTransitionArray::LinearIterator2 ZoneOffsetTransitionArray::begin() const { return LinearIterator2(*this, false); }

    ZoneOffsetTransitionArray::LinearIterator ZoneOffsetTransitionArray::end() { return LinearIterator(*this, true); }

    ZoneOffsetTransitionArray::LinearIterator2 ZoneOffsetTransitionArray::end() const { return LinearIterator2(*this, true); }

    gbool ZoneOffsetTransitionArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<ZoneOffsetTransitionArray>::hasInstance(obj)) {
        return false;
      }

      ZoneOffsetTransitionArray const& array = CORE_XCAST(ZoneOffsetTransitionArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint ZoneOffsetTransitionArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String ZoneOffsetTransitionArray::toString() const {
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

    Object& ZoneOffsetTransitionArray::clone() const {
      try {
        return UNSAFE::newInstance<ZoneOffsetTransitionArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ZoneOffsetTransitionArray::initializeFirstTeens(ZoneOffsetTransition const& s0, ZoneOffsetTransition const& s1, ZoneOffsetTransition const& s2,
                                               ZoneOffsetTransition const& s3, ZoneOffsetTransition const& s4, ZoneOffsetTransition const& s5,
                                               ZoneOffsetTransition const& s6, ZoneOffsetTransition const& s7, ZoneOffsetTransition const& s8,
                                               ZoneOffsetTransition const& s9) {
      new(value + 0) ZoneOffsetTransition(s0);
      new(value + 1) ZoneOffsetTransition(s1);
      new(value + 2) ZoneOffsetTransition(s2);
      new(value + 3) ZoneOffsetTransition(s3);
      new(value + 4) ZoneOffsetTransition(s4);
      new(value + 5) ZoneOffsetTransition(s5);
      new(value + 6) ZoneOffsetTransition(s6);
      new(value + 7) ZoneOffsetTransition(s7);
      new(value + 8) ZoneOffsetTransition(s8);
      new(value + 9) ZoneOffsetTransition(s9);
    }

    void ZoneOffsetTransitionArray::writeAsZoneOffsetTransition(gint idx, ZoneOffsetTransition const& s) {
      new(value + idx) ZoneOffsetTransition(s);
    }

    void ZoneOffsetTransitionArray::writeZoneOffsetTransitions(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
