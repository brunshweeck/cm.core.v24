//
// Created by brunshweeck on 18 août 2024.
//

#include "RandomGenerator.h"

#include <core/lang/ByteArray.h>
#include <core/lang/Float.h>
#include <core/lang/IllegalArgumentException.h>
#include <core/random/Random.h>
#include <core/util/Collections.h>
#include <core/util/HashMap.h>
#include <core/util/Locale.h>
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
    CORE_ALIAS(UNSAFE, misc::Unsafe);
    using namespace util;
#define CORE_INIT_RND(Rnd) { \
    Rnd& rnd = UNSAFE::newInstance<Rnd>(); \
    generators.put(rnd.properties().name(), rnd); \
    generators.put(rnd.properties().name().toLowerCase(Locale::ROOT), rnd); \
    }

    namespace random {
        static Map<String, RandomGenerator>& getGenerators() {
            static HashMap<String, RandomGenerator> generators;
            if (generators.isEmpty()) {
                CORE_INIT_RND(Random)
                CORE_INIT_RND(L32X64MixRandom)
                CORE_INIT_RND(L64X128MixRandom)
                CORE_INIT_RND(L64X256MixRandom)
                CORE_INIT_RND(L64X1024MixRandom)
                CORE_INIT_RND(L128X128MixRandom)
                CORE_INIT_RND(L128X256MixRandom)
                CORE_INIT_RND(L128X1024MixRandom)
                CORE_INIT_RND(L64X128StarStarRandom)
                CORE_INIT_RND(Xoroshiro128PlusPlus)
                CORE_INIT_RND(Xoshiro256PlusPlus)
                CORE_INIT_RND(Xoshiro256PlusPlus)
                CORE_INIT_RND(MersenneTwister)
            }

            return generators;
        }

        RandomGenerator& RandomGenerator::forName(String const& name) {
            String clazz = name;
            if (name.isEmpty())
                clazz = "Random"_Sl;
        FIRST: {
                Object& rnd = getGenerators().getOrNull(clazz);
                if (rnd != null)
                    return CORE_XCAST(RandomGenerator, rnd);
            }
            clazz = clazz.toLowerCase(Locale::ROOT);
        SECOND: {
                Object& rnd = getGenerators().getOrNull(clazz);
                if (rnd != null)
                    return CORE_XCAST(RandomGenerator, rnd);
            }
        THIRD: {
                if (clazz.equals("lxm"_Sl) || clazz.equals("l32x64mix"_Sl))
                    clazz = "l32X64MixRandom"_Sl;
                else if (clazz.equals("l64x128mix"_Sl))
                    clazz = "L64X128MixRandom"_Sl;
                else if (clazz.equals("l64x256mix"_Sl))
                    clazz = "L64X256MixRandom"_Sl;
                else if (clazz.equals("l64x1024mix"_Sl))
                    clazz = "L64X1024MixRandom"_Sl;
                else if (clazz.equals("l128x128mix"_Sl))
                    clazz = "L128X128MixRandom"_Sl;
                else if (clazz.equals("l128x256mix"_Sl))
                    clazz = "L128X256MixRandom"_Sl;
                else if (clazz.equals("l128x1024mix"_Sl))
                    clazz = "L128X1024MixRandom"_Sl;
                else if (clazz.equals("l64x128starstar"_Sl))
                    clazz = "L128X1024StarStarRandom"_Sl;
                else if (clazz.equals("mt19937"_Sl) || clazz.equals("mersenne"_Sl) ||
                    clazz.equals("mersenne-twister"_Sl))
                    clazz = "MersenneTwister"_Sl;
                Object& rnd = getGenerators().getOrNull(clazz);
                if (rnd != null)
                    return CORE_XCAST(RandomGenerator, rnd);
            }
            IllegalArgumentException("Unsupported Random Generator of name: "_S + name).throws($ftrace());
        }

        RandomGenerator& RandomGenerator::defaultGenerator() {
            return forName("L32X64MixRandom"_S);
        }

        gbool RandomGenerator::nextBoolean() const {
            return nextInt() < 0;
        }

        void RandomGenerator::nextBytes(ByteArray& bytes) const {
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
            util::HashMap<String, RandomGenerator>& generators = RandomSupport::REGISTER_GENERATORS;
            // ensure that generators is not empty
            CORE_IGNORE(forName({}));
            auto& set = util::Set<RandomGenerator>::copyOf(generators.values());
            auto arr = set.toArray();
            UNSAFE::deleteInstance(set);
            return arr;
        }
    } // random
} // core
