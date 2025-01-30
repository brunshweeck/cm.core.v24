//
// Created by brunshweeck on 11 sept. 2024.
//

#ifndef CORE24_LOCALEISODATA_H
#define CORE24_LOCALEISODATA_H

#include <core/util/Locale.h>

namespace core {
    namespace util {
        class LocaleISOData final : public Object {
        public:
            /**
             * The 2- and 3-letter ISO 639 language codes.
             */
            static String const ISO639Languages;

            /**
             * The 2- and 3-letter ISO 3166 country codes.
             */
            static String const ISO3166Countries;

            /**
             * Array to hold country codes for ISO3166-3.
             */
            static String const ISO3166Regions;

            static String const NumericalCounties;

            static String const ISOScripts;
        };
    } // util
} // core

#endif //CORE24_LOCALEISODATA_H
