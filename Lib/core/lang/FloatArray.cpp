//
// Created by bruns on 10/05/2024.
//

#include <core/lang/Array.h>
#include "FloatArray.h"
#include <core/lang/OutOfMemoryError.h>
#include <core/lang/XString.h>
#include <core/function/DoubleSupplier.h>
#include <core/function/DoubleUnaryOperator.h>

namespace core {
    FloatArray::FloatArray() CORE_NOTHROW: FloatArray(0) {}

    FloatArray::FloatArray(gint length) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 4LL));
            count = length;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    FloatArray::FloatArray(gint length, gfloat initialValue) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 4LL));
            count = length;

            if (initialValue != 0)
                for (gint i = 0; i < length; i++) value[i] = initialValue;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    FloatArray::FloatArray(FloatArray const& array) : PrimitiveArray(array) {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * 4LL));
        count = array.count;

        for (int i = 0; i < count; ++i) value[i] = array.value[i];
    }

    FloatArray::FloatArray(FloatArray&& array) noexcept {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    FloatArray::FloatArray(VarArgs const& args): FloatArray(args.size()) {
        for (int i = 0; i < args.size(); ++i) {
            value[i] = args.begin()[i];
        }
    }

    gint FloatArray::length() const { return count; }

    gbool FloatArray::isEmpty() const { return count == 0; }

    gfloat& FloatArray::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    gfloat const& FloatArray::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    gfloat FloatArray::set(gint index, gfloat newValue) {
        try {
            misc::Preconditions::checkIndex(index, length());

            const gfloat oldValue = value[index];
            value[index] = newValue;
            return oldValue;
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    FloatArray::~FloatArray() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0)
            UNSAFE::freeMemory(CORE_CAST(glong, a));
    }

    gfloat const& FloatArray::operator[](gint index) const {
        try {
            return get(index);
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    gfloat& FloatArray::operator[](gint index) {
        try {
            return get(index);
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    FloatArray& FloatArray::operator=(FloatArray const& array) {
        try {
            if (this != &array) {
                if (count < array.count)
                    value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value), array.count * 4LL));

                count = array.count;
                for (int i = 0; i < count; i++) value[i] = array.value[i];
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return *this;
    }

    FloatArray& FloatArray::operator=(FloatArray&& array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    FloatArray FloatArray::of() { return FloatArray(); }

    FloatArray FloatArray::copyOf(Array<Float> const& a) {
        try {
            gint len = a.length();
            FloatArray dest = FloatArray(len);
            for (int i = 0; i < len; i++) dest.value[i] = a[i].floatValue();
            return dest;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    FloatArray FloatArray::ofRange(gfloat limit) {
        try { return ofRange(0, limit, 1); } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    FloatArray FloatArray::ofRange(gfloat firstValue, gfloat limit) {
        try { return ofRange(firstValue, limit, 1); } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    FloatArray FloatArray::ofRange(gfloat firstValue, gfloat limit, gdouble offsetByValue) {
        if (offsetByValue == 0)
            IllegalArgumentException("Zero offset"_S).throws($ftrace());

        if (!Float::isFinite(firstValue) || !Float::isFinite(limit))
            IllegalArgumentException("Unbounded range"_S).throws($ftrace());

        if (!Double::isFinite(offsetByValue))
            IllegalArgumentException("Non-finite step"_S).throws($ftrace());

        gdouble d = Math::abs((gdouble) (limit - firstValue) / offsetByValue);
        if (d > Integer::MAX_VALUE)
            OutOfMemoryError("Required length is too large").throws($ftrace());

        gint len = CORE_CAST(gint, d);

        try {
            FloatArray dest = FloatArray(len);
            gdouble next = firstValue;
            for (int i = 0; i < len; i++) {
                dest.value[i] = CORE_CAST(gfloat, next);
                next += offsetByValue;
            }
            return dest;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    FloatArray FloatArray::ofSequence(gint length, function::DoubleUnaryOperator const& generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            FloatArray dest = FloatArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = CORE_CAST(gfloat, generator.apply(i));
            return dest;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    FloatArray FloatArray::ofSequence(gint length, function::DoubleSupplier const& generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            FloatArray dest = FloatArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = CORE_CAST(gfloat, generator.get());
            return dest;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    FloatArray::LinearIterator::LinearIterator(FloatArray& array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    FloatArray::LinearIterator& FloatArray::LinearIterator::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gfloat& FloatArray::LinearIterator::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool FloatArray::LinearIterator::operator==(LinearIterator const& rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool FloatArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    FloatArray::LinearIterator2::LinearIterator2(FloatArray const& array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    FloatArray::LinearIterator2& FloatArray::LinearIterator2::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gfloat const& FloatArray::LinearIterator2::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool FloatArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool FloatArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    FloatArray::LinearIterator FloatArray::begin() { return LinearIterator(*this, false); }

    FloatArray::LinearIterator2 FloatArray::begin() const { return LinearIterator2(*this, false); }

    FloatArray::LinearIterator FloatArray::end() { return LinearIterator(*this, true); }

    FloatArray::LinearIterator2 FloatArray::end() const { return LinearIterator2(*this, true); }

    gbool FloatArray::equals(const Object& obj) const {
        if (this == &obj)
            return true;

        if (!Class<FloatArray>::hasInstance(obj))
            return false;

        FloatArray const& array = CORE_XCAST(FloatArray const, obj);

        if (count != array.count) return false;

        for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

        return true;
    }

    gint FloatArray::hash() const {
        gint h = 0;
        for (int i = 0; i < count; ++i) h = h * 31 ^ (count - i - 1) + Float::hash(value[i]);
        return h;
    }

    String FloatArray::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(Math::max(count * (1 + 2) + 2, 16));
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object& FloatArray::clone() const {
        try {
            return UNSAFE::newInstance<FloatArray>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
} // core
