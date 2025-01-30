//
// Created by bruns on 05/05/2024.
//

#ifndef CORE24_LITERALS_H
#define CORE24_LITERALS_H

#include <exception>
#include <core/misc/Templates.h>

namespace core {
#ifdef CORE_PRINT_BINARY_OPERATOR
#undef CORE_PRINT_BINARY_OPERATOR
#undef CORE_PRINT_UNARY_OPERATOR
#undef CORE_PRINT_REVERSIBLE_BINARY_OPERATOR
#undef CORE_PRINT_ALL_STRINGS_OPERATORS
#undef CORE_PRINT_ALL_OBJECTS_OPERATORS
#undef CORE_PRINT_ALL_NUMBERS_OPERATORS
#undef CORE_PRINT_ALL_STRINGS_LITERALS
#undef CORE_PRINT_ALL_NUMBERS_LITERALS
#endif

#ifndef CORE_PRINT_BINARY_OPERATOR
#define CORE_PRINT_BINARY_OPERATOR(Return, First, Second, Operator) extern Return operator Operator (First, Second);
    //
#define CORE_PRINT_UNARY_OPERATOR(Return, First, Operator) extern Return operator Operator (First);
    //
#define CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Return, First, Second, Operator) \
    CORE_PRINT_BINARY_OPERATOR(Return, First, Second, Operator) \
    CORE_PRINT_BINARY_OPERATOR(Return, Second, First, Operator)

    //
#define CORE_PRINT_ALL_STRINGS_OPERATORS(String) \
    CORE_PRINT_BINARY_OPERATOR(String, String const&, String const&, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, Object const&, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, bool, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, short, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, unsigned short, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, char, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, unsigned char, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, char16_t, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, char32_t, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, wchar_t, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, int, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, unsigned int, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, long, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, unsigned long, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, long long, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, unsigned long long, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, float, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, double, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(String, String const&, long double, +) \
    \
    CORE_PRINT_BINARY_OPERATOR(String &, String &, String const&, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, Object const&, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, bool, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, short, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, unsigned short, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, char, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, unsigned char, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, char16_t, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, char32_t, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, wchar_t, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, int, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, unsigned int, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, long, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, unsigned long, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, long long, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, unsigned long long, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, float, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, double, +=)\
    CORE_PRINT_BINARY_OPERATOR(String &, String &, long double, +=) \
    \
    CORE_PRINT_BINARY_OPERATOR(gbool, String const&, String const &, ==) \
    CORE_PRINT_BINARY_OPERATOR(gbool, String const&, String const &, !=) \
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, String const&, Object const &, ==) \
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, String const&, Object const &, !=) \
    \
    //

    //
#define CORE_PRINT_ALL_OBJECTS_OPERATORS(Object) \
    CORE_PRINT_BINARY_OPERATOR(gbool, Object const&, Object const&, ==)\
    CORE_PRINT_BINARY_OPERATOR(gbool, Object const&, Object const&, !=)\
    \
    //

#define CORE_PRINT_ALL_NUMBERS_OPERATORS(Number) \
    CORE_PRINT_BINARY_OPERATOR(Number, Number const&, Number const&, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, short, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned short, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, int, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned int, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned long, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long long, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned long long, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, float, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, double, +)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long double, +)\
    \
    CORE_PRINT_BINARY_OPERATOR(Number, Number const&, Number const&, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, short, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned short, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, int, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned int, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned long, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long long, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned long long, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, float, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, double, -)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long double, -)\
    \
    CORE_PRINT_BINARY_OPERATOR(Number, Number const&, Number const&, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, short, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned short, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, int, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned int, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned long, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long long, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned long long, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, float, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, double, *)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long double, *)\
    \
    CORE_PRINT_BINARY_OPERATOR(Number, Number const&, Number const&, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, short, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned short, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, int, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned int, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned long, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long long, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, unsigned long long, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, float, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, double, /)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(Number, Number const&, long double, /)\
    \
    CORE_PRINT_BINARY_OPERATOR(gbool, Number const&, Number const&, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, short, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, unsigned short, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, int, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, unsigned int, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, long, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, unsigned long, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, long long, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, unsigned long long, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, float, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, double, ==)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, long double, ==)\
    \
    CORE_PRINT_BINARY_OPERATOR(gbool, Number const&, Number const&, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, short, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, unsigned short, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, int, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, unsigned int, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, long, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, unsigned long, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, long long, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, unsigned long long, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, float, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, double, !=)\
    CORE_PRINT_REVERSIBLE_BINARY_OPERATOR(gbool, Number const&, long double, !=)\
    \
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, Number const&, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, short, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned short, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, int, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned int, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned long, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long long, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned long long, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, float, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, double, +=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long double, +=)\
    \
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, Number const&, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, short, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned short, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, int, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned int, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned long, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long long, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned long long, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, float, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, double, -=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long double, -=)\
    \
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, Number const&, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, short, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned short, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, int, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned int, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned long, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long long, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned long long, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, float, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, double, *=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long double, *=)\
    \
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, Number const&, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, short, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned short, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, int, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned int, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned long, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long long, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, unsigned long long, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, float, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, double, /=)\
    CORE_PRINT_BINARY_OPERATOR(Number &, Number &, long double, /=)\
    \
    CORE_PRINT_UNARY_OPERATOR(Number, Number const&, +)\
    CORE_PRINT_UNARY_OPERATOR(Number, Number const&, -)\
    CORE_PRINT_UNARY_OPERATOR(Number, Number const&, ~)

#define CORE_PRINT_ALL_STRINGS_LITERALS(String, Suffix) \
    CORE_PRINT_BINARY_OPERATOR(String, const char[], size_t, "" ## Suffix) \
    CORE_PRINT_BINARY_OPERATOR(String, const char16_t[], size_t, "" ## Suffix) \
    CORE_PRINT_BINARY_OPERATOR(String, const char32_t[], size_t, "" ## Suffix) \
    CORE_PRINT_BINARY_OPERATOR(String, const wchar_t[], size_t, "" ## Suffix)

#define CORE_PRINT_ALL_NUMBERS_LITERALS(Number, Suffix) \
    CORE_PRINT_UNARY_OPERATOR(Number, unsigned long long, "" ## Suffix) \
    CORE_PRINT_UNARY_OPERATOR(Number, long double, "" ## Suffix)

#endif

#ifdef CORE_SYNC_CLASS
#undef CORE_SYNC_CLASS
#endif


#define CORE_SYNC_CLASS(Prefix) \
    class Prefix ## Sync final : public concurrent::Lock {\
        util::Optional<Lock> internalLock;\
\
    public:\
        Prefix ## Sync() {\
            internalLock = UNSAFE::newInstance<concurrent::ReentrantLock>();\
        }\
\
        void lock() override {\
            if (internalLock.isPresent())\
                internalLock.get().lock();\
        }\
\
        void lockInterruptibly() override {\
            if (internalLock.isPresent())\
                internalLock.get().lockInterruptibly();\
        }\
\
        gbool tryLock() override {\
            if (internalLock.isPresent())\
                return internalLock.get().tryLock();\
            return false;\
        }\
\
        gbool tryLock(glong time, time::Temporal::ChronoUnit unit) override {\
            if (internalLock.isPresent())\
                return internalLock.get().tryLock(time, unit);\
            return false;\
        }\
\
        void unlock() override {\
            if (internalLock.isPresent())\
                internalLock.get().unlock();\
        }\
\
        concurrent::Condition& newCondition() override {\
            if (internalLock.isPresent())\
                return internalLock.get().newCondition();\
            CORE_ASSERT(false);\
        }\
\
        ~Prefix ## Sync() override {\
            if (internalLock.isPresent()) {\
                while (CORE_XCAST(concurrent::ReentrantLock, internalLock.get()).isLocked())\
                    internalLock.get().unlock();\
            }\
            UNSAFE::deleteRegInstance(internalLock.get());\
            internalLock = util::Optional<Lock>();\
        }\
    }


    class Object;
    class String;
    class XString;

    class BooleanArray;
    class ByteArray;
    class ShortArray;
    class IntArray;
    class FloatArray;
    class LongArray;
    class DoubleArray;
    class CharArray;
    class StringArray;

    class BooleanArray2D;
    class ByteArray2D;
    class ShortArray2D;
    class IntArray2D;
    class FloatArray2D;
    class LongArray2D;
    class DoubleArray2D;
    class CharArray2D;

    class BooleanArray3D;
    class ByteArray3D;
    class ShortArray3D;
    class IntArray3D;
    class FloatArray3D;
    class LongArray3D;
    class DoubleArray3D;
    class CharArray3D;

    template <class>
    class Iterable;

    namespace misc {
        class BasicArray;

        template <class T>
        class PrimitiveArray;

        template <class T>
        class ValueArray;

        class ObjectArray;
    }

    class Boolean;
    class Byte;
    class Short;
    class Integer;
    class Long;
    class Float;
    class Double;
    class Complex;
    class Character;
    class CharSequence;

    template <class E = Object>
    class Comparable;
    template <class E = Object>
    class Array;
    template <class Clazz = Object>
    class Class;
    template <class E>
    class Enum;

    class Number;

    class Throwable;

    class IllegalArgumentException;
    class IllegalStateException;
    class UnsupportedOperationException;
    class CloneNotSupportedException;
    class OutOfMemoryError;
    class Exception;
    class Error;
    class IndexOutOfBoundsException;
    class RuntimeException;

    namespace misc {
        class Unsafe;
        class NIL;
    }


    /*
     * This file contains all declaration of literals expression
     * for type of Core24 lib
     */
    inline namespace literals {
        /*
         * Literals
         */

        CORE_PRINT_ALL_STRINGS_LITERALS(String, _S) // For auto deduct encoding (ansi for Microsoft Visual studio
        // Compiler or If constant Unsafe::ANSI_STRING
        //and Unicode of others)

        /**
         * Create new Unicode Literal String with specified string.
         */
        CORE_PRINT_ALL_STRINGS_LITERALS(String, _Su) // For Unicode strings

        /**
         * Create new Latin-1 Literal String with specified string.
         * This operation respect the following rules for all character of literal string:
         * @code
         *  String s1 = "a"_Sl; // product: "a"
         *  String s2 = u"a"_Sl; // product: "\0a" (little endian) or "a\0" (big endian)
         *  String s3 = U"a"_Sl; // product: "\0\0\0a" (little endian) or "a\0\0\0" (big endian)
         *  // if sizeof(wchar_t) == 2
         *  String s4 = L"a"_Sl; // product: "\0a" (little endian) or "a\0" (big endian)
         *  // if sizeof(wchar_t) == 4
         *  String s4 = L"a"_Sl; // product: "\0\0\0a" (little endian) or "a\0\0\0" (big endian)
         * @endcode
         */
        CORE_PRINT_ALL_STRINGS_LITERALS(String, _Sl) // For ansi (latin-1 or bytes) strings

        CORE_PRINT_ALL_NUMBERS_LITERALS(Complex, _i) // 1 + 2_i

        CORE_PRINT_ALL_NUMBERS_LITERALS(Complex, _j) // 1 + 2_j

#if __cplusplus > 201103L
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_UDL

        CORE_PRINT_ALL_NUMBERS_LITERALS(Complex, i) // 1 + 2i
        CORE_PRINT_ALL_NUMBERS_LITERALS(Complex, j) // 1 + 2j

        CORE_WARNING_POP
#endif


        /*
         * Operators
         */


        CORE_PRINT_ALL_STRINGS_OPERATORS(String)

        CORE_PRINT_ALL_NUMBERS_OPERATORS(Complex)

        CORE_PRINT_ALL_OBJECTS_OPERATORS(Object)
    }

#ifdef $trace
#undef $trace
#endif
#ifdef $ftrace
#undef $ftrace
#endif
#ifdef $xtrace
#undef $xtrace
#endif

#ifndef $trace
    /**
     * Obtain the Trace instance representing the current execution point.
     * @note the function or method name is in short format (without details)
     * @param x the unquoted container (namespace, function, class, union, ...) name
     */
#define $trace(...) Trace(""#__VA_ARGS__, CORE_FUNCTION, CORE_FILE, CORE_LINE)

    /**
     * Obtain the Trace instance representing the current execution point.
     * @note the function or method name is in long format (with all details)
     * @param x the unquoted container (namespace, function, class, union, ...) name
     */
#define $ftrace(...) Trace(""#__VA_ARGS__, CORE_FUNCTION_SIGNATURE, CORE_FILE, CORE_LINE)

    /**
     * Obtain the Trace instance representing the current execution point in current class.
     * @note the current container maybe extends Object class to use @c Object::typeName(Object).
     *      the method name is in long format (with all details).
     *      It recommended to use this macro into the non statics methods
     */
#define $xtrace(...) Trace(Object::typeName(*this), CORE_FUNCTION_SIGNATURE, CORE_FILE, CORE_LINE)

#define $shortTrace(...) $trace(__VA_ARGS__)
#define $longTrace(...) $ftrace(__VA_ARGS__)
#define $objTrace(...) $xtrace(__VA_ARGS__)
#endif

#ifdef BIG_ENDIAN
#undef BIG_ENDIAN // for Unsafe::BIG_ENDIAN
#endif
#ifdef min
#undef min // for Math::min
#undef max // for Math::max
#endif
} // core

namespace core {

    namespace util {
        class Arrays;
        template <class E = Object>
        class Iterator;
        template <class E = Object>
        class Iterator2;
        class IntIterator;
        class LongIterator;
        class DoubleIterator;
        template <class E = Object>
        class Spliterator;
        template <class E = Object>
        class Spliterator2;
        class IntSpliterator;
        class LongSpliterator;
        class DoubleSpliterator;
        template <class E = Object>
        class Comparator;
        template <class E = Object>
        class Collection;
        template <class E = Object>
        class List;
        template <class E = Object>
        class Set;
        template <class E = Object>
        class ArrayList;
        template <class E = Object>
        class LinkedList;
        template <class E = Object>
        class Queue;
        template <class E = Object>
        class PriorityQueue;
        template <class E = Object>
        class Deque;
        template <class E = Object>
        class ArrayDeque;
        template <class E = Object>
        class LinkedDeque;
        template <class E = Object>
        class SequencedSet;
        template <class E = Object>
        class SortedSet;
        template <class E = Object>
        class NavigableSet;
        template <class E = Object>
        class HashSet;
        template <class E = Object>
        class LinkedHashSet;
        template <class E = Object>
        class TreeSet;
        template <class E = Object>
        class Vector;
        template <class K = Object, class V = Object>
        class Entry;
        template <class K = Object, class V = Object>
        class Map;
        template <class K = Object, class V = Object>
        class SequencedMap;
        template <class K = Object, class V = Object>
        class SortedMap;
        template <class K = Object, class V = Object>
        class NavigableMap;
        template <class K = Object, class V = Object>
        class HashMap;
        template <class K = Object, class V = Object>
        class LinkedHashMap;
        template <class K = Object, class V = Object>
        class TreeMap;
        template <class T = Object>
        class Optional;


        class Locale;
        class StringTokenizer;
        class StringJoiner;
    }

    namespace function {
        class DoubleConsumer;
        class IntConsumer;
        class LongConsumer;

        class DoublePredicate;
        class DoublePredicate;
        class DoublePredicate;

        template <class R = Object>
        class DoubleFunction;
        template <class R = Object>
        class IntFunction;
        template <class R = Object>
        class LongFunction;

        class DoubleUnaryOperator;
        class DoubleBinaryOperator;
        class IntUnaryOperator;
        class IntBinaryOperator;
        class LongUnaryOperator;
        class LongBinaryOperator;

        class DoubleToIntFunction;
        class DoubleToLongFunction;
        class IntToDoubleFunction;
        class IntToLongFunction;
        class LongToDoubleFunction;
        class LongToIntFunction;

        template <class R = Object>
        class ToDoubleFunction;
        template <class R = Object>
        class ToIntFunction;
        template <class R = Object>
        class ToLongFunction;

        template <class T = Object, class R = Object>
        class Function;
        template <class T = Object>
        class Consumer;
        template <class T = Object>
        class Predicate;

        template <class T = Object, class U = Object, class R = Object>
        class BiFunction;
        template <class T = Object, class U = Object>
        class BiConsumer;
        template <class T = Object, class U = Object>
        class BiPredicate;

        template <class T = Object>
        class UnaryOperator;
        template <class T = Object>
        class BinaryOperator;

        template <class T = Object>
        class Supplier;
        class BooleanSupplier;
        class IntSupplier;
        class LongSupplier;
        class DoubleSupplier;
    }

    namespace atomic {
        class AtomicBoolean;
        class AtomicInteger;
        class AtomicLong;
        template <class T = Object>
        class AtomicReference;
        class AtomicIntegerArray;
        class AtomicLongArray;
        template <class T = Object>
        class AtomicReferenceArray;
    }

    namespace time {
        class Temporal;
        class TemporalQuery;
        class TemporalException;
        class DateTimeException;
        class LocalTime;
        class LocalDate;
        class LocalDateTime;
        class Duration;
        class Chrono;
        class ZoneId;
        class ZoneOffset;
        class ZoneRules;
        class ZoneOffsetTransition;
        class ZoneOffsetTransitionRule;
        class ZonedDateTime;
        class OffsetTime;
        class OffsetDateTime;
        class Calendar;
        class TimeZone;
        class Date;
    }

    namespace random {
        class Random;
        class RandomGenerator;
        class RealDistribution;
        class IntegerDistribution;
    }

    namespace text {
        class Normalizer;
    }

    namespace charset {
        class Charset;
        class CharsetDecoder;
        class CharsetEncoder;
        class CharacterCodingException;
        class IllegalCharsetNameException;
        class MalformedInputException;
        class UnmappableCharacterException;
    }

    namespace io {
        class Buffer;
        class ByteBuffer;
        class CharBuffer;
        class ShortBuffer;
        class IntBuffer;
        class LongBuffer;
        class FloatBuffer;
        class DoubleBuffer;
        class InvalidMarkException;
        class BufferOverflowException;
        class BufferUnderflowException;
        class ReadOnlyBufferException;

        class File;
        class FileArray;
        class FileFilter;
        class FilenameFilter;

        class Serializable;
        class ObjectReader;
        class ObjectWriter;

        class InputStream;
        class OutputStream;
    }

    namespace stream {
        class IntStream;
        class LongStream;
        class FloatStream;
        class DoubleStream;
        template <class E = Object>
        class Stream;
    }

    namespace regex {
        class Pattern;
        class Matcher;
    }

    namespace net {
        class URI;
        class URL;
        class Inet4Address;
        class Inet6Address;
        class URISynthaxException;
        class MalformedURLException;
    }

    namespace concurrent {
        class Lock;
        class Condition;
        class ReentrantLock;
        class ReadWriteLock;
        class QueuedSynchronizer;
        class OwnableSynchronizer;
        class LockSupport;
        class Semaphore;
        template <class E = Object>
        class ConcurrentLinkedQueue;
        template <class E = Object>
        class ConcurrentLinkedDeque;
        template <class K = Object, class V = Object>
        class ConcurrentMap;
        template <class K = Object, class V = Object>
        class ConcurrentNavigableMap;
        template <class K = Object, class V = Object>
        class ConcurrentHashMap;
        class Synchronized;
    }

    class Appendable;
    class Runnable;
    class Thread;
    class Process;
}

CORE_IGNORE_DEPRECATIONS(
    CORE_PRINT_UNARY_OPERATOR(void*, size_t, new) // no inline, required by [replacement.functions]/3
    CORE_PRINT_UNARY_OPERATOR(void*, size_t, new[]) // no inline, required by [replacement.functions]/3
    CORE_PRINT_UNARY_OPERATOR(void, void*, delete)
    CORE_PRINT_UNARY_OPERATOR(void, void*, delete[])
    CORE_PRINT_BINARY_OPERATOR(void, void*, size_t, delete)
    CORE_PRINT_BINARY_OPERATOR(void, void*, size_t, delete[])
)

namespace core {
#ifdef CORE_PRINT_BINARY_OPERATOR
#undef CORE_PRINT_BINARY_OPERATOR
#undef CORE_PRINT_UNARY_OPERATOR
#undef CORE_PRINT_REVERSIBLE_BINARY_OPERATOR
#undef CORE_PRINT_ALL_STRINGS_OPERATORS
#undef CORE_PRINT_ALL_OBJECTS_OPERATORS
#undef CORE_PRINT_ALL_NUMBERS_OPERATORS
#undef CORE_PRINT_ALL_STRINGS_LITERALS
#undef CORE_PRINT_ALL_NUMBERS_LITERALS
#endif
}

#endif // CORE24_LITERALS_H
