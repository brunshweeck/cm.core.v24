CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(Core24
        VERSION 24.01.2025.03
        DESCRIPTION "MY FUTURE VISION FOR C++ LANGUAGE (POINTER TO PURE REFERENCE)"
        LANGUAGES ASM C CXX)

SET(CMAKE_C_STANDARD 99)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_EXPORT_COMPILE_COMMANDS OFF)

ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/Test")

IF (WIN32)
    SET(SYSTEM "Win32")
ELSE ()
    SET(SYSTEM "Unix")
ENDIF (WIN32)

FILE(GLOB CORE_HEADERS
        "Lib/core/*.h"
        "Lib/core/*/*.h")
FILE(GLOB CORE_SOURCES
        "Lib/core/*.cpp"
        "Lib/core/*/*.cpp")
FILE(GLOB META_HEADERS
        "Lib/meta/*.h"
        "Lib/meta/*/*.h"
        "Lib/meta/*/*/*.h"
        "Lib/meta/*/*/*/*.h")
FILE(GLOB META_SOURCES
        "Lib/meta/*.cpp"
        "Lib/meta/*/*.cpp"
        "Lib/meta/*/*/*.cpp"
        "Lib/meta/*/*/*/*.cpp")
FILE(GLOB SYSTEM_HEADERS
        "Internal/${SYSTEM}/core/*.h"
        "Internal/${SYSTEM}/core/*/*.h")
FILE(GLOB SYSTEM_SOURCES
        "Internal/${SYSTEM}/core/*.cpp"
        "Internal/${SYSTEM}/core/*/*.cpp")

SET(FILES
        ${CORE_HEADERS}
        ${META_HEADERS}
        ${SYSTEM_HEADERS}
        ${CORE_SOURCES}
        ${META_SOURCES}
        ${SYSTEM_SOURCES}
        Lib/core/concurrent/ConcurrentLinkedQueue.cpp
        Lib/core/concurrent/ConcurrentLinkedQueue.h
        Lib/core/concurrent/ConcurrentLinkedDeque.cpp
        Lib/core/concurrent/ConcurrentLinkedDeque.h)

INCLUDE(GNUInstallDirs)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Lib")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Internal/${SYSTEM}")

ADD_LIBRARY(Core24 SHARED ${FILES})
ADD_EXECUTABLE(Main "Src/Main.cpp")

TARGET_LINK_LIBRARIES(Main PUBLIC Core24)

INSTALL(TARGETS Core24
        EXPORT Core24Core
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
