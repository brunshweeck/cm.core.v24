//
// Created by brunshweeck on 24 août 2024.
//

#include <core/atomic/AtomicLong.h>
#include <meta/random/RandomSupport.h>
#include <meta/random/Well44497a.h>

namespace core {
    namespace random {
        atomic::AtomicLong Well44497a::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        Well44497a::Well44497a() {
            setSeed(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64));
        }

        Well44497a::Well44497a(gint seed) {
            setSeed(seed);
        }

        Well44497a::Well44497a(IntArray const &seed) {
            setSeed(seed);
        }

        Well44497a::Well44497a(glong seed) {
            setSeed(seed);
        }

        gint Well44497a::nextInt() const {
            return next(32);
        }

        gbool Well44497a::nextBoolean() const {
            return next(1) != 0;
        }

        gfloat Well44497a::nextFloat() const {
            return CORE_CAST(gfloat, next(23)) * 0x1.0p-23F;
        }

        gdouble Well44497a::nextDouble() const {
            const glong high = CORE_CAST(glong, next(26)) << 26;
            const gint low = next(26);
            return CORE_CAST(gdouble, high | low) * 0x1.0p-52;
        }

        glong Well44497a::nextLong() const {
            const glong high = CORE_CAST(glong, next(32)) << 32;
            const glong low = CORE_CAST(glong, next(32)) & 0xffffffffL;
            return high | low;
        }

        gdouble Well44497a::nextGaussian() const {
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

        void Well44497a::setSeed(gint seed) {
            setSeed(IntArray::of(seed));
        }

        void Well44497a::setSeed(IntArray const &seed) {
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

        void Well44497a::setSeed(glong seed) {
            setSeed(IntArray::of(CORE_CAST(gint, (seed + 0u) >> 32), CORE_CAST(gint, seed & 0xffffffffl)));
        }

        gbool Well44497a::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties Well44497a::properties() const {
            return RandomGeneratorProperties("Well44497a"_S, "WELL"_S, 512, 1, 0);
        }

        gint Well44497a::next(gint bits) const {

            const gint indexRm1 = iRm1[index];
            const gint indexRm2 = iRm2[index];

            const gint v0       = v[index];
            const gint vM1      = v[i1[index]];
            const gint vM2      = v[i2[index]];
            const gint vM3      = v[i3[index]];

            // the values below include the errata of the original article
            const gint z0       = (0xFFFF8000 & v[indexRm1]) ^ (0x00007FFF & v[indexRm2]);
            const gint z1       = (v0 ^ (v0 << 24))  ^ (vM1 ^ ((vM1 + 0u) >> 30));
            const gint z2       = (vM2 ^ (vM2 << 10)) ^ (vM3 << 26);
            const gint z3       = z1      ^ z2;
            const gint z2Prime  = ((z2 << 9) ^ ((z2 + 0u) >> 23)) & 0xfbffffff;
            const gint z2Second = ((z2 & 0x00020000) != 0) ? (z2Prime ^ 0xb729fcec) : z2Prime;
            const gint z4       = z0 ^ (z1 ^ ((z1 + 0u) >> 20)) ^ z2Second ^ z3;

            v[index]     = z3;
            v[indexRm1]  = z4;
            v[indexRm2] &= 0xFFFF8000;
            index        = indexRm1;

            return (bits > 32)
                       ? CORE_CAST(gint, (z4 + 0u) << bits - 32)
                       : CORE_CAST(gint, (z4 + 0u) >> 32 - bits);
        }
    } // random
} // core