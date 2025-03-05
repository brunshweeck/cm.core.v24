//
//
// This File has been Created by CMake For Testing in Project CORE-24
//

#ifndef CORE24_TEST_STRING_H
#define CORE24_TEST_STRING_H

#include <core/misc/Unsafe.h>
#include <core/time/Chrono.h>
#include <gtest/gtest.h>
#include <lib/Printer.h>

using namespace core;
using namespace core::util;
using namespace core::time;


class StringTest : public virtual Object, public virtual  testing::Test {
private:

    Chrono chronometer;

    // $test(before)
    void SetUp() final {
        chronometer.start();
    }

    // $test(after)
    void TearDown() final {
        glong duration = chronometer.elapsedTime();
        // out.printf("Elapsed time: %d ms%n", duration);
    }

public:

    // $test(body)
    void TestBody() override = 0;
};

#endif // CORE24_TEST_STRING_H
