//
// Created by admin on 29/12/24.
//

#ifndef CORE24_ZONEOFFSETTRANSITION_H
#define CORE24_ZONEOFFSETTRANSITION_H

#include <core/time/LocalDateTime.h>
#include <core/time/ZoneOffset.h>

namespace core {
  namespace time {
    /**
     * A transition between two offsets caused by a discontinuity in the local time-line.
     * <p>
     * A transition between two offsets is normally the result of a daylight savings cutover.
     * The discontinuity is normally a gap in spring and an overlap in autumn.
     * @c ZoneOffsetTransition models the transition between the two offsets.
     * </p>
     * <p>
     * Gaps occur where there are local date-times that simply do not exist.
     * An example would be when the offset changes from @c +03:00 to @c +04:00.
     * This might be described as 'the clocks will move forward one hour tonight at 1am'.
     * </p>
     * <p>
     * Overlaps occur where there are local date-times that exist twice.
     * An example would be when the offset changes from @c +04:00 to @c +03:00.
     * This might be described as 'the clocks will move back one hour tonight at 2am'.
     * </p>
     * @note
     * This class is immutable and thread-safe.
     */
    class ZoneOffsetTransition final : public Comparable<ZoneOffsetTransition> {
      CORE_ADD_AS_FRIEND(ZoneRules);
      CORE_ADD_AS_FRIEND(ZoneOffsetTransitionRule);
      CORE_ADD_AS_FRIEND(ZoneOffsetTransitionArray);

      /**
       * The transition epoch-second.
       */
      glong epochSecond = 0;

      /**
       * The local transition date-time at the transition.
       */
      LocalDateTime transition;

      /**
       * The offset before transition.
       */
      ZoneOffset offsetBefore_;

      /**
       * The offset after transition.
       */
      ZoneOffset offsetAfter_;

      /**
       * Creates an instance defining a transition between two offsets.
       *
       * @param transition  the transition date-time with the offset before the transition, not null
       * @param offsetBefore  the offset before the transition, not null
       * @param offsetAfter  the offset at and after the transition, not null
       */
      ZoneOffsetTransition(const LocalDateTime& transition, const ZoneOffset& offsetBefore,
                           const ZoneOffset& offsetAfter);

      /**
       * Creates an instance from epoch-second and offsets.
       *
       * @param epochSecond  the transition epoch-second
       * @param offsetBefore  the offset before the transition, not null
       * @param offsetAfter  the offset at and after the transition, not null
       */
      ZoneOffsetTransition(glong epochSecond, const ZoneOffset& offsetBefore, const ZoneOffset& offsetAfter);

    public:
      /**
       * Obtains an instance defining a transition between two offsets.
       * <p>
       * Applications should normally obtain an instance from @b ZoneRules.
       * This factory is only intended for use when creating @b ZoneRules.
       * </p>
       * @param transition  the transition date-time at the transition, which never
       *  actually occurs, expressed local to the before offset
       * @param offsetBefore  the offset before the transition
       * @param offsetAfter  the offset at and after the transition
       * @throws IllegalArgumentException if @c offsetBefore and @c offsetAfter
       *         are equal, or @c transition.nano() returns non-zero value
       */
      static ZoneOffsetTransition of(const LocalDateTime& transition, const ZoneOffset& offsetBefore,
                                     const ZoneOffset& offsetAfter);

      /**
       * Gets the transition instant.
       * <p>
       * This is the instant of the discontinuity, which is defined as the first
       * instant that the 'after' offset applies.
       * <p>
       * The methods {@link #getInstant()}, {@link #getDateTimeBefore()} and {@link #getDateTimeAfter()}
       * all represent the same instant.
       *
       * @return the transition instant, not null
       */
      Instant instant() const;

      /**
       * Gets the transition instant as an epoch second.
       *
       * @return the transition epoch second
       */
      glong toEpochSecond() const;

      /**
       * Gets the local transition date-time, as would be expressed with the 'before' offset.
       * <p>
       * This is the date-time where the discontinuity begins expressed with the 'before' offset.
       * At this instant, the 'after' offset is actually used, therefore the combination of this
       * date-time and the 'before' offset will never occur.
       * </p>
       * <p>
       * The combination of the 'before' date-time and offset represents the same instant
       * as the 'after' date-time and offset.
       * </p>
       * @return the transition date-time expressed with the before offset, not null
       */
      LocalDateTime dateTimeBefore() const;

      /**
       * Gets the local transition date-time, as would be expressed with the 'after' offset.
       * <p>
       * This is the first date-time after the discontinuity, when the new offset applies.
       * <p>
       * The combination of the 'before' date-time and offset represents the same instant
       * as the 'after' date-time and offset.
       * </p>
       * @return the transition date-time expressed with the after offset, not null
       */
      LocalDateTime dateTimeAfter() const;

      /**
       * Gets the offset before the transition.
       * <p>
       * This is the offset in use before the instant of the transition.
       * </p>
       * @return the offset before the transition, not null
       */
      ZoneOffset offsetBefore() const;

      /**
       * Gets the offset after the transition.
       * <p>
       * This is the offset in use on and after the instant of the transition.
       * </p>
       * @return the offset after the transition, not null
       */
      ZoneOffset offsetAfter() const;

      /**
       * Gets the duration of the transition.
       * <p>
       * In most cases, the transition duration is one hour, however this is not always the case.
       * The duration will be positive for a gap and negative for an overlap.
       * Time-zones are second-based, so the nanosecond part of the duration will be zero.
       * </p>
       * @return the duration of the transition, positive for gaps, negative for overlaps
       */
      Duration duration() const;

      /**
       * Does this transition represent a gap in the local time-line.
       * <p>
       * Gaps occur where there are local date-times that simply do not exist.
       * An example would be when the offset changes from @c +01:00 to @c +02:00.
       * This might be described as 'the clocks will move forward one hour tonight at 1am'.
       * </p>
       * @return true if this transition is a gap, false if it is an overlap
       */
      gbool isGap() const;

      /**
       * Does this transition represent an overlap in the local time-line.
       * <p>
       * Overlaps occur where there are local date-times that exist twice.
       * An example would be when the offset changes from @c +02:00 to @c +01:00.
       * This might be described as 'the clocks will move back one hour tonight at 2am'.
       * </p>
       * @return true if this transition is an overlap, false if it is a gap
       */
      gbool isOverlap() const;

      /**
       * Checks if the specified offset is valid during this transition.
       * <p>
       * This checks to see if the given offset will be valid at some point in the transition.
       * A gap will always return false.
       * An overlap will return true if the offset is either the before or after offset.
       * </p>
       * @param offset  the offset to check, null returns false
       * @return true if the offset is valid during the transition
       */
      gbool isValidOffset(ZoneOffset const& offset) const;

      /**
       * Compares this transition to another based on the transition instant.
       * <p>
       * This compares the instants of each transition.
       * The offsets are ignored, making this order inconsistent with equals.
       * </p>
       * @param otherTransition  the transition to compare to, not null
       * @return the comparator value, that is the comparison of this transition instant
       *          with @c otherTransition instant
       */
      gint compareTo(ZoneOffsetTransition const& otherTransition) const override;

      /**
       * Checks if this object equals another.
       * <p>
       * The entire state of the object is compared.
       * </p>
       * @param other  the other object to compare to, null returns false
       * @return true if equal
       */
      gbool equals(Object const& other) const override;

      /**
       * Returns a suitable hash code.
       *
       * @return the hash code
       */
      gint hash() const override;

      /**
       * Returns a string describing this object.
       *
       * @return a string for debugging, not null
       */
      String toString() const override;

      /**
       * Return the shadow copy of this object
       *
       * @return a shadow copy of this object
       */
      Object& clone() const override;

    private:
      /**
       * Gets the duration of the transition in seconds.
       *
       * @return the duration in seconds
       */
      gint getDurationSeconds() const;

      /**
       * Gets the valid offsets during this transition.
       * <p>
       * A gap will return an empty list, while an overlap will return both offsets.
       * </p>
       * @return the list of valid offsets
       */
      ZoneOffsetArray getValidOffsets() const;
    };
  } // time
} // core

#endif //CORE24_ZONEOFFSETTRANSITION_H
