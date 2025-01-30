//
// Created by brunshweeck on 26 mai 2024.
//

#ifndef CORE24_DOUBLECONSTS_H
#define CORE24_DOUBLECONSTS_H

#include <core/Double.h>

namespace core {
    class DoubleConsts final : public Object {
     CORE_EXPLICIT DoubleConsts();

    public:
        /**
         * The number of logical bits in the significand of a
         * @c gdouble number, including the implicit bit.
         */
        static CORE_FAST int SIGNIFICAND_WIDTH = Double::PRECISION;

        /**
         * The exponent the smallest positive @c gdouble
         * subnormal value would have if it could be normalized..
         */
        static CORE_FAST int MIN_SUB_EXPONENT =
                Double::MIN_EXPONENT - (SIGNIFICAND_WIDTH - 1); // -1074

        /**
         * Bias used in representing a @c gdouble exponent.
         */
        static CORE_FAST int EXP_BIAS =
                (1 << (Double::SIZE - SIGNIFICAND_WIDTH - 1)) - 1; // 1023

        /**
         * Bit mask to isolate the sign bit of a @c gdouble.
         */
        static CORE_FAST glong SIGN_BIT_MASK = 1LL << (Double::SIZE - 1);

        /**
         * Bit mask to isolate the exponent field of a @c gdouble.
         */
        static CORE_FAST glong EXP_BIT_MASK =
                ((1LL << (Double::SIZE - SIGNIFICAND_WIDTH)) - 1) << (SIGNIFICAND_WIDTH - 1);

        /**
         * Bit mask to isolate the significand field of a @c gdouble.
         */
        static CORE_FAST glong SIGNIF_BIT_MASK = (1LL << (SIGNIFICAND_WIDTH - 1)) - 1;

        /**
         * Bit mask to isolate the magnitude bits (combined exponent and
         * significand fields) of a @c gdouble.
         */
        static CORE_FAST glong MAG_BIT_MASK = EXP_BIT_MASK | SIGNIF_BIT_MASK;

        CORE_FAST_ASSERT(
            (SIGN_BIT_MASK | EXP_BIT_MASK | SIGNIF_BIT_MASK) == ~0LL
            && (SIGN_BIT_MASK & EXP_BIT_MASK) == 0LL
            && (SIGN_BIT_MASK & SIGNIF_BIT_MASK) == 0LL
            && (EXP_BIT_MASK & SIGNIF_BIT_MASK) == 0LL
            && (SIGN_BIT_MASK | MAG_BIT_MASK) == ~0LL
        );
    };
} // core

#endif //CORE24_DOUBLECONSTS_H
