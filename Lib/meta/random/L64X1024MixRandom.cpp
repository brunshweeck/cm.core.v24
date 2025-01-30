//
// Created by brunshweeck on 20 août 2024.
//

#include <core/LongArray.h>
#include <core/atomic/AtomicLong.h>
#include <meta/random/L64X1024MixRandom.h>
#include <meta/random/RandomSupport.h>

namespace core {
    namespace random {
        atomic::AtomicLong L64X1024MixRandom::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        L64X1024MixRandom::L64X1024MixRandom(glong a, glong s,
                                             glong x0, glong x1, glong x2, glong x3,
                                             glong x4, glong x5, glong x6, glong x7,
                                             glong x8, glong x9, glong x10, glong x11,
                                             glong x12, glong x13, glong x14, glong x15)
            : a(a | 1), s(s), x{x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15} {
            // If x0, x1, ..., x15 are all zero (very unlikely), we must choose nonzero values.
            if ((x0 | x1 | x2 | x3 | x4 | x5 | x6 | x7 | x8 | x9 | x10 | x11 | x12 | x13 | x14 | x15) == 0) {
                glong v = s;
                // At least fifteen of the sixteen values generated here will be nonzero.
                for (gint i = 0; i < N; i += 1)
                    x[i] = RandomSupport::mixStafford13(v += RandomSupport::GOLDEN_RATIO_64);
            }
        }

        L64X1024MixRandom::L64X1024MixRandom(glong seed)
            : L64X1024MixRandom(RandomSupport::mixMurmur64(seed ^= RandomSupport::SILVER_RATIO_64),
                                1,
                                RandomSupport::mixStafford13(seed),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed + RandomSupport::GOLDEN_RATIO_64)) {
            // Using a value with irregularly spaced 1-bits to xor the seed
            // argument tends to improve "pedestrian" seeds such as 0 or
            // other small integers.  We may as well use SILVER_RATIO_64.
            //
            // The seed is hashed by mixMurmur64 to produce the `a` parameter.
            // The seed is hashed by mixStafford13 to produce the initial `x[0]`,
            // which will then be used to produce the first generated value.
            // The other x values are filled in as if by a SplitMix PRNG with
            // GOLDEN_RATIO_64 as the gamma value and mixStafford13 as the mixer.
        }

        L64X1024MixRandom::L64X1024MixRandom()
            : L64X1024MixRandom(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64)) {
        }

        L64X1024MixRandom::L64X1024MixRandom(ByteArray const &seed) {
            // Convert the seed to 18 long values, of which the last 16 are not all zero.
            LongArray longs = RandomSupport::seedBytes2Longs(seed, 18, 16);
            // Force 'a' to be odd.
            a = longs[0] | 1;
            s = longs[1];
            for (int i = 0; i < N; i += 1)
                x[i] = longs[2 + i];
        }

        glong L64X1024MixRandom::nextLong() const {
            // First part of xoroshiro1024: fetch array data
            gint q = p;
            glong s0 = x[p = (p + 1) & (N - 1)];
            glong s15 = x[q];

            // Compute the result based on current state information
            // (this allows the computation to be overlapped with state update).

            glong result = RandomSupport::mixLea64(s + s0);

            // Update the LCG sub-generator
            s = M * s + a; // LCG

            // Second part of xoroshiro1024: update array data
            s15 ^= s0;
            x[q] = Long::rotateLeft(s0, 25) ^ s15 ^ (s15 << 27);
            x[p] = Long::rotateLeft(s15, 36);

            return result;
        }

        gbool L64X1024MixRandom::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties L64X1024MixRandom::properties() const {
            return RandomGeneratorProperties("L64X1024MixRandom"_S, "LXM"_S, 1024, 1, 64, 16);
        }

        void L64X1024MixRandom::setSeed(glong seed) {
            *this = L64X1024MixRandom(seed);
        }
    } // random
} // core
