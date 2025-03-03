//
// Created by brunshweeck on 22/11/24.
//

#include "OptionalValue.h"

#include <core/lang/Throwable.h>

namespace core {
    namespace misc {
        gbool OptionalValue::equals(const Object &other) const {
            if (this == &other)
                return true;
            if (!Class<OptionalValue>::hasInstance(other))
                try { return getContent() == other; } catch (Throwable const &ex) { ex.throws($ftrace()); }
            OptionalValue const& otherOption = CORE_XCAST(OptionalValue const, other);
            if (!hasContent())
                return !otherOption.hasContent();
            if (!otherOption.hasContent())
                return false;
            return getContent() == otherOption.getContent();
        }

        gint OptionalValue::hash() const {
            return !hasContent() ? 0 : getContent().hash();
        }
    } // misc
} // core