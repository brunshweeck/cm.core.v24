//
// Created by admin on 18/01/25.
//

#ifndef CORE24_TIMEZONE_H
#define CORE24_TIMEZONE_H

#include <core/time/ZoneId.h>
#include <core/util/Optional.h>

namespace core {
    namespace time {
        /**
         * @c TimeZone represents a time zone offset, and also figures out daylight
         * savings.
         *
         * <p>
         * Typically, you get a @c TimeZone using @c getDefault
         * which creates a @c TimeZone based on the time zone where the program
         * is running. For example, for a program running in Japan, @c getDefault
         * creates a @c TimeZone object based on Japanese Standard Time.
         * </p>
         * <p>
         * You can also get a @c TimeZone using @c forID
         * along with a time zone ID. For instance, the time zone ID for the
         * U.S. Pacific Time zone is "America/Los_Angeles". So, you can get a
         * U.S. Pacific Time @c TimeZone object with:
         *
         * @code
         *  TimeZone &tz = TimeZone::forID("America/Los_Angeles");
         * @endcode
         * </p>
         * <p>
         * You can use the @c availableIDs method to iterate through
         * all the supported time zone IDs. You can then choose a
         * supported ID to get a @c TimeZone.
         * If the time zone you want is not represented by one of the
         * supported IDs, then a custom time zone ID can be specified to
         * produce a TimeZone. The syntax of a custom time zone ID is:
         *
         * <br>
         * <i>CustomID:</i>
         *         <br> @c GMT <i>Sign</i> <i>Hours</i> @c : <i>Minutes</i> @c : <i>Seconds</i>
         *         <br> @c GMT <i>Sign</i> <i>Hours</i> @c : <i>Minutes</i>
         *         <br> @c GMT <i>Sign</i> <i>Hours</i> <i>Minutes</i>
         *         <br> @c GMT <i>Sign</i> <i>Hours</i>
         * <br>
         * <i>Sign:</i> one of
         *         <br> @c + -
         * <br>
         * <i>Hours:</i>
         *         <br> <i>Digit</i>
         *         <br> <i>Digit</i> <i>Digit</i>
         * <br>
         * <i>Minutes:</i>
         *         <br> <i>Digit</i> <i>Digit</i>
         * <br>
         * <i>Seconds:</i>
         *         <br> <i>Digit</i> <i>Digit</i>
         * <br>
         * <i>Digit:</i> one of
         *         <br> @code 0 1 2 3 4 5 6 7 8 9 @endcode
         * <br>
         *
         * <i>Hours</i> must be between 0 and 23 and <i>Minutes</i>/<i>Seconds</i> must be
         * between 00 and 59.  For example, "GMT+10" and "GMT+0010" mean ten
         * hours and ten minutes ahead of GMT, respectively.
         * </p>
         * <p>
         * The format is locale independent and digits must be taken from the
         * Basic Latin block of the Unicode standard. No daylight saving time
         * transition schedule can be specified with a custom time zone ID. If
         * the specified string doesn't match the syntax, @c "GMT"
         * is used.
         * </p>
         * <p>
         * When creating a @c TimeZone, the specified custom time
         * zone ID is normalized in the following syntax:
         * <br>
         * <i>NormalizedCustomID:</i>
         *         @c GMT <i>Sign</i> <i>TwoDigitHours</i> @c : <i>Minutes</i> [<i>ColonSeconds</i>]
         * <br>
         * <i>Sign:</i> one of
         *         <br> @c + -
         * <br>
         * <i>TwoDigitHours:</i>
         *         <br> <i>Digit</i> <i>Digit</i>
         * <br>
         * <i>Minutes:</i>
         *         <br> <i>Digit</i> <i>Digit</i>
         * <br>
         * <i>ColonSeconds:</i>
         *         <br> @c : <i>Digit</i> <i>Digit</i>
         * <br>
         * <i>Digit:</i> one of
         *         <br> @code 0 1 2 3 4 5 6 7 8 9 @endcode
         * <br>
         * For example, TimeZone.getTimeZone("GMT-8").getID() returns "GMT-08:00".
         * <i>ColonSeconds</i> part only appears if the seconds value is non-zero.
         * </p>
         *
         * @see          Calendar
         * @see          GregorianCalendar
         * @see          SimpleTimeZone
         */
        class TimeZone : public Object {
            /**
             * The string identifier of this @c TimeZone.  This is a
             * programmatic identifier used internally to look up @c TimeZone
             * objects from the system table and also to map them to their localized
             * display names.  @c ID values are unique in the system
             * table but may not be for dynamically created zones.
             * @serial
             */
            String ID;

            /**
             * Cached @b ZoneId for this TimeZone
             */
            util::Optional<ZoneId> mutable zoneId;

        public:
            /**
             * Sole constructor.  (For invocation by subclass constructors, typically
             * implicit.)
             */
            TimeZone() = default;

            /**
             * A style specifier
             */
            enum Style {
                /**
                 * A style specifier for @c displayName() indicating
                 * a short name, such as "PST."
                 * @see LONG
                 */
                SHORT = 0,

                /**
                 * A style specifier for @c displayName() indicating
                 * a long name, such as "Pacific Standard Time."
                 * @see SHORT
                 */
                LONG = 1,
            };

            /**
             * Gets the time zone offset, for current date, modified in case of
             * daylight savings. This is the offset to add to UTC to get local time.
             * <p>
             * This method returns a historically correct offset if an
             * underlying @c TimeZone implementation subclass
             * supports historical Daylight Saving Time schedule and GMT
             * offset changes.
             * </p>
             * @param era the era of the given date.
             * @param year the year in the given date.
             * @param month the month in the given date.
             * Month is 0-based. e.g., 0 for January.
             * @param day the day-in-month of the given date.
             * @param dayOfWeek the day-of-week of the given date.
             * @param milliseconds the milliseconds in day in <em>standard</em>
             * local time.
             *
             * @return the offset in milliseconds to add to GMT to get local time.
             *
             * @see Calendar::ZONE_OFFSET
             * @see Calendar::DST_OFFSET
             */
            virtual gint offset(gint era, gint year, gint month, gint day, gint dayOfWeek,
                                gint milliseconds) const = 0;

            /**
             * Returns the offset of this time zone from UTC at the specified
             * date. If Daylight Saving Time is in effect at the specified
             * date, the offset value is adjusted with the amount of daylight
             * saving.
             * <p>
             * This method returns a historically correct offset value if an
             * underlying TimeZone implementation subclass supports historical
             * Daylight Saving Time schedule and GMT offset changes.
             * </p>
             * @param date the date represented in milliseconds since January 1, 1970 00:00:00 GMT
             * @return the amount of time in milliseconds to add to UTC to get local time.
             *
             * @see Calendar::ZONE_OFFSET
             * @see Calendar::DST_OFFSET
             */
            virtual gint offset(glong date) const;

            /**
             * Sets the base time zone offset to GMT.
             * This is the offset to add to UTC to get local time.
             * <p>
             * If an underlying @c TimeZone implementation subclass
             * supports historical GMT offset changes, the specified GMT
             * offset is set as the latest GMT offset and the difference from
             * the known latest GMT offset value is used to adjust all
             * historical GMT offset values.
             * </p>
             * @param offsetMillis the given base time zone offset to GMT.
             */
            virtual void setRawOffset(gint offsetMillis) = 0;

            /**
             * Returns the amount of time in milliseconds to add to UTC to get
             * standard time in this time zone. Because this value is not
             * affected by daylight saving time, it is called <I>raw
             * offset</I>.
             * <p>
             * If an underlying @c TimeZone implementation subclass
             * supports historical GMT offset changes, the method returns the
             * raw offset value of the current date. In Honolulu, for example,
             * its raw offset changed from GMT-10:30 to GMT-10:00 in 1947, and
             * this method always returns -36000000 milliseconds (i.e., -10
             * hours).
             * </p>
             * @return the amount of raw offset time in milliseconds to add to UTC.
             * @see Calendar::ZONE_OFFSET
             */
            virtual gint rawOffset() const = 0;

            /**
             * Gets the ID of this time zone.
             * @return the ID of this time zone.
             */
            virtual String getID() const;

            /**
             * Sets the time zone ID. This does not change any other data in
             * the time zone object.
             *
             * @param ID the new time zone ID.
             */
            virtual void setID(String ID) = 0;

            /**
             * Returns a long standard time name of this @c TimeZone suitable for
             * presentation to the user in the default locale.
             *
             * <p>
             * This method is equivalent to:
             *
             * @code
             *  displayName(false, TimeZone::LONG, Locale::getDefault(Locale::DISPLAY));
             * @endcode
             * </p>
             *
             * @return the human-readable name of this time zone in the default locale.
             * @see displayName(boolean, int, Locale)
             * @see Locale::getDefault(Locale::Category)
             * @see Locale::Category
             */
            String displayName() const;

            /**
             * Returns a long standard time name of this @c TimeZone suitable for
             * presentation to the user in the specified @c locale.
             *
             * <p>This method is equivalent to:
             *
             * @code
             * displayName(false, TimeZone::LONG, locale);
             * @endcode
             *
             *
             * @param locale the locale in which to supply the display name.
             * @return the human-readable name of this time zone in the given locale.
             * @see displayName(boolean, int, Locale)
             */
            String displayName(util::Locale const& locale) const;

            /**
             * Returns a name in the specified @c style of this @c TimeZone
             * suitable for presentation to the user in the specified
             * @c locale. If the specified @c daylight is @c true, a Daylight
             * Saving Time name is returned (even if this @c TimeZone doesn't
             * observe Daylight Saving Time). Otherwise, a Standard Time name is
             * returned.
             *
             * <p>
             * When looking up a time zone name, the @em default
             * @c Locale search path of @c ResourceBundle derived
             * from the specified @c locale is used. (No @em fallback
             * @c Locale search is performed.) If a timezone name in any
             * @c Locale of the search path, including @b Locale::ROOT, is
             * found, the name is returned. Otherwise, a string in the
             * normalized custom ID format is returned.
             * </p>
             * @param daylight @c true specifying a Daylight Saving Time name, or
             *                 @c false specifying a Standard Time name
             * @param style either @b LONG or @b SHORT
             * @param locale   the locale in which to supply the display name.
             * @return the human-readable name of this time zone in the given locale.
             */
            virtual String displayName(gbool daylight, Style style, util::Locale const& locale) const;

            /**
             * Returns the amount of time to be added to local standard time
             * to get local wall clock time.
             *
             * <p>
             * The default implementation returns 3600000 milliseconds
             * (i.e., one hour) if a call to @b useDaylightTime()
             * returns @c true. Otherwise, 0 (zero) is returned.
             * </p>
             * <p>
             * If an underlying @c TimeZone implementation subclass
             * supports historical and future Daylight Saving Time schedule
             * changes, this method returns the amount of saving time of the
             * last known Daylight Saving Time rule that can be a future
             * prediction.
             * </p>
             * <p>
             * If the amount of saving time at any given time stamp is
             * required, construct a @b Calendar with this @c TimeZone and
             * the time stamp, and call @b Calendar.get(Calendar::DST_OFFSET).
             * </p>
             * @return the amount of saving time in milliseconds
             * @see inDaylightTime(Date)
             * @see offset(long)
             * @see offset(int,int,int,int,int,int)
             * @see Calendar::ZONE_OFFSET
             */
            virtual gint dstSavings() const;

            /**
             * Queries if this @c TimeZone uses Daylight Saving Time.
             *
             * <p>
             * If an underlying @c TimeZone implementation subclass
             * supports historical and future Daylight Saving Time schedule
             * changes, this method refers to the last known Daylight Saving Time
             * rule that can be a future prediction and may not be the same as
             * the current rule. Consider calling @b observesDaylightTime()
             * if the current rule should also be taken into account.
             * </p>
             * @return @c true if this @c TimeZone uses Daylight Saving Time,
             *         @c false, otherwise.
             * @see inDaylightTime(Date)
             * @see Calendar::DST_OFFSET
             */
            virtual gbool useDaylightTime() const;

            /**
             * Returns @c true if this @c TimeZone is currently in
             * Daylight Saving Time, or if a transition from Standard Time to
             * Daylight Saving Time occurs at any future time.
             *
             * <p>
             * The default implementation returns @c true if
             * @c useDaylightTime() or @c inDaylightTime(Date())
             * returns @c true.
             * </p>
             * @return @c true if this @c TimeZone is currently in
             * Daylight Saving Time, or if a transition from Standard Time to
             * Daylight Saving Time occurs at any future time; @c false
             * otherwise.
             * @see useDaylightTime()
             * @see inDaylightTime(Date)
             * @see Calendar::DST_OFFSET
             */
            virtual gbool observesDayLightTime() const;

            /**
             * Queries if the given @c date is in Daylight Saving Time in
             * this time zone.
             *
             * @param date the given Date.
             * @return @c true if the given date is in Daylight Saving Time,
             *         @c false, otherwise.
             */
            virtual gbool inDaylightTime(Date const &date) const = 0;

            /**
             * Gets the @c TimeZone for the given ID.
             *
             * @param ID the ID for a @c TimeZone, either an abbreviation
             * such as "PST", a full name such as "America/Los_Angeles", or a custom
             * ID such as "GMT-8:00".
             *
             * @return the specified @c TimeZone, or the GMT zone if the given ID
             * cannot be understood.
             */
            static TimeZone& of(String const& ID);

            /**
             * Gets the @c TimeZone for the given @c zoneId.
             *
             * @param zoneId a @b ZoneId from which the time zone ID is obtained
             * @return the specified @c TimeZone, or the GMT zone if the given ID
             *         cannot be understood.
             */
            static TimeZone& of(ZoneId const& zoneId);

            /**
             * Converts this @c TimeZone object to a @c ZoneId.
             *
             * @return a @c ZoneId representing the same time zone as this
             *         @c TimeZone
             */
            virtual ZoneId& toZoneId() const;

            /**
             * Gets the available IDs according to the given time zone offset in milliseconds.
             *
             * @param rawOffset the given time zone GMT offset in milliseconds.
             * @return an array of IDs, where the time zone for that ID has
             * the specified GMT offset. For example, "America/Phoenix" and "America/Denver"
             * both have GMT-07:00, but differ in daylight saving behavior.
             * @see rawOffset()
             */
            static StringArray availableIDs(gint rawOffset);

            /**
             * Gets all the available IDs supported.
             * @return an array of IDs.
             */
            static StringArray availableIDs();

            /**
             * Gets the default @c TimeZone of the Java virtual machine. If the
             * cached default @c TimeZone is available, its clone is returned.
             * Otherwise, the method takes the following steps to determine the default
             * time zone.
             *
             * @return the default @c TimeZone
             * @see #setDefault(TimeZone)
             */
            static TimeZone& getDefault();

            /**
             * Sets the @c TimeZone that is returned by the @c getDefault
             * method. @c zone is cached.
             *
             * @param zone the new default @c TimeZone
             * @see getDefault
             * @see PropertyPermission
             */
            static void setDefault(TimeZone const& zone);

            /**
             * Returns true if this zone has the same rule and offset as another zone.
             * That is, if this zone differs only in ID, if at all.  Returns false
             * if the other zone is null.
             * @param other the @c TimeZone object to be compared with
             * @return true if the other zone is not null and is the same as this one,
             * with the possible exception of the ID
             */
            virtual gbool hasSameRules(TimeZone const& other) const;

            /**
             * Creates a copy of this @c TimeZone.
             *
             * @return a clone of this @c TimeZone
             */
            Object& clone() const override = 0;

        private:
            /**
             * Gets the raw GMT offset and the amount of daylight saving of this
             * time zone at the given time.
             * @param date the milliseconds (since January 1, 1970,
             * 00:00:00.000 GMT) at which the time zone offset and daylight
             * saving amount are found
             * @param offsets an array of int where the raw GMT offset
             * (offset[0]) and daylight saving amount (offset[1]) are stored,
             * or null if those values are not needed. The method assumes that
             * the length of the given array is two or larger.
             * @return the total amount of the raw GMT offset and daylight
             * saving at the specified date.
             *
             * @see Calendar::ZONE_OFFSET
             * @see Calendar::DST_OFFSET
             */
            gint offsets(glong date, IntArray& offsets);

            static StringArray displayNames(String const& ID, util::Locale const& locale);

            ZoneId& toZoneID() const;

            static TimeZone& forID(String const& ID, gbool fallback);

            /**
             * Parses a custom time zone identifier and returns a corresponding zone.
             * This method doesn't support the RFC 822 time zone format. (e.g., +hhmm)
             *
             * @param id a string of the <a href="#CustomID">custom ID form</a>.
             * @return a newly created TimeZone with the given offset and
             * no daylight saving time, or null if the id cannot be parsed.
             */
            static util::Optional<TimeZone> parseID(String const& id);
        };
    } // time
} // core

#endif //CORE24_TIMEZONE_H
