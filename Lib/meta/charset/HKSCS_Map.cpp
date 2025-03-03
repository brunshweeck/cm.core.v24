//
// Created by brunshweeck on 8 sept. 2024.
//

#include <core/lang/StringArray.h>
#include <meta/charset/HKSCS_Map.h>
#include <meta/charset/MS950_HKSCS.h>

namespace core {
    namespace charset {

        StringArray HKSCS_Map::b2cBmpStr = {};
        StringArray HKSCS_Map::b2cSuppStr = {};
        String HKSCS_Map::pua = {};
        gint HKSCS_Map::initStatus = 123456789;

        void HKSCS_Map::loadData() {
            if (initStatus == 123456789) {
#include <meta/charset/HKSCS.map.db>
                initStatus = 123454321;
            }
            CORE_ASSERT(initStatus == 123454321);
        }

    } // charset
} // core
