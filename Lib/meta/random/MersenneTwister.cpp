//
// Created by brunshweeck on 22 août 2024.
//

#include <core/atomic/AtomicLong.h>
#include <meta/random/MersenneTwister.h>
#include <meta/random/RandomSupport.h>

namespace core {
    namespace random {
        atomic::AtomicLong MersenneTwister::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        MersenneTwister::MersenneTwister()
            : MersenneTwister(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64)) {
        }

        MersenneTwister::MersenneTwister(gint seed) {
            setSeed(seed);
        }

        MersenneTwister::MersenneTwister(glong seed) {
            setSeed(seed);
        }

        MersenneTwister::MersenneTwister(IntArray const &seed) {
            setSeed(seed);
        }

        void MersenneTwister::setSeed(gint seed) {
            // we use a long masked by 0xffffffffL as a poor man unsigned int
            glong longMT = seed;
            // NB: unlike original C code, we are working with java longs, the cast below makes masking unnecessary
            mt[0] = CORE_CAST(gint, longMT);
            for (mti = 1; mti < N; ++mti) {
                // See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier.
                // initializer from the 2002-01-09 C version by Makoto Matsumoto
                longMT = (1812433253L * (longMT ^ (longMT >> 30)) + mti) & 0xffffffffL;
                mt[mti] = CORE_CAST(gint, longMT);
            }

            nxtGaussian = Double::NaN;
        }

        void MersenneTwister::setSeed(IntArray const &seed) {
            if (seed.isEmpty()) {
                const glong newSeed = DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64);
                setSeed(newSeed);
                return;
            }

            setSeed(19650218);
            gint i = 1;
            gint j = 0;

            for (gint k = Math::max(N, seed.length()); k != 0; k--) {
                glong l0 = (mt[i] & 0x7fffffffL) | ((mt[i] < 0) ? 0x80000000L : 0x0L);
                glong l1 = (mt[i - 1] & 0x7fffffffL) | ((mt[i - 1] < 0) ? 0x80000000L : 0x0L);
                glong l = (l0 ^ ((l1 ^ (l1 >> 30)) * 1664525l)) + seed[j] + j; // non linear
                mt[i] = CORE_CAST(gint, l & 0xffffffffL);
                i++;
                j++;
                if (i >= N) {
                    mt[0] = mt[N - 1];
                    i = 1;
                }
                if (j >= seed.length()) {
                    j = 0;
                }
            }

            for (gint k = N - 1; k != 0; k--) {
                glong l0 = (mt[i] & 0x7fffffffL) | ((mt[i] < 0) ? 0x80000000L : 0x0L);
                glong l1 = (mt[i - 1] & 0x7fffffffL) | ((mt[i - 1] < 0) ? 0x80000000L : 0x0L);
                glong l = (l0 ^ ((l1 ^ (l1 >> 30)) * 1566083941l)) - i; // non linear
                mt[i] = CORE_CAST(gint, l & 0xffffffffL);
                i++;
                if (i >= N) {
                    mt[0] = mt[N - 1];
                    i = 1;
                }
            }

            mt[0] = CORE_CAST(gint, 0x80000000); // MSB is 1; assuring non-zero initial array

            nxtGaussian = Double::NaN;
        }

        void MersenneTwister::setSeed(glong seed) {
            setSeed(IntArray::of(CORE_CAST(gint, seed >> 32), CORE_CAST(gint, seed & 0xffffffffL)));
        }

        gbool MersenneTwister::nextBoolean() const {
            return next(1) != 0;
        }

        gint MersenneTwister::nextInt() const {
            return next(32);
        }

        gfloat MersenneTwister::nextFloat() const {
            return CORE_CAST(gfloat, next(23)) * 0x1.0p-23F;
        }

        glong MersenneTwister::nextLong() const {
            const glong high = CORE_CAST(glong, next(32)) << 32;
            const glong low = CORE_CAST(glong, next(32)) & 0xffffffffL;
            return high | low;
        }

        gdouble MersenneTwister::nextDouble() const {
            const glong high = CORE_CAST(glong, next(26)) << 26;
            const gint low = next(26);
            return CORE_CAST(gdouble, high | low) * 0x1.0p-52;
        }

        gdouble MersenneTwister::nextGaussian() const {
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

        gbool MersenneTwister::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties MersenneTwister::properties() const {
            return RandomGeneratorProperties("MersenneTwister"_S, "MT19937"_S, 19937, 0, 0, 1);
        }

        gint MersenneTwister::next(gint bits) const {
            gint y = 0;

            if (mti >= N) {
                // generate N words at one time
                gint mtNext = mt[0];
                for (int k = 0; k < N - M; ++k) {
                    gint mtCurr = mtNext;
                    mtNext = mt[k + 1];
                    y = CORE_CAST(gint, (mtCurr & 0x80000000) | (mtNext & 0x7fffffff));
                    mt[k] = CORE_CAST(gint, mt[k + M] ^ ((y + 0u) >> 1) ^ (y & 0x1 ? MAG1: MAG0));
                }
                for (int k = N - M; k < N - 1; ++k) {
                    int mtCurr = mtNext;
                    mtNext = mt[k + 1];
                    y = CORE_CAST(gint, (mtCurr & 0x80000000) | (mtNext & 0x7fffffff));
                    mt[k] = CORE_CAST(gint, mt[k + (M - N)] ^ ((y + 0u) >> 1) ^ (y & 0x1 ? MAG1: MAG0));
                }
                y = CORE_CAST(gint, (mtNext & 0x80000000) | (mt[0] & 0x7fffffff));
                mt[N - 1] = CORE_CAST(gint, mt[M - 1] ^ ((y + 0u) >> 1) ^ (y & 0x1 ? MAG1: MAG0));

                mti = 0;
            }

            y = mt[mti++];

            // tempering
            y ^= CORE_CAST(gint, (y + 0u) >> 11);
            y ^= CORE_CAST(gint, (y << 7) & 0x9d2c5680);
            y ^= CORE_CAST(gint, (y << 15) & 0xefc60000);
            y ^= CORE_CAST(gint, (y + 0u) >> 18);

            return (bits > 32)
                       ? CORE_CAST(gint, (y + 0u) << bits - 32)
                       : CORE_CAST(gint, (y + 0u) >> 32 - bits);
        }
    } // random
} // core
