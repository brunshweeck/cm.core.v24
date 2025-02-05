//
// Created by brunshweeck-tazeussong on 03/02/25.
//

#ifndef CORE24_EVENT_H
#define CORE24_EVENT_H

#include <core/Class.h>

#ifdef CORE_SYSTEM_WINDOWS
#include "../../Internal/Win32/core/misc/Event.h"
#else
#include "../../Internal/Unix/core/misc/Event.h"
#endif

#endif //CORE24_EVENT_H
