//
// Created by bruns on 03/10/2024.
//

#ifndef CORE24_INTERRUPTIBLE_H
#define CORE24_INTERRUPTIBLE_H

#include <core/lang/Object.h>

namespace core {
    namespace io {
        class Interruptible : public virtual Object {
        public:
            virtual void interrupt(Thread &t) = 0;
        };
    } // io
} // core

#endif //CORE24_INTERRUPTIBLE_H
