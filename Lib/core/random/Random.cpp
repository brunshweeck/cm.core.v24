//
// Created by brunshweeck on 18 août 2024.
//

#include "Random.h"

#include <core/IllegalArgumentException.h>
#include <core/UnsupportedOperationException.h>
#include <meta/random/RandomSupport.h>

namespace core {
    namespace random {
        Random::Random() {
            RandomSupport::initialSeed();
            seed = atomic::AtomicLong((generateSeed()) ^ RandomSupport::generateSeed());
        }

        Random::Random(glong seed): seed(atomic::AtomicLong(initialScramble(seed))) {}

        class RandomWrapper final : public Random {
            RandomGenerator& generator;

        public:
            CORE_EXPLICIT RandomWrapper(RandomGenerator& generator) : Random(null), generator(generator) {}

            /**
             * setSeed does not exist in @b random::RandomGenerator so can't use it.
             */
            void setSeed(glong seed) override {
                UnsupportedOperationException().throws($ftrace());
            }

            gbool isDeprecated() const override { return generator.isDeprecated(); }

            void nextBytes(ByteArray& bytes) const override { return generator.nextBytes(bytes); }

            gint nextInt() const override { return generator.nextInt(); }

            gint nextInt(gint origin, gint bound) const override { return generator.nextInt(origin, bound); }

            gint nextInt(gint bound) const override { return generator.nextInt(bound); }

            glong nextLong() const override { return generator.nextLong(); }

            glong nextLong(glong bound) const override { return generator.nextLong(bound); }

            glong nextLong(glong origin, glong bound) const override { return generator.nextLong(origin, bound); }

            gbool nextBoolean() const override { return generator.nextBoolean(); }

            gfloat nextFloat() const override { return generator.nextFloat(); }

            gfloat nextFloat(gfloat bound) const override { return generator.nextFloat(bound); }

            gfloat nextFloat(gfloat origin, gfloat bound) const override { return generator.nextFloat(origin, bound); }

            gdouble nextDouble() const override { return generator.nextDouble(); }

            gdouble nextDouble(gdouble bound) const override { return generator.nextDouble(bound); }

            gdouble nextDouble(gdouble origin, gdouble bound) const override {
                return generator.nextDouble(origin, bound);
            }

            gdouble nextGaussian() const override { return generator.nextGaussian(); }

            gdouble nextGaussian(gdouble mean, gdouble standardDeviation) const override {
                return generator.nextGaussian(mean, standardDeviation);
            }

            gdouble nextExponential() const override { return generator.nextExponential(); }

            String toString() const override { return "RandomWrapper["_S + generator + "]"_S; }

            RandomGeneratorProperties properties() const override { return generator.properties(); }
        };

        Random& Random::from(RandomGenerator& generator) {
            if (Class<Random>::hasInstance(generator))
                return CORE_XCAST(Random, generator);
            return UNSAFE::newInstance<RandomWrapper>(generator);
        }

        void Random::setSeed(glong seed) {
            Random::seed.set(seed);
        }

        void Random::nextBytes(ByteArray& bytes) const {
            for (gint i = 0, len = bytes.length(); i < len;)
                for (gint rnd = nextInt(), n = Math::min(len - i, Integer::SIZE / Byte::SIZE); n-- > 0;
                     rnd >>= Byte::SIZE)
                    bytes[i++] = CORE_CAST(gbyte, rnd);
        }

        gint Random::nextInt() const {
            return next(32);
        }

        gint Random::nextInt(gint bound) const {
            if (bound <= 0)
                IllegalArgumentException("bound must be positive"_Sl).throws($ftrace());
            gint r = next(31);
            const gint m = bound - 1;
            if ((bound & m) == 0) // i.e., bound is a power of 2
                r = CORE_CAST(gint, (bound * CORE_CAST(glong, r)) >> 31);
            else
                // reject over-represented candidates
                for (gint u = r; u - (r = u % bound) + m < 0; u = next(31)) {}
            return r;
        }

        glong Random::nextLong() const {
            // it's okay that the bottom word remains signed.
            return (CORE_CAST(glong, next(32)) << 32) + next(32);
        }

        gbool Random::nextBoolean() const {
            return next(1) != 0;
        }

        gfloat Random::nextFloat() const {
            return CORE_CAST(gfloat, next(Float::PRECISION)) * FLOAT_UNIT;
        }

        gdouble Random::nextDouble() const {
            return next(Double::PRECISION) * DOUBLE_UNIT;
        }

        gdouble Random::nextGaussian() const {
            // See Knuth, TAOCP, Vol. 2, 3rd edition, Section 3.4.1 Algorithm C.
            if (haveNextNextGaussian) {
                haveNextNextGaussian = false;
                return nextNextGaussian;
            }
            gdouble v1 = 0.0, v2 = 0.0, s = 0.0;
            do {
                v1 = 2 * nextDouble() - 1; // between -1 and 1
                v2 = 2 * nextDouble() - 1; // between -1 and 1
                s = v1 * v1 + v2 * v2;
            } while (s >= 1 || s == 0);
            const gdouble multiplier = Math::sqrt(-2 * Math::log(s) / s);
            nextNextGaussian = v2 * multiplier;
            haveNextNextGaussian = true;
            return v1 * multiplier;
        }

        gbool Random::isDeprecated() const {
            return false;
        }

        String Random::toString() const {
            return "Random[seed="_S + seed + "]";
        }

        RandomGeneratorProperties Random::properties() const {
            return RandomGeneratorProperties("Random"_S, "Legacy"_S, 48, 0, 0, 0);
        }

        gint Random::next(gint bits) const {
            glong oldSeed = 0, nextSeed = 0;
            atomic::AtomicLong& seed = Random::seed;
            do {
                oldSeed = seed.get();
                nextSeed = (oldSeed * multiplier + addend) & mask;
            } while (!seed.compareAndSet(oldSeed, nextSeed));
            if (bits > 48)
                return CORE_CAST(gint, (nextSeed + 0ULL) << (bits - 48));
            return CORE_CAST(gint, (nextSeed + 0ULL) >> (48 - bits));
        }

        glong Random::generateSeed() {
            // L'Ecuyer, "Tables of Linear Congruential Generators of
            // Different Sizes and Good Lattice Structure", 1999
            for (;;) {
                const glong current = SEED_UNIQUIFIER.get();
                const glong next = current * 1181783497276652981L;
                if (SEED_UNIQUIFIER.compareAndSet(current, next))
                    return next;
            }
        }

        glong Random::initialScramble(glong seed) {
            return (seed ^ multiplier) & mask;
        }

        Random::Random(const misc::NIL& unused): seed(0) {
            CORE_IGNORE(unused);
        }

        atomic::AtomicLong Random::SEED_UNIQUIFIER = atomic::AtomicLong(0x1ED8B55FAC9DECL);
    } // random
} // core
