//
// This File has been Created by CMake For Testing in Project CORE-24
//
#include "TraceTest.h"

#define CORE_NO_THROW(x) EXPECT_NO_THROW(out.println((x).toString()));

TEST_F(TraceTest, try_with_operator_subscript) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye[123]);
}

TEST_F(TraceTest, try_with_operator_call) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye(123));
}

TEST_F(TraceTest, try_with_operator_plus) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(+cobaye);
}

TEST_F(TraceTest, try_with_operator_add) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye + 123);
}

TEST_F(TraceTest, try_with_operator_incr_rhs) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye += 123);
}

TEST_F(TraceTest, try_with_operator_incr) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(++cobaye);
}

TEST_F(TraceTest, try_with_operator_post_inc) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye++);
}

TEST_F(TraceTest, try_with_operator_minus) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(-cobaye);
}

TEST_F(TraceTest, try_with_operator_substract) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye - 123);
}

TEST_F(TraceTest, try_with_operator_decr_rhs) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye -= 123);
}

TEST_F(TraceTest, try_with_operator_decr) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(--cobaye);
}

TEST_F(TraceTest, try_with_operator_post_decr) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye--);
}

TEST_F(TraceTest, try_with_operator_deref) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(*cobaye);
}

TEST_F(TraceTest, try_with_operator_multiply) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye * 123);
}

TEST_F(TraceTest, try_with_operator_auto_multiply) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye *= 123);
}

TEST_F(TraceTest, try_with_operator_divide_by) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye / 123);
}

TEST_F(TraceTest, try_with_operator_auto_divide_by) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye /= 123);
}

TEST_F(TraceTest, try_with_operator_modulo) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye % 123);
}

TEST_F(TraceTest, try_with_operator_auto_modulo) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye %= 123);
}


TEST_F(TraceTest, try_with_operator_xor) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye ^ 123);
}

TEST_F(TraceTest, try_with_operator_auto_xor) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye ^= 123);
}


TEST_F(TraceTest, try_with_operator_assign) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye = 123);
}

TEST_F(TraceTest, try_with_operator_equal) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye == 123);
}


TEST_F(TraceTest, try_with_operator_not) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(!cobaye);
}

TEST_F(TraceTest, try_with_operator_not_equal) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye != 123);
}


TEST_F(TraceTest, try_with_operator_less) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye < 123);
}

TEST_F(TraceTest, try_with_operator_less_equal) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye <= 123);
}


TEST_F(TraceTest, try_with_operator_left_shift) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye << 123);
}

TEST_F(TraceTest, try_with_operator_auto_left_shift) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye <<= 123);
}


TEST_F(TraceTest, try_with_operator_great) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye > 123);
}

TEST_F(TraceTest, try_with_operator_great_equal) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye >= 123);
}


TEST_F(TraceTest, try_with_operator_right_shift) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye >> 123);
}

TEST_F(TraceTest, try_with_operator_auto_right_shift) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye >>= 123);
}


TEST_F(TraceTest, try_with_operator_and) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye & 123);
}

TEST_F(TraceTest, try_with_operator_and_equal) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye &= 123);
}


TEST_F(TraceTest, try_with_operator_boolean_and_shift) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye && 123);
}


TEST_F(TraceTest, try_with_operator_or) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye | 123);
}

TEST_F(TraceTest, try_with_operator_or_equal) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye |= 123);
}


TEST_F(TraceTest, try_with_operator_boolean_or_shift) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye || 123);
}


TEST_F(TraceTest, try_with_operator_pointer) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye.operator->());
}


TEST_F(TraceTest, try_with_operator_bitwise_not) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(~cobaye);
}


TEST_F(TraceTest, try_with_operator_non_static_function) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye.nonStaticFunction());
}


TEST_F(TraceTest, try_with_operator_static_function) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye.staticFunction());
}


TEST_F(TraceTest, try_with_operator_lambda_function) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW(cobaye.lambdaFunction()());
}


TEST_F(TraceTest, try_with_operator_template_function) {
    TraceTest& cobaye = *this;
    CORE_NO_THROW((cobaye.templateFunction<Boolean, Byte, Short, Integer, Long, Float, Double, Complex, Number>()));
}
