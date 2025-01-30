//
// Created by brunshweeck on 6 juin 2024.
//

#include <core/concurrent/ConcurrentLinkedQueue.h>
#include <core/random/Random.h>

using namespace core;
using namespace core::util;
using namespace core::concurrent;
using namespace core::random;

int main() {
    auto queue = ConcurrentLinkedQueue<>();
    auto& rng = RandomGenerator::forName("MersenneTwister");
    for (gint i = 0; i < (1 << 15); i++) {
        Long l = rng.nextLong();
        queue.add(l);
    }
    return rng.properties().equidistribution();
}
