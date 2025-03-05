
# Needed to set the namespace for both the export targets and the
# alias libraries.
SET(LIBRARY_NAME "Core-24" CACHE INTERNAL "")

# The version of library
SET(LIBRARY_VERSION "2024.01.2025.03")

# The version of language used to compile libraries
SET(LIBRARY_CXX_VERSION 11)

# Source directory
SET(LIBRARY_SOURCE_DIR "${PROJECT_SOURCE_DIR}")

SET(LIBRARY_TYPE SHARED)

MACRO(FIX_DEFAULT_COMPILER_SETTINGS)
  if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC|Clang")
    # For MSVC and Clang, CMake sets certain flags to defaults we want to
    # override.
    # This replacement code is taken from sample in the CMake Wiki at
    # https://gitlab.kitware.com/cmake/community/wikis/FAQ#dynamic-replace.
    FOREACH (FLAG
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
      IF (NOT BUILD_SHARED)
        STRING(REPLACE "/MD" "-MT" ${FLAG} "${${FLAG}}")

        # When using Ninja with Clang, static builds pass -D_DLL on Windows.
        # This is incorrect and should not happen, so we fix that here.
        STRING(REPLACE "-D_DLL" "" ${FLAG} "${${FLAG}}")
      endif ()

      # We prefer more strict warning checking for building Google Test.
      # Replaces /W3 with /W4 in defaults.
      STRING(REPLACE "/W3" "/W4" ${FLAG} "${${FLAG}}")

      # Prevent D9025 warning for targets that have exception handling
      # turned off (/EHs-c- flag). Where required, exceptions are explicitly
      # re-enabled using the cxx_exception_flags variable.
      STRING(REPLACE "/EHsc" "" ${FLAG} "${${FLAG}}")
    ENDFOREACH ()
  ENDIF ()
ENDMACRO()

# Create the CMake package file descriptors.
IF (TRUE)
  INCLUDE(CMakePackageConfigHelpers)
  SET(TARGET_EXPORT ${LIBRARY_NAME}Targets CACHE INTERNAL "")
  SET(GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/../.generated" CACHE INTERNAL "")
  SET(FILE_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/.cmake/${LIBRARY_NAME}")
  set(VERSION_FILE "${GENERATED_DIR}/${LIBRARY_NAME}ConfigVersion.cmake")
  WRITE_BASIC_PACKAGE_VERSION_FILE(${VERSION_FILE} VERSION ${LIBRARY_VERSION} COMPATIBILITY AnyNewerVersion)
  INSTALL(
      EXPORT ${TARGET_EXPORT}
      COMPONENT ${PROJECT_NAME}
      NAMESPACE ${LIBRARY_NAME}::
      DESTINATION ${FILE_INSTALL_DIR})
  SET(CONFIG_FILE "${GENERATED_DIR}/${LIBRARY_NAME}Config.cmake")
  IF (EXISTS "${LIBRARY_SOURCE_DIR}/.cmake")
    CONFIGURE_PACKAGE_CONFIG_FILE("${LIBRARY_SOURCE_DIR}/.cmake/Config.cmake.in"
        "${config_file}" INSTALL_DESTINATION ${FILE_INSTALL_DIR})
  ELSE ()
    CONFIGURE_PACKAGE_CONFIG_FILE("${LIBRARY_SOURCE_DIR}/../.cmake/Config.cmake.in"
        "${config_file}" INSTALL_DESTINATION ${FILE_INSTALL_DIR})
  ENDIF ()
  INSTALL(FILES ${VERSION_FILE} ${CONFIG_FILE}
      COMPONENT "${PROJECT_NAME}"
      DESTINATION ${FILE_INSTALL_DIR})
ENDIF ()

# Defines the compiler/linker flags used to build Core-24.
# You can tweak these definitions to suit your need. A
# variable's value is empty before it's explicitly assigned to.
MACRO(CONFIGURE_COMPILER)

  FIX_DEFAULT_COMPILER_SETTINGS()

  # Flags for language
  IF (MSVC)
    SET(LIBRARY_CXX_DEFAULT 11)

    # Newlines inside flags variables break CMake's NMake generator.
    # TODO(vladl@google.com): Add -RTCs and -RTCu to debug builds.
    SET(LIBRARY_CXX_BASE "-GS -W4 -WX -wd4251 -wd4275 -nologo -J")
    SET(LIBRARY_CXX_BASE "${LIBRARY_CXX_BASE} -D_UNICODE -DUNICODE -DWIN32 -D_WIN32")
    SET(LIBRARY_CXX_BASE "${LIBRARY_CXX_BASE} -DSTRICT -DWIN32_LEAN_AND_MEAN")

    SET(LIBRARY_CXX_EXCEPTION "-EHsc -D_HAS_EXCEPTIONS=1")
    SET(LIBRARY_CXX_NO_EXCEPTION "-EHs-c- -D_HAS_EXCEPTIONS=0")
    SET(LIBRARY_CXX_NO_RTTI "-GR-")
    SET(LIBRARY_CXX_STRICT "")

    # Suppress "unreachable code" warning,
    # https://stackoverflow.com/questions/3232669 explains the issue.
    SET(LIBRARY_CXX_BASE "${LIBRARY_CXX_BASE} -wd4702")

    # Ensure MSVC treats source files as UTF-8 encoded.
    IF (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      SET(LIBRARY_CXX_BASE "${LIBRARY_CXX_BASE} -utf-8")
    ENDIF ()
    IF (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
      SET(LIBRARY_CXX_BASE "${LIBRARY_CXX_BASE} /fp:precise -Wno-inconsistent-missing-override -Wno-microsoft-exception-spec -Wno-unused-function -Wno-unused-but-set-variable")
    ENDIF ()
  ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
    #        SET(LIBRARY_CXX_BASE "-Wall -Wshadow -Wconversion -Wundef")
    SET(LIBRARY_CXX_BASE "-Wall -Wconversion -Wundef")
    SET(LIBRARY_CXX_EXCEPTION "-fexceptions")
    SET(LIBRARY_CXX_NO_EXCEPTION "-fno-exceptions")
    SET(LIBRARY_CXX_STRICT "-W -Wpointer-arith -Wreturn-type -Wcast-qual -Wwrite-strings -Wswitch -Wunused-parameter -Wcast-align -Winline -Wredundant-decls")
    SET(LIBRARY_CXX_NO_RTTI "-fno-rtti")
    IF (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
      SET(LIBRARY_CXX_STRICT "${LIBRARY_CXX_STRICT} -Wchar-subscripts")
    ENDIF ()
    IF (CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM")
      SET(LIBRARY_CXX_BASE "${LIBRARY_CXX_BASE} -Wno-implicit-float-size-conversion -ffp-model=precise")
    ENDIF ()
  ELSEIF (CMAKE_COMPILER_IS_GNUCXX)
    #        SET(LIBRARY_CXX_BASE "-Wall -Wshadow -Wundef")
    SET(LIBRARY_CXX_BASE "-Wall -Wundef")
    IF (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7.0.0)
      SET(LIBRARY_CXX_BASE "${LIBRARY_CXX_BASE} -Wno-error=dangling-else")
    ENDIF ()
    SET(LIBRARY_CXX_EXCEPTION "-fexceptions")
    SET(LIBRARY_CXX_NO_EXCEPTION "-fno-exceptions")
    # Until version 4.3.2, GCC doesn't define a macro to indicate
    # whether RTTI is enabled. Therefore we define CORE_HAS_RTTI
    # explicitly.
    SET(LIBRARY_CXX_NO_RTTI "-fno-rtti -DCORE_HAS_RTTI=0")
    SET(LIBRARY_CXX_STRICT "-Wextra -Wno-unused-parameter -Wno-missing-field-initializers")
  ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "SunPro")
    set(LIBRARY_CXX_EXCEPTION "-features=except")
    # Sun Pro doesn't provide macros to indicate whether exceptions and
    # RTTI are enabled, so we define CORE_HAS_* explicitly.
    set(LIBRARY_CXX_NO_EXCEPTION "-features=no%except -DCORE_HAS_EXCEPTIONS=0")
    set(LIBRARY_CXX_NO_RTTI "-features=no%rtti -DCORE_HAS_RTTI=0")
  ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "VisualAge" OR
      CMAKE_CXX_COMPILER_ID STREQUAL "XL")
    # CMake 2.8 changes Visual Age's compiler ID to "XL".
    set(LIBRARY_CXX_EXCEPTION "-qeh")
    set(LIBRARY_CXX_NO_EXCEPTION "-qnoeh")
    # Until version 9.0, Visual Age doesn't define a macro to indicate
    # whether RTTI is enabled. Therefore we define CORE_HAS_RTTI
    # explicitly.
    set(LIBRARY_CXX_NO_RTTI "-qnortti -DCORE_HAS_RTTI=0")
  ELSEIF (CMAKE_CXX_COMPILER_ID STREQUAL "HP")
    set(LIBRARY_CXX_BASE "-AA -mt")
    set(LIBRARY_CXX_EXCEPTION "-DCORE_HAS_EXCEPTIONS=1")
    set(LIBRARY_CXX_NO_EXCEPTION "+noeh -DCORE_HAS_EXCEPTIONS=0")
    # RTTI can not be disabled in HP aCC compiler.
    set(LIBRARY_CXX_NO_RTTI "")
  ENDIF ()

  # For building Core's own tests and samples.

  SET(CXX_EXCEPTION "${LIBRARY_CXX_BASE} ${LIBRARY_CXX_EXCEPTION}")
  SET(CXX_NO_EXCEPTION "${CMAKE_CXX_FLAGS} ${LIBRARY_CXX_BASE} ${LIBRARY_CXX_NO_EXCEPTION}")
  SET(CXX_DEFAULT "${CXX_EXCEPTION}")
  SET(CXX_NO_RTTI "${CXX_DEFAULT} ${LIBRARY_CXX_NO_RTTI}")

  # For building the gtest libraries.
  SET(CXX_STRICT "${CXX_DEFAULT} ${LIBRARY_CXX_STRICT}")
ENDMACRO()

FUNCTION(CREATE_LIBRARY_ NAME TYPES FLAGS #[[SOURCES...]])
  # type can be either STATIC or SHARED to denote a static or shared library.
  # ARGN refers to additional arguments after 'FLAGS'.
  ADD_LIBRARY(${NAME} ${TYPE} ${ARGN})
  ADD_LIBRARY(${LIBRARY_NAME}::${NAME} ALIAS ${NAME})

  GET_TARGET_PROPERTY(PROP1 ${NAME} COMPILE_FLAGS)

  SET_TARGET_PROPERTIES(${NAME}
      PROPERTIES
      COMPILE_FLAGS "${FLAGS}")

  GET_TARGET_PROPERTY(PROP1 ${NAME} COMPILE_FLAGS)
  # Set the output directory for build artifacts.
  SET_TARGET_PROPERTIES(${NAME}
      PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
      PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      COMPILE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
  # Make PDBs match library name.
  GET_TARGET_PROPERTY(PDB_DEBUG_POSTFIX ${NAME} DEBUG_POSTFIX)
  SET_TARGET_PROPERTIES(${NAME}
      PROPERTIES
      PDB_NAME "${NAME}"
      PDB_NAME_DEBUG "${NAME}${PDB_DEBUG_POSTFIX}"
      COMPILE_PDB_NAME "${NAME}"
      COMPILE_PDB_NAME_DEBUG "${NAME}${PDB_DEBUG_POSTFIX}")
  IF (LIBARY_TYPE STREQUAL "SHARED")
    SET_TARGET_PROPERTIES(${NAME}
        PROPERTIES
        COMPILE_DEFINITIONS "CORE_CREATE_SHARED_LIBRARY=1")
    TARGET_COMPILE_DEFINITIONS(${NAME} INTERFACE
        $<INSTALL_INTERFACE:CORE_LINKED_HAS_SHARED_LIBRARY=1>)
    TARGET_COMPILE_FEATURES(${NAME} PUBLIC ${LIBRARY_CXX_VERSION})
  ENDIF ()
ENDFUNCTION()

FUNCTION(CREATE_EXECUTABLE_ NAME FLAGS LIBRARIES #[[SOURCES...]])
  ADD_EXECUTABLE(${NAME} ${ARGN})
  IF (MSVC)
    # BigObj required for tests.
    SET(FLAGS "${FLAGS} -bigobj")
  ENDIF ()
  IF (FLAGS)
    SET_TARGET_PROPERTIES(${NAME}
        PROPERTIES
        COMPILE_FLAGS "${FLAGS}")
  ENDIF ()
  IF (BUILD_SHARED)
    SET_TARGET_PROPERTIES(${NAME}
        PROPERTIES
        COMPILE_DEFINITIONS "CORE_LINKED_AS_SHARED_LIBRARY=1")
  ENDIF ()
  # To support mixing linking in static and dynamic libraries, link each
  # library in with an extra call to target_link_libraries.
  FOREACH (LIBRARY ${LIBRARIES})
    TARGET_LINK_LIBRARIES(${NAME} ${LIBRARY})
  ENDFOREACH ()
ENDFUNCTION()

########################################################################
#
# Helper functions for creating build targets.

FUNCTION(CREATE_SHARED_LIBRARY NAME FLAGS #[[SOURCES...]])
  CREATE_LIBRARY_(${NAME} SHARED ${FLAGS} ${ARGN})
ENDFUNCTION()

FUNCTION(CREATE_LIBRARY NAME FLAGS #[[SOURCES...]])
  CREATE_LIBRARY_(${NAME} "" ${FLAGS} ${ARGN})
ENDFUNCTION()

# cxx_executable(name dir lib srcs...)
#
# Creates a named target that depends on the given libs and is built
# from the given source files. dir/*.cpp is implicitly included in
# the source file list.
FUNCTION(CREATE_EXECUTABLE NAME LIBRARIES #[[DIRECTORIES ...]])
  CREATE_EXECUTABLE_(${NAME} "${CXX_DEFAULT}" "${LIBRARIES}" ${ARGN})
ENDFUNCTION()

FUNCTION(INSTALL_PROJECT_ PROJECT SOURCE_DIR TARGET)
  INSTALL(DIRECTORY ${SOURCE_DIR}
      COMPONENT ${PROJECT}
      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
  # Install the project targets.
  INSTALL(TARGETS ${TARGET}
      EXPORT ${TARGET_EXPORT}
      COMPONENT "${PROJECT}"
      RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
      ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
      LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
  IF (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Install PDBs.
    GET_TARGET_PROPERTY(PDB_NAME ${TARGET} COMPILE_PDB_NAME)
    GET_TARGET_PROPERTY(PDB_NAME_DEBUG ${TARGET} COMPILE_PDB_NAME_DEBUG)
    GET_TARGET_PROPERTY(PDB_OUTPUT_DIRECTORY ${TARGET} PDB_OUTPUT_DIRECTORY)
    INSTALL(FILES
        "${PDB_OUTPUT_DIRECTORY}/\${CMAKE_INSTALL_CONFIG_NAME}/$<$<CONFIG:Debug>:${PDB_NAME_DEBUG}>$<$<NOT:$<CONFIG:Debug>>:${PDB_NAME}>.pdb"
        COMPONENT "${PROJECT_NAME}"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        OPTIONAL)
  ENDIF ()
  # Configure and install pkgconfig files.
  SET(CONF_PC "${GENERATED_DIR}/${TARGET}.pc")
  IF (EXISTS "${PROJECT_SOURCE_DIR}/.cmake")
    CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/.cmake/${TARGET}.pc.in"
        "${CONF_PC}" @ONLY)
  ELSE ()
    CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/../.cmake/${TARGET}.pc.in"
        "${CONF_PC}" @ONLY)
  ENDIF ()
  INSTALL(FILES "${CONF_PC}"
      COMPONENT "${PROJECT_NAME}"
      DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
ENDFUNCTION()

FUNCTION(CREATE_CLASS_TEST CLASS DIRECTORY GENERATE)
  STRING(TOUPPER "${CLASS}" UPPER_NAME)
  IF (${GENERATE})
    IF (EXISTS "${DIRECTORY}")
      MESSAGE("Directory ${DIRECTORY} Already Exists")
    ELSEIF (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY}")
      MESSAGE("Directory ${DIRECTORY} Already Exists")
    ELSE ()
      MESSAGE("Creating Directory ${DIRECTORY}")
      FILE(MAKE_DIRECTORY ${DIRECTORY})
    ENDIF ()
    IF (EXISTS "${DIRECTORY}/${CLASS}Test.cpp")
      MESSAGE("File ${DIRECTORY}/${CLASS}Test.cpp Already Exists")
    ELSEIF (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY}/${CLASS}Test.cpp")
      MESSAGE("File ${DIRECTORY}/${CLASS}Test.cpp Already Exists")
    ELSEIF (EXISTS "${DIRECTORY}/${CLASS}Test.h")
      MESSAGE("File ${DIRECTORY}/${CLASS}Test.h Already Exists")
    ELSEIF (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DIRECTORY}/${CLASS}Test.h")
      MESSAGE("File ${DIRECTORY}/${CLASS}Test.h Already Exists")
    ELSE ()
      MESSAGE("Creating header and source Files For class ${CLASS}Test ...")
      FILE(WRITE
          "${DIRECTORY}/${CLASS}Test.h"
          "//
//
// This File has been Created by CMake For Testing in Project ${PROJECT_NAME}
//

#ifndef CORE24_TEST_${UPPER_NAME}_H
#define CORE24_TEST_${UPPER_NAME}_H

#include <core/misc/Unsafe.h>
#include <core/time/Chrono.h>
#include <gtest/gtest.h>
#include <lib/Printer.h>

using namespace core;
using namespace core::util;
using namespace core::time;


class ${CLASS}Test : public virtual Object, public virtual  testing::Test {
private:

    Chrono chronometer;

    // $test(before)
    void SetUp() final {
        chronometer.start();
    }

    // $test(after)
    void TearDown() final {
        glong duration = chronometer.elapsedTime();
        // out.printf(\"Elapsed time: %d ms%n\", duration);
    }

public:

    // $test(body)
    void TestBody() override = 0;
};

#endif // CORE24_TEST_${UPPER_NAME}_H
")
      FILE(WRITE
          "${DIRECTORY}/${CLASS}Test.cpp"
          "
//
// This File has been Created by CMake For Testing in Project ${PROJECT_NAME}
//
#include \"${CLASS}Test.h\"

TEST_F(${CLASS}Test, test1) {
    // body of your test
}

")
    ENDIF ()
  ENDIF ()

  LIST(APPEND LIBRARIES "GTest::gtest")
  LIST(APPEND LIBRARIES "GTest::gtest_main")
  LIST(APPEND LIBRARIES "GTest::gmock")
  LIST(APPEND LIBRARIES "${LIBRARY_NAME}::core")

  FIND_PACKAGE(GTest REQUIRED)

  CREATE_EXECUTABLE(${CLASS}Test
      "${LIBRARIES}"
      "${DIRECTORY}/${CLASS}Test.cpp" "${DIRECTORY}/${CLASS}Test.h")
ENDFUNCTION()

# Ensure that all objects will be visualized by gdb
IF (EXISTS "${CMAKE_HOME_DIRECTORY}/.gdbinit")
ELSE ()
  FILE(WRITE "${CMAKE_HOME_DIRECTORY}/.gdbinit" "
python
import sys
sys.path.append(\"${CMAKE_CURRENT_SOURCE_DIR}\")
")
ENDIF ()