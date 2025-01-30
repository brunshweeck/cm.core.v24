//
// Created by admin on 25/01/25.
//

#include "ConcurrentLinkedQueue.h"

#include <core/Boolean.h>
#include <core/Byte.h>
#include <core/Short.h>
#include <core/Integer.h>
#include <core/Long.h>
#include <core/Float.h>
#include <core/Double.h>
#include <core/Complex.h>
#include <core/Character.h>
#include <core/String.h>

namespace core {
    namespace concurrent {
        static ConcurrentLinkedQueue<> var = { };
        static ConcurrentLinkedQueue<Boolean> var1 = { };
        static ConcurrentLinkedQueue<Byte> var2 = { };
        static ConcurrentLinkedQueue<Short> var3 = { };
        static ConcurrentLinkedQueue<Integer> var4 = { };
        static ConcurrentLinkedQueue<Long> var5 = { };
        static ConcurrentLinkedQueue<Float> var6 = { };
        static ConcurrentLinkedQueue<Double> var7 = { };
        static ConcurrentLinkedQueue<Complex> var8 = { };
        static ConcurrentLinkedQueue<Character> var9 = { };
        static ConcurrentLinkedQueue<String> var10 = { };
    } // concurrent
} // core
