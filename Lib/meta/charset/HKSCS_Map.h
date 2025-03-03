//
// Created by brunshweeck on 8 sept. 2024.
//

#ifndef CORE24_HKSCS_MAP_H
#define CORE24_HKSCS_MAP_H
#include <core/lang/Object.h>

namespace core {
    namespace charset {
        class HKSCS_Map final : public Object {
        public:
            static StringArray b2cBmpStr;
            static StringArray b2cSuppStr;
            static String pua;
            static gint initStatus;

            static void loadData();
        };
    } // charset
} // core

#endif //CORE24_HKSCS_MAP_H
