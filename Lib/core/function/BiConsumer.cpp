//
// Created by brunshweeck-tazeussong on 02/02/25.
//

#include "BiConsumer.h"

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
    namespace function {
        static BiConsumer<> var = { };
        static BiConsumer<Boolean> var1 = { };
        static BiConsumer<Byte> var2 = { };
        static BiConsumer<Short> var3 = { };
        static BiConsumer<Integer> var4 = { };
        static BiConsumer<Long> var5 = { };
        static BiConsumer<Float> var6 = { };
        static BiConsumer<Double> var7 = { };
        static BiConsumer<Complex> var8 = { };
        static BiConsumer<Character> var9 = { };
        static BiConsumer<String> var10 = { };
    }
}
