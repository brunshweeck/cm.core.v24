//
// Created by brunshweeck on 22 août 2024.
//

#ifndef CORE24_MERSENNETWISTER_H
#define CORE24_MERSENNETWISTER_H

#include <core/random/RandomGenerator.h>

namespace core {
    namespace random {
        /**
         * This class implements a powerful pseudo-random number generator
         * developed by Makoto Matsumoto and Takuji Nishimura during
         * 1996-1997.
         * <p>
         * This generator features an extremely long period
         * (2<sup>19937</sup>-1) and 623-dimensional equidistribution up to 32
         * bits accuracy. The home page for this generator is located at
         * <a href="http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html">
         * http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html</a>.
         * </p>
         * <p>
         * This generator is described in a paper by Makoto Matsumoto and
         * Takuji Nishimura in 1998:
         * <a href="http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/ARTICLES/mt.pdf">
         * Mersenne Twister: A 623-Dimensionally Equidistributed Uniform Pseudo-Random
         * Number Generator</a>, ACM Transactions on Modeling and Computer
         * Simulation, Vol. 8, No. 1, January 1998, pp 3--30
         * </p>
         * <p>This class is mainly a Java port of the 2002-01-26 version of
         * the generator written in C by Makoto Matsumoto and Takuji Nishimura.
         * Here is their original copyright:
         * </p>
         * @note  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
         * All rights reserved.
         * <p>
         * Redistribution and use in source and binary forms, with or without
         * modification, are permitted provided that the following conditions
         * are met:
         * @li Redistributions of source code must retain the above copyright
         *       notice, this list of conditions and the following disclaimer.
         * @li Redistributions in binary form must reproduce the above copyright
         *       notice, this list of conditions and the following disclaimer in the
         *       documentation and/or other materials provided with the distribution.
         * @li The names of its contributors may not be used to endorse or promote
         *       products derived from this software without specific prior written
         *       permission.
         * </p>
         * <p>
         * <b>
         * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
         * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
         * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
         * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
         * DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
         * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
         * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
         * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
         * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
         * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
         * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
         * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
         * DAMAGE.
         * </b>
         * </p>
         */
        class MersenneTwister final : public RandomGenerator {
            /* ---------------- static fields ---------------- */

            /**
             * The seed generator for default constructors.
             */
            static atomic::AtomicLong DEFAULT_GENERATOR;

            /** Size of the bytes pool. */
            static CORE_FAST gint N = 624;

            /** Period second parameter. */
            static CORE_FAST gint M = 397;

            /** X * MATRIX_A for X = {0, 1}. */
            static CORE_FAST gint MAG0 = 0x0;
            static CORE_FAST gint MAG1 = CORE_CAST(gint, 0x9908B0DF);

            /** Bytes pool. */
            gint mutable mt[N] = {};

            /** Current index in the bytes pool. */
            gint mutable mti = 0;

            gdouble mutable nxtGaussian = 0.0;

        public:
            /**
             * Creates a new instance of @b MersenneTwister that is likely to
             * generate sequences of values that are statistically independent
             * of those of any other instances in the current program execution,
             * but may, and typically does, vary across program invocations.
             */
            CORE_IMPLICIT MersenneTwister();

            /**
             * Creates a new instance of @b MersenneTwister using the
             * specified @c long value as the initial seed. Instances of
             * @b MersenneTwister created with the same seed in the same
             * program generate identical sequences of values.
             *
             * @param seed the initial seed
             */
            CORE_EXPLICIT MersenneTwister(gint seed);

            /**
             * Creates a new instance of @b MersenneTwister using the
             * specified @c long value as the initial seed. Instances of
             * @b MersenneTwister created with the same seed in the same
             * program generate identical sequences of values.
             *
             * @param seed the initial seed
             */
            CORE_EXPLICIT MersenneTwister(glong seed);

            /**
             * Creates a new random number generator using an int array seed.
             * @param seed the initial seed (32 bits integers array), if empty
             * the seed of the generator will be the default generated seed.
             */
            CORE_EXPLICIT MersenneTwister(IntArray const &seed);

            /**
             * Reinitialize the generator as if just built with the given int seed.
             * <p>The state of the generator is exactly the same as a new
             * generator built with the same seed.
             * </p>
             * @param seed the initial seed (32 bits integer)
             */
            void setSeed(gint seed);

            /**
             * Reinitialize the generator as if just built with the given int array seed.
             * <p>
             * The state of the generator is exactly the same as a new
             * generator built with the same seed.
             * </p>
             * @param seed the initial seed (32 bits integers array), if empty
             * the seed of the generator will be the default generated seed.
             */
            void setSeed(IntArray const &seed);

            /** Reinitialize the generator as if just built with the given long seed.
             * <p>
             * The state of the generator is exactly the same as a new
             * generator built with the same seed.
             * </p>
             * @param seed the initial seed (64 bits integer)
             */
            void setSeed(glong seed);

            gbool nextBoolean() const override;

            gint nextInt() const override;

            gfloat nextFloat() const override;

            glong nextLong() const override;

            gdouble nextDouble() const override;

            gdouble nextGaussian() const override;

            gbool isDeprecated() const override;

            RandomGeneratorProperties properties() const override;

        private:
            /**
             * Generate next pseudorandom number.
             * <p>
             * This method is the core generation algorithm. It is used by all the
             * public generation methods for the various primitive types
             * @b nextBoolean(), @b nextBytes(byte[]), @b nextDouble(),
             * @b nextFloat(), @b nextGaussian(), @b nextInt(),
             * @b next(int) and @b nextLong().
             * </p>
             * @param bits number of random bits to produce
             * @return random bits generated
             */
            gint next(gint bits) const;
        };
    } // random
} // core

#endif //CORE24_MERSENNETWISTER_H
