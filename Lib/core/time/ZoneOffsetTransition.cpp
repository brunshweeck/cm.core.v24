//
// Created by admin on 29/12/24.
//

#include "ZoneOffsetTransition.h"

#include <core/time/Duration.h>
#include <core/util/List.h>

namespace core {
    namespace time {
        ZoneOffsetTransition::ZoneOffsetTransition(glong epochSecond,
                                                   ZoneOffset const& offsetBefore,
                                                   ZoneOffset const& offsetAfter)
            : epochSeconds(epochSecond),
              transition(LocalDateTime::ofEpochSecond(epochSecond, 0, offsetBefore)),
              before(offsetBefore), after(offsetAfter) {}

        ZoneOffsetTransition::ZoneOffsetTransition(LocalDateTime const& transition,
                                                   ZoneOffset const& offsetBefore,
                                                   ZoneOffset const& offsetAfter)
            : epochSeconds(transition.toEpochSecond()),
              transition(transition), before(offsetBefore), after(offsetAfter) {
            if (offsetBefore.equals(offsetAfter))
                IllegalArgumentException("Offsets must not be equal").throws($ftrace());
            if (transition.nano() != 0)
                IllegalArgumentException("Nano-of-second must be zero").throws($ftrace());
        }

        glong ZoneOffsetTransition::toEpochSeconds() const { return epochSeconds; }

        LocalDateTime ZoneOffsetTransition::dateTimeBefore() const { return transition; }

        LocalDateTime ZoneOffsetTransition::dateTimeAfter() const {
            gint durationSeconds = offsetAfter().totalSeconds() - before.totalSeconds();
            return transition.plusSeconds(durationSeconds);
        }

        ZoneOffset ZoneOffsetTransition::offsetBefore() const { return before; }

        ZoneOffset ZoneOffsetTransition::offsetAfter() const { return after; }

        Duration ZoneOffsetTransition::duration() const {
            return Duration::ofSeconds(offsetAfter().totalSeconds() - before.totalSeconds());
        }

        gbool ZoneOffsetTransition::isGap() const {
            return after.totalSeconds() > before.totalSeconds();
        }

        gbool ZoneOffsetTransition::isOverlap() const {
            return after.totalSeconds() < before.totalSeconds();
        }

        gbool ZoneOffsetTransition::isValidOffset(ZoneOffset const& offset) const {
            return isGap() ? false : before.equals(offset) || after.equals(offset);
        }

        gint ZoneOffsetTransition::compareTo(ZoneOffsetTransition const& otherTransition) const {
            return Long::compare(epochSeconds, otherTransition.epochSeconds);
        }

        gbool ZoneOffsetTransition::equals(Object const& other) const {
            if (this == &other)
                return true;
            if (Class<ZoneOffsetTransition>::hasInstance(other)) {
                ZoneOffsetTransition const& otherTransition = CORE_XCAST(ZoneOffsetTransition const, other);
                return epochSeconds == otherTransition.epochSeconds &&
                        before.equals(otherTransition.before) &&
                        after.equals(otherTransition.after);
            }
            return false;
        }

        gint ZoneOffsetTransition::hash() const {
            return transition.hash() ^ before.hash() ^ Integer::rotateLeft(after.hash(), 16);
        }

        String ZoneOffsetTransition::toString() const {
            XString str;
            str.append("Transition["_Sl)
               .append(isGap() ? "Gap "_Sl : "Overlap"_Sl)
               .append(" at ")
               .append(transition)
               .append(before)
               .append(" to ")
               .append(after)
               .append(u']');
            return str.toString();
        }

        Object& ZoneOffsetTransition::clone() const {
            try {
                return UNSAFE::newInstance<ZoneOffsetTransition>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        util::List<ZoneOffset>& ZoneOffsetTransition::validOffsets() const {
            if (isGap())
                return util::List<ZoneOffset>::of();
            return util::List<ZoneOffset>::of(offsetBefore(), offsetAfter());
        }
    } // time
} // core
