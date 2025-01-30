//
// Created by brunshweeck on 24 août 2024.
//

#ifndef CORE24_WELL44497A_H
#define CORE24_WELL44497A_H

#include <core/random/RandomGenerator.h>

namespace core {
    namespace random {
        /**
         * This class implements the Well44497a pseudo-random number generator
         * from François Panneton, Pierre L'Ecuyer and Makoto Matsumoto.
         * <p>
         * This generator is described in a paper by François Panneton,
         * Pierre L'Ecuyer and Makoto Matsumoto
         * <a href="http://www.iro.umontreal.ca/~lecuyer/myftp/papers/wellrng.pdf">
         * Improved Long-Period Generators Based on Linear Recurrences Modulo 2</a>
         * ACM Transactions on Mathematical Software, 32, 1 (2006). The errata for the paper
         * are in <a href="http://www.iro.umontreal.ca/~lecuyer/myftp/papers/wellrng-errata.txt">
         * wellrng-errata.txt</a>.
         * </p>
         * @see <a href="http://www.iro.umontreal.ca/~panneton/WELLRNG.html">WELL Random number generator</a>
         */
        class Well44497a final : public RandomGenerator {
            /**
             * The seed generator for default constructors.
             */
            static atomic::AtomicLong DEFAULT_GENERATOR;

            static CORE_FAST gint W = 32;

            /** Number of bits in the pool. */
            static CORE_FAST gint K = 44497;

            /** First parameter of the algorithm. */
            static CORE_FAST gint M1 = 23;

            /** Second parameter of the algorithm. */
            static CORE_FAST gint M2 = 481;

            /** Third parameter of the algorithm. */
            static CORE_FAST gint M3 = 229;

            static CORE_FAST gint R = (K - (W - 1)) / W;

            /* ------------- Privates fields -------------------*/

            /**
             * Current index in the bytes pool.
             *
             */
            gint mutable index = 0;

            /**
             * Bytes pool.
             */
            gint mutable v[R] = {};
            /**
             * Index indirection table giving for each index
             * its predecessor taking table size into account.
             *
             */
            gint iRm1[R] = {};
            /**
             * Index indirection table giving for each index
             * its second predecessor taking table size into account.
             *
             */
            gint iRm2[R] = {};
            /**
             * Index indirection table giving for each index the value
             * index + m1 taking table size into account.
             */
            gint i1[R] = {};
            /**
             * Index indirection table giving for each index the value
             * index + m2 taking table size into account.
             */
            gint mutable i2[R] = {};
            /**
             * Index indirection table giving for each index the value
             * index + m3 taking table size into account.
             */
            gint mutable i3[R] = {};

            gdouble mutable nxtGaussian = Double::NaN;

        public:
            /**
             * Creates a new random number generator.
             *
             */
            CORE_IMPLICIT Well44497a();

            /**
             * Creates a new random number generator using a single int seed.
             * @param seed the initial seed (32 bits integer)
             */
            CORE_EXPLICIT Well44497a(gint seed);

            /**
             * Creates a new random number generator using an int array seed.
             * @param seed the initial seed (32 bits integers array), if null
             * the seed of the generator will be related to the current time
             */
            CORE_EXPLICIT Well44497a(IntArray const &seed);

            /**
             * Creates a new random number generator using a single long seed.
             * @param seed the initial seed (64 bits integer)
             */
            CORE_EXPLICIT Well44497a(glong seed);

            gint nextInt() const override;

            gbool nextBoolean() const override;

            gfloat nextFloat() const override;

            gdouble nextDouble() const override;

            glong nextLong() const override;

            gdouble nextGaussian() const override;

            void setSeed(gint seed);

            void setSeed(IntArray const &seed);

            void setSeed(glong seed);

            gbool isDeprecated() const override;

            RandomGeneratorProperties properties() const override;

        private:
            gint next(gint bits) const;
        };
    } // random
} // core

#endif //WELL44497A_H
