//
//
// This File has been Created by CMake For Testing in Project CORE-24
//

#ifndef CORE24_TEST_VECTOR_H
#define CORE24_TEST_VECTOR_H

#include <core/misc/Unsafe.h>
#include <core/time/Chrono.h>
#include <gtest/gtest.h>

#include "../../lib/Printer.h"
#include <core/util/Vector.h>

using namespace core;
using namespace core::util;
using namespace core::time;


class VectorTest : public virtual Object, public virtual testing::Test {
private:
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

    static void iterateAndDoNothing(Vector<Integer>& cobaye) {
        for (auto& item : cobaye) {
            CORE_IGNORE(item);
        }
    }

    static void iterateAndRemoveAll(Vector<Integer>& cobaye) {
        for (auto& item : cobaye) {
            cobaye.remove(item);
        }
    }
};

static Vector<Integer> cobaye;

#endif // CORE24_TEST_VECTOR_H
