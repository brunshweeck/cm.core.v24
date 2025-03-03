//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {
    /**
     * This package contains classes and interfaces that support a generic API
     * for random number generation.
     *
     * <p>
     * These classes and interfaces support the definition and use of "random
     * generators", a term covering what have traditionally been called "random
     * number generators" as well as generators of other sorts of randomly chosen
     * values (eg. booleans). These classes and interfaces cover not only
     * deterministic (pseudorandom) algorithms but also generators of values that
     * use some "truly random" physical source (stochastic algorithms perhaps making
     * use of thermal noise, for example, or quantum-mechanical effects).
     * </p>
     * <p>
     * The principal interface is @b RandomGenerator, which provides
     * methods for requesting individual values of type @c int, @c long,
     * @c float, @c double, or @c bool chosen pseudorandomly
     * from a uniform distribution; methods for requesting values of type
     * @c double chosen pseudorandomly from a normal distribution or from an
     * exponential distribution; and methods for creating streams of values of type
     * @c int, @c long, or @c double chosen pseudorandomly from a
     * uniform distribution (such streams are spliterator-based, allowing for
     * parallel processing of their elements). There are also static factory methods
     * for creating an instance of a specific random number generator algorithm
     * given its name.
     * </p>
     * <p>
     * The principal supporting class is @b RandomGeneratorFactory. This
     * can be used to generate multiple random number generators for a specific
     * algorithm. @b RandomGeneratorFactory also provides methods for
     * selecting random number generator algorithms. RandomGeneratorFactory
     * registers implementations of @b RandomGenerator interface using the
     * service provider API.
     * </p>
     * <p>
     * An important subsidiary interface is
     * @b RandomGenerator.StreamableGenerator, which provides methods for
     * creating spliterator-based streams of @b RandomGenerator objects,
     * allowing for parallel processing of these objects using multiple threads.
     * Unlike @b core::util::Random, most implementations of
     * @b RandomGenerator are <i>not</i> thread-safe. The intent is that
     * instances should not be shared among threads; rather, each thread should have
     * its own random generator(s) to use. The various pseudorandom algorithms
     * provided by this package are designed so that multiple instances will (with
     * very high probability) behave as if statistically independent.
     * </p>
     * <p>
     * For many purposes, these are the only two interfaces that a consumer of
     * pseudorandom values will need. There are also some more specialized
     * interfaces that describe more specialized categories of random number
     * generators @b RandomGenerator.SplittableGenerator SplittableGenerator,
     * @b RandomGenerator.JumpableGenerator JumpableGenerator,
     * @b RandomGenerator.LeapableGenerator LeapableGenerator, and
     * @b RandomGenerator.ArbitrarilyJumpableGenerator ArbitrarilyJumpableGenerator
     * that have specific strategies for creating statistically independent instances.
     * </p>
     * <h3>Using the Random Number Generator Interfaces</h3>
     *
     * To get started, an application should first create one instance of a
     * generator class. Assume that the contents of the package
     * @b core::random has been imported:
     *
     * <p>@c using namespace core::random;</p>
     *
     * Then one can choose a specific implementation by giving the name of a generator
     * algorithm to the static method @b RandomGenerator::of, in which case the
     * no-arguments constructor for that implementation is used:
     *
     * <p>@c auto& g = RandomGenerator.of("L64X128MixRandom");</p>
     *
     * For a single-threaded application, this is all that is needed. One can then
     * invoke methods of @c g such as
     * @b nextLong(),
     * @b nextInt(),
     * @b nextFloat(),
     * @b nextDouble() and
     * @b nextBoolean() to generate individual
     * randomly chosen values. One can also use the methods
     * @b ints(), @b longs() and @b doubles() to create streams of randomly
     * chosen values. The methods @b nextGaussian() and @b nextExponential() draw floating-point
     * values from nonuniform distributions.
     *
     * <p> For a multi-threaded application, one can repeat the preceding steps
     * to create additional @em RandomGenerators, but
     * often it is preferable to use methods of the one single initially
     * created generator to create others like it. (One reason is that some
     * generator algorithms, if asked to create a new set of generators all at
     * once, can make a special effort to ensure that the new generators are
     * statistically independent.) If the initial generator implements the
     * interface @b RandomGenerator.StreamableGenerator, then the method
     * @b rngs() can be used to
     * create a stream of generators. If this is a parallel stream, then it is
     * easy to get parallel execution by using the
     * @b map() method on the stream.
     * <p> For a multi-threaded application that forks new threads dynamically,
     * another approach is to use an initial generator that implements the interface
     * @b RandomGenerator.SplittableGenerator, which is then considered to
     * "belong" to the initial thread for its exclusive use; then whenever any
     * thread needs to fork a new thread, it first uses the
     * @b split() method of its own
     * generator to create a new generator, which is then passed to the newly
     * created thread for exclusive use by that new thread.
     *
     *
     * <h3>Choosing a Random Number Generator Algorithm</h3>
     *
     * <p>
     * There are three groups of random number generator algorithm provided
     * in Java: the Legacy group, the LXM group, and the Xoroshiro/Xoshiro group.
     * </p>
     * <p>
     * The legacy group includes random number generators:
     * Random, ThreadLocalRandom, SplittableRandom, and
     * SecureRandom. Random (LCG) is the weakest of the available algorithms, and it
     * is recommended that users migrate to newer algorithms. If an application
     * requires a random number generator algorithm that is cryptographically
     * secure, then it should continue to use an instance of the class @b java.security.SecureRandom.
     * </p>
     * <p>
     * The algorithms in the LXM group are similar to each other. The parameters
     * of each algorithm can be found in the algorithm name. The number after "L" indicates the
     * number of state bits for the LCG subgenerator, and the number after "X" indicates the
     * number of state bits for the XBG subgenerator. "Mix" indicates that
     * the algorithm uses an 8-operation bit-mixing function; "StarStar" indicates use
     * of a 3-operation bit-scrambler.
     * </p>
     * <p>
     * The algorithms in the Xoroshiro/Xoshiro group are more traditional algorithms
     * (see David Blackman and Sebastiano Vigna, "Scrambled Linear Pseudorandom
     * Number Generators," ACM Transactions on Mathematical Software, 2021);
     * the number in the name indicates the number of state bits.
     * </p>
     * <p>
     * For applications (such as physical simulation, machine learning, and
     * games) that do not require a cryptographically secure algorithm, this package
     * provides multiple implementations of interface @b RandomGenerator that
     * provide trade-offs among speed, space, period, accidental correlation, and
     * equidistribution properties.
     * </p>
     * <p>
     * For applications with no special requirements,
     * @c L64X128MixRandom has a good balance among speed, space,
     * and period, and is suitable for both single-threaded and multi-threaded
     * applications when used properly (a separate instance for each thread).
     * </p>
     * <p>
     * If the application uses only a single thread, then
     * @c Xoroshiro128PlusPlus is even smaller and faster, and
     * certainly has a sufficiently long period.
     * </p>
     * <p>
     * For an application running in a 32-bit hardware environment and using
     * only one thread or a small number of threads, @c L32X64MixRandom may be a good
     * choice.
     * </p>
     * <p>
     * For an application that uses many threads that are allocated in one batch
     * at the start of the computation, either a "jumpable" generator such as
     * @c Xoroshiro128PlusPlus or
     * @c Xoshiro256PlusPlus may be used, or a "splittable"
     * generator such as @c L64X128MixRandom or
     * @c L64X256MixRandom may be used.
     * </p>
     * <p>
     * For an application that creates many threads dynamically, perhaps through
     * the use of spliterators, a "splittable" generator such as
     * @c L64X128MixRandom or @c L64X256MixRandom is
     * recommended. If the number of generators created dynamically may
     * be very large (millions or more), then using generators such as
     * @c L128X128MixRandom or @c L128X256MixRandom,
     * which use a 128-bit parameter rather than a 64-bit parameter for their LCG
     * subgenerator, will make it much less likely that two instances use the same
     * state cycle.
     * </p>
     * <p>
     * For an application that uses tuples of consecutively generated values, it
     * may be desirable to use a generator that is <i>k</i>-equidistributed such
     * that <i>k</i> is at least as large as the length of the tuples being
     * generated. The generator @c L64X256MixRandom is provably
     * 4-equidistributed, and @c L64X1024MixRandom is provably
     * 16-equidistributed.
     * </p>
     * <p>
     * For applications that generate large permutations, it may be best to use
     * a generator whose period is much larger than the total number of possible
     * permutations; otherwise it will be impossible to generate some of the
     * intended permutations. For example, if the goal is to shuffle a deck of 52
     * cards, the number of possible permutations is 52! (52 factorial), which is
     * larger than 2<sup>225</sup> (but smaller than 2<sup>226</sup>), so it may be
     * best to use a generator whose period at least 2<sup>256</sup>, such as
     * @c L64X256MixRandom or @c L64X1024MixRandom
     * or @c L128X256MixRandom or
     * @c L128X1024MixRandom. (It is of course also necessary to
     * provide sufficiently many seed bits when the generator is initialized, or
     * else it will still be impossible to generate some of the intended
     * permutations.)
     * </p>
     *
     * <h3>Random Number Generator Algorithms Available</h3>
     *
     * These algorithms [in the table below] must be found with the current version
     * of Java SE. A particular JDK implementation may recognize additional
     * algorithms; check the JDK's documentation for details. The set of algorithms
     * required by Java SE may be updated by changes to the Java SE specification.
     * Over time, new algorithms may be added and old algorithms may be removed.
     * <p>
     * In addition, as another life-cycle phase, an algorithm may be
     * @em deprecated. A deprecated algorithm is
     * not recommended for use. If a required algorithm is deprecated, it may be
     * removed in a future release. Due to advances in random number generator
     * algorithm development and analysis, an algorithm may be deprecated during the
     * lifetime of a particular Java SE release. Changing the deprecation status of
     * an algorithm is <em>not</em> a specification change. <br/>
     * - <b>Algorithm</b>  | <b>Group</b>  | <b>Period</b>  | <b>StateBits</b>  | <b>Equidistribution</b><br/>
     * - @c L128X1024MixRandom | LXM  | (2<sup>1024</sup> - 1) × 2<sup>128</sup> | 1152 bits | 1 <br/>
     * - @c L128X128MixRandom | LXM  | (2<sup>128</sup> - 1) × 2<sup>128</sup> | 256 bits | 1 <br/>
     * - @c L128X256MixRandom | LXM  | (2<sup>256</sup> - 1) × 2<sup>128</sup> | 384 bits | 1 <br/>
     * - @c L32X64MixRandom | LXM  | (2<sup>64</sup> - 1) × 2<sup>32</sup> | 96 bits | 1 <br/>
     * - @c L64X1024MixRandom | LXM  | (2<sup>1024</sup> - 1) × 2<sup>64</sup> | 1088 bits | 16 <br/>
     * - @c L64X128MixRandom | LXM  | (2<sup>128</sup> - 1) × 2<sup>64</sup> | 192 bits | 2 <br/>
     * - @c L64X128StarStarRandom | LXM  | (2<sup>128</sup> - 1) × 2<sup>64</sup> | 192 bits | 2 <br/>
     * - @c L64X256MixRandom | LXM  | (2<sup>256</sup> - 1) × 2<sup>64</sup> | 320 bits | 4 <br/>
     * - @c Random | Legacy  | 2<sup>48</sup> | 48 bits | 0 <br/>
     * - @c SplittableRandom | Legacy  | 2<sup>64</sup> | 64 bits | 1 <br/>
     * - @c ThreadLocalRandom <sup>*</sup> | Legacy  | 2<sup>64</sup> | 64 bits | 1 <br/>
     * - @c Xoroshiro128PlusPlus | Xoroshiro  | 2<sup>128</sup> - 1 | 128 bits | 1 <br/>
     * - @c Xoshiro256PlusPlus | Xoshiro  | 2<sup>256</sup> - 1 | 256 bits | 3 <br/>
     * </p>
     * <p>
     * <sup>*</sup> ThreadLocalRandom can only be accessed via
     * @b core::concurrent::ThreadLocalRandom::current().
     * </p>
     * <h3>Categories of Random Number Generator Algorithms</h3>
     *
     * Historically, most pseudorandom generator algorithms have been based on some
     * sort of finite-state machine with a single, large cycle of states; when it is
     * necessary to have multiple threads use the same algorithm simultaneously, the
     * usual technique is to arrange for each thread to traverse a different region
     * of the state cycle. These regions may be doled out to threads by starting
     * with a single initial state and then using a "jump function" that travels a
     * long distance around the cycle (perhaps 2<sup>64</sup> steps or more); the
     * jump function is applied repeatedly and sequentially, to identify widely
     * spaced states that are then doled out, one to each thread, to serve as the
     * initial state for the generator to be used by that thread. This strategy is
     * supported by the interface @b RandomGenerator.JumpableGenerator.
     * Sometimes it is desirable to support two levels of jumping (by long distances
     * and by <i>really</i> long distances); this strategy is supported by the
     * interface @b RandomGenerator.LeapableGenerator. In this package,
     * implementations of this interface include "Xoroshiro128PlusPlus" and
     * "Xoshiro256PlusPlus". There is also an interface
     * @b RandomGenerator.ArbitrarilyJumpableGenerator for algorithms that allow
     * jumping along the state cycle by any user-specified distance; there are currently
     * no implementations of this interface in this package.
     *
     * <p>
     * A more recent category of "splittable" pseudorandom generator algorithms
     * uses a large family of state cycles and makes some attempt to ensure that
     * distinct instances use different state cycles; but even if two instances
     * "accidentally" use the same state cycle, they are highly likely to traverse
     * different regions of that shared state cycle. This strategy is
     * supported by the interface @b RandomGenerator.SplittableGenerator.
     * In this package, implementations of this interface include
     * "L32X64MixRandom",
     * "L64X128StarStarRandom",
     * "L64X128MixRandom",
     * "L64X256MixRandom",
     * "L64X1024MixRandom",
     * "L128X128MixRandom",
     * "L128X256MixRandom", and
     * "L128X1024MixRandom"; note that the class
     * @b core::util::SplittableRandom also implements this interface.
     * </p>
     *
     * <h3>The LXM Family of Random Number Generator Algorithms</h3>
     *
     * The structure of the central nextLong (or nextInt) method of an LXM
     * algorithm follows a suggestion in December 2017 by Sebastiano Vigna
     * that using one Linear Congruential Generator (LCG) as a first subgenerator
     * and one Xor-Based Generator (XBG) as a second subgenerator (rather
     * than using two LCG subgenerators) would provide a longer period, superior
     * equidistribution, scalability, and better quality.  Each of the
     * specific implementations here combines one of the best currently known
     * XBG algorithms (xoroshiro128 or xoshiro256, described by Blackman and
     * Vigna in "Scrambled Linear Pseudorandom Number Generators", <i>ACM Transactions
     * on Mathematical Software</i>, 2021) with an LCG that uses one of the best
     * currently known multipliers (found by a search for better multipliers
     * in 2019 by Steele and Vigna, described in "Computationally Easy, Spectrally
     * Good Multipliers for Congruential Pseudorandom Number Generators",
     * <i>Software: Practice and Experience</i> (2021), doi:10.1002/spe.3030),
     * and then applies either a mixing function identified by Doug Lea or
     * or a simple scrambler proposed by Blackman and Vigna. Testing has
     * confirmed that the LXM algorithm is far superior in quality to the
     * SplitMix algorithm (2014) used by @c SplittableRandom
     * (see Steele and Vigna, "LXM: Better Splittable Pseudorandom Number
     * Generators (and Almost as Fast)", <i>Proc. 2021 ACM OOPSLA Conference</i>).
     *
     * Each class with a name of the form
     * @c L<i>p</i>@c X<i>q</i>@c SomethingRandom
     * uses some specific member of the LXM family of random number
     * algorithms; "LXM" is short for "LCG, XBG, Mixer". Every LXM
     * generator has two subgenerators; one is an LCG (Linear Congruential
     * Generator) and the other is an XBG (Xor-Based Generator). Each output of an LXM
     * generator is the result of combining state from the LCG with state from the
     * XBG using a Mixing function (and then the state of the LCG
     * and the state of the XBG are advanced).
     *
     * <p>
     * The LCG subgenerator has an update step of the form @c s = m*s + a,
     * where @c s, @c m, and @c a are all binary integers of the same
     * size, each having <i>p</i> bits; @c s is the mutable state, the
     * multiplier @c m is fixed (the same for all instances of a class) and the
     * addend @c a is a parameter (a final field of the instance). The
     * parameter @c a is required to be odd (this allows the LCG to have the
     * maximal period, namely 2<sup><i>p</i></sup>); therefore there are
     * 2<sup><i>p</i>&minus;1</sup> distinct choices of parameter. (When the size of
     * @c s is 128 bits, then we use the name "@c sh" below to refer to
     * the high half of @c s, that is, the high-order 64 bits of @c s.)
     * </p>
     * <p>
     * The XBG subgenerator can in principle be any one of a wide variety
     * of XBG algorithms; in this package it is always either
     * @c xoroshiro128, @c xoshiro256, or @c xoroshiro1024, in each
     * case without any final scrambler (such as "+" or "**") because LXM uses
     * a separate Mixer later in the process. The XBG state consists of
     * some fixed number of @c int or @c long fields, generally named
     * @c x0, @c x1, and so on, which can take on any values provided that
     * they are not all zero. The collective total size of these fields is <i>q</i>
     * bits; therefore the period of this subgenerator is
     * 2<sup><i>q</i></sup>&minus;1.
     * </p>
     * <p>
     * Because the periods 2<sup><i>p</i></sup> and 2<sup><i>q</i></sup>&minus;1
     * of the two subgenerators are relatively prime, the <em>period</em> of any
     * single instance of an LXM algorithm (the length of the series of generated
     * values before it repeats) is the product of the periods of the subgenerators,
     * that is, 2<sup><i>p</i></sup>(2<sup><i>q</i></sup>&minus;1), which is just
     * slightly smaller than 2<sup>(<i>p</i>+<i>q</i>)</sup>. Moreover, if two
     * distinct instances of the same LXM algorithm have different @c a
     * parameters, then their cycles of produced values will be different.
     * </p>
     * <p>
     * Generally speaking, among the "@c L<i>p</i>@c X<i>q</i>"
     * generators, the memory required for an instance is 2<i>p</i>+<i>q</i> bits.
     * (If <i>q</i> is 1024 or larger, the XBG state is represented as an
     * array, so additional bits are needed for the array object header, and another
     * 32 bits are used for an array index.)
     * </p>
     * <p>
     * Larger values of <i>p</i> imply a lower probability that two distinct
     * instances will traverse the same state cycle, and larger values of <i>q</i>
     * imply that the generator is equidistributed in a larger number of dimensions
     * (this is provably true when <i>p</i> is 64, and conjectured to be
     * approximately true when <i>p</i> is 128). A class with "@c Mix" in its
     * name uses a fairly strong mixing function with excellent avalanche
     * characteristics; a class with "@c StarStar" in its name uses a weaker
     * but faster mixing function.
     * </p>
     * <p>
     * The specific LXM algorithms used in this package are all chosen so that
     * the 64-bit values produced by the @b nextLong()
     * method are exactly equidistributed (for example, for any specific instance of
     * "L64X128MixRandom", over the course of its cycle each of the
     * 2<sup>64</sup> possible @c long values will be produced
     * 2<sup>128</sup>&minus;1 times). The values produced by the
     * @b nextInt(), @b nextFloat(), and @b nextDouble() methods are likewise exactly
     * equidistributed. Some algorithms provide a further guarantee of
     * <i>k</i>-equidistribution for some <i>k</i> greater than 1, meaning that successive
     * non-overlapping <i>k</i>-tuples of 64-bit values produced by the
     * @b nextLong() method are exactly
     * equidistributed (equally likely to occur).
     * </p>
     * <p>
     * The following table gives the period, state size (in bits), parameter
     * size (in bits, including the low-order bit that is required always to be a
     * 1-bit), and equidistribution property for each of the specific LXM algorithms
     * used in this package. <br>
     *
     * - <b>Implementation</b>  | <b>Period</b>  | <b>State size</b>  | <b>Parameter size</b>  | <b>nextLong() values are</b><br/>
     * - @c "L32X64MixRandom"      | 2<sup>32</sup>(2<sup>64</sup>&minus;1)  | 96 bits | 32 bits | <br/>
     * - @c "L64X128StarStarRandom" | 2<sup>64</sup>(2<sup>128</sup>&minus;1)  | 192 bits | 64 bits | 2-equidistributed and exactly equidistributed<br/>
     * - @c "L64X128MixRandom" | 2<sup>64</sup>(2<sup>128</sup>&minus;1)  | 192 bits | 64 bits | 2-equidistributed and exactly equidistributed<br/>
     * - @c "L64X256MixRandom" | 2<sup>64</sup>(2<sup>256</sup>&minus;1)  | 320 bits | 64 bits | 4-equidistributed and exactly equidistributed<br/>
     * - @c "L64X1024MixRandom" | 2<sup>64</sup>(2<sup>1024</sup>&minus;1)  | 1088 bits | 64 bits | 16-equidistributed and exactly equidistributed<br/>
     * - @c "L128X128MixRandom" | 2<sup>128</sup>(2<sup>128</sup>&minus;1)  | 256 bits | 128 bits | exactly equidistributed<br/>
     * - @c "L128X256MixRandom" | 2<sup>128</sup>(2<sup>256</sup>&minus;1)  | 384 bits | 128 bits | exactly equidistributed<br/>
     * - @c "L128X1024MixRandom" | 2<sup>128</sup>(2<sup>1024</sup>&minus;1)  | 1152 bits | 128 bits | exactly equidistributed<br/>
     *
     * For the algorithms listed above whose names begin with @c L32, the
     * 32-bit values produced by the @b nextInt()
     * method are exactly equidistributed, but the 64-bit values produced by the
     * @b nextLong() method are not exactly equidistributed.
     * </p>
     * <p>
     * For the algorithms listed above whose names begin with @c L64 or
     * @c L128, the 64-bit values produced by the
     * @b nextLong() method are <i>exactly
     * equidistributed</i>: every instance, over the course of its cycle, will
     * produce each of the 2<sup>64</sup> possible @c long values exactly the
     * same number of times. For example, any specific instance of
     * "L64X256MixRandom", over the course of its cycle each of the
     * 2<sup>64</sup> possible @c long values will be produced
     * 2<sup>256</sup>&minus;1 times. The values produced by the
     * @b nextInt(),
     * @b nextFloat(), and
     * @b nextDouble() methods are likewise exactly
     * equidistributed.
     * </p>
     * <p>
     * In addition, for the algorithms listed above whose names begin with
     * @c L64, the 64-bit values produced by the
     * @b nextLong() method are
     * <i>k</i>-equidistributed (but not exactly <i>k</i>-equidistributed). To be
     * precise, and taking "L64X256MixRandom" as an example: for
     * any specific instance of "L64X256MixRandom", consider the
     * (overlapping) length-4 subsequences of the cycle of 64-bit values produced by
     * @b nextLong() (assuming no other methods are
     * called that would affect the state). There are
     * 2<sup>64</sup>(2<sup>256</sup>&minus;1) such subsequences, and each
     * subsequence, which consists of 4 64-bit values, can have one of
     * 2<sup>256</sup> values. Of those 2<sup>256</sup> subsequence values, nearly
     * all of them (2<sup>256</sup>&minus;2<sup>64</sup>) occur 2<sup>64</sup> times
     * over the course of the entire cycle, and the other 2<sup>64</sup> subsequence
     * values occur only 2<sup>64</sup>&minus;1 times. So the ratio of the
     * probability of getting any specific one of the less common subsequence values
     * and the probability of getting any specific one of the more common
     * subsequence values is 1&minus;2<sup>-64</sup>. (Note that the set of
     * 2<sup>64</sup> less-common subsequence values will differ from one instance
     * of "L64X256MixRandom" to another, as a function of the
     * additive parameter of the LCG.) The values produced by the
     * @b nextInt(),
     * @b nextFloat(), and
     * @b nextDouble() methods are likewise
     * 4-equidistributed (but not exactly 4-equidistributed).
     * </p>
     * <p>
     * The next table gives the LCG multiplier value, the name of the specific
     * XBG algorithm used, the specific numeric parameters for that XBG
     * algorithm, and the mixing function for each of the specific LXM algorithms
     * used in this package. (Note that the multiplier used for the 128-bit LCG
     * cases is 65 bits wide, so the constant @c 0x1d605bbb58c8abbfdL shown in
     * the table cannot actually be used in code; instead, only the 64 low-order
     * bits @c 0xd605bbb58c8abbfdL are represented in the source code, and the
     * missing 1-bit is handled through special coding of the multiply-add algorithm
     * used in the LCG.)
     * - <b>Implementation</b>  | <b>LCG multiplier</b>@c m  | <b>XBG algorithm</b> <br/>
     * - @c "L32X64MixRandom"      | @c 0xadb4a92d  | @c xoroshiro64 | <code> (26, 9, 13) </code> | mixLea32@c (s+x0)<br/>
     * - @c "L64X128StarStarRandom" | @c 0xd1342543de82ef95L  | @c xoroshiro128 | <code> (24, 16, 37) </code> | Long::rotateLeft((s+x0)* 5, 7) * 9<br/>
     * - @c "L64X128MixRandom" | @c 0xd1342543de82ef95L  | @c xoroshiro128 | <code> (24, 16, 37) </code> | mixLea64@c (s+x0)<br/>
     * - @c "L64X256MixRandom" | @c 0xd1342543de82ef95L  | @c xoshiro256 | <code> (17, 45) </code> | mixLea64@c (s+x0)<br/>
     * - @c "L64X1024MixRandom" | @c 0xd1342543de82ef95L  | @c xoroshiro1024 | <code> (25, 27, 36) </code> | mixLea64@c (s+x0)<br/>
     * - @c "L128X128MixRandom" | @c 0x1d605bbb58c8abbfdL  | @c xoroshiro128 | <code> (24, 16, 37) </code> | mixLea64@c (sh+x0)<br/>
     * - @c "L128X256MixRandom" | @c 0x1d605bbb58c8abbfdL  | @c xoshiro256 | <code> (17, 45) </code> | mixLea64@c (sh+x0)<br/>
     * - @c "L128X1024MixRandom" | @c 0x1d605bbb58c8abbfdL  | @c xoroshiro1024 | <code> (25, 27, 36) </code> | mixLea64@c (sh+x0)<br/>
     * </p>
     */
    namespace random {}
}

#endif //NAMESPACE_DOC_H
