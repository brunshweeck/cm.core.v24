//
// Created by brunshweeck on 18 août 2024.
//

#ifndef CORE24_RANDOMGENERATOR_H
#define CORE24_RANDOMGENERATOR_H


#include <core/random/RandomGeneratorProperties.h>

namespace core {
    namespace random {
        /**
         * The @b RandomGenerator interface is designed to provide a common
         * protocol for objects that generate random or (more typically) pseudorandom
         * sequences of numbers (or Boolean values). Such a sequence may be obtained by
         * either repeatedly invoking a method that returns a single pseudorandomly
         * chosen value, or by invoking a method that returns a stream of
         * pseudorandomly chosen values.
         *
         * <p>
         * Ideally, given an implicitly or explicitly specified range of values,
         * each value would be chosen independently and uniformly from that range. In
         * practice, one may have to settle for some approximation to independence and
         * uniformity.
         * </p>
         * <p>
         * In the case of @c int, @c long, and @c boolean values, if
         * there is no explicit specification of range, then the range includes all
         * possible values of the type. In the case of @c float and @c double
         * values, first a value is always chosen uniformly from the set of
         * 2<sup><i>w</i></sup> values between 0.0 (inclusive) and 1.0 (exclusive),
         * where <i>w</i> is @b Float::PRECISION for @c float values
         * and @b Double::PRECISION for @c double
         * values, such that adjacent values differ by 2<sup>&minus;<i>w</i></sup>
         * (notice that this set is a <i>subset</i> of the set of
         * <i>all representable floating-point values</i> between 0.0 (inclusive) and 1.0 (exclusive));
         * then if an explicit range was specified, then the chosen number is
         * computationally scaled and translated to appear to have been chosen
         * approximately uniformly from that explicit range.
         * </p>
         * <p>
         * Each method that returns a stream produces a stream of values each of
         * which is chosen in the same manner as for a method that returns a single
         * pseudorandomly chosen value. For example, if @c r implements
         * @b RandomGenerator, then the method call @c r.ints(100) returns a
         * stream of 100 @c int values. These are not necessarily the exact same
         * values that would have been returned if instead @c r.nextInt() had been
         * called 100 times; all that is guaranteed is that each value in the stream is
         * chosen in a similar pseudorandom manner from the same range.
         * </p>
         * <p>
         * Every object that implements the @b RandomGenerator interface by
         * using a pseudorandom algorithm is assumed to contain a finite amount of
         * state. Using such an object to generate a pseudorandomly chosen value alters
         * its state by computing a new state as a function of the current state,
         * without reference to any information other than the current state. The number
         * of distinct possible states of such an object is called its <i>period</i>.
         * (Some implementations of the @b RandomGenerator interface may be truly
         * random rather than pseudorandom, for example relying on the statistical
         * behavior of a physical object to derive chosen values. Such implementations
         * do not have a fixed period.)
         * </p>
         * <p>
         * As a rule, objects that implement the @b RandomGenerator interface
         * need not be thread-safe. It is recommended that multithreaded applications
         * use either @b ThreadLocalRandom or (preferably) pseudorandom number
         * generators that implement the @b SplittableGenerator or
         * @b JumpableGenerator interface.
         * </p>
         * <p>
         * Objects that implement @b RandomGenerator are typically not
         * cryptographically secure. Consider instead using @b SecureRandom to get
         * a cryptographically secure pseudorandom number generator for use by
         * security-sensitive applications. Note, however, that @b SecureRandom
         * does implement the @b RandomGenerator interface, so that instances of
         * @b SecureRandom may be used interchangeably with other types of
         * pseudorandom generators in applications that do not require a secure
         * generator.
         * </p>
         * <p>
         * Unless explicit stated otherwise, the use of null for any method argument
         * will cause a NullPointerException.
         * </p>
         */
        class RandomGenerator : public virtual Object {
        public:
            /**
            * Returns an instance of @b RandomGenerator that utilizes the
            * @c name <em>algorithm</em>.
            *
            * @param name  Name of random number generator
            *              <em>algorithm</em>
            *
            * @return An instance of @b RandomGenerator
            *
            * @throws IllegalArgumentException if the named algorithm is not found
            */
            static RandomGenerator &forName(String const &name);

            /**
             * Returns a @b RandomGenerator meeting the minimal requirement
             * of having an <em>algorithm</em>
             * whose state bits are greater than or equal 64.
             *
             * @note  Since algorithms will improve over time, there is no
             * guarantee that this method will return the same algorithm over time.
             * <p>
             * The default implementation selects L32X64MixRandom.
             * </p>
             * @return a @b RandomGenerator
             */
            static RandomGenerator &defaultGenerator();

            /**
             * Return true if the implementation of RandomGenerator (algorithm) has been
             * marked for deprecation.
             *
             * @note Random number generator algorithms evolve over time; new
             * algorithms will be introduced and old algorithms will
             * lose standing. If an older algorithm is deemed unsuitable
             * for continued use, I will be marked as deprecated to indicate
             * that I may be removed at some point in the future.
             *
             * @return true if the implementation of RandomGenerator (algorithm) has been
             *         marked for deprecation
             *
             * @note The default implementation checks for the @Deprecated annotation.
             */
            virtual gbool isDeprecated() const = 0;

            /**
             * Returns a pseudorandomly chosen @c boolean value.
             *
             * <p>
             * The default implementation tests the high-order bit (sign bit) of a
             * value produced by @b nextInt(), on the
             * grounds that some algorithms for pseudorandom number generation produce
             * values whose high-order bits have better statistical quality than the
             * low-order bits.
             * </p>
             * @return a pseudorandomly chosen @c boolean value
             *
             * @note The default implementation produces a result based on the
             * sign bit of a number generated by @b nextInt().
             */
            virtual gbool nextBoolean() const;

            /**
             * Fills a user-supplied byte array with generated byte values
             * pseudorandomly chosen uniformly from the range of values between -128
             * (inclusive) and 127 (inclusive).
             *
             * @note Algorithm used to fill the byte array;
             *        @code
             *           void nextBytes(ByteArray& bytes) {
             *               int i = 0;
             *               int len = bytes.length();
             *               for (int words = len >> 3; words--> 0; ) {
             *                   long rnd = nextLong();
             *                   for (int n = 8; n--> 0; rnd >>>= Byte::SIZE)
             *                       bytes[i++] = (byte)rnd;
             *               }
             *               if (i < len)
             *                   for (long rnd = nextLong(); i < len; rnd >>>= Byte::SIZE)
             *                       bytes[i++] = (byte)rnd;
             *           }
             *         @endcode
             *
             * @param  bytes the byte array to fill with pseudorandom bytes
             *
             * @note The default implementation produces results from repeated calls
             * to @b nextLong().
             */
            virtual void nextBytes(ByteArray &bytes) const;

            /**
             * Returns a pseudorandom @c float value between zero (inclusive) and
             * one (exclusive).
             *
             * @return a pseudorandom @c float value between zero (inclusive) and one (exclusive)
             *
             * @note The default implementation uses the @b Float::PRECISION
             * high-order bits from a call to @b nextInt().
             */
            virtual gfloat nextFloat() const;

            /**
             * Returns a pseudorandomly chosen @c float value between zero
             * (inclusive) and the specified bound (exclusive).
             *
             * @param bound the upper bound (exclusive) for the returned value.
             *        Must be positive and finite
             *
             * @return a pseudorandomly chosen @c float value between
             *         zero (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if @c bound is not
             *         both positive and finite
             *
             * @note The default implementation checks that @c bound is a
             * positive finite float. Then invokes @c nextFloat(), scaling
             * the result so that the final result lies between @c 0.0f (inclusive)
             * and @c bound (exclusive).
             */
            virtual gfloat nextFloat(gfloat bound) const;

            /**
             * Returns a pseudorandomly chosen @c float value between the
             * specified origin (inclusive) and the specified bound (exclusive).
             *
             * @param origin the least value that can be returned
             * @param bound the upper bound (exclusive)
             *
             * @return a pseudorandomly chosen @c float value between the
             *         origin (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if @c origin is not finite,
             *         or @c bound is not finite, or @c origin
             *         is greater than or equal to @c bound
             *
             * @note The default implementation verifies that the @c origin
             *           and @c bound are valid then invokes @c nextFloat()
             *           scaling and translating the result to fit between @c origin
             *           and @c bound (exclusive).
             */
            virtual gfloat nextFloat(gfloat origin, gfloat bound) const;

            /**
             * Returns a pseudorandom @c double value between zero (inclusive) and
             * one (exclusive).
             *
             * @return a pseudorandom @c double value between zero (inclusive)
             *         and one (exclusive)
             *
             * @note The default implementation uses the @b Double::PRECISION
             * high-order bits from a call to @b nextLong().
             */
            virtual gdouble nextDouble() const;

            /**
             * Returns a pseudorandomly chosen @c double value between zero
             * (inclusive) and the specified bound (exclusive).
             *
             * @param bound the upper bound (exclusive) for the returned value.
             *        Must be positive and finite
             *
             * @return a pseudorandomly chosen @c double value between
             *         zero (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if @c bound is not
             *         both positive and finite
             *
             * @note The default implementation checks that @c bound is a
             * positive finite double. Then invokes @c nextDouble(), scaling
             * the result so that the final result lies between @c 0.0 (inclusive)
             * and @c bound (exclusive).
             */
            virtual gdouble nextDouble(gdouble bound) const;

            /**
             * Returns a pseudorandomly chosen @c double value between the
             * specified origin (inclusive) and the specified bound (exclusive).
             *
             * @param origin the least value that can be returned
             * @param bound the upper bound (exclusive) for the returned value
             *
             * @return a pseudorandomly chosen @c double value between the
             *         origin (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if @c origin is not finite,
             *         or @c bound is not finite, or @c origin
             *         is greater than or equal to @c bound

             * @note The default implementation verifies that the @c origin
             *           and @c bound are valid, then invokes @c nextDouble()
             *           scaling and translating the result to fit between @c origin
             *           and @c bound( exclusive).
             */
            virtual gdouble nextDouble(gdouble origin, gdouble bound) const;

            /**
             * Returns a pseudorandomly chosen @c int value.
             *
             * @return a pseudorandomly chosen @c int value
             *
             * @note The default implementation uses the 32 high-order bits from a call to
             * @b nextLong().
             */
            virtual gint nextInt() const;

            /**
             * Returns a pseudorandomly chosen @c int value between zero
             * (inclusive) and the specified bound (exclusive).
             *
             * @param bound the upper bound (exclusive) for the returned value.
             * Must be positive.
             *
             * @return a pseudorandomly chosen @c int value between
             *         zero (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if @c bound is not positive
             *
             * @note The default implementation checks that @c bound is a
             * positive @c int. Then invokes @c nextInt(), limiting the result
             * to be greater than or equal zero and less than @c bound. If @c bound
             * is a power of two then limiting is a simple masking operation. Otherwise,
             * the result is re-calculated by invoking @c nextInt() until the
             * result is greater than or equal zero and less than @c bound.
             */
            virtual gint nextInt(gint bound) const;

            /**
             * Returns a pseudorandomly chosen @c int value between the specified
             * origin (inclusive) and the specified bound (exclusive).
             *
             * @param origin the least value that can be returned
             * @param bound the upper bound (exclusive) for the returned value
             *
             * @return a pseudorandomly chosen @c int value between the
             *         origin (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if @c origin is greater than
             *         or equal to @c bound
             *
             * @note The default implementation checks that @c origin and
             * @c bound are positive @c ints. Then invokes @c nextInt(),
             * limiting the result to be greater that or equal @c origin and less
             * than @c bound. If @c bound is a power of two then limiting is a
             * simple masking operation. Otherwise, the result is re-calculated  by
             * invoking @c nextInt() until the result is greater than or equal
             * @c origin and less than @c bound.
             */
            virtual gint nextInt(gint origin, gint bound) const;

            /**
             * Returns a pseudorandomly chosen @c long value.
             *
             * @return a pseudorandomly chosen @c long value
             */
            virtual glong nextLong() const = 0;

            /**
             * Returns a pseudorandomly chosen @c long value between zero
             * (inclusive) and the specified bound (exclusive).
             *
             * @param bound the upper bound (exclusive) for the returned value.
             * Must be positive.
             *
             * @return a pseudorandomly chosen @c long value between
             *         zero (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if @c bound is not positive
             *
             * @note The default implementation checks that @c bound is a
             * positive  @c long. Then invokes @c nextLong(), limiting the
             * result to be greater than or equal zero and less than @c bound. If
             * @c bound is a power of two then limiting is a simple masking
             * operation. Otherwise, the result is re-calculated by invoking
             * @c nextLong() until the result is greater than or equal zero and
             * less than @c bound.
             */
            virtual glong nextLong(glong bound) const;

            /**
             * Returns a pseudorandomly chosen @c long value between the
             * specified origin (inclusive) and the specified bound (exclusive).
             *
             * @param origin the least value that can be returned
             * @param bound the upper bound (exclusive) for the returned value
             *
             * @return a pseudorandomly chosen @c long value between the
             *         origin (inclusive) and the bound (exclusive)
             *
             * @throws IllegalArgumentException if @c origin is greater than
             *         or equal to @c bound
             *
             * @note The default implementation checks that @c origin and
             * @c bound are positive @c longs. Then invokes @c nextLong(),
             * limiting the result to be greater than or equal @c origin and less
             * than @c bound. If @c bound is a power of two then limiting is a
             * simple masking operation. Otherwise, the result is re-calculated by
             * invoking @c nextLong() until the result is greater than or equal
             * @c origin and less than @c bound.
             */
            virtual glong nextLong(glong origin, glong bound) const;

            /**
             * Returns a @c double value pseudorandomly chosen from a Gaussian
             * (normal) distribution whose mean is 0 and whose standard deviation is 1.
             *
             * @return a @c double value pseudorandomly chosen from a
             *         Gaussian distribution
             *
             * @note The default implementation uses McFarland's fast modified
             * ziggurat algorithm (largely table-driven, with rare cases handled by
             * computation and rejection sampling). Walker's alias method for sampling
             * a discrete distribution also plays a role.
             */
            virtual gdouble nextGaussian() const;

            /**
             * Returns a @c double value pseudorandomly chosen from a Gaussian
             * (normal) distribution with a mean and standard deviation specified by the
             * arguments.
             *
             * @param mean the mean of the Gaussian distribution to be drawn from
             * @param standardDeviation the standard deviation (square root of the variance)
             *        of the Gaussian distribution to be drawn from
             *
             * @return a @c double value pseudorandomly chosen from the
             *         specified Gaussian distribution
             *
             * @throws IllegalArgumentException if @c standardDeviation is negative
             *
             * @note The default implementation uses McFarland's fast modified
             * ziggurat algorithm (largely table-driven, with rare cases handled by
             * computation and rejection sampling). Walker's alias method for sampling
             * a discrete distribution also plays a role.
             */
            virtual gdouble nextGaussian(gdouble mean, gdouble standardDeviation) const;

            /**
             * Returns a nonnegative @c double value pseudorandomly chosen from
             * an exponential distribution whose mean is 1.
             *
             * @return a nonnegative @c double value pseudorandomly chosen from an
             *         exponential distribution
             *
             * @note The default implementation uses McFarland's fast modified
             * ziggurat algorithm (largely table-driven, with rare cases handled by
             * computation and rejection sampling). Walker's alias method for sampling
             * a discrete distribution also plays a role.
             */
            virtual gdouble nextExponential() const;

            virtual void setSeed(glong seed) = 0;

            /**
             * Return the properties of this generator
             *
             * @return the properties of this generator.
             */
            virtual RandomGeneratorProperties properties() const = 0;

            static Array<RandomGenerator> availableGenerators();
        };
    } // random
} // core

#endif //CORE24_RANDOMGENERATOR_H
