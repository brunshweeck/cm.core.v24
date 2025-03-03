//
// Created by brunshweeck on 8 juin 2024.
//

#ifndef CORE24_PROCESSOR_H
#define CORE24_PROCESSOR_H

#include <core/misc/OS.h>

namespace core {
    namespace misc {
        /**
         * This file uses preprocessor #defines to setValue various CORE_PROCESSOR_*
         * <p>
         * #defines based on the following patterns:
         * <ol>
         * <li> CORE_PROCESSOR_{FAMILY}
         * <li> CORE_PROCESSOR_{FAMILY}_{VARIANT}
         * <li> CORE_PROCESSOR_{FAMILY}_{REVISION}
         * </ol>
         *
         * <p>
         * The first is always defined. Defines for the various revisions/variants are
         * optional and usually dependent on how the compiler was invoked. Variants
         * that are a superset of another should have a defined for the superset.
         *
         * <p>
         * In addition to the processor family, variants, and revisions, we also setValue
         * CORE_BYTE_ORDER appropriately for the target processor. For bi-endian
         * processors, we try to auto-detect the byte order using the __BIG_ENDIAN__,
         * __LITTLE_ENDIAN__, or __BYTE_ORDER__ preprocessor macros.
        */

#if defined(__ORDER_BIG_ENDIAN__)
#define CORE_BIG_ENDIAN __ORDER_BIG_ENDIAN__
#else
#define CORE_BIG_ENDIAN 4321
#endif


#if defined(__ORDER_LITTLE_ENDIAN__)
#define CORE_LITTLE_ENDIAN __ORDER_LITTLE_ENDIAN__
#else
#define CORE_LITTLE_ENDIAN 1234
#endif

        /**
         * Alpha family, no revisions or variants
         * Alpha is bi-endian, use endianness auto-detection
         * implemented below.
         */
#if defined(__alpha__) || defined(_M_ALPHA)
#define CORE_PROCESSOR_ALPHA

/**
 * ARM family, known revisions: V5, V6, V7, V8
 * ARM is bi-endian, detect using __ARMEL__ or __ARMEB__, falling back to
 * auto-detection implemented below.
 */
#elif defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(_M_ARM64) || defined(__aarch64__) || defined(__ARM64__)

#if defined(__aarch64__) || defined(__ARM64__) || defined(_M_ARM64)
#define CORE_PROCESSOR_ARM_64
#define CORE_PROCESSOR_WORDSIZE 8

#else
#define CORE_PROCESSOR_ARM_32
#endif

#if defined(__ARM_ARCH) && __ARM_ARCH > 1
#define CORE_PROCESSOR_ARM __ARM_ARCH

#elif defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM > 1
#define CORE_PROCESSOR_ARM __TARGET_ARCH_ARM

#elif defined(_M_ARM) && _M_ARM > 1
#define CORE_PROCESSOR_ARM _M_ARM

#elif defined(__ARM64_ARCH_8__)  || defined(__aarch64__) || defined(__ARMv8__)  || defined(__ARMv8_A__)  || defined(_M_ARM64)
#define CORE_PROCESSOR_ARM 8

#elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__) || defined(_ARM_ARCH_7) || defined(__CORE_CORTEXA__)
#define CORE_PROCESSOR_ARM 7

#elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6ZK__) || defined(__ARM_ARCH_6M__)
#define CORE_PROCESSOR_ARM 6

#elif defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_5TE__)
#define CORE_PROCESSOR_ARM 5

#else
#define CORE_PROCESSOR_ARM 0
#endif

#if CORE_PROCESSOR_ARM >= 8
#define CORE_PROCESSOR_ARM_V8
#endif

#if CORE_PROCESSOR_ARM >= 7
#define CORE_PROCESSOR_ARM_V7
#endif

#if CORE_PROCESSOR_ARM >= 6
#define CORE_PROCESSOR_ARM_V6
#endif

#if CORE_PROCESSOR_ARM >= 5
#define CORE_PROCESSOR_ARM_V5

#else
#error "Unsupported ARM architecture: ARM architecture too old"
#endif

#if defined(__ARMEL__) || defined(_M_ARM64)
#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN

#elif defined(__ARMEB__)
#define CORE_BYTE_ORDER CORE_BIG_ENDIAN

#endif

        /**
         * AVR32 family, no revisions or variants
         * AVR32 is big-endian.
         */
#elif defined(__avr32__)
#define CORE_PROCESSOR_AVR32
#define CORE_BYTE_ORDER CORE_BIG_ENDIAN

/**
 * Blackfin family, no revisions or variants
 * Blackfin is little-endian.
 */
#elif defined(__bfin__)
#define CORE_PROCESSOR_BLACKFIN
#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN

/**
 * PA-RISC family, no revisions or variants
 * PA-RISC is big-endian.
 */
#elif defined(__hppa__)
#define CORE_PROCESSOR_HPPA
#define CORE_BYTE_ORDER CORE_BIG_ENDIAN

/**
 * x86 family, known variants: 32/64 bits
 * x86 is little-endian.
 */
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86)
#define CORE_PROCESSOR_X86_32
#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN
#define CORE_PROCESSOR_WORDSIZE   4

/**
 * We define CORE_PROCESSOR_X86 == 6 for anything above a equivalent or better
 * than a Pentium Pro (the processor whose architecture was called P6) or an
 * Athlon.
 * All processors since the Pentium III and the Athlon 4 have SSE support, so
 * we use that to detect. That leaves the original Athlon, Pentium Pro and
 * Pentium II.
 */
#if defined(_M_IX86)
#define CORE_PROCESSOR_X86     (_M_IX86/100)

#elif defined(__i686__) || defined(__athlon__) || defined(__SSE__) || defined(__pentiumpro__)
#define CORE_PROCESSOR_X86     6

#elif defined(__i586__) || defined(__k6__) || defined(__pentium__)
#define CORE_PROCESSOR_X86     5

#elif defined(__i486__) || defined(__80486__)
#define CORE_PROCESSOR_X86     4

#else
#define CORE_PROCESSOR_X86     3
#endif

#elif defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(_M_X64)
#define CORE_PROCESSOR_X86       6
#define CORE_PROCESSOR_X86_64
#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN
#define CORE_PROCESSOR_WORDSIZE   8

        /**
         * Itanium (IA-64) family, no revisions or variants
         * Itanium is bi-endian, use endianness auto-detection implemented below.
         */
#elif defined(__ia64) || defined(__ia64__) || defined(_M_IA64)
#define CORE_PROCESSOR_IA64
#define CORE_PROCESSOR_WORDSIZE   8

/**
 * LoongArch family, known variants: 32- and 64-bit
 * LoongArch is little-endian.
 */
#elif defined(__loongarch__)
#define CORE_PROCESSOR_LOONGARCH

#if __loongarch_grlen == 64
#define CORE_PROCESSOR_LOONGARCH_64

#else
#define CORE_PROCESSOR_LOONGARCH_32
#endif

#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN

/**
 * Motorola 68000 family, no revisions or variants
 * M68K is big-endian.
 */
#elif defined(__m68k__)
#define CORE_PROCESSOR_M68K
#define CORE_BYTE_ORDER CORE_BIG_ENDIAN

/**
 * MIPS family, known revisions: I, II, III, IV, 32, 64
 * MIPS is bi-endian, use endianness auto-detection implemented below.
 */
#elif defined(__mips) || defined(__mips__) || defined(_M_MRX000)
#define CORE_PROCESSOR_MIPS

#if defined(_MIPS_ARCH_MIPS1) || (defined(__mips) && __mips - 0 >= 1)
#define CORE_PROCESSOR_MIPS_I
#endif

#if defined(_MIPS_ARCH_MIPS2) || (defined(__mips) && __mips - 0 >= 2)
#define CORE_PROCESSOR_MIPS_II
#endif

#if defined(_MIPS_ARCH_MIPS3) || (defined(__mips) && __mips - 0 >= 3)
#define CORE_PROCESSOR_MIPS_III
#endif

#if defined(_MIPS_ARCH_MIPS4) || (defined(__mips) && __mips - 0 >= 4)
#define CORE_PROCESSOR_MIPS_IV
#endif

#if defined(_MIPS_ARCH_MIPS5) || (defined(__mips) && __mips - 0 >= 5)
#define CORE_PROCESSOR_MIPS_V
#endif

#if defined(_MIPS_ARCH_MIPS32) || defined(__mips32) || (defined(__mips) && __mips - 0 >= 32)
#define CORE_PROCESSOR_MIPS_32
#endif

#if defined(_MIPS_ARCH_MIPS64) || defined(__mips64)
#define CORE_PROCESSOR_MIPS_64
#define CORE_PROCESSOR_WORDSIZE 8
#endif

#if defined(__MIPSEL__)
#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN

#elif defined(__MIPSEB__)
#define CORE_BYTE_ORDER CORE_BIG_ENDIAN
#endif

        /**
         * Power family, known variants: 32- and 64-bit
         * There are many more known variants/revisions that we do not handle/detect.
         * See http://en.wikipedia.org/wiki/Power_Architecture
         * and http://en.wikipedia.org/wiki/File:PowerISA-evolution.svg
         * Power is bi-endian, use endianness auto-detection implemented below.
         */
#elif defined(__ppc__) || defined(__ppc) || defined(__powerpc__) || defined(_ARCH_COM) || defined(_ARCH_PWR) || defined(_ARCH_PPC)  || defined(_M_MPPC) || defined(_M_PPC)
#define CORE_PROCESSOR_POWER

#if defined(__ppc64__) || defined(__powerpc64__) || defined(__64BIT__)
#define CORE_PROCESSOR_POWER_64
#define CORE_PROCESSOR_WORDSIZE 8

#else
#define CORE_PROCESSOR_POWER_32
#endif

        /**
         * RISC-V family, known variants: 32- and 64-bit
         * RISC-V is little-endian.
         */
#elif defined(__riscv)
#define CORE_PROCESSOR_RISCV

#if __riscv_xlen == 64
#define CORE_PROCESSOR_RISCV_64

#else
#define CORE_PROCESSOR_RISCV_32
#endif

#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN

/**
 * S390 family, known variant: S390X (64-bit)
 * S390 is big-endian.
 */
#elif defined(__s390__)
#define CORE_PROCESSOR_S390

#if defined(__s390x__)
#define CORE_PROCESSOR_S390X
#endif

#define CORE_BYTE_ORDER CORE_BIG_ENDIAN

/**
 * SuperH family, optional revision: SH-4A
 * SuperH is bi-endian, use endianness auto-detection implemented below.
 */
#elif defined(__sh__)
#define CORE_PROCESSOR_SH

#if defined(__sh4a__)
#define CORE_PROCESSOR_SH_4A
#endif

        /**
         * SPARC family, optional revision: V9
         * SPARC is big-endian only prior to V9, while V9 is bi-endian with big-endian
         * as the default byte order. Assume all SPARC systems are big-endian.
         */
#elif defined(__sparc__)
#define CORE_PROCESSOR_SPARC

#if defined(__sparc_v9__) || defined(__sparcv9)
#define CORE_PROCESSOR_SPARC_V9
#endif

#if defined(__sparc64__)
#define CORE_PROCESSOR_SPARC_64
#endif

#define CORE_BYTE_ORDER CORE_BIG_ENDIAN

// -- Web Assembly --
#elif defined(__EMSCRIPTEN__)
#define CORE_PROCESSOR_WASM
#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN
#define CORE_PROCESSOR_WORDSIZE 8

#ifdef CORE_XCOMPILER_SSE2
#define CORE_PROCESSOR_X86 6   // enables SIMD support
#define CORE_PROCESSOR_X86_64 // wasm64
#define CORE_PROCESSOR_WASM_64
#endif

#endif

        /*
          NOTE:
          GCC 4.6 added __BYTE_ORDER__, __ORDER_BIG_ENDIAN__, __ORDER_LITTLE_ENDIAN__
          and __ORDER_PDP_ENDIAN__ in SVN r165881. If you are using GCC 4.6 or newer,
          this code will properly detect your target byte order; if you are not, and
          the __LITTLE_ENDIAN__ or __BIG_ENDIAN__ macros are not defined, then this
          code will fail to detect the target byte order.
        */
        // Some processors support either endian format, try to detect, which we are using.
#if !defined(CORE_BYTE_ORDER)
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == CORE_BIG_ENDIAN || __BYTE_ORDER__ == CORE_LITTLE_ENDIAN)
// Reuse __BYTE_ORDER__ as-is, since our CORE_*_ENDIAN #defines match the preprocessor defaults
#define CORE_BYTE_ORDER __BYTE_ORDER__

#elif defined(__BIG_ENDIAN__) || defined(_big_endian__) || defined(_BIG_ENDIAN)
#define CORE_BYTE_ORDER CORE_BIG_ENDIAN

#elif defined(__LITTLE_ENDIAN__) || defined(_little_endian__) || defined(_LITTLE_ENDIAN)
#define CORE_BYTE_ORDER CORE_LITTLE_ENDIAN

#else
#error "Unable to detect Byte order"
#endif

#endif

        /**
         * Size of address and the machine register size. We detect a 64-bit system by:
         * <ul>
         * <li> GCC and compatible compilers (Clang, ICC on OS X and Windows) always define
         * __SIZEOF_POINTER__. This catches all known cases of ILP32 builds on 64-bit processors.
         * <li> Most other Unix compilers define __LP64__ or _LP64 on 64-bit mode (Long and Address 64-bit)
         * <li> If CORE_PROCESSOR_WORDSIZE was defined above, iterator is assumed to match the address size.
        */

#if defined(__SIZEOF_POINTER__)
#define CORE_ADDRESS_SIZE           __SIZEOF_POINTER__

#elif defined(__LP64__) || defined(_LP64)
#define CORE_ADDRESS_SIZE           8

#elif defined(CORE_PROCESSOR_WORDSIZE)
#define CORE_ADDRESS_SIZE           CORE_PROCESSOR_WORDSIZE

#else
#define CORE_ADDRESS_SIZE           4
#endif

        /**
         * Define CORE_PROCESSOR_WORDSIZE to be the size of the machine's word (usually,
         * the size of the register). On some architectures where address could be
         * smaller than the register, the macro is defined above.
         * Falls back to CORE_ADDRESS_SIZE if not setValue explicitly for the platform.
         */
#ifndef CORE_PROCESSOR_WORDSIZE
#define CORE_PROCESSOR_WORDSIZE        CORE_ADDRESS_SIZE
#endif


#if defined(CORE_PROCESSOR_ALPHA)
#define CORE_ARCH "alpha"

#elif defined(CORE_PROCESSOR_ARM_32)
#define CORE_ARCH "arm"

#elif defined(CORE_PROCESSOR_ARM_64)
#define CORE_ARCH "arm64"

#elif defined(CORE_PROCESSOR_AVR32)
#define CORE_ARCH "avr32"

#elif defined(CORE_PROCESSOR_BLACKFIN)
#define CORE_ARCH "bfin"

#elif defined(CORE_PROCESSOR_WASM_64)
#define CORE_ARCH "wasm64"

#elif defined(CORE_PROCESSOR_WASM)
#define CORE_ARCH "wasm"

#elif defined(CORE_PROCESSOR_HPPA)
#define CORE_ARCH "hppa"

#elif defined(CORE_PROCESSOR_X86_32)
#define CORE_ARCH "i386"

#elif defined(CORE_PROCESSOR_X86_64)
#define CORE_ARCH "x86_64"

#elif defined(CORE_PROCESSOR_IA64)
#define CORE_ARCH "ia64"

#elif defined(CORE_PROCESSOR_LOONGARCH_32)
#define CORE_ARCH "loongarch32"

#elif defined(CORE_PROCESSOR_LOONGARCH_64)
#define CORE_ARCH "loongarch64"

#elif defined(CORE_PROCESSOR_M68K)
#define CORE_ARCH "m68k"

#elif defined(CORE_PROCESSOR_MIPS_64)
#define CORE_ARCH "mips64"

#elif defined(CORE_PROCESSOR_MIPS)
#define CORE_ARCH "mips"

#elif defined(CORE_PROCESSOR_POWER_32)
#define CORE_ARCH "power"

#elif defined(CORE_PROCESSOR_POWER_64)
#define CORE_ARCH "power64"

#elif defined(CORE_PROCESSOR_RISCV_32)
#define CORE_ARCH "riscv32"

#elif defined(CORE_PROCESSOR_RISCV_64)
#define CORE_ARCH "riscv64"

#elif defined(CORE_PROCESSOR_S390_X)
#define CORE_ARCH "s390x"

#elif defined(CORE_PROCESSOR_S390)
#define CORE_ARCH "s390"

#elif defined(CORE_PROCESSOR_SH)
#define CORE_ARCH "sh"

#elif defined(CORE_PROCESSORS_SPARC_64)
#define CORE_ARCH "sparc64"

#elif defined(CORE_PROCESSOR_SPARC_V9)
#define CORE_ARCH "sparcv9"

#elif defined(CORE_PROCESSOR_SPARC)
#define CORE_ARCH "sparc"

#else
#define CORE_ARCH "???"
#endif

        // endianness
#if CORE_BYTE_ORDER == CORE_LITTLE_ENDIAN
#define CORE_ARCH_ENDIANNESS "LE"
#elif CORE_BYTE_ORDER == CORE_BIG_ENDIAN
#define CORE_ARCH_ENDIANNESS "BE"
#endif

        // address type
#if defined(CORE_SYSTEM_WIN64)
#define CORE_ARCH_ADDRESS "LLP64"

#elif defined(__LP64__) || CORET_ADDRESS_SIZE + 0 == 8
#define CORE_ARCH_ADDRESS "LP64"

#else
#define CORE_ARCH_ADDRESS "ILP32"
#endif

#if defined(__ARM_EABI__) || defined(__mips_eabi)
#define CORE_ARCH_ABI1 "-eabi"

#elif defined(_MIPS_SIM)
#if _MIPS_SIM == _ABIO32

#define CORE_ARCH_ABI1 "-o32"
#elif _MIPS_SIM == _ABIN32

#define CORE_ARCH_ABI1 "-n32"
#elif _MIPS_SIM == _ABI64

#define CORE_ARCH_ABI1 "-n64"
#elif _MIPS_SIM == _ABIO64

#define CORE_ARCH_ABI1 "-o64"
#endif

#else
#define CORE_ARCH_ABI1 ""
#endif

#if defined(__ARM_PCS_VFP) || defined(__mips_hard_float)
// Use "-hardfloat" for platforms that usually have no FPUs
// (and for the platforms which had "-hardfloat" before we established the rule)
#define CORE_ARCH_ABI2 "-hardfloat"

#elif defined(_SOFT_FLOAT)
// Use "-softfloat" for core_architectures that usually have FPUs
#define CORE_ARCH_ABI2 "-softfloat"

#else
#define CORE_ARCH_ABI2 ""
#endif

#define CORE_ARCH_ABI CORE_ARCH_ABI1 CORE_ARCH_ABI2
#define CORE_ARCH_FULL CORE_ARCH "-" CORE_ARCH_ENDIANNESS "-" CORE_ARCH_ADDRESS CORE_ARCH_ABI
    } // misc
} // core

#endif //CORE24_PROCESSOR_H
