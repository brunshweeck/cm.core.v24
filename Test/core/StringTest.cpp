//
// This File has been Created by CMake For Testing in Project CORE-24
//
#include "StringTest.h"

TEST_F(StringTest, try_construct_new_ansi_string_with_ansi_string) {
    EXPECT_TRUE(isAnsiString("Hello, World"_Sl));
    EXPECT_EQ("Hello, World"_Sl, "Hello, World");
}

TEST_F(StringTest, try_construct_new_ansi_string_with_utf8_string) {
    EXPECT_TRUE(isAnsiString(u8"ブラン背ヴィク"_Sl));
    EXPECT_NE(u8"ブラン背ヴィク"_Sl, u8"ブラン背ヴィク");
}

TEST_F(StringTest, try_construct_new_ansi_string_with_utf16_string) {
    EXPECT_TRUE(isAnsiString(u"ブラン背ヴィク"_Sl));
    EXPECT_NE(u"ブラン背ヴィク"_Sl, u"ブラン背ヴィク");
}

TEST_F(StringTest, try_construct_new_ansi_string_with_utf32_string) {
    EXPECT_TRUE(isAnsiString(U"ブラン背ヴィク"_Sl));
    EXPECT_NE(U"ブラン背ヴィク"_Sl, U"ブラン背ヴィク");
}

TEST_F(StringTest, try_construct_new_ansi_string_with_wide_string) {
    EXPECT_TRUE(isAnsiString(L"ブラン背ヴィク"_Sl));
    EXPECT_NE(L"ブラン背ヴィク"_Sl, L"ブラン背ヴィク");
}

TEST_F(StringTest, try_construct_new_unicode_string_with_ansi_string) {
    EXPECT_TRUE(isAnsiString("Hello, World"_Su));
    EXPECT_EQ("Hello, World"_Su, "Hello, World");
}

TEST_F(StringTest, try_construct_new_unicode_string_with_utf8_string) {
    EXPECT_TRUE(isNonAnsiString(u8"ブラン背ヴィク"_Su));
    EXPECT_EQ(u8"ブラン背ヴィク"_Su, u8"ブラン背ヴィク");
}

TEST_F(StringTest, try_construct_new_unicode_string_with_utf16_string) {
    EXPECT_TRUE(isNonAnsiString(u"ブラン背ヴィク"_Su));
    EXPECT_EQ(u"ブラン背ヴィク"_Su, u"ブラン背ヴィク");
}

TEST_F(StringTest, try_construct_new_unicode_string_with_utf32_string) {
    EXPECT_TRUE(isNonAnsiString(U"ブラン背ヴィク"_Su));
    EXPECT_EQ(U"ブラン背ヴィク"_Su, U"ブラン背ヴィク");
}

TEST_F(StringTest, try_construct_new_unicode_string_with_wide_string) {
    EXPECT_TRUE(isNonAnsiString(L"ブラン背ヴィク"_Su));
    EXPECT_EQ(L"ブラン背ヴィク"_Su, L"ブラン背ヴィク");
}

TEST_F(StringTest, try_construct_new_string_with_null_pointer_string) {
    char* nullStr = null;
    EXPECT_ANY_THROW((String)nullStr);
}
