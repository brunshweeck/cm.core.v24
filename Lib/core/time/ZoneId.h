//
// Created by admin on 13/12/24.
//

#ifndef CORE24_ZONEID_H
#define CORE24_ZONEID_H

#include <core/lang/String.h>

namespace core {
  namespace time {
    /**
     * A time-zone ID, such as @c Europe/Paris.
     * <p>
     * A @c ZoneId is used to identify the rules used to convert between
     * an @b Instant and a @b LocalDateTime.
     * There are two distinct types of ID:
     *
     * @li Fixed offsets - a fully resolved offset from UTC/Greenwich, that uses
     *  the same offset for all local date-times
     * @li Geographical regions - an area where a specific set of rules for finding
     *  the offset from UTC/Greenwich apply
     *
     * Most fixed offsets are represented by @b ZoneOffset.
     * Calling @b normalized() on any @c ZoneId will ensure that a
     * fixed offset ID will be represented as a @c ZoneOffset.
     * </p>
     * <p>
     * The actual rules, describing when and how the offset changes, are defined by @b ZoneRules.
     * This class is simply an ID used to obtain the underlying rules.
     * This approach is taken because rules are defined by governments and change
     * frequently, whereas the ID is stable.
     * </p>
     * <p>
     * The distinction has other effects. Serializing the @c ZoneId will only send
     * the ID, whereas serializing the rules sends the entire data set.
     * Similarly, a comparison of two IDs only examines the ID, whereas
     * a comparison of two rules examines the entire data set.
     * </p>
     *
     * <h3>Time-zone IDs</h3>
     * The ID is unique within the system.
     * There are three types of ID.
     * <p>
     * The simplest type of ID is that from @c ZoneOffset.
     * This consists of 'Z' and IDs starting with '+' or '-'.
     * </p>
     * <p>
     * The next type of ID are offset-style IDs with some form of prefix,
     * such as 'GMT+2' or 'UTC+01:00'.
     * The recognised prefixes are 'UTC', 'GMT' and 'UT'.
     * The offset is the suffix and will be normalized during creation.
     * These IDs can be normalized to a @c ZoneOffset using @c normalized().
     * </p>
     * <p>
     * The third type of ID are region-based IDs. A region-based ID must be of
     * two or more characters, and not start with 'UTC', 'GMT', 'UT' '+' or '-'.
     * Region-based IDs are defined by configuration.
     * </p>
     * <p>
     * Time-zone rules are defined by governments and change frequently.
     * There are a number of organizations, known here as groups, that monitor
     * time-zone changes and collate them.
     * The default group is the IANA Time Zone Database (TZDB).
     * Other organizations include IATA (the airline industry body) and Microsoft.
     * </p>
     * <p>
     * Each group defines its own format for the region ID it provides.
     * The TZDB group defines IDs such as 'Europe/London' or 'America/New_York'.
     * TZDB IDs take precedence over other groups.
     * </p>
     * <p>
     * It is strongly recommended that the group name is included in all IDs supplied by
     * groups other than TZDB to avoid conflicts. For example, IATA airline time-zone
     * region IDs are typically the same as the three letter airport code.
     * However, the airport of Utrecht has the code 'UTC', which is obviously a conflict.
     * The recommended format for region IDs from groups other than TZDB is 'group~region'.
     * Thus, if IATA data were defined, Utrecht airport would be 'IATA~UTC'.
     * </p>
     * <h3>Serialization</h3>
     * This class can be serialized and stores the string zone ID in the external form.
     * The @c ZoneOffset subclass uses a dedicated format that only stores the
     * offset from UTC/Greenwich.
     * <p>
     * A @c ZoneId can be deserialized in a Java Runtime where the ID is unknown.
     * For example, if a server-side Java Runtime has been updated with a new zone ID, but
     * the client-side Java Runtime has not been updated. In this case, the @c ZoneId
     * object will exist, and can be queried using @c getId, @c equals,
     * @c hash, @c toString, @c displayName and @c normalized.
     * However, any call to @c getRules will fail with @c ZoneRulesException.
     * </p>
     */
    class ZoneId CORE_SEALED(ZoneOffset, ZoneRegion) : public virtual Object {
    public:
      CORE_ALIAS(StringSet, util::Set<String>);
      CORE_ALIAS(Optional, util::Optional<>);
      CORE_ALIAS(OptionalZone, util::Optional<ZoneId>);
      CORE_ADD_AS_FRIEND(ZonedDateTime);

      /**
       * Gets the system default time-zone.
       *
       * @return the zone ID
       * @throws DateTimeException if the converted zone ID has an invalid format
       * @throws ZoneRulesException if the converted zone region ID cannot be found
       */
      static ZoneId const& systemZone();

      /**
       * Gets the set of available zone IDs.
       * <p>
       * This set includes the string form of all available region-based IDs.
       * Offset-based zone IDs are not included in the returned set.
       * The ID can be passed to @b of(String) to create a @c ZoneId.
       * </p>
       * <p>
       * The set of zone IDs can increase over time, although in a typical application
       * the set of IDs is fixed. Each call to this method is thread-safe.
       * </p>
       * @return a modifiable copy of the set of zone IDs
       */
      static StringSet const& availableZoneIds();

      /**
       * Obtains an instance of @c ZoneId from an ID ensuring that the
       * ID is valid and available for use.
       * <p>
       * This method parses the ID producing a @c ZoneId or @c ZoneOffset.
       * A @c ZoneOffset is returned if the ID is 'Z', or starts with '+' or '-'.
       * The result will always be a valid ID for which @b ZoneRules can be obtained.
       * <p>
       * Parsing matches the zone ID step by step as follows.
       *
       * @li If the zone ID equals 'Z', the result is @c ZoneOffset::UTC.
       * @li If the zone ID consists of a single letter, the zone ID is invalid
       *  and @c DateTimeException is thrown.
       * @li If the zone ID starts with '+' or '-', the ID is parsed as a
       *  @c ZoneOffset using @b ZoneOffset::of(String).
       * @li If the zone ID equals 'GMT', 'UTC' or 'UT' then the result is a @c ZoneId
       *  with the same ID and rules equivalent to @c ZoneOffset::UTC.
       * @li If the zone ID starts with 'UTC+', 'UTC-', 'GMT+', 'GMT-', 'UT+' or 'UT-'
       *  then the ID is a prefixed offset-based ID. The ID is split in two, with
       *  a two or three letter prefix and a suffix starting with the sign.
       *  The suffix is parsed as a @b ZoneOffset.
       *  The result will be a @c ZoneId with the specified UTC/GMT/UT prefix
       *  and the normalized offset ID as per @b ZoneOffset::getId().
       *  The rules of the returned @c ZoneId will be equivalent to the
       *  parsed @c ZoneOffset.
       * @li All other IDs are parsed as region-based zone IDs. Region IDs must
       *  match the regular expression @c [A-Za-z][A-Za-z0-9~/._+-]+
       *  otherwise a @c DateTimeException is thrown. If the zone ID is not
       *  in the configured set of IDs, @c ZoneRulesException is thrown.
       *  The detailed format of the region ID depends on the group supplying the data.
       *  The default set of data is supplied by the IANA Time Zone Database (TZDB).
       *  This has region IDs of the form '{area}/{city}', such as 'Europe/Paris' or 'America/New_York'.
       *  This is compatible with most IDs from @b util::TimeZone.
       * </p>
       *
       * @param zoneId  the time-zone ID
       * @return the zone ID
       * @throws DateTimeException if the zone ID has an invalid format
       * @throws ZoneRulesException if the zone ID is a region ID that cannot be found
       */
      static ZoneId const& of(String const& zoneId);

      /**
       * Obtains an instance of @c ZoneId wrapping an offset.
       * <p>
       * If the prefix is "GMT", "UTC", or "UT" a @c ZoneId
       * with the prefix and the non-zero offset is returned.
       * If the prefix is empty @c "" the @c ZoneOffset is returned.
       * </p>
       * @param prefix  the time-zone ID
       * @param offset  the offset
       * @return the zone ID
       * @throws IllegalArgumentException if the prefix is not one of
       *     "GMT", "UTC", or "UT", or ""
       */
      static ZoneId const& ofOffset(String const& prefix, ZoneOffset const& offset);

      /**
       * Obtains an instance of @c ZoneId from a temporal object.
       * <p>
       * This obtains a zone based on the specified temporal.
       * A @c Temporal represents an arbitrary set of date and time information,
       * which this factory converts to an instance of @c ZoneId.
       * </p>
       * <p>
       * A @c Temporal represents some form of date and time information.
       * This factory converts the arbitrary temporal object to an instance of @c ZoneId.
       * </p>
       * <p>
       * The conversion will try to obtain the zone in a way that favours region-based
       * zones over offset-based zones using @b TemporalQueries::zone().
       * </p>
       * <p>
       * This method matches the signature of the functional interface @b TemporalQuery
       * allowing it to be used as a query via method reference, @b ZoneId::from.
       * </p>
       * @param temporal  the temporal object to convert
       * @return the zone ID
       * @throws DateTimeException if unable to convert to a @c ZoneId
       */
      static ZoneId const& from(TemporalAccessor const& temporal);

      /**
       * Gets the unique time-zone ID.
       * <p>
       * This ID uniquely defines this object.
       * The format of an offset based ID is defined by @b ZoneOffset::getId().
       * </p>
       * @return the time-zone unique ID
       */
      virtual String getId() const = 0;

      /**
       * Gets the textual representation of the zone, such as 'British Time' or
       * '+02:00'.
       * <p>
       * This returns the textual name used to identify the time-zone ID,
       * suitable for presentation to the user.
       * The parameters control the style of the returned text and the locale.
       * </p>
       * <p>
       * If no textual mapping is found then the @b full ID is returned.
       * </p>
       * @param style  the length of the text required
       * @param locale  the locale to use
       * @return the text value of the zone
       */
      String displayName(FormatStyle style, util::Locale const& locale) const;

      /**
       * Gets the time-zone rules for this ID allowing calculations to be performed.
       * <p>
       * The rules provide the functionality associated with a time-zone,
       * such as finding the offset for a given instant or local date-time.
       * </p>
       * <p>
       * The rules are supplied by @b Cldr provider. An advanced provider may
       * support dynamic updates to the rules without restarting the Java Runtime.
       * If so, then the result of this method may change over time.
       * Each individual call will be still remain thread-safe.
       * </p>
       * <p>
       * @b ZoneOffset will always return a set of rules where the offset never changes.
       * </p>
       * @return the rules
       * @throws ZoneRulesException if no rules are available for this ID
       */
      virtual ZoneRules getRules() const = 0;

      /**
       * Normalizes the time-zone ID, returning a @c ZoneOffset where possible.
       * <p>
       * The returns a normalized @c ZoneId that can be used in place of this ID.
       * The result will have @c ZoneRules equivalent to those returned by this object,
       * however the ID returned by @c getId() may be different.
       * </p>
       * <p>
       * The normalization checks if the rules of this @c ZoneId have a fixed offset.
       * If they do, then the @c ZoneOffset equal to that offset is returned.
       * Otherwise, @c this is returned.
       * </p>
       * @return the time-zone unique ID
       */
      virtual ZoneId const& normalized() const;

      /**
       * Checks if this time-zone ID is equal to another time-zone ID.
       * <p>
       * The comparison is based on the ID.
       * </p>
       * @param obj  the object to check, null returns false
       * @return true if this is equal to the other time-zone ID
       */
      gbool equals(Object const& obj) const override;

      /**
       * A hash code for this time-zone ID.
       *
       * @return a suitable hash code
       */
      gint hash() const override;

      /**
       * Outputs this zone as a @c String, using the ID.
       *
       * @return a string representation of this time-zone ID
       */
      String toString() const override;

    private:
      /**
       * Get the effective offset for an instant at the given epochSecond.
       */
      virtual ZoneOffset getOffset(glong epochSecond) const = 0;

      static ZoneId const& of(String const& zoneId, gbool checkAvailable);

      /**
       * Parse once a prefix is established.
       *
       * @param zoneId  the time-zone ID
       * @param prefixLength  the length of the prefix, 2 or 3
       * @param checkAvailable
       * @return the zone ID
       * @throws DateTimeException if the zone ID has an invalid format
       */
      static ZoneId const& ofWithPrefix(const String& zoneId, gint prefixLength, gbool checkAvailable);

    protected:
      /**
       * Return the rules corresponding to specified region ID
       *
       * @param region the zone region id
       * @return the rules corresponding to specified region ID
       */
      static ZoneRules regionToRules(String const& region);
    };
  } // time
} // core

#endif //CORE24_ZONEID_H
