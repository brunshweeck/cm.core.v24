//
// Created by brunshweeck on 20 août 2024.
//

#include <core/atomic/AtomicLong.h>
#include <meta/random/L32X64MixRandom.h>
#include <meta/random/RandomSupport.h>

namespace core {
    namespace random {

        atomic::AtomicLong L32X64MixRandom::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        L32X64MixRandom::L32X64MixRandom(gint a, gint s, gint x0, gint x1) : a(a | 1), s(s), x{x0, x1} {
            // If x0 and x1 are both zero, we must choose nonzero values.
            if ((x0 | x1) == 0) {
                gint v = s;
                // At least one of the two values generated here will be nonzero.
                x[0] = RandomSupport::mixMurmur32(v += RandomSupport::GOLDEN_RATIO_32);
                x[1] = RandomSupport::mixMurmur32(v + RandomSupport::GOLDEN_RATIO_32);
            }
        }

        L32X64MixRandom::L32X64MixRandom(glong seed)
            : L32X64MixRandom(CORE_CAST(gint, (seed ^= RandomSupport::SILVER_RATIO_64) >> 32),
                              1,
                              RandomSupport::mixLea32(CORE_CAST(gint, seed)),
                              RandomSupport::mixLea32(CORE_CAST(gint, seed) + RandomSupport::GOLDEN_RATIO_32)) {
            // Using a value with irregularly spaced 1-bits to xor the seed
            // argument tends to improve "pedestrian" seeds such as 0 or
            // other small integers.  We may as well use SILVER_RATIO_64.
            //
            // The high half of the seed is hashed by mixMurmur32 to produce the `a` parameter.
            // The low half of the seed is hashed by mixLea32 to produce the initial `x0`,
            // which will then be used to produce the first generated value.
            // Then x1 is filled in as if by a SplitMix PRNG with
            // GOLDEN_RATIO_32 as the gamma value and mixLea32 as the mixer.
        }

        L32X64MixRandom::L32X64MixRandom()
            : L32X64MixRandom(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64)) {
            // Using GOLDEN_RATIO_64 here gives us a good Weyl sequence of values.
        }

        L32X64MixRandom::L32X64MixRandom(ByteArray const &seed) {
            // Convert the seed to 4 int values, of which the last 2 are not all zero.
            IntArray ints = RandomSupport::seedBytes2Ints(seed, 4, 2);
            // Force 'a' to be odd.
            a = ints[0] | 1;
            s = ints[1];
            x[0] = ints[2];
            x[1] = ints[3];
        }

        gint L32X64MixRandom::nextInt() const {
            // Compute the result based on current state information
            // (this allows the computation to be overlapped with state update).
            const gint result = RandomSupport::mixLea32(s + x[0]);

            // Update the LCG sub-generator
            s = M * s + a;

            // Update the XBG sub-generator
            int q0 = x[0], q1 = x[1]; {
                // Xoroshiro64
                q1 ^= q0;
                q0 = Integer::rotateLeft(q0, 26);
                q0 = q0 ^ q1 ^ (q1 << 9);
                q1 = Integer::rotateLeft(q1, 13);
            }
            x[0] = q0;
            x[1] = q1;

            return result;
        }

        glong L32X64MixRandom::nextLong() const {
            return (CORE_CAST(glong, nextInt()) << 32) ^ CORE_CAST(glong, nextInt());
        }

        gbool L32X64MixRandom::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties L32X64MixRandom::properties() const {
            return RandomGeneratorProperties("L32X64MixRandom"_S, "LXM"_S, 64, 1, 32, 1);
        }

        void L32X64MixRandom::setSeed(glong seed) {
            *this = L32X64MixRandom(seed);
        }
    } // random
} // core
