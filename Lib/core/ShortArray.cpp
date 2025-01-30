//
// Created by bruns on 10/05/2024.
//

#include <core/Array.h>
#include <core/ShortArray.h>
#include <core/OutOfMemoryError.h>
#include <core/XString.h>
#include <core/function/IntSupplier.h>
#include <core/function/IntUnaryOperator.h>

namespace core {
    ShortArray::ShortArray() CORE_NOTHROW: ShortArray(0) {
    }

    ShortArray::ShortArray(gint length) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 2LL));
            count = length;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray::ShortArray(gint length, gshort initialValue) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 2LL));
            count = length;

            if (initialValue != 0)
                for (gint i = 0; i < length; i++) value[i] = initialValue;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray::ShortArray(ShortArray const &array) : PrimitiveArray(array) {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * 2LL));
        count = array.count;

        for (int i = 0; i < count; ++i) value[i] = array.value[i];
    }

    ShortArray::ShortArray(ShortArray &&array) noexcept {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    ShortArray::ShortArray(VarArgs const& args): ShortArray(args.size()) {
        for (int i = 0; i < args.size(); ++i) {
            value[i] = args.begin()[i];
        }
    }

    gint ShortArray::length() const { return count; }

    gbool ShortArray::isEmpty() const { return count == 0; }

    gshort &ShortArray::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gshort const &ShortArray::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gshort ShortArray::set(gint index, gshort newValue) {
        try {
            misc::Preconditions::checkIndex(index, length());

            const gshort oldValue = value[index];
            value[index] = newValue;
            return oldValue;
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    ShortArray::~ShortArray() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0)
            UNSAFE::freeMemory(CORE_CAST(glong, a));
    }

    gshort const &ShortArray::operator[](gint index) const {
        try {
            return get(index);
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gshort &ShortArray::operator[](gint index) {
        try {
            return get(index);
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    ShortArray &ShortArray::operator=(ShortArray const &array) {
        try {
            if (this != &array) {
                if (count < array.count)
                    value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value), array.count * 2LL));

                count = array.count;
                for (int i = 0; i < count; i++) value[i] = array.value[i];
            }
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
        return *this;
    }

    ShortArray &ShortArray::operator=(ShortArray &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    ShortArray ShortArray::of() { return ShortArray(); }

    ShortArray ShortArray::copyOf(Array<Short> const &a) {
        try {
            gint len = a.length();
            ShortArray dest = ShortArray(len);
            for (int i = 0; i < len; i++) dest.value[i] = a[i].shortValue();
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray ShortArray::ofRange(gshort limit) {
        try { return ofRange(0, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray ShortArray::ofRange(gshort firstValue, gshort limit) {
        try { return ofRange(firstValue, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray ShortArray::ofRange(gshort firstValue, gshort limit, gint offsetByValue) {
        if (offsetByValue == 0)
            IllegalArgumentException("Zero offset"_S).throws($ftrace());

        gint len = Math::abs((limit - firstValue) / offsetByValue);

        try {
            ShortArray dest = ShortArray(len);
            for (int i = 0, next = firstValue; i < len; i++, next += offsetByValue)
                dest.value[i] = CORE_CAST(gshort, next);
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray ShortArray::ofSequence(gint length, function::IntUnaryOperator const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            ShortArray dest = ShortArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = CORE_CAST(gshort, generator.apply(i));
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray ShortArray::ofSequence(gint length, function::IntSupplier const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            ShortArray dest = ShortArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = CORE_CAST(gshort, generator.get());
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    ShortArray::LinearIterator::LinearIterator(ShortArray &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    ShortArray::LinearIterator &ShortArray::LinearIterator::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gshort &ShortArray::LinearIterator::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool ShortArray::LinearIterator::operator==(LinearIterator const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ShortArray::LinearIterator::operator!=(LinearIterator const &rhs) const { return !(*this == rhs); }

    ShortArray::LinearIterator2::LinearIterator2(ShortArray const &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    ShortArray::LinearIterator2 &ShortArray::LinearIterator2::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gshort const &ShortArray::LinearIterator2::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool ShortArray::LinearIterator2::operator==(LinearIterator2 const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool ShortArray::LinearIterator2::operator!=(LinearIterator2 const &rhs) const { return !(*this == rhs); }

    ShortArray::LinearIterator ShortArray::begin() { return LinearIterator(*this, false); }

    ShortArray::LinearIterator2 ShortArray::begin() const { return LinearIterator2(*this, false); }

    ShortArray::LinearIterator ShortArray::end() { return LinearIterator(*this, true); }

    ShortArray::LinearIterator2 ShortArray::end() const { return LinearIterator2(*this, true); }

    gbool ShortArray::equals(const Object &obj) const {
        if (this == &obj)
            return true;

        if (!Class<ShortArray>::hasInstance(obj))
            return false;

        ShortArray const &array = CORE_XCAST(ShortArray const, obj);

        if (count != array.count) return false;

        for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

        return true;
    }

    gint ShortArray::hash() const {
        gint h = 0;
        for (int i = 0; i < count; ++i) h = h * 31 ^ (count - i - 1) + Short::hash(value[i]);
        return h;
    }

    String ShortArray::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(Math::max(count * (1 + 2) + 2, 16));
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &ShortArray::clone() const {
        try {
            return UNSAFE::newInstance<ShortArray>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core