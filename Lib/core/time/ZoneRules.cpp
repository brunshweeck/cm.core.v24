//
// Created by admin on 29/12/24.
//

#include "ZoneRules.h"

#include <core/time/ZoneOffsetTransition.h>
#include <core/time/ZoneOffsetTransitionRule.h>
#include <core/util/ArrayList.h>
#include <core/util/HashMap.h>

#include "Duration.h"
#include "core/util/Optional.h"

CORE_WARNING_PUSH
CORE_WARNING_DISABLE_DEPRECATED

namespace core {
    using namespace util;

    namespace time {
        ZoneRules::ZoneRules(ZoneOffset const& baseStandardOffset,
                             ZoneOffset const& baseWallOffset,
                             List<ZoneOffsetTransition> const& standardOffsetTransitionList,
                             List<ZoneOffsetTransition> const& transitionList,
                             List<ZoneOffsetTransitionRule> const& lastRules) {
            // convert standard transitions

            standardTransitions = LongArray(standardOffsetTransitionList.size());
            standardOffsets = Array<ZoneOffset>(standardOffsetTransitionList.size() + 1);

            standardOffsets.set(0, baseStandardOffset);
            for (gint i = 0; i < standardOffsetTransitionList.size(); ++i) {
                standardTransitions[i] = standardOffsetTransitionList.get(i).toEpochSeconds();
                standardOffsets.set(i + 1, standardOffsetTransitionList.get(i).offsetAfter());
            }

            // convert savings transitions to locals
            ArrayList<LocalDateTime> localTransitionList;
            ArrayList<ZoneOffset> localTransitionOffsetList;
            localTransitionOffsetList.add(baseWallOffset);
            for (ZoneOffsetTransition const& trans : transitionList) {
                if (trans.isGap()) {
                    localTransitionList.add(trans.dateTimeBefore());
                    localTransitionList.add(trans.dateTimeAfter());
                } else {
                    localTransitionList.add(trans.dateTimeAfter());
                    localTransitionList.add(trans.dateTimeBefore());
                }
                localTransitionOffsetList.add(trans.offsetAfter());
            }

            savingsLocalTransitions = localTransitionList.toArray();
            wallOffsets = localTransitionOffsetList.toArray();

            // convert savings transitions to instants
            savingsInstantTransitions = LongArray(transitionList.size());
            for (int i = 0; i < transitionList.size(); i++) {
                savingsInstantTransitions[i] = transitionList.get(i).toEpochSeconds();
            }

            // last rules
            if (lastRules.size() > 0) {
                Array<ZoneOffsetTransitionRule> rulesArray = lastRules.toArray();
                if (rulesArray.length() > 16) {
                    IllegalArgumentException("Too many transition rules").throws($ftrace());
                }
                ZoneRules::lastRules = rulesArray;
                lastRulesCache = &UNSAFE::newInstance<HashMap<Integer, Array<ZoneOffsetTransition>>>();
            } else {
                ZoneRules::lastRules = { };
                lastRulesCache = null;
            }
        }

        ZoneRules::ZoneRules(ZoneOffset const& offset) {
            standardOffsets = Array<ZoneOffset>(1, offset);
            wallOffsets = standardOffsets;
            lastRulesCache = null;
        }

        gbool ZoneRules::isFixedOffset() const {
            return standardOffsets[0].equals(wallOffsets[0]) &&
                    standardTransitions.length() == 0 &&
                    savingsInstantTransitions.length() == 0 &&
                    lastRules.length() == 0;
        }

        ZoneOffset ZoneRules::offset(LocalDateTime const& localDT) const {
            Object const& info = offsetInfo(localDT);
            if (Class<ZoneOffsetTransition>::hasInstance(info)) {
                ZoneOffsetTransition const& zot = CORE_XCAST(ZoneOffsetTransition const, info);
                ZoneOffset offset = zot.offsetBefore();
                // UNSAFE::deleteRegInstance(zot);
                return offset;
            }

            ZoneOffset offset = CORE_XCAST(ZoneOffset const, info);
            // UNSAFE::deleteRegInstance(info);
            return offset;
        }

        List<ZoneOffset>& ZoneRules::validOffsets(LocalDateTime const& localDT) const {
            // should probably be optimized
            Object const& info = offsetInfo(localDT);
            if (Class<ZoneOffsetTransition>::hasInstance(info)) {
                return CORE_XCAST(ZoneOffsetTransition const, info).validOffsets();
            }
            return List<ZoneOffset>::of(CORE_XCAST(ZoneOffset const, info));
        }

        Optional<ZoneOffsetTransition> ZoneRules::transition(LocalDateTime const& localDT) const {
            // should probably be optimized
            Object const& info = offsetInfo(localDT);
            if (Class<ZoneOffsetTransition>::hasInstance(info)) {
                return CORE_XCAST(ZoneOffsetTransition const, info);
            }
            return Optional<ZoneOffsetTransition>();
        }

        ZoneOffset ZoneRules::standardOffset(LocalDateTime const& localDT) const {
            if (standardTransitions.length() == 0) {
                return standardOffsets[0];
            }
            long epochSec = localDT.toEpochSecond();
            int index = Arrays::binarySearch(standardTransitions, epochSec);
            if (index < 0) {
                // switch negative insert position to start of matched range
                index = -index - 2;
            }
            return standardOffsets[index + 1];
        }

        Duration ZoneRules::daylightSavings(LocalDateTime const& localDT) const {
            if (isFixedOffset()) {
                return Duration::ZERO;
            }
            ZoneOffset standardOffset = ZoneRules::standardOffset(localDT);
            ZoneOffset actualOffset = offset(localDT);
            return Duration::ofSeconds(actualOffset.totalSeconds() - standardOffset.totalSeconds());
        }

        gbool ZoneRules::isDaylightSavings(LocalDateTime const& localDT) const {
            return (standardOffset(localDT).equals(ZoneRules::offset(localDT)) == false);
        }

        gbool ZoneRules::isValidOffset(LocalDateTime const& localDT, ZoneOffset const& offset) const {
            return validOffsets(localDT).contains(offset);
        }

        Optional<ZoneOffsetTransition> ZoneRules::nextTransition(LocalDateTime const& localDT) const {
            if (savingsInstantTransitions.length() == 0) {
                return Optional<ZoneOffsetTransition>();
            }
            glong epochSec = localDT.toEpochSecond();
            // check if using last rules
            if (epochSec >= savingsInstantTransitions[savingsInstantTransitions.length() - 1]) {
                if (lastRules.length() == 0) {
                    return Optional<ZoneOffsetTransition>();
                }
                // search year the instant is in
                int year = findYear(epochSec, wallOffsets[wallOffsets.length() - 1]);
                Array<ZoneOffsetTransition> transArray = findTransitionArray(year);
                for (ZoneOffsetTransition const& trans : transArray) {
                    if (epochSec < trans.toEpochSeconds()) {
                        return trans;
                    }
                }
                // use first from following year
                if (year < Temporal::MAX_YEAR) {
                    transArray = findTransitionArray(year + 1);
                    return transArray[0];
                }
                return Optional<ZoneOffsetTransition>();
            }

            // using historic rules
            int index = Arrays::binarySearch(savingsInstantTransitions, epochSec);
            if (index < 0) {
                index = -index - 1; // switched value is the next transition
            } else {
                index += 1; // exact match, so need to add one to get the next
            }
            return ZoneOffsetTransition(savingsInstantTransitions[index], wallOffsets[index], wallOffsets[index + 1]);
        }

        Optional<ZoneOffsetTransition> ZoneRules::previousTransition(LocalDateTime const& localDT) const {
            if (savingsInstantTransitions.length() == 0) {
                return Optional<ZoneOffsetTransition>();
            }
            glong epochSec = localDT.toEpochSecond();
            if (localDT.nano() > 0 && epochSec < Long::MAX_VALUE) {
                epochSec += 1; // allow rest of method to only use seconds
            }

            // check if using last rules
            glong lastHistoric = savingsInstantTransitions[savingsInstantTransitions.length() - 1];
            if (lastRules.length() > 0 && epochSec > lastHistoric) {
                // search year the instant is in
                ZoneOffset lastHistoricOffset = wallOffsets[wallOffsets.length() - 1];
                int year = findYear(epochSec, lastHistoricOffset);
                Array<ZoneOffsetTransition> transArray = findTransitionArray(year);
                for (int i = transArray.length() - 1; i >= 0; i--) {
                    if (epochSec > transArray[i].toEpochSeconds()) {
                        return transArray[i];
                    }
                }
                // use last from preceding year
                int lastHistoricYear = findYear(lastHistoric, lastHistoricOffset);
                if (--year > lastHistoricYear) {
                    transArray = findTransitionArray(year);
                    return transArray[transArray.length() - 1];
                }
                // drop through
            }

            // using historic rules
            int index = Arrays::binarySearch(savingsInstantTransitions, epochSec);
            if (index < 0) {
                index = -index - 1;
            }
            if (index <= 0) {
                return Optional<ZoneOffsetTransition>();
            }
            return ZoneOffsetTransition(savingsInstantTransitions[index - 1],
                                        wallOffsets[index - 1], wallOffsets[index]);
        }

        List<ZoneOffsetTransition>& ZoneRules::transitions() const {
            ArrayList<ZoneOffsetTransition> list;
            for (int i = 0; i < savingsInstantTransitions.length(); i++) {
                list.add(ZoneOffsetTransition(savingsInstantTransitions[i], wallOffsets[i], wallOffsets[i + 1]));
            }
            return List<ZoneOffsetTransition>::copyOf(list);
        }

        List<ZoneOffsetTransitionRule>& ZoneRules::transitionRules() const {
            return List<ZoneOffsetTransitionRule>::of(lastRules);
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

        ZoneRules::ZoneRules(LongArray const& standardTransitions,
                             Array<ZoneOffset> const& standardOffsets,
                             LongArray const& savingsInstantTransitions,
                             Array<ZoneOffset> const& wallOffsets,
                             Array<ZoneOffsetTransitionRule> const& lastRules)
            : standardTransitions(standardTransitions),
              standardOffsets(standardOffsets),
              savingsInstantTransitions(savingsInstantTransitions),
              wallOffsets(wallOffsets),
              lastRules(lastRules) {
            if (lastRules.length() > 0) {
                lastRulesCache = &UNSAFE::newInstance<HashMap<Integer, Array<ZoneOffsetTransition>>>();
            } else {
                lastRulesCache = null;
            }

            if (savingsInstantTransitions.length() == 0) {
                savingsLocalTransitions = { };
            } else {
                // convert savings transitions to locals
                ArrayList<LocalDateTime> localTransitionList;
                for (int i = 0; i < savingsInstantTransitions.length(); i++) {
                    const ZoneOffset& before = wallOffsets[i];
                    const ZoneOffset& after = wallOffsets[i + 1];
                    ZoneOffsetTransition trans = ZoneOffsetTransition(savingsInstantTransitions[i], before, after);
                    if (trans.isGap()) {
                        localTransitionList.add(trans.dateTimeBefore());
                        localTransitionList.add(trans.dateTimeAfter());
                    } else {
                        localTransitionList.add(trans.dateTimeAfter());
                        localTransitionList.add(trans.dateTimeBefore());
                    }
                }
                savingsLocalTransitions = localTransitionList.toArray();
            }
        }

        Object const& ZoneRules::offsetInfo(LocalDateTime const& dt) const {
            if (savingsLocalTransitions.length() == 0) {
                return wallOffsets[0];
            }
            // check if using last rules
            if (lastRules.length() > 0 && dt.isAfter(savingsLocalTransitions[savingsLocalTransitions.length() - 1])) {
                Array<ZoneOffsetTransition> transArray = findTransitionArray(dt.year());
                Optional<> info = null;
                for (const ZoneOffsetTransition& trans : transArray) {
                    info = findOffsetInfo(dt, trans);
                    if (Class<ZoneOffsetTransition>::hasInstance(info) || info.get().equals(trans.offsetBefore())) {
                        return info.get();
                    }
                }
                return info.get();
            }

            // using historic rules
            int index = Arrays::binarySearch(savingsLocalTransitions, dt);
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
                    return UNSAFE::newInstance<ZoneOffsetTransition>(dtBefore, offsetBefore, offsetAfter);
                } else {
                    // overlap
                    return *new ZoneOffsetTransition(dtAfter, offsetBefore, offsetAfter);
                }
            } else {
                // normal (neither gap or overlap)
                return wallOffsets[index / 2 + 1];
            }
        }

        Optional<> ZoneRules::findOffsetInfo(LocalDateTime const& dt,
                                             ZoneOffsetTransition const& trans) {
            LocalDateTime localTransition = trans.dateTimeBefore();
            if (trans.isGap()) {
                if (dt.isBefore(localTransition)) {
                    return trans.offsetBefore();
                }
                if (dt.isBefore(trans.dateTimeAfter())) {
                    return trans;
                } else {
                    return trans.offsetAfter();
                }
            } else {
                if (dt.isBefore(localTransition) == false) {
                    return trans.offsetAfter();
                }
                if (dt.isBefore(trans.dateTimeAfter())) {
                    return trans.offsetBefore();
                } else {
                    return trans;
                }
            }
        }

        Array<ZoneOffsetTransition> ZoneRules::findTransitionArray(gint year) const {
            Integer yearObj = year; // should use Year class, but this saves a class load
            Object& trans = lastRulesCache->getOrNull(yearObj);
            if (trans != null) {
                return CORE_XCAST(Array<ZoneOffsetTransition>, trans);
            }
            Array<ZoneOffsetTransitionRule> ruleArray = lastRules;
            Array<ZoneOffsetTransition> transArray = Array<ZoneOffsetTransition>(ruleArray.length());
            for (int i = 0; i < ruleArray.length(); i++) {
                transArray.set(i, ruleArray[i].createTransition(year));
            }
            if (year < LAST_CACHED_YEAR) {
                lastRulesCache->putIfAbsent(yearObj, transArray);
            }
            return transArray;
        }

        gint ZoneRules::findYear(glong epochSecond, ZoneOffset const& offset) {
            glong localSecond = epochSecond + offset.totalSeconds();
            glong zeroDay = Math::floorDiv(localSecond, 86400) + LocalDate::DAYS_0000_TO_1970;

            // find the march-based year
            zeroDay -= 60; // adjust to 0000-03-01 so leap day is at end of four year cycle
            glong adjust = 0;
            if (zeroDay < 0) {
                // adjust negative years to positive for calculation
                glong adjustCycles = (zeroDay + 1) / LocalDate::DAYS_PER_CYCLE - 1;
                adjust = adjustCycles * 400;
                zeroDay += -adjustCycles * LocalDate::DAYS_PER_CYCLE;
            }
            glong yearEst = (400 * zeroDay + 591) / LocalDate::DAYS_PER_CYCLE;
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
            return (gint) Math::min(yearEst, (glong) Temporal::MAX_YEAR);
        }
    } // time
} // core

CORE_WARNING_POP
