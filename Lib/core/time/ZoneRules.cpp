//
// Created by admin on 29/12/24.
//

#include "ZoneRules.h"

#include <core/time/Duration.h>
#include <core/time/ZoneOffsetTransition.h>
#include <core/time/spi/ZoneOffsetTransitionArray.h>
#include <core/time/spi/ZoneOffsetTransitionRuleArray.h>
#include <core/util/ArrayList.h>
#include <core/util/Optional.h>

#include "Instant.h"
#include "Year.h"

CORE_WARNING_PUSH
CORE_WARNING_DISABLE_DEPRECATED

namespace core {
  using namespace util;

  namespace time {
    ZoneRules::ZoneRules(ZoneOffset const& baseStandardOffset, ZoneOffset const& baseWallOffset,
                         ZoneOffsetTransitionArray const& standardOffsetTransitionList,
                         ZoneOffsetTransitionArray const& transitionList,
                         ZoneOffsetTransitionRuleArray const& lastRules) {
      // convert standard transitions

      this->standardTransitions = LongArray(standardOffsetTransitionList.length());

      this->standardOffsets = ZoneOffsetArray(standardOffsetTransitionList.length() + 1);
      this->standardOffsets.set(0, baseStandardOffset);
      for (gint i = 0; i < standardOffsetTransitionList.length(); i++) {
        this->standardTransitions.set(i, standardOffsetTransitionList[i].toEpochSecond());
        this->standardOffsets.set(i + 1, standardOffsetTransitionList[i].offsetAfter());
      }

      // convert savings transitions to locals

      LocalDateTimeArray localTransitionList = LocalDateTimeArray(transitionList.length() << 1);
      ZoneOffsetArray localTransitionOffsetList = ZoneOffsetArray(transitionList.length() + 2);
      gint i = 0, j = 0;

      localTransitionOffsetList.set(j++, baseWallOffset);
      for (ZoneOffsetTransition const& trans : transitionList) {
        if (trans.isGap()) {
          localTransitionList.set(i++, trans.dateTimeBefore());
          localTransitionList.set(i++, trans.dateTimeAfter());
        } else {
          localTransitionList.set(i++, trans.dateTimeAfter());
          localTransitionList.set(i++, trans.dateTimeBefore());
        }
        localTransitionOffsetList.set(j++, trans.offsetAfter());
      }
      this->savingsLocalTransitions = localTransitionList;
      this->wallOffsets = localTransitionOffsetList;

      // convert savings transitions to instants
      this->savingsInstantTransitions = LongArray(transitionList.length());
      for (i = 0; i < transitionList.length(); i++) {
        this->savingsInstantTransitions[i] = transitionList.get(i).toEpochSecond();
      }

      // last rules
      if (lastRules.length() > 0) {
        const ZoneOffsetTransitionRuleArray& rulesArray = lastRules;
        if (rulesArray.length() > 16) {
          IllegalArgumentException("Too many transition rules").throws($ftrace());
        }
        this->lastRules = rulesArray;
      } else {
        this->lastRules = { };
      }
    }

    ZoneRules::ZoneRules(LongArray const& standardTransitions, ZoneOffsetArray const& standardOffsets,
                         LongArray const& savingsInstantTransitions, ZoneOffsetArray const& wallOffsets,
                         ZoneOffsetTransitionRuleArray const& lastRules) {
      this->standardTransitions = standardTransitions;
      this->standardOffsets = standardOffsets;
      this->savingsInstantTransitions = savingsInstantTransitions;
      this->wallOffsets = wallOffsets;
      this->lastRules = lastRules;

      if (savingsInstantTransitions.length() == 0) {
        this->savingsLocalTransitions = { };
      } else {
        // convert savings transitions to locals
        LocalDateTimeArray localTransitionList = LocalDateTimeArray(savingsInstantTransitions.length() << 1);
        gint j = 0;
        for (gint i = 0; i < savingsInstantTransitions.length(); i++) {
          const ZoneOffset& before = wallOffsets[i];
          const ZoneOffset& after = wallOffsets[i + 1];
          ZoneOffsetTransition trans = ZoneOffsetTransition(savingsInstantTransitions[i], before, after);
          if (trans.isGap()) {
            localTransitionList.set(j++, trans.dateTimeBefore());
            localTransitionList.set(j++, trans.dateTimeAfter());
          } else {
            localTransitionList.set(j++, trans.dateTimeAfter());
            localTransitionList.set(j++, trans.dateTimeBefore());
          }
        }
        this->savingsLocalTransitions = localTransitionList;
      }
    }

    ZoneRules::ZoneRules(ZoneOffset const& offset) {
      this->standardOffsets = ZoneOffsetArray(1);
      this->standardOffsets.set(0, offset);
      this->standardTransitions = { };
      this->savingsInstantTransitions = { };
      this->savingsLocalTransitions = { };
      this->wallOffsets = standardOffsets;
      this->lastRules = { };
    }

    ZoneRules ZoneRules::of(ZoneOffset const& baseStandardOffset, ZoneOffset const& baseWallOffset,
                            ZoneOffsetTransitionArray const& standardOffsetTransitionList,
                            ZoneOffsetTransitionArray const& transitionList,
                            ZoneOffsetTransitionRuleArray const& lastRules) {
      return ZoneRules(baseStandardOffset, baseWallOffset, standardOffsetTransitionList, transitionList, lastRules);
    }

    ZoneRules ZoneRules::of(ZoneOffset const& offset) {
      return ZoneRules(offset);
    }

    gbool ZoneRules::isFixedOffset() const {
      return standardOffsets[0].equals(wallOffsets[0]) &&
          standardTransitions.length() == 0 &&
          savingsInstantTransitions.length() == 0 &&
          lastRules.length() == 0;
    }

    ZoneOffset ZoneRules::offset(Instant const& instant) const {
      if (savingsInstantTransitions.length() == 0) {
        return wallOffsets[0];
      }
      glong epochSec = instant.epochSecond();
      // check if using last rules
      if (lastRules.length() > 0 && epochSec > savingsInstantTransitions[savingsInstantTransitions.length() - 1]) {
        gint year = findYear(epochSec, wallOffsets[wallOffsets.length() - 1]);
        ZoneOffsetTransitionArray transArray = findTransitionArray(year);
        ZoneOffsetTransitionArray trans = ZoneOffsetTransitionArray(1);
        for (gint i = 0; i < transArray.length(); i++) {
          trans.set(0, transArray[i]);
          if (epochSec < trans[0].toEpochSecond()) {
            return trans[0].offsetBefore();
          }
        }
        return trans[0].offsetAfter();
      }

      // using historic rules
      gint index = Arrays::binarySearch(savingsInstantTransitions, epochSec);
      if (index < 0) {
        // switch negative insert position to start of matched range
        index = -index - 2;
      }
      return wallOffsets[index + 1];
    }

    ZoneOffset ZoneRules::offset(LocalDateTime const& ldt) const {
      Optional option = getOffsetInfo(ldt);
      if (option.tryCast<ZoneOffsetTransition>().isPresent())
        return ((OptionalTransition) option).deleteOnClose().get().offsetBefore();
      return ((OptionalOffset) option).deleteOnClose().get();
    }

    ZoneOffsetArray ZoneRules::validOffsets(LocalDateTime const& ldt) const {
      // should probably be optimized
      Optional option = getOffsetInfo(ldt);
      if (option.tryCast<ZoneOffsetTransition>().isPresent())
        return ((OptionalTransition) option).deleteOnClose().get().getValidOffsets();
      return ZoneOffsetArray::of(((OptionalOffset) option).deleteOnClose().get());
    }

    ZoneRules::OptionalTransition ZoneRules::transition(LocalDateTime const& ldt) const {
      return getOffsetInfo(ldt).tryCast<ZoneOffsetTransition>();
    }

    ZoneOffset ZoneRules::standardOffset(Instant const& instant) const {
      if (standardTransitions.length() == 0) {
        return standardOffsets[0];
      }
      glong epochSec = instant.epochSecond();
      gint index = Arrays::binarySearch(standardTransitions, epochSec);
      if (index < 0) {
        // switch negative insert position to start of matched range
        index = -index - 2;
      }
      return standardOffsets[index + 1];
    }

    Duration ZoneRules::daylightSavings(Instant const& instant) const {
      if (isFixedOffset()) {
        return Duration::ZERO;
      }
      ZoneOffset standardOffset_ = standardOffset(instant);
      ZoneOffset actualOffset = offset(instant);
      return Duration::ofSeconds(actualOffset.totalSeconds() - standardOffset_.totalSeconds());
    }

    gbool ZoneRules::isDaylightSavings(Instant const& instant) const {
      return (standardOffset(instant).equals(offset(instant)) == false);
    }

    gbool ZoneRules::isValidOffset(LocalDateTime const& ldt, ZoneOffset const& offset) const {
      ZoneOffsetArray zoneOffsets = validOffsets(ldt);
      return Arrays::binarySearch(zoneOffsets, offset) >= 0;
    }

    ZoneRules::OptionalTransition ZoneRules::nextTransition(Instant const& instant) const {
      if (savingsInstantTransitions.length() == 0) {
        return OptionalTransition::empty();
      }
      glong epochSec = instant.epochSecond();
      // check if using last rules
      if (epochSec >= savingsInstantTransitions[savingsInstantTransitions.length() - 1]) {
        if (lastRules.length() == 0) {
          return OptionalTransition::empty();
        }
        // search year the instant is in
        gint year = findYear(epochSec, wallOffsets[wallOffsets.length() - 1]);
        ZoneOffsetTransitionArray transArray = findTransitionArray(year);
        for (ZoneOffsetTransition const& trans : transArray) {
          if (epochSec < trans.toEpochSecond()) {
            return trans;
          }
        }
        // use first from following year
        if (year < Year::MAX_VALUE) {
          transArray = findTransitionArray(year + 1);
          return transArray[0];
        }
        return OptionalTransition::empty();
      }

      // using historic rules
      gint index = Arrays::binarySearch(savingsInstantTransitions, epochSec);
      if (index < 0) {
        index = -index - 1; // switched value is the next transition
      } else {
        index += 1; // exact match, so need to add one to get the next
      }
      return ZoneOffsetTransition(savingsInstantTransitions[index], wallOffsets[index], wallOffsets[index + 1]);
    }

    ZoneRules::OptionalTransition ZoneRules::previousTransition(Instant const& instant) const {
      if (savingsInstantTransitions.length() == 0) {
        return OptionalTransition::empty();
      }
      glong epochSec = instant.epochSecond();
      if (instant.nano() > 0 && epochSec < Long::MAX_VALUE) {
        epochSec += 1; // allow rest of method to only use seconds
      }

      // check if using last rules
      glong lastHistoric = savingsInstantTransitions[savingsInstantTransitions.length() - 1];
      if (lastRules.length() > 0 && epochSec > lastHistoric) {
        // search year the instant is in
        ZoneOffset lastHistoricOffset = wallOffsets[wallOffsets.length() - 1];
        gint year = findYear(epochSec, lastHistoricOffset);
        ZoneOffsetTransitionArray transArray = findTransitionArray(year);
        for (gint i = transArray.length() - 1; i >= 0; i--) {
          if (epochSec > transArray[i].toEpochSecond()) {
            return transArray[i];
          }
        }
        // use last from preceding year
        gint lastHistoricYear = findYear(lastHistoric, lastHistoricOffset);
        if (--year > lastHistoricYear) {
          transArray = findTransitionArray(year);
          return transArray[transArray.length() - 1];
        }
        // drop through
      }

      // using historic rules
      gint index = Arrays::binarySearch(savingsInstantTransitions, epochSec);
      if (index < 0) {
        index = -index - 1;
      }
      if (index <= 0) {
        return OptionalTransition::empty();
      }
      return ZoneOffsetTransition(savingsInstantTransitions[index - 1], wallOffsets[index - 1], wallOffsets[index]);
    }

    ZoneOffsetTransitionArray ZoneRules::transitions() const {
      ZoneOffsetTransitionArray list = ZoneOffsetTransitionArray(savingsInstantTransitions.length());
      for (gint i = 0; i < savingsInstantTransitions.length(); i++) {
        list.set(i, ZoneOffsetTransition(savingsInstantTransitions[i], wallOffsets[i], wallOffsets[i + 1]));
      }
      return list;
    }

    ZoneOffsetTransitionRuleArray ZoneRules::transitionRules() const {
      return lastRules;
    }

    gbool ZoneRules::equals(Object const& otherRules) const {
      if (this == &otherRules)
        return true;
      if (!Class<ZoneRules>::hasInstance(otherRules))
        return false;
      ZoneRules const& other = CORE_XCAST(ZoneRules const, otherRules);
      return Arrays::equals(standardTransitions, other.standardTransitions)
          && Arrays::equals(standardOffsets, other.standardOffsets)
          && Arrays::equals(savingsInstantTransitions, other.savingsInstantTransitions)
          && Arrays::equals(wallOffsets, other.wallOffsets)
          && Arrays::equals(lastRules, other.lastRules);
    }

    gint ZoneRules::hash() const {
      return Arrays::hash(standardTransitions) ^
          Arrays::hash(standardOffsets) ^
          Arrays::hash(savingsInstantTransitions) ^
          Arrays::hash(wallOffsets) ^
          Arrays::hash(lastRules);
    }

    String ZoneRules::toString() const {
      return "ZoneRules[currentStandardOffset="_Sl + standardOffsets[standardOffsets.length() - 1] + "]";
    }

    Object& ZoneRules::clone() const {
      return UNSAFE::newInstance<ZoneRules>(*this);
    }

    ZoneRules::Optional ZoneRules::getOffsetInfo(LocalDateTime const& ldt) const {
      if (savingsLocalTransitions.length() == 0) {
        return wallOffsets[0];
      }
      // check if using last rules
      if (lastRules.length() > 0 &&
        ldt.isAfter(savingsLocalTransitions[savingsLocalTransitions.length() - 1])) {
        ZoneOffsetTransitionArray transArray = findTransitionArray(ldt.year());
        Optional info = null;
        for (ZoneOffsetTransition const& trans : transArray) {
          info = findOffsetInfo(ldt, trans);
          if (Class<ZoneOffsetTransition>::hasInstance(info) || info.equals(trans.offsetBefore())) {
            return info;
          }
        }
        return info;
      }

      // using historic rules
      gint index = Arrays::binarySearch(savingsLocalTransitions, ldt);
      if (index == -1) {
        // before first transition
        return wallOffsets[0];
      }
      if (index < 0) {
        // switch negative insert position to start of matched range
        index = -index - 2;
      } else if (index < savingsLocalTransitions.length() - 1 &&
        savingsLocalTransitions[index].equals(savingsLocalTransitions[index + 1])) {
        // handle overlap immediately following gap
        index++;
      }
      if ((index & 1) == 0) {
        // gap or overlap
        LocalDateTime dtBefore = savingsLocalTransitions[index];
        LocalDateTime dtAfter = savingsLocalTransitions[index + 1];
        ZoneOffset offsetBefore = wallOffsets[index / 2];
        ZoneOffset offsetAfter = wallOffsets[index / 2 + 1];
        if (offsetAfter.totalSeconds() > offsetBefore.totalSeconds()) {
          // gap
          return ZoneOffsetTransition(dtBefore, offsetBefore, offsetAfter).offsetBefore();
        } else {
          // overlap
          return ZoneOffsetTransition(dtAfter, offsetBefore, offsetAfter).offsetBefore();
        }
      } else {
        // normal (neither gap or overlap)
        return wallOffsets[index / 2 + 1];
      }
    }

    ZoneRules::Optional ZoneRules::findOffsetInfo(LocalDateTime const& ldt, ZoneOffsetTransition const& trans) const {
      CORE_IGNORE(this);
      LocalDateTime localTransition = trans.dateTimeBefore();
      if (trans.isGap()) {
        if (ldt.isBefore(localTransition)) {
          return trans.offsetBefore();
        }
        if (ldt.isBefore(trans.dateTimeAfter())) {
          return trans;
        } else {
          return trans.offsetAfter();
        }
      } else {
        if (ldt.isBefore(localTransition) == false) {
          return trans.offsetAfter();
        }
        if (ldt.isBefore(trans.dateTimeAfter())) {
          return trans.offsetBefore();
        } else {
          return trans;
        }
      }
    }

    ZoneOffsetTransitionArray ZoneRules::findTransitionArray(gint year) const {
      ZoneOffsetTransitionArray transArray = ZoneOffsetTransitionArray(lastRules.length());
      for (int i = 0; i < lastRules.length(); i++) {
        transArray.set(i, lastRules[i].createTransition(year));
      }
      return transArray;
    }

    gint ZoneRules::findYear(glong epochSecond, ZoneOffset const& offset) {
      glong localSecond = epochSecond + offset.totalSeconds();
      glong zeroDay = Math::floorDiv(localSecond, 86400) + DAYS_0000_TO_1970;

      // find the march-based year
      zeroDay -= 60; // adjust to 0000-03-01 so leap day is at end of four-year cycle
      glong adjust = 0;
      if (zeroDay < 0) {
        // adjust negative years to positive for calculation
        glong adjustCycles = (zeroDay + 1) / DAYS_PER_CYCLE - 1;
        adjust = adjustCycles * 400;
        zeroDay += -adjustCycles * DAYS_PER_CYCLE;
      }
      glong yearEst = (400 * zeroDay + 591) / DAYS_PER_CYCLE;
      glong doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
      if (doyEst < 0) {
        // fix estimate
        yearEst--;
        doyEst = zeroDay - (365 * yearEst + yearEst / 4 - yearEst / 100 + yearEst / 400);
      }
      yearEst += adjust; // reset any negative year

      // convert march-based values back to january-based, adjust year
      if (doyEst >= 306) {
        yearEst++;
      }

      // Cap to the max value
      return (gint) Math::min(yearEst, (glong) Year::MAX_VALUE);
    }
  } // time
} // core

CORE_WARNING_POP
