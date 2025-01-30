//
// Created by admin on 03/01/25.
//

#include "MonthDay.h"

#include <core/XString.h>
#include <core/time/ZoneId.h>
#include <core/util/Optional.h>

namespace core {
    using namespace util;

    namespace time {
        MonthDay::MonthDay(LocalDate::Month month, gint dayOfMonth) {
            try {
                checkValue(month, MONTH_OF_YEAR);
                checkValue(dayOfMonth, DAY_OF_MONTH);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            md = month << MONTH_OFFSET & MONTH_MASK | dayOfMonth << DAY_OFFSET & DAY_MASK;
        }

        MonthDay::MonthDay(gint month, gint dayOfMonth) {
            try {
                checkValue(month, MONTH_OF_YEAR);
                checkValue(dayOfMonth, DAY_OF_MONTH);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            md = month << MONTH_OFFSET & MONTH_MASK | dayOfMonth << DAY_OFFSET & DAY_MASK;
        }

        MonthDay MonthDay::from(Temporal const& temporal) {
            if (Class<MonthDay>::hasInstance(temporal))
                return CORE_XCAST(MonthDay const, temporal);

            try {
                LocalDate date = LocalDate::from(temporal);
                return MonthDay(date.month(), date.dayOfMonth());
            } catch (DateTimeException const& ex) {
                DateTimeException("Unable to obtain MonthDay from Temporal: " +
                                  temporal + " of type " + typeName(temporal), ex).throws($ftrace());
            }
        }

        gbool MonthDay::isSupported(ChronoField field) const {
            return field == MONTH_OF_YEAR || field == DAY_OF_MONTH;
        }

        gint MonthDay::get(ChronoField field) const {
            if (field == MONTH_OF_YEAR)
                return month();
            if (field == DAY_OF_MONTH)
                return dayOfMonth();
            TemporalException("Unsupported field " + Temporal::toString(field)).throws($ftrace());
        }

        glong MonthDay::getLong(ChronoField field) const {
            try { return get(field); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        LocalDate::Month MonthDay::month() const {
            return (LocalDate::Month) (md >> MONTH_OFFSET & MONTH_MASK);
        }

        gint MonthDay::dayOfMonth() const {
            return md >> DAY_OFFSET & DAY_MASK;
        }

        gbool MonthDay::isValidYear(gint year) const {
            return !(dayOfMonth() == 29 && month() == LocalDate::FEBRUARY && !LocalDate::isLeapYear(year));
        }

        MonthDay MonthDay::withMonth(gint month) const {
            try {
                return MonthDay(checkValue(month, MONTH_OF_YEAR), dayOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        MonthDay MonthDay::withMonth(LocalDate::Month month) const {
            try {
                return MonthDay(checkValue(month, MONTH_OF_YEAR), dayOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        MonthDay MonthDay::withDayOfMonth(gint dayOfMonth) const {
            try {
                return MonthDay(month(), checkValue(dayOfMonth, DAY_OF_MONTH));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Optional<> MonthDay::query(TemporalQuery const& query) const {
            return Optional<>();
        }

        LocalDate MonthDay::atYear(gint year) const {
            try {
                return LocalDate(checkValue(year, YEAR), month(), dayOfMonth());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint MonthDay::compareTo(MonthDay const& other) const {
            gint r = month() - other.month();
            if (r != 0) return r;
            return dayOfMonth() - other.dayOfMonth();
        }

        gbool MonthDay::isAfter(MonthDay const& other) const {
            return compareTo(other) > 0;
        }

        gbool MonthDay::isBefore(MonthDay const& other) const {
            return compareTo(other) < 0;
        }

        gbool MonthDay::equals(Object const& other) const {
            return this == &other ||
                    Class<MonthDay>::hasInstance(other) && compareTo(CORE_XCAST(MonthDay const, other)) == 0;
        }

        gint MonthDay::hash() const {
            return md;
        }

        String MonthDay::toString() const {
            gint m = month();
            gint dom = dayOfMonth();
            return XString(10).append("--")
                              .append(m < 10 ? "0" : "").append(m)
                              .append(dom < 10 ? "0" : "").append(dom)
                              .toString();
        }

        Object& MonthDay::clone() const {
            try {
                return UNSAFE::newInstance<MonthDay>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // time
} // core
