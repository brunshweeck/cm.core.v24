//
// Created by brunshweeck on 20 août 2024.
//

#include <core/lang/LongArray.h>
#include <core/atomic/AtomicLong.h>
#include <meta/random/RandomSupport.h>
#include <meta/random/Xoshiro256PlusPlus.h>

namespace core {
    namespace random {
        atomic::AtomicLong Xoshiro256PlusPlus::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        Xoshiro256PlusPlus::Xoshiro256PlusPlus(glong x0, glong x1, glong x2, glong x3) : x{x0, x1, x2, x3} {
            // If x0, x1, x2 and x3 are both zero, we must choose nonzero values.
            if ((x0 | x1 | x2 | x3) == 0) {
                // At least three of the four values generated here will be nonzero.
                x[0] = RandomSupport::mixStafford13(x0 += RandomSupport::GOLDEN_RATIO_64);
                x[1] = x0 += RandomSupport::GOLDEN_RATIO_64;
                x[2] = x0 += RandomSupport::GOLDEN_RATIO_64;
                x[3] = x0 += RandomSupport::GOLDEN_RATIO_64;
            }
        }

        Xoshiro256PlusPlus::Xoshiro256PlusPlus(glong seed)
            : Xoshiro256PlusPlus(RandomSupport::mixStafford13(seed ^= RandomSupport::SILVER_RATIO_64),
                                 RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                 RandomSupport::mixStafford13(seed += RandomSupport::GOLDEN_RATIO_64),
                                 RandomSupport::mixStafford13(seed + RandomSupport::GOLDEN_RATIO_64)) {
            // Using a value with irregularly spaced 1-bits to xor the seed
            // argument tends to improve "pedestrian" seeds such as 0 or
            // other small integers.  We may as well use SILVER_RATIO_64.
            //
            // The x values are then filled in as if by a SplitMix PRNG with
            // GOLDEN_RATIO_64 as the gamma value and Stafford13 as the mixer.
        }

        Xoshiro256PlusPlus::Xoshiro256PlusPlus()
            : Xoshiro256PlusPlus(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64)) {
        }

        Xoshiro256PlusPlus::Xoshiro256PlusPlus(ByteArray const &seed) {
            // Convert the seed to 4 long values, of which the last 2 are not all zero.
            LongArray longs = RandomSupport::seedBytes2Longs(seed, 4, 2);
            x[0] = longs[0];
            x[1] = longs[1];
            x[2] = longs[2];
            x[3] = longs[3];
        }

        glong Xoshiro256PlusPlus::nextLong() const {
            // Compute the result based on current state information
            // (this allows the computation to be overlapped with state update).
            glong result = Long::rotateLeft(x[0] + x[3], 23) + x[0]; // "plusplus" scrambler

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

        gbool Xoshiro256PlusPlus::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties Xoshiro256PlusPlus::properties() const {
            return RandomGeneratorProperties("Xoshiro256PlusPlus"_S, "Xoshiro"_S, 256, 1, 0, 3);
        }

        void Xoshiro256PlusPlus::setSeed(glong seed) {
            *this = Xoshiro256PlusPlus(seed);
        }
    } // random
} // core
