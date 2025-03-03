//
// Created by admin on 18/01/25.
//

#ifndef CORE24_OBJECTS_H
#define CORE24_OBJECTS_H
#include <core/lang/Float.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/Long.h>
#include <core/lang/XString.h>

namespace core {
    namespace util {
        /**
         * This class consists of @c static utility methods for operating
         * on objects, or checking certain conditions before operation.  These utilities
         * include @c null-safe or @c null-tolerant methods for computing the
         * hash code of an object, returning a string for an object, comparing two
         * objects, and checking if indexes or sub-range values are out of bounds.
         */
        class Objects final : public Object {
            /**
             *
             */
            Objects();

        public:
            /**
             * Returns @c true if the arguments are equal to each other
             * and @c false otherwise.
             * Consequently, if both arguments are @c null, @c true
             * is returned.  Otherwise, if the first argument is not {@code
             * null}, equality is determined by calling the {@link
             * Object#equals equals} method of the first argument with the
             * second argument of this method. Otherwise, @c false is
             * returned.
             *
             * @param a an object
             * @param b an object to be compared with {@code a} for equality
             * @return @c true if the arguments are equal to each other
             * and @c false otherwise
             * @see Object#equals(Object)
             */
            static gbool equals(Object const& a, Object const& b);

            /**
             * Returns the hash code of a non-@c null argument and 0 for
             * a @c null argument.
             *
             * @param obj an object
             * @return the hash code of a non-@c null argument and 0 for
             * a @c null argument
             * @see Object#hash
             */
            static gint hash(Object const& obj);

            /**
             * Generates a hash code for a sequence of input values. The hash
             * code is generated as if all the input values were placed into an
             * array, and that array were hashed by calling {@link
             * Arrays#hash(Object[])}.
             *
             * <p>This method is useful for implementing {@link
             * Object#hash()} on objects containing multiple fields. For
             * example, if an object that has three fields, @c x, {@code
             * y}, and {@code z}, one could write:
             *
             * <blockquote><pre>
             * &#064;Override public int hash() {
             *     return Objects.hash(x, y, z);
             * }
             * </pre></blockquote>
             *
             * <b>Warning: When a single object reference is supplied, the returned
             * value does not equal the hash code of that object reference.</b> This
             * value can be computed by calling {@link #hash(Object)}.
             *
             * @param values the values to be hashed
             * @return a hash value of the sequence of input values
             * @see Arrays#hash(Object[])
             * @see List#hash
             */
            template <class... Args>
            static gint hash(Args const&... values);

            /**
             * Returns the result of calling {@code toString} for a non-{@code
             * null} argument and {@code "null"} for a @c null argument.
             *
             * @param obj an object
             * @return the result of calling {@code toString} for a non-{@code
             * null} argument and {@code "null"} for a @c null argument
             * @see Object#toString
             * @see String#valueOf(Object)
             */
            static String toString(Object const& obj);

            /**
             * Returns the result of calling {@code toString} for a non-{@code
             * null} argument and {@code "null"} for a @c null argument.
             *
             * @param obj an object
             * @return the result of calling {@code toString} for a non-{@code
             * null} argument and {@code "null"} for a @c null argument
             * @see Object#toString
             * @see String#valueOf(Object)
             */
            template <class Arg>
            static String toString(Arg const& arg);

            /**
             * Returns the result of calling {@code toString} on the first
             * argument if the first argument is not @c null and returns
             * the second argument otherwise.
             *
             * @param obj an object
             * @param nullDefault string to return if the first argument is
             *        @c null
             * @return the result of calling {@code toString} on the first
             * argument if it is not @c null and the second argument
             * otherwise.
             * @see Objects#toString(Object)
             */
            static String toString(Object const& obj, String const& nullDefault);

            /**
             * Checks that the specified object reference is not @c null. This
             * method is designed primarily for doing parameter validation in methods
             * and constructors, as demonstrated below:
             * <blockquote><pre>
             * public Foo(Bar bar) {
             *     this.bar = Objects.requireNonNull(bar);
             * }
             * </pre></blockquote>
             *
             * @param obj the object reference to check for nullity
             * @param <T> the type of the reference
             * @return {@code obj} if not @c null
             * @throws NullPointerException if {@code obj} is @c null
             */
            template <class T>
            static T& requireNonNull(T& obj) {
                if (Pointers::isNull(obj)) {
                    if (Class<T>::isPointer())
                        IllegalArgumentException("Null pointer").throws($ftrace());
                    else
                        IllegalArgumentException("Null object").throws($ftrace());
                }
                return obj;
            }

            /**
             * Checks that the specified object reference is not @c null. This
             * method is designed primarily for doing parameter validation in methods
             * and constructors, as demonstrated below:
             * <blockquote><pre>
             * public Foo(Bar bar) {
             *     this.bar = Objects.requireNonNull(bar);
             * }
             * </pre></blockquote>
             *
             * @param obj the object reference to check for nullity
             * @param <T> the type of the reference
             * @return {@code obj} if not @c null
             * @throws NullPointerException if {@code obj} is @c null
             */
            template <class T>
            static T const& requireNonNull(T const& obj) {
                if (Pointers::isNull(obj)) {
                    if (Class<T>::isPointer())
                        IllegalArgumentException("Null pointer").throws($ftrace());
                    else
                        IllegalArgumentException("Null object").throws($ftrace());
                }
                return obj;
            }

            /**
             * Checks that the specified object reference is not @c null. This
             * method is designed primarily for doing parameter validation in methods
             * and constructors, as demonstrated below:
             * <blockquote><pre>
             * public Foo(Bar bar) {
             *     this.bar = Objects.requireNonNull(bar);
             * }
             * </pre></blockquote>
             *
             * @param obj the object reference to check for nullity
             * @param <T> the type of the reference
             * @return {@code obj} if not @c null
             * @throws NullPointerException if {@code obj} is @c null
             */
            template <class T>
            static T&& requireNonNull(T&& obj) {
                if (Pointers::isNull(obj)) {
                    if (Class<T>::isPointer())
                        IllegalArgumentException("Null pointer").throws($ftrace());
                    else
                        IllegalArgumentException("Null object").throws($ftrace());
                }
                return (T&&) obj;
            }

            /**
             * Checks that the specified object reference is not @c null and
             * throws a customized {@link NullPointerException} if it is. This method
             * is designed primarily for doing parameter validation in methods and
             * constructors with multiple parameters, as demonstrated below:
             * <blockquote><pre>
             * public Foo(Bar bar, Baz baz) {
             *     this.bar = Objects.requireNonNull(bar, "bar must not be null");
             *     this.baz = Objects.requireNonNull(baz, "baz must not be null");
             * }
             * </pre></blockquote>
             *
             * @param obj     the object reference to check for nullity
             * @param message detail message to be used in the event that a {@code
             *                NullPointerException} is thrown
             * @param <T> the type of the reference
             * @return {@code obj} if not @c null
             * @throws NullPointerException if {@code obj} is @c null
             */
            template <class T>
            static T& requireNonNull(T& obj, String const& message) {
                if (Pointers::isNull(obj))
                    IllegalArgumentException(message).throws($ftrace());
                return obj;
            }

            /**
             * Checks that the specified object reference is not @c null and
             * throws a customized {@link NullPointerException} if it is. This method
             * is designed primarily for doing parameter validation in methods and
             * constructors with multiple parameters, as demonstrated below:
             * <blockquote><pre>
             * public Foo(Bar bar, Baz baz) {
             *     this.bar = Objects.requireNonNull(bar, "bar must not be null");
             *     this.baz = Objects.requireNonNull(baz, "baz must not be null");
             * }
             * </pre></blockquote>
             *
             * @param obj     the object reference to check for nullity
             * @param message detail message to be used in the event that a {@code
             *                NullPointerException} is thrown
             * @param <T> the type of the reference
             * @return {@code obj} if not @c null
             * @throws NullPointerException if {@code obj} is @c null
             */
            template <class T>
            static T const& requireNonNull(T const& obj, String const& message) {
                if (Pointers::isNull(obj))
                    IllegalArgumentException(message).throws($ftrace());
                return obj;
            }

            /**
             * Checks that the specified object reference is not @c null and
             * throws a customized {@link NullPointerException} if it is. This method
             * is designed primarily for doing parameter validation in methods and
             * constructors with multiple parameters, as demonstrated below:
             * <blockquote><pre>
             * public Foo(Bar bar, Baz baz) {
             *     this.bar = Objects.requireNonNull(bar, "bar must not be null");
             *     this.baz = Objects.requireNonNull(baz, "baz must not be null");
             * }
             * </pre></blockquote>
             *
             * @param obj     the object reference to check for nullity
             * @param message detail message to be used in the event that a {@code
             *                NullPointerException} is thrown
             * @param <T> the type of the reference
             * @return {@code obj} if not @c null
             */
            template <class T>
            static T&& requireNonNull(T&& obj, String const& message) {
                if (Pointers::isNull(obj))
                    IllegalArgumentException(message).throws($ftrace());
                return (T&&) obj;
            }

            /**
             * Returns @c true if the provided reference is @c null otherwise
             * returns @c false.
             *
             * @apiNote This method exists to be used as a
             * {@link java.util.function.Predicate}, {@code filter(Objects::isNull)}
             *
             * @param obj a reference to be checked against @c null
             * @return @c true if the provided reference is @c null otherwise
             * @c false
             *
             * @see java.util.function.Predicate
             * @since 1.8
             */
            template <class T>
            static gbool isNull(T&& obj) {
                return Pointers::isNull(obj);
            }

            /**
             * Checks if the @c index is within the bounds of the range from
             * @c 0 (inclusive) to @c length (exclusive).
             *
             * <p>The @c index is defined to be out of bounds if any of the
             * following inequalities is true:
             * <ul>
             *  <li>{@code index < 0}</li>
             *  <li>{@code index >= length}</li>
             *  <li>{@code length < 0}, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param index the index
             * @param length the upper-bound (exclusive) of the range
             * @return @c index if it is within bounds of the range
             * @throws IndexOutOfBoundsException if the @c index is out of bounds
             * @since 9
             */
            static gint checkIndex(gint index, gint length);

            /**
             * Checks if the sub-range from @c fromIndex (inclusive) to
             * {@code toIndex} (exclusive) is within the bounds of range from @c 0
             * (inclusive) to @c length (exclusive).
             *
             * <p>The sub-range is defined to be out of bounds if any of the following
             * inequalities is true:
             * <ul>
             *  <li>{@code fromIndex < 0}</li>
             *  <li>{@code fromIndex > toIndex}</li>
             *  <li>{@code toIndex > length}</li>
             *  <li>{@code length < 0}, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param fromIndex the lower-bound (inclusive) of the sub-range
             * @param toIndex the upper-bound (exclusive) of the sub-range
             * @param length the upper-bound (exclusive) the range
             * @return @c fromIndex if the sub-range within bounds of the range
             * @throws IndexOutOfBoundsException if the sub-range is out of bounds
             * @since 9
             */
            static gint checkIndexForRange(gint fromIndex, gint toIndex, gint length);

            /**
             * Checks if the sub-range from @c fromIndex (inclusive) to
             * {@code fromIndex + size} (exclusive) is within the bounds of range from
             * @c 0 (inclusive) to @c length (exclusive).
             *
             * <p>The sub-range is defined to be out of bounds if any of the following
             * inequalities is true:
             * <ul>
             *  <li>{@code fromIndex < 0}</li>
             *  <li>{@code size < 0}</li>
             *  <li>{@code fromIndex + size > length}, taking into account integer overflow</li>
             *  <li>{@code length < 0}, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param fromIndex the lower-bound (inclusive) of the sub-interval
             * @param size the size of the sub-range
             * @param length the upper-bound (exclusive) of the range
             * @return @c fromIndex if the sub-range within bounds of the range
             * @throws IndexOutOfBoundsException if the sub-range is out of bounds
             * @since 9
             */
            static gint checkIndexForSize(gint fromIndex, gint size, gint length);

            /**
             * Checks if the @c index is within the bounds of the range from
             * @c 0 (inclusive) to @c length (exclusive).
             *
             * <p>The @c index is defined to be out of bounds if any of the
             * following inequalities is true:
             * <ul>
             *  <li>{@code index < 0}</li>
             *  <li>{@code index >= length}</li>
             *  <li>{@code length < 0}, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param index the index
             * @param length the upper-bound (exclusive) of the range
             * @return @c index if it is within bounds of the range
             * @throws IndexOutOfBoundsException if the @c index is out of bounds
             * @since 16
             */
            static glong checkIndex(glong index, glong length);

            /**
             * Checks if the sub-range from @c fromIndex (inclusive) to
             * {@code toIndex} (exclusive) is within the bounds of range from @c 0
             * (inclusive) to @c length (exclusive).
             *
             * <p>The sub-range is defined to be out of bounds if any of the following
             * inequalities is true:
             * <ul>
             *  <li>{@code fromIndex < 0}</li>
             *  <li>{@code fromIndex > toIndex}</li>
             *  <li>{@code toIndex > length}</li>
             *  <li>{@code length < 0}, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param fromIndex the lower-bound (inclusive) of the sub-range
             * @param toIndex the upper-bound (exclusive) of the sub-range
             * @param length the upper-bound (exclusive) the range
             * @return @c fromIndex if the sub-range within bounds of the range
             * @throws IndexOutOfBoundsException if the sub-range is out of bounds
             * @since 16
             */
            static glong checkIndexForRange(glong fromIndex, glong toIndex, glong length);

            /**
             * Checks if the sub-range from @c fromIndex (inclusive) to
             * {@code fromIndex + size} (exclusive) is within the bounds of range from
             * @c 0 (inclusive) to @c length (exclusive).
             *
             * <p>The sub-range is defined to be out of bounds if any of the following
             * inequalities is true:
             * <ul>
             *  <li>{@code fromIndex < 0}</li>
             *  <li>{@code size < 0}</li>
             *  <li>{@code fromIndex + size > length}, taking into account integer overflow</li>
             *  <li>{@code length < 0}, which is implied from the former inequalities</li>
             * </ul>
             *
             * @param fromIndex the lower-bound (inclusive) of the sub-interval
             * @param size the size of the sub-range
             * @param length the upper-bound (exclusive) of the range
             * @return @c fromIndex if the sub-range within bounds of the range
             * @throws IndexOutOfBoundsException if the sub-range is out of bounds
             * @since 16
             */
            static glong checkIndexForSize(glong fromIndex, glong size, glong length);

        private:
            class Hasher final : public Object {
                Hasher();

            public:
                static gint hash(Object const& obj);

                static gint hash(gbool b);

                static gint hash(gbyte b);

                static gint hash(gshort s);

                static gint hash(gint i);

                static gint hash(glong i);

                static gint hash(gfloat f);

                static gint hash(gdouble d);

                static gint hash(gchar c);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isInteger()>  = 1>
                static gint hash(T i);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isFloating()>  = 1>
                static gint hash(T f);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isCharacter()>  = 1>
                static gint hash(T c);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isString()>  = 1>
                static gint hash(T const& s);

                template <class T, ClassOf(1)::OnlyIf<!Class<T>::isString() && Class<T>::isArray()>  = 1>
                static gint hash(T const& a);

                template <class T, ClassOf(1)::OnlyIf<!Class<T>::isString() && Class<T>::isPointer()>  = 1>
                static gint hash(T const& a);
            };

            class Stringifier final : public Object {
                Stringifier();

            public:
                static String toString(Object const& obj);

                static String toString(gbool b);

                static String toString(gbyte b);

                static String toString(gshort s);

                static String toString(gint i);

                static String toString(glong i);

                static String toString(gfloat f);

                static String toString(gdouble d);

                static String toString(gchar c);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isInteger()>  = 1>
                static String toString(T i);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isFloating()>  = 1>
                static String toString(T f);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isCharacter()>  = 1>
                static String toString(T c);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isString()>  = 1>
                static String toString(T const& s);

                template <class T, ClassOf(1)::OnlyIf<!Class<T>::isString() && Class<T>::isArray()>  = 1>
                static String toString(T const& a);

                template <class T, ClassOf(1)::OnlyIf<!Class<T>::isString() && Class<T>::isPointer()>  = 1>
                static String toString(T const& a);
            };

            class Pointers final : public Object {
                Pointers();

            public:
                static gbool isNull(Object const& obj);

                template <class T, ClassOf(1)::OnlyIf<Class<T>::isPointer()>  = 1>
                static gbool isPointer(T&& ptr);

                template <class T, ClassOf(1)::OnlyIf<!Class<T>::isPointer() && !Class<>::isSuper<T>()>  = 1>
                static gbool isPointer(T&&);
            };
        };


        template <class... Args>
        gint Objects::hash(Args const&... values) {
            gint hashes[] = {Hasher::hash(values)...};
            gint result = 0;
            for (gint hash : hashes)
                result = 31 * result + hash;
            return result;
        }

        template <class Arg>
        String Objects::toString(Arg const& arg) {
            try {
                return Stringifier::toString(arg);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isInteger()>>
        gint Objects::Hasher::hash(T i) {
            if (Class<T>::size() <= Integer::BYTES)
                return hash((gint) i);
            return hash((glong) i);
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isFloating()>>
        gint Objects::Hasher::hash(T f) {
            if (Class<T>::size() <= Float::BYTES)
                return hash((gfloat) f);
            return hash((gdouble) f);
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isCharacter()>>
        gint Objects::Hasher::hash(T c) {
            return hash((gint) c);
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isString()>>
        gint Objects::Hasher::hash(T const& s) {
            if (Class<T>::isArray()) {
                // No risk
                return hash(String(s));
            }
            if (s) {
                // No Risk
                return hash(String(s));
            }
            // Null pointer
            return hash(null);
        }

        template <class T, Class<typeof(1)>::OnlyIf<!Class<T>::isString() && Class<T>::isArray()>>
        gint Objects::Hasher::hash(T const& a) {
            gint result = 0;
            CORE_ALIAS(E, typename Class<T>::ArrayElement);
            for (E const& e : a) {
                result = 31 * result + hash(e);
            }
            return result;
        }

        template <class T, Class<typeof(1)>::OnlyIf<!Class<T>::isString() && Class<T>::isPointer()>>
        gint Objects::Hasher::hash(T const& a) {
            // We'll convert pointer to address
            return a == null ? hash(null) : hash((glong) a);
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isInteger()>>
        String Objects::Stringifier::toString(T i) {
            if (Class<T>::size() <= Integer::BYTES)
                return toString((gint) i);
            return toString((glong) i);
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isFloating()>>
        String Objects::Stringifier::toString(T f) {
            if (Class<T>::size() <= Float::BYTES)
                return toString((gfloat) f);
            return toString((gdouble) f);
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isCharacter()>>
        String Objects::Stringifier::toString(T c) {
            if (Class<T>::size() <= Character::BYTES)
                return toString((gchar) (c < 0 ? c & 0xff : c));
            return Character::toString((gint) c);
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isString()>>
        String Objects::Stringifier::toString(T const& s) {
            if (Class<T>::isArray()) {
                return String(s);
            }
            if (s) {
                return String(s);
            }
            return String::valueOf(null);
        }

        template <class T, Class<typeof(1)>::OnlyIf<!Class<T>::isString() && Class<T>::isArray()>>
        String Objects::Stringifier::toString(T const& a) {
            XString str;
            str.append('[');
            gint size = Class<T>::count();
            for (gint i = 0; i < size; i++) {
                str.append(toString(a[i]));
                if (i < size - 1)
                    str.append(", ");
            }
            str.append(']');
            return str.toString();
        }

        template <class T, Class<typeof(1)>::OnlyIf<!Class<T>::isString() && Class<T>::isPointer()>>
        String Objects::Stringifier::toString(T const& a) {
            return XString("0x").append(Long::toHexString((glong) a)).toString();
        }

        template <class T, Class<typeof(1)>::OnlyIf<Class<T>::isPointer()>>
        gbool Objects::Pointers::isPointer(T&& ptr) {
            return ptr == null;
        }

        template <class T, Class<typeof(1)>::OnlyIf<!Class<T>::isPointer() && !Class<>::isSuper<T>()>>
        gbool Objects::Pointers::isPointer(T&&) {
            return false;
        }
    } // util
} // core

#endif //CORE24_OBJECTS_H
