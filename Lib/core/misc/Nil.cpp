//
// Created by bruns on 09/05/2024.
//

#include <core/misc/Nil.h>

namespace core {
    namespace misc {
        NIL NIL::INSTANCE = {};
    } // misc

    using namespace misc;

    NIL& null = NIL::INSTANCE;

} // core
