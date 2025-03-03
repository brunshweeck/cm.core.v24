//
// Created by admin on 03/12/24.
//

#include "Formatter.h"

#include <core/lang/StringArray.h>
#include <core/util/Locale.h>
#include <meta/ext/MissingFormatArgumentException.h>
#include <meta/util/cldr/Cldr.h>
#include <meta/util/cldr/LikelySubtag.h>

namespace core {
    using namespace time;
    using namespace util;

    using locales::Cldr;
    using locales::LikelySubtag;

    namespace ext {
        Formatter::Formatter(Locale const& locale, String const& src, gint argNumber)
            : locale(locale), src(src), argNumber(argNumber) {
            String l = locale.language();
            String s = locale.script();
            String r = locale.country();

            // ReSharper disable once CppDeprecatedEntity
            CORE_IGNORE_DEPRECATIONS(args = Array<Arg>(argNumber + 1);)

            gint LID = Cldr::languageToID(l);
            gint SID = Cldr::scriptToID(s);
            gint RID = Cldr::regionToID(r);

            if (LID < 0 || SID < 0 || RID < 0) {
                // Resolve invalid ids
                IntArray IDs = LikelySubtag::resolve(LID, SID, RID);
                IntArray pIDs = Cldr::parentLocale(LID, SID, RID);
                if (IDs.length() >= 3 && (LID >= 0 || SID >= 0 || RID >= 0)) {
                    LID = IDs[0];
                    SID = IDs[1];
                    RID = IDs[2];
                } else if (!pIDs.isEmpty()) {
                    LID = pIDs[0];
                    SID = pIDs[1];
                    RID = pIDs[2];
                }
            }

            String digits = query(LID, SID, RID, DIGITS);
            DFS.ZeroDigit = digits.isEmpty() ? u'0' : digits.charAt(0);
            String groupSeparator = query(LID, SID, RID, GROUPING_SEPARATOR);
            DFS.GroupingSeparator = groupSeparator.isEmpty() ? u',' : groupSeparator.charAt(0);
            String decimalSeparator = query(LID, SID, RID, DECIMAL_SEPARATOR);
            DFS.DecimalSeparator = decimalSeparator.isEmpty() ? u'.' : decimalSeparator.charAt(0);
            String percentSymbol = query(LID, SID, RID, PERCENT_SYMBOL);
            DFS.Percent = percentSymbol.isEmpty() ? u'%' : percentSymbol.charAt(0);
            DFS.Infinity = query(LID, SID, RID, INFINITY_SYMBOL);
            if (DFS.Infinity.isEmpty()) DFS.Infinity = "infinity";
            DFS.NaN = query(LID, SID, RID, NAN_SYMBOL);
            if (DFS.NaN.isEmpty()) DFS.NaN = "nan";
            String plusSign = query(LID, SID, RID, PLUS_SIGN);
            DFS.Plus = plusSign.isEmpty() ? u'+' : plusSign.charAt(0);
            String minusSign = query(LID, SID, RID, MINUS_SIGN);
            DFS.Minus = minusSign.isEmpty() ? u'-' : minusSign.charAt(0);
            DFS.Exponent = query(LID, SID, RID, EXPONENT_SYMBOL);
            if (DFS.Exponent.isEmpty()) DFS.Exponent = "E";
            DFS.Days = queryArray(LID, SID, RID, DISPLAY_DAYS_OF_WEEK, FormatStyle::FULL);
            if (DFS.Days.isEmpty())
                DFS.Days = queryArray(LID, SID, RID, DISPLAY_DAYS_OF_WEEK, FormatStyle::LONG);
            gbool mod = false;
            for (gint i = 0; i < DFS.Days.length(); i++) {
                if (DFS.Days[i].isEmpty()) {
                    DFS.Days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
                    mod = true;
                    break;
                }
            }
            DFS.AbbrDays = queryArray(LID, SID, RID, DISPLAY_DAYS_OF_WEEK, FormatStyle::MEDIUM);
            if (DFS.AbbrDays.isEmpty())
                DFS.AbbrDays = queryArray(LID, SID, RID, DISPLAY_DAYS_OF_WEEK, FormatStyle::SHORT);
            if (!mod) {
                for (int i = 0; i < DFS.AbbrDays.length(); ++i) {
                    if (DFS.AbbrDays[i].isEmpty())
                        DFS.AbbrDays[i] = DFS.Days[i];
                }
            } else {
                for (int i = 0; i < DFS.AbbrDays.length(); ++i) {
                    if (DFS.AbbrDays[i].isEmpty()) {
                        DFS.AbbrDays = {"Mon", "Tues", "Wed", "Thur", "Fri", "Sat", "Sun"};
                        break;
                    }
                }
            }
            DFS.Months = queryArray(LID, SID, RID, DISPLAY_MONTHS, FormatStyle::FULL);
            if (DFS.Months.isEmpty())
                DFS.Months = queryArray(LID, SID, RID, DISPLAY_MONTHS, FormatStyle::LONG);
            mod = false;
            for (gint i = 0; i < DFS.Months.length(); i++) {
                if (DFS.Months[i].isEmpty()) {
                    DFS.Months = {
                        "January", "February", "March", "April", "May", "June", "July", "August", "September",
                        "October", "November", "December"
                    };
                    mod = true;
                    break;
                }
            }
            DFS.AbbrMonths = queryArray(LID, SID, RID, DISPLAY_MONTHS, FormatStyle::MEDIUM);
            if (DFS.Months.isEmpty())
                DFS.Months = queryArray(LID, SID, RID, DISPLAY_MONTHS, FormatStyle::SHORT);
            if (!mod) {
                for (int i = 0; i < DFS.AbbrMonths.length(); ++i) {
                    if (DFS.AbbrMonths[i].isEmpty()) {
                        DFS.AbbrMonths[i] = DFS.Months[i];
                    }
                }
            } else {
                for (int i = 0; i < DFS.AbbrMonths.length(); ++i) {
                    if (DFS.AbbrMonths[i].isEmpty()) {
                        DFS.AbbrMonths = {
                            "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
                        };
                        break;
                    }
                }
            }
            // [midnight, am, noon, pm, morning-start, morning-end, afternoon-start,
            // afternoon-end, evening-start, evening-end, night-start, night-end]
            DFS.AmPm = queryArray(LID, SID, RID, DISPLAY_AM_PM, FormatStyle::FULL);
            if (DFS.AmPm.isEmpty() || DFS.AmPm[1].isEmpty() || DFS.AmPm[3].isEmpty())
                DFS.AmPm = queryArray(LID, SID, RID, DISPLAY_AM_PM, FormatStyle::LONG);
            DFS.AmPm = StringArray::of(DFS.AmPm[1], DFS.AmPm[3]);
            if (DFS.AmPm[0].isEmpty()) DFS.AmPm[0] = "AM"_Sl;
            if (DFS.AmPm[1].isEmpty()) DFS.AmPm[1] = "PM"_Sl;
        }

        gchar Formatter::getZeroDigit() const {
            return DFS.ZeroDigit;
        }

        gchar Formatter::getGroupingSeparator() const {
            return DFS.GroupingSeparator;
        }

        gchar Formatter::getDecimalSeparator() const {
            return DFS.DecimalSeparator;
        }

        gchar Formatter::getPercent() const {
            return DFS.Percent;
        }

        String Formatter::getInfinity() const {
            return DFS.Infinity;
        }

        String Formatter::getNaN() const {
            return DFS.NaN;
        }

        gchar Formatter::getMinusSign() const {
            return DFS.Minus;
        }

        String Formatter::getExponentSeparator() const {
            return DFS.Exponent;
        }

        gint Formatter::getGroupingSize() const {
            return DFS.GroupingSize;
        }

        gbool Formatter::isGroupingUsed() const {
            return DFS.GroupingSize > 0;
        }

        gbool Formatter::formatArgs() {
            if (!parser.isComplete())
                try { parser.parse(src); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gbool result = true;

            gint limit = parser.fragments.size();
            while (++fragment < limit) {
                FormatFragment& fs = parser.fragments.get(fragment);
                gint index = fs.index();
                switch (index) {
                    case -2: {
                        // fixed string, "%n", or "%%"
                        try { fs.apply(*this, null); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        break;
                    }
                    case -1: { // relative index
                        if (last < 0 || last > argNumber - 1)
                            MissingFormatArgumentException(fs.toString()).throws($ftrace());
                        try {
                            Arg& arg = args[last];
                            result &= fs.apply(*this, arg.get());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        break;
                    }
                    case 0: { // ordinary index
                        lastOrdinary++;
                        last = lastOrdinary;
                        if (lastOrdinary > argNumber - 1)
                            MissingFormatArgumentException(fs.toString()).throws($ftrace());
                        try {
                            Arg& arg = args[lastOrdinary];
                            result &= fs.apply(*this, arg.get());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        break;
                    }
                    default: {
                        last = index - 1;
                        if (lastOrdinary > argNumber - 1)
                            MissingFormatArgumentException(fs.toString()).throws($ftrace());

                        try {
                            Arg& arg = args[last];
                            result &= fs.apply(*this, arg.get());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        break;
                    }
                }
            }
            return result;
        }

        String Formatter::toString() const {
            return out.toString();
        }

        void Formatter::addArg(Arg& arg) {
            gint index = lastArgIndex + 1;
            if (index >= argNumber) {
                args = Arrays::copyOf(args, argNumber << 1);
            }
            Arrays::fastSet(args, index, arg);
            lastArgIndex = index;
        }

        Formatter::~Formatter() {
            for (int i = 0; i < args.length(); ++i) {
                Object& arg = args.getOrNull(i);
                if (arg == null)
                    continue;
                delete &arg;
            }
        }

        StringArray Formatter::getWeekdays() const {
            return DFS.Days;
        }

        StringArray Formatter::getShortWeekdays() const {
            return DFS.AbbrDays;
        }

        StringArray Formatter::getMonths() const {
            return DFS.Months;
        }

        StringArray Formatter::getShortMonths() const {
            return DFS.AbbrMonths;
        }

        StringArray Formatter::getAmPmStrings() const {
            return DFS.AmPm;
        }

        String Formatter::query(gint LID, gint SID, gint RID, Request request, FormatStyle style) {
            gbool L = LID >= 0, S = SID >= 0, R = RID >= 0;
            Cldr& cldr = Cldr::forLocale(LID, SID, RID, -1);
            S = cldr.hasScript();
            R = cldr.hasRegion();

            String result;
            switch (request) {
                case EXPONENT_SYMBOL:
                    result = cldr.exponential();
                    break;
                case INFINITY_SYMBOL:
                    result = cldr.infinity();
                    break;
                case NAN_SYMBOL:
                    result = cldr.nan();
                    break;
                case MINUS_SIGN:
                    result = cldr.minusSign();
                    break;
                case PLUS_SIGN:
                    result = cldr.plusSign();
                    break;
                case PERCENT_SYMBOL:
                    result = cldr.percent();
                    break;
                case DIGITS:
                    result = String(cldr.digits());
                    break;
                case GROUPING_SEPARATOR:
                    result = cldr.groupSeparator();
                    break;
                case DECIMAL_SEPARATOR:
                    result = cldr.decimalSeparator();
                    break;
                case DECIMAL_FORMAT:
                    result = cldr.decimalPattern();
                    break;
                case TIME_FORMAT:
                    switch (style) {
                        case FormatStyle::FULL:
                            result = cldr.timePattern(0);
                            if (result.isEmpty())
                                break;
                        case FormatStyle::LONG:
                            result = cldr.timePattern(1);
                            break;
                        case FormatStyle::MEDIUM:
                            result = cldr.timePattern(2);
                            break;
                        case FormatStyle::SHORT:
                            result = cldr.timePattern(3);
                            break;
                        default:
                            return String();
                    }
                    break;
                case DATE_FORMAT:
                    switch (style) {
                        case FormatStyle::FULL:
                            result = cldr.datePattern(0);
                            break;
                        case FormatStyle::LONG:
                            result = cldr.datePattern(1);
                            break;
                        case FormatStyle::MEDIUM:
                            result = cldr.datePattern(2);
                            break;
                        case FormatStyle::SHORT:
                            result = cldr.datePattern(3);
                            break;
                        default:
                            return String();
                    }
                    break;
                case DATETIME_FORMAT:
                    switch (style) {
                        case FormatStyle::FULL:
                            result = cldr.datetimePattern(0);
                            break;
                        case FormatStyle::LONG:
                            result = cldr.datetimePattern(1);
                            break;
                        case FormatStyle::MEDIUM:
                            result = cldr.datetimePattern(2);
                            break;
                        case FormatStyle::SHORT:
                            result = cldr.datetimePattern(3);
                            break;
                        default:
                            return String();
                    }
                    break;
                default:
                    return String();
            }

            if (result.isEmpty()) {
                if (L && S || L && R)
                    return query(LID, S ? -1 : SID, R ? -1 : RID, request, style);
                return query(-1, -1, -1, request, style);
            }

            return result;
        }

        StringArray Formatter::queryArray(gint LID, gint SID, gint RID, Request request, FormatStyle style) {
            gbool L = LID >= 0, S = SID >= 0, R = RID >= 0;
            Cldr& cldr = Cldr::forLocale(LID, SID, RID, -1);
            S = cldr.hasScript();
            R = cldr.hasRegion();

            StringArray result;
            switch (request) {
                case DISPLAY_MONTHS:
                    switch (style) {
                        case FormatStyle::FULL:
                        case FormatStyle::LONG:
                            result = cldr.displayMonths(0, false);
                            if (result.isEmpty())
                                result = cldr.displayMonths(0, true);
                            break;
                        case FormatStyle::MEDIUM:
                            result = cldr.displayMonths(1, false);
                            if (result.isEmpty())
                                result = cldr.displayMonths(1, true);
                            break;
                        case FormatStyle::SHORT:
                            result = cldr.displayMonths(2, false);
                            if (result.isEmpty())
                                result = cldr.displayMonths(2, true);
                            break;
                        default:
                            return StringArray();
                    }
                    break;
                case DISPLAY_DAYS_OF_WEEK:
                    switch (style) {
                        case FormatStyle::FULL:
                        case FormatStyle::LONG:
                            result = cldr.displayDays(0, false);
                            if (result.isEmpty())
                                result = cldr.displayDays(0, true);
                            break;
                        case FormatStyle::MEDIUM:
                            result = cldr.displayDays(1, false);
                            if (result.isEmpty())
                                result = cldr.displayDays(1, true);
                            break;
                        case FormatStyle::SHORT:
                            result = cldr.displayDays(2, false);
                            if (result.isEmpty())
                                result = cldr.displayDays(2, true);
                            break;
                        default:
                            return StringArray();
                    }
                    break;
                case DISPLAY_AM_PM:
                    switch (style) {
                        case FormatStyle::FULL:
                        case FormatStyle::LONG:
                            result = cldr.displayDayPeriods(0, false);
                            if (result.isEmpty())
                                result = cldr.displayDayPeriods(0, true);
                            break;
                        case FormatStyle::MEDIUM:
                            result = cldr.displayDayPeriods(1, false);
                            if (result.isEmpty())
                                result = cldr.displayDayPeriods(1, true);
                            break;
                        case FormatStyle::SHORT:
                            result = cldr.displayDayPeriods(2, false);
                            if (result.isEmpty())
                                result = cldr.displayDayPeriods(2, true);
                            break;
                        default:
                            return StringArray();
                    }
                    break;
                default:
                    return StringArray();
            }

            if (result.isEmpty()) {
                if (L && S || L && R)
                    return queryArray(LID, S ? -1 : SID, R ? -1 : RID, request, style);

                if (L)
                    return queryArray(-1, -1, -1, request, style);
            }

            if (result.isEmpty()) {
                switch (request) {
                    case DISPLAY_MONTHS: return StringArray(12);
                    case DISPLAY_DAYS_OF_WEEK: return StringArray(7);
                    case DISPLAY_AM_PM: return StringArray(10);
                    default: break;
                }
                return StringArray();
            }

            return result;
        }
    } // ext
} // core
