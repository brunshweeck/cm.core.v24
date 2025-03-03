//
// Created by brunshweeck on 25 mai 2024.
//

#include <core/lang/ArithmeticException.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/lang/Integer.h>
#include <core/lang/Long.h>
#include "Math.h"
#include <core/random/Random.h>
#include "meta/lang/DoubleConsts.h"
#include "meta/lang/FdLibm.h"
#include "meta/lang/FloatConsts.h"
#include "meta/lang/MathUtils.h"

namespace core {
using random::Random;

// Constants used in scalb
static const gdouble twoToTheDoubleScaleUp = Math::powerOfTwoD(512);
static const gdouble twoToTheDoubleScaleDown = Math::powerOfTwoD(-512);

gdouble Math::sin(gdouble a) { return MathUtils::sin(a); }

gdouble Math::cos(gdouble a) { return MathUtils::cos(a); }

gdouble Math::tan(gdouble a) { return MathUtils::tan(a); }

gdouble Math::asin(gdouble a) { return MathUtils::asin(a); }

gdouble Math::acos(gdouble a) { return MathUtils::acos(a); }

gdouble Math::atan(gdouble a) { return MathUtils::atan(a); }

gdouble Math::toRadians(gdouble angdeg) { return angdeg * DEGREES_TO_RADIANS; }

gdouble Math::toDegrees(gdouble angrad) { return angrad * RADIANS_TO_DEGREES; }

gdouble Math::exp(gdouble a) { return FdLibm::Exp::compute(a); }

gdouble Math::log(gdouble a) {
  return MathUtils::log(a); // default impl. delegates to MathUtils
    }

    gdouble Math::log10(gdouble a) { return MathUtils::log10(a); }

    gdouble Math::log2(gdouble a) {
        if (a == 2.0)
            return 0;
        return MathUtils::log(a) / log(2);
    }

    gdouble Math::sqrt(gdouble a) {
        return MathUtils::sqrt(a); // default impl. delegates to MathUtils
        // Note that hardware sqrt instructions
        // frequently can be directly used by JITs
        // and should be much faster than doing
        // Math.sqrt in software.
    }

    gdouble Math::cbrt(gdouble a) { return FdLibm::Cbrt::compute(a); }

    gdouble Math::IEEEremainder(gdouble f1, gdouble f2) { return MathUtils::IEEEremainder(f1, f2); }

    static gdouble floorOrCeil(gdouble a,
                               gdouble negativeBoundary,
                               gdouble positiveBoundary,
                               gdouble sign) {
        gint exponent = Math::exponent(a);

        if (exponent < 0) {
            /*
             * Absolute value of argument is less than 1.
             * floorOrCeil(-0.0) => -0.0
             * floorOrCeil(+0.0) => +0.0
             */
            return ((a == 0.0) ? a : ((a < 0.0) ? negativeBoundary : positiveBoundary));
        } else if (exponent >= 52) {
            /*
             * Infinity, NaN, or a value so large it must be integral.
             */
            return a;
        }
        // Else the argument is either an integral value already XOR it
        // has to be rounded to one.
        // assert exponent >= 0 && exponent <= 51;

        glong doppel = Double::toRawLongBits(a);
        glong mask = DoubleConsts::SIGNIF_BIT_MASK >> exponent;

        if ((mask & doppel) == 0L)
            return a; // integral value
        else {
            gdouble result = Double::fromLongBits(doppel & (~mask));
            if (sign * a > 0.0)
                result = result + sign;
            return result;
        }
    }

    gdouble Math::ceil(gdouble a) { return floorOrCeil(a, -0.0, 1.0, 1.0); }

    gdouble Math::floor(gdouble a) { return floorOrCeil(a, -1.0, 0.0, -1.0); }

    gdouble Math::rint(gdouble a) {
        /*
         * If the absolute value of a is not less than 2^52, it
         * is either a finite integer (the double format does not have
         * enough significand bits for a number that large to have any
         * fractional portion), an infinity, or a NaN.  In any of
         * these cases, rint of the argument is the argument.
         *
         * Otherwise, the sum (twoToThe52 + a ) will properly round
         * away any fractional portion of a since ulp(twoToThe52) ==
         * 1.0; subtracting out twoToThe52 from this sum will then be
         * exact and leave the rounded integer portion of a.
         */
        gdouble twoToThe52 = CORE_CAST(gdouble, 1LL << 52); // 2^52
        gdouble sign = Math::copySign(1.0, a); // preserve sign info
        a = Math::abs(a);

        if (a < twoToThe52) // E_min <= ilogb(a) <= 51
            a = ((twoToThe52 + a) - twoToThe52);

        return sign * a; // restore original sign
    }

    gdouble Math::atan2(gdouble y, gdouble x) { return MathUtils::atan2(y, x); }

    gdouble Math::pow(gdouble a, gdouble b) { return FdLibm::Pow::compute(a, b); }

    gint Math::round(gfloat a) {
        gint intBits = Float::toRawIntBits(a);
        gint biasedExp = (intBits & FloatConsts::EXP_BIT_MASK)
                         >> (FloatConsts::SIGNIFICAND_WIDTH - 1);
        gint shift = (FloatConsts::SIGNIFICAND_WIDTH - 2
                      + FloatConsts::EXP_BIAS) - biasedExp;
        if ((shift & -32) == 0) {
            // shift >= 0 && shift < 32
            // a is a finite number such that pow(2,-32) <= ulp(a) < 1
            gint r = ((intBits & FloatConsts::SIGNIF_BIT_MASK)
                      | (FloatConsts::SIGNIF_BIT_MASK + 1));
            if (intBits < 0)
                r = -r;
            // In the comments below each expression evaluates to the value
            // the corresponding mathematical expression:
            // (r) evaluates to a / ulp(a)
            // (r >> shift) evaluates to floor(a * 2)
            // ((r >> shift) + 1) evaluates to floor((a + 1/2) * 2)
            // (((r >> shift) + 1) >> 1) evaluates to floor(a + 1/2)
            return ((r >> shift) + 1) >> 1;
        } else {
            // a is either
            // - a finite number with abs(a) < exp(2,FloatConsts.SIGNIFICAND_WIDTH-32) < 1/2
            // - a finite number with ulp(a) >= 1 and hence a is a mathematical integer
            // - an infinity or NaN
            return (gint) a;
        }
    }

    glong Math::round(gdouble a) {
        glong longBits = Double::toRawLongBits(a);
        glong biasedExp = (longBits & DoubleConsts::EXP_BIT_MASK)
                          >> (DoubleConsts::SIGNIFICAND_WIDTH - 1);
        glong shift = (DoubleConsts::SIGNIFICAND_WIDTH - 2
                       + DoubleConsts::EXP_BIAS) - biasedExp;
        if ((shift & -64) == 0) {
            // shift >= 0 && shift < 64
            // a is a finite number such that pow(2,-64) <= ulp(a) < 1
            glong r = ((longBits & DoubleConsts::SIGNIF_BIT_MASK)
                       | (DoubleConsts::SIGNIF_BIT_MASK + 1));
            if (longBits < 0) {
                r = -r;
            }
            // In the comments below each expression evaluates to the value
            // the corresponding mathematical expression:
            // (r) evaluates to a / ulp(a)
            // (r >> shift) evaluates to floor(a * 2)
            // ((r >> shift) + 1) evaluates to floor((a + 1/2) * 2)
            // (((r >> shift) + 1) >> 1) evaluates to floor(a + 1/2)
            return ((r >> shift) + 1) >> 1;
        } else {
            // - a is either
            // - a finite number with abs(a) < exp(2,DoubleConsts::SIGNIFICAND_WIDTH-64) < 1/2
            // - a finite number with ulp(a) >= 1 and hence a is a mathematical integer
            // - an infinity or NaN
            return (glong) a;
        }
    }

    static Random rng = {};
    gint startCount = 0;

    gdouble Math::random() {
        random::Random &random = rng;
        if (startCount == 0) {
            random = random::Random();
            startCount = 1;
        }
        return random.nextDouble();
    }

    gint Math::addExact(gint x, gint y) {
        gint r = x + y;
        // HD 2-12 Overflow iff both arguments have the opposite sign of the result
        if (((x ^ r) & (y ^ r)) < 0)
            ArithmeticException("integer overflow"_S).throws($ftrace());
        return r;
    }

    glong Math::addExact(glong x, glong y) {
        glong r = x + y;
        // HD 2-12 Overflow iff both arguments have the opposite sign of the result
        if (((x ^ r) & (y ^ r)) < 0)
            ArithmeticException("glong overflow"_S).throws($ftrace());
        return r;
    }

    gint Math::subtractExact(gint x, gint y) {
        gint r = x - y;
        // HD 2-12 Overflow iff the arguments have different signs and
        // the sign of the result is different from the sign of x
        if (((x ^ y) & (x ^ r)) < 0)
            ArithmeticException("integer overflow"_S).throws($ftrace());
        return r;
    }

    glong Math::subtractExact(glong x, glong y) {
        glong r = x - y;
        // HD 2-12 Overflow iff the arguments have different signs and
        // the sign of the result is different from the sign of x
        if (((x ^ y) & (x ^ r)) < 0) {
            ArithmeticException("glong overflow"_S).throws($ftrace());
        }
        return r;
    }

    gint Math::multiplyExact(gint x, gint y) {
        glong r = (glong) x * (glong) y;
        if ((gint) r != r) {
            ArithmeticException("integer overflow"_S).throws($ftrace());
        }
        return (gint) r;
    }

    glong Math::multiplyExact(glong x, gint y) {
        return multiplyExact(x, (glong) y);
    }

    glong Math::multiplyExact(glong x, glong y) {
        glong r = x * y;
        glong ax = Math::abs(x);
        glong ay = Math::abs(y);
        if (((ax | ay) + 0u) >> 31 != 0) {
            // Some bits greater than 2^31 that might cause overflow
            // Check the result using the divide operator
            // and check for the special case of Long::MIN_VALUE * -1
            if (((y != 0) && (r / y != x)) ||
                (x == Long::MIN_VALUE && y == -1)) {
                ArithmeticException("glong overflow"_S).throws($ftrace());
            }
        }
        return r;
    }

    gint Math::divideExact(gint x, gint y) {
        gint q = x / y;
        if ((x & y & q) >= 0)
            return q;
        ArithmeticException("integer overflow"_S).throws($ftrace());
    }

    glong Math::divideExact(glong x, glong y) {
        glong q = x / y;
        if ((x & y & q) >= 0)
            return q;
        ArithmeticException("glong overflow"_S).throws($ftrace());
    }

    gint Math::floorDivExact(gint x, gint y) {
        const gint q = x / y;
        if ((x & y & q) >= 0) {
            // if the signs are different and modulo not zero, round down
            if ((x ^ y) < 0 && (q * y != x)) {
                return q - 1;
            }
            return q;
        }
        ArithmeticException("integer overflow"_S).throws($ftrace());
    }

    glong Math::floorDivExact(glong x, glong y) {
        const glong q = x / y;
        if ((x & y & q) >= 0) {
            // if the signs are different and modulo not zero, round down
            if ((x ^ y) < 0 && (q * y != x)) {
                return q - 1;
            }
            return q;
        }
        ArithmeticException("glong overflow"_S).throws($ftrace());
    }

    gint Math::ceilDivExact(gint x, gint y) {
        const gint q = x / y;
        if ((x & y & q) >= 0) {
            // if the signs are the same and modulo not zero, round up
            if ((x ^ y) >= 0 && (q * y != x)) {
                return q + 1;
            }
            return q;
        }
        ArithmeticException("integer overflow"_S).throws($ftrace());
    }

    glong Math::ceilDivExact(glong x, glong y) {
        const glong q = x / y;
        if ((x & y & q) >= 0) {
            // if the signs are the same and modulo not zero, round up
            if ((x ^ y) >= 0 && (q * y != x)) {
                return q + 1;
            }
            return q;
        }
        ArithmeticException("glong overflow"_S).throws($ftrace());
    }

    gint Math::incrementExact(gint a) {
        if (a == Integer::MAX_VALUE) {
            ArithmeticException("integer overflow"_S).throws($ftrace());
        }

        return a + 1;
    }

    glong Math::incrementExact(glong a) {
        if (a == Long::MAX_VALUE) {
            ArithmeticException("glong overflow"_S).throws($ftrace());
        }

        return a + 1LL;
    }

    gint Math::decrementExact(gint a) {
        if (a == Integer::MIN_VALUE) {
            ArithmeticException("integer overflow"_S).throws($ftrace());
        }

        return a - 1;
    }

    glong Math::decrementExact(glong a) {
        if (a == Long::MIN_VALUE) {
            ArithmeticException("glong overflow"_S).throws($ftrace());
        }

        return a - 1LL;
    }

    gint Math::negateExact(gint a) {
        if (a == Integer::MIN_VALUE) {
            ArithmeticException("integer overflow"_S).throws($ftrace());
        }

        return -a;
    }

    glong Math::negateExact(glong a) {
        if (a == Long::MIN_VALUE) {
            ArithmeticException("glong overflow"_S).throws($ftrace());
        }

        return -a;
    }

    gint Math::toIntExact(glong value) {
        if ((gint) value != value) {
            ArithmeticException("integer overflow"_S).throws($ftrace());
        }
        return (gint) value;
    }

    glong Math::multiplyFull(gint x, gint y) {
        return (glong) x * (glong) y;
    }

    glong Math::multiplyHigh(glong x, glong y) {
        // Use technique from section 8-2 of Henry S. Warren, Jr.,
        // Hacker's Delight (2nd ed.) (Addison Wesley, 2013), 173-174.
        glong x1 = x >> 32;
        glong x2 = x & 0xFFFFFFFFL;
        glong y1 = y >> 32;
        glong y2 = y & 0xFFFFFFFFL;

        glong z2 = x2 * y2;
        glong t = x1 * y2 + ((z2 + 0u) >> 32);
        glong z1 = t & 0xFFFFFFFFL;
        glong z0 = t >> 32;
        z1 += x2 * y1;

        return x1 * y1 + z0 + (z1 >> 32);
    }

    glong Math::unsignedMultiplyHigh(glong x, glong y) {
        // Compute via multiplyHigh() to leverage the intrinsic
        glong result = Math::multiplyHigh(x, y);
        result += (y & (x >> 63)); // equivalent to `if (x < 0) result += y;`
        result += (x & (y >> 63)); // equivalent to `if (y < 0) result += x;`
        return result;
    }

    gint Math::floorDiv(gint x, gint y) {
        const gint q = x / y;
        // if the signs are different and modulo not zero, round down
        if ((x ^ y) < 0 && (q * y != x)) {
            return q - 1;
        }
        return q;
    }

    gint Math::floorDiv(glong x, gint y) {
        return (gint) floorDiv(x, (glong) y);
    }

    glong Math::floorDiv(glong x, glong y) {
        const glong q = x / y;
        // if the signs are different and modulo not zero, round down
        if ((x ^ y) < 0 && (q * y != x)) {
            return q - 1;
        }
        return q;
    }

    gint Math::floorMod(gint x, gint y) {
        const gint r = x % y;
        // if the signs are different and modulo not zero, adjust result
        if ((x ^ y) < 0 && r != 0) {
            return r + y;
        }
        return r;
    }

    gint Math::floorMod(glong x, gint y) {
        // Result cannot overflow the range of int.
        return (gint) floorMod(x, (glong) y);
    }

    glong Math::floorMod(glong x, glong y) {
        const glong r = x % y;
        // if the signs are different and modulo not zero, adjust result
        if ((x ^ y) < 0 && r != 0)
            return r + y;
        return r;
    }

    gint Math::ceilDiv(gint x, gint y) {
        const gint q = x / y;
        // if the signs are the same and modulo not zero, round up
        if ((x ^ y) >= 0 && (q * y != x))
            return q + 1;
        return q;
    }

    gint Math::ceilDiv(glong x, gint y) {
        return (gint) ceilDiv(x, (glong) y);
    }

    glong Math::ceilDiv(glong x, glong y) {
        const glong q = x / y;
        // if the signs are the same and modulo not zero, round up
        if ((x ^ y) >= 0 && (q * y != x))
            return q + 1;
        return q;
    }

    gint Math::ceilMod(gint x, gint y) {
        const gint r = x % y;
        // if the signs are the same and modulo not zero, adjust result
        if ((x ^ y) >= 0 && r != 0)
            return r - y;
        return r;
    }

    gint Math::ceilMod(glong x, gint y) {
        // Result cannot overflow the range of int.
        return (gint) ceilMod(x, (glong) y);
    }

    glong Math::ceilMod(glong x, glong y) {
        const glong r = x % y;
        // if the signs are the same and modulo not zero, adjust result
        if ((x ^ y) >= 0 && r != 0)
            return r - y;
        return r;
    }

    gint Math::abs(gint a) {
        return (a < 0) ? -a : a;
    }

    gint Math::absExact(gint a) {
        if (a == Integer::MIN_VALUE)
            ArithmeticException(
                "Overflow to represent absolute value of Integer::MIN_VALUE"_S).throws($ftrace());
        return abs(a);
    }

    glong Math::abs(glong a) {
        return (a < 0) ? -a : a;
    }

    glong Math::absExact(glong a) {
        if (a == Long::MIN_VALUE)
            ArithmeticException("Overflow to represent absolute value of Long::MIN_VALUE"_S)
                    .throws($ftrace());
        return abs(a);
    }

    gfloat Math::abs(gfloat a) {
        // Convert to bit field form, zero the sign bit, and convert back
        return Float::fromIntBits(Float::toRawIntBits(a) & FloatConsts::MAG_BIT_MASK);
    }

    gdouble Math::abs(gdouble a) {
        // Convert to bit field form, zero the sign bit, and convert back
        return Double::fromLongBits(Double::toRawLongBits(a) & DoubleConsts::MAG_BIT_MASK);
    }

    gint Math::max(gint a, gint b) {
        return (a >= b) ? a : b;
    }

    glong Math::max(glong a, glong b) {
        return (a >= b) ? a : b;
    }

    gfloat Math::max(gfloat a, gfloat b) {
        if (a != a)
            return a; // a is NaN
        if ((a == 0.0f) &&
            (b == 0.0f) &&
            (Float::toRawIntBits(a) == Float::toRawIntBits(-0.0f))) {
            // Raw conversion ok since NaN can't map to -0.0.
            return b;
        }
        return (a >= b) ? a : b;
    }

    gdouble Math::max(gdouble a, gdouble b) {
        if (a != a)
            return a; // a is NaN
        if ((a == 0.0) &&
            (b == 0.0) &&
            (Double::toRawLongBits(a) == Double::toRawLongBits(-0.0))) {
            // Raw conversion ok since NaN can't map to -0.0.
            return b;
        }
        return (a >= b) ? a : b;
    }

    gint Math::min(gint a, gint b) {
        return (a <= b) ? a : b;
    }

    glong Math::min(glong a, glong b) {
        return (a <= b) ? a : b;
    }

    gfloat Math::min(gfloat a, gfloat b) {
        if (a != a)
            return a; // a is NaN
        if ((a == 0.0f) &&
            (b == 0.0f) &&
            (Float::toRawIntBits(b) == Float::toRawIntBits(-0.0f))) {
            // Raw conversion ok since NaN can't map to -0.0.
            return b;
        }
        return (a <= b) ? a : b;
    }

    gdouble Math::min(gdouble a, gdouble b) {
        if (a != a)
            return a; // a is NaN
        if ((a == 0.0) &&
            (b == 0.0) &&
            (Double::toRawLongBits(b) == Double::toRawLongBits(-0.0))) {
            // Raw conversion ok since NaN can't map to -0.0.
            return b;
        }
        return (a <= b) ? a : b;
    }

    gint Math::clamp(glong value, gint min, gint max) {
        if (min > max) {
            IllegalArgumentException(min + " > "_S + max).throws($ftrace());
        }
        return (gint) Math::min((glong) max, Math::max(value, (glong) min));
    }

    glong Math::clamp(glong value, glong min, glong max) {
        if (min > max) {
            IllegalArgumentException(min + " > "_S + max).throws($ftrace());
        }
        return Math::min(max, Math::max(value, min));
    }

    gdouble Math::clamp(gdouble value, gdouble min, gdouble max) {
        // This unusual condition allows keeping only one branch
        // on common path when min < max and neither of them is NaN.
        // If min == max, we should additionally check for +0.0/-0.0 case,
        // so we're still visiting the if statement.
        if (!(min < max)) {
            // min greater than, equal to, or unordered with respect to max; NaN values are unordered
            if (Double::isNaN(min)) {
                IllegalArgumentException("min is NaN"_S).throws($ftrace());
            }
            if (Double::isNaN(max)) {
                IllegalArgumentException("max is NaN"_S).throws($ftrace());
            }
            if (Double::compare(min, max) > 0) {
                IllegalArgumentException(min + " > "_S + max).throws($ftrace());
            }
            // Fall-through if min and max are exactly equal (or min = -0.0 and max = +0.0)
            // and none of them is NaN
        }
        return Math::min(max, Math::max(value, min));
    }

    gfloat Math::clamp(gfloat value, gfloat min, gfloat max) {
        // This unusual condition allows keeping only one branch
        // on common path when min < max and neither of them is NaN.
        // If min == max, we should additionally check for +0.0/-0.0 case,
        // so we're still visiting the if statement.
        if (!(min < max)) {
            // min greater than, equal to, or unordered with respect to max; NaN values are unordered
            if (Float::isNaN(min)) {
                IllegalArgumentException("min is NaN"_S).throws($ftrace());
            }
            if (Float::isNaN(max)) {
                IllegalArgumentException("max is NaN"_S).throws($ftrace());
            }
            if (Float::compare(min, max) > 0) {
                IllegalArgumentException(min + " > "_S + max).throws($ftrace());
            }
            // Fall-through if min and max are exactly equal (or min = -0.0 and max = +0.0)
            // and none of them is NaN
        }
        return Math::min(max, Math::max(value, min));
    }

    gdouble Math::fma(gdouble a, gdouble b, gdouble c) {
        /*
         * Infinity and NaN arithmetic is not quite the same with two
         * roundings as opposed to just one so the simple expression
         * "a * b + c" cannot always be used to compute the correct
         * result.  With two roundings, the product can overflow and
         * if the addend is infinite, a spurious NaN can be produced
         * if the infinity from the overflow and the infinite addend
         * have opposite signs.
         */

        // First, screen for and handle non-finite input values whose
        // arithmetic is not supported by BigDecimal.
        if (Double::isNaN(a) || Double::isNaN(b) || Double::isNaN(c)) {
            return Double::NaN;
        } else {
            // All inputs non-NaN
            gbool infiniteA = Double::isInfinite(a);
            gbool infiniteB = Double::isInfinite(b);
            gbool infiniteC = Double::isInfinite(c);
            gdouble result;

            if (infiniteA || infiniteB || infiniteC) {
                if (infiniteA && b == 0.0 ||
                    infiniteB && a == 0.0) {
                    return Double::NaN;
                }
                gdouble product = a * b;
                if (Double::isInfinite(product) && !infiniteA && !infiniteB) {
                    // Intermediate overflow; might cause a
                    // spurious NaN if added to infinite c.
                    // assert Double::isInfinite(c);
                    return c;
                } else {
                    result = product + c;
                    /// assert !Double::isFinite(result);
                    return result;
                }
            } else {
                return MathUtils::fma(a, b, c);
            }
        }
    }

    gfloat Math::fma(gfloat a, gfloat b, gfloat c) {
        if (Float::isFinite(a) && Float::isFinite(b) && Float::isFinite(c)) {
            if (a == 0.0F || b == 0.0F) {
                return a * b + c; // Handled signed zero cases
            } else {
                //                return (BigDecimal((gdouble) a * (gdouble) b) // Exact multiply
                //                        .add(BigDecimal((gdouble) c)))      // Exact sum
                //                        .floatValue();                            // One rounding
                //                // to a float value
                return MathUtils::fma(a, b, c);
            }
        } else {
            // At least one of a,b, and c is non-finite. The result
            // will be non-finite as well and will be the same
            // non-finite value under double as float arithmetic.
            return (gfloat) fma((gdouble) a, (gdouble) b, (gdouble) c);
        }
    }

    gdouble Math::ulp(gdouble d) {
        gint exp = exponent(d);

        switch (exp) {
            case Double::MAX_EXPONENT + 1:
                return Math::abs(d); // NaN or infinity
            case Double::MIN_EXPONENT - 1:
                return Double::MIN_VALUE; // zero or subnormal
            default: {
                /// assert exp <= Double::MAX_EXPONENT && exp >= Double::MIN_EXPONENT;

                // ulp(x) is usually 2^(SIGNIFICAND_WIDTH-1)*(2^ilogb(x))
                exp = exp - (DoubleConsts::SIGNIFICAND_WIDTH - 1);
                if (exp >= Double::MIN_EXPONENT) {
                    return powerOfTwoD(exp);
                } else {
                    // return a subnormal result; left shift integer
                    // representation of Double::MIN_VALUE appropriate
                    // number of positions
                    return Double::fromLongBits(1L <<
                                                (exp -
                                                 (Double::MIN_EXPONENT - (DoubleConsts::SIGNIFICAND_WIDTH - 1))));
                }
            }
        }
    }

    gfloat Math::ulp(gfloat f) {
        gint exp = exponent(f);

        switch (exp) {
            case Float::MAX_EXPONENT + 1:
                return Math::abs(f); // NaN or infinity
            case Float::MIN_EXPONENT - 1:
                return Float::MIN_VALUE; // zero or subnormal
            default: {
                /// assert exp <= Float::MAX_EXPONENT && exp >= Float::MIN_EXPONENT;

                // ulp(x) is usually 2^(SIGNIFICAND_WIDTH-1)*(2^ilogb(x))
                exp = exp - (FloatConsts::SIGNIFICAND_WIDTH - 1);
                if (exp >= Float::MIN_EXPONENT) {
                    return powerOfTwoF(exp);
                } else {
                    // return a subnormal result; left shift integer
                    // representation of FloatConsts::MIN_VALUE appropriate
                    // number of positions
                    return Float::fromIntBits(1 <<
                                              (exp - (Float::MIN_EXPONENT - (FloatConsts::SIGNIFICAND_WIDTH - 1))));
                }
            }
        }
    }

    gdouble Math::signum(gdouble d) {
        return (d == 0.0 || Double::isNaN(d)) ? d : copySign(1.0, d);
    }

    gfloat Math::signum(gfloat f) {
        return (f == 0.0f || Float::isNaN(f)) ? f : copySign(1.0f, f);
    }

    gdouble Math::sinh(gdouble x) {
        return MathUtils::sinh(x);
    }

    gdouble Math::cosh(gdouble x) {
        return MathUtils::cosh(x);
    }

    gdouble Math::tanh(gdouble x) {
        return MathUtils::tanh(x);
    }

    gdouble Math::hypot(gdouble x, gdouble y) {
        return FdLibm::Hypot::compute(x, y);
    }

    gdouble Math::expm1(gdouble x) {
        return MathUtils::expm1(x);
    }

    gdouble Math::log1p(gdouble x) {
        return MathUtils::log1p(x);
    }

    gdouble Math::copySign(gdouble magnitude, gdouble sign) {
        return Double::fromLongBits((Double::toRawLongBits(sign) &
                                     DoubleConsts::SIGN_BIT_MASK) |
                                    (Double::toRawLongBits(magnitude) &
                                     (DoubleConsts::EXP_BIT_MASK |
                                      DoubleConsts::SIGNIF_BIT_MASK)));
    }

    gfloat Math::copySign(gfloat magnitude, gfloat sign) {
        return Float::fromIntBits((Float::toRawIntBits(sign) &
                                   FloatConsts::SIGN_BIT_MASK) |
                                  (Float::toRawIntBits(magnitude) &
                                   (FloatConsts::EXP_BIT_MASK |
                                    FloatConsts::SIGNIF_BIT_MASK)));
    }

    gint Math::exponent(gfloat f) {
        /*
         * Bitwise convert f to integer, mask out exponent bits, shift
         * to the right and then subtract out float's bias adjust to
         * get true exponent value
         */
        return ((Float::toRawIntBits(f) & FloatConsts::EXP_BIT_MASK) >>
                (FloatConsts::SIGNIFICAND_WIDTH - 1)) -
               FloatConsts::EXP_BIAS;
    }

    gint Math::exponent(gdouble d) {
        /*
         * Bitwise convert d to long, mask out exponent bits, shift
         * to the right and then subtract out double's bias adjust to
         * get true exponent value.
         */
        return (gint) (((Double::toRawLongBits(d) & DoubleConsts::EXP_BIT_MASK) >>
                        (DoubleConsts::SIGNIFICAND_WIDTH - 1)) -
                       DoubleConsts::EXP_BIAS);
    }

    gdouble Math::nextAfter(gdouble start, gdouble direction) {
        /*
         * The cases:
         *
         * nextAfter(+infinity, 0)  == MAX_VALUE
         * nextAfter(+infinity, +infinity)  == +infinity
         * nextAfter(-infinity, 0)  == -MAX_VALUE
         * nextAfter(-infinity, -infinity)  == -infinity
         *
         * are naturally handled without any additional testing
         */

        /*
         * IEEE 754 floating-point numbers are lexicographically
         * ordered if treated as signed-magnitude integers.
         * Since integers are two's complement, incrementing the two's
         * complement representation of a logically negative floating-point
         * value *decrements* the signed-magnitude representation. Therefore, when
         * the integer representation of a floating-point value
         * is negative, the adjustment to the representation is in
         * the opposite direction from what would initially be expected.
         */

        // Branch to descending case first as it is more costly than ascending
        // case due to start != 0.0d conditional.
        if (start > direction) {
            // descending
            if (start != 0.0) {
                const glong transducer = Double::toRawLongBits(start);
                return Double::fromLongBits(transducer + ((transducer > 0L) ? -1L : 1L));
            } else {
                // start == 0.0d && direction < 0.0d
                return -Double::MIN_VALUE;
            }
        } else if (start < direction) {
            // ascending
            // Add +0.0 to get rid of a -0.0 (+0.0 + -0.0 => +0.0)
            // then bitwise convert start to integer.
            const glong transducer = Double::toRawLongBits(start + 0.0);
            return Double::fromLongBits(transducer + ((transducer >= 0L) ? 1L : -1L));
        } else if (start == direction) {
            return direction;
        } else {
            /// isNaN(start) || isNaN(direction)
            return start + direction;
        }
    }

    gfloat Math::nextAfter(gfloat start, gfloat direction) {
        /*
         * The cases:
         *
         * nextAfter(+infinity, 0)  == MAX_VALUE
         * nextAfter(+infinity, +infinity)  == +infinity
         * nextAfter(-infinity, 0)  == -MAX_VALUE
         * nextAfter(-infinity, -infinity)  == -infinity
         *
         * are naturally handled without any additional testing
         */

        /*
         * IEEE 754 floating-point numbers are lexicographically
         * ordered if treated as signed-magnitude integers.
         * Since integers are two's complement,
         * incrementing the two's complement representation of a
         * logically negative floating-point value *decrements*
         * the signed-magnitude representation. Therefore, when
         * the integer representation of a floating-point value
         * is negative, the adjustment to the representation is in
         * the opposite direction from what would initially be expected.
         */

        // Branch to descending case first as it is more costly than ascending
        // case due to start != 0.0f conditional.
        if (start > direction) {
            // descending
            if (start != 0.0f) {
                const gint transducer = Float::toRawIntBits(start);
                return Float::fromIntBits(transducer + ((transducer > 0) ? -1 : 1));
            } else {
                // start == 0.0f && direction < 0.0f
                return -Float::MIN_VALUE;
            }
        } else if (start < direction) {
            // ascending
            // Add +0.0 to get rid of a -0.0 (+0.0 + -0.0 => +0.0)
            // then bitwise convert start to integer.
            const gint transducer = Float::toRawIntBits(start + 0.0f);
            return Float::fromIntBits(transducer + ((transducer >= 0) ? 1 : -1));
        } else if (start == direction) {
            return direction;
        } else {
            /// isNaN(start) || isNaN(direction)
            return start + direction;
        }
    }

    gdouble Math::nextUp(gdouble d) {
        // Use a single conditional and handle the likely cases first.
        if (d < Double::POSITIVE_INFINITY) {
            // Add +0.0 to get rid of a -0.0 (+0.0 + -0.0 => +0.0).
            const glong transducer = Double::toRawLongBits(d + 0.0);
            return Double::fromLongBits(transducer + ((transducer >= 0L) ? 1L : -1L));
        } else {
            // d is NaN or +Infinity
            return d;
        }
    }

    gfloat Math::nextUp(gfloat f) {
        // Use a single conditional and handle the likely cases first.
        if (f < Float::POSITIVE_INFINITY) {
            // Add +0.0 to get rid of a -0.0 (+0.0 + -0.0 => +0.0).
            const gint transducer = Float::toRawIntBits(f + 0.0F);
            return Float::fromIntBits(transducer + ((transducer >= 0) ? 1 : -1));
        } else {
            // f is NaN or +Infinity
            return f;
        }
    }

    gdouble Math::nextDown(gdouble d) {
        if (Double::isNaN(d) || d == Double::NEGATIVE_INFINITY)
            return d;
        else {
            if (d == 0.0)
                return -Double::MIN_VALUE;
            else
                return Double::fromLongBits(Double::toRawLongBits(d) +
                                            ((d > 0.0) ? -1L : +1L));
        }
    }

    gfloat Math::nextDown(gfloat f) {
        if (Float::isNaN(f) || f == Float::NEGATIVE_INFINITY)
            return f;
        else {
            if (f == 0.0f)
                return -Float::MIN_VALUE;
            else
                return Float::fromIntBits(Float::toRawIntBits(f) +
                                          ((f > 0.0f) ? -1 : +1));
        }
    }

    gdouble Math::scalb(gdouble d, gint scaleFactor) {
        /*
         * When scaling up, it does not matter what order the
         * multiply-store operations are done; the result will be
         * finite or overflow regardless of the operation ordering.
         * However, to get the correct result when scaling down, a
         * particular ordering must be used.
         *
         * When scaling down, the multiply-store operations are
         * sequenced so that it is not possible for two consecutive
         * multiply-stores to return subnormal results.  If one
         * multiply-store result is subnormal, the next multiply will
         * round it away to zero.  This is done by first multiplying
         * by 2 ^ (scaleFactor % n) and then multiplying several
         * times by 2^n as needed where n is the exponent of number
         * that is a convenient power of two.  In this way, at most one
         * real rounding error occurs.
         */

        // magnitude of a power of two so large that scaling a finite
        // nonzero value by it would be guaranteed to over or
        // underflow; due to rounding, scaling down takes an
        // additional power of two which is reflected here
        const gint MAX_SCALE = Double::MAX_EXPONENT + -Double::MIN_EXPONENT +
                               DoubleConsts::SIGNIFICAND_WIDTH + 1;
        gint exp_adjust = 0;
        gint scale_increment = 0;
        gdouble exp_delta = Double::NaN;

        // Make sure scaling factor is in a reasonable range

        if (scaleFactor < 0) {
            scaleFactor = Math::max(scaleFactor, -MAX_SCALE);
            scale_increment = -512;
            exp_delta = twoToTheDoubleScaleDown;
        } else {
            scaleFactor = Math::min(scaleFactor, MAX_SCALE);
            scale_increment = 512;
            exp_delta = twoToTheDoubleScaleUp;
        }

        // Calculate (scaleFactor % +/-512), 512 = 2^9, using
        // technique from "Hacker's Delight" section 10-2.
        gint t = (gint) (((scaleFactor >> (9 - 1)) + 0u) >> (32 - 9));
        exp_adjust = ((scaleFactor + t) & (512 - 1)) - t;

        d *= powerOfTwoD(exp_adjust);
        scaleFactor -= exp_adjust;

        while (scaleFactor != 0) {
            d *= exp_delta;
            scaleFactor -= scale_increment;
        }
        return d;
    }

    gfloat Math::scalb(gfloat f, gint scaleFactor) {
        // magnitude of a power of two so large that scaling a finite
        // nonzero value by it would be guaranteed to over or
        // underflow; due to rounding, scaling down takes an
        // additional power of two which is reflected here
        const gint MAX_SCALE = Float::MAX_EXPONENT + -Float::MIN_EXPONENT +
                               FloatConsts::SIGNIFICAND_WIDTH + 1;

        // Make sure scaling factor is in a reasonable range
        scaleFactor = Math::max(Math::min(scaleFactor, MAX_SCALE), -MAX_SCALE);

        /*
         * Since + MAX_SCALE for float fits well within the double
         * exponent range and + float -> double conversion is exact
         * the multiplication below will be exact. Therefore, the
         * rounding that occurs when the double product is cast to
         * float will be the correctly rounded float result.
         */
        return (gfloat) ((gdouble) f * powerOfTwoD(scaleFactor));
    }

    gdouble Math::powerOfTwoD(gint n) {
        if (!(n >= Double::MIN_EXPONENT && n <= Double::MAX_EXPONENT))
            return Double::NaN;
        return Double::fromLongBits((((glong) n + (glong) DoubleConsts::EXP_BIAS) <<
                                     (DoubleConsts::SIGNIFICAND_WIDTH - 1))
                                    & DoubleConsts::EXP_BIT_MASK);
    }

    gfloat Math::powerOfTwoF(gint n) {
        if (!(n >= Float::MIN_EXPONENT && n <= Float::MAX_EXPONENT))
            return Float::NaN;
        return Float::fromIntBits(((n + FloatConsts::EXP_BIAS) <<
                                   (FloatConsts::SIGNIFICAND_WIDTH - 1))
                                  & FloatConsts::EXP_BIT_MASK);
    }
    } // namespace core
