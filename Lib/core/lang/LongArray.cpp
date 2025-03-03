//
// Created by bruns on 10/05/2024.
//

#include <core/lang/Array.h>
#include "LongArray.h"
#include <core/lang/OutOfMemoryError.h>
#include <core/lang/XString.h>
#include <core/function/LongSupplier.h>
#include <core/function/LongUnaryOperator.h>

namespace core {
    LongArray::LongArray() CORE_NOTHROW: LongArray(0) {
    }

    LongArray::LongArray(gint length) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 8LL));
            count = length;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray::LongArray(gint length, glong initialValue) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 8LL));
            count = length;

            if (initialValue != 0)
                for (gint i = 0; i < length; i++) value[i] = initialValue;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray::LongArray(LongArray const &array) : PrimitiveArray(array) {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * 8LL));
        count = array.count;

        for (int i = 0; i < count; ++i) value[i] = array.value[i];
    }

    LongArray::LongArray(LongArray &&array) noexcept {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    LongArray::LongArray(VarArgs const &args): LongArray(args.size()) {
        for (int i = 0; i < args.size(); ++i) {
            value[i] = args.begin()[i];
        }
    }

    gint LongArray::length() const { return count; }

    gbool LongArray::isEmpty() const { return count == 0; }

    glong &LongArray::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    glong const &LongArray::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    glong LongArray::set(gint index, glong newValue) {
        try {
            misc::Preconditions::checkIndex(index, length());

            const glong oldValue = value[index];
            value[index] = newValue;
            return oldValue;
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    LongArray::~LongArray() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0)
            UNSAFE::freeMemory(CORE_CAST(glong, a));
    }

    glong const &LongArray::operator[](gint index) const {
        try {
            return get(index);
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    glong &LongArray::operator[](gint index) {
        try {
            return get(index);
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    LongArray &LongArray::operator=(LongArray const &array) {
        try {
            if (this != &array) {
                if (count < array.count)
                    value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value), array.count * 8LL));

                count = array.count;
                for (int i = 0; i < count; i++) value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
        return *this;
    }

    LongArray &LongArray::operator=(LongArray &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    LongArray LongArray::of() { return LongArray(); }

    LongArray LongArray::copyOf(Array<Long> const &a) {
        try {
            gint len = a.length();
            LongArray dest = LongArray(len);
            for (int i = 0; i < len; i++) dest.value[i] = a[i].intValue();
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray LongArray::ofRange(glong limit) {
        try { return ofRange(0, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray LongArray::ofRange(glong firstValue, glong limit) {
        try { return ofRange(firstValue, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray LongArray::ofRange(glong firstValue, glong limit, glong offsetByValue) {
        if (offsetByValue == 0)
            IllegalArgumentException("Zero offset"_S).throws($ftrace());

        glong len = Math::abs((limit - firstValue) / offsetByValue);
        if (len > Integer::MAX_VALUE)
            OutOfMemoryError("Required length is too large").throws($ftrace());

        try {
            LongArray dest = LongArray(CORE_CAST(gint, len));
            for (glong i = 0, next = firstValue; i < len; i++, next += offsetByValue)
                dest.value[i] = next;
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray LongArray::ofSequence(gint length, function::LongUnaryOperator const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            LongArray dest = LongArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = generator.apply(i);
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray LongArray::ofSequence(gint length, function::LongSupplier const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            LongArray dest = LongArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = generator.get();
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    LongArray::LinearIterator::LinearIterator(LongArray &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    LongArray::LinearIterator &LongArray::LinearIterator::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    glong &LongArray::LinearIterator::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool LongArray::LinearIterator::operator==(LinearIterator const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool LongArray::LinearIterator::operator!=(LinearIterator const &rhs) const { return !(*this == rhs); }

    LongArray::LinearIterator2::LinearIterator2(LongArray const &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    LongArray::LinearIterator2 &LongArray::LinearIterator2::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    glong const &LongArray::LinearIterator2::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool LongArray::LinearIterator2::operator==(LinearIterator2 const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool LongArray::LinearIterator2::operator!=(LinearIterator2 const &rhs) const { return !(*this == rhs); }

    LongArray::LinearIterator LongArray::begin() { return LinearIterator(*this, false); }

    LongArray::LinearIterator2 LongArray::begin() const { return LinearIterator2(*this, false); }

    LongArray::LinearIterator LongArray::end() { return LinearIterator(*this, true); }

    LongArray::LinearIterator2 LongArray::end() const { return LinearIterator2(*this, true); }

    gbool LongArray::equals(const Object &obj) const {
        if (this == &obj)
            return true;

        if (!Class<LongArray>::hasInstance(obj))
            return false;

        LongArray const &array = CORE_XCAST(LongArray const, obj);

        if (count != array.count) return false;

        for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

        return true;
    }

    gint LongArray::hash() const {
        gint h = 0;
        for (int i = 0; i < count; ++i) h = h * 31 ^ (count - i - 1) + Long::hash(value[i]);
        return h;
    }

    String LongArray::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(Math::max(count * (1 + 2) + 2, 16));
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &LongArray::clone() const {
        try {
            return UNSAFE::newInstance<LongArray>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
