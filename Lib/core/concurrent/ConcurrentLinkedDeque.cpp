//
// Created by admin on 26/01/25.
//

#include "ConcurrentLinkedDeque.h"

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
        static ConcurrentLinkedDeque<> var = { };
        static ConcurrentLinkedDeque<Boolean> var1 = { };
        static ConcurrentLinkedDeque<Byte> var2 = { };
        static ConcurrentLinkedDeque<Short> var3 = { };
        static ConcurrentLinkedDeque<Integer> var4 = { };
        static ConcurrentLinkedDeque<Long> var5 = { };
        static ConcurrentLinkedDeque<Float> var6 = { };
        static ConcurrentLinkedDeque<Double> var7 = { };
        static ConcurrentLinkedDeque<Complex> var8 = { };
        static ConcurrentLinkedDeque<Character> var9 = { };
        static ConcurrentLinkedDeque<String> var10 = { };
    } // concurrent
} // core
