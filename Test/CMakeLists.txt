#[[


            ____                 ____  _  _     _____         _
           / ___|___  _ __ ___  |___ \| || |   |_   _|__  ___| |_ ___
          | |   / _ \| '__/ _ \   __) | || |_    | |/ _ \/ __| __/ __|
          | |__| (_) | | |  __/  / __/|__   _|   | |  __/\__ \ |_\__ \
           \____\___/|_|  \___| |_____|  |_|     |_|\___||___/\__|___/



]]


INCLUDE("${PROJECT_SOURCE_DIR}/Config.cmake")

#[[ Base ]]
CREATE_CLASS_TEST(String "${CMAKE_CURRENT_SOURCE_DIR}/core")

#[[ Charsets ]]
CREATE_CLASS_TEST(Charset "${CMAKE_CURRENT_SOURCE_DIR}/core/charset")

#[[ I/O]]
CREATE_CLASS_TEST(File "${CMAKE_CURRENT_SOURCE_DIR}/core/io")

#[[ Collections ]]
CREATE_CLASS_TEST(Vector "${CMAKE_CURRENT_SOURCE_DIR}/core/util")
#CREATE_CLASS_TEST(ArrayList "${CMAKE_CURRENT_SOURCE_DIR}/core/util")
#CREATE_CLASS_TEST(TreeMap "${CMAKE_CURRENT_SOURCE_DIR}/core/util")

#[[ Concurrence ]]
