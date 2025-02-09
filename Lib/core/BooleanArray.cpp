//
// Created by bruns on 10/05/2024.
//

#include <core/BooleanArray.h>
#include <core/util/NoSuchElementException.h>
#include <core/function/BooleanSupplier.h>
#include <core/function/IntUnaryOperator.h>
#include <core/util/Arrays.h>

namespace core {
    using namespace util;
    using namespace function;

    BooleanArray::BooleanArray(): BooleanArray(0) {
    }

    BooleanArray::BooleanArray(gint length) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());
        if (length > 0) {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length));
            count = length;
        }
    }

    BooleanArray::BooleanArray(gint length, gbool initialValue) {
        if (length < 0)
            IllegalArgumentException("Negative array size"_S).throws($ftrace());

        if (length > 0) {
            value = CORE_CAST(ARRAY, UNSAFE::allocateMemory(length));
            count = length;
            if (initialValue)
                Arrays::fill(*this, initialValue);
        }
    }

    BooleanArray::BooleanArray(BooleanArray const &array): BooleanArray(array.count) {
        Arrays::copy(array, 0, *this, 0, count);
    }

    BooleanArray::BooleanArray(BooleanArray &&array) CORE_NOTHROW: BooleanArray(0) {
        UNSAFE::swapValues(value, array.value);
        UNSAFE::swapValues(count, array.count);
    }

    BooleanArray::BooleanArray(VarArgs const &args): BooleanArray(CORE_CAST(gint, args.size())) {
        for (int i = 0; i < count; ++i)
            value[i] = args.begin()[i];
    }

    gint BooleanArray::length() const { return count; }

    gbool BooleanArray::isEmpty() const { return count == 0; }

    gbool &BooleanArray::get(gint index) {
        try {
            misc::Preconditions::checkIndex(index, count);
            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gbool const &BooleanArray::get(gint index) const {
        try {
            misc::Preconditions::checkIndex(index, count);
            return value[index];
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gbool BooleanArray::set(gint index, gbool newValue) {
        try {
            misc::Preconditions::checkIndex(index, count);
            gbool const oldValue = value[index];
            value[index] = newValue;
            return oldValue;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray::~BooleanArray() {
        ARRAY a = value;
        const gint n = count;

        value = null;
        count = 0;

        if (n > 0)
            UNSAFE::freeMemory(CORE_CAST(glong, a));
    }

    BooleanArray BooleanArray::of() { return BooleanArray(); }

    BooleanArray BooleanArray::copyOf(Array<Boolean> const &a) {
        try {
            gint nb = a.length();
            BooleanArray ba = BooleanArray(nb);
            for (int i = 0; i < nb; i++) ba.value[i] = a[i].booleanValue();
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray BooleanArray::ofSequence(gint length, function::IntUnaryOperator const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            BooleanArray ba = BooleanArray(length);
            for (int i = 0; i < length; i++) ba.value[i] = generator.apply(i);
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    BooleanArray BooleanArray::ofSequence(gint length, function::BooleanSupplier const &generator) {
        if (length < 0)
            IllegalArgumentException().throws($ftrace());

        try {
            BooleanArray ba = BooleanArray(length);
            for (int i = 0; i < length; i++) ba.value[i] = generator.get();
            return ba;
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }

    gbool const &BooleanArray::operator[](gint index) const { return get(index); }

    gbool &BooleanArray::operator[](gint index) { return get(index); }

    BooleanArray &BooleanArray::operator=(BooleanArray const &array) {
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

    BooleanArray &BooleanArray::operator=(BooleanArray &&array) CORE_NOTHROW {
        if (this != &array) {
            UNSAFE::swapValues(value, array.value);
            UNSAFE::swapValues(count, array.count);
        }
        return *this;
    }

    BooleanArray::LinearIterator::LinearIterator(BooleanArray &array, gbool isEnd): array(array) {
        if (isEnd)
            next = array.length();
        if (next == array.length())
            this->isEnd = true;
    }

    BooleanArray::LinearIterator &BooleanArray::LinearIterator::operator++() {
        if (!isEnd) {
            gint const count = array.count;
            next++;
            if (next == count)
                isEnd = true;
        }
        return *this;
    }

    gbool &BooleanArray::LinearIterator::operator*() const {
        if (!isEnd)
            return array.value[next];
        NoSuchElementException().throws($ftrace());
    }

    gbool BooleanArray::LinearIterator::operator==(LinearIterator const &rhs) const {
        return this == &rhs
                   ? true
                   : &array == &rhs.array
                         ? isEnd == rhs.isEnd
                               ? next == rhs.next
                               : (isEnd ? rhs.next : next) >= array.count
                         : false;
    }

    gbool BooleanArray::LinearIterator::operator!=(LinearIterator const &rhs) const {
        return !(*this == rhs);
    }

    BooleanArray::LinearIterator2::LinearIterator2(BooleanArray const &array, gbool isEnd): array(array) {
        if (isEnd)
            next = array.count;
        if (next == array.count)
            this->isEnd = true;
    }

    BooleanArray::LinearIterator2 &BooleanArray::LinearIterator2::operator++() {
        if (!isEnd) {
            const gint count = array.count;
            next++;
            if (next == count)
                isEnd = true;
        }
        return *this;
    }

    gbool const &BooleanArray::LinearIterator2::operator*() const {
        if (!isEnd)
            return array.value[next];
        NoSuchElementException().throws($ftrace());
    }

    gbool BooleanArray::LinearIterator2::operator==(LinearIterator2 const &rhs) const {
        return this == &rhs
                   ? true
                   : &array == &rhs.array
                         ? (isEnd == rhs.isEnd)
                               ? next == rhs.next
                               : (isEnd ? rhs.next : next) >= array.count
                         : false;
    }

    gbool BooleanArray::LinearIterator2::operator!=(LinearIterator2 const &rhs) const {
        return !(*this == rhs);
    }

    BooleanArray::LinearIterator BooleanArray::begin() {
        return LinearIterator(*this, false);
    }

    BooleanArray::LinearIterator2 BooleanArray::begin() const {
        return LinearIterator2(*this, false);
    }

    BooleanArray::LinearIterator BooleanArray::end() {
        return LinearIterator(*this, true);
    }

    BooleanArray::LinearIterator2 BooleanArray::end() const {
        return LinearIterator2(*this, true);
    }

    gbool BooleanArray::equals(const Object &obj) const {
        if (this == &obj)
            return true;

        if (!Class<BooleanArray>::hasInstance(obj))
            return false;

        BooleanArray const &array = CORE_XCAST(BooleanArray const, obj);

        if (count != array.count)
            return false;

        for (int i = 0; i < count; ++i)
            if (value[i] != array.value[i])
                return false;

        return true;
    }

    gint BooleanArray::hash() const {
        gint hash = 0;
        for (int i = 0; i < count; ++i)
            hash = hash * 31 ^ ((count - i - 1) + Boolean::hash(value[i]));
        return hash;
    }

    String BooleanArray::toString() const {
        const gint count = length();

        XString str = XString(Math::max(count * (5 + 2) + 2, 16));
        str.append(u'[');
        for (int i = 0; i < count; ++i) {
            str.append(value[i]);
            if (i < count - 1)
                str.append(", "_S);
        }
        str.append(u']');
        return str.toString();
    }

    Object &BooleanArray::clone() const {
        try {
            return UNSAFE::newInstance<BooleanArray>(*this);
        } catch (Throwable const &ex) { ex.throws($ftrace()); }
    }
} // core
