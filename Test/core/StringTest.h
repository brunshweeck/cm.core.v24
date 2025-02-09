//
// This File has been Created by CMake For Testing in Project CORE-24
//

#ifndef CORE24_TEST_STRING_H
#define CORE24_TEST_STRING_H

#include <core/misc/Unsafe.h>
#include <core/time/Chrono.h>
#include "../lib/Printer.h"
#include <gtest/gtest.h>

using namespace core;
using namespace core::util;
using namespace core::time;


class StringTest : public virtual Object, public virtual testing::Test {
private:
    // $mock()
    Chrono chronometer;

    // $test(before)
    void SetUp() final {
        chronometer.start();
    }

    // $test(after)
    void TearDown() final {
        glong duration = chronometer.elapsedTime();
        out.printf("Elapsed time: %d ms%n", duration);
    }

public:
    // $test(body)
    void TestBody() override = 0;


    static gbool isAnsiString(String const& str) {
        for (gint i = 0; i < str.length(); i++) {
            if (str.charAt(i) > 0xFF)
                return false;
        }
        return true;
    }

    static gbool isNonAnsiString(String const& str) {
        return !isAnsiString(str);
    }
};

#endif // CORE24_TEST_STRING_H
