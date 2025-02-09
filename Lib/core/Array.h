//
// Created by brunshweeck on 8 juin 2024.
//

#ifndef CORE24_ARRAY_H
#define CORE24_ARRAY_H

#include <core/ClassCastException.h>
#include <core/IllegalArgumentException.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>
#include <core/util/NoSuchElementException.h>

namespace core {
    CORE_WARNING_PUSH
    CORE_WARNING_DISABLE_DEPRECATED

    /**
     * @c Array class wrap the reference array.  It's equivalent to @code T[N] @endcode
     * This array style is mostly used by collections to ensure that
     * instance of collection element is same with array returned by
     * @c Collection::toArray.
     *
     * @tparam E The type of reference stored on array.
     */
    template <class E>
    class Array final : public misc::ObjectArray {
        CORE_FULL_CHECK_SLIMMED_TYPE(E);

    public:
        /**
         * Construct new empty @c Array instance.
         */
        CORE_IMPLICIT Array() = default;

        /**
         * Construct new @c Array instance able to contains
         * the given number of references.
         *
         * @note All places will be not initialized.
         *
         * @param length The number of value to be allocated on this array
         * @throws IllegalArgumentException If the given length is negative
         *
         * @note Deprecation: This constructor do not initialize newly created places with valid default constructor,
         *              and you should to set all places with Array::set(gint, Object) before the using array.
         *              Is Recommended to used Constructor with two arguments parameters.
         */
        CORE_DEPRECATED
        CORE_EXPLICIT Array(gint length) {
            if (length < 0)
                IllegalArgumentException("Negative array size: "_S + length).throws($ftrace());

            try {
                reserve(length);

                if (Class<E>::hasInstance(null)) { for (int i = 0; i < length; ++i) { set0(i, null); } }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Construct new @c Array instance able to contains
         * the given number of references.
         *
         * @note All places will be initialized with given reference.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param length The number of value to be allocated on this array
         * @param initialValue The references used to initialize all values of this array.
         */
        CORE_EXPLICIT Array(gint length, E const& initialValue) {
            if (length < 0)
                IllegalArgumentException("Negative array size: "_S + length).throws($ftrace());

            try {
                reserve(length);

                E& initializer = UNSAFE::copyInstance(initialValue);
                for (gint i = 0; i < length; i++)
                    set0(i, initializer);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Construct new instance of Array<?> and initialize
         * with arbitrary number of arguments.
         *
         * @tparam X the types of given arguments
         * @param args the arguments used to construct this array
         */
        template <class X = E>
        CORE_IMPLICIT Array(VarArgs<X, E> const& args) {
            gint len = args.size();

            try {
                reserve(len);

                for (gint i = 0; i < len; i++) {
                    E& elt = Array<E>::of(args.begin()[i])[0];
                    set0(i, elt);
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Construct new array with given another array
         *
         * @param array The original array
         */
        Array(Array const& array) {
            try {
                gint n = array.length();

                reserve(n);
                setAll0(array);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Construct new array with given another array
         *
         * @param array The original array
         */
        Array(Array&& array) CORE_NOTHROW { swapAll0(array); }

        Array& operator=(const Array& other) {
            try {
                if (this != &other) {
                    reserve(other.length());
                    setAll0(other);
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        Array& operator=(Array&& other) CORE_NOTHROW {
            swapAll0(other);
            return *this;
        }

        template <class X,
                  ClassOf(1)::OnlyIf<Class<X>::template isExtends<E>()> CaptureX = 1>
        Array(Array<X> const& a): Array() {
            try {
                reserve(a.length());
                setAll0(a);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Return the number of references on this array
         *
         * @return the number references supported by this array.
         */
        gint length() const override { return ObjectArray::length(); }

        /**
         * Test if this array has no elements.
         *
         * @return the boolean value that indicate if this array has no element.
         */
        gbool isEmpty() const override { return ObjectArray::isEmpty(); }

        /**
        * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, @c IllegalStateException is throwing.
         *
         * @param index the position of reference
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         * @throws IllegalStateException If the no reference found.
         */
        E& get(gint index) {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (!check0(index))
                util::NoSuchElementException().throws($ftrace());
            return CORE_XCAST(E, get0(index));
        }

        /**
        * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, @c IllegalStateException is throwing.
         *
         * @param index the position of reference
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         * @throws IllegalStateException If the no reference found.
         */
        E const& get(gint index) const {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (!check0(index))
                util::NoSuchElementException().throws($ftrace());
            return CORE_XCAST(E const, get0(index));
        }

        /**
         * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, the null reference is returned.
         *
         * @param index the position of reference
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         */
        Object& getOrNull(gint index) {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return get0(index);
        }

        /**
         * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, the null reference is returned.
         *
         * @param index the position of reference
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         */
        Object const& getOrNull(gint index) const {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return get0(index);
        }

        /**
         * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, the fallback reference is returned.
         *
         * @param index the position of reference
         * @param fallback reference to be returned if no reference found.
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         */
        E& getOrElse(gint index, E const& fallback) {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (!check0(index)) return UNSAFE::copyInstance(fallback);
            return CORE_XCAST(E, get0(index));
        }

        /**
        * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, the fallback reference is returned.
         *
         * @param index the position of reference
         * @param fallback reference to be returned if no reference found.
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         */
        E const& getOrElse(gint index, E const& fallback) const {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (!check0(index)) return UNSAFE::copyInstance(fallback);
            return CORE_XCAST(E const, get0(index));
        }

        Object& set(gint index, E const& newValue) {
            try {
                misc::Preconditions::checkIndex(index, length());
                E& value = UNSAFE::copyInstance(newValue);

                return set0(index, value);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Object& clone() const override {
            try { return UNSAFE::newInstance<Array>(*this); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ~Array() override { reserve(0); }

        E& operator[](gint index) { return get(index); }

        E const& operator[](gint index) const { return get(index); }

        template <class X,
                ClassOf(1)::OnlyIf<Class<X>::template isExtends<E>()> CaptureX = 1>
        CORE_EXPLICIT operator Array<X>() const {
            Array<X> a;
            gint len = length();
            try { a.reserve(len); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (gint i = 0; i < len; i++) {
                if (!check0(i)) // Value is not set
                    continue;
                Object& obj = (Object&) get0(i);
                if (!Class<X>::hasInstance(obj)) {
                    ClassCastException("Unable to convert object of type " + typeName(obj) +
                        " to element of " + typeName(a)).throws($ftrace());
                }
                a.set0(i, obj);
            }
            return a;
        }

    private:
        // Error
        template <class V,
                  gbool isAccepted = Class<E>::template accept<V>(),
                  gbool isBoolean = Class<V>::isBoolean(),
                  gbool isInteger = Class<V>::isInteger(),
                  gbool isFloating = Class<V>::isFloating(),
                  gbool isCharacter = Class<V>::isCharacter(),
                  gbool isString = Class<V>::isString()>
        class Value {
        public:
            static gbool initialize(Array&, gint&, V) {
                CORE_FAST_XASSERT(isAccepted, "Illegal argument");
                return false;
            }
        };

        // Boolean
        template <class V>
        class Value<V, true, true, false, false, false, false> {
        public:
            template <class Vx>
            static gbool initialize(Array& dest, gint& index, Vx&& value) {
                using Boolean2E = Class<Boolean>::IfElse<Class<Boolean>::isExtends<E>(), E>;
                dest.set(index++, Boolean2E::valueOf(value));
                return true;
            }
        };

        // Integer
        template <class V>
        class Value<V, true, false, true, false, false, false> {
        public:
            static gbool initialize(Array& dest, gint& index, gbyte value) {
                using Byte2E = Class<Byte>::IfElse<Class<Byte>::isExtends<E>(), E>;
                dest.set(index++, Byte2E::valueOf(value));
                return true;
            }

            static gbool initialize(Array& dest, gint& index, gshort value) {
                using Short2E = Class<Short>::IfElse<Class<Short>::isExtends<E>(), E>;
                dest.set(index++, Short2E::valueOf(value));
                return true;
            }

            static gbool initialize(Array& dest, gint& index, gint value) {
                using Integer2E = Class<Integer>::IfElse<Class<Integer>::isExtends<E>(), E>;
                dest.set(index++, Integer2E::valueOf(value));
                return true;
            }

            static gbool initialize(Array& dest, gint& index, glong value) {
                using Long2E = Class<Long>::IfElse<Class<Long>::isExtends<E>(), E>;
                dest.set(index++, Long2E::valueOf(value));
                return true;
            }

            template <class X>
            static gbool initialize(Array& dest, gint& index, X&& value) {
                if (Class<V>::size() == 1)
                    return initialize(dest, index, CORE_CAST(gbyte, value));
                if (Class<V>::size() == 2)
                    return initialize(dest, index, CORE_CAST(gshort, value));
                if (Class<V>::size() == 4)
                    return initialize(dest, index, CORE_CAST(gint, value));
                return initialize(dest, index, CORE_CAST(glong, value));
            }
        };

        // Floating
        template <class V>
        class Value<V, true, false, false, true, false, false> {
        public:
            static gbool initialize(Array& dest, gint& index, gfloat value) {
                using Float2E = Class<Float>::IfElse<Class<Float>::isExtends<E>(), E>;
                dest.set(index++, Float2E::valueOf(value));
                return true;
            }

            static gbool initialize(Array& dest, gint& index, gdouble value) {
                using Double2E = Class<Double>::IfElse<Class<Double>::isExtends<E>(), E>;
                dest.set(index++, Double2E::valueOf(value));
                return true;
            }

            template <class X>
            static gbool initialize(Array& dest, gint& index, X&& value) {
                if (Class<V>::size() <= 4)
                    return initialize(dest, index, CORE_CAST(gfloat, value));
                return initialize(dest, index, CORE_CAST(gdouble, value));
            }
        };

        // Characters
        template <class V>
        class Value<V, true, false, false, false, true, false> {
        public:
            // character of 1 byte (widening cas)
            static gbool initialize(Array& dest, gint& index, gbyte value) {
                using Character2E = Class<Character>::IfElse<Class<Character>::isExtends<E>(), E>;
                dest.set(index++, Character2E::valueOf(value & 0xff));
                return true;
            }

            // character of 2 bytes
            static gbool initialize(Array& dest, gint& index, gchar value) {
                using Character2E = Class<Character>::IfElse<Class<Character>::isExtends<E>(), E>;
                dest.set(index++, Character2E::valueOf(value));
                return true;
            }

            // character of 4 bytes (narrowing cast)
            static gbool initialize(Array& dest, gint& index, gint value) {
                using Character2E = Class<Character>::IfElse<Class<Character>::isExtends<E>(), E>;
                dest.set(index++, Character2E::valueOf(value & 0xffff));
                return true;
            }

            template <class X>
            static gbool initialize(Array& dest, gint& index, X&& value) {
                if (Class<V>::size() == 1)
                    return initialize(dest, index, CORE_CAST(gbyte, value));
                if (Class<V>::size() == 2)
                    return initialize(dest, index, CORE_CAST(gchar, value));
                return initialize(dest, index, CORE_CAST(gint, value));
            }
        };

        // Strings
        template <class V>
        class Value<V, true, false, false, false, false, true> {
        public:
            // using Foreign class
            template <class Vx>
            static gbool initialize(Array& dest, gint& index, Vx&& value) {
                dest.set(index++, String(UNSAFE::forwardInstance<Vx>(value)));
                return true;
            }
        };

        // Objects
        template <class V>
        class Value<V, true, false, false, false, false, false> {
        public:
            template <class Vx>
            static gbool initialize(Array& dest, gint& index, Vx&& value) {
                dest.set(index++, UNSAFE::forwardInstance<Vx>(value));
                return true;
            }
        };

    public:
        /**
         * Obtain newly created array with arbitrary number of arguments.
         *
         * @note The order of element is resulting array is not always
         *       equals to order of given arguments. In this case the order
         *       of resulting array is reversed from order of given arguments.
         * @param args the trailing arguments
         * @return the new array
         */
        template <class... T>
        static Array of(T&&... args) {
            try {
                CORE_FAST gint count = sizeof...(T);
                Array a = Array(count);
                {
                    gint wi = 0;
                    gbool _[] = {Value<T>::initialize(a, wi, UNSAFE::forwardInstance<T>(args))...};
                }
                return a;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Obtains new Array instance with C-Style array.
         *
         * @tparam Es type of C-style array
         * @tparam Capture1 the value used to ensure that the given argument is C-Style array
         * @tparam Ee the type of element containing into the given array
         * @tparam Capture2 the value used to ensure the compatibility of elements between given array and desired array
         * @param es the C-Style array
         * @return the new array
         */
        template <class Es,
                  ClassOf(1)::OnlyIf<Class<Es>::isArray()> Capture1 = 1,
                  class Ee = typename Class<Es>::ArrayElement,
                  ClassOf(1)::OnlyIf<Class<Ee>::template isExtends<E>()> Capture2 = 1>
        static Array copyOf(Es& es) {
            gint count = Class<Es>::count();

            try {
                Array a = Array(count);

                for (int i = 0; i < count; ++i) a.set(i, es[i]);

                return a;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Obtains new Array instance with C-Style array.
         *
         * @tparam Es type of C-style array
         * @tparam Capture1 the value used to ensure that the given argument is C-Style array
         * @tparam Ee the type of element containing into the given array
         * @tparam Capture2 the value used to ensure the compatibility of elements between given array and desired array
         * @param es the C-Style array
         * @return the new array
         */
        template <class Es,
                  ClassOf(1)::OnlyIf<Class<Es>::isArray()> Capture1 = 1,
                  class Ee = typename Class<Es>::ArrayElement,
                  ClassOf(1)::OnlyIfAll<!Class<Ee>::template isExtends<E>(),
                                        Class<E>::template isConvertible<Es>()> Capture2 = 1>
        static Array copyOf(Es& es) {
            gint count = Class<Es>::count();

            try {
                Array a = Array(count);

                for (int i = 0; i < count; ++i) a.set(i, CORE_CAST(E, es[i]));

                return a;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

    private:
        class LinearIterator final : public Object {
            CORE_ADD_AS_FRIEND(Array);

            Array& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator(Array& array, gbool isEnd)
                : array(array),
                  next(isEnd ? array.length() : 0),
                  isEnd(isEnd || array.length() == 0) {}

        public:
            LinearIterator& operator++() {
                isEnd = isEnd || ++next >= array.length();

                return *this;
            }

            E& operator*() const {
                if (!isEnd && array.check0(next)) return CORE_XCAST(E, array.get0(next));

                util::NoSuchElementException().throws($ftrace());
            }

            gbool operator==(LinearIterator const& rhs) const {
                return this == &rhs || &array == &rhs.array && (isEnd && rhs.isEnd || next == rhs.next);
            }

            gbool operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }
        };

        class LinearIterator2 final : public Object {
            CORE_ADD_TEMPLATES_AS_FRIEND(Array);

            Array const& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator2(Array const& array, gbool isEnd)
                : array(array),
                  next(isEnd ? array.length() : 0),
                  isEnd(isEnd || array.length() == 0) {}

        public:
            LinearIterator2& operator++() {
                isEnd = isEnd || ++next >= array.length();

                return *this;
            }

            E const& operator*() const {
                if (!isEnd && array.check0(next)) return CORE_XCAST(E const, array.get0(next));

                util::NoSuchElementException().throws($ftrace());
            }

            gbool operator==(LinearIterator2 const& rhs) const {
                return this == &rhs || &array == &rhs.array && (isEnd && rhs.isEnd || next == rhs.next);
            }

            gbool operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }
        };

    public:
        LinearIterator begin() { return LinearIterator(*this, false); }

        LinearIterator2 begin() const { return LinearIterator2(*this, false); }

        LinearIterator end() { return LinearIterator(*this, true); }

        LinearIterator2 end() const { return LinearIterator2(*this, true); }
    };

    /**
     * @c Array class wrap the reference array.  It's equivalent in C to @c Object*[N]
     * This array style is mostly used by collections to ensure that
     * instance of collection element is same with array returned by
     * @c Collection::toArray.
     */
    template <>
    class Array<Object> final : public misc::ObjectArray {
    public:
        /**
         * Construct new empty @c Array instance.
         */
        CORE_IMPLICIT Array() = default;

        /**
         * Construct new @c Array instance able to contains
         * the given number of references.
         *
         * @note All places will be not initialized.
         *
         * @param length The number of value to be allocated on this array
         * @throws IllegalArgumentException If the given length is negative
         *
         * @note Deprecation: This constructor do not initialize newly created places with valid default constructor,
         *              and you should to set all places with Array::set(gint, Object) before the using array.
         *              Is Recommended to used Constructor with two arguments parameters.
         */
        CORE_EXPLICIT Array(gint length) {
            if (length < 0)
                IllegalArgumentException("Negative array size: "_S + length).throws($ftrace());

            try { reserve(length); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Construct new @c Array instance able to contains
         * the given number of references.
         *
         * @note All places will be initialized with given reference.
         *
         * @throws IllegalArgumentException If the given length is negative
         * @param length The number of value to be allocated on this array
         * @param initialValue The references used to initialize all values of this array.
         */
        CORE_EXPLICIT Array(gint length, Object const& initialValue) {
            if (length < 0)
                IllegalArgumentException("Negative array size: "_S + length).throws($ftrace());

            try {
                reserve(length);
                if (initialValue != null) {
                    Object& initializer = UNSAFE::copyInstance(initialValue);
                    for (gint i = 0; i < length; i++)
                        set0(i, initializer);
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Construct new instance of Array<?> and initialize
         * with arbitrary number of arguments.
         *
         * @tparam E the types of given arguments
         * @param args the arguments used to construct this array
         */
        template <class E>
        CORE_IMPLICIT Array(VarArgs<E, Object> const& args) {
            gint len = CORE_CAST(gint, args.size());

            try {
                reserve(len);

                for (gint i = 0; i < len; i++) {
                    Object& obj = Array<>::of(args.begin()[i])[0];
                    set0(i, obj);
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Construct new array with given another array
         *
         * @param array The original array
         */
        Array(Array const& array): Array() {
            try {
                gint n = array.length();

                reserve(n);
                setAll0(array);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Construct new array with given another array
         *
         * @param array The original array
         */
        Array(Array&& array) CORE_NOTHROW { swapAll0(array); }

        Array& operator=(const Array& other) {
            try {
                if (this != &other) {
                    reserve(other.length());
                    setAll0(other);
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return *this;
        }

        Array& operator=(Array&& other) CORE_NOTHROW {
            swapAll0(other);
            return *this;
        }

        template <class X,
                  ClassOf(1)::OnlyIf<Class<Object>::isSuper<X>()> CaptureX = 1>
        Array(Array<X> const& a): Array() {
            try {
                reserve(a.length());
                setAll0(a);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Return the number of references on this array
         *
         * @return the number references supported by this array.
         */
        gint length() const override { return ObjectArray::length(); }

        /**
         * Test if this array has no elements.
         *
         * @return the boolean value that indicate if this array has no element.
         */
        gbool isEmpty() const override { return ObjectArray::isEmpty(); }

        /**
        * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, @c IllegalStateException is throwing.
         *
         * @param index the position of reference
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         * @throws IllegalStateException If the no reference found.
         */
        Object& get(gint index) {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (!check0(index))
                util::NoSuchElementException().throws($ftrace());
            return get0(index);
        }

        /**
        * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, @c IllegalStateException is throwing.
         *
         * @param index the position of reference
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         * @throws IllegalStateException If the no reference found.
         */
        Object const& get(gint index) const {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (!check0(index))
                util::NoSuchElementException().throws($ftrace());
            return get0(index);
        }

        /**
         * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, the null reference is returned.
         *
         * @param index the position of reference
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         */
        Object& getOrNull(gint index) {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return get0(index);
        }

        /**
         * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, the null reference is returned.
         *
         * @param index the position of reference
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         */
        Object const& getOrNull(gint index) const {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return get0(index);
        }

        /**
         * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, the fallback reference is returned.
         *
         * @param index the position of reference
         * @param fallback reference to be returned if no reference found.
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         */
        Object& getOrElse(gint index, Object const& fallback) {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (!check0(index)) return UNSAFE::copyInstance(fallback);
            return get0(index);
        }

        /**
        * Return the reference value at the specified index.
         *
         * @note During operation, if no reference found at the
         * given position, the fallback reference is returned.
         *
         * @param index the position of reference
         * @param fallback reference to be returned if no reference found.
         * @return the reference at specified index
         * @throws IndexOutOfBoundsException If the index out of range.
         */
        Object const& getOrElse(gint index, Object const& fallback) const {
            try {
                misc::Preconditions::checkIndex(index, length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (!check0(index)) return UNSAFE::copyInstance(fallback);
            return get0(index);
        }

        Object& set(gint index, Object const& newValue) {
            try {
                misc::Preconditions::checkIndex(index, length());
                Object& value = UNSAFE::copyInstance(newValue);

                return set0(index, value);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Object& clone() const override {
            try { return UNSAFE::newInstance<Array>(*this); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ~Array() override { reserve(0); }

        Object& operator[](gint index) { return get(index); }

        Object const& operator[](gint index) const { return get(index); }

        template <class X, ClassOf(1)::OnlyIf<Class<Object>::isSuper<X>()> CaptureX = 1>
        CORE_EXPLICIT operator Array<X>() const {
            Array<X> a;
            gint len = length();
            try { a.reserve(len); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            for (gint i = 0; i < len; i++) {
                if (!check0(i)) // Value is not set
                    continue;
                Object& obj = (Object&) get0(i);
                if (!Class<X>::hasInstance(obj))
                    ClassCastException("Unable to convert object of type " + typeName(obj) +
                        " to element of " + typeName(a)).throws($ftrace());
                a.set0(i, obj);
            }
            return a;
        }

    private:
        // Error
        template <class V,
                  gbool isAccepted = Class<Object>::accept<V>(),
                  gbool isBoolean = Class<V>::isBoolean(),
                  gbool isInteger = Class<V>::isInteger(),
                  gbool isFloating = Class<V>::isFloating(),
                  gbool isCharacter = Class<V>::isCharacter(),
                  gbool isString = Class<V>::isString()>
        class Value {
        public:
            template <class Vx>
            static gbool initialize(Array&, gint&, Vx&&) {
                CORE_FAST_XASSERT(isAccepted, "Illegal argument");
                return false;
            }
        };

        // Boolean
        template <class V>
        class Value<V, true, true, false, false, false, false> {
        public:
            template <class Vx>
            static gbool initialize(Array& dest, gint& index, V&& value) {
                dest.set(index++, Boolean::valueOf(value));
                return true;
            }
        };

        // Integer
        template <class V>
        class Value<V, true, false, true, false, false, false> {
        public:
            static gbool initialize(Array& dest, gint& index, gbyte value) {
                dest.set(index++, Byte::valueOf(value));
                return true;
            }

            static gbool initialize(Array& dest, gint& index, gshort value) {
                dest.set(index++, Short::valueOf(value));
                return true;
            }

            static gbool initialize(Array& dest, gint& index, gint value) {
                dest.set(index++, Integer::valueOf(value));
                return true;
            }

            static gbool initialize(Array& dest, gint& index, glong value) {
                dest.set(index++, Long::valueOf(value));
                return true;
            }

            template <class X>
            static gbool initialize(Array& dest, gint& index, X&& value) {
                if (Class<V>::size() == 1)
                    return initialize(dest, index, CORE_CAST(gbyte, value));
                if (Class<V>::size() == 2)
                    return initialize(dest, index, CORE_CAST(gshort, value));
                if (Class<V>::size() == 4)
                    return initialize(dest, index, CORE_CAST(gint, value));
                return initialize(dest, index, CORE_CAST(glong, value));
            }
        };

        // Floating
        template <class V>
        class Value<V, true, false, false, true, false, false> {
        public:
            static gbool initialize(Array& dest, gint& index, gfloat value) {
                dest.set(index++, Float::valueOf(value));
                return true;
            }

            static gbool initialize(Array& dest, gint& index, gdouble value) {
                dest.set(index++, Double::valueOf(value));
                return true;
            }

            template <class X>
            static gbool initialize(Array& dest, gint& index, X&& value) {
                if (Class<V>::size() <= 4)
                    return initialize(dest, index, CORE_CAST(gfloat, value));
                return initialize(dest, index, CORE_CAST(gdouble, value));
            }
        };

        // Characters
        template <class V>
        class Value<V, true, false, false, false, true, false> {
        public:
            // character of 1 byte (widening cas)
            static gbool initialize(Array& dest, gint& index, gbyte value) {
                dest.set(index++, Character::valueOf(value & 0xff));
                return true;
            }

            // character of 2 bytes
            static gbool initialize(Array& dest, gint& index, gchar value) {
                dest.set(index++, Character::valueOf(value));
                return true;
            }

            // character of 4 bytes (narrowing cast)
            static gbool initialize(Array& dest, gint& index, gint value) {
                dest.set(index++, Character::valueOf(value & 0xffff));
                return true;
            }

            template <class X>
            static gbool initialize(Array& dest, gint& index, X&& value) {
                if (Class<V>::size() == 1)
                    return initialize(dest, index, CORE_CAST(gbyte, value));
                if (Class<V>::size() == 2)
                    return initialize(dest, index, CORE_CAST(gchar, value));
                return initialize(dest, index, CORE_CAST(gint, value));
            }
        };

        // Strings
        template <class V>
        class Value<V, true, false, false, false, false, true> {
        public:
            // using Foreign class
            template <class Vx>
            static gbool initialize(Array& dest, gint& index, Vx&& value) {
                dest.set(index++, String(UNSAFE::forwardInstance<Vx>(value)));
                return true;
            }
        };

        // Objects
        template <class V>
        class Value<V, true, false, false, false, false, false> {
        public:
            template <class Vx>
            static gbool initialize(Array& dest, gint& index, Vx&& value) {
                dest.set(index++, UNSAFE::forwardInstance<Vx>(value));
                return true;
            }
        };

    public:
        /**
         * Obtain newly created array with arbitrary number of arguments.
         *
         * @note The order of element is resulting array is not always
         *       equals to order of given arguments. In this case the order
         *       of resulting array is reversed from order of given arguments.
         * @param args the trailing arguments
         * @return the new array
         */
        template <class... T>
        static Array of(T&&... args) {
            try {
                CORE_FAST gint count = sizeof...(T);
                Array a = Array(count);
                {
                    gint wi = 0;
                    gbool _[] = {Value<T>::initialize(a, wi, UNSAFE::forwardInstance<T>(args))...};
                }
                return a;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Obtains new Array instance with C-Style array.
         *
         * @tparam Es type of C-style array
         * @tparam Capture1 the value used to ensure that the given argument is C-Style array
         * @tparam Ee the type of element containing into the given array
         * @tparam Capture2 the value used to ensure the compatibility of elements between given array and desired array
         * @param es the C-Style array
         * @return the new array
         */
        template <class Es,
                  ClassOf(1)::OnlyIf<Class<Es>::isArray()> Capture1 = 1,
                  class Ee = typename Class<Es>::ArrayElement,
                  ClassOf(1)::OnlyIf<Class<Object>::isSuper<Ee>()> Capture2 = 1>
        static Array copyOf(Es& es) {
            gint count = Class<Es>::size() / Class<Ee>::size();

            try {
                Array a = Array(count);

                for (int i = 0; i < count; ++i) a.set(i, es[i]);

                return a;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        /**
         * Obtains new Array instance with C-Style array.
         *
         * @tparam Es type of C-style array
         * @tparam Capture1 the value used to ensure that the given argument is C-Style array
         * @tparam Ee the type of element containing into the given array
         * @tparam Capture2 the value used to ensure the compatibility of elements between given array and desired array
         * @param es the C-Style array
         * @return the new array
         */
        template <class Es,
                  ClassOf(1)::OnlyIf<Class<Es>::isArray()> Capture1 = 1,
                  class Ee = typename Class<Es>::ArrayElement,
                  ClassOf(1)::OnlyIfAll<!Class<Object>::isSuper<Ee>(),
                                        Class<Object>::isConvertible<Es>()> Capture2 = 1>
        static Array copyOf(Es& es) {
            gint count = Class<Es>::size() / Class<Ee>::size();

            try {
                Array a = Array(count);

                for (int i = 0; i < count; ++i) a.set(i, es[i]);

                return a;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

    private:
        class LinearIterator final : public Object {
            CORE_ADD_TEMPLATES_AS_FRIEND(Array);

            Array& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator(Array& array, gbool isEnd)
                : array(array),
                  next(isEnd ? array.length() : 0),
                  isEnd(isEnd || array.length() == 0) {}

        public:
            LinearIterator& operator++() {
                isEnd = isEnd || ++next >= array.length();

                return *this;
            }

            Object& operator*() const {
                if (!isEnd && array.check0(next)) return array.get0(next);

                util::NoSuchElementException().throws($ftrace());
            }

            gbool operator==(LinearIterator const& rhs) const {
                return this == &rhs || (&array == &rhs.array && ((isEnd && rhs.isEnd) || next == rhs.next));
            }

            gbool operator!=(LinearIterator const& rhs) const { return !(*this == rhs); }
        };

        class LinearIterator2 final : public Object {
            CORE_ADD_TEMPLATES_AS_FRIEND(Array);

            Array const& array;
            gint next = 0;
            gbool isEnd = false;

            CORE_EXPLICIT LinearIterator2(Array const& array, gbool isEnd)
                : array(array),
                  next(isEnd ? array.length() : 0),
                  isEnd(isEnd || array.length() == 0) {}

        public:
            LinearIterator2& operator++() {
                isEnd = isEnd || ++next >= array.length();

                return *this;
            }

            Object const& operator*() const {
                if (!isEnd && array.check0(next)) return array.get0(next);

                util::NoSuchElementException().throws($ftrace());
            }

            gbool operator==(LinearIterator2 const& rhs) const {
                return this == &rhs || (&array == &rhs.array && ((isEnd && rhs.isEnd) || next == rhs.next));
            }

            gbool operator!=(LinearIterator2 const& rhs) const { return !(*this == rhs); }
        };

    public:
        LinearIterator begin() { return LinearIterator(*this, false); }

        LinearIterator2 begin() const { return LinearIterator2(*this, false); }

        LinearIterator end() { return LinearIterator(*this, true); }

        LinearIterator2 end() const { return LinearIterator2(*this, true); }
    };

    CORE_WARNING_POP
} // core

#endif //CORE24_ARRAY_H
