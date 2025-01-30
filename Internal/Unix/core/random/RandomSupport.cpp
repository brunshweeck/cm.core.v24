//
// Created by admin on 18/12/24.
//

#include <meta/random/RandomSupport.h>
#include <core/Unix.h>

namespace core {
    namespace random {
        glong RandomSupport::generateSeed() {
            return Seed();
        }
    }
}
