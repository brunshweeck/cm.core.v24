//
// Created by brunshweeck on 3 sept. 2024.
//

#include <core/lang/Character.h>
#include <core/lang/IllegalArgumentException.h>
#include "StringArray.h"
#include <core/lang/XString.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/Arrays.h>
#include <core/util/NoSuchElementException.h>

namespace core {
  StringArray::StringArray() CORE_NOTHROW: StringArray(0) {}

  StringArray::StringArray(gint length): ValueArray() {
    if (length < 0)
      IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

    try {
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(String)));
      new(value) String[length];
      count = length;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray::StringArray(gint length, String initialValue): ValueArray() {
    if (length < 0)
      IllegalArgumentException("Negative array size"_S).throws($ftrace(core));

    try {
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(String)));
      value = new(value) String[length];
      count = length;
      if (initialValue.length() > 0) {
        value[0] = UNSAFE::moveInstance(initialValue);
        for (int i = 1; i < length; ++i) value[i] = value[0];
      }
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray::StringArray(StringArray const& array): ValueArray(array) {
    try {
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(count * sizeof(String)));
      count = array.count;

      for (int i = 0; i < count; ++i) value[i] = array.value[i];
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray::StringArray(StringArray&& array) CORE_NOTHROW {
    UNSAFE::swapValues(value, array.value);
    UNSAFE::swapValues(count, array.count);
  }

  StringArray::StringArray(VarArgs const& args): StringArray() {
    try {
      gint length = args.size();
      value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * sizeof(String)));
      count = length;
      for (int i = 0; i < length; ++i)
        new(value + i) String(UNSAFE::moveInstance(args.begin()[i]));
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray& StringArray::operator=(const StringArray& other) {
    if (this != &other) {
      try {
        if (count < other.count) {
          value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value),
                              other.count * sizeof(String)));
          new(value + count) String[other.count - count];
        }
        count = other.count;

        for (int i = 0; i < other.count; ++i) value[i] = other.value[i];
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
    return *this;
  }

  StringArray& StringArray::operator=(StringArray&& other) CORE_NOTHROW {
    if (this != &other) {
      UNSAFE::swapValues(value, other.value);
      UNSAFE::swapValues(count, other.count);
    }
    return *this;
  }

  gint StringArray::length() const { return count; }

  gbool StringArray::isEmpty() const { return (count == 0); }

  String& StringArray::get(gint index) {
    try {
      misc::Preconditions::checkIndex(index, count);
      return value[index];
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  String const& StringArray::get(gint index) const {
    try {
      misc::Preconditions::checkIndex(index, count);
      return value[index];
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  String StringArray::set(gint index, String newValue) {
    try {
      misc::Preconditions::checkIndex(index, count);
      value[index] = UNSAFE::moveInstance(newValue);
      return UNSAFE::moveInstance(newValue);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray::~StringArray() {
    const ARRAY a = value;
    const gint n = count;

    value = null;
    count = 0;

    if (n > 0) {
      for (int i = n - 1; i >= 0; --i)
        a[i].~String();

      UNSAFE::freeMemory(CORE_CAST(glong, a));
    }
  }

  StringArray StringArray::copyOf(Array<String> const& a) {
    gint n = a.length();
    StringArray sa = StringArray(n);
    for (gint i = 0; i < n; i++) sa[n] = a[i];
    return sa;
  }

  StringArray StringArray::of() { return { }; }

  StringArray StringArray::of(String s0) { return StringArray(1, UNSAFE::moveInstance(s0)); }

  StringArray StringArray::of(String s0, String s1) {
    try {
      StringArray sa = StringArray(2);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray StringArray::of(String s0, String s1, String s2) {
    try {
      StringArray sa = StringArray(3);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      sa.value[2] = UNSAFE::moveInstance(s2);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray StringArray::of(String s0, String s1, String s2, String s3) {
    try {
      StringArray sa = StringArray(4);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      sa.value[2] = UNSAFE::moveInstance(s2);
      sa.value[3] = UNSAFE::moveInstance(s3);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray StringArray::of(String s0, String s1, String s2, String s3, String s4) {
    try {
      StringArray sa = StringArray(5);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      sa.value[2] = UNSAFE::moveInstance(s2);
      sa.value[3] = UNSAFE::moveInstance(s3);
      sa.value[4] = UNSAFE::moveInstance(s4);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray StringArray::of(String s0, String s1, String s2, String s3, String s4,
                              String s5) {
    try {
      StringArray sa = StringArray(6);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      sa.value[2] = UNSAFE::moveInstance(s2);
      sa.value[3] = UNSAFE::moveInstance(s3);
      sa.value[4] = UNSAFE::moveInstance(s4);
      sa.value[5] = UNSAFE::moveInstance(s5);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray StringArray::of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6) {
    try {
      StringArray sa = StringArray(7);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      sa.value[2] = UNSAFE::moveInstance(s2);
      sa.value[3] = UNSAFE::moveInstance(s3);
      sa.value[4] = UNSAFE::moveInstance(s4);
      sa.value[5] = UNSAFE::moveInstance(s5);
      sa.value[6] = UNSAFE::moveInstance(s6);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray StringArray::of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6, String s7) {
    try {
      StringArray sa = StringArray(8);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      sa.value[2] = UNSAFE::moveInstance(s2);
      sa.value[3] = UNSAFE::moveInstance(s3);
      sa.value[4] = UNSAFE::moveInstance(s4);
      sa.value[5] = UNSAFE::moveInstance(s5);
      sa.value[6] = UNSAFE::moveInstance(s6);
      sa.value[7] = UNSAFE::moveInstance(s7);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray StringArray::of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6, String s7, String s8) {
    try {
      StringArray sa = StringArray(9);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      sa.value[2] = UNSAFE::moveInstance(s2);
      sa.value[3] = UNSAFE::moveInstance(s3);
      sa.value[4] = UNSAFE::moveInstance(s4);
      sa.value[5] = UNSAFE::moveInstance(s5);
      sa.value[6] = UNSAFE::moveInstance(s6);
      sa.value[7] = UNSAFE::moveInstance(s7);
      sa.value[8] = UNSAFE::moveInstance(s8);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  StringArray StringArray::of(String s0, String s1, String s2, String s3, String s4,
                              String s5, String s6, String s7, String s8, String s9) {
    try {
      StringArray sa = StringArray(10);
      sa.value[0] = UNSAFE::moveInstance(s0);
      sa.value[1] = UNSAFE::moveInstance(s1);
      sa.value[2] = UNSAFE::moveInstance(s2);
      sa.value[3] = UNSAFE::moveInstance(s3);
      sa.value[4] = UNSAFE::moveInstance(s4);
      sa.value[5] = UNSAFE::moveInstance(s5);
      sa.value[6] = UNSAFE::moveInstance(s6);
      sa.value[7] = UNSAFE::moveInstance(s7);
      sa.value[8] = UNSAFE::moveInstance(s8);
      sa.value[9] = UNSAFE::moveInstance(s9);
      return sa;
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  String& StringArray::operator[](gint index) { return get(index); }

  String const& StringArray::operator[](gint index) const { return get(index); }

  StringArray::LinearIterator::LinearIterator(StringArray& array, gbool isEnd)
    : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

  StringArray::LinearIterator& StringArray::LinearIterator::operator++() {
    isEnd = isEnd || ++next >= array.count;

    return *this;
  }

  String& StringArray::LinearIterator::operator*() const {
    if (!isEnd) return array[next];

    util::NoSuchElementException().throws($ftrace());
  }

  gbool StringArray::LinearIterator::operator==(LinearIterator const& rhs) const {
    return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
  }

  gbool StringArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

  StringArray::LinearIterator2::LinearIterator2(StringArray const& array, gbool isEnd)
    : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

  StringArray::LinearIterator2& StringArray::LinearIterator2::operator++() {
    isEnd = isEnd || ++next >= array.count;

    return *this;
  }

  String const& StringArray::LinearIterator2::operator*() const {
    if (!isEnd) return array[next];

    util::NoSuchElementException().throws($ftrace());
  }

  gbool StringArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
    return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
  }

  gbool StringArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

  StringArray::LinearIterator StringArray::begin() { return LinearIterator(*this, false); }

  StringArray::LinearIterator2 StringArray::begin() const { return LinearIterator2(*this, false); }

  StringArray::LinearIterator StringArray::end() { return LinearIterator(*this, true); }

  StringArray::LinearIterator2 StringArray::end() const { return LinearIterator2(*this, true); }

  gbool StringArray::equals(const Object& obj) const {
    if (this == &obj) {
      return true;
    }

    if (!Class<StringArray>::hasInstance(obj)) {
      return false;
    }

    StringArray const& array = CORE_XCAST(StringArray const, obj);

    if (count != array.count) return false;

    for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

    return true;
  }

  gint StringArray::hash() const {
    gint const count = length();
    gint hash = 0;
    for (int i = 0; i < count; ++i) {
      hash = hash * 31 ^ (count - i - 1) + value[i].hash();
    }
    return hash;
  }

  String StringArray::toString() const {
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

  Object& StringArray::clone() const {
    try {
      return UNSAFE::newInstance<StringArray>(*this);
    } catch (Throwable const& ex) { ex.throws($ftrace()); }
  }

  void StringArray::initializeFirstTeens(String& s0, String& s1, String& s2, String& s3, String& s4,
                                         String& s5, String& s6, String& s7, String& s8, String& s9) {
    value[0] = UNSAFE::moveInstance(s0);
    value[1] = UNSAFE::moveInstance(s1);
    value[2] = UNSAFE::moveInstance(s2);
    value[3] = UNSAFE::moveInstance(s3);
    value[4] = UNSAFE::moveInstance(s4);
    value[5] = UNSAFE::moveInstance(s5);
    value[6] = UNSAFE::moveInstance(s6);
    value[7] = UNSAFE::moveInstance(s7);
    value[8] = UNSAFE::moveInstance(s8);
    value[9] = UNSAFE::moveInstance(s9);
  }

  void StringArray::writeAsString(gint idx, String const& s) { value[idx] = s; }

  void StringArray::writeAsString(gint idx, String& s) { value[idx] = s; }

  void StringArray::writeAsString(gint idx, String&& s) { value[idx] = UNSAFE::moveInstance(s); }

  void StringArray::writeStrings(gint idx) {
    CORE_ASSERT(idx == count && idx > 10);
  }
} // core
