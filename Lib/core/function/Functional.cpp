//
// Created by brunshweeck on 06/11/24.
//

#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        CORE_ALIAS(UNSAFE, misc::Unsafe);

        gint Functional::SharedHandle::state() const {
            return UNSAFE::getIntVolatile(null, CORE_CAST(glong, &counter));
        }

        gbool Functional::SharedHandle::acquire() {
            return UNSAFE::getAndAddIntAcquire(null, CORE_CAST(glong, &counter), -1) <= 0;
        }

        gint Functional::SharedHandle::release() {
            return UNSAFE::getAndAddIntRelease(null, CORE_CAST(glong, &counter), +1);
        }
    }
}
