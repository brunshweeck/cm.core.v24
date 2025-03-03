#[[

                        ____                 ____  _  _
                       / ___|___  _ __ ___  |___ \| || |
                      | |   / _ \| '__/ _ \   __) | || |_
                      | |__| (_) | | |  __/  / __/|__   _|
                       \____\___/|_|  \___| |_____|  |_|
                        The new vision of C++ (since 2024)

]]

PROJECT(CORE
    VERSION 2025.05.06
    LANGUAGES C CXX ASM
)

INCLUDE("../Config.cmake")

MESSAGE(NOTICE ${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB LibHeaders
    "core/atomic/*.h"
    "core/charset/*.h"
    "core/concurrent/*.h"
    "core/function/*.h"
    "core/io/*.h"
    "core/lang/*.h"
    "core/misc/*.h"
    "core/net/*.h"
    "core/random/*.h"
    "core/regex/*.h"
    "core/stream/*.h"
    "core/text/*.h"
    "core/time/*.h"
    "core/time/spi/*.h"
    "core/util/*.h"
)

FILE(GLOB LibSources
    "core/atomic/*.cpp"
    "core/charset/*.cpp"
    "core/concurrent/*.cpp"
    "core/function/*.cpp"
    "core/io/*.cpp"
    "core/lang/*.cpp"
    "core/misc/*.cpp"
    "core/net/*.cpp"
    "core/random/*.cpp"
    "core/regex/*.cpp"
    "core/stream/*.cpp"
    "core/text/*.cpp"
    "core/time/*.cpp"
    "core/time/spi/*.cpp"
    "core/util/*.cpp"
)

FILE(GLOB MetaSources
    "meta/atomic/*.cpp"
    "meta/atomic/*.h"
    "meta/charset/*.cpp"
    "meta/charset/*.h"
    "meta/concurrent/*.cpp"
    "meta/concurrent/*.h"
    "meta/function/*.cpp"
    "meta/function/*.h"
    "meta/io/*.cpp"
    "meta/io/*.h"
    "meta/lang/*.cpp"
    "meta/lang/*.h"
    "meta/misc/*.cpp"
    "meta/misc/*.h"
    "meta/net/*.cpp"
    "meta/net/*.h"
    "meta/random/*.cpp"
    "meta/random/*.h"
    "meta/regex/*.cpp"
    "meta/regex/*.h"
    "meta/stream/*.cpp"
    "meta/stream/*.h"
    "meta/text/*.cpp"
    "meta/text/*.h"
    "meta/time/*.cpp"
    "meta/time/*.h"
    "meta/time/spi/*.cpp"
    "meta/time/spi/*.h"
    "meta/util/*.cpp"
    "meta/util/*.h"
    "meta/util/cldr/*.cpp"
    "meta/util/cldr/*.h"
    "meta/util/cldr/locales/*.cpp"
    "meta/util/cldr/locales/*.h"
    "meta/ext/*.cpp"
    "meta/ext/*.h"
)

FILE(GLOB NativeSources
    "native/*.cpp"
    "native/*.h"
    "native/*/*.cpp"
    "native/*/*.h"
    "native/*/*/*.cpp"
    "native/*/*/*.h"
    "native/*/*/*/*.cpp"
    "native/*/*/*/*.h"
    "native/*/*/*/*/*.cpp"
    "native/*/*/*/*/*.h"
    #[[ old dirs ]]
    "../Internal/${SYSTEM_DIR}/*.cpp"
    "../Internal/${SYSTEM_DIR}/*.h"
    "../Internal/${SYSTEM_DIR}/*/*.cpp"
    "../Internal/${SYSTEM_DIR}/*/*.h"
    "../Internal/${SYSTEM_DIR}/*/*/*.cpp"
    "../Internal/${SYSTEM_DIR}/*/*/*.h"
    "../Internal/${SYSTEM_DIR}/*/*/*/*.cpp"
    "../Internal/${SYSTEM_DIR}/*/*/*/*.h"
    "../Internal/${SYSTEM_DIR}/*/*/*/*/*.cpp"
    "../Internal/${SYSTEM_DIR}/*/*/*/*/*.h"
)

FILE(GLOB CORE_SOURCES ${LibHeaders} ${LibSources} ${MetaSources} ${NativeSources})

CREATE_SHARED_LIBRARY(
    core
    ${CXX_DEFAULT}
    ${CORE_SOURCES}
)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}")

INSTALL_PROJECT_(
    ${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}
    core)
