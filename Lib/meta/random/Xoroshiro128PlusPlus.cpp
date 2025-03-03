//
// Created by brunshweeck on 20 août 2024.
//

#include <core/lang/LongArray.h>
#include <core/atomic/AtomicLong.h>
#include <meta/random/RandomSupport.h>
#include <meta/random/Xoroshiro128PlusPlus.h>

namespace core {
    namespace random {
        atomic::AtomicLong Xoroshiro128PlusPlus::DEFAULT_GENERATOR = atomic::AtomicLong(RandomSupport::initialSeed());

        Xoroshiro128PlusPlus::Xoroshiro128PlusPlus(glong x0, glong x1) : x{x0, x1} {
            // If x0 and x1 are both zero, we must choose nonzero values.
            if ((x0 | x1) == 0) {
                x[0] = RandomSupport::GOLDEN_RATIO_64;
                x[1] = RandomSupport::SILVER_RATIO_64;
            }
        }

        Xoroshiro128PlusPlus::Xoroshiro128PlusPlus(glong seed)
            : Xoroshiro128PlusPlus(RandomSupport::mixStafford13(seed ^= RandomSupport::SILVER_RATIO_64),
                                   RandomSupport::mixStafford13(seed + RandomSupport::GOLDEN_RATIO_64)) {
            // Using a value with irregularly spaced 1-bits to xor the seed
            // argument tends to improve "pedestrian" seeds such as 0 or
            // other small integers.  We may as well use SILVER_RATIO_64.
            //
            // The x values are then filled in as if by a SplitMix PRNG with
            // GOLDEN_RATIO_64 as the gamma value and Stafford13 as the mixer.
        }

        Xoroshiro128PlusPlus::Xoroshiro128PlusPlus()
            : Xoroshiro128PlusPlus(DEFAULT_GENERATOR.getAndAdd(RandomSupport::GOLDEN_RATIO_64)) {
        }

        Xoroshiro128PlusPlus::Xoroshiro128PlusPlus(ByteArray const &seed) {
            // Convert the seed to 4 long values, of which the last 2 are not all zero.
            LongArray longs = RandomSupport::seedBytes2Longs(seed, 4, 2);
            x[0] = longs[0];
            x[1] = longs[1];
        }

        glong Xoroshiro128PlusPlus::nextLong() const {
            glong s0 = x[0];
            glong s1 = x[1];
            // Compute the result based on current state information
            // (this allows the computation to be overlapped with state update).
            glong result = Long::rotateLeft(s0 + s1, 17) + s0; // "plusplus" scrambler

            s1 ^= s0;
            x[0] = Long::rotateLeft(s0, 49) ^ s1 ^ (s1 << 21); // a, b
            x[1] = Long::rotateLeft(s1, 28); // c

            return result;
        }

        gbool Xoroshiro128PlusPlus::isDeprecated() const {
            return false;
        }

        RandomGeneratorProperties Xoroshiro128PlusPlus::properties() const {
            return RandomGeneratorProperties("Xoroshiro128PlusPlus"_S, "Xoroshiro"_S, 128, 1, 0, 1);
        }

        void Xoroshiro128PlusPlus::setSeed(glong seed) {
            *this = Xoroshiro128PlusPlus(seed);
        }
    } // random
} // core
