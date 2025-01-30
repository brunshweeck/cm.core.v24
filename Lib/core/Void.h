//
// Created by brunshweeck on 13 juin 2024.
//

#ifndef CORE24_VOID_H
#define CORE24_VOID_H

#include <core/Object.h>

namespace core
{
    /**
     * The @c Class object representing the pseudo-type corresponding to
     * the keyword @c void.
     */
    class Void final : public Object
    {

        /*
         * The Void class cannot be instantiated.
         */
        CORE_EXPLICIT Void();
    };
} // core

#endif //CORE24_VOID_H
