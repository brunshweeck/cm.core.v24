//
// Created by brunshweeck on 20 août 2024.
//

#ifndef CORE24_XOSHIRO128PLUSPLUS_H
#define CORE24_XOSHIRO128PLUSPLUS_H

#include <core/random/RandomGenerator.h>

namespace core {
    namespace random {
        /**
         * A "jumpable and leapable" pseudorandom number generator (PRNG) whose period
         * is roughly 2<sup>128</sup>.  Class @b Xoroshiro128PlusPlus implements
         * interfaces @b RandomGenerator and @b LeapableGenerator,
         * and therefore supports methods for producing pseudorandomly chosen
         * numbers of type @c int, @c long, @c float, and @c double
         * as well as creating new @b Xoroshiro128PlusPlus objects
         * by "jumping" or "leaping".
         * <p>
         * The class @b Xoroshiro128PlusPlus uses the @c xoroshiro128 algorithm
         * (parameters 49, 21, 28) with the "++" scrambler that computes
         * @c Long.rotateLeft(s0 + s1, 17) + s0.
         * (See David Blackman and Sebastiano Vigna, "Scrambled Linear Pseudorandom
         * Number Generators," ACM Transactions on Mathematical Software, 2021.)
         * Its state consists of two @c long fields @c x0 and @c x1,
         * which can take on any values provided that they are not both zero.
         * The period of this generator is 2<sup>128</sup>-1.
         * </p>
         * <p>
         * The 64-bit values produced by the @c nextLong() method are equidistributed.
         * To be precise, over the course of the cycle of length 2<sup>128</sup>-1,
         * each nonzero @c long value is generated 2<sup>64</sup> times,
         * but the value 0 is generated only 2<sup>64</sup>-1 times.
         * The values produced by the @c nextInt(), @c nextFloat(), and @c nextDouble()
         * methods are likewise equidistributed.
         * </p>
         * <p>
         * Instances @b Xoroshiro128PlusPlus are <em>not</em> thread-safe.
         * They are designed to be used so that each thread as its own instance.
         * The methods @b jump and @b leap and @b jumps and @b leaps
         * can be used to construct new instances of @b Xoroshiro128PlusPlus that traverse
         * other parts of the state cycle.
         * </p>
         */
        class Xoroshiro128PlusPlus final : public RandomGenerator {
            /*
             * Implementation Overview.
             *
             * This is an implementation of the xoroshiro128++ algorithm version 1.0,
             * written in 2019 by David Blackman and Sebastiano Vigna (vigna@acm.org).
             *
             * The jump operation moves the current generator forward by 2*64
             * steps; this has the same effect as calling nextLong() 2**64
             * times, but is much faster.  Similarly, the leap operation moves
             * the current generator forward by 2*96 steps; this has the same
             * effect as calling nextLong() 2**96 times, but is much faster.
             * The copy method may be used to make a copy of the current
             * generator.  Thus one may repeatedly and cumulatively copy and
             * jump to produce a sequence of generators whose states are well
             * spaced apart along the overall state cycle (indeed, the jumps()
             * and leaps() methods each produce a stream of such generators).
             * The generators can then be parceled out to other threads.
             *
             * File organization: First the non-public methods that constitute the
             * main algorithm, then the public methods.  Note that many methods are
             * defined by classes @b AbstractJumpableGenerator and @b AbstractGenerator.
             */


            /* ---------------- static fields ---------------- */

            /**
             * The seed generator for default constructors.
             */
            static atomic::AtomicLong DEFAULT_GENERATOR;

            /* ---------------- instance fields ---------------- */

            /**
             * The per-instance state.
             * At least one of the 2 elements of the array x must be nonzero.
             */
            glong mutable x[2] = {};

        public:
            /* ---------------- constructors ---------------- */

            /**
             * Basic constructor that initializes all fields from parameters.
             * It then adjusts the field values if necessary to ensure that
             * all constraints on the values of fields are met.
             *
             * @param x0 first word of the initial state
             * @param x1 second word of the initial state
             */
            CORE_EXPLICIT Xoroshiro128PlusPlus(glong x0, glong x1);

            /**
             * Creates a new instance of @b Xoroshiro128PlusPlus using the
             * specified @c long value as the initial seed. Instances of
             * @b Xoroshiro128PlusPlus created with the same seed in the same
             * program generate identical sequences of values.
             *
             * @param seed the initial seed
             */
            CORE_EXPLICIT Xoroshiro128PlusPlus(glong seed);

            /**
             * Creates a new instance of @b Xoroshiro128PlusPlus that is likely to
             * generate sequences of values that are statistically independent
             * of those of any other instances in the current program execution,
             * but may, and typically does, vary across program invocations.
             */
            CORE_IMPLICIT Xoroshiro128PlusPlus();

            /**
             * Creates a new instance of @b Xoroshiro128PlusPlus using the specified array of
             * initial seed bytes. Instances of @b Xoroshiro128PlusPlus created with the same
             * seed array in the same program execution generate identical sequences of values.
             *
             * @param seed the initial seed
             */
            CORE_EXPLICIT Xoroshiro128PlusPlus(ByteArray const &seed);

            /*
             * The following two comments are quoted from http://prng.di.unimi.it/xoroshiro128plusplus.c
             */

            /*
             * To the extent possible under law, the author has dedicated all copyright
             * and related and neighboring rights to this software to the public domain
             * worldwide. This software is distributed without any warranty.
             * <p>
             * See http://creativecommons.org/publicdomain/zero/1.0/.
             */

            /*
             * This is xoroshiro128++ 1.0, one of our all-purpose, rock-solid,
             * small-state generators. It is extremely (sub-ns) fast and it passes all
             * tests we are aware of, but its state space is large enough only for
             * mild parallelism.
             * <p>
             * For generating just floating-point numbers, xoroshiro128+ is even
             * faster (but it has a very mild bias, see notes in the comments).
             * <p>
             * The state must be seeded so that it is not everywhere zero. If you have
             * a 64-bit seed, we suggest to seed a splitmix64 generator and use its
             * output to fill s.
             */

            glong nextLong() const override;

            gbool isDeprecated() const override;

            RandomGeneratorProperties properties() const override;

            void setSeed(glong seed) override;
        };
    } // random
} // core

#endif //CORE24_XOSHIRO128PLUSPLUS_H
