//
// Created by brunshweeck on 24 août 2024.
//

#include <core/atomic/AtomicLong.h>
#include <meta/random/RandomSupport.h>
#include <meta/random/Well1024a.h>

namespace core {
    namespace random {
        atomic::AtomicLong Well1024a::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        Well1024a::Well1024a() {
            setSeed(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64));
        }

        Well1024a::Well1024a(gint seed) {
            setSeed(seed);
        }

        Well1024a::Well1024a(IntArray const &seed) {
            setSeed(seed);
        }

        Well1024a::Well1024a(glong seed) {
            setSeed(seed);
        }

        gint Well1024a::nextInt() const {
            return next(32);
        }

        gbool Well1024a::nextBoolean() const {
            return next(1) != 0;
        }

        gfloat Well1024a::nextFloat() const {
            return CORE_CAST(gfloat, next(23)) * 0x1.0p-23F;
        }

        gdouble Well1024a::nextDouble() const {
            const glong high = CORE_CAST(glong, next(26)) << 26;
            const gint low = next(26);
            return CORE_CAST(gdouble, high | low) * 0x1.0p-52;
        }

        glong Well1024a::nextLong() const {
            const glong high = CORE_CAST(glong, next(32)) << 32;
            const glong low = CORE_CAST(glong, next(32)) & 0xffffffffL;
            return high | low;
        }

        gdouble Well1024a::nextGaussian() const {
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

        void Well1024a::setSeed(gint seed) {
            setSeed(IntArray::of(seed));
        }

        void Well1024a::setSeed(IntArray const &seed) {
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

        void Well1024a::setSeed(glong seed) {
            setSeed(IntArray::of(CORE_CAST(gint, (seed + 0u) >> 32), CORE_CAST(gint, seed & 0xffffffffl)));
        }

        gbool Well1024a::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties Well1024a::properties() const {
            return RandomGeneratorProperties("Well1024a"_S, "WELL"_S, 512, 1, 0);
        }

        gint Well1024a::next(gint bits) const {

            const gint indexRm1 = iRm1[index];

            const gint v0       = v[index];
            const gint vM1      = v[i1[index]];
            const gint vM2      = v[i2[index]];
            const gint vM3      = v[i3[index]];

            const gint z0 = v[indexRm1];
            const gint z1 = v0  ^ (vM1 ^ CORE_CAST(gint, (vM1 + 0u) >> 8));
            const gint z2 = (vM2 ^ (vM2 << 19)) ^ (vM3 ^ (vM3 << 14));
            const gint z3 = z1      ^ z2;
            const gint z4 = (z0 ^ (z0 << 11)) ^ (z1 ^ (z1 << 7)) ^ (z2 ^ (z2 << 13));

            v[index]     = z3;
            v[indexRm1]  = z4;
            index        = indexRm1;

            return (bits > 32)
                       ? CORE_CAST(gint, (z4 + 0u) << bits - 32)
                       : CORE_CAST(gint, (z4 + 0u) >> 32 - bits);
        }
    } // random
} // core