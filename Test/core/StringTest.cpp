//
// Created by admin on 11/01/25.
//

#include <core/IllegalArgumentException.h>
#include <gtest/gtest.h>
#include <lib/Printer.h>
#include <core/charset/Charset.h>

using namespace core;

class StringTest : public Object, public testing::Test {
public:
    const char *nullMultiBytesString = {};
    const char16_t *nullUtf16String = {};
    const char32_t *nullUtf32String = {};
    const wchar_t *nullUnicodeString = {};

private:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(StringTest, constructorWithLiteralMultiBytesString) {
    EXPECT_NO_THROW(String("HelloWorld"));
}

TEST_F(StringTest, constructorWithLiteralUtf16String) {
    EXPECT_NO_THROW(String(u"HelloWorld"));
}

TEST_F(StringTest, constructorWithLiteralUtf32String) {
    EXPECT_NO_THROW(String(U"HelloWorld"));
}

TEST_F(StringTest, constructorWithLiteralUnicodeString) {
    EXPECT_NO_THROW(String(L"HelloWorld"));
}

TEST_F(StringTest, constructorWithLiteralMultiBytesStringAsLatin1String) {
    EXPECT_NO_THROW("HelloWorld"_Sl);
}

TEST_F(StringTest, constructorWithLiteralUtf16StringAsLatin1String) {
    EXPECT_NO_THROW(u"HelloWorld"_Sl);
}

TEST_F(StringTest, constructorWithLiteralUtf32StringAsLatin1String) {
    EXPECT_NO_THROW(U"HelloWorld"_Sl);
}

TEST_F(StringTest, constructorWithLiteralUnicodeStringAsLatin1String) {
    EXPECT_NO_THROW(L"HelloWorld"_Sl);
}


TEST_F(StringTest, constructorWithLiteralMultiBytesStringAsUnicodeString) {
    EXPECT_NO_THROW("HelloWorld"_Su);
}

TEST_F(StringTest, constructorWithLiteralUtf16StringAsUnicodeString) {
    EXPECT_NO_THROW(u"HelloWorld"_Su);
}

TEST_F(StringTest, constructorWithLiteralUtf32StringAsUnicodeString) {
    EXPECT_NO_THROW(U"HelloWorld"_Su);
}

TEST_F(StringTest, constructorWithLiteralUnicodeStringAsUnicodeString) {
    EXPECT_NO_THROW(L"HelloWorld"_Su);
}

TEST_F(StringTest, constructorWithNullMultiBytesString) {
    EXPECT_THROW(String((const char *) null), IllegalArgumentException);
}

TEST_F(StringTest, constructorWithNullUtf16String) {
    EXPECT_THROW(String((const char16_t *) null), IllegalArgumentException);
}

TEST_F(StringTest, constructorWithNullUtf32String) {
    EXPECT_THROW(String((const char32_t *) null), IllegalArgumentException);
}

TEST_F(StringTest, constructorWithNullUnicodeString) {
    EXPECT_THROW(String((const wchar_t *) null), IllegalArgumentException);
}

TEST_F(StringTest, compareLatin1StringWithUnicodeString) {
    EXPECT_NE(L"HelloWorld"_Sl, L"HelloWorld"_Su);
    EXPECT_NE(L"ブラン背ヴィク"_Sl, L"ブラン背ヴィク"_Su);
}
