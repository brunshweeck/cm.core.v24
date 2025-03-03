//
// Created by brunshweeck on 24/02/25.
//

#include "ZoneOffsetTransitionRuleArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/time/ZoneOffsetTransition.h>

namespace core {
  namespace time {
    ZoneOffsetTransitionRuleArray::ZoneOffsetTransitionRuleArray() CORE_NOTHROW: ZoneOffsetTransitionRuleArray(0) {}

    ZoneOffsetTransitionRuleArray::ZoneOffsetTransitionRuleArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffsetTransitionRule)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZoneOffsetTransitionRule(Month::JANUARY, 1, DayOfWeek::WEDNESDAY, LocalTime::of(0, 0), false,
                                                  ZoneOffsetTransitionRule::TimeDefinition::STANDARD, ZoneOffset::UTC,
                                                  ZoneOffset::UTC, ZoneOffset::UTC);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray::ZoneOffsetTransitionRuleArray(gint length,
                                                                 ZoneOffsetTransitionRule const& initialValue):
      ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffsetTransitionRule)));
        for (gint i = 0; i < length; i++)
          new(value + i) ZoneOffsetTransitionRule(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray::ZoneOffsetTransitionRuleArray(
      ZoneOffsetTransitionRuleArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(ZoneOffsetTransitionRule)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) ZoneOffsetTransitionRule(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray::ZoneOffsetTransitionRuleArray(ZoneOffsetTransitionRuleArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    ZoneOffsetTransitionRuleArray::ZoneOffsetTransitionRuleArray(VarArgs const& args): ZoneOffsetTransitionRuleArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(ZoneOffsetTransitionRule)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) ZoneOffsetTransitionRule(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray& ZoneOffsetTransitionRuleArray::operator
    =(const ZoneOffsetTransitionRuleArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(ZoneOffsetTransitionRule)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) ZoneOffsetTransitionRule(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    ZoneOffsetTransitionRuleArray& ZoneOffsetTransitionRuleArray::operator=(
      ZoneOffsetTransitionRuleArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint ZoneOffsetTransitionRuleArray::length() const { return count; }

    gbool ZoneOffsetTransitionRuleArray::isEmpty() const { return count == 0; }

    ZoneOffsetTransitionRule& ZoneOffsetTransitionRuleArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRule const& ZoneOffsetTransitionRuleArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRule ZoneOffsetTransitionRuleArray::set(gint index, ZoneOffsetTransitionRule newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        ZoneOffsetTransitionRule oldValue = value[index];
        new(value + index) ZoneOffsetTransitionRule(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray::~ZoneOffsetTransitionRuleArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::copyOf(Array<ZoneOffsetTransitionRule> const& a) {
      gint n = a.length();
      ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) ZoneOffsetTransitionRule(a[i]);
      return sa;
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of() { return { }; }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0) {
      return ZoneOffsetTransitionRuleArray(1, UNSAFE::moveInstance(s0));
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(2);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1,
                                                                    ZoneOffsetTransitionRule const& s2) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(3);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        new(sa.value + 2) ZoneOffsetTransitionRule(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1,
                                                                    ZoneOffsetTransitionRule const& s2,
                                                                    ZoneOffsetTransitionRule const& s3) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(4);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        new(sa.value + 2) ZoneOffsetTransitionRule(s2);
        new(sa.value + 3) ZoneOffsetTransitionRule(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1,
                                                                    ZoneOffsetTransitionRule const& s2,
                                                                    ZoneOffsetTransitionRule const& s3,
                                                                    ZoneOffsetTransitionRule const& s4) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(5);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        new(sa.value + 2) ZoneOffsetTransitionRule(s2);
        new(sa.value + 3) ZoneOffsetTransitionRule(s3);
        new(sa.value + 4) ZoneOffsetTransitionRule(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1,
                                                                    ZoneOffsetTransitionRule const& s2,
                                                                    ZoneOffsetTransitionRule const& s3,
                                                                    ZoneOffsetTransitionRule const& s4,
                                                                    ZoneOffsetTransitionRule const& s5) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(6);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        new(sa.value + 2) ZoneOffsetTransitionRule(s2);
        new(sa.value + 3) ZoneOffsetTransitionRule(s3);
        new(sa.value + 4) ZoneOffsetTransitionRule(s4);
        new(sa.value + 5) ZoneOffsetTransitionRule(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1,
                                                                    ZoneOffsetTransitionRule const& s2,
                                                                    ZoneOffsetTransitionRule const& s3,
                                                                    ZoneOffsetTransitionRule const& s4,
                                                                    ZoneOffsetTransitionRule const& s5,
                                                                    ZoneOffsetTransitionRule const& s6) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(7);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        new(sa.value + 2) ZoneOffsetTransitionRule(s2);
        new(sa.value + 3) ZoneOffsetTransitionRule(s3);
        new(sa.value + 4) ZoneOffsetTransitionRule(s4);
        new(sa.value + 5) ZoneOffsetTransitionRule(s5);
        new(sa.value + 6) ZoneOffsetTransitionRule(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1,
                                                                    ZoneOffsetTransitionRule const& s2,
                                                                    ZoneOffsetTransitionRule const& s3,
                                                                    ZoneOffsetTransitionRule const& s4,
                                                                    ZoneOffsetTransitionRule const& s5,
                                                                    ZoneOffsetTransitionRule const& s6,
                                                                    ZoneOffsetTransitionRule const& s7) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(8);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        new(sa.value + 2) ZoneOffsetTransitionRule(s2);
        new(sa.value + 3) ZoneOffsetTransitionRule(s3);
        new(sa.value + 4) ZoneOffsetTransitionRule(s4);
        new(sa.value + 5) ZoneOffsetTransitionRule(s5);
        new(sa.value + 6) ZoneOffsetTransitionRule(s6);
        new(sa.value + 7) ZoneOffsetTransitionRule(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1,
                                                                    ZoneOffsetTransitionRule const& s2,
                                                                    ZoneOffsetTransitionRule const& s3,
                                                                    ZoneOffsetTransitionRule const& s4,
                                                                    ZoneOffsetTransitionRule const& s5,
                                                                    ZoneOffsetTransitionRule const& s6,
                                                                    ZoneOffsetTransitionRule const& s7,
                                                                    ZoneOffsetTransitionRule const& s8) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(9);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        new(sa.value + 2) ZoneOffsetTransitionRule(s2);
        new(sa.value + 3) ZoneOffsetTransitionRule(s3);
        new(sa.value + 4) ZoneOffsetTransitionRule(s4);
        new(sa.value + 5) ZoneOffsetTransitionRule(s5);
        new(sa.value + 6) ZoneOffsetTransitionRule(s6);
        new(sa.value + 7) ZoneOffsetTransitionRule(s7);
        new(sa.value + 8) ZoneOffsetTransitionRule(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRuleArray ZoneOffsetTransitionRuleArray::of(ZoneOffsetTransitionRule const& s0,
                                                                    ZoneOffsetTransitionRule const& s1,
                                                                    ZoneOffsetTransitionRule const& s2,
                                                                    ZoneOffsetTransitionRule const& s3,
                                                                    ZoneOffsetTransitionRule const& s4,
                                                                    ZoneOffsetTransitionRule const& s5,
                                                                    ZoneOffsetTransitionRule const& s6,
                                                                    ZoneOffsetTransitionRule const& s7,
                                                                    ZoneOffsetTransitionRule const& s8,
                                                                    ZoneOffsetTransitionRule const& s9) {
      try {
        ZoneOffsetTransitionRuleArray sa = ZoneOffsetTransitionRuleArray(10);
        new(sa.value + 0) ZoneOffsetTransitionRule(s0);
        new(sa.value + 1) ZoneOffsetTransitionRule(s1);
        new(sa.value + 2) ZoneOffsetTransitionRule(s2);
        new(sa.value + 3) ZoneOffsetTransitionRule(s3);
        new(sa.value + 4) ZoneOffsetTransitionRule(s4);
        new(sa.value + 5) ZoneOffsetTransitionRule(s5);
        new(sa.value + 6) ZoneOffsetTransitionRule(s6);
        new(sa.value + 7) ZoneOffsetTransitionRule(s7);
        new(sa.value + 8) ZoneOffsetTransitionRule(s8);
        new(sa.value + 9) ZoneOffsetTransitionRule(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffsetTransitionRule& ZoneOffsetTransitionRuleArray::operator[](gint index) { return get(index); }

    ZoneOffsetTransitionRule const& ZoneOffsetTransitionRuleArray::operator[](gint index) const { return get(index); }

    ZoneOffsetTransitionRuleArray::LinearIterator::LinearIterator(ZoneOffsetTransitionRuleArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZoneOffsetTransitionRuleArray::LinearIterator& ZoneOffsetTransitionRuleArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZoneOffsetTransitionRule& ZoneOffsetTransitionRuleArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZoneOffsetTransitionRuleArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZoneOffsetTransitionRuleArray::LinearIterator::operator!=(LinearIterator const& rhs) const {
      return !(*this == rhs);
    }

    ZoneOffsetTransitionRuleArray::LinearIterator2::LinearIterator2(ZoneOffsetTransitionRuleArray const& array,
                                                                    gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    ZoneOffsetTransitionRuleArray::LinearIterator2& ZoneOffsetTransitionRuleArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    ZoneOffsetTransitionRule const& ZoneOffsetTransitionRuleArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool ZoneOffsetTransitionRuleArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ZoneOffsetTransitionRuleArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const {
      return !(*this == rhs);
    }

    ZoneOffsetTransitionRuleArray::LinearIterator ZoneOffsetTransitionRuleArray::begin() {
      return LinearIterator(*this, false);
    }

    ZoneOffsetTransitionRuleArray::LinearIterator2 ZoneOffsetTransitionRuleArray::begin() const {
      return LinearIterator2(*this, false);
    }

    ZoneOffsetTransitionRuleArray::LinearIterator ZoneOffsetTransitionRuleArray::end() {
      return LinearIterator(*this, true);
    }

    ZoneOffsetTransitionRuleArray::LinearIterator2 ZoneOffsetTransitionRuleArray::end() const {
      return LinearIterator2(*this, true);
    }

    gbool ZoneOffsetTransitionRuleArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<ZoneOffsetTransitionRuleArray>::hasInstance(obj)) {
        return false;
      }

      ZoneOffsetTransitionRuleArray const& array = CORE_XCAST(ZoneOffsetTransitionRuleArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint ZoneOffsetTransitionRuleArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String ZoneOffsetTransitionRuleArray::toString() const {
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

    Object& ZoneOffsetTransitionRuleArray::clone() const {
      try {
        return UNSAFE::newInstance<ZoneOffsetTransitionRuleArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void ZoneOffsetTransitionRuleArray::initializeFirstTeens(ZoneOffsetTransitionRule const& s0,
                                                             ZoneOffsetTransitionRule const& s1,
                                                             ZoneOffsetTransitionRule const& s2,
                                                             ZoneOffsetTransitionRule const& s3,
                                                             ZoneOffsetTransitionRule const& s4,
                                                             ZoneOffsetTransitionRule const& s5,
                                                             ZoneOffsetTransitionRule const& s6,
                                                             ZoneOffsetTransitionRule const& s7,
                                                             ZoneOffsetTransitionRule const& s8,
                                                             ZoneOffsetTransitionRule const& s9) {
      new(value + 0) ZoneOffsetTransitionRule(s0);
      new(value + 1) ZoneOffsetTransitionRule(s1);
      new(value + 2) ZoneOffsetTransitionRule(s2);
      new(value + 3) ZoneOffsetTransitionRule(s3);
      new(value + 4) ZoneOffsetTransitionRule(s4);
      new(value + 5) ZoneOffsetTransitionRule(s5);
      new(value + 6) ZoneOffsetTransitionRule(s6);
      new(value + 7) ZoneOffsetTransitionRule(s7);
      new(value + 8) ZoneOffsetTransitionRule(s8);
      new(value + 9) ZoneOffsetTransitionRule(s9);
    }

    void ZoneOffsetTransitionRuleArray::writeAsZoneOffsetTransitionRule(gint idx, ZoneOffsetTransitionRule const& s) {
      new(value + idx) ZoneOffsetTransitionRule(s);
    }

    void ZoneOffsetTransitionRuleArray::writeZoneOffsetTransitionRules(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
