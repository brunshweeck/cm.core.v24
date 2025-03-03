//
// Created by brunshweeck-tazeussong on 03/02/25.
//

#ifndef CORE24_CONSOLE_H
#define CORE24_CONSOLE_H

#include <core/lang/Class.h>

#ifdef CORE_SYSTEM_WINDOWS
#include <core/NativeConsole.h>
#else
#include <core/NativeConsole.h>
#endif

namespace core {
    CORE_ALIAS(Console, NativeConsole);
}

#endif //CORE24_CONSOLE_H
