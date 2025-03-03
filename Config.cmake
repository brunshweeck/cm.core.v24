

IF (EXISTS "Settings.cmake")
  INCLUDE("Settings.cmake")
  INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Lib")
ELSE ()
  INCLUDE("../Settings.cmake")
  INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}")
ENDIF ()

CONFIGURE_COMPILER()

IF (WIN32)
  SET(SYSTEM_DIR "Win32")
ELSE ()
  SET(SYSTEM_DIR "Unix")
ENDIF ()

#IF (MSVC)
#    SET(COMPILE_OPTIONS)
#ELSE ()
#    SET(COMPILE_OPTIONS
#            -ftrivial-auto-var-init=zero
#            -mlong-double-80 # These switches control the size of "long double" type.
#            -Wdouble-promotion # Give a warning when a value of type "float" is implicitly  promoted to  "double".
#    )
#ENDIF ()

#ADD_COMPILE_OPTIONS(${COMPILE_OPTIONS})

SET(CMAKE_CXX_STANDARD_DEFAULT ${LIBRARY_CXX_VERSION})
SET(CMAKE_C_STANDARD_DEFAULT 99)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)


