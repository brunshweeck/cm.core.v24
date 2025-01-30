#include <core/Double.h>
#include <core/util/Comparator.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>


using namespace core;
using namespace core::util;
using testing::internal::ComparisonBase;

namespace core {
    template <class Impl>
    class StrictlyComparator
            : public Comparator<Double>, public ComparisonBase<StrictlyComparator<Impl>, Double, Impl> {
    public:
        StrictlyComparator(): ComparisonBase<StrictlyComparator, Double, Impl>(0.0) {}

        CORE_EXPLICIT StrictlyComparator(const Double& rhs)
            : ComparisonBase<StrictlyComparator, Double, Impl>(rhs) {}

        gint compare(Double const& t1, Double const& t2) const CORE_NOTHROW override {
            gdouble lhs = t1.doubleValue();
            gdouble rhs = t2.doubleValue();
            gint r = Double::compare(lhs, rhs);
            if (r == 0)
                return Double::toRawLongBits(lhs) - Double::toRawLongBits(rhs);
            return r;
        }

        virtual gbool operator()(Double const& t1, Double const& t2) const = 0;


        static const char* Desc() { return " corresponding for value "; }

        static const char* NegatedDesc() { return " not corresponding for value "; }
    };

    class StrictlyEqual final : public StrictlyComparator<StrictlyEqual> {
    public:
        StrictlyEqual() = default;

        CORE_EXPLICIT StrictlyEqual(const Double& rhs)
            : StrictlyComparator(rhs) {}

        gbool operator()(Double const& t1, Double const& t2) const override {
            return compare(t1, t2.doubleValue()) == 0;
        }
    };

    class StrictlyNotEqual final : public StrictlyComparator<StrictlyNotEqual> {
    public:
        StrictlyNotEqual() = default;

        CORE_EXPLICIT StrictlyNotEqual(const Double& rhs)
            : StrictlyComparator(rhs) {}

        gbool operator()(Double const& t1, Double const& t2) const override {
            return compare(t1, t2.doubleValue()) != 0;
        }
    };

    class StrictlyLessThan final : public StrictlyComparator<StrictlyLessThan> {
    public:
        StrictlyLessThan() = default;

        CORE_EXPLICIT StrictlyLessThan(const Double& rhs)
            : StrictlyComparator(rhs) {}

        gbool operator()(Double const& t1, Double const& t2) const override {
            return compare(t1, t2.doubleValue()) < 0;
        }
    };

    class StrictlyGreaterThan final : public StrictlyComparator<StrictlyGreaterThan> {
    public:
        StrictlyGreaterThan() = default;

        CORE_EXPLICIT StrictlyGreaterThan(const Double& rhs)
            : StrictlyComparator(rhs) {}

        gbool operator()(Double const& t1, Double const& t2) const override {
            return compare(t1, t2.doubleValue()) > 0;
        }
    };

    class StrictlyLessThanOrEqual final : public StrictlyComparator<StrictlyLessThanOrEqual> {
    public:
        StrictlyLessThanOrEqual() = default;

        CORE_EXPLICIT StrictlyLessThanOrEqual(const Double& rhs)
            : StrictlyComparator(rhs) {}

        gbool operator()(Double const& t1, Double const& t2) const override {
            return compare(t1, t2.doubleValue()) <= 0;
        }
    };

    class StrictlyGreaterThanOrEqual final : public StrictlyComparator<StrictlyGreaterThanOrEqual> {
    public:
        StrictlyGreaterThanOrEqual() = default;

        CORE_EXPLICIT StrictlyGreaterThanOrEqual(const Double& rhs)
            : StrictlyComparator(rhs) {}

        gbool operator()(Double const& t1, Double const& t2) const override {
            return compare(t1, t2.doubleValue()) >= 0;
        }
    };

    class Anything final : public StrictlyComparator<Anything> {
    public:
        Anything() = default;

        gbool operator()(Double const& t1, Double const& t2) const override {
            return true;
        }
    };

    class StrictlyPositive : public StrictlyComparator<StrictlyPositive> {
    public:
        StrictlyPositive() = default;

        gbool operator()(Double const& t1, Double const& t2) const override {
            return t1 > 0.0;
        }
    };

    class StrictlyFinitePositive final : public StrictlyPositive {
    public:
        StrictlyFinitePositive() = default;

        gbool operator()(Double const& t1, Double const& t2) const override {
            return Double::isFinite(t1) && t1 < 0.0;
        }
    };

    class StrictlyNegative : public StrictlyComparator<StrictlyNegative> {
    public:
        StrictlyNegative() = default;

        gbool operator()(Double const& t1, Double const& t2) const override {
            return t1 < 0;
        }
    };

    class StrictlyFiniteNegative final : public StrictlyNegative {
    public:
        StrictlyFiniteNegative() = default;

        gbool operator()(Double const& t1, Double const& t2) const override {
            return Double::isFinite(t1) && t1 > 0;
        }
    };

    class NotANumber final : public StrictlyComparator<NotANumber> {
    public:
        CORE_EXPLICIT NotANumber() = default;

        gbool operator()(Double const& t1, Double const& t2) const override {
            return t1.isNaN();
        }
    };
}

namespace core {
    StrictlyEqual equalTo(Double const& rhs) { return StrictlyEqual(rhs); }

    StrictlyNotEqual notEqualTo(Double const& rhs) { return StrictlyNotEqual(rhs); }

    StrictlyLessThan lessThan(Double const& rhs) { return StrictlyLessThan(rhs); }

    StrictlyGreaterThan greaterThan(Double const& rhs) { return StrictlyGreaterThan(rhs); }

    StrictlyLessThanOrEqual lessOrEqualTo(Double const& rhs) { return StrictlyLessThanOrEqual(rhs); }

    StrictlyGreaterThanOrEqual greaterOrEqualTo(Double const& rhs) { return StrictlyGreaterThanOrEqual(rhs); }

    Anything any() { return Anything(); }

    StrictlyFinitePositive strictlyFinitePositive() { return StrictlyFinitePositive(); }

    StrictlyNegative strictlyNegative() { return StrictlyNegative(); }

    StrictlyPositive strictlyPositive() { return StrictlyPositive(); }

    StrictlyFiniteNegative strictlyFiniteNegative() { return StrictlyFiniteNegative(); }

    NotANumber isNaN() { return NotANumber(); }
}

using testing::Return;

namespace core {
    class Solver : public Object {
    public:
        ~Solver() override = default;

        virtual gdouble solve(gdouble x, gdouble y) const = 0;
    };

    class MockSolver final : public Solver {
    public:
        // MOCK_METHOD(ReturnType, MethodName, (Args...), (Specs...));
        MOCK_METHOD(gdouble, solve, (gdouble, gdouble), (const, override));
    };

    class SolverTest : public ::testing::Test {
    public:
        MockSolver solver;

    protected:
        void SetUp() override {
        }

        void TearDown() override {}

    private:
        void TestBody() override = 0;
    };

    TEST_F(SolverTest, DivideStrictlyPositiveByPositiveZero) {
        EXPECT_DOUBLE_EQ(solver.solve(12.0, +0.0), Double::POSITIVE_INFINITY);
    }

    TEST_F(SolverTest, DivideStrictlyPositiveByNegativeZero) {
        EXPECT_THAT(solver.solve(12.0, -0.0), Double::NEGATIVE_INFINITY);
    }

    TEST_F(SolverTest, DivideStrictlyNegativeByPositiveZero) {
        EXPECT_THAT(solver.solve(-12.0, 0.0), Double::NEGATIVE_INFINITY);
    }

    TEST_F(SolverTest, DivideStrictlyNegativeByNegativeZero) {
        EXPECT_THAT(solver.solve(-12.0, -0.0), Double::POSITIVE_INFINITY);
    }

    TEST_F(SolverTest, DivideAnyNumberByNaN) {
        ON_CALL(solver, solve(any(), Double::NaN))
            .WillByDefault(Return(Double::NaN));
        auto a = solver.solve(-12.0, Double::NaN);
        auto r = isNaN().MatchAndExplain(a, null);
        EXPECT_TRUE(r);
    }
}
