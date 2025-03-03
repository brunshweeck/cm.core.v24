//
// Created by bruns on 10/05/2024.
//

#include <core/lang/Array.h>
#include <core/lang/ByteArray.h>
#include <core/lang/OutOfMemoryError.h>
#include <core/lang/XString.h>
#include <core/function/IntSupplier.h>
#include <core/function/IntUnaryOperator.h>

namespace core {
    ByteArray::ByteArray() CORE_NOTHROW: ByteArray(0) {
    }

    ByteArray::ByteArray(gint length) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length));
            count = length;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray::ByteArray(gint length, gbyte initialValue) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length));
            count = length;

            if (initialValue != 0)
                for (gint i = 0; i < length; i++) value[i] = initialValue;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray::ByteArray(ByteArray const &array) : PrimitiveArray(array) {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count));
        count = array.count;

        for (int i = 0; i < count; ++i) value[i] = array.value[i];
    }

    ByteArray::ByteArray(ByteArray &&array) noexcept {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    ByteArray::ByteArray(VarArgs const &args): ByteArray(args.size()) {
        for (int i = 0; i < args.size(); ++i)
            value[i] = args.begin()[i];
    }

    gint ByteArray::length() const { return count; }

    gbool ByteArray::isEmpty() const { return count == 0; }

    gbyte &ByteArray::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gbyte const &ByteArray::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gbyte ByteArray::set(gint index, gbyte newValue) {
        try {
            misc::Preconditions::checkIndex(index, length());

            const gbyte oldValue = value[index];
            value[index] = newValue;
            return oldValue;
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    ByteArray::~ByteArray() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0)
            UNSAFE::freeMemory(CORE_CAST(glong, a));
    }

    gbyte const &ByteArray::operator[](gint index) const {
        try {
            return get(index);
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gbyte &ByteArray::operator[](gint index) {
        try {
            return get(index);
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    ByteArray &ByteArray::operator=(ByteArray const &array) {
        try {
            if (this != &array) {
                if (count < array.count)
                    value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value), array.count));

                count = array.count;
                for (int i = 0; i < count; i++) value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
        return *this;
    }

    ByteArray &ByteArray::operator=(ByteArray &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    ByteArray ByteArray::of() { return ByteArray(); }

    ByteArray ByteArray::copyOf(Array<Byte> const &a) {
        try {
            gint len = a.length();
            ByteArray dest = ByteArray(len);
            for (int i = 0; i < len; i++) dest.value[i] = a[i].byteValue();
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray ByteArray::ofRange(gbyte limit) {
        try { return ofRange(0, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray ByteArray::ofRange(gbyte firstValue, gbyte limit) {
        try { return ofRange(firstValue, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray ByteArray::ofRange(gbyte firstValue, gbyte limit, gint offsetByValue) {
        if (offsetByValue == 0)
            IllegalArgumentException("Zero offset"_S).throws($ftrace());

        gint len = Math::abs((limit - firstValue) / offsetByValue);

        try {
            ByteArray dest = ByteArray(len);
            for (int i = 0, next = firstValue; i < len; i++, next += offsetByValue)
                dest.value[i] = CORE_CAST(gbyte, next);
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray ByteArray::ofSequence(gint length, function::IntUnaryOperator const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            ByteArray dest = ByteArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = CORE_CAST(gbyte, generator.apply(i));
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray ByteArray::ofSequence(gint length, function::IntSupplier const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            ByteArray dest = ByteArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = CORE_CAST(gbyte, generator.get());
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ByteArray::LinearIterator::LinearIterator(ByteArray &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    ByteArray::LinearIterator &ByteArray::LinearIterator::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gbyte &ByteArray::LinearIterator::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool ByteArray::LinearIterator::operator==(LinearIterator const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ByteArray::LinearIterator::operator!=(LinearIterator const &rhs) const { return !(*this == rhs); }

    ByteArray::LinearIterator2::LinearIterator2(ByteArray const &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    ByteArray::LinearIterator2 &ByteArray::LinearIterator2::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gbyte const &ByteArray::LinearIterator2::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool ByteArray::LinearIterator2::operator==(LinearIterator2 const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ByteArray::LinearIterator2::operator!=(LinearIterator2 const &rhs) const { return !(*this == rhs); }

    ByteArray::LinearIterator ByteArray::begin() { return LinearIterator(*this, false); }

    ByteArray::LinearIterator2 ByteArray::begin() const { return LinearIterator2(*this, false); }

    ByteArray::LinearIterator ByteArray::end() { return LinearIterator(*this, true); }

    ByteArray::LinearIterator2 ByteArray::end() const { return LinearIterator2(*this, true); }

    gbool ByteArray::equals(const Object &obj) const {
        if (this == &obj)
            return true;

        if (!Class<ByteArray>::hasInstance(obj))
            return false;

        ByteArray const &array = CORE_XCAST(ByteArray const, obj);

        if (count != array.count) return false;

        for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

        return true;
    }

    gint ByteArray::hash() const {
        gint h = 0;
        for (int i = 0; i < count; ++i) h = h * 31 ^ (count - i - 1) + Byte::hash(value[i]);
        return h;
    }

    String ByteArray::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(Math::max(count * (1 + 2) + 2, 16));
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(CORE_CAST(gint, value[i])).append(',').append(' ');

        return str.append(CORE_CAST(gint, value[count - 1])).append(']').toString();
    }

    Object &ByteArray::clone() const {
        try {
            return UNSAFE::newInstance<ByteArray>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
