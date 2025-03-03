//
// Created by brunshweeck on 8 sept. 2024.
//

#include <meta/charset/HKSCS_XPMap.h>
#include <meta/charset/MS950_HKSCS_XP.h>

namespace core {
    namespace charset {
        StringArray HKSCS_XPMap::b2cBmpStr = {};
        StringArray HKSCS_XPMap::b2cSuppStr = {};
        String HKSCS_XPMap::pua = {};
        gint HKSCS_XPMap::initStatus = 123456789;

        void HKSCS_XPMap::loadData() {
            if (initStatus == 123456789) {
#include <meta/charset/HKSCSxp.map.db>
                initStatus = 123454321;
            }
            CORE_ASSERT(initStatus == 123454321);
        }
    } // charset
} // core
