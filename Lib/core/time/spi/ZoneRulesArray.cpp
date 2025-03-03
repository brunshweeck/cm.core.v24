//
// Created by brunshweeck on 02/03/25.
//

#include "ZoneRulesArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    ZoneRulesArray::ZoneRulesArray() CORE_NOTHROW: ZoneRulesArray(0) {}

    ZoneRulesArray::ZoneRulesArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneRules)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZoneRules(ZoneRules::of(ZoneOffset::UTC));
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray::ZoneRulesArray(gint length, ZoneRules const& initialValue): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneRules)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZoneRules(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray::ZoneRulesArray(ZoneRulesArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(ZoneRules)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) ZoneRules(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray::ZoneRulesArray(ZoneRulesArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    ZoneRulesArray::ZoneRulesArray(VarArgs const& args): ZoneRulesArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneRules)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) ZoneRules(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray& ZoneRulesArray::operator=(const ZoneRulesArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(ZoneRules)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) ZoneRules(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    ZoneRulesArray& ZoneRulesArray::operator=(ZoneRulesArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint ZoneRulesArray::length() const { return count; }

    gbool ZoneRulesArray::isEmpty() const { return count == 0; }

    ZoneRules& ZoneRulesArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRules const& ZoneRulesArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRules ZoneRulesArray::set(gint index, ZoneRules newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        ZoneRules oldValue = value[index];
        new(value + index) ZoneRules(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray::~ZoneRulesArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    ZoneRulesArray ZoneRulesArray::copyOf(Array<ZoneRules> const& a) {
      gint n = a.length();
      ZoneRulesArray sa = ZoneRulesArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) ZoneRules(a[i]);
      return sa;
    }

    ZoneRulesArray ZoneRulesArray::of() { return { }; }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0) {
      return ZoneRulesArray(1, UNSAFE::moveInstance(s0));
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(2);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(3);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        new(sa.value + 2) ZoneRules(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                        ZoneRules const& s3) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(4);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        new(sa.value + 2) ZoneRules(s2);
        new(sa.value + 3) ZoneRules(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                        ZoneRules const& s3, ZoneRules const& s4) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(5);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        new(sa.value + 2) ZoneRules(s2);
        new(sa.value + 3) ZoneRules(s3);
        new(sa.value + 4) ZoneRules(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                        ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(6);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        new(sa.value + 2) ZoneRules(s2);
        new(sa.value + 3) ZoneRules(s3);
        new(sa.value + 4) ZoneRules(s4);
        new(sa.value + 5) ZoneRules(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                        ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                        ZoneRules const& s6) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(7);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        new(sa.value + 2) ZoneRules(s2);
        new(sa.value + 3) ZoneRules(s3);
        new(sa.value + 4) ZoneRules(s4);
        new(sa.value + 5) ZoneRules(s5);
        new(sa.value + 6) ZoneRules(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                        ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                        ZoneRules const& s6, ZoneRules const& s7) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(8);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        new(sa.value + 2) ZoneRules(s2);
        new(sa.value + 3) ZoneRules(s3);
        new(sa.value + 4) ZoneRules(s4);
        new(sa.value + 5) ZoneRules(s5);
        new(sa.value + 6) ZoneRules(s6);
        new(sa.value + 7) ZoneRules(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                        ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                        ZoneRules const& s6, ZoneRules const& s7, ZoneRules const& s8) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(9);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        new(sa.value + 2) ZoneRules(s2);
        new(sa.value + 3) ZoneRules(s3);
        new(sa.value + 4) ZoneRules(s4);
        new(sa.value + 5) ZoneRules(s5);
        new(sa.value + 6) ZoneRules(s6);
        new(sa.value + 7) ZoneRules(s7);
        new(sa.value + 8) ZoneRules(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRulesArray ZoneRulesArray::of(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                        ZoneRules const& s3,
                                        ZoneRules const& s4, ZoneRules const& s5, ZoneRules const& s6,
                                        ZoneRules const& s7, ZoneRules const& s8, ZoneRules const& s9) {
      try {
        ZoneRulesArray sa = ZoneRulesArray(10);
        new(sa.value + 0) ZoneRules(s0);
        new(sa.value + 1) ZoneRules(s1);
        new(sa.value + 2) ZoneRules(s2);
        new(sa.value + 3) ZoneRules(s3);
        new(sa.value + 4) ZoneRules(s4);
        new(sa.value + 5) ZoneRules(s5);
        new(sa.value + 6) ZoneRules(s6);
        new(sa.value + 7) ZoneRules(s7);
        new(sa.value + 8) ZoneRules(s8);
        new(sa.value + 9) ZoneRules(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneRules& ZoneRulesArray::operator[](gint index) { return get(index); }

    ZoneRules const& ZoneRulesArray::operator[](gint index) const { return get(index); }

    ZoneRulesArray::LinearIterator::LinearIterator(ZoneRulesArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZoneRulesArray::LinearIterator& ZoneRulesArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZoneRules& ZoneRulesArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZoneRulesArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZoneRulesArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    ZoneRulesArray::LinearIterator2::LinearIterator2(ZoneRulesArray const& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZoneRulesArray::LinearIterator2& ZoneRulesArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZoneRules const& ZoneRulesArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZoneRulesArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZoneRulesArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    ZoneRulesArray::LinearIterator ZoneRulesArray::begin() { return LinearIterator(*this, false); }

    ZoneRulesArray::LinearIterator2 ZoneRulesArray::begin() const { return LinearIterator2(*this, false); }

    ZoneRulesArray::LinearIterator ZoneRulesArray::end() { return LinearIterator(*this, true); }

    ZoneRulesArray::LinearIterator2 ZoneRulesArray::end() const { return LinearIterator2(*this, true); }

    gbool ZoneRulesArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<ZoneRulesArray>::hasInstance(obj)) {
        return false;
      }

      ZoneRulesArray const& array = CORE_XCAST(ZoneRulesArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint ZoneRulesArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String ZoneRulesArray::toString() const {
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

    Object& ZoneRulesArray::clone() const {
      try {
        return UNSAFE::newInstance<ZoneRulesArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ZoneRulesArray::initializeFirstTeens(ZoneRules const& s0, ZoneRules const& s1, ZoneRules const& s2,
                                               ZoneRules const& s3, ZoneRules const& s4, ZoneRules const& s5,
                                               ZoneRules const& s6, ZoneRules const& s7, ZoneRules const& s8,
                                               ZoneRules const& s9) {
      new(value + 0) ZoneRules(s0);
      new(value + 1) ZoneRules(s1);
      new(value + 2) ZoneRules(s2);
      new(value + 3) ZoneRules(s3);
      new(value + 4) ZoneRules(s4);
      new(value + 5) ZoneRules(s5);
      new(value + 6) ZoneRules(s6);
      new(value + 7) ZoneRules(s7);
      new(value + 8) ZoneRules(s8);
      new(value + 9) ZoneRules(s9);
    }

    void ZoneRulesArray::writeAsZoneRules(gint idx, ZoneRules const& s) {
      new(value + idx) ZoneRules(s);
    }

    void ZoneRulesArray::writeZoneRuless(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
