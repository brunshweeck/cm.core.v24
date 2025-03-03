//
// Created by bruns on 05/05/2024.
//

#ifndef CORE24_TYPES_H
#define CORE24_TYPES_H

#include <core/misc/Compiler.h>

namespace core {
    namespace misc {
        extern "C" {
            typedef char __literal_chr_t;
            typedef unsigned long long __literal_int64_t;
            typedef long double __literal_float64_t;

            typedef signed char __int8_t;
            typedef unsigned char __uint8_t;
            typedef signed short __int16_t;
            typedef unsigned short __uint16_t;
            typedef signed int __int32_t;
            typedef unsigned int __uint32_t;
#if __WORDSIZE == 64
            typedef signed long __int64_t;
            typedef unsigned long __uint64_t;
            typedef __uint64_t __memory_size_t;
#else
        typedef signed long long __int64_t;
        typedef unsigned long long __uint64_t;
        typedef __uint64_t __memory_size_t;
#endif
            typedef float __float32_t;
            typedef double __float64_t;
            typedef char16_t __ucs2_t;
            typedef char32_t __ucs4_t;
            typedef bool __bool_t;

            typedef _Fcomplex __cplex32_t;
            typedef _Dcomplex __cplex64_t;
            typedef _Lcomplex __cplex64x_t;
        }


#ifndef CORE_ALIAS
#define CORE_ALIAS(A, ...) using A = $(__VA_ARGS__)
#endif


#ifndef CORE_ADD_AS_FRIEND

#define CORE_ADD_GLOBAL_FRIENDS() \
    template <gint, class...> friend class ::core::misc::TEST;\
    template <class> friend class ::core::Class; \
    friend ::core::misc::Unsafe

#define CORE_ADD_AS_FRIEND(T) friend T

#define CORE_ADD_TEMPLATES_AS_FRIEND(T, ...) template<class $(__VA_ARGS__)> friend class T
#endif

#ifndef CORE_SEALED
#define CORE_SEALED(permits, ...) $()
#endif


#ifndef STRINGIFY
#define STRINGIFY(...) #__VA_ARGS__
#endif

#ifndef CORE_FAST_ASSERT
#define CORE_FAST_ASSERT(x) static_assert(x, STRINGIFY(x))
#define CORE_FAST_XASSERT(x, text) static_assert(x, text)
#endif

#ifndef CORE_FAST
#define CORE_FAST CORE_CONSTEXPR
#endif


#define $(...) __VA_ARGS__

        /**
         * Reverse order two argument.
         * Example:
         * @code
         *     $(1, 2) => 1 2
         *     $$(a, b) => 2 1
         * @endcode
         *
         * @param a the first argument.
         */
#define $$(a, ...) __VA_ARGS__ a

#ifndef CORE_CAST
#define CORE_CAST(T, ...)  ((T)(__VA_ARGS__))
#define CORE_FCAST(T, ...) T(__VA_ARGS__)
#define CORE_DCAST(T, x) (dynamic_cast<$(T)>($(x)))
#define CORE_XCAST(T, ...) CORE_DCAST($(T) &, $(__VA_ARGS__))
#endif //CORE_CAST

#ifndef CORE_IGNORE
#define CORE_IGNORE(x) CORE_CAST(void, x)
#define CORE_HIDE_FIRST(x, ...) ((CORE_IGNORE(x), $(__VA_ARGS__)))
#endif //CORE_IGNORE

#ifndef CORE_FILE
#define CORE_FILE __FILE__
#endif //CORE_FILE

#ifndef CORE_LINE
#define CORE_LINE __LINE__
#endif //CORE_LINE

#ifndef CORE_IMPORT_FIELD_OR_METHOD
#define CORE_IMPORT_FIELD_OR_METHOD(FromClass, FieldOrMethod, ...) using FromClass::FieldOrMethod $(__VA_ARGS__)
#endif //CORE_IMPORT_FIELD_OR_METHOD
    } // misc
} // core


/*
 * Define the generic type (g???)
 */

// Generic Boolean (1 byte)
CORE_ALIAS(gbool, core::misc::__bool_t);
// Generic Byte    (1 byte)
CORE_ALIAS(gbyte, core::misc::__int8_t);
// Generic Short   (2 bytes)
CORE_ALIAS(gshort, core::misc::__int16_t);
// Generic Char    (2 bytes)
CORE_ALIAS(gchar, core::misc::__ucs2_t);
// Generic Integer (4 bytes)
CORE_ALIAS(gint, core::misc::__int32_t);
// Generic Float   (4 bytes)
CORE_ALIAS(gfloat, core::misc::__float32_t);
// Generic Long    (8 bytes)
CORE_ALIAS(glong, core::misc::__int64_t);
// Generic Double  (8 bytes)
CORE_ALIAS(gdouble, core::misc::__float64_t);

/**
 *  Check The validity of compiler
 */
CORE_FAST_ASSERT(sizeof(gbool) == 1L);
CORE_FAST_ASSERT(sizeof(gbyte) == 1L);
CORE_FAST_ASSERT(sizeof(gshort) == 2L);
CORE_FAST_ASSERT(sizeof(gchar) == 2L);
CORE_FAST_ASSERT(sizeof(gint) == 4L);
CORE_FAST_ASSERT(sizeof(gfloat) == 4L);
CORE_FAST_ASSERT(sizeof(glong) == 8L);
CORE_FAST_ASSERT(sizeof(gdouble) == 8L);

#ifndef CORE_TRY

#define CORE_TRY(...) try { __VA_ARGS__; }

#define CORE_CATCH(T, ...) catch (T const & ex) { __VA_ARGS__; }

#define CORE_CATCH_ANY(...) catch (...) { __VA_ARGS__; }

#define CORE_TRY_ONLY(...) \
    CORE_TRY(__VA_ARGS__) \
    CORE_CATCH_ANY()

#define CORE_TRY_RETHROW_AT(ClassName, ...) \
    CORE_TRY(__VA_ARGS__) \
    CORE_CATCH(Throwable, ex.throws($ftrace(ClassName)))

#define CORE_TRY_RETHROW(...) CORE_TRY_RETHROW_AT(, __VA_ARGS__)

#endif

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifndef CORE_CHECK_TYPE
#define CORE_CHECK_TYPE(T) \
    CORE_FAST_ASSERT(!Class<T>::isConstant()); \
    CORE_FAST_ASSERT(!Class<T>::isVolatile()); \
    CORE_FAST_ASSERT(!Class<T>::isRvalue()); \
    CORE_FAST_ASSERT(Class<T>::isClass());

#define CORE_CHECK_PRIME_TYPE(T) \
    CORE_FAST_ASSERT(!Class<T>::isConstant()); \
    CORE_FAST_ASSERT(!Class<T>::isVolatile()); \
    CORE_FAST_ASSERT(!Class<T>::isRvalue()); \
    CORE_FAST_ASSERT(!Class<T>::isReference());

#define CORE_CHECK_SLIMMED_TYPE(T) \
    CORE_CHECK_TYPE(T) \
    CORE_FAST_ASSERT(!Class<T>::isReference());

#define CORE_FULL_CHECK_TYPE(T) \
    CORE_CHECK_TYPE(T) \
    CORE_FAST_ASSERT(Class<Object>::isSuper<T>());

#define CORE_FULL_CHECK_SLIMMED_TYPE(T) \
    CORE_CHECK_SLIMMED_TYPE(T) \
    CORE_FAST_ASSERT(Class<Object>::isSuper<T>());

#endif

namespace core {
    namespace misc {
        class NIL;
    }

    class Object;

    class Boolean;

    class Byte;
    class Short;
    class Integer;
    class Long;

    class Float;
    class Double;

    class Character;
    class String;
    class Complex;

    class BooleanArray;
    class ByteArray;
    class ShortArray;
    class IntArray;
    class LongArray;
    class FloatArray;
    class DoubleArray;
    class CharArray;

    template <class E>
    class Array;

    template <class E>
    class Enum;

    class Void;
}

#ifndef CORE_ASSERT

#define __CORE_ASSERT_AT(expression, ClassName) \
    do{ \
        if((gbool)(expression) == true) {\
            break; \
        } else { \
            AssertionError(STRINGIFY(expression)""_S).throws($ftrace(ClassName));\
        } \
    } while (false)

#define __CORE_ASSERT_AT2(expression, message, ClassName) \
    do{ \
        if((gbool)(expression) == true) {\
            break;\
        } else { \
            AssertionError(message).throws($ftrace(ClassName));\
        } \
    } while (false)

#define CORE_ASSERT(...) __CORE_ASSERT_AT($(__VA_ARGS__), )
#define CORE_ASSERT2(expression, message) __CORE_ASSERT_AT2(expression, message, )
#endif


#ifndef CORE_CONSTANT_VALUES
#if __has_builtin(__builtin_nanf)
#define CORE_CONSTANT_NOT_A_NUMBER CORE_CAST(gdouble, __builtin_nanf(""))
#define CORE_CONSTANT_POSITIVE_INFINITY CORE_CAST(gdouble, __builtin_inff())
#else
#define CORE_CONSTANT_NOT_A_NUMBER -CORE_CAST(gdouble, CORE_FCAST(gfloat, 1.0E300) * 0.0F)
#define CORE_CONSTANT_POSITIVE_INFINITY CORE_CAST(gdouble, CORE_FCAST(gfloat, 1.0E300) * 1.0F)
#endif
#endif

#endif // CORE24_TYPES_H
