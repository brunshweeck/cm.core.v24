//
// Created by brunshweeck on 28/11/24.
//

#include "AutoSync.h"

#include <core/concurrent/Lock.h>

namespace core {
    namespace text {
        AutoSync::AutoSync(concurrent::Lock& lock): lock(lock) {
            try { relock(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void AutoSync::relock() const {
            lock.lock();
        }

        void AutoSync::unlock() const {
            lock.unlock();
        }

        AutoSync::~AutoSync() {
            try { unlock(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // icu
} // core
