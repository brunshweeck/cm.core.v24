//
// Created by admin on 13/12/24.
//

#ifndef CORE24_TEMPORALQUERY_H
#define CORE24_TEMPORALQUERY_H
#include "core/Object.h"

namespace core {
    namespace time {

        /**
         * Strategy for querying a temporal object.
         * <p>
         * Queries are a key tool for extracting information from temporal objects.
         * They exist to externalize the process of querying, permitting different
         * approaches, as per the strategy design pattern.
         * Examples might be a query that checks if the date is the day before February 29th
         * in a leap year, or calculates the number of days to your next birthday.
         * </p>
         * <p>
         * The @b TemporalField interface provides another mechanism for querying
         * temporal objects. That interface is limited to returning a @c long.
         * By contrast, queries can return any type.
         * </p>
         * <p>
         * There are two equivalent ways of using a @c TemporalQuery.
         * The first is to invoke the method on this interface directly.
         * The second is to use @b Temporal::query(TemporalQuery):
         * @code
         *   // these two lines are equivalent, but the second approach is recommended
         *   auto& t = thisQuery.queryFrom(temporal);
         *   auto& t = temporal.query(thisQuery);
         * @endcode
         * It is recommended to use the second approach, @c query(TemporalQuery),
         * as it is a lot clearer to read in code.
         * <p>
         * The most common implementations are method references, such as
         * @c LocalDate::from and @c ZoneId::from.
         * </p>
         */
        class TemporalQuery: public virtual Object {
        public:

            /**
             * Queries the specified temporal object.
             * <p>
             * This queries the specified temporal object to return an object using the logic
             * encapsulated in the implementing class.
             * Examples might be a query that checks if the date is the day before February 29th
             * in a leap year, or calculates the number of days to your next birthday.
             * </p>
             * <p>
             * There are two equivalent ways of using this method.
             * The first is to invoke this method directly.
             * The second is to use @b Temporal::query(TemporalQuery):
             * @code
             *   // these two lines are equivalent, but the second approach is recommended
             *   auto &t = thisQuery.queryFrom(temporal);
             *   auto &t = temporal.query(thisQuery);
             * @endcode
             * It is recommended to use the second approach, @c query(TemporalQuery),
             * as it is a lot clearer to read in code.
             * </p>
             * @note
             * The implementation must take the input object and query it.
             * The implementation defines the logic of the query and is responsible for
             * documenting that logic.
             * It may use any method on @c Temporal to determine the result.
             * The input object must not be altered.
             * <p>
             * The input temporal object may be in a calendar system other than ISO.
             * Implementations may choose to document compatibility with other calendar systems,
             * or reject non-ISO temporal objects by <b> querying the chronology</b>.
             * </p>
             * <p>
             * This method may be called from multiple threads in parallel.
             * It must be thread-safe when invoked.
             * </p>
             * @param temporal  the temporal object to query
             * @return the queried value, may return null to indicate not found
             * @throws DateTimeException if unable to query
             * @throws ArithmeticException if numeric overflow occurs
             */
            virtual util::Optional<> queryFrom(Temporal const& temporal) const = 0;

            /**
             * A strict query for the @c ZoneId.
             */
            static TemporalQuery const &ZONE_ID;

            /**
             * A query for the @c Chronology.
             */
            static TemporalQuery const &CHRONOLOGY;


            /**
             * A query for the smallest supported unit.
             */
            static TemporalQuery const &PRECISION;
            /**
             * A query for @c ZoneOffset returning null if not found.
             */
            static TemporalQuery const &OFFSET;

            /**
             * A lenient query for the @c ZoneId, falling back to the @c ZoneOffset.
             */
            static TemporalQuery const &ZONE;

            /**
             * A query for @c LocalDate returning null if not found.
             */
            static TemporalQuery const &LOCAL_DATE;

            /**
             * A query for @c LocalTime returning null if not found.
             */
            static TemporalQuery const &LOCAL_TIME;
        };
    } // time
} // core

#endif //CORE24_TEMPORALQUERY_H
