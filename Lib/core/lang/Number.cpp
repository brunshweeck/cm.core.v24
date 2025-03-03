//
// Created by bruns on 09/05/2024.
//

#include "Number.h"

namespace core
{
    gbyte Number::byteValue() const
    {
        return CORE_FCAST(gbyte, intValue());
    }

    gshort Number::shortValue() const
    {
        return CORE_FCAST(gshort, intValue());
    }
} // core
