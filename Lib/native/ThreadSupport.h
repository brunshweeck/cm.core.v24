//
// Created by brunshweeck-tazeussong on 03/02/25.
//

#ifndef CORE24_THREADSUPPORT_H
#define CORE24_THREADSUPPORT_H


#include <core/Class.h>

#ifdef CORE_SYSTEM_WINDOWS
#include "../../Internal/Win32/core/ThreadSupport.h"
#else
#include "../../Internal/Unix/core/ThreadSupport.h"
#endif


#endif //CORE24_THREADSUPPORT_H
