//
// Created by brunshweeck on 26 mai 2024.
//

#ifndef CORE24_FLOATCONSTS_H
#define CORE24_FLOATCONSTS_H

#include <core/Float.h>

namespace core
{

    class FloatConsts final
    {
    private:
        CORE_EXPLICIT FloatConsts();

    public:

        /**
         * The number of logical bits in the significand of a
         * @c gfloat number, including the implicit bit.
         */
        static CORE_FAST int SIGNIFICAND_WIDTH = Float::PRECISION;

        /**
         * The exponent the smallest positive @c gfloat
         * subnormal value would have if it could be normalized.
         */
        static CORE_FAST int MIN_SUB_EXPONENT = Float::MIN_EXPONENT - (SIGNIFICAND_WIDTH - 1); // -149

        /**
         * Bias used in representing a @c gfloat exponent.
         */
        static CORE_FAST int EXP_BIAS = (1 << (Float::SIZE - SIGNIFICAND_WIDTH - 1)) - 1; // 127

        /**
         * Bit mask to isolate the sign bit of a @c gfloat.
         */
        static CORE_FAST int SIGN_BIT_MASK = 1 << (Float::SIZE - 1);

        /**
         * Bit mask to isolate the exponent field of a @c gfloat.
         */
        static CORE_FAST int EXP_BIT_MASK =
                ((1 << (Float::SIZE - SIGNIFICAND_WIDTH)) - 1) << (SIGNIFICAND_WIDTH - 1);

        /**
         * Bit mask to isolate the significand field of a @c gfloat.
         */
        static CORE_FAST int SIGNIF_BIT_MASK = (1 << (SIGNIFICAND_WIDTH - 1)) - 1;

        /**
         * Bit mask to isolate the magnitude bits (combined exponent and
         * significand fields) of a @c gfloat.
         */
        static CORE_FAST int MAG_BIT_MASK = EXP_BIT_MASK | SIGNIF_BIT_MASK;

        // verify bit masks cover all bit positions and that the bit
        // masks are non-overlapping
        CORE_FAST_ASSERT(
                (SIGN_BIT_MASK | EXP_BIT_MASK | SIGNIF_BIT_MASK) == ~0
                && (SIGN_BIT_MASK & EXP_BIT_MASK) == 0
                && (SIGN_BIT_MASK & SIGNIF_BIT_MASK) == 0
                && (EXP_BIT_MASK & SIGNIF_BIT_MASK) == 0
                && (SIGN_BIT_MASK | MAG_BIT_MASK) == ~0
        );
    };

} // core

#endif //CORE24_FLOATCONSTS_H
