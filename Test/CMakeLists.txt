MESSAGE(NOTICE "

            ____                 ____  _  _     _____         _
           / ___|___  _ __ ___  |___ \| || |   |_   _|__  ___| |_ ___
          | |   / _ \| '__/ _ \   __) | || |_    | |/ _ \/ __| __/ __|
          | |__| (_) | | |  __/  / __/|__   _|   | |  __/\__ \ |_\__ \
           \____\___/|_|  \___| |_____|  |_|     |_|\___||___/\__|___/


"
)


INCLUDE("${PROJECT_SOURCE_DIR}/Test/Config.cmake")

#SET(GTEST PRIVATE Core-24 PRIVATE GTest::gtest PRIVATE GTest::gmock PRIVATE GTest::gtest_main)
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/Test/core")
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/Test/core/charset")
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/Test/core/io")
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/Test/core/util")