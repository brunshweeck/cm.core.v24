//
// Created by brunshweeck on 18 août 2024.
//

#include <core/ByteArray.h>
#include <core/Float.h>
#include <core/IllegalArgumentException.h>
#include <core/random/RandomGenerator.h>
#include <core/util/HashMap.h>
#include <core/random/Random.h>
#include <meta/random/L128X1024MixRandom.h>
#include <meta/random/L128X128MixRandom.h>
#include <meta/random/L128X256MixRandom.h>
#include <meta/random/L32X64MixRandom.h>
#include <meta/random/L64X1024MixRandom.h>
#include <meta/random/L64X128MixRandom.h>
#include <meta/random/L64X128StarStarRandom.h>
#include <meta/random/L64X256MixRandom.h>
#include <meta/random/MersenneTwister.h>
#include <meta/random/RandomSupport.h>
#include <meta/random/Xoroshiro128PlusPlus.h>
#include <meta/random/Xoshiro256PlusPlus.h>

namespace core {
    namespace random {
        RandomGenerator &RandomGenerator::forName(String const &name) {
            util::HashMap<String, RandomGenerator> &generators = RandomSupport::REGISTER_GENERATORS;
            if (generators.isEmpty()) {
                RandomGenerator const &rng0 = UNSAFE::newInstance<Random>();
                RandomGenerator const &rng1 = UNSAFE::newInstance<L32X64MixRandom>();
                RandomGenerator const &rng2 = UNSAFE::newInstance<L64X128MixRandom>();
                RandomGenerator const &rng3 = UNSAFE::newInstance<L64X256MixRandom>();
                RandomGenerator const &rng4 = UNSAFE::newInstance<L64X1024MixRandom>();
                RandomGenerator const &rng5 = UNSAFE::newInstance<L128X128MixRandom>();
                RandomGenerator const &rng6 = UNSAFE::newInstance<L128X256MixRandom>();
                RandomGenerator const &rng7 = UNSAFE::newInstance<L128X1024MixRandom>();
                RandomGenerator const &rng8 = UNSAFE::newInstance<L64X128StarStarRandom>();
                RandomGenerator const &rng9 = UNSAFE::newInstance<Xoroshiro128PlusPlus>();
                RandomGenerator const &rng10 = UNSAFE::newInstance<Xoshiro256PlusPlus>();
                RandomGenerator const &rng11 = UNSAFE::newInstance<MersenneTwister>();
                generators.put("random"_S, rng0);
                generators.put(""_S, rng0);
                generators.put("lcg"_S, rng0);
                generators.put("l32x64mixrandom"_S, rng1);
                generators.put("l32x64mix"_S, rng1);
                generators.put("lxm"_S, rng1);
                generators.put("l64x128mixrandom"_S, rng2);
                generators.put("l64x128mix"_S, rng2);
                generators.put("l64x256mixrandom"_S, rng3);
                generators.put("l64x256mix"_S, rng3);
                generators.put("l64x1024mixrandom"_S, rng4);
                generators.put("l64x1024mix"_S, rng4);
                generators.put("l128x128mixrandom"_S, rng5);
                generators.put("l128x128mix"_S, rng5);
                generators.put("l128x256mixrandom"_S, rng6);
                generators.put("l128x256mix"_S, rng6);
                generators.put("l128x1024mixrandom"_S, rng7);
                generators.put("l128x1024mix"_S, rng7);
                generators.put("l64x128starstarrandom"_S, rng8);
                generators.put("l64x128starstar"_S, rng8);
                generators.put("l64x128**"_S, rng8);
                generators.put("xoroshiro128plusplus"_S, rng9);
                generators.put("xoroshiro128++"_S, rng9);
                generators.put("xoshiro256plusplus"_S, rng10);
                generators.put("xoshiro256++"_S, rng10);
                generators.put("mt19937"_S, rng11);
                generators.put("mersennetwister"_S, rng11);
                generators.put("mersenne-twister"_S, rng11);
                generators.put("mersenne twister"_S, rng11);
            }
            String const algorithm = name.toLowerCase();
            Object &generator = generators.getOrNull(algorithm);
            if (generator != null)
                return CORE_XCAST(RandomGenerator, generator);
            IllegalArgumentException("Generator of name `"_S + name + "` not found")
                    .throws($ftrace());
        }

        RandomGenerator &RandomGenerator::defaultGenerator() {
            return forName("L32X64MixRandom"_S);
        }

        gbool RandomGenerator::nextBoolean() const {
            return nextInt() < 0;
        }

        void RandomGenerator::nextBytes(ByteArray &bytes) const {
            gint i = 0;
            gint len = bytes.length();
            for (gint words = len >> 3; words-- > 0;) {
                glong rnd = nextLong();
                for (gint n = 8; n-- > 0; rnd = CORE_CAST(glong, (rnd + 0ULL) >> Byte::SIZE))
                    bytes[i++] = CORE_CAST(gbyte, rnd);
            }
            if (i < len)
                for (glong rnd = nextLong(); i < len; rnd = CORE_CAST(glong, (rnd + 0ULL) >> Byte::SIZE))
                    bytes[i++] = CORE_CAST(gbyte, rnd);
        }

        gfloat RandomGenerator::nextFloat() const {
            return CORE_CAST(gint, (nextInt() + 0U) >> (Float::SIZE - Float::PRECISION)) * 0x1.0p-24f;
        }

        gfloat RandomGenerator::nextFloat(gfloat bound) const {
            RandomSupport::checkBound(bound);
            return RandomSupport::boundedNextFloat(*this, bound);
        }

        gfloat RandomGenerator::nextFloat(gfloat origin, gfloat bound) const {
            RandomSupport::checkRange(origin, bound);
            return RandomSupport::boundedNextFloat(*this, origin, bound);
        }

        gdouble RandomGenerator::nextDouble() const {
            return CORE_CAST(glong, (nextLong() + 0ULL) >> (Double::SIZE - Double::PRECISION)) * 0x1.0p-53;
        }

        gdouble RandomGenerator::nextDouble(gdouble bound) const {
            RandomSupport::checkBound(bound);
            return RandomSupport::boundedNextDouble(*this, bound);
        }

        gdouble RandomGenerator::nextDouble(gdouble origin, gdouble bound) const {
            RandomSupport::checkRange(origin, bound);
            return RandomSupport::boundedNextDouble(*this, origin, bound);
        }

        gint RandomGenerator::nextInt() const {
            return CORE_CAST(gint, (nextLong() + 0ULL) >> 32);
        }

        gint RandomGenerator::nextInt(gint bound) const {
            RandomSupport::checkBound(bound);
            return RandomSupport::boundedNextInt(*this, bound);
        }

        gint RandomGenerator::nextInt(gint origin, gint bound) const {
            RandomSupport::checkRange(origin, bound);
            return RandomSupport::boundedNextInt(*this, origin, bound);
        }

        glong RandomGenerator::nextLong(glong bound) const {
            RandomSupport::checkBound(bound);
            return RandomSupport::boundedNextLong(*this, bound);
        }

        glong RandomGenerator::nextLong(glong origin, glong bound) const {
            RandomSupport::checkBound(bound);
            return RandomSupport::boundedNextLong(*this, origin, bound);
        }

        gdouble RandomGenerator::nextGaussian() const {
            // See Knuth, TAOCP, Vol. 2, 3rd edition, Section 3.4.1 Algorithm C.
            return RandomSupport::nextGaussian(*this);
        }

        gdouble RandomGenerator::nextGaussian(gdouble mean, gdouble standardDeviation) const {
            if (standardDeviation < 0.0)
                IllegalArgumentException("standard deviation must be non-negative"_S)
                        .throws($ftrace());
            return mean + standardDeviation * RandomSupport::nextGaussian(*this);
        }

        gdouble RandomGenerator::nextExponential() const {
            return RandomSupport::nextExponential(*this);
        }

        Array<RandomGenerator> RandomGenerator::availableGenerators() {
            util::HashMap<String, RandomGenerator> &generators = RandomSupport::REGISTER_GENERATORS;
            // ensure that generators is not empty
            CORE_IGNORE(forName({}));
            auto& set = util::Set<RandomGenerator>::copyOf(generators.values());
            auto arr = set.toArray();
            UNSAFE::deleteInstance(set);
            return arr;
        }

    } // random
} // core
