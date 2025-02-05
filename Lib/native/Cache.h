//
// Created by brunshweeck-tazeussong on 03/02/25.
//

#ifndef CACHE_H
#define CACHE_H
#include <core/Class.h>

#ifdef CORE_SYSTEM_WINDOWS
#include "../../Internal/Win32/core/misc/Cache.h"
#else
#include "../../Internal/Unix/core/misc/Cache.h"
#endif
#endif //CACHE_H
