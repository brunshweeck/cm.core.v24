//
// Created by admin on 29/12/24.
//

#ifndef CORE24_ZONERULES_H
#define CORE24_ZONERULES_H

#include <core/time/ZoneOffset.h>
#include <core/time/ZoneOffsetTransitionRule.h>
#include <core/time/LocalDateTime.h>
#include <core/Array.h>
#include <core/LongArray.h>

namespace core {
    namespace time {
        /**
         * The rules defining how the zone offset varies for a single time-zone.
         * <p>
         * The rules model all the historic and future transitions for a time-zone.
         * @b ZoneOffsetTransition is used for known transitions, typically historic.
         * @b ZoneOffsetTransitionRule is used for future transitions that are based
         * on the result of an algorithm.
         * </p>
         * <p>
         * Serializing an instance of @c ZoneRules will store the entire set of rules.
         * It does not store the zone ID as it is not part of the state of this object.
         * </p>
         * <p>
         * A rule implementation may or may not store full information about historic
         * and future transitions, and the information stored is only as accurate as
         * that supplied to the implementation by the rules provider.
         * Applications should treat the data provided as representing the best information
         * available to the implementation of this rule.
         * </p>
         */
        class ZoneRules final : public Object {
            template <class K, class V>
            CORE_ALIAS(ConcurrentMap, typename Class<util::Map<K, Array<V>>>::Pointer);
            CORE_ADD_AS_FRIEND(ZoneId);
            CORE_ADD_AS_FRIEND(ZoneOffset);

            /**
             * The transitions between standard offsets (epoch seconds), sorted.
             */
            LongArray standardTransitions;
            /**
             * The standard offsets.
             */
            Array<ZoneOffset> standardOffsets;
            /**
             * The transitions between instants (epoch seconds), sorted.
             */
            LongArray savingsInstantTransitions;
            /**
             * The transitions between local date-times, sorted.
             * This is a paired array, where the first entry is the start of the transition
             * and the second entry is the end of the transition.
             */
            Array<LocalDateTime> savingsLocalTransitions;
            /**
             * The wall offsets.
             */
            Array<ZoneOffset> wallOffsets;
            /**
             * The last rule.
             */
            Array<ZoneOffsetTransitionRule> lastRules;
            /**
             * The map of recent transitions.
             */
            ConcurrentMap<Integer, ZoneOffsetTransition> lastRulesCache;

            static CORE_FAST gint LAST_CACHED_YEAR = 2100;

        public:
            /**
             * Obtains an instance of a ZoneRules.
             *
             * @param baseStandardOffset  the standard offset to use before legal rules were set
             * @param baseWallOffset  the wall offset to use before legal rules were set
             * @param standardOffsetTransitionList  the list of changes to the standard offset
             * @param transitionList  the list of transitions
             * @param lastRules  the recurring last rules, size 16 or less
             * @return the zone rules
             */
            CORE_EXPLICIT ZoneRules(ZoneOffset const& baseStandardOffset,
                                    ZoneOffset const& baseWallOffset,
                                    util::List<ZoneOffsetTransition> const& standardOffsetTransitionList,
                                    util::List<ZoneOffsetTransition> const& transitionList,
                                    util::List<ZoneOffsetTransitionRule> const& lastRules);

            /**
             * Obtains an instance of ZoneRules that has fixed zone rules.
             *
             * @param offset  the offset this fixed zone rules is based on
             * @return the zone rules
             * @see #isFixedOffset()
             */
            CORE_EXPLICIT ZoneRules(ZoneOffset const& offset);

            /**
             * Checks of the zone rules are fixed, such that the offset never varies.
             *
             * @return true if the time-zone is fixed and the offset never changes
             */
            gbool isFixedOffset() const;

            /**
             * Gets a suitable offset for the specified local date-time in these rules.
             * <p>
             * The mapping from a local date-time to an offset is not straightforward.
             * There are three cases:
             *
             * @li Normal, with one valid offset. For the vast majority of the year, the normal
             *  case applies, where there is a single valid offset for the local date-time.</li>
             * @li Gap, with zero valid offsets. This is when clocks jump forward typically
             *  due to the spring daylight savings change from "winter" to "summer".
             *  In a gap there are local date-time values with no valid offset.</li>
             * @li Overlap, with two valid offsets. This is when clocks are set back typically
             *  due to the autumn daylight savings change from "summer" to "winter".
             *  In an overlap there are local date-time values with two valid offsets.</li>
             *
             * Thus, for any given local date-time there can be zero, one or two valid offsets.
             * This method returns the single offset in the Normal case, and in the Gap or Overlap
             * case it returns the offset before the transition.
             * </p>
             * <p>
             * Since, in the case of Gap and Overlap, the offset returned is a "best" value, rather
             * than the "correct" value, it should be treated with care. Applications that care
             * about the correct offset should use a combination of this method,
             * @b validOffsets(LocalDateTime) and @b transition(LocalDateTime).
             * </p>
             * @param localDT  the local date-time to query, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the best available offset for the local date-time
             */
            ZoneOffset offset(LocalDateTime const& localDT) const;

            /**
             * Gets the offset applicable at the specified local date-time in these rules.
             * <p>
             * The mapping from a local date-time to an offset is not straightforward.
             * There are three cases:
             *
             * @li Normal, with one valid offset. For the vast majority of the year, the normal
             *  case applies, where there is a single valid offset for the local date-time.</li>
             * @li Gap, with zero valid offsets. This is when clocks jump forward typically
             *  due to the spring daylight savings change from "winter" to "summer".
             *  In a gap there are local date-time values with no valid offset.</li>
             * @li Overlap, with two valid offsets. This is when clocks are set back typically
             *  due to the autumn daylight savings change from "summer" to "winter".
             *  In an overlap there are local date-time values with two valid offsets.</li>
             *
             * Thus, for any given local date-time there can be zero, one or two valid offsets.
             * This method returns that list of valid offsets, which is a list of size 0, 1 or 2.
             * In the case where there are two offsets, the earlier offset is returned at index 0
             * and the later offset at index 1.
             * </p>
             * <p>
             * There are various ways to handle the conversion from a @c LocalDateTime.
             * One technique, using this method, would be:
             * @code
             *  List<ZoneOffset>& validOffsets = rules.getValidOffsets(localDT);
             *  if (validOffsets.size() == 1) {
             *    // Normal case: only one valid offset
             *    zoneOffset = validOffsets.get(0);
             *  } else {
             *    // Gap or Overlap: determine what to do from transition (which will be non-null)
             *    ZoneOffsetTransition trans = rules.getTransition(localDT);
             *  }
             * @endcode
             * </p>
             * <p>
             * In theory, it is possible for there to be more than two valid offsets.
             * This would happen if clocks to be put back more than once in quick succession.
             * This has never happened in the history of time-zones and thus has no special handling.
             * However, if it were to happen, then the list would return more than 2 entries.
             * </p>
             * @param localDT  the local date-time to query for valid offsets
             * @return the list of valid offsets, may be immutable
             */
            util::List<ZoneOffset>& validOffsets(LocalDateTime const& localDT) const;

            /**
             * Gets the offset transition applicable at the specified local date-time in these rules.
             * <p>
             * The mapping from a local date-time to an offset is not straightforward.
             * There are three cases:
             *
             * @li Normal, with one valid offset. For the vast majority of the year, the normal
             *  case applies, where there is a single valid offset for the local date-time.</li>
             * @li Gap, with zero valid offsets. This is when clocks jump forward typically
             *  due to the spring daylight savings change from "winter" to "summer".
             *  In a gap there are local date-time values with no valid offset.</li>
             * @li Overlap, with two valid offsets. This is when clocks are set back typically
             *  due to the autumn daylight savings change from "summer" to "winter".
             *  In an overlap there are local date-time values with two valid offsets.</li>
             *
             * A transition is used to model the cases of a Gap or Overlap.
             * The Normal case will return null.
             * </p>
             * <p>
             * There are various ways to handle the conversion from a @c LocalDateTime.
             * One technique, using this method, would be:
             * @code
             *  ZoneOffsetTransition trans = rules.getTransition(localDT);
             *  if (trans != null) {
             *    // Gap or Overlap: determine what to do from transition
             *  } else {
             *    // Normal case: only one valid offset
             *    zoneOffset = rule.getOffset(localDT);
             *  }
             * @endcode
             * </p>
             * @param localDT  the local date-time to query for offset transition, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the offset transition, null if the local date-time is not in transition
             */
            util::Optional<ZoneOffsetTransition> transition(LocalDateTime const& localDT) const;

            /**
             * Gets the standard offset for the specified instant in this zone.
             * <p>
             * This provides access to historic information on how the standard offset
             * has changed over time.
             * The standard offset is the offset before any daylight saving time is applied.
             * This is typically the offset applicable during winter.
             * </p>
             * @param localDT  the instant to find the offset information for, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the standard offset
             */
            ZoneOffset standardOffset(LocalDateTime const& localDT) const;

            /**
             * Gets the amount of daylight savings in use for the specified instant in this zone.
             * <p>
             * This provides access to historic information on how the amount of daylight
             * savings has changed over time.
             * This is the difference between the standard offset and the actual offset.
             * Typically the amount is zero during winter and one hour during summer.
             * Time-zones are second-based, so the nanosecond part of the duration will be zero.
             * </p>
             * <p>
             * This default implementation calculates the duration from the
             * @b actual and @b standard offsets.
             * </p>
             * @param localDT  the instant to find the daylight savings for, but null
             *  may be ignored if the rules have a single offset for all instants
             * @return the difference between the standard and actual offset
             */
            Duration daylightSavings(LocalDateTime const& localDT) const;

            /**
             * Checks if the specified instant is in daylight savings.
             * <p>
             * This checks if the standard offset and the actual offset are the same
             * for the specified instant.
             * If they are not, it is assumed that daylight savings is in operation.
             * </p>
             * <p>
             * This default implementation compares the @b actual and @b standard offsets.
             * </p>
             * @param localDT  the instant to find the offset information for
             * @return the standard offset
             */
            gbool isDaylightSavings(LocalDateTime const &localDT) const;

            /**
             * Checks if the offset date-time is valid for these rules.
             * <p>
             * To be valid, the local date-time must not be in a gap and the offset
             * must match one of the valid offsets.
             * </p>
             * <p>
             * This default implementation checks if @b validOffsets(LocalDateTime)
             * contains the specified offset.
             * </p>
             * @param localDT  the date-time to check, but null
             *  may be ignored if the rules have a single offset for all instants
             * @param offset  the offset to check, null returns false
             * @return true if the offset date-time is valid for these rules
             */
            gbool isValidOffset(LocalDateTime const& localDT, ZoneOffset const& offset) const;

            /**
             * Gets the next transition after the specified instant.
             * <p>
             * This returns details of the next transition after the specified instant.
             * For example, if the instant represents a point where "Summer" daylight savings time
             * applies, then the method will return the transition to the next "Winter" time.
             * </p>
             * @param localDT  the instant to get the next transition after
             * @return the next transition after the specified instant, null if this is after the last transition
             */
            util::Optional<ZoneOffsetTransition> nextTransition(LocalDateTime const& localDT) const;

            /**
             * Gets the previous transition before the specified instant.
             * <p>
             * This returns details of the previous transition before the specified instant.
             * For example, if the instant represents a point where "summer" daylight saving time
             * applies, then the method will return the transition from the previous "winter" time.
             * </p>
             * @param localDT  the instant to get the previous transition after
             * @return the previous transition before the specified instant, null if this is before the first transition
             */
            util::Optional<ZoneOffsetTransition> previousTransition(LocalDateTime const& localDT) const;

            /**
             * Gets the complete list of fully defined transitions.
             * <p>
             * The complete set of transitions for this rules instance is defined by this method
             * and @b transitionRules(). This method returns those transitions that have
             * been fully defined. These are typically historical, but may be in the future.
             * </p>
             * <p>
             * The list will be empty for fixed offset rules and for any time-zone where there has
             * only ever been a single offset. The list will also be empty if the transition rules are unknown.
             * </p>
             * @return an immutable list of fully defined transitions
             */
            util::List<ZoneOffsetTransition> &transitions() const;

            /**
             * Gets the list of transition rules for years beyond those defined in the transition list.
             * <p>
             * The complete set of transitions for this rules instance is defined by this method
             * and @b transitions(). This method returns instances of @b ZoneOffsetTransitionRule
             * that define an algorithm for when transitions will occur.
             * </p>
             * <p>
             * For any given @c ZoneRules, this list contains the transition rules for years
             * beyond those years that have been fully defined. These rules typically refer to future
             * daylight saving time rule changes.
             * </p>
             * <p>
             * If the zone defines daylight savings into the future, then the list will normally
             * be of size two and hold information about entering and exiting daylight savings.
             * If the zone does not have daylight savings, or information about future changes
             * is uncertain, then the list will be empty.
             * </p>
             * <p>
             * The list will be empty for fixed offset rules and for any time-zone where there is no
             * daylight saving time. The list will also be empty if the transition rules are unknown.
             * </p>
             * @return an immutable list of transition rules
             */
            util::List<ZoneOffsetTransitionRule> &transitionRules() const;

            /**
             * Checks if this set of rules equals another.
             * <p>
             * Two rule sets are equal if they will always result in the same output
             * for any given input instant or local date-time.
             * Rules from two different groups may return false even if they are in fact the same.
             * </p>
             * <p>
             * This definition should result in implementations comparing their entire state.
             * </p>
             * @param otherRules  the other rules, null returns false
             * @return true if this rules is the same as that specified
             */
            gbool equals(Object const& otherRules) const override;

            /**
             * Returns a suitable hash code given the definition of @c equals.
             *
             * @return the hash code
             */
            gint hash() const override;

            /**
             * Returns a string describing this object.
             *
             * @return a string for debugging
             */
            String toString() const override;

        private:
            CORE_EXPLICIT ZoneRules(LongArray const& standardTransitions,
                                    Array<ZoneOffset> const& standardOffsets,
                                    LongArray const& savingsInstantTransitions,
                                    Array<ZoneOffset> const& wallOffsets,
                                    Array<ZoneOffsetTransitionRule> const& lastRules);

            Object const& offsetInfo(LocalDateTime const& localDT) const;

            static util::Optional<> findOffsetInfo(LocalDateTime const& localDT, ZoneOffsetTransition const &trans);

            Array<ZoneOffsetTransition> findTransitionArray(gint year) const;

            static gint findYear(glong epochSecond, ZoneOffset const& offset);
        };
    } // time
} // core

#endif //CORE24_ZONERULES_H
