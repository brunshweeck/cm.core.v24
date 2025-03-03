//
// Created by brunshweeck on 20 août 2024.
//

#ifndef CORE24_XOSHIRO256PLUSPLUS_H
#define CORE24_XOSHIRO256PLUSPLUS_H

#include <core/random/RandomGenerator.h>

namespace core {
    namespace random {
        /**
         * A "jumpable and leapable" pseudorandom number generator (PRNG) whose period
         * is roughly 2<sup>256</sup>.  Class @b Xoshiro256PlusPlus implements
         * interfaces @b RandomGenerator and @b LeapableGenerator,
         * and therefore supports methods for producing pseudorandomly chosen
         * values of type @c int, @c long, @c float, @c double,
         * and @c boolean (and for producing streams of pseudorandomly chosen
         * numbers of type @c int, @c long, and @c double),
         * as well as methods for creating new @b Xoshiro256PlusPlus objects
         * by moving forward either a large distance (2<sup>128</sup>) or a very large
         * distance (2<sup>192</sup>) around the state cycle.
         * <p>
         * Series of generated values pass the TestU01 BigCrush and PractRand test suites
         * that measure independence and uniformity properties of random number generators.
         * (Most recently validated with
         * <a href="http://simul.iro.umontreal.ca/testu01/tu01.html">version 1.2.3 of TestU01</a>
         * and <a href="http://pracrand.sourceforge.net">version 0.90 of PractRand</a>.
         * Note that TestU01 BigCrush was used to test not only values produced by the @c nextLong()
         * method but also the result of bit-reversing each value produced by @c nextLong().)
         * These tests validate only the methods for certain
         * types and ranges, but similar properties are expected to hold, at
         * least approximately, for others as well.
         * </p>
         * <p>
         * The class @b Xoshiro256PlusPlus uses the @c xoshiro256 algorithm,
         * version 1.0 (parameters 17, 45), with the "++" scrambler that computes
         * @c Long.rotateLeft(s0 + s3, 23) + s0.
         * (See David Blackman and Sebastiano Vigna, "Scrambled Linear Pseudorandom
         * Number Generators," ACM Transactions on Mathematical Software, 2021.)
         * Its state consists of four @c long fields @c x0, @c x1, @c x2,
         * and @c x3, which can take on any values provided that they are not all zero.
         * The period of this generator is 2<sup>256</sup>-1.
         * </p>
         * <p>
         * The 64-bit values produced by the @c nextLong() method are equidistributed.
         * To be precise, over the course of the cycle of length 2<sup>256</sup>-1,
         * each nonzero @c long value is generated 2<sup>192</sup> times,
         * but the value 0 is generated only 2<sup>192</sup>-1 times.
         * The values produced by the @c nextInt(), @c nextFloat(), and @c nextDouble()
         * methods are likewise equidistributed.
         * Moreover, the 64-bit values produced by the @c nextLong() method are 3-equidistributed.
         * </p>
         * <p>
         * Instances @b Xoshiro256PlusPlus are <em>not</em> thread-safe.
         * They are designed to be used so that each thread as its own instance.
         * The methods @b jump and @b leap and @b jumps and @b leaps
         * can be used to construct new instances of @b Xoshiro256PlusPlus that traverse
         * other parts of the state cycle.
         * </p>
         */
        class Xoshiro256PlusPlus final : public RandomGenerator {
            /*
             * Implementation Overview.
             *
             * This is an implementation of the xoshiro256++ algorithm version 1.0,
             * written in 2019 by David Blackman and Sebastiano Vigna (vigna@acm.org).
             *
             * The jump operation moves the current generator forward by 2*128
             * steps; this has the same effect as calling nextLong() 2**128
             * times, but is much faster.  Similarly, the leap operation moves
             * the current generator forward by 2*192 steps; this has the same
             * effect as calling nextLong() 2**192 times, but is much faster.
             * The copy method may be used to make a copy of the current
             * generator.  Thus one may repeatedly and cumulatively copy and
             * jump to produce a sequence of generators whose states are well
             * spaced apart along the overall state cycle (indeed, the jumps()
             * and leaps() methods each produce a stream of such generators).
             * The generators can then be parceled out to other threads.
             *
             * File organization: First static fields, then instance
             * fields, then constructors, then instance methods.
             */


            /* ---------------- static fields ---------------- */

            /**
             * The seed generator for default constructors.
             */
            static atomic::AtomicLong DEFAULT_GENERATOR;

            /* ---------------- instance fields ---------------- */

            /**
             * The per-instance state: s for the LCG; the array x for the XBG;
             * At least one of the 4 elements of the array x must be nonzero.
             */
            glong mutable x[4] = {};

        public:
            /* ---------------- constructors ---------------- */

            /**
             * Basic constructor that initializes all fields from parameters.
             * It then adjusts the field values if necessary to ensure that
             * all constraints on the values of fields are met.
             *
             * @param x0 first word of the initial state
             * @param x1 second word of the initial state
             * @param x2 third word of the initial state
             * @param x3 fourth word of the initial state
             */
            CORE_EXPLICIT Xoshiro256PlusPlus(glong x0, glong x1, glong x2, glong x3);

            /**
             * Creates a new instance of @b Xoshiro256PlusPlus using the
             * specified @c long value as the initial seed. Instances of
             * @b Xoshiro256PlusPlus created with the same seed in the same
             * program generate identical sequences of values.
             *
             * @param seed the initial seed
             */
            CORE_EXPLICIT Xoshiro256PlusPlus(glong seed);

            /**
             * Creates a new instance of @b Xoshiro256PlusPlus that is likely to
             * generate sequences of values that are statistically independent
             * of those of any other instances in the current program execution,
             * but may, and typically does, vary across program invocations.
             */
            CORE_IMPLICIT Xoshiro256PlusPlus();

            /**
             * Creates a new instance of @b Xoshiro256PlusPlus using the specified array of
             * initial seed bytes. Instances of @b Xoshiro256PlusPlus created with the same
             * seed array in the same program execution generate identical sequences of values.
             *
             * @param seed the initial seed
             */
            CORE_EXPLICIT Xoshiro256PlusPlus(ByteArray const &seed);

            /*
             * The following two comments are quoted from http://prng.di.unimi.it/xoshiro256plusplus.c
             */

            /*
             * To the extent possible under law, the author has dedicated all copyright
             * and related and neighboring rights to this software to the public domain
             * worldwide. This software is distributed without any warranty.
             * <p>
             * See http://creativecommons.org/publicdomain/zero/1.0/.
             */

            /*
             * This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
             * It has excellent (sub-ns) speed, a state (256 bits) that is large
             * enough for any parallel application, and it passes all tests we are
             * aware of.
             *
             * For generating just floating-point numbers, xoshiro256+ is even faster.
             *
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

#endif //CORE24_XOSHIRO256PLUSPLUS_H
