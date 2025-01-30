
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

## For Download and add immediately into the build main
## For link target remove Gtest::
#INCLUDE(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
#SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

IF (WIN32)
    SET(SYSTEM "Win32")
ELSE ()
    SET(SYSTEM "Unix")
ENDIF (WIN32)

SET(GTEST PRIVATE Core-24 PRIVATE GTest::gtest PRIVATE GTest::gmock PRIVATE GTest::gtest_main)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Internal/${SYSTEM}")
INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Lib")
FIND_PACKAGE(GTest CONFIG REQUIRED)
INCLUDE(GoogleTest)
ENABLE_TESTING()

ADD_EXECUTABLE(StringTest "StringTest.cpp")
ADD_EXECUTABLE(FileTest "FileTest.cpp")
ADD_EXECUTABLE(ThrowableTest "ThrowableTest.cpp")
ADD_EXECUTABLE(TurtleTest "Turtle.cpp")

TARGET_LINK_LIBRARIES(StringTest ${GTEST})
TARGET_LINK_LIBRARIES(FileTest ${GTEST})
TARGET_LINK_LIBRARIES(ThrowableTest ${GTEST})
TARGET_LINK_LIBRARIES(TurtleTest ${GTEST})

GTEST_DISCOVER_TESTS(StringTest)
GTEST_DISCOVER_TESTS(FileTest)
GTEST_DISCOVER_TESTS(ThrowableTest)
GTEST_DISCOVER_TESTS(TurtleTest)

ADD_SUBDIRECTORY("core/util")
