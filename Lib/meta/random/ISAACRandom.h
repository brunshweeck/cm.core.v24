//
// Created by brunshweeck on 24 août 2024.
//

#ifndef CORE24_ISAACRANDOM_H
#define CORE24_ISAACRANDOM_H

#include <core/random/RandomGenerator.h>
#include <meta/random/RandomSupport.h>

namespace core {
    namespace random {
        /**
         * <a href="http://burtleburtle.net/bob/rand/isaacafa.html">
         *  ISAAC: a fast cryptographic pseudo-random number generator</a>
         * <br/>
         * ISAAC (Indirection, Shift, Accumulate, Add, and Count) generates 32-bit
         * random numbers.
         * ISAAC has been designed to be cryptographically secure and is inspired
         * by RC4.
         * Cycles are guaranteed to be at least 2<sup>40</sup> values long, and they
         * are 2<sup>8295</sup> values long on average.
         * The results are uniformly distributed, unbiased, and unpredictable unless
         * you know the seed.
         * <br/>
         * This code is based (with minor changes and improvements) on the original
         * implementation of the algorithm by Bob Jenkins.
         * <br/>
         */
        class ISAACRandom final : public RandomGenerator {
            /**
             * The seed generator for default constructors.
             */
            static atomic::AtomicLong DEFAULT_GENERATOR;

            /** Log of size of rsl[] and mem[] */
            static CORE_FAST gint SIZE_LOG = 8;
            /** Size of rsl[] and mem[] */
            static CORE_FAST gint SIZE = 1 << SIZE_LOG;
            /** Half-size of rsl[] and mem[] */
            static CORE_FAST gint HALF_SIZE = SIZE >> 1;
            /** For pseudo-random lookup */
            static CORE_FAST gint MASK = (SIZE - 1) << 2;
            /** The golden ratio */
            static CORE_FAST gint GOLDEN_RATIO = RandomSupport::GOLDEN_RATIO_32;
            /** The results given to the user */
            gint mutable rsl[SIZE] = {};
            /** The internal state */
            gint mutable mem[SIZE] = {};
            /** Count through the results in rsl[] */
            gint mutable count = 0;
            /** Accumulator */
            gint mutable isaacA = 0;
            /** The last result */
            gint mutable isaacB = 0;
            /** Counter, guarantees cycle is at least 2^40 */
            gint mutable isaacC = 0;
            /** Service variable. */
            gint mutable arr[8] = {};
            /** Service variable. */
            gint mutable isaacX = 0;
            /** Service variable. */
            gint mutable isaacI = 0;
            /** Service variable. */
            gint mutable isaacJ = 0;

            gdouble mutable nxtGaussian = Double::NaN;

        public:
            /**
             * Creates a new ISAAC random number generator.
             */
            CORE_IMPLICIT ISAACRandom();

            /**
             * Creates a new ISAAC random number generator using a single long seed.
             *
             * @param seed Initial seed.
             */
            CORE_IMPLICIT ISAACRandom(glong seed);

            /**
             * Creates a new ISAAC random number generator using an int array seed.
             *
             * @param seed Initial seed. If empty, the seed will be related
             * to the current time.
             */
            CORE_IMPLICIT ISAACRandom(IntArray const &seed);

            /**
             * Sets the seed of the underlying random number generator using an
             * @c int array seed.
             * <p>Sequences of values generated starting with the same seeds
             * should be identical.
             * </p>
             * @param seed the seed value
             */
            void setSeed(IntArray const &seed);

            /**
             * Sets the seed of the underlying random number generator using a
             * @c long seed.
             * <p>Sequences of values generated starting with the same seeds
             * should be identical.
             * </p>
             * @param seed the seed value
             */
            void setSeed(glong seed);

            /**
             * Sets the seed of the underlying random number generator using an
             * @c int seed.
             * <p>Sequences of values generated starting with the same seeds
             * should be identical.
             * </p>
             * @param seed the seed value
             */
            void setSeed(gint seed);

            gbool isDeprecated() const override;

            /**
             * Returns the next pseudorandom, uniformly distributed @c int
             * value from this random number generator's sequence.
             * All 2<font size="-1"><sup>32</sup></font> possible @c int values
             * should be produced with  (approximately) equal probability.
             *
             * @return the next pseudorandom, uniformly distributed @c int
             *  value from this random number generator's sequence
             */
            gint nextInt() const override;

            /**
             * Returns the next pseudorandom, uniformly distributed @c long
             * value from this random number generator's sequence.  All
             * 2<font size="-1"><sup>64</sup></font> possible @c long values
             * should be produced with (approximately) equal probability.
             *
             * @return  the next pseudorandom, uniformly distributed @c long
             *value from this random number generator's sequence
             */
            glong nextLong() const override;

            /**
             * Returns the next pseudorandom, uniformly distributed
             * @c boolean value from this random number generator's
             * sequence.
             *
             * @return  the next pseudorandom, uniformly distributed
             * @c boolean value from this random number generator's
             * sequence
             */
            gbool nextBoolean() const override;

            /**
             * Returns the next pseudorandom, uniformly distributed @c float
             * value between @c 0.0 and @c 1.0 from this random
             * number generator's sequence.
             *
             * @return  the next pseudorandom, uniformly distributed @c float
             * value between @c 0.0 and @c 1.0 from this
             * random number generator's sequence
             */
            gfloat nextFloat() const override;

            /**
             * Returns the next pseudorandom, uniformly distributed
             * @c double value between @c 0.0 and
             * @c 1.0 from this random number generator's sequence.
             *
             * @return  the next pseudorandom, uniformly distributed
             *  @c double value between @c 0.0 and
             *  @c 1.0 from this random number generator's sequence
             */
            gdouble nextDouble() const override;

            /**
             * Returns the next pseudorandom, Gaussian ("normally") distributed
             * @c double value with mean @c 0.0 and standard
             * deviation @c 1.0 from this random number generator's sequence.
             *
             * @return  the next pseudorandom, Gaussian ("normally") distributed
             * @c double value with mean @c 0.0 and
             * standard deviation @c 1.0 from this random number
             *  generator's sequence
             */
            gdouble nextGaussian() const override;

            RandomGeneratorProperties properties() const override;

        private:
            /** Generate next pseudorandom number.
             * <p>This method is the core generation algorithm. It is used by all the
             * public generation methods for the various primitive types
             * @b nextBoolean(), @b nextBytes(ByteArray), @b nextDouble(),
             * @b nextFloat(), @b nextGaussian(), @b nextInt(),
             * @b next(int) and @b nextLong().</p>
             * @param bits number of random bits to produce
             * @return random bits generated
             */
            gint next(gint bits) const;

            /** Generate 256 results */
            void isaac() const;

            /** Intermediate internal loop. */
            void isaac2() const;

            /** Lowest level internal loop. */
            void isaac3() const;

            /** Initialize, or reinitialize, this instance of rand. */
            void initState() const;

            /** Shuffle array. */
            void shuffle() const;

            /** Set the state by copying the internal arrays.
             *
             * @param start First index into @b mem array.
             */
            void setState(gint start) const;
        };
    } // random
} // core

#endif //CORE24_ISAACRANDOM_H
