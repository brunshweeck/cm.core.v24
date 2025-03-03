//
// Created by brunshweeck on 01/03/25.
//

#include "InstantArray.h"

#include <core/lang/Array.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
  namespace time {
    InstantArray::InstantArray() CORE_NOTHROW: InstantArray(0) {}

    InstantArray::InstantArray(gint length): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(Instant)));
        for (gint i = 0; i < length; i++)
          new(value + i) Instant(Instant::EPOCH);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray::InstantArray(gint length, Instant const& initialValue): ValueArray() {
      if (length < 0)
        IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(Instant)));
        for (gint i = 0; i < length; i++)
          new(value + i) Instant(initialValue);
        count = length;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray::InstantArray(InstantArray const& array): ValueArray(array) {
      try {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(Instant)));
        count = array.count;

        for (gint i = 0; i < count; i++)
          new(value + i) Instant(array.value[i]);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray::InstantArray(InstantArray&& array) CORE_NOTHROW {
      UNSAFE::swapValues(value, array.value);
      UNSAFE::swapValues(count, array.count);
    }

    InstantArray::InstantArray(VarArgs const& args): InstantArray() {
      try {
        gint length = args.size();
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(Instant)));
        count = length;
        for (int i = 0; i < length; ++i)
          new(value + i) Instant(UNSAFE::moveInstance(args.begin()[i]));
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray& InstantArray::operator=(const InstantArray& other) {
      if (this != &other) {
        try {
          if (count < other.count) {
            value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                                other.count * sizeof(Instant)));
          }
          for (gint i = 0; i < other.count; i++)
            new(value + i) Instant(other.value[i]);
          count = other.count;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
      }
      return *this;
    }

    InstantArray& InstantArray::operator=(InstantArray&& other) CORE_NOTHROW {
      if (this != &other) {
        UNSAFE::swapValues(value, other.value);
        UNSAFE::swapValues(count, other.count);
      }
      return *this;
    }

    gint InstantArray::length() const { return count; }

    gbool InstantArray::isEmpty() const { return count == 0; }

    Instant& InstantArray::get(gint index) {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Instant const& InstantArray::get(gint index) const {
      try {
        misc::Preconditions::checkIndex(index, count);
        return value[index];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Instant InstantArray::set(gint index, Instant newValue) {
      try {
        misc::Preconditions::checkIndex(index, count);
        Instant oldValue = value[index];
        new(value + index) Instant(newValue);
        return oldValue;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray::~InstantArray() {
      const ARRAY a = value;
      const gint n = count;

      value = null;
      count = 0;

      if (n > 0) {
        UNSAFE::freeMemory(CORE_CAST(glong, a));
      }
    }

    InstantArray InstantArray::copyOf(Array<Instant> const& a) {
      gint n = a.length();
      InstantArray sa = InstantArray(n);
      for (gint i = 0; i < n; i++)
        new(sa.value + i) Instant(a[i]);
      return sa;
    }

    InstantArray InstantArray::of() { return { }; }

    InstantArray InstantArray::of(Instant const& s0) {
      return InstantArray(1, UNSAFE::moveInstance(s0));
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1) {
      try {
        InstantArray sa = InstantArray(2);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1, Instant const& s2) {
      try {
        InstantArray sa = InstantArray(3);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        new(sa.value + 2) Instant(s2);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1, Instant const& s2,
                                        Instant const& s3) {
      try {
        InstantArray sa = InstantArray(4);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        new(sa.value + 2) Instant(s2);
        new(sa.value + 3) Instant(s3);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1, Instant const& s2,
                                        Instant const& s3, Instant const& s4) {
      try {
        InstantArray sa = InstantArray(5);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        new(sa.value + 2) Instant(s2);
        new(sa.value + 3) Instant(s3);
        new(sa.value + 4) Instant(s4);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1, Instant const& s2,
                                        Instant const& s3, Instant const& s4, Instant const& s5) {
      try {
        InstantArray sa = InstantArray(6);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        new(sa.value + 2) Instant(s2);
        new(sa.value + 3) Instant(s3);
        new(sa.value + 4) Instant(s4);
        new(sa.value + 5) Instant(s5);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1, Instant const& s2,
                                        Instant const& s3, Instant const& s4, Instant const& s5,
                                        Instant const& s6) {
      try {
        InstantArray sa = InstantArray(7);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        new(sa.value + 2) Instant(s2);
        new(sa.value + 3) Instant(s3);
        new(sa.value + 4) Instant(s4);
        new(sa.value + 5) Instant(s5);
        new(sa.value + 6) Instant(s6);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1, Instant const& s2,
                                        Instant const& s3, Instant const& s4, Instant const& s5,
                                        Instant const& s6, Instant const& s7) {
      try {
        InstantArray sa = InstantArray(8);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        new(sa.value + 2) Instant(s2);
        new(sa.value + 3) Instant(s3);
        new(sa.value + 4) Instant(s4);
        new(sa.value + 5) Instant(s5);
        new(sa.value + 6) Instant(s6);
        new(sa.value + 7) Instant(s7);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1, Instant const& s2,
                                        Instant const& s3, Instant const& s4, Instant const& s5,
                                        Instant const& s6, Instant const& s7, Instant const& s8) {
      try {
        InstantArray sa = InstantArray(9);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        new(sa.value + 2) Instant(s2);
        new(sa.value + 3) Instant(s3);
        new(sa.value + 4) Instant(s4);
        new(sa.value + 5) Instant(s5);
        new(sa.value + 6) Instant(s6);
        new(sa.value + 7) Instant(s7);
        new(sa.value + 8) Instant(s8);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    InstantArray InstantArray::of(Instant const& s0, Instant const& s1, Instant const& s2,
                                        Instant const& s3,
                                        Instant const& s4, Instant const& s5, Instant const& s6,
                                        Instant const& s7, Instant const& s8, Instant const& s9) {
      try {
        InstantArray sa = InstantArray(10);
        new(sa.value + 0) Instant(s0);
        new(sa.value + 1) Instant(s1);
        new(sa.value + 2) Instant(s2);
        new(sa.value + 3) Instant(s3);
        new(sa.value + 4) Instant(s4);
        new(sa.value + 5) Instant(s5);
        new(sa.value + 6) Instant(s6);
        new(sa.value + 7) Instant(s7);
        new(sa.value + 8) Instant(s8);
        new(sa.value + 9) Instant(s9);
        return sa;
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Instant& InstantArray::operator[](gint index) { return get(index); }

    Instant const& InstantArray::operator[](gint index) const { return get(index); }

    InstantArray::LinearIterator::LinearIterator(InstantArray& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    InstantArray::LinearIterator& InstantArray::LinearIterator::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    Instant& InstantArray::LinearIterator::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool InstantArray::LinearIterator::operator==(LinearIterator const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool InstantArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    InstantArray::LinearIterator2::LinearIterator2(InstantArray const& array, gbool isEnd)
      : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    InstantArray::LinearIterator2& InstantArray::LinearIterator2::operator++() {
      isEnd = isEnd || ++next >= array.count;

      return *this;
    }

    Instant const& InstantArray::LinearIterator2::operator*() const {
      if (!isEnd) return array[next];

      util::NoSuchElementException().throws($ftrace());
    }

    gbool InstantArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
      return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool InstantArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    InstantArray::LinearIterator InstantArray::begin() { return LinearIterator(*this, false); }

    InstantArray::LinearIterator2 InstantArray::begin() const { return LinearIterator2(*this, false); }

    InstantArray::LinearIterator InstantArray::end() { return LinearIterator(*this, true); }

    InstantArray::LinearIterator2 InstantArray::end() const { return LinearIterator2(*this, true); }

    gbool InstantArray::equals(const Object& obj) const {
      if (this == &obj) {
        return true;
      }

      if (!Class<InstantArray>::hasInstance(obj)) {
        return false;
      }

      InstantArray const& array = CORE_XCAST(InstantArray const, obj);

      if (count != array.count) return false;

      for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

      return true;
    }

    gint InstantArray::hash() const {
      gint const count = length();
      gint hash = 0;
      for (int i = 0; i < count; ++i) {
        hash = hash * 31 ^ (count - i - 1) + value[i].hash();
      }
      return hash;
    }

    String InstantArray::toString() const {
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

    Object& InstantArray::clone() const {
      try {
        return UNSAFE::newInstance<InstantArray>(*this);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void InstantArray::initializeFirstTeens(Instant const& s0, Instant const& s1, Instant const& s2,
                                               Instant const& s3, Instant const& s4, Instant const& s5,
                                               Instant const& s6, Instant const& s7, Instant const& s8,
                                               Instant const& s9) {
      new(value + 0) Instant(s0);
      new(value + 1) Instant(s1);
      new(value + 2) Instant(s2);
      new(value + 3) Instant(s3);
      new(value + 4) Instant(s4);
      new(value + 5) Instant(s5);
      new(value + 6) Instant(s6);
      new(value + 7) Instant(s7);
      new(value + 8) Instant(s8);
      new(value + 9) Instant(s9);
    }

    void InstantArray::writeAsInstant(gint idx, Instant const& s) {
      new(value + idx) Instant(s);
    }

    void InstantArray::writeInstants(gint idx) {
      CORE_ASSERT(idx == count && idx > 10);
    }
  } // time
} // core
