//
// Created by bruns on 30/09/2024.
//

#ifndef CORE24_OWNABLESYNCHRONIZER_H
#define CORE24_OWNABLESYNCHRONIZER_H

#include <core/Thread.h>

namespace core {
    namespace concurrent {
        /**
         * A synchronizer that may be exclusively owned by a thread.  This
         * class provides a basis for creating concurrent and related synchronizers
         * that may entail a notion of ownership.  The
         * @c OwnableSynchronizer class itself does not manage or
         * use this information. However, subclasses and tools may use
         * appropriately maintained values to help control and monitor access
         * and provide diagnostics.
         */
        class OwnableSynchronizer : public virtual Object {
            CORE_ALIAS(THREAD, Class<Thread>::Pointer);

            /**
             * The current owner of exclusive mode synchronization.
             */
            THREAD volatile exclOwnerThread = {};

        protected:
            /**
             * Empty constructor for use by subclasses.
             */
            CORE_IMPLICIT OwnableSynchronizer() = default;

            /**
             * Sets the thread that currently owns exclusive access.
             * A @c null argument indicates that no thread owns access.
             * This method does not otherwise impose any synchronization or
             * @c volatile field accesses.
             * @param thread the owner thread
             */
            virtual void setExclusiveOwnerThread(Object &thread);

            /**
             * Returns the thread last set by @c setExclusiveOwnerThread,
             * or @c null if never set.  This method does not otherwise
             * impose any synchronization or @c volatile field accesses.
             * @return the owner thread
             */
            virtual Object &exclusiveOwnerThread() const;
        };
    } // concurrent
} // core

#endif //CORE24_OWNABLESYNCHRONIZER_H
