//
// Created by brunshweeck on 24 août 2024.
//

#include <core/atomic/AtomicLong.h>
#include <meta/random/RandomSupport.h>
#include <meta/random/Well19937a.h>

namespace core {
    namespace random {
        atomic::AtomicLong Well19937a::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        Well19937a::Well19937a() {
            setSeed(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64));
        }

        Well19937a::Well19937a(gint seed) {
            setSeed(seed);
        }

        Well19937a::Well19937a(IntArray const &seed) {
            setSeed(seed);
        }

        Well19937a::Well19937a(glong seed) {
            setSeed(seed);
        }

        gint Well19937a::nextInt() const {
            return next(32);
        }

        gbool Well19937a::nextBoolean() const {
            return next(1) != 0;
        }

        gfloat Well19937a::nextFloat() const {
            return CORE_CAST(gfloat, next(23)) * 0x1.0p-23F;
        }

        gdouble Well19937a::nextDouble() const {
            const glong high = CORE_CAST(glong, next(26)) << 26;
            const gint low = next(26);
            return CORE_CAST(gdouble, high | low) * 0x1.0p-52;
        }

        glong Well19937a::nextLong() const {
            const glong high = CORE_CAST(glong, next(32)) << 32;
            const glong low = CORE_CAST(glong, next(32)) & 0xffffffffL;
            return high | low;
        }

        gdouble Well19937a::nextGaussian() const {
            gdouble random = nxtGaussian;
            if (Double::isNaN(random)) {
                // generate a new pair of gaussian numbers
                const gdouble x = nextDouble();
                const gdouble y = nextDouble();
                const gdouble alpha = 2 * Math::PI * x;
                const gdouble r = Math::sqrt(-2 * Math::log(y));
                random = r * Math::cos(alpha);
                nxtGaussian = r * Math::sin(alpha);
            } else {
                // use the second element of the pair already generated
                nxtGaussian = Double::NaN;
            }

            return random;
        }

        void Well19937a::setSeed(gint seed) {
            setSeed(IntArray::of(seed));
        }

        void Well19937a::setSeed(IntArray const &seed) {
            if (seed.isEmpty()) {
                setSeed(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64));
                return;
            }

            const gint seedLen = seed.length();
            for (int i = 0, to = Math::min(seedLen, R); i < to; ++i)
                v[i] = seed[i];

            if (seedLen < R) {
                for (int i = seedLen; i < R; ++i) {
                    glong l = v[i - seedLen];
                    v[i] = CORE_CAST(gint, (1812433253l * (l ^ (l >> 30)) + i) & 0xffffffffL);
                }
            }

            index = 0;
            nxtGaussian = Double::NaN; // Clear normal deviate cache
        }

        void Well19937a::setSeed(glong seed) {
            setSeed(IntArray::of(CORE_CAST(gint, (seed + 0u) >> 32), CORE_CAST(gint, seed & 0xffffffffl)));
        }

        gbool Well19937a::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties Well19937a::properties() const {
            return RandomGeneratorProperties("Well19937a"_S, "WELL"_S, 512, 1, 0);
        }

        gint Well19937a::next(gint bits) const {

            const gint indexRm1 = iRm1[index];
            const gint indexRm2 = iRm2[index];

            const gint v0       = v[index];
            const gint vM1      = v[i1[index]];
            const gint vM2      = v[i2[index]];
            const gint vM3      = v[i3[index]];

            const gint z0 = CORE_CAST(gint, 0x80000000 & v[indexRm1]) ^ (0x7FFFFFFF & v[indexRm2]);
            const gint z1 = (v0 ^ (v0 << 25))  ^ (vM1 ^ CORE_CAST(gint, (vM1+0u) >> 27));
            const gint z2 = CORE_CAST(gint, (vM2+0u) >> 9) ^ (vM3 ^ CORE_CAST(gint, (vM3+0u) >> 1));
            const gint z3 = z1      ^ z2;
            const gint z4 = z0 ^ (z1 ^ (z1 << 9)) ^ (z2 ^ (z2 << 21)) ^ (z3 ^ CORE_CAST(gint, (z3+0u) >> 21));

            v[index]     = z3;
            v[indexRm1]  = z4;
            v[indexRm2] &= CORE_CAST(gint, 0x80000000);
            index        = indexRm1;

            return (bits > 32)
                       ? CORE_CAST(gint, (z4 + 0u) << bits - 32)
                       : CORE_CAST(gint, (z4 + 0u) >> 32 - bits);
        }
    } // random
} // core