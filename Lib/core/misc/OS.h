//
// Created by bruns on 04/05/2024.
//

#ifndef CORE24_OS_H
#define CORE24_OS_H

namespace core {
    namespace misc {
        /*
         * This file is created to help any user, that needs to know
         * current operating system information, in instantaneous method.
         */


        /**
         * The operating system (OS), must be one of: CORE_SYSTEM_?
         * (where ? represent the name of system)
         *
         * @li DARWIN  - Any Darwin System (macOS, iOS, watchOS, TvOS, ...)
         * @li MACOS   - MarcOS
         * @li IOS     - iOS
         * @li WATCHOS - watchOS
         * @li TVOS    - TvOS
         * @li WIN32   - Windows 32/64-bits System (Windows 2000, Windows XP, Windows 7, Windows Servers, ...)
         * @li WINRT   - Windows Runtime
         * @li CYGWIN  - Cygwin
         * @li SOLARIS - Sun system
         * @li HPUX    - HP-UX
         * @li LINUX   - Linux (has variants)
         * @li FREEBSD - FreeBSD (has variants)
         * @li NETBSD  - NetBSD
         * @li OPENBSD - OpenBSD
         * @li INTERIX - Interix
         * @li AIX     - AIX
         * @li HURD    - GNU Hurd
         * @li QNX     - QNX (has variants)
         * @li LYNX    - LynxOS
         * @li BSD4    - Any BSD 4.4 system
         * @li UNIX    - Any UNIX BSD/SYSV system
         * @li ANDROID - Android platform
         * @li HAIKU   - Haiku
         * @li WEBOS   - LG WebOS
         *
         * The following operating systems have variants:
         * @li LINUX   - both CORE_SYSTEM_LINUX and CORE_SYSTEM_ANDROID are defined when building for Android.
         *             - only CORE_SYSTEM_LINUX is defined if building for other Linux systems.
         * @li MACOS   - both CORE_SYSTEM_BSD4 and CORE_SYSTEM_IOS are defined when building for iOS
         *             - both CORE_SYSTEM_BSD4 and CORE_SYSTEM_MACOS are defined when building for macOS
         * @li FREEBSD - CORE_SYSTEM_FREEBSD is defined only when building for FreeBSD with a BSD userland
         *             - CORE_SYSTEM_FREEBSD_KERNEL is always defined on FreeBSD, even if the userland is from GNU
         */


#if defined(__APPLE__) && defined(__GNUC__) || defined(__xlC__) || defined(__xlc__)
#include <TargetConditionals.h>

#if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#define CORE_SYSTEM_DARWIN

#ifdef __LP64__
#define CORE_SYSTEM_DARWIN64
#else
#define CORE_SYSTEM_DARWIN32
#endif

#if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#define CORE_SYSTEM_UIKIT

#if defined(TARGET_OS_WATCH) && TARGET_OS_WATCH
#define CORE_SYSTEM_WATCHOS
#elif defined(TARGET_OS_TV) && TARGET_OS_TV
#define CORE_SYSTEM_TVOS
#else
// TARGET_OS_IOS is only available in newer SDKs,
// so assume any other iOS-based platform is iOS for now
#define CORE_SYSTEM_IOS
#endif

#else
// TARGET_OS_OSX is only available in newer SDKs,
// so assume any non iOS-based platform is macOS for now
#define CORE_SYSTEM_MACOS
#endif

#endif
#elif defined(__WEBOS__)
#define CORE_SYSTEM_WEBOS

#elif defined(__ANDROID__) || defined(ANDROID)
    #define CORE_SYSTEM_ANDROID
#define CORE_SYSTEM_LINUX

#elif defined(__CYGWIN__)
#define CORE_SYSTEM_CYGWIN

#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#define CORE_SYSTEM_WIN64
#define CORE_SYSTEM_WIN32

#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#if defined(WINAPI_FAMILY)
#ifndef WINAPI_FAMILY_PC_APP
#define WINAPI_FAMILY_PC_APP WINAPI_FAMILY_APP
#endif
#if defined(WINAPI_FAMILY_PHONE_APP) && WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
#define CORE_SYSTEM_WINRT
#elif WINAPI_FAMILY==WINAPI_FAMILY_PC_APP
#define CORE_SYSTEM_WINRT
#else
#define CORE_SYSTEM_WIN32
#endif
#else
#define CORE_SYSTEM_WIN32
#endif
#elif defined(__sun) || defined(sun)
#define CORE_SYSTEM_SOLARIS

#elif defined(hpux) || defined(__hpux)
#define CORE_SYSTEM_HUPX

#elif defined(__native_client__)
#define CORE_SYSTEM_NACL

#elif defined(__EMSCRIPTEN__)
#define CORE_SYSTEM_WASM

#elif defined(__linux__) || defined(__linux)
#define CORE_SYSTEM_LINUX

#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#define CORE_SYSTEM_FREEBSD
#define CORE_SYSTEM_BSD4

#elif defined(__NetBSD__)
#define CORE_SYSTEM_NETBSD
#define CORE_SYSTEM_BSD4

#elif defined(__OpenBSD__)
#define CORE_SYSTEM_OPENBSD
#define CORE_SYSTEM_BSD4

#elif defined(__INTERIX)
#define CORE_SYSTEM_INTERIX
#define CORE_SYSTEM_BSD4

#elif defined(_AIX)
#define CORE_SYSTEM_AIX

#elif defined(__Lynx__)
#define CORE_SYSTEM_LYNX

#elif defined(__GNU__)
#define CORE_SYSTEM_HURD

#elif defined(__QNXNTO__)
#define CORE_SYSTEM_QNX

#elif defined(__INTEGRITY)
#define CORE_SYSTEM_INTEGITY

#elif defined(__rtems__)
#define CORE_SYSTEM_RTERMS

#elif defined(VXWORKS)
#define CORE_SYSTEM_VXWORKS

#elif defined(__HAIKU__)
#define CORE_SYSTEM_HAIKU

#elif defined(__MAKEDEPEND__)

#endif

#if defined(CORE_SYSTEM_WIN32) || defined(CORE_SYSTEM_WIN64) || defined(CORE_SYSTEM_WINRT)
#define CORE_SYSTEM_WINDOWS
#define CORE_SYSTEM_WIN
#endif

#ifdef CORE_SYSTEM_DARWIN
#define CORE_SYSTEM_MAC
#endif

#ifdef CORE_SYSTEM_DARWIN32
#define CORE_SYSTEM_MAC32
#endif

#ifdef CORE_SYSTEM_DARWIN64
#define CORE_SYSTEM_MAC64
#endif

#ifdef CORE_SYSTEM_MACOS
#define CORE_SYSTEM_MACX
#define CORE_SYSTEM_OSX
#endif

#ifdef CORE_SYSTEM_DARWIN
#include <Availability.h>
#include <AvailabilityMacros.h>

#ifdef CORE_SYSTEM_MACOS
#if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_6
#undef __MAC_OS_X_VERSION_MIN_REQUIRED
#define __MAC_OS_X_VERSION_MIN_REQUIRED __MAC_10_6
#endif
#if !defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
#undef MAC_OS_X_VERSION_MIN_REQUIRED
#define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_6
#endif
#endif

        // Numerical checks are preferred to named checks, but to be safe
        // we define the missing version names in case Qt uses them.

#if !defined(__MAC_10_11)
#define __MAC_10_11 101100
#endif
#if !defined(__MAC_10_12)
#define __MAC_10_12 101200
#endif
#if !defined(__MAC_10_13)
#define __MAC_10_13 101300
#endif
#if !defined(__MAC_10_14)
#define __MAC_10_14 101400
#endif
#if !defined(__MAC_10_15)
#define __MAC_10_15 101500
#endif
#if !defined(__MAC_10_16)
#define __MAC_10_16 101600
#endif
#if !defined(MAC_OS_X_VERSION_10_11)
#define MAC_OS_X_VERSION_10_11 __MAC_10_11
#endif
#if !defined(MAC_OS_X_VERSION_10_12)
#define MAC_OS_X_VERSION_10_12 __MAC_10_12
#endif
#if !defined(MAC_OS_X_VERSION_10_13)
#define MAC_OS_X_VERSION_10_13 __MAC_10_13
#endif
#if !defined(MAC_OS_X_VERSION_10_14)
#define MAC_OS_X_VERSION_10_14 __MAC_10_14
#endif
#if !defined(MAC_OS_X_VERSION_10_15)
#define MAC_OS_X_VERSION_10_15 __MAC_10_15
#endif
#if !defined(MAC_OS_X_VERSION_10_16)
#define MAC_OS_X_VERSION_10_16 __MAC_10_16
#endif

#if !defined(__IPHONE_10_0)
#define __IPHONE_10_0 100000
#endif
#if !defined(__IPHONE_10_1)
#define __IPHONE_10_1 100100
#endif
#if !defined(__IPHONE_10_2)
#define __IPHONE_10_2 100200
#endif
#if !defined(__IPHONE_10_3)
#define __IPHONE_10_3 100300
#endif
#if !defined(__IPHONE_11_0)
#define __IPHONE_11_0 110000
#endif
#if !defined(__IPHONE_12_0)
#define __IPHONE_12_0 120000
#endif
#endif

#ifdef __LSB_VERSION__

#if __LSB_VERSION__ < 40
#error "This version of the Linux Standard Base is unsupported"
#endif

#ifndef CORE_LINUXBASE
#define CORE_LINUXBASE
#endif

#endif //__LSB_VERSION__
    } // misc
} // core

#endif // CORE24_OS_H
