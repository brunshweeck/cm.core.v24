//
// Created by brunshweeck on 13 juin 2024.
//

#include <core/UnsupportedOperationException.h>
#include <core/Void.h>

namespace core {
    Void::Void() {
        UnsupportedOperationException().throws($ftrace());
    }
} // core
