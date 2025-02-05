//
// Created by brunshweeck-tazeussong on 03/02/25.
//

#ifndef CORE24_MEMORY_H
#define CORE24_MEMORY_H

#include <core/Class.h>

#ifdef CORE_SYSTEM_WINDOWS
#include "../../Internal/Win32/core/misc/MemoryHeap.h"
#else
#include "../../Internal/Unix/core/misc/MemoryHeap.h"
#endif

#endif //CORE24_MEMORY_H
