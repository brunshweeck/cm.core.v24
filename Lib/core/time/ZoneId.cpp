//
// Created by admin on 13/12/24.
//

#include "ZoneId.h"

#include <core/lang/IntArray2D.h>
#include <core/lang/LongArray2D.h>
#include <core/time/TemporalQuery.h>
#include <core/time/ZoneOffset.h>
#include <core/time/ZoneRules.h>
#include <core/time/FormatStyle.h>
#include <core/time/ZoneRulesException.h>
#include <core/util/HashSet.h>
#include <core/util/Locale.h>
#include <core/util/Optional.h>
#include <meta/time/ZoneRegion.h>
#include <meta/util/cldr/Cldr.h>

namespace core {
  using namespace util;
  using namespace locales;

  namespace time {
    namespace {
      Map<String, ZoneId>& CACHE_IDS = misc::Unsafe::newInstance<HashMap<String, ZoneId>>();
    }

    Set<String> const& ZoneId::availableZoneIds() {
      static Set<String>& all = UNSAFE::newInstance<HashSet<String>>();
      if (all.isEmpty()) {
        Cldr::timezoneToID(String()); // load timezone data
        for (String const& tz : Cldr::TIMEZONES)
          all.add(tz);
      }
      return Set<String>::copyOf(all);
    }

    ZoneId const& ZoneId::of(String const& zoneId) {
      try { return of(zoneId, true); } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    ZoneId const& ZoneId::ofOffset(String const& prefix, ZoneOffset const& offset) {
      // try {  } catch (Throwable const &ex) { ex.throws($ftrace()); }
      if (prefix.isEmpty()) {
        CACHE_IDS.putIfAbsent(offset.getId(), ZoneOffset::ofTotalSeconds(offset.totalSeconds()));
        return CACHE_IDS.get(offset.getId());
      }

      if (!prefix.equals("GMT"_Sl) && !prefix.equals("UTC"_Sl) && !prefix.equals("UT"_Sl)) {
        IllegalArgumentException("prefix should be GMT, UTC or UT, is: "_Sl + prefix).throws($ftrace());
      }

      String p = prefix;
      if (offset.totalSeconds() != 0) {
        p = prefix.concat(offset.getId());
      }
      ZoneRegion region = ZoneRegion(p, offset.getRules());
      CACHE_IDS.putIfAbsent(p, region);
      return CACHE_IDS.get(p);
    }

    ZoneId const& ZoneId::from(TemporalAccessor const& temporal) {
      OptionalZone obj = temporal.query(TemporalQuery::zone()).tryCast<ZoneId>();
      if (obj.isEmpty()) {
        DateTimeException("Unable to obtain ZoneId from Temporal: "_Sl +
          temporal + " of type " + typeName(temporal)).throws($ftrace());
      }
      return obj.get();
    }

    static String queryDisplayName(gint region, FormatStyle style, gint LID, gint SID, gint RID) {
      gbool L = LID >= 0, S = SID >= 0, R = RID;
      Cldr& cldr = Cldr::forLocale(LID, RID, SID, -1);
      S = cldr.hasScript();
      R = cldr.hasRegion();
      String result;
      switch (style) {
        case FormatStyle::FULL:
        case FormatStyle::LONG:
          result = cldr.displayTimezone(region, 0);
          if (result.isEmpty())
            result = cldr.displayTimezone(region, 1);
          if (result.isEmpty())
            result = cldr.displayTimezone(region, 2);
          break;
        case FormatStyle::MEDIUM:
        case FormatStyle::SHORT:
          result = cldr.displayTimezone(region, 3);
          if (result.isEmpty())
            result = cldr.displayTimezone(region, 4);
          if (result.isEmpty())
            result = cldr.displayTimezone(region, 5);
          break;
      }
      if (result.isEmpty()) {
        if ((L && R) || (L && S))
          return queryDisplayName(region, style, LID, S ? -1 : SID, R ? -1 : RID);
        if (L)
          return queryDisplayName(region, style, -1, -1, -1);
      }
      return result;
    }

    static String queryGenericDisplayName(gint tz, FormatStyle style, gint LID, gint SID, gint RID) {
      gbool L = LID >= 0, S = SID >= 0, R = RID;
      Cldr& cldr = Cldr::forLocale(LID, RID, SID, -1);
      S = cldr.hasScript();
      R = cldr.hasRegion();
      String result;
      switch (style) {
        case FormatStyle::FULL:
        case FormatStyle::LONG:
          result = cldr.displayGlobalTimezone(tz, 0);
          if (result.isEmpty())
            result = cldr.displayGlobalTimezone(tz, 1);
          if (result.isEmpty())
            result = cldr.displayGlobalTimezone(tz, 2);
          break;
        case FormatStyle::MEDIUM:
        case FormatStyle::SHORT:
          result = cldr.displayGlobalTimezone(tz, 3);
          if (result.isEmpty())
            result = cldr.displayGlobalTimezone(tz, 4);
          if (result.isEmpty())
            result = cldr.displayGlobalTimezone(tz, 5);
          break;
      }
      if (result.isEmpty()) {
        if ((L && R) || (L && S))
          return queryDisplayName(tz, style, LID, S ? -1 : SID, R ? -1 : RID);
        if (L)
          return queryDisplayName(tz, style, -1, -1, -1);
      }
      return result;
    }

    String ZoneId::displayName(FormatStyle style, Locale const& locale) const {
      gint tz = Cldr::timezoneToID(getId());
      if (tz == -1)
        return String();

      IntArray res = locale.base.resolve(true);
      gint LID = res[0];
      gint SID = res[1];
      gint RID = res[2];

      String display = queryDisplayName(tz, style, LID, SID, RID);
      if (!display.isEmpty())
        return display;

      gint genericTz = Cldr::timezoneToLongZoneID(tz);
      display = queryGenericDisplayName(genericTz, style, LID, SID, RID);

      if (!display.isEmpty())
        return display;
      display = getId();
      if (display.startsWith("GMT+"))
        return display;
      if (display.startsWith("Etc/GMT+")) {
        ZoneRules rules = getRules();
        ZoneOffset offset = rules.offset(LocalDateTime::now());
        gint seconds = offset.totalSeconds();
        gint hours = seconds / 3600;
        seconds = seconds - hours * 3600;
        gint minutes = seconds / 60;
        return String::format("GMT%+02d:%02d", hours, minutes);
      }

      return String();
    }

    ZoneId const& ZoneId::normalized() const {
      try {
        ZoneRules rules = getRules();
        if (rules.isFixedOffset()) {
          return UNSAFE::newInstance<ZoneOffset>(rules.offset(LocalDateTime::EPOCH));
        }
      } catch (ZoneRulesException const& _) {
        // invalid ZoneRegion is not important to this method
      }
      return *this;
    }

    gbool ZoneId::equals(Object const& obj) const {
      return this == &obj || (Class<ZoneId>::hasInstance(obj) && getId() == CORE_XCAST(ZoneId const, obj).getId());
    }

    gint ZoneId::hash() const {
      return getId().hash();
    }

    String ZoneId::toString() const {
      return getId();
    }

    ZoneId const& ZoneId::of(String const& zoneId, gbool checkAvailable) {
      if (zoneId.length() <= 1 || zoneId.startsWith("+") || zoneId.startsWith("-")) {
        return UNSAFE::newInstance<ZoneOffset>(ZoneOffset::of(zoneId));
      }

      {
        Object& zId = CACHE_IDS.getOrNull(zoneId);
        if (zId != null)
          return CORE_XCAST(ZoneId, zId);
      }

      if (zoneId.startsWith("UTC"_Sl) || zoneId.startsWith("GMT"_Sl)) {
        return ofWithPrefix(zoneId, 3, checkAvailable);
      }
      if (zoneId.startsWith("UT"_Sl)) {
        return ofWithPrefix(zoneId, 2, checkAvailable);
      }
      ZoneRegion region = ZoneRegion::ofID(zoneId, checkAvailable);
      CACHE_IDS.putIfAbsent(zoneId, region);
      return CACHE_IDS.get(zoneId);
    }

    ZoneId const& ZoneId::ofWithPrefix(const String& zoneId, int prefixLength, gbool checkAvailable) {
      String prefix = zoneId.subString(0, prefixLength);
      if (zoneId.length() == prefixLength) {
        return ofOffset(prefix, ZoneOffset::UTC);
      }
      if (zoneId.charAt(prefixLength) != '+' && zoneId.charAt(prefixLength) != '-') {
        return UNSAFE::newInstance<ZoneRegion>(ZoneRegion::ofID(zoneId, checkAvailable));
        // drop through to ZoneRulesProvider
      }
      try {
        ZoneOffset offset = ZoneOffset::of(zoneId.subString(prefixLength));
        if (offset == ZoneOffset::UTC) {
          return ofOffset(prefix, offset);
        }
        return ofOffset(prefix, offset);
      } catch (DateTimeException const& ex) {
        DateTimeException("Invalid ID for offset-based ZoneId: "_Sl + zoneId, ex).throws($ftrace());
      }
    }

    CORE_WARNING_PUSH
    CORE_WARNING_DISABLE_DEPRECATED

    ZoneRules ZoneId::regionToRules(String const& region) {
      gint reg = Cldr::timezoneToID(region);
      if (reg < -1) {
        IllegalArgumentException("Invalid ZoneId: "_Sl + region).throws($ftrace());
      }
      LongArray2D rules = Cldr::regionToRules(reg);
      IntArray2D transitionRules = Cldr::regionToTransitionRules(reg);

      const LongArray& standardTransitions = rules[0];
      ZoneOffsetArray standardOffsets = ZoneOffsetArray(rules[1].length());
      for (gint i = 0; i < rules[1].length(); i++) {
        standardOffsets.set(i, ZoneOffset::ofTotalSeconds(rules[1][i]));
      }
      const LongArray& savingsInstantTransitions = rules[2];
      ZoneOffsetArray wallOffsets = ZoneOffsetArray(rules[3].length());
      for (gint i = 0; i < rules[3].length(); i++) {
        wallOffsets.set(i, ZoneOffset::ofTotalSeconds(rules[3][i]));
      }
      ZoneOffsetTransitionRuleArray lastRules = ZoneOffsetTransitionRuleArray(transitionRules.length());
      for (gint i = 0; i < transitionRules.length(); i++) {
        CORE_ASSERT(transitionRules[i].length() == 9);
        lastRules.set(i, ZoneOffsetTransitionRule::of(
                        (Month) transitionRules[i][0],
                        transitionRules[i][1],
                        (DayOfWeek) transitionRules[i][2],
                        LocalTime::ofSecondOfDay(transitionRules[i][3]),
                        transitionRules[i][4] == 1,
                        (ZoneOffsetTransitionRule::TimeDefinition) transitionRules[i][5],
                        ZoneOffset::ofTotalSeconds(transitionRules[i][6]),
                        ZoneOffset::ofTotalSeconds(transitionRules[i][7]),
                        ZoneOffset::ofTotalSeconds(transitionRules[i][8])
                      ));
      }
      return ZoneRules(standardTransitions, standardOffsets, savingsInstantTransitions, wallOffsets, lastRules);
    }

    CORE_WARNING_POP
  } // time
} // core
