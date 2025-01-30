//
// Created by brunshweeck on 20 août 2024.
//

#include <core/LongArray.h>
#include <core/atomic/AtomicLong.h>
#include <meta/random/L128X256MixRandom.h>
#include <meta/random/RandomSupport.h>

namespace core {
    namespace random {
        atomic::AtomicLong L128X256MixRandom::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        L128X256MixRandom::L128X256MixRandom(glong ah, glong al, glong sh, glong sl,
                                             glong x0, glong x1, glong x2, glong x3)
            : ah(ah), al(al | 1), sh(sh), sl(sl), x{x0, x1, x2, x3} {
            // If x0, x1, x2, and x3 are all zero, we must choose nonzero values.
            if ((x0 | x1 | x2 | x3) == 0) {
                glong v = sh;
                // At least one of the two values generated here will be nonzero.
                x[0] = RandomSupport::mixStafford13(v += RandomSupport::GOLDEN_RATIO_64);
                x[1] = RandomSupport::mixStafford13(v += RandomSupport::GOLDEN_RATIO_64);
                x[2] = RandomSupport::mixStafford13(v += RandomSupport::GOLDEN_RATIO_64);
                x[3] = RandomSupport::mixStafford13(v + RandomSupport::GOLDEN_RATIO_64);
            }
        }

        L128X256MixRandom::L128X256MixRandom(glong seed)
            : L128X256MixRandom(RandomSupport::mixMurmur64(seed ^= RandomSupport::SILVER_RATIO_64),
                                RandomSupport::mixMurmur64(seed += RandomSupport::GOLDEN_RATIO_64),
                                0,
                                1,
                                RandomSupport::mixStafford13(seed),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                RandomSupport::mixStafford13(seed + RandomSupport::GOLDEN_RATIO_64)) {
            // Using a value with irregularly spaced 1-bits to xor the seed
            // argument tends to improve "pedestrian" seeds such as 0 or
            // other small integers.  We may as well use SILVER_RATIO_64.
            //
            // The seed is hashed by mixMurmur64 to produce the `a` parameter.
            // The seed is hashed by mixStafford13 to produce the initial `x0`,
            // which will then be used to produce the first generated value.
            // The other x values are filled in as if by a SplitMix PRNG with
            // GOLDEN_RATIO_64 as the gamma value and mixStafford13 as the mixer.
        }

        L128X256MixRandom::L128X256MixRandom()
            : L128X256MixRandom(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64)) {
        }

        L128X256MixRandom::L128X256MixRandom(ByteArray const &seed) {
            // Convert the seed to 8 long values, of which the last 4 are not all zero.
            LongArray longs = RandomSupport::seedBytes2Longs(seed, 8, 4);
            // Force 'a' to be odd.
            ah = longs[0];
            al = longs[1] | 1;
            sh = longs[2];
            sl = longs[3];
            x[0] = longs[4];
            x[1] = longs[5];
            x[2] = longs[6];
            x[3] = longs[7];
        }

        glong L128X256MixRandom::nextLong() const {
            // Compute the result based on current state information
            // (this allows the computation to be overlapped with state update).
            glong result = RandomSupport::mixLea64(sh + x[0]);

            // Update the LCG sub-generator
            // The LCG is, in effect, s = ((1LL << 64) + ML) * s + a, if only we had 128-bit arithmetic.
            glong u = ML * sl;
            sh = (ML * sh) + Math::unsignedMultiplyHigh(ML, sl) + sl + ah;
            sl = u + al;
            if (Long::compareUnsigned(sl, u) < 0) ++sh; // Handle the carry propagation from low half to high half.

            // Update the XBG sub-generator
            glong q0 = x[0], q1 = x[1], q2 = x[2], q3 = x[3]; {
                // xoshiro256 1.0
                glong t = q1 << 17;
                q2 ^= q0;
                q3 ^= q1;
                q1 ^= q2;
                q0 ^= q3;
                q2 ^= t;
                q3 = Long::rotateLeft(q3, 45);
            }
            x[0] = q0;
            x[1] = q1;
            x[2] = q2;
            x[3] = q3;

            return result;
        }

        gbool L128X256MixRandom::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties L128X256MixRandom::properties() const {
            return RandomGeneratorProperties("L128X256MixRandom"_S, "LXM"_S, 256, 1, 128, 1);
        }

        void L128X256MixRandom::setSeed(glong seed) {
            *this = L128X256MixRandom(seed);
        }
    } // random
} // core
