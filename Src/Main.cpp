//
// Created by brunshweeck on 6 juin 2024.
//

#include <core/concurrent/ConcurrentLinkedQueue.h>
#include <core/random/Random.h>

using namespace core;
using namespace core::util;
using namespace core::concurrent;
using namespace core::random;

#define CORE_LAMBDA_TRY_RETHROW(action, executionPoint) \
    [&]() { try { action; } catch(Throwable const& ex) { ex.throws(executionPoint); } catch(...) { throw; } }()

#define CORE_TRY_(action) CORE_LAMBDA_TRY_RETHROW(action, $ftrace())

int main() {
    CORE_TRY_RETHROW((IllegalArgumentException().throws($ftrace())))
    return 0;
}
