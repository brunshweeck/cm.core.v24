
IF (WIN32)
    SET(SYSTEM_DIR "Win32")
ELSE ()
    SET(SYSTEM_DIR "Unix")
ENDIF ()

IF (MSVC)
    SET(COMPILE_OPTIONS )
ELSE ()
    SET(COMPILE_OPTIONS
            -ftrivial-auto-var-init=zero
            -mlong-double-80 # These switches control the size of "long double" type.
            -Wdouble-promotion # Give a warning when a value of type "float" is implicitly  promoted to  "double".
    )
ENDIF ()

ADD_COMPILE_OPTIONS(${COMPILE_OPTIONS})

IF (NOT EXISTS "${PROJECT_SOURCE_DIR}/Lib/native/${SYSTEM_DIR}")
    FILE(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/Lib/native/${SYSTEM_DIR}")
ENDIF ()

IF (NOT EXISTS "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}")
    FILE(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}")
ENDIF ()

IF (NOT EXISTS "${PROJECT_SOURCE_DIR}/Lib")
    FILE(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/Lib")
ENDIF ()

IF (NOT EXISTS "${PROJECT_SOURCE_DIR}/Src")
    FILE(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/Src")
ENDIF ()

IF (NOT EXISTS "${PROJECT_SOURCE_DIR}/Test")
    FILE(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/Test")
ENDIF ()

SET(CMAKE_CXX_STANDARD_DEFAULT 11)
SET(CMAKE_C_STANDARD_DEFAULT  99)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_C_STANDARD 99)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

INCLUDE(GNUInstallDirs)
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Lib")

IF (MSVC)
ELSE ()
    SET(COMPILE_OPTIONS -save)
ENDIF ()

FILE(GLOB LibHeaders
        "${PROJECT_SOURCE_DIR}/Lib/core/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/core/**/*.h"
)

FILE(GLOB LibSources
        "${PROJECT_SOURCE_DIR}/Lib/core/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/core/**/*.cpp"
)

FILE(GLOB MetaSources
        "${PROJECT_SOURCE_DIR}/Lib/meta/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/meta/*/*/*/*/*.h"
)

FILE(GLOB NativeSources
        "${PROJECT_SOURCE_DIR}/Lib/native/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/native/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/native/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/native/*/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/native/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/native/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/native/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/native/*/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/Lib/native/*/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Lib/native/*/*/*/*/*.h"
        #[[ old dirs ]]
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*.cpp"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*.h"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*/*.h"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*/*/*/*.h"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*/*/*/*/*.cpp"
        "${PROJECT_SOURCE_DIR}/Internal/${SYSTEM_DIR}/*/*/*/*/*.h"
)