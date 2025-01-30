//
// Created by bruns on 10/05/2024.
//

#include <core/Array.h>
#include <core/DoubleArray.h>
#include <core/OutOfMemoryError.h>
#include <core/XString.h>
#include <core/function/DoubleSupplier.h>
#include <core/function/DoubleUnaryOperator.h>

namespace core {
    DoubleArray::DoubleArray() CORE_NOTHROW: DoubleArray(0) {
    }

    DoubleArray::DoubleArray(gint length) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 8LL));
            count = length;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray::DoubleArray(gint length, gdouble initialValue) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 8LL));
            count = length;

            if (initialValue != 0)
                for (gint i = 0; i < length; i++) value[i] = initialValue;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray::DoubleArray(DoubleArray const &array) : PrimitiveArray(array) {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * 8LL));
        count = array.count;

        for (int i = 0; i < count; ++i) value[i] = array.value[i];
    }

    DoubleArray::DoubleArray(DoubleArray &&array) noexcept {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    DoubleArray::DoubleArray(VarArgs const &args): DoubleArray(args.size()) {
        for (int i = 0; i < args.size(); ++i) {
            value[i] = args.begin()[i];
        }
    }

    gint DoubleArray::length() const { return count; }

    gbool DoubleArray::isEmpty() const { return count == 0; }

    gdouble &DoubleArray::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gdouble const &DoubleArray::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gdouble DoubleArray::set(gint index, gdouble newValue) {
        try {
            misc::Preconditions::checkIndex(index, length());

            const gdouble oldValue = value[index];
            value[index] = newValue;
            return oldValue;
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray::~DoubleArray() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0)
            UNSAFE::freeMemory(CORE_CAST(glong, a));
    }

    gdouble const &DoubleArray::operator[](gint index) const {
        try {
            return get(index);
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    gdouble &DoubleArray::operator[](gint index) {
        try {
            return get(index);
        } catch (Exception const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray &DoubleArray::operator=(DoubleArray const &array) {
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

    DoubleArray &DoubleArray::operator=(DoubleArray &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    DoubleArray DoubleArray::of() { return DoubleArray(); }

    DoubleArray DoubleArray::copyOf(Array<Double> const &a) {
        try {
            gint len = a.length();
            DoubleArray dest = DoubleArray(len);
            for (int i = 0; i < len; i++) dest.value[i] = a[i].doubleValue();
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray DoubleArray::ofRange(gdouble limit) {
        try { return ofRange(0, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray DoubleArray::ofRange(gdouble firstValue, gdouble limit) {
        try { return ofRange(firstValue, limit, 1); } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray DoubleArray::ofRange(gdouble firstValue, gdouble limit, gdouble offsetByValue) {
        if (offsetByValue == 0)
            IllegalArgumentException("Zero offset"_S).throws($ftrace());

        if (!Double::isFinite(firstValue) || !Double::isFinite(limit))
            IllegalArgumentException("Unbounded range"_S).throws($ftrace());

        if (!Double::isFinite(offsetByValue))
            IllegalArgumentException("Non-finite step"_S).throws($ftrace());

        gdouble d = Math::abs((limit - firstValue) / offsetByValue);
        if (d > Integer::MAX_VALUE)
            OutOfMemoryError("Required length is too large").throws($ftrace());

        gint len = CORE_CAST(gint, d);

        try {
            DoubleArray dest = DoubleArray(len);
            gdouble next = firstValue;
            for (int i = 0; i < len; i++) {
                dest.value[i] = next;
                next += offsetByValue;
            }
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray DoubleArray::ofSequence(gint length, function::DoubleUnaryOperator const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            DoubleArray dest = DoubleArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = generator.apply(i);
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray DoubleArray::ofSequence(gint length, function::DoubleSupplier const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            DoubleArray dest = DoubleArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = generator.get();
            return dest;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    DoubleArray::LinearIterator::LinearIterator(DoubleArray &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    DoubleArray::LinearIterator &DoubleArray::LinearIterator::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gdouble &DoubleArray::LinearIterator::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool DoubleArray::LinearIterator::operator==(LinearIterator const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool DoubleArray::LinearIterator::operator!=(LinearIterator const &rhs) const { return !(*this == rhs); }

    DoubleArray::LinearIterator2::LinearIterator2(DoubleArray const &array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {
    }

    DoubleArray::LinearIterator2 &DoubleArray::LinearIterator2::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gdouble const &DoubleArray::LinearIterator2::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool DoubleArray::LinearIterator2::operator==(LinearIterator2 const &rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool DoubleArray::LinearIterator2::operator!=(LinearIterator2 const &rhs) const { return !(*this == rhs); }

    DoubleArray::LinearIterator DoubleArray::begin() { return LinearIterator(*this, false); }

    DoubleArray::LinearIterator2 DoubleArray::begin() const { return LinearIterator2(*this, false); }

    DoubleArray::LinearIterator DoubleArray::end() { return LinearIterator(*this, true); }

    DoubleArray::LinearIterator2 DoubleArray::end() const { return LinearIterator2(*this, true); }

    gbool DoubleArray::equals(const Object &obj) const {
        if (this == &obj)
            return true;

        if (!Class<DoubleArray>::hasInstance(obj))
            return false;

        DoubleArray const &array = CORE_XCAST(DoubleArray const, obj);

        if (count != array.count) return false;

        for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

        return true;
    }

    gint DoubleArray::hash() const {
        gint h = 0;
        for (int i = 0; i < count; ++i) h = h * 31 ^ (count - i - 1) + Double::hash(value[i]);
        return h;
    }

    String DoubleArray::toString() const {
        if (count == 0)
            return "[]"_S;

        XString str = XString(Math::max(count * (1 + 2) + 2, 16));
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object &DoubleArray::clone() const {
        try {
            return UNSAFE::newInstance<DoubleArray>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
