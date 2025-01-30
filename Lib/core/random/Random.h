//
// Created by brunshweeck on 18 août 2024.
//

#ifndef CORE24_RANDOM_H
#define CORE24_RANDOM_H

#include <core/random/RandomGenerator.h>
#include <core/atomic/AtomicLong.h>

namespace core {
    namespace random {
        /**
         * An instance of this class is used to generate a stream of
         * pseudorandom numbers; its period is only 2<sup>48</sup>.
         * The class uses a 48-bit seed, which is
         * modified using a linear congruential formula. (See Donald E. Knuth,
         * <cite>The Art of Computer Programming, Volume 2, Third
         * edition: Semi-numerical Algorithms</cite>, Section 3.2.1.)
         * <p>
         * If two instances of @c Random are created with the same
         * seed, and the same sequence of method calls is made for each, they
         * will generate and return identical sequences of numbers. In order to
         * guarantee this property, particular algorithms are specified for the
         * class @c Random. Java implementations must use all the algorithms
         * shown here for the class @c Random, for the sake of absolute
         * portability of Java code. However, subclasses of class @c Random
         * are permitted to use other algorithms, so long as they adhere to the
         * general contracts for all the methods.
         * </p>
         * <p>
         * The algorithms implemented by class @c Random use a
         * @c protected utility method that on each invocation can supply
         * up to 32 pseudorandomly generated bits.
         * </p>
         * <p>
         * Many applications will find the method @b Math::random simpler to use.
         *
         * </p>
         * <p>
         * Instances of @c random::Random are thread-safe.
         * However, the concurrent use of the same @c random::Random
         * instance across threads may encounter contention and consequent
         * poor performance. Consider instead using
         * @b concurrent::ThreadLocalRandom in multithreaded
         * designs.
         *
         * </p>
         * <p>
         * Instances of @c random::Random are not cryptographically
         * secure.  Consider instead using @b random::SecureRandom to
         * get a cryptographically secure pseudo-random number generator for use
         * by security-sensitive applications.
         * </p>
         */
        class Random : public virtual RandomGenerator {
            /**
             * The internal state associated with this pseudorandom number generator.
             * (The specs for the methods in this class describe the ongoing
             * computation of this value.)
             */
            atomic::AtomicLong mutable seed;

            static CORE_FAST glong multiplier = 0x5DEECE66DLL;
            static CORE_FAST glong addend = 0xBLL;
            static CORE_FAST glong mask = (1LL << 48) - 1;

            static CORE_FAST gdouble DOUBLE_UNIT = 0x1.0P-53; // 1.0 / (1L << Double::PRECISION)
            static CORE_FAST gfloat FLOAT_UNIT = 0x1.0P-24F; // 1.0f / (1 << Float::PRECISION)

        public:
            /**
             * Creates a new random number generator. This constructor sets
             * the seed of the random number generator to a value very likely
             * to be distinct from any other invocation of this constructor.
             */
            CORE_IMPLICIT Random();

            /**
             * Creates a new random number generator using a single @c long seed.
             * The seed is the initial value of the internal state of the pseudorandom
             * number generator which is maintained by method @b next.
             *
             * @note The invocation @c Random(seed) is equivalent to:
             * @code
             *   auto rnd = Random();
             *   rnd.setSeed(seed);
             * @endcode
             *
             * @param seed the initial seed
             * @see   setSeed(long)
             */
            CORE_EXPLICIT Random(glong seed);

            /**
             * Returns an instance of @c Random that delegates method calls to the @b RandomGenerator
             * argument. If the generator is an instance of @c Random, it is returned. Otherwise, this method
             * returns an instance of @c Random that delegates all methods except @c setSeed to the generator.
             * The returned instance's @c setSeed method always throws @b UnsupportedOperationException.
             * The returned instance is not serializable.
             *
             * @param generator the @c RandomGenerator calls are delegated to
             * @return the delegating @c Random instance
             */
            static Random &from(RandomGenerator &generator);

            /**
             * Sets or updates the seed of this random number generator using the
             * provided @c long seed value (optional operation).
             *
             * @note
             * The implementation in this class alters the state of this random number
             * generator so that it is in the same state as if it had just been created with
             * @b Random(seed). It atomically updates the seed to
             * @code (seed ^ 0x5DEECE66DL) & ((1L << 48) - 1) @endcode
             * and clears the @c haveNextNextGaussian flag used by @b nextGaussian.
             * Note that this uses only 48 bits of the given seed value.
             *
             * @param seed the seed value
             * @throws UnsupportedOperationException if the @c setSeed
             *         operation is not supported by this random number generator
             */
            void setSeed(glong seed) override;

            /**
             * Generates random bytes and places them into a user-supplied
             * byte array.  The number of random bytes produced is equal to
             * the length of the byte array.
             *
             * @note The method @c nextBytes is
             * implemented by class @c Random as if by:
             * @code
             *  void nextBytes(ByteArray& bytes) {
             *   for (gint i = 0; i < bytes.length(); )
             *     for (gint rnd = nextInt(), n = Math.min(bytes.length - i, 4);
             *          n-- > 0; rnd >>= 8)
             *       bytes[i++] = (gbyte)rnd;
             *  }
             * @endcode
             *
             * @param  bytes the byte array to fill with random bytes
             */
            void nextBytes(ByteArray &bytes) const override;

            /**
             * Returns the next pseudorandom, uniformly distributed @c int
             * value from this random number generator's sequence. The general
             * contract of @c nextInt is that one @c int value is
             * pseudorandomly generated and returned. All 2<sup>32</sup> possible
             * @c int values are produced with (approximately) equal probability.
             *
             * @note The method @c nextInt is
             * implemented by class @c Random as if by:
             * @code
             *   int nextInt() {
             *     return next(32);
             *   }
             * @endcode
             *
             * @return the next pseudorandom, uniformly distributed @c int
             *         value from this random number generator's sequence
             */
            gint nextInt() const override;

            /**
             * Returns a pseudorandom, uniformly distributed @c int value
             * between 0 (inclusive) and the specified value (exclusive), drawn from
             * this random number generator's sequence.  The general contract of
             * @c nextInt is that one @c int value in the specified range
             * is pseudorandomly generated and returned.  All @c bound possible
             * @c int values are produced with (approximately) equal
             * probability.
             *
             * @note The method @c nextInt(int) is implemented by
             * class @c Random as if by:
             * @code
             *  gint nextInt(gint bound) {
             *   if (bound <= 0)
             *     IllegalArgumentException("bound must be positive").throws($ftrace());
             *
             *   if ((bound & -bound) == bound)  // i.e., bound is a power of 2
             *     return (gint)((bound * (long)next(31)) >> 31);
             *
             *   gint bits, val;
             *   do {
             *       bits = next(31);
             *       val = bits % bound;
             *   } while (bits - val + (bound-1) < 0);
             *   return val;
             *  }
             * @endcode
             *
             * <p>The hedge "approximately" is used in the foregoing description only
             * because the next method is only approximately an unbiased source of
             * independently chosen bits.  If it were a perfect source of randomly
             * chosen bits, then the algorithm shown would choose @c int
             * values from the stated range with perfect uniformity.
             * </p>
             * <p>
             * The algorithm is slightly tricky.  It rejects values that would result
             * in an uneven distribution (due to the fact that 2^31 is not divisible
             * by n). The probability of a value being rejected depends on n.  The
             * worst case is n=2^30+1, for which the probability of a reject is 1/2,
             * and the expected number of iterations before the loop terminates is 2.
             * </p>
             * <p>
             * The algorithm treats the case where n is a power of two specially: it
             * returns the correct number of high-order bits from the underlying
             * pseudo-random number generator.  In the absence of special treatment,
             * the correct number of <i>low-order</i> bits would be returned.  Linear
             * congruential pseudo-random number generators such as the one
             * implemented by this class are known to have short periods in the
             * sequence of values of their low-order bits.  Thus, this special case
             * greatly increases the length of the sequence of values returned by
             * successive calls to this method if n is a small power of two.
             * </p>
             * @param bound the upper bound (exclusive).  Must be positive.
             * @return the next pseudorandom, uniformly distributed @c int
             *         value between zero (inclusive) and @c bound (exclusive)
             *         from this random number generator's sequence
             * @throws IllegalArgumentException if bound is not positive
             */
            gint nextInt(gint bound) const override;

            /**
             * Returns the next pseudorandom, uniformly distributed @c long
             * value from this random number generator's sequence. The general
             * contract of @c nextLong is that one @c long value is
             * pseudorandomly generated and returned.
             *
             * @note The method @c nextLong is implemented by class @c Random
             * as if by:
             * @code
             *   glong nextLong() {
             *     return ((long)next(32) << 32) + next(32);
             *   }
             * @endcode
             *
             * Because class @c Random uses a seed with only 48 bits,
             * this algorithm will not return all possible @c long values.
             *
             * @return the next pseudorandom, uniformly distributed @c long
             *         value from this random number generator's sequence
             */
            glong nextLong() const override;

            /**
             * Returns the next pseudorandom, uniformly distributed
             * @c boolean value from this random number generator's
             * sequence. The general contract of @c nextBoolean is that one
             * @c boolean value is pseudorandomly generated and returned.  The
             * values @c true and @c false are produced with
             * (approximately) equal probability.
             *
             * @note The method @c nextBoolean is implemented by class
             * @c Random as if by:
             * @code
             *    gbool nextBoolean() {
             *      return next(1) != 0;
             *    }
             * @endcode
             *
             * @return the next pseudorandom, uniformly distributed
             *         @c boolean value from this random number generator's
             *         sequence
             */
            gbool nextBoolean() const override;

            /**
             * Returns the next pseudorandom, uniformly distributed @c float
             * value between @c 0.0 and @c 1.0 from this random
             * number generator's sequence.
             *
             * <p>
             * The general contract of @c nextFloat is that one
             * @c float value, chosen (approximately) uniformly from the
             * range @c 0.0f (inclusive) to @c 1.0f (exclusive), is
             * pseudorandomly generated and returned. All 2<sup>24</sup> possible
             * @c float values of the form <i>m&nbsp;x&nbsp;</i>2<sup>-24</sup>,
             * where <i>m</i> is a positive integer less than 2<sup>24</sup>, are
             * produced with (approximately) equal probability.
             * </p>
             * @note The method @c nextFloat is implemented by class
             * @c Random as if by:
             * @code
             *   gfloat nextFloat() {
             *     return next(24) / ((gfloat)(1 << 24));
             *   }
             * @endcode
             * <p>
             * The hedge "approximately" is used in the foregoing description only
             * because the next method is only approximately an unbiased source of
             * independently chosen bits. If it were a perfect source of randomly
             * chosen bits, then the algorithm shown would choose @c float
             * values from the stated range with perfect uniformity.
             * </p>
             * <p>
             * [In early versions of Java, the result was incorrectly calculated as:
             *  @code return next(30) / ((gfloat)(1 << 30)); @endcode
             * This might seem to be equivalent, if not better, but in fact it
             * introduced a slight non-uniformity because of the bias in the rounding
             * of floating-point numbers: it was slightly more likely that the
             * low-order bit of the significand would be 0 than that it would be 1.]
             * </p>
             * @return the next pseudorandom, uniformly distributed @c float
             *         value between @c 0.0f and @c 1.0f from this
             *         random number generator's sequence
             */
            gfloat nextFloat() const override;

            /**
             * Returns the next pseudorandom, uniformly distributed
             * @c double value between @c 0.0 and
             * @c 1.0 from this random number generator's sequence.
             *
             * <p>
             * The general contract of @c nextDouble is that one
             * @c double value, chosen (approximately) uniformly from the
             * range @c 0.0 (inclusive) to @c 1.0 (exclusive), is
             * pseudorandomly generated and returned.
             * </p>
             * @note The method @c nextDouble is implemented by class
             * @c Random as if by:
             * @code
             *  gdouble nextDouble() {
             *    return (((glong)next(26) << 27) + next(27)) / (gdouble)(1L << 53);
             *  }
             * @endcode
             * <p>
             * The hedge "approximately" is used in the foregoing description only
             * because the @c next method is only approximately an unbiased source
             * of independently chosen bits. If it were a perfect source of randomly
             * chosen bits, then the algorithm shown would choose @c double values
             * from the stated range with perfect uniformity.
             * </p>
             * <p>
             * [In early versions of Java, the result was incorrectly calculated as:
             * @code return (((glong)next(27) << 27) + next(27)) / (gdouble)(1L << 54); @endcode
             * This might seem to be equivalent, if not better, but in fact it
             * introduced a large non-uniformity because of the bias in the rounding of
             * floating-point numbers: it was three times as likely that the low-order
             * bit of the significand would be 0 than that it would be 1! This
             * non-uniformity probably doesn't matter much in practice, but we strive
             * for perfection.]
             * </p>
             * @return the next pseudorandom, uniformly distributed @c double
             *         value between @c 0.0 and @c 1.0 from this
             *         random number generator's sequence
             * @see Math#random
             */
            gdouble nextDouble() const override;

            /**
             * Returns the next pseudorandom, Gaussian ("normally") distributed
             * @c double value with mean @c 0.0 and standard
             * deviation @c 1.0 from this random number generator's sequence.
             * <p>
             * The general contract of @c nextGaussian is that one
             * @c double value, chosen from (approximately) the usual
             * normal distribution with mean @c 0.0 and standard deviation
             * @c 1.0, is pseudorandomly generated and returned.
             * </p>
             * @note The method @c nextGaussian is implemented by class
             * @c Random as if by a thread-safe version of the following:
             * @code
             * private:
             *   double nextNextGaussian;
             *   boolean haveNextNextGaussian = false;
             *
             * public:
             *  gdouble nextGaussian() {
             *   if (haveNextNextGaussian) {
             *     haveNextNextGaussian = false;
             *     return nextNextGaussian;
             *   } else {
             *     gdouble v1, v2, s;
             *     do {
             *       v1 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
             *       v2 = 2 * nextDouble() - 1;   // between -1.0 and 1.0
             *       s = v1 * v1 + v2 * v2;
             *     } while (s >= 1 || s == 0);
             *     gdouble multiplier = Math::sqrt(-2 * Math::log(s)/s);
             *     nextNextGaussian = v2 * multiplier;
             *     haveNextNextGaussian = true;
             *     return v1 * multiplier;
             *   }
             *  }
             * @endcode
             * <p>
             * This uses the <i>polar method</i> of G. E. P. Box, M. E. Muller, and
             * G. Marsaglia, as described by Donald E. Knuth in <cite>The Art of
             * Computer Programming, Volume 2, third edition: Semi-numerical Algorithms</cite>,
             * section 3.4.1, subsection C, algorithm P. Note that it generates two
             * independent values at the cost of only one call to @c Math::log
             * and one call to @c Math::sqrt.
             * </p>
             * @return the next pseudorandom, Gaussian ("normally") distributed
             *         @c double value with mean @c 0.0 and
             *         standard deviation @c 1.0 from this random number
             *         generator's sequence
             */
            gdouble nextGaussian() const override;

            gbool isDeprecated() const override;

            String toString() const override;

            RandomGeneratorProperties properties() const override;

        protected:
            /**
             * Generates the next pseudorandom number. This method returns an
             * @c int value such that, if the argument @c bits is between
             * @c 1 and @c 32 (inclusive), then that many low-order
             * bits of the returned value will be (approximately) independently
             * chosen bit values, each of which is (approximately) equally
             * likely to be @c 0 or @c 1.
             *
             * @note
             * The other random-producing methods in this class are implemented
             * in terms of this method, so subclasses can override just this
             * method to provide a different source of pseudorandom numbers for
             * the entire class.
             *
             * @note
             * The implementation in this class atomically updates the seed to
             *  @code (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1) @endcode
             * and returns
             *  @code (gint)(seed >>> (48 - bits)) @endcode .
             *
             * <p>
             * This is a linear congruential pseudorandom number generator, as
             * defined by D. H. Lehmer and described by Donald E. Knuth in
             * <cite>The Art of Computer Programming, Volume 2, Third edition:
             * Semi-numerical Algorithms</cite>, section 3.2.1.
             * </p>
             * @param  bits random bits
             * @return the next pseudorandom value from this random number
             *         generator's sequence
             * @since  1.1
             */
            gint next(gint bits) const;

            CORE_EXPLICIT Random(const misc::NIL &unused);

        private:
            static glong generateSeed();

            static glong initialScramble(glong seed);

            static atomic::AtomicLong SEED_UNIQUIFIER;

            gdouble mutable nextNextGaussian = 0.0;
            gbool mutable haveNextNextGaussian = false;
        };
    } // random
} // core

#endif //CORE24_RANDOM_H
