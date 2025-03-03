//
//
// This File has been Created by CMake For Testing in Project CORE-24
//

#ifndef CORE24_TEST_TRACE_H
#define CORE24_TEST_TRACE_H

#include <core/misc/Unsafe.h>
#include <core/time/Chrono.h>
#include <gtest/gtest.h>

#include "../lib/Printer.h"

using namespace core;
using namespace core::util;
using namespace core::time;


class TraceTest : public virtual Object, public virtual testing::Test {
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

    // List of function used for test
    Trace operator[](gint) const { return $ftrace(); }

    Trace operator()(gint) const { return $ftrace(); }

    Trace operator+() const { return $ftrace(); }

    Trace operator+(gint) const { return $ftrace(); }

    Trace operator+=(gint) const { return $ftrace(); }

    Trace operator++() const { return $ftrace(); }

    Trace operator++(gint) const { return $ftrace(); }

    Trace operator-() const { return $ftrace(); }

    Trace operator-(gint) const { return $ftrace(); }

    Trace operator-=(gint) const { return $ftrace(); }

    Trace operator--() const { return $ftrace(); }

    Trace operator--(gint) const { return $ftrace(); }

    Trace operator*() const { return $ftrace(); }

    Trace operator*(gint) const { return $ftrace(); }

    Trace operator*=(gint) const { return $ftrace(); }

    Trace operator/(gint) const { return $ftrace(); }

    Trace operator/=(gint) const { return $ftrace(); }

    Trace operator%(gint) const { return $ftrace(); }

    Trace operator%=(gint) const { return $ftrace(); }

    Trace operator^(gint) const { return $ftrace(); }

    Trace operator^=(gint) const { return $ftrace(); }

    Trace operator=(gint) const { return $ftrace(); }

    Trace operator==(gint) const { return $ftrace(); }

    Trace operator!() const { return $ftrace(); }

    Trace operator!=(gint) const { return $ftrace(); }

    Trace operator<(gint) const { return $ftrace(); }

    Trace operator<=(gint) const { return $ftrace(); }

    Trace operator<<(gint) const { return $ftrace(); }

    Trace operator<<=(gint) const { return $ftrace(); }

    Trace operator>(gint) const { return $ftrace(); }

    Trace operator>=(gint) const { return $ftrace(); }

    Trace operator>>(gint) const { return $ftrace(); }

    Trace operator>>=(gint) const { return $ftrace(); }

    Trace operator&(gint) const { return $ftrace(); }

    Trace operator&=(gint) const { return $ftrace(); }

    Trace operator&&(gint) const { return $ftrace(); }

    Trace operator|(gint) const { return $ftrace(); }

    Trace operator|=(gint) const { return $ftrace(); }

    Trace operator||(gint) const { return $ftrace(); }

    Trace operator->() const { return $ftrace(); }

    Trace operator~() const { return $ftrace(); }

    virtual Trace nonStaticFunction() const { return $ftrace(); }

    static Trace staticFunction() { return $ftrace(); }

    static auto lambdaFunction() { return (Trace(*)()) []()-> Trace { return $ftrace(); }; }

    template <class ...Args>
    static Trace templateFunction() { return $ftrace(); }
};

#endif // CORE24_TEST_TRACE_H
