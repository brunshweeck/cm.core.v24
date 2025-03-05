#[[


            ____                 ____  _  _     _____         _
           / ___|___  _ __ ___  |___ \| || |   |_   _|__  ___| |_ ___
          | |   / _ \| '__/ _ \   __) | || |_    | |/ _ \/ __| __/ __|
          | |__| (_) | | |  __/  / __/|__   _|   | |  __/\__ \ |_\__ \
           \____\___/|_|  \___| |_____|  |_|     |_|\___||___/\__|___/



]]


INCLUDE("${PROJECT_SOURCE_DIR}/Config.cmake")

INCLUDE_DIRECTORIES(".")

#[[ Base ]]
CREATE_CLASS_TEST(String "core/lang" ON)
CREATE_CLASS_TEST(Trace "core/lang" OFF)

#[[ Charsets ]]
CREATE_CLASS_TEST(Charset "core/charset" ON)

#[[ I/O]]
CREATE_CLASS_TEST(File "core/io" ON)

#[[ Collections ]]
CREATE_CLASS_TEST(Vector "core/util" ON)
#CREATE_CLASS_TEST(ArrayList "${CMAKE_CURRENT_SOURCE_DIR}/core/util")
#CREATE_CLASS_TEST(TreeMap "${CMAKE_CURRENT_SOURCE_DIR}/core/util")

#[[ Concurrence ]]
