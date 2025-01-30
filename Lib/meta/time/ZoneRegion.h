//
// Created by admin on 29/12/24.
//

#ifndef CORE24_ZONEREGION_H
#define CORE24_ZONEREGION_H

#include <core/time/ZoneOffset.h>

namespace core {
    namespace time {

        /**
         * A geographical region where the same time-zone rules apply.
         * <p>
         * Time-zone information is categorized as a set of rules defining when and
         * how the offset from UTC/Greenwich changes. These rules are accessed using
         * identifiers based on geographical regions, such as countries or states.
         * The most common region classification is the Time Zone Database (TZDB),
         * which defines regions such as 'Europe/Paris' and 'Asia/Tokyo'.
         * </p>
         * <p>
         * The region identifier, modeled by this class, is distinct from the
         * underlying rules, modeled by @b ZoneRules.
         * The rules are defined by governments and change frequently.
         * By contrast, the region identifier is well-defined and long-lived.
         * This separation also allows rules to be shared between regions if appropriate.
         * </p>
         */
        class ZoneRegion final: public ZoneId {
            CORE_ALIAS(Rules, Class<ZoneRules>::Pointer);
            /**
             * The time-zone ID.
             */
            String id;
            /**
             * The time-zone rules, null if zone ID was loaded leniently.
             */
            Rules rules = null;

        public:
            /**
             * Obtains an instance of @c ZoneRegion from an identifier.
             *
             * @param zoneId  the time-zone ID
             * @param checkAvailable  whether to check if the zone ID is available
             * @return the zone ID
             * @throws DateTimeException if the ID format is invalid
             * @throws ZoneRulesException if checking availability and the ID cannot be found
             */
            static ZoneRegion ofID(String const &zoneId, gbool checkAvailable);

            /**
             * Constructor.
             *
             * @param id  the time-zone ID
             */
            CORE_EXPLICIT ZoneRegion(String const &id);

            /**
             * Constructor.
             *
             * @param id  the time-zone ID
             * @param rules  the rules
             */
            CORE_EXPLICIT ZoneRegion(String const &id, ZoneRules const &rules);

            String getId() const override;

            ZoneRules getRules() const override;

            Object& clone() const override;

        private:
            ZoneOffset getOffset(glong epochSecond) const override;

            /**
             * Checks that the given string is a legal ZondId name.
             *
             * @param zoneId  the time-zone ID
             * @throws DateTimeException if the ID format is invalid
             */
            static void checkName(const String& zoneId);
        };
    } // time
} // core

#endif //CORE24_ZONEREGION_H
