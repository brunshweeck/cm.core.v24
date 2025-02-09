//
// Created by bruns on 09/05/2024.
//


#include <core/Array.h>
#include <core/CharArray.h>
#include <core/XString.h>
#include <core/function/IntSupplier.h>
#include <core/function/IntUnaryOperator.h>

namespace core {

    using namespace misc;

    CharArray::CharArray() CORE_NOTHROW: CharArray(0) {}

    CharArray::CharArray(gint length) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_Sl).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 2LL));
            count = length;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray::CharArray(gint length, gchar initialValue) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_Sl).throws($ftrace());

        try {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length * 2LL));
            count = length;

            if (initialValue != 0)
                for (gint i = 0; i < length; i++) value[i] = initialValue;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray::CharArray(CharArray const& array) : PrimitiveArray(array) {
        value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(array.count * 2LL));
        count = array.count;

        for (int i = 0; i < count; ++i) value[i] = array.value[i];
    }

    CharArray::CharArray(CharArray&& array) noexcept {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    CharArray::CharArray(VarArgs const& args): CharArray(args.size()) {
        for (int i = 0; i < args.size(); ++i)
            value[i] = args.begin()[i];
    }

    gint CharArray::length() const { return count; }

    gbool CharArray::isEmpty() const { return count == 0; }

    gchar& CharArray::get(gint index) {
        try {
            Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    gchar const& CharArray::get(gint index) const {
        try {
            Preconditions::checkIndex(index, length());

            return value[index];
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    gchar CharArray::set(gint index, gchar newValue) {
        try {
            Preconditions::checkIndex(index, length());

            const gchar oldValue = value[index];
            value[index] = newValue;
            return oldValue;
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    CharArray::~CharArray() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0)
            UNSAFE::freeMemory(CORE_CAST(glong, a));
    }

    gchar const& CharArray::operator[](gint index) const {
        try {
            return get(index);
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    gchar& CharArray::operator[](gint index) {
        try {
            return get(index);
        } catch (Exception const& ex) { ex.throws($ftrace()); }
    }

    CharArray& CharArray::operator=(CharArray const& array) {
        try {
            if (this != &array) {
                if (count < array.count)
                    value = CORE_CAST(ARRAY, UNSAFE::reallocateMemory(CORE_CAST(glong, value), array.count * 2LL));

                count = array.count;
                for (int i = 0; i < count; i++) value[i] = array.value[i];
            }
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
        return *this;
    }

    CharArray& CharArray::operator=(CharArray&& array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    CharArray CharArray::of() { return CharArray(); }

    CharArray CharArray::copyOf(Array<Character> const& a) {
        try {
            gint len = a.length();
            CharArray dest = CharArray(len);
            for (int i = 0; i < len; i++) dest.value[i] = a[i].charValue();
            return dest;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray CharArray::ofRange(gchar limit) {
        try { return ofRange(0, limit, 1); } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray CharArray::ofRange(gchar firstValue, gchar limit) {
        try { return ofRange(firstValue, limit, 1); } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray CharArray::ofRange(gchar firstValue, gchar limit, gint offsetByValue) {
        if (offsetByValue == 0)
            IllegalArgumentException("Zero offset"_Sl).throws($ftrace());

        gint len = Math::abs((limit - firstValue) / offsetByValue);

        try {
            CharArray dest = CharArray(len);
            for (int i = 0, next = firstValue; i < len; i++, next += offsetByValue)
                dest.value[i] = next;
            return dest;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray CharArray::ofSequence(gint length, function::IntUnaryOperator const& generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            CharArray dest = CharArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = generator.apply(i);
            return dest;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray CharArray::ofSequence(gint length, function::IntSupplier const& generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            CharArray dest = CharArray(length);
            for (int i = 0; i < length; i++) dest.value[i] = generator.get();
            return dest;
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    CharArray::LinearIterator::LinearIterator(CharArray& array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    CharArray::LinearIterator& CharArray::LinearIterator::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gchar& CharArray::LinearIterator::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool CharArray::LinearIterator::operator==(LinearIterator const& rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool CharArray::LinearIterator::operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }

    CharArray::LinearIterator2::LinearIterator2(CharArray const& array, gbool isEnd)
        : array(array), next(isEnd ? array.count : 0), isEnd(isEnd || array.count == 0) {}

    CharArray::LinearIterator2& CharArray::LinearIterator2::operator++() {
        isEnd = isEnd || ++next >= array.count;

        return *this;
    }

    gchar const& CharArray::LinearIterator2::operator*() const {
        if (!isEnd) return array[next];

        util::NoSuchElementException().throws($ftrace());
    }

    gbool CharArray::LinearIterator2::operator==(LinearIterator2 const& rhs) const {
        return (this == &rhs) || (&array == &rhs.array) && ((isEnd && rhs.isEnd) || (next == rhs.next));
    }

    gbool CharArray::LinearIterator2::operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }

    CharArray::LinearIterator CharArray::begin() { return LinearIterator(*this, false); }

    CharArray::LinearIterator2 CharArray::begin() const { return LinearIterator2(*this, false); }

    CharArray::LinearIterator CharArray::end() { return LinearIterator(*this, true); }

    CharArray::LinearIterator2 CharArray::end() const { return LinearIterator2(*this, true); }

    gbool CharArray::equals(const Object& obj) const {
        if (this == &obj)
            return true;

        if (!Class<CharArray>::hasInstance(obj))
            return false;

        CharArray const& array = CORE_XCAST(CharArray const, obj);

        if (count != array.count) return false;

        for (int i = 0; i < count; ++i) if (value[i] != array.value[i]) return false;

        return true;
    }

    gint CharArray::hash() const {
        gint h = 0;
        for (int i = 0; i < count; ++i) h = h * 31 ^ (count - i - 1) + Character::hash(value[i]);
        return h;
    }

    String CharArray::toString() const {
        if (count == 0)
            return "[]"_Sl;

        XString str = XString(Math::max(count * (1 + 2) + 2, 16));
        str.append(u'[');
        for (int i = 0; i < count - 1; ++i) str.append(value[i]).append(',').append(' ');

        return str.append(value[count - 1]).append(']').toString();
    }

    Object& CharArray::clone() const {
        try {
            return UNSAFE::newInstance<CharArray>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
} // core
