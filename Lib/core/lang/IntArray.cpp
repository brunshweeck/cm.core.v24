//
// Created by bruns on 10/05/2024.
//

#include <core/lang/Array.h>
#include "IntArray.h"
#include <core/lang/OutOfMemoryError.h>
#include <core/lang/XString.h>
#include <core/function/IntSupplier.h>
#include <core/function/IntUnaryOperator.h>

namespace core {
    IntArray::IntArray() CORE_NOTHROW: IntArray(0) {
    }

    IntArray::IntArray(gint length) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 4LL));
            count = length;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray::IntArray(gint length, gint initialValue): PrimitiveArray() {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 4LL));
            count = length;

            if (initialValue != 0)
                for (gint i = 0; i < length; i++) value[i] = initialValue;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray::IntArray(IntArray const &array) : PrimitiveArray(array) {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * 4LL));
        count = array.count;

        for (int i = 0; i < count; ++i) value[i] = array.value[i];
    }

    IntArray::IntArray(IntArray &&array) noexcept {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    IntArray::IntArray(VarArgs const &args): IntArray(args.size()) {
        for (int i = 0; i < args.size(); ++i) {
            value[i] = args.begin()[i];
        }
    }

    gint IntArray::length() const { return count; }

    gbool IntArray::isEmpty() const { return count == 0; }

    gint &IntArray::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gint const &IntArray::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gint IntArray::set(gint index, gint newValue) {
        try {
            misc::Preconditions::checkIndex(index, length());

            const gint oldValue = value[index];
            value[index] = newValue;
            return oldValue;
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    IntArray::~IntArray() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0)
            UNSAFE::freeMemory(CORE_CAST(glong, a));
    }

    gint const &IntArray::operator[](gint index) const {
        try { return get(index); } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gint &IntArray::operator[](gint index) {
        try { return get(index); } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    IntArray &IntArray::operator=(IntArray const &array) {
        try {
            if (this != &array) {
                if (count < array.count)
                    value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value), array.count * 4LL));

                count = array.count;
                for (int i = 0; i < count; i++) value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
        return *this;
    }

    IntArray &IntArray::operator=(IntArray &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    IntArray IntArray::of() { return IntArray(); }

    IntArray IntArray::copyOf(Array<Integer> const &a) {
        try {
            gint len = a.length();
            IntArray dest = IntArray(len);
            for (int i = 0; i < len; i++) dest.value[i] = a[i].intValue();
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray IntArray::ofRange(gint limit) {
        try { return ofRange(0, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray IntArray::ofRange(gint firstValue, gint limit) {
        try { return ofRange(firstValue, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray IntArray::ofRange(gint firstValue, gint limit, gint offsetByValue) {
        if (offsetByValue == 0)
            IllegalArgumentException("Zero offset"_S).throws($ftrace());

        gint len = Math::abs((limit - firstValue) / offsetByValue);

        try {
            IntArray dest = IntArray(len);
            for (int i = 0, next = firstValue; i < len; i++, next += offsetByValue)
                dest.value[i] = next;
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray IntArray::ofSequence(gint length, function::IntUnaryOperator const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            IntArray dest = IntArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = generator.apply(i);
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray IntArray::ofSequence(gint length, function::IntSupplier const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            IntArray dest = IntArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = generator.get();
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    IntArray::LinearIterator::LinearIterator(IntArray &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    IntArray::LinearIterator &IntArray::LinearIterator::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gint &IntArray::LinearIterator::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool IntArray::LinearIterator::operator==(LinearIterator const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool IntArray::LinearIterator::operator!=(LinearIterator const &rhs) const { return !(*this == rhs); }

    IntArray::LinearIterator2::LinearIterator2(IntArray const &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    IntArray::LinearIterator2 &IntArray::LinearIterator2::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gint const &IntArray::LinearIterator2::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool IntArray::LinearIterator2::operator==(LinearIterator2 const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool IntArray::LinearIterator2::operator!=(LinearIterator2 const &rhs) const { return !(*this == rhs); }

    IntArray::LinearIterator IntArray::begin() { return LinearIterator(*this, false); }

    IntArray::LinearIterator2 IntArray::begin() const { return LinearIterator2(*this, false); }

    IntArray::LinearIterator IntArray::end() { return LinearIterator(*this, true); }

    IntArray::LinearIterator2 IntArray::end() const { return LinearIterator2(*this, true); }

    gbool IntArray::equals(const Object &obj) const {
        if (this == &obj)
            return true;

        if (!Class<IntArray>::hasInstance(obj))
            return false;

        IntArray const &array = CORE_XCAST(IntArray const, obj);

        if (count != array.count) return false;

        for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

        return true;
    }

    gint IntArray::hash() const {
        gint h = 0;
        for (int i = 0; i < count; ++i) h = h * 31 ^ (count - i - 1) + Integer::hash(value[i]);
        return h;
    }

    String IntArray::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(Math::max(count * (1 + 2) + 2, 16));
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &IntArray::clone() const {
        try {
            return UNSAFE::newInstance<IntArray>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
