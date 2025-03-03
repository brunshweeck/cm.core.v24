//
// Created by admin on 21/12/24.
//

#ifndef CORE24_ATOMICREFERENCE_H
#define CORE24_ATOMICREFERENCE_H

#include <core/lang/Object.h>

namespace core {
    namespace atomic {
        /**
         * An object reference that may be updated atomically.
         *
         * @tparam T The type of object referred to by this reference
         */
        template <class T>
        class AtomicReference final : public Object {
            CORE_ALIAS(VALUE, typename Class<T>::Pointer);

            VALUE value = { };

        public:
            /**
             * Creates a new AtomicReference with the given initial value.
             *
             * @param initialValue the initial value
             */
            CORE_EXPLICIT AtomicReference(T& initialValue);
        };
    } // atomic
} // core

#endif //CORE24_ATOMICREFERENCE_H
