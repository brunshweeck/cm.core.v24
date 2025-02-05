//
// Created by brunshweeck-tazeussong on 03/02/25.
//

#ifndef CORE24_VARIABLE_H
#define CORE24_VARIABLE_H

#include <core/Class.h>

#ifdef CORE_SYSTEM_WINDOWS
#include "../../Internal/Win32/core/misc/VarHandle.h"
#else
#include "../../Internal/Unix/core/misc/VarHandle.h"
#endif

#endif //CORE24_VARIABLE_H
