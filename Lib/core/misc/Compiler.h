//
// Created by bruns on 05/05/2024.
//

#ifndef CORE24_COMPILER_H
#define CORE24_COMPILER_H

#include <core/misc/Processor.h>

namespace core {
  namespace misc {
    /*
     * This file is created to help any user, that needs to know
     * current operating system information, in instantaneous method.
     */

    /**
     * The Compiler, must be one of: CORE_COMPILER_?
     * (where ? represent the name of compiler): <br/>
     * - COVERITY - Coverity cov-scan <br/>
     * - SYM      - Digital Mars C/C++ (used to be Symantec C++) <br/>
     * - MSVC     - Microsoft Visual C/C++, Intel C++ for Windows <br/>
     * - BOR      - Borland/Turbo C++ <br/>
     * - WAT      - Watcom C++ <br/>
     * - GNU      - GNU C++ <br/>
     * - COMEAU   - Comeau C++ <br/>
     * - EDG      - Edison Design Group C++ <br/>
     * - OC       - CenterLine C++ <br/>
     * - SUN      - Forte Developer, or Sun Studio C++ <br/>
     * - MIPS     - MIPSpro C++ <br/>
     * - DEC      - DEC C++ <br/>
     * - HPACC    - HP aC++ <br/>
     * - USLC     - SCO OUDK and UDK <br/>
     * - CDS      - Reliant C++ <br/>
     * - KAI      - KAI C++ <br/>
     * - INTEL    - Intel C++ for Linux, Intel C++ for Windows <br/>
     * - HIGHC    - MetaWare High C/C++ <br/>
     * - PGI      - Portland Group C++ <br/>
     * - PGI      - Portland Group C++ <br/>
     * - GHS      - Green Hills Optimizing C++ Compilers <br/>
     * - RVCT     - ARM Realview Compiler Suite <br/>
     * - CLANG    - C++ front-end for the LLVM compiler <br/>
     */


#if defined(__COVERITY__)
#define CORE_COMPILER_COVERITY
#endif

    /**
     * Symantec C++ is now Digital Mars
     */
#if defined(__DMC__) || defined(__SC__)
#define CORE_COMPILER_SYM

#if defined(__SC__) && __SC__ < 0x750
#error "Unsupported Compiler version: This Compiler not support 'explicit' keyword"
#endif

#elif defined(_MSC_VER)
#define CORE_COMPILER_MSVC (_MSC_VER)

#ifdef __clang__
#define CORE_COMPILER_CLANG ((__clang_major__ * 100) + __clang_minor__)
#endif

#define CORE_OUTLINE_TEMPLATE inline
#define CORE_XCOMPILER_MANGLES_RETURN_TYPE
#define CORE_XCOMPILER_MANGLES_ACCESS_SPECIFIER
#define CORE_FUNCTION __func__
#define CORE_FUNCTION_SIGNATURE __FUNCSIG__
#define CORE_ASSUME(expr) __assume(expr)
#define CORE_UNREACHABLE() __assume(0)
#define CORE_EXPORT __declspec(dllexport)
#define CORE_IMPORT __declspec(dllimport)

#elif defined(__BORLANDC__) || defined(__TURBOC__)
#define CORE_COMPILER_BOR
#define CORE_INLINE_TEMPLATE

#if __BORLANDC__ < 0x502
#error "Unsupported Compiler version"
#endif

#elif defined(__WATCOMC__)
#define CORE_COMPILER_WAT

/**
 * ARM Realview Compiler Suite
 * RVCT compiler also defines __EDG__ and __GNUC__ (if --gnu flag is given),
 * so check for iterator before that
 */
#elif defined(__ARMCC__) || defined(__CC_ARM)
#define CORE_COMPILER_RVCT

/**
 * work-around for missing compiler intrinsics
 */
#define __is_empty(X) false

/**
 * work-around for missing compiler intrinsics
 */
#define __is_pod(X) false

#define CORE_DEPRECATED __attribute__ ((__deprecated__))

#ifdef CORE_SYSTEM_LINUX
#define CORE_EXPORT     __attribute__((visibility("default")))
#define CORE_IMPORT     __attribute__((visibility("default")))
#define CORE_HIDDEN     __attribute__((visibility("hidden")))

#else
#define CORE_EXPORT     __declspec(dllexport)
#define CORE_IMPORT     __declspec(dllimport)
#endif

#elif defined(__GNUC__)
#define CORE_COMPILER_GNU          (__GNUC__ * 100 + __GNUC_MINOR__)

#if defined(__MINGW32__)
#define CORE_COMPILER_MINGW
#endif

#if defined(__clang__)

    /**
     * Clang also masquerades as GCC
     */
#if defined(__apple_build_version__)

    /**
     * The Clang version reported by Apple Clang in __clang_major__
     * and __clang_minor__ does _not_ reflect the actual upstream
     * version of the compiler. To allow consumers to use a single
     * define to verify the Clang version we hard-code the versions
     * based on the best available info we have about the actual
     * version: http://en.wikipedia.org/wiki/Xcode#Toolchain_Versions
     */
#if __apple_build_version__   >= 13160021 // Xcode 13.3
#define CORE_COMPILER_CLANG 1300

#elif __apple_build_version__ >= 13000029 // Xcode 13.0
#define CORE_COMPILER_CLANG 1200

#elif __apple_build_version__ >= 12050022 // Xcode 12.5
#define CORE_COMPILER_CLANG 1110

#elif __apple_build_version__ >= 12000032 // Xcode 12.0
#define CORE_COMPILER_CLANG 1000

#elif __apple_build_version__ >= 11030032 // Xcode 11.4
#define CORE_COMPILER_CLANG 900

#elif __apple_build_version__ >= 11000033 // Xcode 11.0
#define CORE_COMPILER_CLANG 800

#else
#error "Unsupported Apple Clang version"
#endif

#else
/**
 * Non-Apple Clang, so we trust the versions reported
 */
#define CORE_COMPILER_CLANG ((__clang_major__ * 100) + __clang_minor__)
#endif

#define CORE_COMPILER_CLANG_ONLY CORE_COMPILER_CLANG
#if __has_builtin(__builtin_assume)
#define CORE_ASSUME(expr)   __builtin_assume(expr)
#else

#define CORE_ASSUME(expr)  if (expr){} else __builtin_unreachable()
#endif

#define CORE_UNREACHABLE() __builtin_unreachable()

#if !defined(__has_extension)

/**
 * Compatibility with older Clang versions
 */
#define __has_extension __has_feature
#endif

#if defined(__APPLE__)

/**
 * Apple/clang specific features
 */
#define CORE_CF_RETURNS_RETAINED __attribute__((cf_returns_retained))

#ifdef __OBJC__
#define CORE_NS_RETURNS_AUTORELEASED __attribute__((ns_returns_autoreleased))
#endif

#endif

#ifdef __EMSCRIPTEN__
#define CORE_COMPILER_EMSCRIPTEN
#endif

#else
    /**
     * Plain GCC
     */
#define CORE_COMPILER_GNU_ONLY CORE_COMPILER_GNU
#if CORE_COMPILER_GNU >= 405
#define CORE_ASSUME(expr)  if (expr){} else __builtin_unreachable()
#define CORE_UNREACHABLE() __builtin_unreachable()
#define CORE_XDEPRECATED(text) __attribute__ ((__deprecated__(text)))
#endif

#endif

#ifdef CORE_SYSTEM_WINOWS
#define CORE_EXPORT     __declspec(dllexport)
#define CORE_IMPORT     __declspec(dllimport)

#elif defined(CORE_VISIBILITY_AVAILABLE)
#define CORE_EXPORT_OVERRIDABLE __attribute__((visibility("default"), weak))
#ifdef CORE_USE_PROTECTED_VISIBILITY
#define CORE_EXPORT     __attribute__((visibility("protected")))

#else
#define CORE_EXPORT     __attribute__((visibility("default")))
#endif

#define CORE_IMPORT     __attribute__((visibility("default")))
#define CORE_HIDDEN     __attribute__((visibility("hidden")))
#endif

#define CORE_FUNCTION       __func__
#define CORE_FUNCTION_SIGNATURE       __PRETTY_FUNCTION__
#define CORE_CLASSOF(expr)    __typeof__(expr)
#define CORE_DEPRECATED __attribute__ ((__deprecated__))
#define CORE_XUNUSED     __attribute__((__unused__))
#define CORE_LIKELY(expr)    __builtin_expect(expr, true)
#define CORE_UNLIKELY(expr)  __builtin_expect(expr, false)
#define CORE_NORETURN        __attribute__((__noreturn__))
#define CORE_REQUIRED_RESULT __attribute__ ((__warn_unused_result__))
#define CORE_PURE_FUNCTION __attribute__((pure))
#define CORE_CONST_FUNCTION __attribute__((const))
#define CORE_COLD_FUNCTION __attribute__((cold))
#if !defined(CORE_MOC_CPP)
#define CORE_PACKED __attribute__ ((__packed__))
#ifndef __ARM_EABI__
#define CORE_NO_ARM_EABI
#endif
#endif

#if CORE_COMPILER_GNU >= 403 && !defined(CORE_COMPILER_CLANG)
#define CORE_ALLOC_SIZE(x) __attribute__((alloc_size(x)))
#endif

    /* IBM compiler versions are a bit messy. There are actually two products:
       the C product, and the C++ product. The C++ compiler is always packaged
       with the latest version of the C compiler. Version numbers do not always
       match. This little table (I'm not sure, iterator's accurate) should be helpful:

       C++ product                C product

       C Set 3.1                  C Compiler 3.0
       ...                        ...
       C++ Compiler 3.6.6         C Compiler 4.3
       ...                        ...
       Visual Age C++ 4.0         ...
       ...                        ...
       Visual Age C++ 5.0         C Compiler 5.0
       ...                        ...
       Visual Age C++ 6.0         C Compiler 6.0

       Now:
       __xlC__    is the version of the C compiler in hexadecimal notation
                  is only an approximation of the C++ compiler version
       __IBMCPP__ is the version of the C++ compiler in decimal notation,
                  but iterator is not defined on older compilers like C Set 3.1 */
#elif defined(__xlC__)
#define CORE_COMPILER_XLC
#if __xlC__ < 0x400
#error "Compiler not supported"
#elif __xlC__ >= 0x0600
#define CORE_CLASSOF(expr)      __typeof__(expr)
#define CORE_PACKED            __attribute__((__packed__))
#endif

    /* Older versions of DEC C++ do not define __EDG__ or __EDG - observed
       on DEC C++ V5.5-004. New versions do define  __EDG__ - observed on
       Compaq C++ V6.3-002.
       This compiler is different enough from other EDG compilers to handle
       iterator separately anyway. */
#elif defined(__DECCXX) || defined(__DECC)
#define CORE_COMPILER_DEC
/* Compaq C++ V6 compilers are EDG-based but I'm not sure about older
   DEC C++ V5 compilers. */
#if defined(__EDG__)
#define CORE_COMPILER_EDG
#endif
    /* Compaq has disabled EDG's _BOOL macro and uses _BOOL_EXISTS instead
       - observed on Compaq C++ V6.3-002.
       In any case versions prior to Compaq C++ V6.0-005 do not have bool. */
#if !defined(_BOOL_EXISTS)
#error "Compiler not supported"
#endif
    /* Spurious (?) error messages observed on Compaq C++ V6.5-014. */
    /* Apply to all versions prior to Compaq C++ V6.0-000 - observed on
       DEC C++ V5.5-004. */
#if __DECCXX_VER < 60060000
#define CORE_BROKEN_TEMPLATE_SPECIALIZATION
#endif
/* avoid undefined symbol problems with out-of-line template members */
#define CORE_OUTLINE_TEMPLATE inline

/* The Portland Group C++ compiler is based on EDG and does define __EDG__
   but the C compiler does not */
#elif defined(__PGI)
#define CORE_COMPILER_PGI
#if defined(__EDG__)
#define CORE_COMPILER_EDG
#endif

    /* Compilers with EDG front end are similar. To detect them we test:
       __EDG documented by SGI, observed on MIPSpro 7.3.1.1 and KAI C++ 4.0b
       __EDG__ documented in EDG online docs, observed on Compaq C++ V6.3-002
       and PGI C++ 5.2-4 */
#elif !defined(CORE_SYSTEM_HPUX) && (defined(__EDG) || defined(__EDG__))
#define CORE_COMPILER_EDG
/* From the EDG documentation (does not seem to apply to Compaq C++ or GHS C):
   _BOOL
        Defined in C++ mode when bool is a keyword. The name of this
        predefined macro is specified by a configuration flag. _BOOL
        is the default.
   __BOOL_DEFINED
        Defined in Microsoft C++ mode when bool is a keyword. */
#if !defined(_BOOL) && !defined(__BOOL_DEFINED) && !defined(__ghs)
#error "Compiler not supported"
#endif

    /* The Comeau compiler is based on EDG and does define __EDG__ */
#if defined(__COMO__)
#define CORE_COMPILER_COMEAU

/* The `using' keyword was introduced to avoid KAI C++ warnings
   but iterator's now causing KAI C++ errors instead. The standard is
   unclear about the use of this keyword, and in practice every
   compiler is using its own setValue of rules. Forget iterator. */
#elif defined(__KCC)
#define CORE_COMPILER_KAI

/* Uses CFront, make sure to read the manual how to tweak templates. */
#elif defined(__ghs)
#define CORE_COMPILER_GHS
#define CORE_DEPRECATED __attribute__ ((__deprecated__))
#define CORE_PACKED __attribute__ ((__packed__))
#define CORE_FUNCTION       __func__
#define CORE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#define CORE_CLASSOF(expr)      __typeof__(expr)
#define CORE_UNREACHABLE()
#if defined(__cplusplus)
#define CORE_XCOMPILER_AUTO_TYPE
#define CORE_XCOMPILER_STATIC_ASSERT
#define CORE_XCOMPILER_RANGE_FOR
#if __GHS_VERSION_NUMBER >= 201505
#define CORE_XCOMPILER_ALIGNAS
#define CORE_XCOMPILER_ALIGNOF
#define CORE_XCOMPILER_ATOMICS
#define CORE_XCOMPILER_ATTRIBUTES
#define CORE_XCOMPILER_AUTO_FUNCTION
#define CORE_XCOMPILER_CLASS_ENUM
#define CORE_XCOMPILER_DECLTYPE
#define CORE_XCOMPILER_DEFAULT_MEMBERS
#define CORE_XCOMPILER_DELETE_MEMBERS
#define CORE_XCOMPILER_DELEGATING_CONSTRUCTORS
#define CORE_XCOMPILER_EXPLICIT_CONVERSIONS
#define CORE_XCOMPILER_EXPLICIT_OVERRIDES
#define CORE_XCOMPILER_EXTERN_TEMPLATES
#define CORE_XCOMPILER_INHERITING_CONSTRUCTORS
#define CORE_XCOMPILER_INITIALIZER_LISTS
#define CORE_XCOMPILER_LAMBDA
#define CORE_XCOMPILER_NONSTATIC_MEMBER_INIT
#define CORE_XCOMPILER_NOEXCEPT
#define CORE_XCOMPILER_NULLPTR
#define CORE_XCOMPILER_RANGE_FOR
#define CORE_XCOMPILER_RAW_STRINGS
#define CORE_XCOMPILER_REF_QUALIFIERS
#define CORE_XCOMPILER_RVALUE_REFS
#define CORE_XCOMPILER_STATIC_ASSERT
#define CORE_XCOMPILER_TEMPLATE_ALIAS
#define CORE_XCOMPILER_THREAD_LOCAL
#define CORE_XCOMPILER_UDL
#define CORE_XCOMPILER_UNICODE_STRINGS
#define CORE_XCOMPILER_UNIFORM_INIT
#define CORE_XCOMPILER_UNRESTRICTED_UNIONS
#define CORE_XCOMPILER_VARIADIC_MACROS
#define CORE_XCOMPILER_VARIADIC_TEMPLATES
#endif
#endif //__cplusplus

#elif defined(__DCC__)
#define CORE_COMPILER_DIAB
#if !defined(__bool)
#error "Compiler not supported"
#endif

    /* The UnixWare 7 UDK compiler is based on EDG and does define __EDG__ */
#elif defined(__USLC__) && defined(__SCO_VERSION__)
#define CORE_COMPILER_USLC
/* The latest UDK 7.1.1b does not need this, but previous versions do */
#if !defined(__SCO_VERSION__) || (__SCO_VERSION__ < 302200010)
#define CORE_OUTLINE_TEMPLATE inline
#endif

    /* Never tested! */
#elif defined(CENTERLINE_CLPP) || defined(OBJECTCENTER)
#define CORE_COMPILER_OC

/* CDS++ defines __EDG__ although this is not documented in the Reliant
   documentation. It also follows conventions like _BOOL and this documented */
#elif defined(sinix)
#define CORE_COMPILER_CDS
#endif

    /* VxWorks' DIAB toolchain has an additional EDG type C++ compiler
       (see __DCC__ above). This one is for C mode files (__EDG is not defined) */
#elif defined(_DIAB_TOOL)
#define CORE_COMPILER_DIAB
#define CORE_FUNCTION       __func__
#define CORE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
/* Never tested! */
#elif defined(__HIGHC__)
#define CORE_COMPILER_HIGHC

#elif defined(__SUNPRO_CC) || defined(__SUNPRO_C)
#define CORE_COMPILER_SUN
#define CORE_XCOMPILER_MANGLES_RETURN_TYPE
/* 5.0 compiler or better
    'bool' is enabled by default but can be disabled using -features=nobool
    in which case _BOOL is not defined
        this is the default in 4.2 compatibility mode triggered by -compat=4 */
#if __SUNPRO_CC >= 0x500
#define CORE_NO_TEMPLATE_TEMPLATE_PARAMETERS
   /* see http://developers.sun.com/sunstudio/support/Ccompare.html */
#if __SUNPRO_CC >= 0x590
#define CORE_CLASSOF(expr)    __typeof__(expr)
#endif
#if __SUNPRO_CC >= 0x550
#define CORE_EXPORT     __global
#endif
#if !defined(_BOOL)
#error "Compiler not supported"
#endif
    /* 4.2 compiler or older */
#else
#error "Compiler not supported"
#endif

    /* CDS++ does not seem to define __EDG__ or __EDG according to Reliant
       documentation but nevertheless uses EDG conventions like _BOOL */
#elif defined(sinix)
#define CORE_COMPILER_EDG
#define CORE_COMPILER_CDS
#if !defined(_BOOL)
#error "Compiler not supported"
#endif
#define CORE_BROKEN_TEMPLATE_SPECIALIZATION

#else
#error "This Framework (core) not support this compiler"
#endif

    /*
     * SG10's SD-6 feature detection and some useful extensions from Clang and GCC
     * https://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations
     * http://clang.llvm.org/docs/LanguageExtensions.html#feature-checking-macros
     * Not using wrapper macros, per http://eel.is/c++draft/cpp.cond#7.sentence-2
     */
#ifndef __has_builtin
#define __has_builtin(x)             0
#endif
#ifndef __has_feature
#define __has_feature(x)             0
#endif
#ifndef __has_attribute
#define __has_attribute(x)           0
#endif
#ifndef __has_cpp_attribute
#define __has_cpp_attribute(x)       0
#endif
#ifndef __has_include
#define __has_include(x)             0
#endif
#ifndef __has_include_next
#define __has_include_next(x)        0
#endif

    /*
       detecting ASAN can be helpful to disable slow tests
       clang uses feature, gcc  defines __SANITIZE_ADDRESS__
       unconditionally check both in case other compilers mirror
       either of those options
     */
#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)
#define CORE_ADDRESS_SANITIZER 1
#endif

#ifdef __cplusplus
#if __has_include(<version>)

#include <version>

#endif
#endif

    /*
     * C++11 support
     *
     *  Paper           Macro                                  SD-6 macro
     *  N2341           CORE_XCOMPILER_ALIGNAS
     *  N2341           CORE_XCOMPILER_ALIGNOF
     *  N2427           CORE_XCOMPILER_ATOMICS
     *  N2761           CORE_XCOMPILER_ATTRIBUTES               __cpp_attributes = 200809
     *  N2541           CORE_XCOMPILER_AUTO_FUNCTION
     *  N1984 N2546     CORE_XCOMPILER_AUTO_TYPE
     *  N2437           CORE_XCOMPILER_CLASS_ENUM
     *  N2235           CORE_XCOMPILER_CONSTEXPR                __cpp_constexpr = 200704
     *  N2343 N3276     CORE_XCOMPILER_DECLTYPE                 __cpp_decltype = 200707
     *  N2346           CORE_XCOMPILER_DEFAULT_MEMBERS
     *  N2346           CORE_XCOMPILER_DELETE_MEMBERS
     *  N1986           CORE_XCOMPILER_DELEGATING_CONSTRUCTORS
     *  N2437           CORE_XCOMPILER_EXPLICIT_CONVERSIONS
     *  N3206 N3272     CORE_XCOMPILER_EXPLICIT_OVERRIDES
     *  N1987           CORE_XCOMPILER_EXTERN_TEMPLATES
     *  N2540           CORE_XCOMPILER_INHERITING_CONSTRUCTORS
     *  N2672           CORE_XCOMPILER_INITIALIZER_LISTS
     *  N2658 N2927     CORE_XCOMPILER_LAMBDA                   __cpp_lambdas = 200907
     *  N2756           CORE_XCOMPILER_NONSTATIC_MEMBER_INIT
     *  N2855 N3050     CORE_XCOMPILER_NOEXCEPT
     *  N2431           CORE_XCOMPILER_NULLPTR
     *  N2930           CORE_XCOMPILER_RANGE_FOR
     *  N2442           CORE_XCOMPILER_RAW_STRINGS              __cpp_raw_strings = 200710
     *  N2439           CORE_XCOMPILER_REF_QUALIFIERS
     *  N2118 N2844 N3053 CORE_XCOMPILER_RVALUE_REFS            __cpp_rvalue_references = 200610
     *  N1720           CORE_XCOMPILER_STATIC_ASSERT            __cpp_static_assert = 200410
     *  N2258           CORE_XCOMPILER_TEMPLATE_ALIAS
     *  N2659           CORE_XCOMPILER_THREAD_LOCAL
     *  N2660           CORE_XCOMPILER_THREADSAFE_STATICS
     *  N2765           CORE_XCOMPILER_UDL                      __cpp_user_defined_literals = 200809
     *  N2442           CORE_XCOMPILER_UNICODE_STRINGS          __cpp_unicode_literals = 200710
     *  N2640           CORE_XCOMPILER_UNIFORM_INIT
     *  N2544           CORE_XCOMPILER_UNRESTRICTED_UNIONS
     *  N1653           CORE_XCOMPILER_VARIADIC_MACROS
     *  N2242 N2555     CORE_XCOMPILER_VARIADIC_TEMPLATES       __cpp_variadic_templates = 200704
     *
     *
     * For the C++ standards C++14 and C++17, we use only the SD-6 macro.
     *
     * For any future version of the C++ standard, we use only the C++20 feature test macro.
     * For library features, we assume <version> is present (this header includes iterator).
     *
     * For a full listing of feature test macros, see
     *  https://isocpp.org/std/standing-documents/sd-6-sg10-feature-test-recommendations (by macro)
     *  https://en.cppreference.com/w/User:D41D8CD98F/feature_testing_macros       (by C++ version)
     *
     * C++ extensions:
     * CORE_XCOMPILER_RESTRICTED_VLA       variable-length arrays, prior to __cpp_runtime_arrays
     */

    /*
     * Now that we require C++17, we unconditionally expect threadsafe statics mandated since C++11
     */
#define CORE_XCOMPILER_THREADSAFE_STATICS

#if defined(CORE_COMPILER_CLANG)
/* General C++ features */
#define CORE_XCOMPILER_RESTRICTED_VLA
#if __has_feature(attribute_deprecated_with_message)
#define CORE_XDEPRECATED(text) __attribute__ ((__deprecated__(text)))
#endif

    // Clang supports binary literals in C, C++98 and C++11 modes
    // It's been supported "since the dawn of time itself" (cf. commit 179883)
#if __has_extension(cxx_binary_literals)
#define CORE_XCOMPILER_BINARY_LITERALS
#endif

    // Variadic macros are supported for gnu++98, c++11, c99 ... since 2.9
#if CORE_COMPILER_CLANG >= 209
#if !defined(__STRICT_ANSI__) || defined(__GXX_EXPERIMENTAL_CXX0X__) \
      || (defined(__cplusplus) && (__cplusplus >= 201103L)) \
      || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
#define CORE_XCOMPILER_VARIADIC_MACROS
#endif
#endif

    /* C++11 features, see http://clang.llvm.org/cxx_status.html */
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
    /* Detect C++ features using __has_feature(), see http://clang.llvm.org/docs/LanguageExtensions.html#cxx11 */
#if __has_feature(cxx_alignas)
#define CORE_XCOMPILER_ALIGNAS
#define CORE_XCOMPILER_ALIGNOF
#endif
#if __has_feature(cxx_atomic) && __has_include(<atomic>)
#define CORE_XCOMPILER_ATOMICS
#endif
#if __has_feature(cxx_attributes)
#define CORE_XCOMPILER_ATTRIBUTES
#endif
#if __has_feature(cxx_auto_type)
#define CORE_XCOMPILER_AUTO_FUNCTION
#define CORE_XCOMPILER_AUTO_TYPE
#endif
#if __has_feature(cxx_strong_enums)
#define CORE_XCOMPILER_CLASS_ENUM
#endif
#if __has_feature(cxx_constexpr) && CORE_COMPILER_CLANG > 302 /* CLANG 3.2 has bad/partial support */
#define CORE_XCOMPILER_CONSTEXPR
#endif
#if __has_feature(cxx_decltype) /* && __has_feature(cxx_decltype_incomplete_return_types) */
#define CORE_XCOMPILER_DECLTYPE
#endif
#if __has_feature(cxx_defaulted_functions)
#define CORE_XCOMPILER_DEFAULT_MEMBERS
#endif
#if __has_feature(cxx_deleted_functions)
#define CORE_XCOMPILER_DELETE_MEMBERS
#endif
#if __has_feature(cxx_delegating_constructors)
#define CORE_XCOMPILER_DELEGATING_CONSTRUCTORS
#endif
#if __has_feature(cxx_explicit_conversions)
#define CORE_XCOMPILER_EXPLICIT_CONVERSIONS
#endif
#if __has_feature(cxx_override_control)
#define CORE_XCOMPILER_EXPLICIT_OVERRIDES
#endif
#if __has_feature(cxx_inheriting_constructors)
#define CORE_XCOMPILER_INHERITING_CONSTRUCTORS
#endif
#if __has_feature(cxx_generalized_initializers)
#define CORE_XCOMPILER_INITIALIZER_LISTS
#define CORE_XCOMPILER_UNIFORM_INIT /* both covered by this feature macro, according to docs */
#endif
#if __has_feature(cxx_lambdas)
#define CORE_XCOMPILER_LAMBDA
#endif
#if __has_feature(cxx_noexcept)
#define CORE_XCOMPILER_NOEXCEPT
#endif
#if __has_feature(cxx_nonstatic_member_init)
#define CORE_XCOMPILER_NONSTATIC_MEMBER_INIT
#endif
#if __has_feature(cxx_nullptr)
#define CORE_XCOMPILER_NULLPTR
#endif
#if __has_feature(cxx_range_for)
#define CORE_XCOMPILER_RANGE_FOR
#endif
#if __has_feature(cxx_raw_string_literals)
#define CORE_XCOMPILER_RAW_STRINGS
#endif
#if __has_feature(cxx_reference_qualified_functions)
#define CORE_XCOMPILER_REF_QUALIFIERS
#endif
#if __has_feature(cxx_rvalue_references)
#define CORE_XCOMPILER_RVALUE_REFS
#endif
#if __has_feature(cxx_static_assert)
#define CORE_XCOMPILER_STATIC_ASSERT
#endif
#if __has_feature(cxx_alias_templates)
#define CORE_XCOMPILER_TEMPLATE_ALIAS
#endif
#if __has_feature(cxx_thread_local)
#if !defined(__FreeBSD__) /* FreeBSD clang fails on __cxa_thread_atexit */
#define CORE_XCOMPILER_THREAD_LOCAL
#endif
#endif
#if __has_feature(cxx_user_literals)
#define CORE_XCOMPILER_UDL
#endif
#if __has_feature(cxx_unicode_literals)
#define CORE_XCOMPILER_UNICODE_STRINGS
#endif
#if __has_feature(cxx_unrestricted_unions)
#define CORE_XCOMPILER_UNRESTRICTED_UNIONS
#endif
#if __has_feature(cxx_variadic_templates)
#define CORE_XCOMPILER_VARIADIC_TEMPLATES
#endif
    /* Features that have no __has_feature() check */
#if CORE_COMPILER_CLANG >= 209 /* since clang 2.9 */
#define CORE_XCOMPILER_EXTERN_TEMPLATES
#endif
#endif

    /* C++1y features, deprecated macros. Do not update this list. */
#if __cplusplus > 201103L
    //#if __has_feature(cxx_binary_literals)
    //#define CORE_XCOMPILER_BINARY_LITERALS  // see above
    //#endif
#if __has_feature(cxx_generic_lambda)
#define CORE_XCOMPILER_GENERIC_LAMBDA
#endif
#if __has_feature(cxx_init_capture)
#define CORE_XCOMPILER_LAMBDA_CAPTURES
#endif
#if __has_feature(cxx_XCONSTEXPR)
#define CORE_XCOMPILER_XCONSTEXPR_FUNCTIONS
#endif
#if __has_feature(cxx_decltype_auto) && __has_feature(cxx_return_type_deduction)
#define CORE_XCOMPILER_RETURN_TYPE_DEDUCTION
#endif
#if __has_feature(cxx_variable_templates)
#define CORE_XCOMPILER_VARIABLE_TEMPLATES
#endif
#if __has_feature(cxx_runtime_array)
#define CORE_XCOMPILER_VLA
#endif
#endif

#if defined(__STDC_VERSION__)
#if __has_feature(c_static_assert)
#define CORE_XCOMPILER_STATIC_ASSERT
#endif
#if __has_feature(c_thread_local) && __has_include(<core/lang/threads.h>)
#if !defined(__FreeBSD__) /* FreeBSD clang fails on __cxa_thread_atexit */
#define CORE_XCOMPILER_THREAD_LOCAL
#endif
#endif
#endif

#ifndef CORE_XUNUSED
#define CORE_XUNUSED __attribute__((__unused__))
#endif
#define CORE_XUNUSED_MEMBER CORE_XUNUSED
#endif //  defined(CORE_COMPILER_CLANG)

#if defined(CORE_COMPILER_GNU_ONLY)
#define CORE_XCOMPILER_RESTRICTED_VLA
#if CORE_COMPILER_GNU >= 403
    //   GCC supports binary literals in C, C++98 and C++11 modes
#define CORE_XCOMPILER_BINARY_LITERALS
#endif
#if !defined(__STRICT_ANSI__) || defined(__GXX_EXPERIMENTAL_CXX0X__) \
 || (defined(__cplusplus) && (__cplusplus >= 201103L)) \
 || (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L))
    // Variadic macros are supported for gnu++98, c++11, C99 ... since forever (gcc 2.97)
#define CORE_XCOMPILER_VARIADIC_MACROS
#endif
#if defined(__GXX_EXPERIMENTAL_CXX0X__) || __cplusplus >= 201103L
#if CORE_COMPILER_GNU >= 403
    /* C++11 features supported in GCC 4.3: */
#define CORE_XCOMPILER_DECLTYPE
#define CORE_XCOMPILER_RVALUE_REFS
#define CORE_XCOMPILER_STATIC_ASSERT
#endif
#if CORE_COMPILER_GNU >= 404
    /* C++11 features supported in GCC 4.4: */
#define CORE_XCOMPILER_AUTO_FUNCTION
#define CORE_XCOMPILER_AUTO_TYPE
#define CORE_XCOMPILER_EXTERN_TEMPLATES
#define CORE_XCOMPILER_UNIFORM_INIT
#define CORE_XCOMPILER_UNICODE_STRINGS
#define CORE_XCOMPILER_VARIADIC_TEMPLATES
#endif
#if CORE_COMPILER_GNU >= 405
    /* C++11 features supported in GCC 4.5: */
#define CORE_XCOMPILER_EXPLICIT_CONVERSIONS
    /* GCC 4.4 implements initializer_list but does not define typedefs required
     * by the standard. */
#define CORE_XCOMPILER_INITIALIZER_LISTS
#define CORE_XCOMPILER_LAMBDA
#define CORE_XCOMPILER_RAW_STRINGS
#define CORE_XCOMPILER_CLASS_ENUM
#endif
#if CORE_COMPILER_GNU >= 406
    /* Pre-4.6 compilers implement a non-final snapshot of N2346, hence default and delete
     * functions are supported only if they are public. Starting from 4.6, GCC handles
     * final version - the access modifier is not relevant. */
#define CORE_XCOMPILER_DEFAULT_MEMBERS
#define CORE_XCOMPILER_DELETE_MEMBERS
    /* C++11 features supported in GCC 4.6: */
#define CORE_XCOMPILER_NULLPTR
#define CORE_XCOMPILER_UNRESTRICTED_UNIONS
#define CORE_XCOMPILER_RANGE_FOR
#endif
#if CORE_COMPILER_GNU >= 407
    /* GCC 4.4 implemented <atomic> and std::atomic using its old intrinsics.
     * However, the implementation is incomplete for most platforms until GCC 4.7:
     * instead, std::atomic would use an external lock.*/
#define CORE_XCOMPILER_ATOMICS
    /* GCC 4.6.x has problems dealing with noexcept expressions,
     * so turn the feature on for 4.7 and above, only */
#define CORE_XCOMPILER_NOEXCEPT
    /* C++11 features supported in GCC 4.7: */
#define CORE_XCOMPILER_NONSTATIC_MEMBER_INIT
#define CORE_XCOMPILER_DELEGATING_CONSTRUCTORS
#define CORE_XCOMPILER_EXPLICIT_OVERRIDES
#define CORE_XCOMPILER_TEMPLATE_ALIAS
#define CORE_XCOMPILER_UDL
#endif
#if CORE_COMPILER_GNU >= 408
#define CORE_XCOMPILER_ATTRIBUTES
#define CORE_XCOMPILER_ALIGNAS
#define CORE_XCOMPILER_ALIGNOF
#define CORE_XCOMPILER_INHERITING_CONSTRUCTORS
#define CORE_XCOMPILER_THREAD_LOCAL
#if CORE_COMPILER_GNU > 408 || __GNUC_PATCHLEVEL__ >= 1
#define CORE_XCOMPILER_REF_QUALIFIERS
#endif
#endif
#if CORE_COMPILER_GNU >= 500
    /* GCC 4.6 introduces constexpr, but iterator's bugged (at least) in the whole
     * 4.x series, see e.g. https://gcc.gnu.org/bugzilla/show_bug.cgi?id=57694 */
#define CORE_XCOMPILER_CONSTEXPR
#endif
#endif
#if __cplusplus > 201103L
#if CORE_COMPILER_GNU >= 409
/* C++1y features in GCC 4.9 - deprecated, do not update this list */
//#define CORE_XCOMPILER_BINARY_LITERALS   // already supported since GCC 4.3 as an extension
#define CORE_XCOMPILER_LAMBDA_CAPTURES
#define CORE_XCOMPILER_RETURN_TYPE_DEDUCTION
#endif
#endif
#if defined(__STDC_VERSION__) && __STDC_VERSION__ > 199901L
#if CORE_COMPILER_GNU >= 407
/* C11 features supported in GCC 4.7: */
#define CORE_XCOMPILER_STATIC_ASSERT
#endif
#if CORE_COMPILER_GNU >= 409 && defined(__has_include)
    /* C11 features supported in GCC 4.9: */
#if __has_include(<core/lang/threads.h>)
#define CORE_XCOMPILER_THREAD_LOCAL
#endif
#endif
#endif
#endif

#if defined(CORE_COMPILER_MSVC) && !defined(CORE_COMPILER_CLANG)
#if defined(__cplusplus)
        /* C++11 features supported in VC8 = VC2005: */
#define CORE_XCOMPILER_VARIADIC_MACROS

        /* 2005 supports the override and final contextual keywords, in
         the same positions as the C++11 variants, but 'final' is
         called 'sealed' instead:
         http://msdn.microsoft.com/en-us/library/0w2w91tf%28v=vs.80%29.aspx
         The behavior is slightly different in C++/CLI, which requires the
         "virtual" keyword to be present too, so don't define for that.
         So don't define CORE_XCOMPILER_EXPLICIT_OVERRIDES (since iterator's not
         the same as the C++11 version), but define the CORE_* flags
         accordingly. */
        /* C++11 features supported in VC10 = VC2010: */
#define CORE_XCOMPILER_AUTO_FUNCTION
#define CORE_XCOMPILER_AUTO_TYPE
#define CORE_XCOMPILER_DECLTYPE
#define CORE_XCOMPILER_EXTERN_TEMPLATES
#define CORE_XCOMPILER_LAMBDA
#define CORE_XCOMPILER_NULLPTR
#define CORE_XCOMPILER_RVALUE_REFS
#define CORE_XCOMPILER_STATIC_ASSERT
        /* C++11 features supported in VC11 = VC2012: */
#define CORE_XCOMPILER_EXPLICIT_OVERRIDES /* ...and use std C++11 now   */
#define CORE_XCOMPILER_CLASS_ENUM
#define CORE_XCOMPILER_ATOMICS
        /* C++11 features in VC12 = VC2013 */
#define CORE_XCOMPILER_DELETE_MEMBERS
#define CORE_XCOMPILER_DELEGATING_CONSTRUCTORS
#define CORE_XCOMPILER_EXPLICIT_CONVERSIONS
#define CORE_XCOMPILER_NONSTATIC_MEMBER_INIT
#define CORE_XCOMPILER_RAW_STRINGS
#define CORE_XCOMPILER_TEMPLATE_ALIAS
#define CORE_XCOMPILER_VARIADIC_TEMPLATES
#define CORE_XCOMPILER_INITIALIZER_LISTS // VC 12 SP 2 RC
        /* C++11 features in VC14 = VC2015 */
#define CORE_XCOMPILER_DEFAULT_MEMBERS
#define CORE_XCOMPILER_ALIGNAS
#define CORE_XCOMPILER_ALIGNOF
#define CORE_XCOMPILER_INHERITING_CONSTRUCTORS
#define CORE_XCOMPILER_NOEXCEPT
#define CORE_XCOMPILER_RANGE_FOR
#define CORE_XCOMPILER_REF_QUALIFIERS
#define CORE_XCOMPILER_THREAD_LOCAL
#define CORE_XCOMPILER_UDL
#define CORE_XCOMPILER_UNICODE_STRINGS
#define CORE_XCOMPILER_UNRESTRICTED_UNIONS
#if _MSC_FULL_VER >= 190023419
#define CORE_XCOMPILER_ATTRIBUTES
        // Almost working, see https://connect.microsoft.com/VisualStudio/feedback/details/2011648
        //#define CORE_XCOMPILER_CONSTEXPR
#define CORE_XCOMPILER_UNIFORM_INIT
#endif
#if _MSC_VER >= 1910
#define CORE_XCOMPILER_CONSTEXPR
#endif
#endif /* __cplusplus */
#endif // defined(CORE_COMPILER_MSVC) && !defined(CORE_COMPILER_CLANG)

#ifdef CORE_XCOMPILER_UNICODE_STRINGS
#define CORE_STDLIB_UNICODE_STRINGS
#elif defined(__cplusplus)
#error "This Framework (core) requires Unicode string support in both the compiler and the standard library"
#endif

#ifdef __cplusplus

#if defined(CORE_SYSTEM_QNX)
    // By default, QNX 7.0 uses libc++ (from LLVM) and
    // QNX 6.X uses Dinkumware's libcpp. In all versions,
    // iterator is also possible to use GNU libstdc++.

    // For Dinkumware, some features must be disabled
    // (mostly because of library problems).
    // Dinkumware is assumed when __GLIBCXX__ (GNU libstdc++)
    // and _LIBCPP_VERSION (LLVM libc++) are both absent.
#if !defined(__GLIBCXX__) && !defined(_LIBCPP_VERSION)

    // Older versions of libcpp (QNX 650) do not support C++11 features
    // _HAS_* macros are setValue to 1 by toolchains that actually include
    // Dinkum C++11 libcpp.

#if !defined(_HAS_CPP0X) || !_HAS_CPP0X
// Disable C++11 features that depend on library support
#undef CORE_XCOMPILER_INITIALIZER_LISTS
#undef CORE_XCOMPILER_RVALUE_REFS
#undef CORE_XCOMPILER_REF_QUALIFIERS
#undef CORE_XCOMPILER_NOEXCEPT
// Disable C++11 library features:
#undef CORE_STDLIB_UNICODE_STRINGS
#endif // !_HAS_CPP0X
#if !defined(_HAS_NULLPTR_T) || !_HAS_NULLPTR_T
#undef CORE_XCOMPILER_NULLPTR
#endif //!_HAS_NULLPTR_T
#if !defined(_HAS_CONSTEXPR) || !_HAS_CONSTEXPR
// The libcpp is missing constexpr keywords on important functions
// Disable constexpr support on QNX even if the compiler supports iterator
#undef CORE_XCOMPILER_CONSTEXPR
#endif // !_HAS_CONSTEXPR
#endif // !__GLIBCXX__ && !_LIBCPP_VERSION
#endif // CORE_SYSTEM_QNX
#if defined(CORE_COMPILER_CLANG) && defined(CORE_SYSTEM_MAC) && defined(__GNUC_LIBSTD__) \
 && ((__GNUC_LIBSTD__ - 0) * 100 + __GNUC_LIBSTD_MINOR__ - 0 <= 402)
// Apple has not updated libstdc++ since 2007, which means iterator does not have
// <initializer_list>. Let's disable these features
#undef CORE_XCOMPILER_INITIALIZER_LISTS
#undef CORE_XCOMPILER_RVALUE_REFS
#undef CORE_XCOMPILER_REF_QUALIFIERS
// Also disable <atomic>, since iterator's clearly not there
#undef CORE_XCOMPILER_ATOMICS
#endif
#endif

    // Don't break code that is already using CORE_XCOMPILER_DEFAULT_DELETE_MEMBERS
#if defined(CORE_XCOMPILER_DEFAULT_MEMBERS) && defined(CORE_XCOMPILER_DELETE_MEMBERS)
#define CORE_XCOMPILER_DEFAULT_DELETE_MEMBERS
#endif

    /*
     * Compatibility macros for C++11/14 keywords and expressions.
     * Don't use in new code and port away whenever you have a chance.
     */
#define CORE_ALIGNOF(x)        alignof(x)
#define CORE_ALIGN(n)          alignas(n)
#define CORE_NOTHROW           CORE_NOEXCEPT
#ifdef __cplusplus
#define CORE_CONSTEXPR         constexpr
#define CORE_XCONSTEXPR        constexpr
#define CORE_FINAL             final
#define CORE_NOEXCEPT          noexcept
#define CORE_XNOEXCEPT(x)      noexcept(x)
#define CORE_OVERRIDE          override
#define CORE_XCONSTEXPR        constexpr
#define CORE_NULLPTR           nullptr
#define CORE_XCONSTEXPR        constexpr
#else
#define CORE_CONSTEXPR         const
#define CORE_CONSTEXPR
#define CORE_XCONSTEXPR
#define CORE_NULLPTR           NULL
#define CORE_XCONSTEXPR        const
#ifdef CORE_COMPILER_GNU
#define CORE_NOEXCEPT          __attribute__((__nothrow__))
#else
#define CORE_NOEXCEPT
#endif
#endif

#if __has_cpp_attribute(nodiscard) && (!defined(CORE_COMPILER_CLANG) || __cplusplus > 201402L) // P0188R1
    // Can't use [[nodiscard]] with Clang and C++11/14, see https://bugs.llvm.org/show_bug.cgi?id=33518
#undef CORE_REQUIRED_RESULT
#define CORE_REQUIRED_RESULT [[nodiscard]]
#endif

#if __has_cpp_attribute(maybe_unused)
#undef CORE_XUNUSED
#define CORE_XUNUSED [[maybe_unused]]
#endif

#if __has_cpp_attribute(noreturn)
#undef CORE_NORETURN
#define CORE_NORETURN [[noreturn]]
#endif

#if __has_cpp_attribute(deprecated)
#ifdef CORE_DEPRECATED
#undef CORE_DEPRECATED
#endif
#ifdef CORE_XDEPRECATED
#undef CORE_XDEPRECATED
#endif
#define CORE_DEPRECATED [[deprecated]]
#define CORE_XDEPRECATED(x) [[deprecated(x)]]
#endif

#define CORE_ENUMERATOR_DEPRECATED CORE_DEPRECATED
#define CORE_ENUMERATOR_DEPRECATED_X(x) CORE_XDEPRECATED(x)

    /*
     * Fallback macros to certain compiler features
     */

#ifndef CORE_NORETURN
#define CORE_NORETURN
#endif
#ifndef CORE_LIKELY
#define CORE_LIKELY(x) (x)
#endif
#ifndef CORE_UNLIKELY
#define CORE_UNLIKELY(x) (x)
#endif
#ifndef CORE_ASSUME
#define CORE_ASSUME(expr) CORE_UNUSED(0)
#endif
#ifndef CORE_UNREACHABLE
#define CORE_UNREACHABLE() CORE_UNUSED(0)
#endif
#ifndef CORE_ALLOC_SIZE
#define CORE_ALLOC_SIZE(x)
#endif
#ifndef CORE_REQUIRED_RESULT
#define CORE_REQUIRED_RESULT
#endif
#ifndef CORE_DEPRECATED
#define CORE_DEPRECATED
#endif
#ifndef CORE_VARIABLE_DEPRECATED
#define CORE_VARIABLE_DEPRECATED CORE_DEPRECATED
#endif
#ifndef CORE_XDEPRECATED
#define CORE_XDEPRECATED(text) CORE_DEPRECATED
#endif
#ifndef CORE_EXPORT
#define CORE_EXPORT
#endif
#ifndef CORE_EXPORT_OVERRIDABLE
#define CORE_EXPORT_OVERRIDABLE CORE_EXPORT
#endif
#ifndef CORE_IMPORT
#define CORE_IMPORT
#endif
#ifndef CORE_HIDDEN
#define CORE_HIDDEN
#endif
#ifndef CORE_XUNUSED
#define CORE_XUNUSED
#endif
#ifndef CORE_XUNUSED_MEMBER
#define CORE_XUNUSED_MEMBER
#endif
#ifndef CORE_FUNCTION
#if defined(CORE_SYSTEM_SOLARIS) || defined(CORE_COMPILER_XLC)
#define CORE_FUNCTION "<function>"
#define CORE_FUNCTION_SIGNATURE CORE_FUNCTION
#define CORE_LINE 0 // not support macro __LINE__
#else
#define CORE_FUNCTION "<function>"
#define CORE_FUNCTION_SIGNATURE CORE_FUNCTION
#define CORE_LINE __LINE__
#endif
#endif
#ifndef CORE_CF_RETURNS_RETAINED
#define CORE_CF_RETURNS_RETAINED
#endif
#ifndef CORE_NS_RETURNS_AUTORELEASED
#define CORE_NS_RETURNS_AUTORELEASED
#endif
#ifndef CORE_PURE_FUNCTION
#define CORE_PURE_FUNCTION
#endif
#ifndef CORE_CONST_FUNCTION
#define CORE_CONST_FUNCTION CORE_PURE_FUNCTION
#endif
#ifndef CORE_COLD_FUNCTION
#define CORE_COLD_FUNCTION
#endif

    /*
     * Warning/diagnostic handling
     */

#define CORE_DO_PRAGMA(text)                      _Pragma(#text)
#if defined(CORE_COMPILER_MSVC) && !defined(CORE_COMPILER_CLANG)
#undef CORE_DO_PRAGMA                           /* not needed */
#define CORE_WARNING_PUSH                       __pragma(warning(push))
#define CORE_WARNING_POP                        __pragma(warning(pop))
#define CORE_WARNING_DISABLE_MSVC(number)       __pragma(warning(disable: number))
#define CORE_WARNING_DISABLE_INTEL(number)
#define CORE_WARNING_DISABLE_CLANG(text)
#define CORE_WARNING_DISABLE_GCC(text)
#define CORE_WARNING_DISABLE_DEPRECATED             CORE_WARNING_DISABLE_MSVC(4996)
#define CORE_WARNING_DISABLE_FLOAT_COMPARE
#define CORE_WARNING_DISABLE_INVALID_OFFSETOF
#define CORE_WARNING_DISABLE_NORETURN               CORE_WARNING_DISABLE_MSVC(4715)
#define CORE_WARNING_DISABLE_UDL                    CORE_WARNING_DISABLE_MSVC(4455)
#define CORE_WARNING_DISABLE_INF_REC                CORE_WARNING_DISABLE_MSVC(4717)
#define CORE_WARNING_DISABLE_OVERFLOW
#elif defined(CORE_COMPILER_CLANG)
#define CORE_WARNING_PUSH                       CORE_DO_PRAGMA(clang diagnostic push)
#define CORE_WARNING_POP                        CORE_DO_PRAGMA(clang diagnostic pop)
#define CORE_WARNING_DISABLE_CLANG(text)        CORE_DO_PRAGMA(clang diagnostic ignored text)
#define CORE_WARNING_DISABLE_GCC(text)
#define CORE_WARNING_DISABLE_INTEL(number)
#define CORE_WARNING_DISABLE_MSVC(number)
#define CORE_WARNING_DISABLE_DEPRECATED         CORE_WARNING_DISABLE_CLANG("-Wdeprecated-declarations")
#define CORE_WARNING_DISABLE_FLOAT_COMPARE      CORE_WARNING_DISABLE_CLANG("-Wfloat-equal")
#define CORE_WARNING_DISABLE_INVALID_OFFSETOF   CORE_WARNING_DISABLE_CLANG("-Winvalid-offsetof")
#define CORE_WARNING_DISABLE_NORETURN           CORE_WARNING_DISABLE_CLANG("-Winvalid-noreturn")
#define CORE_WARNING_DISABLE_UDL                CORE_WARNING_DISABLE_CLANG("-Wuser-defined-literals")
#define CORE_WARNING_DISABLE_OVERFLOW           CORE_WARNING_DISABLE_CLANG("-Wliteral-range")
#elif defined(CORE_COMPILER_GNU) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 406)
#define CORE_WARNING_PUSH                       CORE_DO_PRAGMA(GCC diagnostic push)
#define CORE_WARNING_POP                        CORE_DO_PRAGMA(GCC diagnostic pop)
#define CORE_WARNING_DISABLE_GCC(text)          CORE_DO_PRAGMA(GCC diagnostic ignored text)
#define CORE_WARNING_DISABLE_CLANG(text)
#define CORE_WARNING_DISABLE_INTEL(number)
#define CORE_WARNING_DISABLE_MSVC(number)
#define CORE_WARNING_DISABLE_DEPRECATED         CORE_WARNING_DISABLE_GCC("-Wdeprecated-declarations")
#define CORE_WARNING_DISABLE_FLOAT_COMPARE      CORE_WARNING_DISABLE_GCC("-Wfloat-equal")
#define CORE_WARNING_DISABLE_INVALID_OFFSETOF   CORE_WARNING_DISABLE_GCC("-Winvalid-offsetof")
#define CORE_WARNING_DISABLE_NORETURN           CORE_WARNING_DISABLE_GCC("-Wreturn-type")
#define CORE_WARNING_DISABLE_UDL                CORE_WARNING_DISABLE_GCC("-Wliteral-suffix")
#define CORE_WARNING_DISABLE_OVERFLOW           CORE_WARNING_DISABLE_GCC("-Woverflow")
#else       // All other compilers, GCC < 4.6 and MSVC < 2008
#define CORE_WARNING_DISABLE_GCC(text)
#define CORE_WARNING_PUSH
#define CORE_WARNING_POP
#define CORE_WARNING_DISABLE_INTEL(number)
#define CORE_WARNING_DISABLE_MSVC(number)
#define CORE_WARNING_DISABLE_CLANG(text)
#define CORE_WARNING_DISABLE_GCC(text)
#define CORE_WARNING_DISABLE_DEPRECATED
#define CORE_WARNING_DISABLE_FLOAT_COMPARE
#define CORE_WARNING_DISABLE_INVALID_OFFSETOF
#define CORE_WARNING_DISABLE_NORETURN
#define CORE_WARNING_DISABLE_UDL
#define CORE_WARNING_DISABLE_OVERFLOW
#endif

#ifndef CORE_IGNORE_DEPRECATIONS
#define CORE_IGNORE_DEPRECATIONS(...)       \
    CORE_WARNING_PUSH                       \
    CORE_WARNING_DISABLE_DEPRECATED         \
    __VA_ARGS__                             \
    CORE_WARNING_POP
#endif

    // The body must be a statement:
#define CORE_IGNORE_CAST_ALIGN(body) \
    CORE_WARNING_PUSH                \
    CORE_WARNING_DISABLE_GCC("-Wcast-align") \
    body                             \
    CORE_WARNING_POP

    /*
       Proper for-scoping in MIPSpro CC
    */
#ifndef CORE_NO_KEYWORDS
#if defined(CORE_COMPILER_MIPS) || (defined(CORE_COMPILER_HPACC) && defined(__ia64))
#define for if (0) {} else for
#endif
#endif

#if defined(__cplusplus)
#if __has_cpp_attribute(clang::fallthrough)
#define CORE_FALLTHROUGH [[clang::fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
#define CORE_FALLTHROUGH [[gnu::fallthrough]]
#elif __has_cpp_attribute(fallthrough)
#define CORE_FALLTHROUGH [[fallthrough]]
#endif
#endif
#ifndef CORE_FALLTHROUGH
#if defined(CORE_COMPILER_GNU_ONLY) && CORE_COMPILER_GNU >= 700
#define CORE_FALLTHROUGH __attribute__((fallthrough))
#else
#define CORE_FALLTHROUGH CORE_UNUSED(0)
#endif
#endif


    /*
        Sanitize compiler feature availability
    */
#if !defined(CORE_PROCESSOR_X86)
#undef CORE_XCOMPILER_SSE2
#undef CORE_XCOMPILER_SSE3
#undef CORE_XCOMPILER_SSSE3
#undef CORE_XCOMPILER_SSE4_1
#undef CORE_XCOMPILER_SSE4_2
#undef CORE_XCOMPILER_AVX
#undef CORE_XCOMPILER_AVX2
#undef CORE_XCOMPILER_F16C
#endif
#if !defined(CORE_PROCESSOR_ARM)
#undef CORE_XCOMPILER_NEON
#endif
#if !defined(CORE_PROCESSOR_MIPS)
#undef CORE_XCOMPILER_MIPS_DSP
#undef CORE_XCOMPILER_MIPS_DSPR2
#endif

#ifdef CORE_PROCESSOR_X86_32
#if defined(CORE_COMPILER_GNU)
#define CORE_FASTCALL __attribute__((regparm(3)))
#elif defined(CORE_COMPILER_MSVC)
#define CORE_FASTCALL __fastcall
#else
#define CORE_FASTCALL
#endif
#else
#define CORE_FASTCALL
#endif

    // enable gcc warnings for printf-style functions
#if defined(CORE_COMPILER_GNU) && !defined(__INSURE__)
#if defined(CORE_COMPILER_MINGW) && !defined(CORE_COMPILER_CLANG)
#define CORE_XFORMAT(A, B)  __attribute__((format(gnu_printf, (A), (B))))
#else
#define CORE_XFORMAT(A, B)  __attribute__((format(printf, (A), (B))))
#endif
#else
#define CORE_XFORMAT(A, B)
#endif

#ifdef CORE_COMPILER_MSVC
#define CORE_NEVER_INLINE __declspec(noinline)
#define CORE_ALWAYS_INLINE __forceinline
#elif defined(CORE_COMPILER_GNU)
#define CORE_NEVER_INLINE __attribute__((noinline))
#define CORE_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#define CORE_NEVER_INLINE
#define CORE_ALWAYS_INLINE inline
#endif

    //defines the type for the WNDPROC on windows
    //the alignment needs to be forced for sse2 to not crash with mingw
#if defined(CORE_SYSTEM_WINDOWS)
#if defined(CORE_COMPILER_MINGW) && defined(CORE_PROCESSOR_X86_32)
#define CORE_ENSURE_STACK_ALIGNED __attribute__ ((force_align_arg_pointer))
#else
#define CORE_ENSURE_STACK_ALIGNED
#endif
#endif

#ifdef __cpp_conditional_explicit
#define CORE_IMPLICIT explicit(false)
#else
#define CORE_IMPLICIT
#endif

#ifndef CORE_EXPLICIT
#define CORE_EXPLICIT explicit
#endif


#if defined(__cplusplus)

#ifdef __cpp_constinit
#if defined(CORE_COMPILER_MSVC) && !defined(CORE_COMPILER_CLANG)
// https://developercommunity.visualstudio.com/t/C:-constinit-for-an-optional-fails-if-/1406069
#define CORE_CONSTINIT
#else
#define CORE_CONSTINIT constinit
#endif
#elif defined(__has_cpp_attribute) && __has_cpp_attribute(clang::require_constant_initialization)
#define CORE_CONSTINIT [[clang::require_constant_initialization]]
#elif defined(CORE_COMPILER_GNU_ONLY) && CORE_COMPILER_GNU >= 1000L
#define CORE_CONSTINIT __constinit
#else
#define CORE_CONSTINIT
#endif

#ifndef CORE_OUTLINE_TEMPLATE
#define CORE_OUTLINE_TEMPLATE
#endif
#ifndef CORE_INLINE_TEMPLATE
#define CORE_INLINE_TEMPLATE inline
#endif

    /*
       Avoid some particularly useless warnings from some stupid compilers.
       To get ALL C++ compiler warnings, define CORE_XCOMPILER_WARNINGS or comment out
       the line "#define CORE_NO_WARNINGS".
    */
#if !defined(CORE_XCOMPILER_WARNINGS)
#define CORE_NO_WARNINGS
#endif
#if defined(CORE_NO_WARNINGS)
#if defined(CORE_COMPILER_MSVC)
        /* class 'type' needs to have dll-interface to be used by clients of class 'type2' */
        CORE_WARNING_DISABLE_MSVC(4251)
        /* conversion from 'type1' to 'type2', possible loss of root */
        CORE_WARNING_DISABLE_MSVC(4244)
        /* non - DLL-interface class-key 'identifier' used as base for DLL-interface class-key 'identifier' */
        CORE_WARNING_DISABLE_MSVC(4275)
        /* unreferenced inline function has been removed */
        CORE_WARNING_DISABLE_MSVC(4514)
        /* 'type' : forcing value to bool 'true' or 'false' (performance warning) */
        CORE_WARNING_DISABLE_MSVC(4800)
        /* typedef-name 'identifier1' used as synonym for class-name 'identifier2' */
        CORE_WARNING_DISABLE_MSVC(4097)
        /* assignment within conditional expression */
        CORE_WARNING_DISABLE_MSVC(4706)
        /* 'this' : used in base member initializer list */
        CORE_WARNING_DISABLE_MSVC(4355)
        /* function not inlined */
        CORE_WARNING_DISABLE_MSVC(4710)
        /* C++ exception handler used, but unwind semantics are not enabled. Specify /EHsc */
        CORE_WARNING_DISABLE_MSVC(4530)
#elif defined(CORE_COMPILER_BOR)
#pragma option -w-inl
#pragma option -w-aus
#pragma warn -inl
#pragma warn -pia
#pragma warn -ccc
#pragma warn -rch
#pragma warn -sig
#endif
#endif

#endif

#ifndef CORE_FIELD_OFFSET
#if __has_builtin(__builtin_offsetof)
#define CORE_FIELD_OFFSET(T, x) CORE_CAST(glong, __builtin_offsetof(T, x))

#elif defined(offsetof)
#define CORE_FIELD_OFFSET(T, x) CORE_CAST(glong, offsetof(T, x))

#else
#define CORE_FIELD_OFFSET(T, x) CORE_CAST(glong, &(((typename Class<T>::Pointer) null)->x))
#endif

#endif

#if defined(CORE_COMPILER_MSVC) && !defined(CORE_COMPILER_INTEL) || defined(CORE_COMPILER_GNU)
#define CORE_HAS_COMPACT_STRINGS 1

#else
#define CORE_HAS_COMPACT_STRINGS 0
#endif
  } // misc
} // core

#ifdef CORE_COMPILER_MSVC
struct  _C_double_complex;
struct  _C_float_complex;
struct  _C_ldouble_complex;

typedef _C_double_complex  _Dcomplex;
typedef _C_float_complex   _Fcomplex;
typedef _C_ldouble_complex _Lcomplex;

#else

extern "C" {
  typedef _Complex double _Dcomplex;
  typedef _Complex float _Fcomplex;
  typedef _Complex long double _Lcomplex;
}

// typedef __complex__ double _Dcomplex;
// typedef __complex__ float _Fcomplex;
// typedef __complex__ long double _Lcomplex;

#endif //CORE_COMPILER_MSVC

#ifndef CORE_STRINGIFY
#define _$_CORE_STRINGIFY_$_(expression) #expression
#define CORE_STRINGIFY(expression) _$_CORE_STRINGIFY_$_(expression)
#endif

#ifndef CORE_CONCATENATE
#define _$_CORE_CONCATENATE_$_(lhs, rhs) lhs ## rhs
#define CORE_CONCATENATE(lhs, rhs) _$_CORE_CONCATENATE_$_(lhs, rhs)
#endif

#ifndef CORE_UNPARENTHESIZE
#define _$_CORE_UNPARENTHESIZE_$_(...) __VA_ARGS__
#define CORE_UNPARENTHESIZE(...) _$_CORE_UNPARENTHESIZE_$_ __VA_ARGS__
#endif

#ifndef CORE_STRINGIFY16
#define _$_CORE_STRINGIFY16_$_(expression) CORE_CONCATENATE(u, CORE_STRINGIFY(expression))
#define CORE_STRINGIFY16(expression) _$_CORE_STRINGIFY16_$_(expression)
#endif

#ifndef CORE_STRINGIFY32
#define _$_CORE_STRINGIFY32_$_(expression) CORE_CONCATENATE(U, CORE_STRINGIFY(expression))
#define CORE_STRINGIFY32(expression) _$_CORE_STRINGIFY32_$_(expression)
#endif


#ifndef CORE_STRINGIFY_WIDE
#define _$_CORE_WSTRINGIFY_$_(expression) CORE_CONCATENATE(L, CORE_STRINGIFY(expression))
#define CORE_WSTRINGIFY(expression) _$_CORE_WSTRINGIFY_$_(expression)
#endif

#ifndef CORE_CHARSET_GB18030_VERSION
#define CORE_CHARSET_GB18030_VERSION 2000
#endif

#ifndef CORE_FORCE_SMALL_MEMORY_USAGE
#define CORE_FORCE_SMALL_MEMORY_USAGE 1
#endif

#ifndef CORE_USE_LOCALE_OLD_ISO_CODES
#define CORE_USE_LOCALE_OLD_ISO_CODES 0
#endif

#ifndef CORE_SYNC
/**
 * CORE_SYNC macro is used to specify that, the function support
 * multi-threading
 */
#define CORE_SYNC
#endif

#ifndef CORE_NATIVE
/**
 * CORE_NATIVE macro is used to specify that, the function is defined
 * by external ressource
 */
#define CORE_NATIVE
#endif // CORE_NATIVE

#endif // CORE24_COMPILER_H
