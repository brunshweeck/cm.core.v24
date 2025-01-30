//
// Created by bruns on 30/09/2024.
//

#include <core/AssertionError.h>
#include <core/Thread.h>
#include <core/concurrent/OwnableSynchronizer.h>

namespace core {
    namespace concurrent {
        void OwnableSynchronizer::setExclusiveOwnerThread(Object &thread) {
            if (thread == null) {
                exclOwnerThread = null;
                return;
            }

            CORE_ASSERT(Class<Thread>::hasInstance(thread));
            Thread &t = CORE_XCAST(Thread, thread);
            exclOwnerThread = &t;
        }

        Object & OwnableSynchronizer::exclusiveOwnerThread() const {
            if (exclOwnerThread == null)
                return null;

            return *exclOwnerThread;
        }
    } // concurrent
} // core
