//
// This File has been Created by CMake For Testing in Project CORE-24
//
#include "FileTest.h"

#include <core/io/File.h>

TEST_F(FileTest, try_construct_new_file_with_absolute_path) {
    ASSERT_TRUE(File("/Dir1/Sample.txt").isAbsolute());
}

TEST_F(FileTest, try_construct_new_file_with_relative_path) {
    ASSERT_FALSE(File("Dir1/Sample.txt").isAbsolute());
}

TEST_F(FileTest, try_find_absolute_path_of_file) {
    File file = File("Sample.txt");
    ASSERT_TRUE(file.absoluteFile().isAbsolute());
}
