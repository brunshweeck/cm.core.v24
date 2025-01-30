//
// Created by admin on 03/12/24.
//

#ifndef FORMATTER_H
#define FORMATTER_H

#include <core/StringArray.h>

#include "FormatParser.h"
#include "core/time/Temporal.h"


namespace core {
    using namespace util;
    using namespace time;

    namespace ext {
        class Formatter final : public Object {
        public:
            Locale const& locale;
            String const& src;
            FormatParser parser;
            XString out;
            // index of last argument referenced
            gint last = -1;
            // last ordinary index
            gint lastOrdinary = -1;
            gint argNumber = 0;
            gint fragment = -1;

            class DecimalFormatterSymbols final : public Object {
            public:
                gchar ZeroDigit;
                gchar GroupingSeparator;
                gchar DecimalSeparator;
                gchar Percent;
                String Infinity;
                String NaN;
                gchar Plus;
                gchar Minus;
                String Exponent;
                gint GroupingSize = -1;
                StringArray Days; // 7 items (long)
                StringArray AbbrDays; // 7 items (abbreviate)
                StringArray Months; // 12 items (long)
                StringArray AbbrMonths; // 12 items (abbreviate)
                StringArray AmPm; // 2 items
            } DFS;

            class Arg : public Object {
            public:
                Arg() = default;

                virtual Object& get() const = 0;

                ~Arg() override = default;
            };

            Array<Arg> args;
            gint lastArgIndex = -1;

            Formatter(Locale const& locale, String const& src, gint argNumber);

            /**
             * Gets the character used for zero. Different for Arabic, etc.
             *
             * @return the character used for zero
             */
            gchar getZeroDigit() const;

            /**
             * Gets the character used for grouping separator. Different for French, etc.
             *
             * @return the grouping separator
             */
            gchar getGroupingSeparator() const;

            /**
             * Gets the character used for decimal sign. Different for French, etc.
             *
             * @return the character used for decimal sign
             */
            gchar getDecimalSeparator() const;

            /**
             * Gets the character used for percent sign. Different for Arabic, etc.
             *
             * @return the character used for percent sign
             */
            gchar getPercent() const;

            /**
             * Gets the string used to represent infinity. Almost always left
             * unchanged.
             *
             * @return the string representing infinity
             */
            String getInfinity() const;

            /**
             * Gets the string used to represent "not a number". Almost always left
             * unchanged.
             *
             * @return the string representing "not a number"
             */
            String getNaN() const;

            /**
             * Gets the character used to represent minus sign. If no explicit
             * negative format is specified, one is formed by prefixing
             * minusSign to the positive format.
             *
             * @return the character representing minus sign
             */
            gchar getMinusSign() const;

            /**
             * Returns the string used to separate the mantissa from the exponent.
             * Examples: "x10^" for 1.23x10^4, "E" for 1.23E4.
             *
             * @return the exponent separator string
             * @see #setExponentSeparator(java.lang.String)
             */
            String getExponentSeparator() const;

            gint getGroupingSize() const;

            gbool isGroupingUsed() const;

            gbool formatArgs();

            String toString() const override;

            void addArg(Arg& arg);

            ~Formatter() override;

            StringArray getWeekdays() const;

            StringArray getShortWeekdays() const;

            StringArray getMonths() const;

            StringArray getShortMonths() const;

            StringArray getAmPmStrings() const;

        private:
            enum Request {
                EXPONENT_SYMBOL,
                INFINITY_SYMBOL,
                NAN_SYMBOL,
                MINUS_SIGN,
                PLUS_SIGN,
                PERCENT_SYMBOL,
                DIGITS,
                GROUPING_SEPARATOR,
                DECIMAL_SEPARATOR,
                DECIMAL_FORMAT,
                TIME_FORMAT,
                DATE_FORMAT,
                DATETIME_FORMAT,
                DISPLAY_MONTHS,
                DISPLAY_DAYS_OF_WEEK,
                DISPLAY_AM_PM,
            };

            static String query(gint LID, gint SID, gint RID, Request request,
                                Temporal::FormatStyle style = Temporal::FULL);

            static StringArray queryArray(gint LID, gint SID, gint RID, Request request,
                Temporal::FormatStyle style = Temporal::FULL);
        };
    } // ext
} // core

#endif //FORMATTER_H
