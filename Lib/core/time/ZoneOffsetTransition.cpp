//
// Created by admin on 29/12/24.
//

#include "ZoneOffsetTransition.h"

#include <core/time/Duration.h>
#include <core/time/spi/ZoneOffsetArray.h>
#include <core/util/List.h>

#include "Instant.h"

namespace core {
  namespace time {
    ZoneOffsetTransition::ZoneOffsetTransition(const LocalDateTime& transition, const ZoneOffset& offsetBefore,
                                               const ZoneOffset& offsetAfter)
      : epochSecond(transition.toEpochSecond(offsetBefore)), transition(transition), offsetBefore_(offsetBefore),
        offsetAfter_(offsetAfter) {
      CORE_ASSERT(transition.nano() == 0);
    }

    ZoneOffsetTransition::ZoneOffsetTransition(glong epochSecond, const ZoneOffset& offsetBefore,
                                               const ZoneOffset& offsetAfter)
      : epochSecond(epochSecond), transition(LocalDateTime::ofEpochSecond(epochSecond, 0, offsetBefore)),
        offsetBefore_(offsetBefore), offsetAfter_(offsetAfter) {}

    ZoneOffsetTransition ZoneOffsetTransition::of(const LocalDateTime& transition, const ZoneOffset& offsetBefore,
                                                  const ZoneOffset& offsetAfter) {
      if (offsetBefore.equals(offsetAfter)) {
        IllegalArgumentException("Offsets must not be equal").throws($ftrace());
      }
      if (transition.nano() != 0) {
        IllegalArgumentException("Nano-of-second must be zero").throws($ftrace());
      }
      return ZoneOffsetTransition(transition, offsetBefore, offsetAfter);
    }

    Instant ZoneOffsetTransition::instant() const {
      return Instant::ofEpochSecond(epochSecond);
    }

    glong ZoneOffsetTransition::toEpochSecond() const {
      return epochSecond;
    }

    LocalDateTime ZoneOffsetTransition::dateTimeBefore() const {
      return transition;
    }

    LocalDateTime ZoneOffsetTransition::dateTimeAfter() const {
      try {
        return transition.plusSeconds(getDurationSeconds());
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneOffset ZoneOffsetTransition::offsetBefore() const {
      return offsetBefore_;
    }

    ZoneOffset ZoneOffsetTransition::offsetAfter() const {
      return offsetAfter_;
    }

    Duration ZoneOffsetTransition::duration() const {
      return Duration::ofSeconds(getDurationSeconds());
    }

    gbool ZoneOffsetTransition::isGap() const {
      return offsetAfter().totalSeconds() > offsetBefore().totalSeconds();
    }

    gbool ZoneOffsetTransition::isOverlap() const {
      return offsetAfter().totalSeconds() < offsetBefore().totalSeconds();
    }

    gbool ZoneOffsetTransition::isValidOffset(ZoneOffset const& offset) const {
      return isGap() ? false : (offsetBefore().equals(offset) || offsetAfter().equals(offset));
    }

    gint ZoneOffsetTransition::compareTo(ZoneOffsetTransition const& otherTransition) const {
      return Long::compare(epochSecond, otherTransition.epochSecond);
    }

    gbool ZoneOffsetTransition::equals(Object const& other) const {
      if (this == &other)
        return true;
      if (Class<ZoneOffsetTransition>::hasInstance(other)) {
        ZoneOffsetTransition const& otherTransition = CORE_XCAST(ZoneOffsetTransition const, other);
        return epochSecond == otherTransition.epochSecond &&
            offsetBefore_.equals(otherTransition.offsetBefore_) &&
            offsetAfter_.equals(otherTransition.offsetAfter_);
      }
      return false;
    }

    gint ZoneOffsetTransition::hash() const {
      return transition.hash() ^ offsetBefore_.hash() ^ Integer::rotateLeft(offsetAfter_.hash(), 16);
    }

    String ZoneOffsetTransition::toString() const {
      XString str;
      str.append("Transition["_Sl)
         .append(isGap() ? "Gap "_Sl : "Overlap"_Sl)
         .append(" at ")
         .append(transition)
         .append(offsetBefore_)
         .append(" to ")
         .append(offsetAfter_)
         .append(u']');
      return str.toString();
    }

    Object& ZoneOffsetTransition::clone() const {
      return UNSAFE::newInstance<ZoneOffsetTransition>(*this);
    }

    gint ZoneOffsetTransition::getDurationSeconds() const {
      return offsetAfter().totalSeconds() - offsetBefore().totalSeconds();
    }

    ZoneOffsetArray ZoneOffsetTransition::getValidOffsets() const {
      if (isGap())
        return ZoneOffsetArray::of();
      return ZoneOffsetArray::of(offsetBefore(), offsetAfter());
    }
  } // time
} // core
