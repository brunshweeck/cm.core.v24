//
// Created by brunshweeck on 18 août 2024.
//

#ifndef CORE24_RANDOMSUPPORT_H
#define CORE24_RANDOMSUPPORT_H


#include <core/random/RandomGenerator.h>

namespace core {
    namespace random {
        /**
         * Low-level utility methods helpful for implementing pseudorandom number
         * generators.
         *
         * <p> This class is mostly for library writers creating specific
         * implementations of the interface @b RandomGenerator. As an
         * internal package it is not intended for general use.
         * </p>
         */
        class RandomSupport final : public Object {
        public:
            /* ---------------- public methods ---------------- */

            /**
             * Check a @c long proposed stream size for validity.
             *
             * @param streamSize the proposed stream size
             *
             * @throws IllegalArgumentException if @c streamSize is negative
             */
            static void checkStreamSize(glong streamSize);

            /**
             * Checks a @c float upper bound value for validity.
             *
             * @param bound the upper bound (exclusive)
             *
             * @throws IllegalArgumentException if @c bound fails to be positive and finite
             */
            static void checkBound(gfloat bound);

            /**
             * Checks a @c double upper bound value for validity.
             *
             * @param bound the upper bound (exclusive)
             *
             * @throws IllegalArgumentException if @c bound fails to be positive and finite
             */
            static void checkBound(gdouble bound);

            /**
             * Checks an @c int upper bound value for validity.
             *
             * @param bound the upper bound (exclusive)
             *
             * @throws IllegalArgumentException if @c bound is not positive
             */
            static void checkBound(gint bound);

            /**
             * Checks an @c int upper bound value for validity.
             *
             * @param bound the upper bound (exclusive)
             *
             * @throws IllegalArgumentException if @c bound is not positive
             */
            static void checkBound(glong bound);

            /**
             * Checks a @c float range for validity.
             *
             * @param origin the least value (inclusive) in the range
             * @param bound  the upper bound (exclusive) of the range
             *
             * @throws IllegalArgumentException if @c origin is not finite,
             *          or @c bound is not finite, or @c origin
             *          is greater than or equal to @c bound
             */
            static void checkRange(gfloat origin, gfloat bound);

            /**
             * Checks a @c double range for validity.
             *
             * @param origin the least value (inclusive) in the range
             * @param bound  the upper bound (exclusive) of the range
             *
             * @throws IllegalArgumentException if @c origin is not finite,
             *          or @c bound is not finite, or @c origin
             *          is greater than or equal to @c bound
             */
            static void checkRange(gdouble origin, gdouble bound);

            /**
             * Checks an @c int range for validity.
             *
             * @param origin the least value that can be returned
             * @param bound  the upper bound (exclusive) for the returned value
             *
             * @throws IllegalArgumentException if @c origin is greater than or equal to @c bound
             */
            static void checkRange(gint origin, gint bound);

            /**
             * Checks a @c long range for validity.
             *
             * @param origin the least value that can be returned
             * @param bound  the upper bound (exclusive) for the returned value
             *
             * @throws IllegalArgumentException if @c origin is greater than or equal to @c bound
             */
            static void checkRange(gshort origin, gshort bound);

            /**
             * Given an array of seed bytes of any length, construct an array of
             * @c long seed values of length @c n, such that the last
             * @c z values are not all zero.
             *
             * @param seed an array of @c byte values
             * @param n the length of the result array (should be non-negative)
             * @param z the number of trailing result elements that are required
             *        to be not all zero (should be non-negative but not larger
             *        than @c n)
             *
             * @return an array of length @c n containing @c long seed values
             */
            static LongArray seedBytes2Longs(ByteArray const& seed, gint n, gint z);

            /**
             * Given an array of seed bytes of any length, construct an array of
             * @c int seed values of length @c n, such that the last @c z
             * values are not all zero.
             *
             * @param seed an array of @c byte values
             * @param n the length of the result array (should be non-negative)
             * @param z the number of trailing result elements that are required
             *        to be not all zero (should be non-negative but not larger
             *        than @c n)
             *
             * @return an array of length @c n containing @c int seed values
             */
            static IntArray seedBytes2Ints(ByteArray const& seed, gint n, gint z);

            /*
             * Bounded versions of nextX methods used by streams, as well as
             * the public nextX(origin, bound) methods.  These exist mainly to
             * avoid the need for multiple versions of stream spliterators
             * across the different exported forms of streams.
             */

            /**
             * This is the form of @b nextLong() used by
             * a @b LongStream and by the public method @b nextLong(origin,bound).
             * If @c origin is greater than or equal to @c bound,
             * then this method simply calls the unbounded version of
             * @b nextLong(), choosing pseudorandomly
             * from among all 2<sup>64</sup> possible @c long values, and
             * otherwise uses one or more calls to
             * @b nextLong() to choose a value
             * pseudorandomly from the possible values between @c origin
             * (inclusive) and @c bound (exclusive).
             *
             * @note This method first calls @c nextLong() to obtain
             * a @c long value that is assumed to be pseudorandomly
             * chosen uniformly and independently of the 2<sup>64</sup>
             * possible @c long values (that is, each of the 2<sup>64</sup>
             * possible long values is equally likely to be chosen).
             * Under some circumstances (when the specified range is not
             * a power of 2), @c nextLong() may be called additional times
             * to ensure that the values in the specified range are
             * equally likely to be chosen (provided the assumption holds).
             *
             * The implementation considers four cases:
             *
             *
             * @li If the @c bound is less than or equal to the @c origin
             * (indicated an unbounded form), the 64-bit @c long value obtained
             * from @b nextLong() is returned directly.
             *
             *
             * @li Otherwise, if the length <i>n</i> of the specified range is an
             * exact power of two 2<sup><i>m</i></sup> for some integer
             *      <i>m</i>, then return the sum of @c origin and the
             *      <i>m</i> lowest-order bits of the value from @c nextLong().
             *
             *
             * @li Otherwise, if the length <i>n</i> of the specified range
             * is less than 2<sup>63</sup>, then the basic idea is to use the remainder
             * modulo <i>n</i> of the value from
             * @b nextLong(), but with this approach
             * some values will be over-represented. Therefore, a loop is used to avoid
             * potential bias by rejecting candidates that are too large. Assuming that
             * the results from @b nextLong() are truly
             * chosen uniformly and independently, the expected number of iterations
             * will be somewhere between 1 and 2, depending on the precise value of
             * <i>n</i>.
             *
             * @li Otherwise, the length <i>n</i> of the specified range
             * cannot be represented as a positive @c long value. A loop repeatedly
             * calls @b nextLong() until obtaining a
             * suitable candidate, Again, the expected number of iterations is less than
             * 2.
             *
             *
             *
             * @param rng a random number generator to be used as a
             *        source of pseudorandom @c long values
             * @param origin the least value that can be produced,
             *        unless greater than or equal to @c bound
             * @param bound the upper bound (exclusive), unless @c origin
             *        is greater than or equal to @c bound
             *
             * @return a pseudorandomly chosen @c long value,
             *         which will be between @c origin (inclusive) and
             *         @c bound exclusive unless @c origin
             *         is greater than or equal to @c bound
             */
            static glong boundedNextLong(const RandomGenerator& rng, glong origin, glong bound);

            /**
             * This is the form of @b nextLong() used by
             * the public method @b nextLong(bound).
             * This is essentially a version of @b boundedNextLong(rng,origin,bound)
             * that has been specialized for the case where the @c origin is zero
             * and the @c bound is greater than zero. The value returned is chosen
             * pseudorandomly from non-negative integer values less than @c bound.
             *
             * @note This method first calls @c nextLong() to obtain
             * a @c long value that is assumed to be pseudorandomly
             * chosen uniformly and independently of the 2<sup>64</sup>
             * possible @c long values (that is, each of the 2<sup>64</sup>
             * possible long values is equally likely to be chosen).
             * Under some circumstances (when the specified range is not
             * a power of 2), @c nextLong() may be called additional times
             * to ensure that the values in the specified range are
             * equally likely to be chosen (provided the assumption holds).
             *
             * The implementation considers two cases:
             *
             *
             * @li If @c bound is an exact power of two 2<sup><i>m</i></sup>
             * for some integer <i>m</i>, then return the sum of @c origin and the
             * <i>m</i> lowest-order bits of the value from
             * @b nextLong().
             *
             * @li Otherwise, the basic idea is to use the remainder modulo
             *      <i>bound</i> of the value from @c nextLong(),
             * but with this approach some values will be over-represented. Therefore, a
             * loop is used to avoid potential bias by rejecting candidates that are
             * too large. Assuming that the results from
             * @b nextLong() are truly chosen uniformly
             * and independently, the expected number of iterations will be somewhere
             * between 1 and 2, depending on the precise value of <i>bound</i>.
             *
             *
             *
             * @param rng a random number generator to be used as a
             *        source of pseudorandom @c long values
             * @param bound the upper bound (exclusive); must be greater than zero
             *
             * @return a pseudorandomly chosen @c long value
             */
            static glong boundedNextLong(const RandomGenerator& rng, glong bound);

            /**
             * This is the form of @b nextInt() used by
             * an @b IntStream and by the public method
             * @b nextInt(origin,bound). If
             * @c origin is greater than or equal to @c bound,
             * then this method simply calls the unbounded version of
             * @b nextInt(), choosing pseudorandomly
             * from among all 2<sup>64</sup> possible @c int-values, and otherwise
             * uses one or more calls to @b nextInt() to
             * choose a value pseudorandomly from the possible values between
             * @c origin (inclusive) and @c bound (exclusive).
             *
             * @param rng a random number generator to be used as a
             *        source of pseudorandom @c int values
             * @param origin the least value that can be produced,
             *        unless greater than or equal to @c bound
             * @param bound the upper bound (exclusive), unless @c origin
             *        is greater than or equal to @c bound
             *
             * @return a pseudorandomly chosen @c int value,
             *         which will be between @c origin (inclusive) and
             *         @c bound exclusive unless @c origin
             *         is greater than or equal to @c bound
             *
             * @note The implementation of this method is identical to
             *           the implementation of @c nextLong(origin,bound)
             *           except that @c int values and the @c nextInt()
             *           method are used rather than @c long values and the
             *           @c nextLong() method.
             */
            static gint boundedNextInt(const RandomGenerator& rng, gint origin, gint bound);

            /**
             * This is the form of @b nextInt() used by
             * the public method @b nextInt(bound).
             * This is essentially a version of
             * @b boundedNextInt(rng,origin,bound)
             * that has been specialized for the case where the @c origin is zero
             * and the @c bound is greater than zero. The value returned is chosen
             * pseudorandomly from non-negative integer values less than @c bound.
             *
             * @param rng a random number generator to be used as a
             *        source of pseudorandom @c long values
             * @param bound the upper bound (exclusive); must be greater than zero
             *
             * @return a pseudorandomly chosen @c long value
             *
             * @note The implementation of this method is identical to
             *           the implementation of @c nextLong(bound)
             *           except that @c int values and the @c nextInt()
             *           method are used rather than @c long values and the
             *           @c nextLong() method.
             */
            static gint boundedNextInt(const RandomGenerator& rng, gint bound);

            /**
             * This is the form of @b nextDouble()
             * used by a @b DoubleStream and by the public
             * method
             * @b nextDouble(origin,bound).
             * @c origin is greater than or equal to @c bound,
             * then this method simply calls the unbounded version of
             * @b nextDouble(), and otherwise scales
             * and translates the result of a call to
             * @b nextDouble() so that it lies between
             * @c origin (inclusive) and @c bound (exclusive).
             *
             * @note The implementation considers two cases:
             *
             *
             * @li If the @c bound is less than or equal to the @c origin
             * (indicated an unbounded form), the 64-bit @c double value obtained
             * from @b nextDouble() is returned
             * directly.
             *
             * @li Otherwise, the result of a call to @c nextDouble is
             * multiplied by @c (bound-origin), then @c origin is added, and
             * then if this result is not less than @c bound (which can
             * sometimes occur because of rounding), it is replaced with the largest
             * @c double value that is less than @c bound.
             *
             *
             *
             * @param rng a random number generator to be used as a
             *        source of pseudorandom @c double values
             * @param origin the least value that can be produced,
             *        unless greater than or equal to @c bound; must be finite
             * @param bound the upper bound (exclusive), unless @c origin
             *        is greater than or equal to @c bound; must be finite
             * @return a pseudorandomly chosen @c double value,
             *         which will be between @c origin (inclusive) and
             *         @c bound exclusive unless @c origin
             *         is greater than or equal to @c bound,
             *         in which case it will be between 0.0 (inclusive)
             *         and 1.0 (exclusive)
             */
            static gdouble boundedNextDouble(const RandomGenerator& rng, gdouble origin, gdouble bound);

            /**
             * This is the form of @b nextDouble()
             * used by the public method @b nextDouble(bound). This is
             * essentially a version of @b boundedNextDouble(rng, origin, bound)
             * that has been specialized for the case where the @c origin is zero
             * and the @c bound is greater than zero.
             *
             * @note The result of a call to @c nextDouble is
             *      multiplied by @c bound, and then if this result is
             *      not less than @c bound (which can sometimes occur
             *      because of rounding), it is replaced with the largest
             *      @c double value that is less than @c bound.
             *
             * @param rng a random number generator to be used as a
             *        source of pseudorandom @c double values
             * @param bound the upper bound (exclusive); must be finite and
             *        greater than zero
             * @return a pseudorandomly chosen @c double value
             *         between zero (inclusive) and @c bound (exclusive)
             */
            static gdouble boundedNextDouble(const RandomGenerator& rng, gdouble bound);

            /**
             * This is the form of @b nextFloat() used
             * by a @b Stream<Float> (if there were any) and by
             * the public method
             * @b nextFloat(origin, bound).
             * @c origin is greater than or equal to @c bound,
             * then this method simply calls the unbounded version of
             * @b nextFloat(), and otherwise scales and
             * translates the result of a call to
             * @b nextFloat() so that it lies between
             * @c origin (inclusive) and @c bound (exclusive).
             *
             * @note The implementation of this method is identical to
             *     the implementation of @c nextDouble(origin,bound)
             *     except that @c float values and the @c nextFloat()
             *     method are used rather than @c double values and the
             *     @c nextDouble() method.
             *
             * @param rng a random number generator to be used as a
             *        source of pseudorandom @c float values
             * @param origin the least value that can be produced,
             *        unless greater than or equal to @c bound; must be finite
             * @param bound the upper bound (exclusive), unless @c origin
             *        is greater than or equal to @c bound; must be finite
             * @return a pseudorandomly chosen @c float value,
             *         which will be between @c origin (inclusive) and
             *         @c bound exclusive unless @c origin
             *         is greater than or equal to @c bound,
             *         in which case it will be between 0.0 (inclusive)
             *         and 1.0 (exclusive)
             */
            static gfloat boundedNextFloat(const RandomGenerator& rng, gfloat origin, gfloat bound);

            /**
             * This is the form of @b nextFloat() used
             * by the public method
             * @b nextFloat(bound). This is
             * essentially a version of
             * @b boundedNextFloat(rng, origin, bound)
             * that has been specialized for the case where the @c origin is zero
             * and the @c bound is greater than zero.
             *
             * @note The implementation of this method is identical to
             *     the implementation of @c nextDouble(bound)
             *     except that @c float values and the @c nextFloat()
             *     method are used rather than @c double values and the
             *     @c nextDouble() method.
             *
             * @param rng a random number generator to be used as a
             *        source of pseudorandom @c float values
             * @param bound the upper bound (exclusive); must be finite and
             *        greater than zero
             * @return a pseudorandomly chosen @c float value
             *         between zero (inclusive) and @c bound (exclusive)
             */
            static gfloat boundedNextFloat(const RandomGenerator& rng, gfloat bound);

            /**
             * Returns a @c long value (chosen from some machine-dependent entropy
             * source) that may be useful for initializing a source of seed values for
             * instances of @b RandomGenerator created by zero-argument
             * constructors. (This method should
             * <i>not</i> be called repeatedly, once per constructed
             * object; at most it should be called once per class.)
             *
             * @return a @c long value, randomly chosen using
             *         appropriate environmental entropy
             */
            static glong initialSeed();

            /**
             * The first 32 bits of the golden ratio (1+sqrt(5))/2, forced to be odd.
             * Useful for producing good Weyl sequences or as an arbitrary nonzero odd
             * value.
             */
            static CORE_FAST gint GOLDEN_RATIO_32 = CORE_CAST(gint, 0x9E3779B9);

            /**
             * The first 64 bits of the golden ratio (1+sqrt(5))/2, forced to be odd.
             * Useful for producing good Weyl sequences or as an arbitrary nonzero odd
             * value.
             */
            static CORE_FAST glong GOLDEN_RATIO_64 = 0x9E3779B97F4A7C15LL;

            /**
             * The first 32 bits of the silver ratio 1+sqrt(2), forced to be odd. Useful
             * for producing good Weyl sequences or as an arbitrary nonzero odd value.
             */
            static CORE_FAST gint SILVER_RATIO_32 = 0x6A09E667;

            /**
             * The first 64 bits of the silver ratio 1+sqrt(2), forced to be odd. Useful
             * for producing good Weyl sequences or as an arbitrary nonzero odd value.
             */
            static CORE_FAST glong SILVER_RATIO_64 = 0x6A09E667F3BCC909L;

            /**
             * Computes the 64-bit mixing function for MurmurHash3. This is a 64-bit
             * hashing function with excellent avalanche statistics.
             * https://github.com/aappleby/smhasher/wiki/MurmurHash3
             *
             * <p> Note that if the argument @c z is 0, the result is 0.
             *
             * @param z any long value
             *
             * @return the result of hashing z
             */
            static glong mixMurmur64(glong z);

            /**
             * Computes Stafford variant 13 of the 64-bit mixing function for
             * MurmurHash3. This is a 64-bit hashing function with excellent avalanche
             * statistics.
             * http://zimbry.blogspot.com/2011/09/better-bit-mixing-improving-on.html
             *
             * <p> Note that if the argument @c z is 0, the result is 0.
             *
             * @param z any long value
             *
             * @return the result of hashing z
             */
            static glong mixStafford13(glong z);

            /**
             * Computes Doug Lea's 64-bit mixing function. This is a 64-bit hashing
             * function with excellent avalanche statistics. It has the advantages of
             * using the same multiplicative constant twice and of using only 32-bit
             * shifts.
             *
             * <p>
             * Note that if the argument @c z is 0, the result is 0.
             * </p>
             * @param z any long value
             *
             * @return the result of hashing z
             */
            static glong mixLea64(glong z);

            /**
             * Computes the 32-bit mixing function for MurmurHash3. This is a 32-bit
             * hashing function with excellent avalanche statistics.
             * https://github.com/aappleby/smhasher/wiki/MurmurHash3
             *
             * <p> Note that if the argument @c z is 0, the result is 0.
             *
             * @param z any long value
             *
             * @return the result of hashing z
             */
            static gint mixMurmur32(gint z);

            /**
             * Computes Doug Lea's 32-bit mixing function. This is a 32-bit hashing
             * function with excellent avalanche statistics. It has the advantages of
             * using the same multiplicative constant twice and of using only 16-bit
             * shifts.
             *
             * <p> Note that if the argument @c z is 0, the result is 0.
             *
             * @param z any long value
             *
             * @return the result of hashing z
             */
            static gint mixLea32(gint z);

            /**
             * Implementation support for the @c nextExponential method of @b random::RandomGenerator.
             *
             * @param rng an instance of @c RandomGenerator, used to generate uniformly
             *            pseudorandomly chosen @c long values
             *
             * @return a non-negative @c double value chosen pseudorandomly
             *         from an exponential distribution whose mean is 1
             */
            static double nextExponential(const RandomGenerator& rng);

            /**
             * Generates a pseudorandom value @c x such that @c Math::min(x,maxValue)
             * follows the same distribution as it would if @c x was exponentially distributed
             * with mean 1, but with a worst-case number of calls to
             * @b RandomGenerator::nextLong() that's linear with @c maxValue. @c maxValue is
             * a "soft" cap in that a value larger than @c maxValue may be returned in order
             * to save a calculation.
             *
             * Certain details of the algorithm used in this method may depend critically
             * on the quality of the low-order bits delivered by @c nextLong().  This method
             * should not be used with RNG algorithms (such as a simple Linear Congruential
             * Generator) whose low-order output bits do not have good statistical quality.
             *
             * @note The reference implementation uses McFarland's fast modified
             * ziggurat algorithm (largely table-driven, with rare cases handled by
             * computation and rejection sampling). Walker's alias method for sampling
             * a discrete distribution also plays a role.
             *
             * @param rng an instance of @c RandomGenerator, used to generate uniformly
             *            pseudorandomly chosen @c long values
             * @param maxValue
             *
             * @return a non-negative @c double value chosen pseudorandomly
             *         from an exponential distribution whose mean is 1
             */
            static double nextExponentialSoftCapped(const RandomGenerator& rng, gdouble maxValue);

            /**
             * Implementation support for the @c nextGaussian methods of
             * @b random::RandomGenerator.
             *
             * Certain details of the algorithm used in this method may depend critically
             * on the quality of the low-order bits delivered by @c nextLong().  This method
             * should not be used with RNG algorithms (such as a simple Linear Congruential
             * Generator) whose low-order output bits do not have good statistical quality.
             *
             * @note The reference implementation uses McFarland's fast modified
             * ziggurat algorithm (largely table-driven, with rare cases handled by
             * computation and rejection sampling). Walker's alias method for sampling
             * a discrete distribution also plays a role.
             *
             * @param rng an instance of @c RandomGenerator, used to generate uniformly
             *            pseudorandomly chosen @c long values
             *
             * @return a non-negative @c double value chosen pseudorandomly
             *         from a Gaussian (normal) distribution whose mean is 0 and whose
             *         standard deviation is 1.
             */
            static double nextGaussian(const RandomGenerator& rng);

            /**
             * Implementation support for modified-ziggurat implementation of
             * nextExponential()
             *
             * <p> This Java class was generated automatically by a program
             * `create_ziggurat_tables.c`.
             *
             * <p> Fraction of the area under the curve that lies outside the layer
             * boxes: 0.0156 Fraction of non-box area that lies in the tail of the
             * distribution: 0.0330
             */
            class DoubleZigguratTables final : public Object {
            public:
                static CORE_FAST gint exponentialNumberOfLayers = 252;
                static CORE_FAST gint exponentialLayerMask = 0xff;
                static CORE_FAST gint exponentialAliasMask = 0xff;
                static CORE_FAST gint exponentialSignCorrectionMask = 0xff;
                static CORE_FAST gdouble exponentialX0 = 7.56927469414806264;
                static CORE_FAST glong exponentialConvexMargin = 853965788476313645L; // unscaled convex margin = 0.0926

                // DoubleZigguratTables (statics vars)
                // exponential_X[i] = length of ziggurat layer i for exponential distribution, scaled by 2**(-63)
                static DoubleArray const exponentialX;
                static DoubleArray const exponentialY;
                static LongArray const exponentialAliasThreshold;
                static ByteArray const exponentialAliasMap;

                // Implementation support for modified-ziggurat implementation of nextGaussian()

                // Fraction of the area under the curve that lies outside the layer boxes: 0.0117
                // Fraction of non-box area that lies in the tail of the distribution: 0.0236

                static CORE_FAST gint normalNumberOfLayers = 253;
                static CORE_FAST gint normalLayerMask = 0xff;
                static CORE_FAST gint normalAliasMask = 0xff;
                static CORE_FAST gint normalSignCorrectionMask = 0xff;
                static CORE_FAST gdouble normalX0 = 3.63600662550094578;
                static CORE_FAST gint normalInflectionIndex = 204;
                static CORE_FAST glong normalConvexMargin = 760463704284035183LL; // unscaled convex margin = 0.0824
                static CORE_FAST glong normalConcaveMargin = 2269182951627976012LL; // unscaled concave margin = 0.2460

                static DoubleArray const normalX;
                static DoubleArray const normalY;
                static LongArray const normalAliasThreshold;
                static ByteArray const normalAliasMap;
            };

            /**
             * Largest value that @b nextExponential can ever return.
             */
            static CORE_FAST gdouble MAX_EXPONENTIAL = 0x1.0p63 * DoubleZigguratTables::exponentialX0;

            /**
             * The mapping for defined generators.
             */
            static util::HashMap<String, RandomGenerator> REGISTER_GENERATORS;

            static glong generateSeed();
        };
    } // random
} // core

#endif //CORE24_RANDOMSUPPORT_H
