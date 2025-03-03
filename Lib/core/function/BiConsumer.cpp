//
// Created by brunshweeck-tazeussong on 02/02/25.
//

#include "BiConsumer.h"

#include <core/lang/Boolean.h>
#include <core/lang/Byte.h>
#include <core/lang/Short.h>
#include <core/lang/Integer.h>
#include <core/lang/Long.h>
#include <core/lang/Float.h>
#include <core/lang/Double.h>
#include <core/lang/Complex.h>
#include <core/lang/Character.h>
#include <core/lang/String.h>

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
