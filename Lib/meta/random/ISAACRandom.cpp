//
// Created by brunshweeck on 24 août 2024.
//

#include <core/atomic/AtomicLong.h>
#include <meta/random/ISAACRandom.h>

namespace core {
    namespace random {
        atomic::AtomicLong ISAACRandom::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        ISAACRandom::ISAACRandom() {
            setSeed(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64));
        }

        ISAACRandom::ISAACRandom(glong seed) {
            setSeed(seed);
        }

        ISAACRandom::ISAACRandom(IntArray const &seed) {
            setSeed(seed);
        }

        void ISAACRandom::setSeed(IntArray const &seed) {
            if (seed.isEmpty()) {
                setSeed(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64));
                return;
            }
            gint seedLen = seed.length();
            gint rslLen = SIZE;
            for (gint i = 0, to = Math::min(seedLen, rslLen); i < to; i++)
                rsl[i] = seed[i];
            if (seedLen < rslLen) {
                for (gint j = seedLen; j < rslLen; j++) {
                    glong k = rsl[j - seedLen];
                    rsl[j] = CORE_CAST(gint, 0x6c078965L * (k ^ k >> 30) + j & 0xffffffffL);
                }
            }
            initState();
        }

        void ISAACRandom::setSeed(glong seed) {
            setSeed(IntArray::of(CORE_CAST(gint, seed+0uLL >> 32), CORE_CAST(gint, seed & 0xffffffffL)));
        }

        void ISAACRandom::setSeed(gint seed) {
            setSeed(IntArray::of(seed));
        }

        gbool ISAACRandom::isDeprecated() const {
            return false;
        }

        gint ISAACRandom::nextInt() const {
            return next(32);
        }

        glong ISAACRandom::nextLong() const {
            const glong high = CORE_CAST(glong, next(32)) << 32;
            const glong low = CORE_CAST(glong, next(32)) & 0xffffffffL;
            return high | low;
        }

        gbool ISAACRandom::nextBoolean() const {
            return next(1) != 0;
        }

        gfloat ISAACRandom::nextFloat() const {
            return CORE_CAST(gfloat, next(23)) * 0x1.0p-23F;
        }

        gdouble ISAACRandom::nextDouble() const {
            const glong high = CORE_CAST(glong, next(26)) << 26;
            const gint low = next(26);
            return CORE_CAST(gdouble, high | low) * 0x1.0p-52;
        }

        gdouble ISAACRandom::nextGaussian() const {
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

        RandomGeneratorProperties ISAACRandom::properties() const {
            return RandomGeneratorProperties("ISSACRandom"_S, "ISAAC"_S, 8295, 0, 0, 1);
        }

        gint ISAACRandom::next(gint bits) const {
            if (count < 0) {
                isaac();
                count = SIZE - 1;
            }
            return (bits > 32)
                       ? CORE_CAST(gint, (rsl[count--] + 0u) << bits - 32)
                       : CORE_CAST(gint, (rsl[count--] + 0u) >> 32 - bits);
        }

        void ISAACRandom::isaac() const {
            isaacI = 0;
            isaacJ = HALF_SIZE;
            isaacB += ++isaacC;
            while (isaacI < HALF_SIZE) {
                isaac2();
            }
            isaacJ = 0;
            while (isaacJ < HALF_SIZE) {
                isaac2();
            }
        }

        void ISAACRandom::isaac2() const {
            isaacX = mem[isaacI];
            isaacA ^= isaacA << 13;
            isaacA += mem[isaacJ++];
            isaac3();
            isaacX = mem[isaacI];
            isaacA ^= CORE_CAST(gint, isaacA + 0u) >> 6;
            isaacA += mem[isaacJ++];
            isaac3();
            isaacX = mem[isaacI];
            isaacA ^= isaacA << 2;
            isaacA += mem[isaacJ++];
            isaac3();
            isaacX = mem[isaacI];
            isaacA ^= CORE_CAST(gint, isaacA + 0u) >> 16;
            isaacA += mem[isaacJ++];
            isaac3();
        }

        void ISAACRandom::isaac3() const {
            mem[isaacI] = mem[(isaacX & MASK) >> 2] + isaacA + isaacB;
            isaacB = mem[(mem[isaacI] >> SIZE_LOG & MASK) >> 2] + isaacX;
            rsl[isaacI++] = isaacB;
        }

        void ISAACRandom::initState() const {
            isaacA = 0;
            isaacB = 0;
            isaacC = 0;
            const gint arrLen = 8;
            for (int j = 0; j < arrLen; j++) {
                arr[j] = GOLDEN_RATIO;
            }
            for (int j = 0; j < 4; j++) {
                shuffle();
            }
            // fill in mem[] with messy stuff
            for (int j = 0; j < SIZE; j += 8) {
                arr[0] += rsl[j];
                arr[1] += rsl[j + 1];
                arr[2] += rsl[j + 2];
                arr[3] += rsl[j + 3];
                arr[4] += rsl[j + 4];
                arr[5] += rsl[j + 5];
                arr[6] += rsl[j + 6];
                arr[7] += rsl[j + 7];
                shuffle();
                setState(j);
            }
            // second pass makes all seed affect all of mem
            for (int j = 0; j < SIZE; j += 8) {
                arr[0] += mem[j];
                arr[1] += mem[j + 1];
                arr[2] += mem[j + 2];
                arr[3] += mem[j + 3];
                arr[4] += mem[j + 4];
                arr[5] += mem[j + 5];
                arr[6] += mem[j + 6];
                arr[7] += mem[j + 7];
                shuffle();
                setState(j);
            }
            isaac();
            count = SIZE - 1;
            nxtGaussian = Double::NaN;
        }

        void ISAACRandom::shuffle() const {
            arr[0] ^= arr[1] << 11;
            arr[3] += arr[0];
            arr[1] += arr[2];
            arr[1] ^= CORE_CAST(gint, (arr[2] + 0u) >> 2);
            arr[4] += arr[1];
            arr[2] += arr[3];
            arr[2] ^= arr[3] << 8;
            arr[5] += arr[2];
            arr[3] += arr[4];
            arr[3] ^= CORE_CAST(gint, (arr[4] + 0u) >> 16);
            arr[6] += arr[3];
            arr[4] += arr[5];
            arr[4] ^= arr[5] << 10;
            arr[7] += arr[4];
            arr[5] += arr[6];
            arr[5] ^= CORE_CAST(gint, (arr[6] + 0u) >> 4);
            arr[0] += arr[5];
            arr[6] += arr[7];
            arr[6] ^= arr[7] << 8;
            arr[1] += arr[6];
            arr[7] += arr[0];
            arr[7] ^= CORE_CAST(gint, (arr[0] + 0u) >> 9);
            arr[2] += arr[7];
            arr[0] += arr[1];
        }

        void ISAACRandom::setState(gint start) const {
            mem[start] = arr[0];
            mem[start + 1] = arr[1];
            mem[start + 2] = arr[2];
            mem[start + 3] = arr[3];
            mem[start + 4] = arr[4];
            mem[start + 5] = arr[5];
            mem[start + 6] = arr[6];
            mem[start + 7] = arr[7];
        }
    } // random
} // core
