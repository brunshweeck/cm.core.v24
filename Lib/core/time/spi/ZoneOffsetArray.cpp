//
// Created by brunshweeck on 24/02/25.
//

#include "ZoneOffsetArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    ZoneOffsetArray::ZoneOffsetArray() CORE_NOTHROW: ZoneOffsetArray(0) {}

    ZoneOffsetArray::ZoneOffsetArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffset)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZoneOffset(ZoneOffset::ofTotalSeconds(0));
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray::ZoneOffsetArray(gint length, ZoneOffset const& initialValue): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffset)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZoneOffset(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray::ZoneOffsetArray(ZoneOffsetArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(ZoneOffset)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) ZoneOffset(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray::ZoneOffsetArray(ZoneOffsetArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    ZoneOffsetArray::ZoneOffsetArray(VarArgs const& args): ZoneOffsetArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffset)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) ZoneOffset(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray& ZoneOffsetArray::operator=(const ZoneOffsetArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(ZoneOffset)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) ZoneOffset(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    ZoneOffsetArray& ZoneOffsetArray::operator=(ZoneOffsetArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint ZoneOffsetArray::length() const { return count; }

    gbool ZoneOffsetArray::isEmpty() const { return count == 0; }

    ZoneOffset& ZoneOffsetArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffset const& ZoneOffsetArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffset ZoneOffsetArray::set(gint index, ZoneOffset newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        ZoneOffset oldValue = value[index];
        new(value + index) ZoneOffset(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray::~ZoneOffsetArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    ZoneOffsetArray ZoneOffsetArray::copyOf(Array<ZoneOffset> const& a) {
      gint n = a.length();
      ZoneOffsetArray sa = ZoneOffsetArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) ZoneOffset(a[i]);
      return sa;
    }

    ZoneOffsetArray ZoneOffsetArray::of() { return { }; }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0) {
      return ZoneOffsetArray(1, UNSAFE::moveInstance(s0));
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(2);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(3);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        new(sa.value + 2) ZoneOffset(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                        ZoneOffset const& s3) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(4);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        new(sa.value + 2) ZoneOffset(s2);
        new(sa.value + 3) ZoneOffset(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                        ZoneOffset const& s3, ZoneOffset const& s4) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(5);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        new(sa.value + 2) ZoneOffset(s2);
        new(sa.value + 3) ZoneOffset(s3);
        new(sa.value + 4) ZoneOffset(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                        ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(6);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        new(sa.value + 2) ZoneOffset(s2);
        new(sa.value + 3) ZoneOffset(s3);
        new(sa.value + 4) ZoneOffset(s4);
        new(sa.value + 5) ZoneOffset(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                        ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                        ZoneOffset const& s6) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(7);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        new(sa.value + 2) ZoneOffset(s2);
        new(sa.value + 3) ZoneOffset(s3);
        new(sa.value + 4) ZoneOffset(s4);
        new(sa.value + 5) ZoneOffset(s5);
        new(sa.value + 6) ZoneOffset(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                        ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                        ZoneOffset const& s6, ZoneOffset const& s7) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(8);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        new(sa.value + 2) ZoneOffset(s2);
        new(sa.value + 3) ZoneOffset(s3);
        new(sa.value + 4) ZoneOffset(s4);
        new(sa.value + 5) ZoneOffset(s5);
        new(sa.value + 6) ZoneOffset(s6);
        new(sa.value + 7) ZoneOffset(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                        ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                        ZoneOffset const& s6, ZoneOffset const& s7, ZoneOffset const& s8) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(9);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        new(sa.value + 2) ZoneOffset(s2);
        new(sa.value + 3) ZoneOffset(s3);
        new(sa.value + 4) ZoneOffset(s4);
        new(sa.value + 5) ZoneOffset(s5);
        new(sa.value + 6) ZoneOffset(s6);
        new(sa.value + 7) ZoneOffset(s7);
        new(sa.value + 8) ZoneOffset(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetArray ZoneOffsetArray::of(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                        ZoneOffset const& s3,
                                        ZoneOffset const& s4, ZoneOffset const& s5, ZoneOffset const& s6,
                                        ZoneOffset const& s7, ZoneOffset const& s8, ZoneOffset const& s9) {
      try {
        ZoneOffsetArray sa = ZoneOffsetArray(10);
        new(sa.value + 0) ZoneOffset(s0);
        new(sa.value + 1) ZoneOffset(s1);
        new(sa.value + 2) ZoneOffset(s2);
        new(sa.value + 3) ZoneOffset(s3);
        new(sa.value + 4) ZoneOffset(s4);
        new(sa.value + 5) ZoneOffset(s5);
        new(sa.value + 6) ZoneOffset(s6);
        new(sa.value + 7) ZoneOffset(s7);
        new(sa.value + 8) ZoneOffset(s8);
        new(sa.value + 9) ZoneOffset(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffset& ZoneOffsetArray::operator[](gint index) { return get(index); }

    ZoneOffset const& ZoneOffsetArray::operator[](gint index) const { return get(index); }

    ZoneOffsetArray::LinearIterator::LinearIterator(ZoneOffsetArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZoneOffsetArray::LinearIterator& ZoneOffsetArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZoneOffset& ZoneOffsetArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZoneOffsetArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZoneOffsetArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    ZoneOffsetArray::LinearIterator2::LinearIterator2(ZoneOffsetArray const& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZoneOffsetArray::LinearIterator2& ZoneOffsetArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZoneOffset const& ZoneOffsetArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZoneOffsetArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZoneOffsetArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    ZoneOffsetArray::LinearIterator ZoneOffsetArray::begin() { return LinearIterator(*this, false); }

    ZoneOffsetArray::LinearIterator2 ZoneOffsetArray::begin() const { return LinearIterator2(*this, false); }

    ZoneOffsetArray::LinearIterator ZoneOffsetArray::end() { return LinearIterator(*this, true); }

    ZoneOffsetArray::LinearIterator2 ZoneOffsetArray::end() const { return LinearIterator2(*this, true); }

    gbool ZoneOffsetArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<ZoneOffsetArray>::hasInstance(obj)) {
        return false;
      }

      ZoneOffsetArray const& array = CORE_XCAST(ZoneOffsetArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint ZoneOffsetArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String ZoneOffsetArray::toString() const {
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

    Object& ZoneOffsetArray::clone() const {
      try {
        return UNSAFE::newInstance<ZoneOffsetArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ZoneOffsetArray::initializeFirstTeens(ZoneOffset const& s0, ZoneOffset const& s1, ZoneOffset const& s2,
                                               ZoneOffset const& s3, ZoneOffset const& s4, ZoneOffset const& s5,
                                               ZoneOffset const& s6, ZoneOffset const& s7, ZoneOffset const& s8,
                                               ZoneOffset const& s9) {
      new(value + 0) ZoneOffset(s0);
      new(value + 1) ZoneOffset(s1);
      new(value + 2) ZoneOffset(s2);
      new(value + 3) ZoneOffset(s3);
      new(value + 4) ZoneOffset(s4);
      new(value + 5) ZoneOffset(s5);
      new(value + 6) ZoneOffset(s6);
      new(value + 7) ZoneOffset(s7);
      new(value + 8) ZoneOffset(s8);
      new(value + 9) ZoneOffset(s9);
    }

    void ZoneOffsetArray::writeAsZoneOffset(gint idx, ZoneOffset const& s) {
      new(value + idx) ZoneOffset(s);
    }

    void ZoneOffsetArray::writeZoneOffsets(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
