//
// Created by admin on 12/01/25.
//

#include "Printer.h"
#include <core/io/File.h>
#include <core/net/URI.h>
#include <gtest/gtest.h>

class FileTest : public Object, public testing::Test {
protected:
    String path;
    String uriPath;
    String filePath;
    String dirPath;
    String dirsPath;

    void SetUp() override {
        uriPath = "file:///Sample.dir/dir%201/dir%202/dir%203";
        filePath = "Sample.txt";
        dirPath = "Sample.dir";
        dirsPath = "Sample.dir/dir1/dir2/dir3";
        path = "Sample.dir/Sample.txt";
    }

    void TearDown() override {
    }

private:
    void TestBody() override = 0;
};

TEST_F(FileTest, constructorWithDirectPath) {
    EXPECT_NO_THROW(File(this->path));
}

TEST_F(FileTest, constructorWithParentAndChildPath) {
    EXPECT_NO_THROW(File(this->dirPath, this->filePath));
}

TEST_F(FileTest, constructorWithURI) {
    EXPECT_NO_THROW(File(URI(uriPath)));
}

TEST_F(FileTest, isAbsolute) {
    File file = File("/Sample.dir/dir1/dir2/dir3");
    EXPECT_TRUE(file.isAbsolute());
}


