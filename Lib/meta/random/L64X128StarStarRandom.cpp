//
// Created by brunshweeck on 20 août 2024.
//

#include <core/lang/LongArray.h>
#include <core/atomic/AtomicLong.h>
#include <meta/random/L64X128StarStarRandom.h>
#include <meta/random/RandomSupport.h>

namespace core {
    namespace random {
        atomic::AtomicLong L64X128StarStarRandom::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        L64X128StarStarRandom::L64X128StarStarRandom(glong a, glong s, glong x0, glong x1) : a(a | 1), s(s), x{x0, x1} {
            // If x0 and x1 are both zero, we must choose nonzero values.
            if ((x0 | x1) == 0) {
                glong v = s;
                // At least one of the two values generated here will be nonzero.
                x[0] = RandomSupport::mixStafford13(v += RandomSupport::GOLDEN_RATIO_64);
                x[1] = RandomSupport::mixStafford13(v + RandomSupport::GOLDEN_RATIO_64);
            }
        }

        L64X128StarStarRandom::L64X128StarStarRandom(glong seed)
            : L64X128StarStarRandom(RandomSupport::mixMurmur64(seed ^= RandomSupport::SILVER_RATIO_64),
                                    1,
                                    RandomSupport::mixStafford13(seed),
                                    RandomSupport::mixStafford13(seed + RandomSupport::GOLDEN_RATIO_64)) {
            // Using a value with irregularly spaced 1-bits to xor the seed
            // argument tends to improve "pedestrian" seeds such as 0 or
            // other small integers.  We may as well use SILVER_RATIO_64.
            //
            // The seed is hashed by mixMurmur64 to produce the `a` parameter.
            // The seed is hashed by mixStafford13 to produce the initial `x0`,
            // which will then be used to produce the first generated value.
            // Then x1 is filled in as if by a SplitMix PRNG with
            // GOLDEN_RATIO_64 as the gamma value and mixStafford13 as the mixer.
        }

        L64X128StarStarRandom::L64X128StarStarRandom()
            : L64X128StarStarRandom(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64)) {
        }

        L64X128StarStarRandom::L64X128StarStarRandom(ByteArray const &seed) {
            // Convert the seed to 4 long values, of which the last 2 are not all zero.
            LongArray longs = RandomSupport::seedBytes2Longs(seed, 4, 2);
            // Force 'a' to be odd.
            a = longs[0] | 1;
            s = longs[1];
            x[0] = longs[2];
            x[1] = longs[3];
        }

        glong L64X128StarStarRandom::nextLong() const {
            // Compute the result based on current state information
            // (this allows the computation to be overlapped with state update).
            glong result = Long::rotateLeft((s + x[0]) * 5, 7) * 9; // "starstar" scrambler

            // Update the LCG sub-generator
            s = M * s + a;

            // Update the XBG sub-generator
            glong q0 = x[0], q1 = x[1]; {
                // xoroshiro128v1_0
                q1 ^= q0;
                q0 = Long::rotateLeft(q0, 24);
                q0 = q0 ^ q1 ^ (q1 << 16);
                q1 = Long::rotateLeft(q1, 37);
            }
            x[0] = q0;
            x[1] = q1;

            return result;
        }

        gbool L64X128StarStarRandom::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties L64X128StarStarRandom::properties() const {
            return RandomGeneratorProperties("L64X128StarStarRandom"_S, "LXM"_S, 128, 1, 64, 2);
        }

        void L64X128StarStarRandom::setSeed(glong seed) {
            *this = L64X128StarStarRandom(seed);
        }
    } // random
} // core
