#[[
================================================================================
                            CollectionsTest
                            ---------------
        This CMakeLists.txt file is linked with Cxx file Use to test followings
        collections types:
        - List<E>
            |_ ArrayList<E>
            |_ LinkedList<E>
            |_ Vector<E>
        - Set<E>
            |_ HashSet<E>
            |_ TreeSet<E>
        - Queue<E>
            |_ Deque<E>
            |   |_ ArrayDeque<E>
            |   |_ LinkedDeque<E>
            |_ PriorityQueue<E>
================================================================================
]]
IF (WIN32)
    SET(SYSTEM_LIB "Win32")
ELSE ()
    SET(SYSTEM_LIB "Unix")
ENDIF (WIN32)

IF (WIN32)
    SET(SYSTEM "Win32")
ELSE ()
    SET(SYSTEM "Unix")
ENDIF (WIN32)

FIND_PACKAGE(GTest CONFIG REQUIRED)
SET(GTEST PRIVATE Core24 PRIVATE GTest::gtest PRIVATE GTest::gmock PRIVATE GTest::gtest_main)
INCLUDE(GoogleTest)
ENABLE_TESTING()

ADD_EXECUTABLE(CollectionsTest CollectionsTest.cpp)

TARGET_INCLUDE_DIRECTORIES(CollectionsTest SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/${SYSTEM}")
TARGET_INCLUDE_DIRECTORIES(CollectionsTest SYSTEM AFTER PRIVATE "${PROJECT_SOURCE_DIR}/Lib")
TARGET_LINK_LIBRARIES(CollectionsTest ${GTEST})


