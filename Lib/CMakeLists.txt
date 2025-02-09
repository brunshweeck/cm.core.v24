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

INCLUDE("${PROJECT_SOURCE_DIR}/../Config.cmake")


FILE(GLOB LibHeaders
        "${PROJECT_SOURCE_DIR}/core/*.h"
        "${PROJECT_SOURCE_DIR}/core/**/*.h"
)

FILE(GLOB LibSources
        "${PROJECT_SOURCE_DIR}/core/*.cpp"
        "${PROJECT_SOURCE_DIR}/core/**/*.cpp"
)

FILE(GLOB MetaSources
        "${PROJECT_SOURCE_DIR}/meta/*.cpp"
        "${PROJECT_SOURCE_DIR}/meta/*.h"
        "${PROJECT_SOURCE_DIR}/meta/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/meta/*/*.h"
        "${PROJECT_SOURCE_DIR}/meta/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/meta/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/meta/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/meta/*/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/meta/*/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/meta/*/*/*/*/*.h"
)

FILE(GLOB NativeSources
        "${PROJECT_SOURCE_DIR}/native/*.cpp"
        "${PROJECT_SOURCE_DIR}/native/*.h"
        "${PROJECT_SOURCE_DIR}/native/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/native/*/*.h"
        "${PROJECT_SOURCE_DIR}/native/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/native/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/native/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/native/*/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/native/*/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/native/*/*/*/*/*.h"
        #[[ old dirs ]]
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*.cpp"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*.h"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*/*.h"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/../Internal/${SYSTEM_DIR}/*/*/*/*/*.h"
)

FILE(GLOB CORE_SOURCES ${LibHeaders} ${LibSources} ${MetaSources} ${NativeSources})

CREATE_SHARED_LIBRARY(
        core
        ${CXX_DEFAULT}
        ${CORE_SOURCES}
)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}")

INSTALL_PROJECT_(
        ${PROJECT_NAME}
        ${PROJECT_SOURCE_DIR}
        core)
