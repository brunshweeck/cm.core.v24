//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {
    /**
     * <p>
     * The main API for dates, times, instants, and durations.
     * </p>
     * <p>
     * The classes defined here represent the principle date-time concepts,
     * including instants, durations, dates, times, time-zones and periods.
     * They are based on the ISO calendar system, which is the <i>de facto</i> world
     * calendar following the proleptic Gregorian rules.
     * All the classes are immutable and thread-safe.
     * </p>
     * <p>
     * Each date time instance is composed of fields that are conveniently
     * made available by the APIs.  For lower level access to the fields refer
     * to the @c time::temporal package.
     * Each class includes support for printing and parsing all manner of dates and times.
     * Refer to the @c time::format package for customization options.
     * </p>
     * <p>
     * The @c time::chrono package contains the calendar neutral API
     * @b ChronoLocalDate,
     * @b ChronoLocalDateTime,
     * @b ChronoZonedDateTime and
     * @b Era.
     * This is intended for use by applications that need to use localized calendars.
     * It is recommended that applications use the ISO-8601 date and time classes from
     * this package across system boundaries, such as to the database or across the network.
     * The calendar neutral API should be reserved for interactions with users.
     * </p>
     *
     * <h3>Dates and Times</h3>
     * <p>
     * @b time::LocalDate stores a date without a time.
     * This stores a date like '2010-12-03' and could be used to store a birthday.
     * </p>
     * <p>
     * @b time::LocalTime stores a time without a date.
     * This stores a time like '11:30' and could be used to store an opening or closing time.
     * </p>
     * <p>
     * @b time::LocalDateTime stores a date and time.
     * This stores a date-time like '2010-12-03T11:30'.
     * </p>
     * <p>
     * @b time::ZonedDateTime stores a date and time with a time-zone.
     * This is useful if you want to perform accurate calculations of
     * dates and times taking into account the @b time::ZoneId, such as 'Europe/Paris'.
     * Where possible, it is recommended to use a simpler class without a time-zone.
     * The widespread use of time-zones tends to add considerable complexity to an application.
     * </p>
     *
     * <h3>Duration and Period</h3>
     * <p>
     * Beyond dates and times, the API also allows the storage of periods and durations of time.
     * A @b time::Duration is a simple measure of time along the time-line in nanoseconds.
     * A @b time::Period expresses an amount of time in units meaningful
     * to humans, such as years or days.
     * </p>
     *
     * <h3>Additional value types</h3>
     * <p>
     * @b time::Month stores a month on its own.
     * This stores a single month-of-year in isolation, such as 'DECEMBER'.
     * </p>
     * <p>
     * @b time::DayOfWeek stores a day-of-week on its own.
     * This stores a single day-of-week in isolation, such as 'TUESDAY'.
     * </p>
     * <p>
     * @b time::Year stores a year on its own.
     * This stores a single year in isolation, such as '2010'.
     * </p>
     * <p>
     * @b time::YearMonth stores a year and month without a day or time.
     * This stores a year and month, such as '2010-12' and could be used for a credit card expiry.
     * </p>
     * <p>
     * @b time::MonthDay stores a month and day without a year or time.
     * This stores a month and day-of-month, such as '--12-03' and
     * could be used to store an annual event like a birthday without storing the year.
     * </p>
     * <p>
     * @b time::OffsetTime stores a time and offset from UTC without a date.
     * This stores a date like '11:30+01:00'.
     * The @b ZoneOffset is of the form '+01:00'.
     * </p>
     * <p>
     * @b time::OffsetDateTime stores a date and time and offset from UTC.
     * This stores a date-time like '2010-12-03T11:30+01:00'.
     * This is sometimes found in XML messages and other forms of persistence,
     * but contains less information than a full time-zone.
     * </p>
     *
     * <h3>Package specification</h3>
     * <p>
     * All calculations should check for numeric overflow and throw either a @b core::ArithmeticException
     * or a @b time::DateTimeException.
     * </p>
     *
     * <h3>Design notes (non-normative)</h3>
     * <p>
     * The API has been designed to reject null early and to be clear about this behavior.
     * A key exception is any method that takes an object and returns a boolean, for the purpose
     * of checking or validating, will generally return false for null.
     * </p>
     * <p>
     * The API is designed to be type-safe where reasonable in the main high-level API.
     * Thus, there are separate classes for the distinct concepts of date, time and date-time,
     * plus variants for offset and time-zone.
     * This can seem like a lot of classes, but most applications can begin with just five date/time types.
     *
     * - @b time::Instant - a timestamp <br/>
     * - @b time::LocalDate - a date without a time, or any reference to an offset or time-zone <br/>
     * - @b time::LocalTime - a time without a date, or any reference to an offset or time-zone <br/>
     * - @b time::LocalDateTime - combines date and time, but still without any offset or time-zone <br/>
     * - @b time::ZonedDateTime - a "full" date-time with time-zone and resolved offset from UTC/Greenwich <br/>
     *
     * <p>
     * @c Instant is the closest equivalent class to @c java.util.Date.
     * @c ZonedDateTime is the closest equivalent class to @c java.util.GregorianCalendar.
     * </p>
     * <p>
     * Where possible, applications should use @c LocalDate, @c LocalTime and @c LocalDateTime
     * to better model the domain. For example, a birthday should be stored in a code @c LocalDate.
     * Bear in mind that any use of a @em time-zone, such as 'Europe/Paris', adds
     * considerable complexity to a calculation.
     * Many applications can be written only using @c LocalDate, @c LocalTime and @c Instant,
     * with the time-zone added at the user interface (UI) layer.
     * </p>
     * <p>
     * The offset-based date-time types @c OffsetTime and @c OffsetDateTime,
     * are intended primarily for use with network protocols and database access.
     * For example, most databases cannot automatically store a time-zone like 'Europe/Paris', but
     * they can store an offset like '+02:00'.
     * </p>
     * <p>
     * Classes are also provided for the most important sub-parts of a date, including @c Month,
     * @c DayOfWeek, @c Year, @c YearMonth and @c MonthDay.
     * These can be used to model more complex date-time concepts.
     * For example, @c YearMonth is useful for representing a credit card expiry.
     * </p>
     * <p>
     * Note that while there are a large number of classes representing different aspects of dates,
     * there are relatively few dealing with different aspects of time.
     * Following type-safety to its logical conclusion would have resulted in classes for
     * hour-minute, hour-minute-second and hour-minute-second-nanosecond.
     * While logically pure, this was not a practical option as it would have almost tripled the
     * number of classes due to the combinations of date and time.
     * Thus, @c LocalTime is used for all precisions of time, with zeroes used to imply lower precision.
     * </p>
     * <p>
     * Following full type-safety to its ultimate conclusion might also argue for a separate class
     * for each field in date-time, such as a class for HourOfDay and another for DayOfMonth.
     * This approach was tried, but was excessively complicated in the Java language, lacking usability.
     * A similar problem occurs with periods.
     * There is a case for a separate class for each period unit, such as a type for Years and a type for Minutes.
     * However, this yields a lot of classes and a problem of type conversion.
     * Thus, the set of date-time types provided is a compromise between purity and practicality.
     * </p>
     * <p>
     * The API has a relatively large surface area in terms of number of methods.
     * This is made manageable through the use of consistent method prefixes.
     *
     * - @c of - static factory method <br/>
     * - @c parse - static factory method focused on parsing <br/>
     * - @c get - gets the value of something <br/>
     * - @c is - checks if something is true <br/>
     * - @c with - the immutable equivalent of a setter <br/>
     * - @c plus - adds an amount to an object <br/>
     * - @c minus - subtracts an amount from an object <br/>
     * - @c to - converts this object to another type <br/>
     * - @c at - combines this object with another, such as @c date.atTime(time) <br/>
     *
     * <p>
     * Multiple calendar systems is an awkward addition to the design challenges.
     * The first principle is that most users want the standard ISO calendar system.
     * As such, the main classes are ISO-only. The second principle is that most of those that want a
     * non-ISO calendar system want it for user interaction, thus it is a UI localization issue.
     * As such, date and time objects should be held as ISO objects in the data model and persistent
     * storage, only being converted to and from a local calendar for display.
     * The calendar system would be stored separately in the user preferences.
     * </p>
     * <p>
     * There are, however, some limited use cases where users believe they need to store and use
     * dates in arbitrary calendar systems throughout the application.
     * This is supported by @b time::chrono::ChronoLocalDate, however it is vital to read
     * all the associated warnings in the Javadoc of that interface before using it.
     * In summary, applications that require general interoperation between multiple calendar systems
     * typically need to be written in a very different way to those only using the ISO calendar,
     * thus most applications should just use ISO and avoid @c ChronoLocalDate.
     * </p>
     * <p>
     * The API is also designed for user extensibility, as there are many ways of calculating time.
     * The @em field and @em unit API, accessed via @b TemporalAccessor and
     * @b Temporal provide considerable flexibility to applications.
     * In addition, the @b TemporalQuery and
     * @b TemporalAdjuster interfaces provide day-to-day
     * power, allowing code to read close to business requirements:
     * </p>
     * @code
     *   LocalDate customerBirthday = customer.loadBirthdayFromDatabase();
     *   LocalDate today = LocalDate::now();
     *   if (customerBirthday.equals(today)) {
     *     LocalDate specialOfferExpiryDate = today.plusWeeks(2).withDayOfWeek(next(FRIDAY));
     *     customer.sendBirthdaySpecialOffer(specialOfferExpiryDate);
     *   }
     *
     * @endcode
     */
    namespace time {}
}

#endif //NAMESPACE_DOC_H
