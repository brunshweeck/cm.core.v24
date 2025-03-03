//
// Created by brunshweeck on 13 juin 2024.
//

#include "Void.h"

#include <core/lang/UnsupportedOperationException.h>

namespace core {
    Void::Void() {
        UnsupportedOperationException().throws($ftrace());
    }
} // core
