//
// Created by admin on 03/12/24.
//

#include "FormatParser.h"

#include <core/Complex.h>
#include <core/NumberFormatException.h>
#include <core/regex/Matcher.h>
#include <core/time/LocalDateTime.h>
#include <core/util/Optional.h>
#include <meta/DoubleConsts.h>

#include "DecimalFormatter.h"
#include "DuplicateFormatFlagsException.h"
#include "Formatter.h"
#include "IllegalFormatArgumentIndexException.h"
#include "IllegalFormatCodePointException.h"
#include "IllegalFormatFlagsException.h"
#include "IllegalFormatPrecisionException.h"
#include "IllegalFormatWidthException.h"
#include "MissingFormatWidthException.h"
#include "UnknownFormatConversionException.h"
#include "UnknownFormatFlagsException.h"
#include "core/time/TemporalQuery.h"
#include "core/time/ZoneOffset.h"
#include "core/time/ZoneRules.h"

namespace core {
    using namespace util;
    using namespace time;
    using namespace text;
    using namespace atomic;

    namespace ext {
        gbool Flags::contains(gint flags, gint f) {
            return (flags & f) == f;
        }

        gbool Flags::containsAny(gint flags, gint f) {
            return (flags & f) != 0;
        }

        gint Flags::add(gint flags, gint f) {
            return flags | f;
        }

        gint Flags::remove(gint flags, gint f) {
            return flags & ~f;
        }

        gint Flags::parse(String const& s, gint start, gint end) {
            gint f = 0;
            for (gint i = start; i < end; i++) {
                gchar c = s.charAt(i);
                gint v = parse(c);
                if (contains(f, v))
                    DuplicateFormatFlagsException(toString(v)).throws($ftrace());
                f = add(f, v);
            }
            return f;
        }

        String Flags::toString(gint f) {
            XString sb = XString();
            if (contains(f, LEFT_JUSTIFY)) sb.append('-');
            if (contains(f, UPPERCASE)) sb.append('^');
            if (contains(f, ALTERNATE)) sb.append('#');
            if (contains(f, PLUS)) sb.append('+');
            if (contains(f, LEADING_SPACE)) sb.append(' ');
            if (contains(f, ZERO_PAD)) sb.append('0');
            if (contains(f, GROUP)) sb.append(',');
            if (contains(f, PARENTHESES)) sb.append('(');
            if (contains(f, PREVIOUS)) sb.append('<');
            return sb.toString();
        }

        gint Flags::parse(gchar c) {
            switch (c) {
                case ' ': return LEADING_SPACE;
                case '#': return ALTERNATE;
                case '(': return PARENTHESES;
                case '+': return PLUS;
                case ',': return GROUP;
                case '-': return LEFT_JUSTIFY;
                case '0': return ZERO_PAD;
                case '<': return PREVIOUS;
                default: UnknownFormatFlagsException(String::valueOf(c)).throws($ftrace());
            }
        }

        Object& FormatFragment::clone() const {
            return (FormatFragment&) *this;
        }

        FixedFragment::FixedFragment(String const& str, gint start, gint end): str(str),
                                                                               start(start),
                                                                               end(end) {}

        gint FixedFragment::index() const {
            return -2;
        }

        gbool FixedFragment::apply(Formatter& fmt, Object const& arg) {
            fmt.out.append(str, start, end);
            return arg == null; // arg is not used
        }

        String FixedFragment::toString() const {
            return str.subString(start, end);
        }

        SpecialFragment::SpecialFragment(gchar conv) {
            c = conv;
            if (Character::isUpperCase(conv)) {
                f = Flags::UPPERCASE;
                c = Character::toLowerCase(conv);
            }
            if (Conversion::isText(conv)) {
                i = -2;
            }
        }

        SpecialFragment::SpecialFragment(String const& s, regex::Matcher const& m) {
            try { index(s, m.start(1), m.end(1)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            try { flags(s, m.start(2), m.end(2)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            try { width(s, m.start(3), m.end(3)); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            try { precision(s, m.start(4), m.end(4)); } catch (Throwable const& ex) { ex.throws($ftrace()); }

            gint tTStart = m.start(5);
            if (tTStart >= 0) {
                dt = true;
                if (s.charAt(tTStart) == 'T') {
                    f = Flags::add(f, Flags::UPPERCASE);
                }
            }
            conversion(s.charAt(m.start(6)));

            if (dt)
                checkDate();
            else if (Conversion::isGeneral(c))
                try { checkGeneral(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            else if (Conversion::isCharacter(c))
                try { checkCharacter(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            else if (Conversion::isInteger(c))
                try { checkInteger(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            else if (Conversion::isFloat(c))
                try { checkFloat(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            else if (Conversion::isText(c))
                try { checkText(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            else
                UnknownFormatConversionException(String::valueOf(c)).throws($ftrace());
        }

        gint SpecialFragment::index() const { return i; }

        gbool SpecialFragment::apply(Formatter& fmt, Object const& arg) {
            if (dt)
                try { return applyDate(fmt, arg); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            switch (c) {
                case Conversion::DECIMAL_INTEGER:
                case Conversion::OCTAL_INTEGER:
                case Conversion::HEXADECIMAL_INTEGER:
                    try { return applyInteger(fmt, arg); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case Conversion::SCIENTIFIC:
                case Conversion::GENERAL:
                case Conversion::DECIMAL_FLOAT:
                case Conversion::HEXADECIMAL_FLOAT:
                    try { return applyFloat(fmt, arg); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case Conversion::CHARACTER:
                    try { return applyCharacter(fmt, arg); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case Conversion::BOOLEAN:
                    try { return applyBoolean(fmt, arg); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case Conversion::STRING:
                    try { return applyString(fmt, arg); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case Conversion::HASHCODE:
                    try { return applyHash(fmt, arg); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case Conversion::LINE_SEPARATOR:
                    try {
                        fmt.out.append('\n');
                        break;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                case Conversion::PERCENT_SIGN:
                    try { return apply(fmt, "%"); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                default:
                    CORE_ASSERT(false);
            }
            return true;
        }

        String SpecialFragment::toString() const {
            XString sb = XString("%"_Sl);
            // Flags::UPPERCASE is set internally for legal conversions.
            sb.append(Flags::toString(Flags::remove(f, Flags::UPPERCASE)));
            if (i > 0)
                sb.append(i).append('$');
            if (w != -1)
                sb.append(w);
            if (p != -1)
                sb.append('.').append(p);
            if (dt)
                sb.append(Flags::contains(f, Flags::UPPERCASE) ? 'T' : 't');
            sb.append(Flags::contains(f, Flags::UPPERCASE)
                          ? Character::toUpperCase(c)
                          : c);
            return sb.toString();
        }

        void SpecialFragment::index(String const& str, gint start, gint end) {
            if (start >= 0) {
                try {
                    // skip the trailing '$'
                    i = Integer::parseInt(str, start, end - 1, 10);
                    if (i <= 0) {
                        IllegalFormatArgumentIndexException(i).throws($ftrace());
                    }
                } catch (NumberFormatException const& _) {
                    IllegalFormatArgumentIndexException(Integer::MIN_VALUE).throws($ftrace());
                }
            }
        }

        void SpecialFragment::flags(String const& str, gint start, gint end) {
            f = Flags::parse(str, start, end);
            if (Flags::contains(f, Flags::PREVIOUS))
                i = -1;
        }

        void SpecialFragment::width(String const& str, gint start, gint end) {
            if (start >= 0) {
                try {
                    w = Integer::parseInt(str, start, end, 10);
                    if (w < 0)
                        IllegalFormatWidthException(w).throws($ftrace());
                } catch (NumberFormatException const& _) {
                    IllegalFormatWidthException(Integer::MIN_VALUE).throws($ftrace());
                }
            }
        }

        void SpecialFragment::precision(String const& str, gint start, gint end) {
            if (start >= 0) {
                try {
                    // skip the leading '.'
                    p = Integer::parseInt(str, start + 1, end, 10);
                    if (p < 0)
                        IllegalFormatPrecisionException(p).throws($ftrace());
                } catch (NumberFormatException const& _) {
                    IllegalFormatPrecisionException(Integer::MIN_VALUE).throws($ftrace());
                }
            }
        }

        void SpecialFragment::conversion(gchar conv) {
            c = conv;
            if (!dt) {
                if (!Conversion::isValid(c)) {
                    UnknownFormatConversionException(String::valueOf(c)).throws($ftrace());
                }
                if (Character::isUpperCase(c)) {
                    f = Flags::add(f, Flags::UPPERCASE);
                    c = Character::toLowerCase(c);
                }
                if (Conversion::isText(c)) {
                    i = -2;
                }
            }
        }

        // gbool SpecialFragment::check() {}

        void SpecialFragment::checkGeneral() const {
            if ((c == Conversion::BOOLEAN || c == Conversion::HASHCODE) && Flags::contains(f, Flags::ALTERNATE))
                failMismatch(Flags::ALTERNATE, c).throws($ftrace());
            // '-' requires a width
            if (w == -1 && Flags::contains(f, Flags::LEFT_JUSTIFY))
                MissingFormatWidthException(toString()).throws($ftrace());
            try {
                checkBadFlags(Flags::PLUS | Flags::LEADING_SPACE | Flags::ZERO_PAD | Flags::GROUP | Flags::PARENTHESES);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void SpecialFragment::checkDate() const {
            if (p != -1)
                IllegalFormatPrecisionException(p).throws($ftrace());
            if (!DateTime::isValid(c))
                UnknownFormatConversionException("t"_S + c).throws($ftrace());
            try {
                checkBadFlags(Flags::ALTERNATE | Flags::PLUS | Flags::LEADING_SPACE |
                    Flags::ZERO_PAD | Flags::GROUP | Flags::PARENTHESES);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            // '-' requires a width
            if (w == -1 && Flags::contains(f, Flags::LEFT_JUSTIFY)) {
                MissingFormatWidthException(toString()).throws($ftrace());
            }
        }

        void SpecialFragment::checkCharacter() const {
            if (p != -1)
                IllegalFormatPrecisionException(p).throws($ftrace());
            try {
                checkBadFlags(Flags::ALTERNATE | Flags::PLUS | Flags::LEADING_SPACE |
                    Flags::ZERO_PAD | Flags::GROUP | Flags::PARENTHESES);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            // '-' requires a width
            if (w == -1 && Flags::contains(f, Flags::LEFT_JUSTIFY)) {
                MissingFormatWidthException(toString()).throws($ftrace());
            }
        }

        void SpecialFragment::checkInteger() const {
            try { checkNumeric(); } catch (Throwable const& ex) { ex.throws($ftrace()); }

            if (p != -1) {
                IllegalFormatPrecisionException(p).throws($ftrace());
            }

            if (c == Conversion::DECIMAL_INTEGER)
                try { return checkBadFlags(Flags::ALTERNATE); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (c == Conversion::OCTAL_INTEGER)
                try { return checkBadFlags(Flags::GROUP); } catch (Throwable const& ex) { ex.throws($ftrace()); }

            try { return checkBadFlags(Flags::GROUP); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void SpecialFragment::checkFloat() const {
            try { checkNumeric(); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            if (c == Conversion::DECIMAL_FLOAT) {} else if (c == Conversion::HEXADECIMAL_FLOAT) {
                try {
                    return checkBadFlags(Flags::PARENTHESES | Flags::GROUP);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            } else if (c == Conversion::SCIENTIFIC) {
                try { return checkBadFlags(Flags::GROUP); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            } else if (c == Conversion::GENERAL) {
                try { checkBadFlags(Flags::ALTERNATE); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        }

        void SpecialFragment::checkNumeric() const {
            if (w != -1 && w < 0) {
                IllegalFormatWidthException(w).throws($ftrace());
            }

            if (p != -1 && p < 0) {
                IllegalFormatPrecisionException(p).throws($ftrace());
            }

            // '-' and '0' require a width
            if (w == -1 && (Flags::containsAny(f, Flags::LEFT_JUSTIFY | Flags::ZERO_PAD))) {
                MissingFormatWidthException(toString()).throws($ftrace());
            }

            // bad combination
            if ((Flags::contains(f, Flags::PLUS | Flags::LEADING_SPACE))
                || (Flags::contains(f, Flags::LEFT_JUSTIFY | Flags::ZERO_PAD))) {
                IllegalFormatFlagsException(Flags::toString(f)).throws($ftrace());
            }
        }

        void SpecialFragment::checkText() const {
            if (p != -1)
                IllegalFormatPrecisionException(p).throws($ftrace());
            switch (c) {
                case Conversion::PERCENT_SIGN:
                    if (f != Flags::LEFT_JUSTIFY && f != Flags::NONE)
                        IllegalFormatFlagsException(Flags::toString(f)).throws($ftrace());
                // '-' requires a width
                    if (w == -1 && Flags::contains(f, Flags::LEFT_JUSTIFY)) {
                        MissingFormatWidthException(toString()).throws($ftrace());
                    }
                    break;
                case Conversion::LINE_SEPARATOR:
                    if (w != -1) {
                        IllegalFormatWidthException(w).throws($ftrace());
                    }
                    if (f != Flags::NONE) {
                        IllegalFormatFlagsException(Flags::toString(f)).throws($ftrace());
                    }
                    break;
                default:
                    CORE_ASSERT(false);
            }
        }

        void SpecialFragment::checkBadFlags(gint badFlags) const {
            if ((f & badFlags) != 0) {
                failMismatch(f & badFlags, c).throws($ftrace());
            }
        }

        gbool SpecialFragment::applyInteger(Formatter& fmt, Object const& arg) const {
            if (arg == null)
                return apply(fmt, "null");
            if (Class<Byte>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Byte const, arg).byteValue());
            if (Class<Short>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Short const, arg).shortValue());
            if (Class<Integer>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Integer const, arg).intValue());
            if (Class<Long>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Long const, arg).longValue());
            if (Class<LiteralInt>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(LiteralInt const, arg));
            failConversion(c, arg).throws($ftrace());
        }

        gbool SpecialFragment::applyFloat(Formatter& fmt, Object const& arg) const {
            if (arg == null)
                return apply(fmt, "null");
            if (Class<Float>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Float const, arg).floatValue());
            if (Class<Double>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Double const, arg).doubleValue());
            if (Class<LiteralFloat>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(LiteralFloat const, arg));
            if (Class<Byte>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Byte const, arg).doubleValue());
            if (Class<Short>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Short const, arg).doubleValue());
            if (Class<Integer>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Integer const, arg).doubleValue());
            if (Class<Long>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(Long const, arg).doubleValue());
            if (Class<LiteralInt>::hasInstance(arg))
                return apply(fmt, (gdouble) CORE_XCAST(LiteralInt const, arg).value);
            if (Class<Complex>::hasInstance(arg)) {
                return apply(fmt, CORE_XCAST(Complex const, arg));
            }

            failConversion(c, arg).throws($ftrace());
        }

        gbool SpecialFragment::applyCharacter(Formatter& fmt, Object const& arg) const {
            if (arg == null)
                return apply(fmt, "null");
            String s;
            if (Class<Character>::hasInstance(arg))
                s = CORE_XCAST(Character const, arg).toString();
            else if (Class<Byte>::hasInstance(arg)) {
                gbyte b = CORE_XCAST(Byte const, arg);
                s = String::valueOf(Character::toChars(b & 0xFF));
            } else if (Class<Short>::hasInstance(arg)) {
                gshort i = CORE_XCAST(Short const, arg);
                s = String::valueOf(Character::toChars(i & 0xFFFF));
            } else if (Class<Integer>::hasInstance(arg)) {
                gint i = CORE_XCAST(Integer const, arg);
                if (Character::isValidCodePoint(i))
                    s = String::valueOf(Character::toChars(i));
                else
                    IllegalFormatCodePointException(i).throws($ftrace());
            } else if (Class<Long>::hasInstance(arg)) {
                glong i = CORE_XCAST(Long const, arg);
                if (i >= 0 && i <= 0x10FFFF)
                    s = String::valueOf(Character::toChars((gint) i));
                else
                    failConversion(c, arg).throws($ftrace());
            } else if (Class<LiteralInt>::hasInstance(arg)) {
                glong i = CORE_XCAST(LiteralInt const, arg).value;
                if (i >= 0 && i <= 0x10FFFF)
                    s = String::valueOf(Character::toChars((gint) i));
                else
                    failConversion(c, arg).throws($ftrace());
            } else
                failConversion(c, arg).throws($ftrace());
            return apply(fmt, s);
        }

        gbool SpecialFragment::applyString(Formatter& fmt, Object const& arg) const {
            if (Flags::contains(f, Flags::ALTERNATE))
                failMismatch(Flags::ALTERNATE, 's');
            if (arg == null)
                return apply(fmt, "null");
            if (Class<String>::hasInstance(arg))
                return apply(fmt, CORE_XCAST(String const, arg));

            return apply(fmt, arg.toString());
        }

        gbool SpecialFragment::applyBoolean(Formatter& fmt, Object const& arg) const {
            String s;
            if (arg != null) {
                if (Class<Boolean>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(Boolean const, arg).booleanValue() ? "true" : "false");
                if (Class<Byte>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(Byte const, arg).byteValue() ? "true" : "false");
                if (Class<Short>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(Short const, arg).shortValue() ? "true" : "false");
                if (Class<Character>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(Character const, arg).charValue() ? "true" : "false");
                if (Class<Integer>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(Integer const, arg).intValue() ? "true" : "false");
                if (Class<Long>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(Long const, arg).longValue() ? "true" : "false");
                if (Class<Float>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(Float const, arg).floatValue() ? "true" : "false");
                if (Class<Double>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(Double const, arg).doubleValue() ? "true" : "false");
                if (Class<LiteralInt>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(LiteralInt const, arg).value ? "true" : "false");
                if (Class<LiteralFloat>::hasInstance(arg))
                    return apply(fmt, CORE_XCAST(LiteralFloat const, arg).value ? "true" : "false");
                failConversion(c, arg).throws($ftrace());
            }
            return apply(fmt, "false");
        }

        gbool SpecialFragment::applyDate(Formatter& fmt, Object const& arg) const {
            if (arg == null) {
                return apply(fmt, "null");
            }

            // Instead of Calendar.setLenient(true), perhaps we should
            // wrap the IllegalArgumentException that might be thrown?
            if (Class<Number>::hasInstance(arg)) {
                if (Class<Long>::hasInstance(arg)) {
                    // Note that the following method uses an instance of the
                    // default time zone (TimeZone.getDefaultRef()).
                    glong l = CORE_XCAST(Long const, arg).longValue();
                    LocalDateTime ldt = LocalDateTime::ofEpochSecond(
                        Math::floorDiv(l, LocalTime::MILLIS_PER_SECOND),
                        Math::floorMod(l, LocalTime::MILLIS_PER_SECOND) * LocalTime::NANOS_PER_MILLI);
                    return apply(fmt, ldt);
                }
                if (Class<Integer>::hasInstance(arg)) {
                    // Note that the following method uses an instance of the
                    // default time zone (TimeZone.getDefaultRef()).
                    glong l = CORE_XCAST(Integer const, arg).longValue();
                    LocalDateTime ldt = LocalDateTime::ofEpochSecond(
                        Math::floorDiv(l, LocalTime::MILLIS_PER_SECOND),
                        Math::floorMod(l, LocalTime::MILLIS_PER_SECOND) * LocalTime::NANOS_PER_MILLI);
                    return apply(fmt, ldt);
                }
                if (Class<Short>::hasInstance(arg)) {
                    // Note that the following method uses an instance of the
                    // default time zone (TimeZone.getDefaultRef()).
                    glong l = CORE_XCAST(Short const, arg).longValue();
                    LocalDateTime ldt = LocalDateTime::ofEpochSecond(
                        Math::floorDiv(l, LocalTime::MILLIS_PER_SECOND),
                        Math::floorMod(l, LocalTime::MILLIS_PER_SECOND) * LocalTime::NANOS_PER_MILLI);
                    try { return apply(fmt, ldt); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            } else if (Class<Temporal>::hasInstance(arg)) {
                try {
                    return apply(fmt, CORE_XCAST(Temporal const, arg));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
            failConversion(c, arg).throws($ftrace());
        }

        gbool SpecialFragment::applyHash(Formatter& fmt, Object const& arg) const {
            if (arg == null)
                return apply(fmt, "null");
            return apply(fmt, Integer::toHexString(arg.hash()));
        }

        gbool SpecialFragment::apply(Formatter& fmt, String const& arg) const {
            if (p != -1 && p < arg.length()) {
                if (Flags::contains(f, Flags::UPPERCASE))
                    justify(fmt.out, arg.subString(0, p).toUpperCase(fmt.locale));
                else
                    justify(fmt.out, arg.subString(0, p));
            } else if (Flags::contains(f, Flags::UPPERCASE))
                justify(fmt.out, arg.toUpperCase(fmt.locale));
            else
                justify(fmt.out, arg);
            return true;
        }

        gbool SpecialFragment::apply(Formatter& fmt, XString& sb, gdouble const& value,
                                     gint flag, gchar c, gint precision, gbool neg) const {
            if (c == Conversion::SCIENTIFIC) {
                // Create a new DecimalFormatter with the desired
                // precision.
                gint prec = (precision == -1 ? 6 : precision);

                DecimalFormatter fd = DecimalFormatter::valueOf(value, prec, DecimalFormatter::SCIENTIFIC);

                XString& mant = UNSAFE::newInstance<XString>();
                mant.append(fd.getMantissa());
                addZeros(mant, prec);

                // If the precision is zero and the '#' flag is set, add the
                // requested decimal point.
                if (Flags::contains(flag, Flags::ALTERNATE) && (prec == 0)) {
                    mant.append('.');
                }

                CharArray exp = (value == 0.0)
                                    ? CharArray({'+', '0', '0'})
                                    : fd.getExponent();

                gint newW = w;
                if (w != -1) {
                    newW = adjust(w - exp.length() - 1, flag, neg);
                }

                magnitude(fmt, sb, mant, 0, flag, newW);
                sb.append(Flags::contains(flag, Flags::UPPERCASE) ? 'E' : 'e');

                gchar sign = exp[0];
                CORE_ASSERT(sign == '+' || sign == '-');
                sb.append(sign);

                magnitudeEx(fmt, sb, exp, 1);

                UNSAFE::deleteInstance(mant);
            } else if (c == Conversion::DECIMAL_FLOAT) {
                // Create a new DecimalFormatter with the desired
                // precision.
                gint prec = (precision == -1 ? 6 : precision);

                DecimalFormatter fd = DecimalFormatter::valueOf(value, prec, DecimalFormatter::PLAIN);

                XString& mant = UNSAFE::newInstance<XString>();
                mant.append(fd.getMantissa());
                addZeros(mant, prec);

                // If the precision is zero and the '#' flag is set, add the
                // requested decimal point.
                if (Flags::contains(flag, Flags::ALTERNATE) && (prec == 0))
                    mant.append('.');

                gint newW = w;
                if (w != -1)
                    newW = adjust(w, flag, neg);
                magnitude(fmt, sb, mant, 0, flag, newW);

                UNSAFE::deleteInstance(mant);
            } else if (c == Conversion::GENERAL) {
                gint prec = precision;
                if (precision == -1)
                    prec = 6;
                else if (precision == 0)
                    prec = 1;

                CharArray exp;
                XString mant = XString();
                gint expRounded;
                if (value == 0.0) {
                    exp = { };
                    mant.append('0');
                    expRounded = 0;
                } else {
                    DecimalFormatter fd = DecimalFormatter::valueOf(value, prec, DecimalFormatter::GENERAL);
                    exp = fd.getExponent();
                    mant.append(fd.getMantissa());
                    expRounded = fd.getExponentRounded();
                }

                if (!exp.isEmpty()) {
                    prec -= 1;
                } else {
                    prec -= expRounded + 1;
                }

                addZeros(mant, prec);
                // If the precision is zero and the '#' flag is set, add the
                // requested decimal point.
                if (Flags::contains(flag, Flags::ALTERNATE) && (prec == 0)) {
                    mant.append('.');
                }

                gint newW = w;
                if (w != -1) {
                    if (!exp.isEmpty())
                        newW = adjust(w - exp.length() - 1, flag, neg);
                    else
                        newW = adjust(w, flag, neg);
                }
                magnitude(fmt, sb, mant, 0, flag, newW);

                if (!exp.isEmpty()) {
                    sb.append(Flags::contains(flag, Flags::UPPERCASE) ? 'E' : 'e');

                    gchar sign = exp[0];
                    CORE_ASSERT(sign == '+' || sign == '-');
                    sb.append(sign);

                    magnitudeEx(fmt, sb, exp, 1);
                }
            } else if (c == Conversion::HEXADECIMAL_FLOAT) {
                gint prec = precision;
                if (precision == -1)
                    // assume that we want all of the digits
                    prec = 0;
                else if (precision == 0)
                    prec = 1;

                String s = hex(value, prec);

                XString va = XString();
                gbool upper = Flags::contains(flag, Flags::UPPERCASE);
                sb.append(upper ? "0X" : "0x");

                if (Flags::contains(flag, Flags::ZERO_PAD)) {
                    gint leadingCharacters = 2;
                    if (Flags::contains(flag, Flags::LEADING_SPACE) ||
                        Flags::contains(flag, Flags::PLUS) || neg) {
                        leadingCharacters = 3;
                    }
                    trailingZeros(sb, w - s.length() - leadingCharacters);
                }

                gint idx = s.indexOf('p');
                if (idx == -1)
                    idx = s.indexOf('P');
                if (upper) {
                    String tmp = s.subString(0, idx);
                    // don't localize hex
                    tmp = tmp.toUpperCase(Locale::ROOT);
                    va.append(tmp);
                } else {
                    va.append(s, 0, idx);
                }
                if (prec != 0) {
                    addZeros(va, prec);
                }
                sb.append(va);
                sb.append(upper ? 'P' : 'p');
                sb.append(s, idx + 1, s.length());
            }
            return true;
        }

        void SpecialFragment::addZeros(XString& out, gint precision) {
            // Look for the dot.  If we don't find one, the we'll need to add
            // it before we add the zeros.
            gint len = out.length();
            gint i;
            for (i = 0; i < len; i++) {
                if (out.charAt(i) == '.') {
                    break;
                }
            }
            gbool needDot = false;
            if (i == len) {
                needDot = true;
            }

            // Determine existing precision.
            gint outPrec = len - i - (needDot ? 0 : 1);
            CORE_ASSERT(outPrec <= precision);
            if (outPrec == precision) {
                return;
            }

            // Add dot if previously determined to be necessary.
            if (needDot) {
                out.append('.');
            }

            // Add zeros.
            trailingZeros(out, precision - outPrec);
        }

        void SpecialFragment::justify(XString& out, CharSequence const& cs) const {
            if (w == -1) {
                out.append(cs);
                return;
            }
            gbool padRight = Flags::contains(f, Flags::LEFT_JUSTIFY);
            gint sp = w - cs.length();
            if (padRight) {
                out.append(cs);
            }
            for (gint i = 0; i < sp; i++) {
                out.append(' ');
            }
            if (!padRight) {
                out.append(cs);
            }
        }

        static gdouble SCALEUP = Math::scalb(1.0, 54);

        String SpecialFragment::hex(gdouble d, gint prec) {
            // Let Double.toHexString handle simple cases
            if (!Double::isFinite(d) || d == 0.0 || prec == 0 || prec >= 13) {
                // remove "0x"
                return Double::toHexString(d).subString(2);
            } else {
                CORE_ASSERT(prec >= 1 && prec <= 12);

                gint exponent = Math::exponent(d);
                gbool subnormal
                        = (exponent == Double::MIN_EXPONENT - 1);

                // If this is subnormal input so normalize (could be faster to
                // do as integer operation).
                if (subnormal) {
                    d *= SCALEUP;
                    // Calculate the exponent.  This is not just exponent + 54
                    // since the former is not the normalized exponent.
                    exponent = Math::exponent(d);
                    CORE_ASSERT2(exponent >= Double::MIN_EXPONENT &&
                                 exponent <= Double::MAX_EXPONENT, exponent);
                }

                gint precision = 1 + prec * 4;
                gint shiftDistance
                        = DoubleConsts::SIGNIFICAND_WIDTH - precision;
                CORE_ASSERT(shiftDistance >= 1 && shiftDistance < DoubleConsts::SIGNIFICAND_WIDTH);

                glong doppel = Double::toLongBits(d);
                // Deterime the number of bits to keep.
                glong newSignif
                        = (doppel & (DoubleConsts::EXP_BIT_MASK
                            | DoubleConsts::SIGNIF_BIT_MASK))
                        >> shiftDistance;
                // Bits to round away.
                glong roundingBits = doppel & ~(~0L << shiftDistance);

                // To decide how to round, look at the low-order bit of the
                // working significand, the highest order discarded bit (the
                // round bit) and whether any of the lower order discarded bits
                // are nonzero (the sticky bit).

                gbool leastZero = (newSignif & 0x1L) == 0L;
                gbool round
                        = ((1L << (shiftDistance - 1)) & roundingBits) != 0L;
                gbool sticky = shiftDistance > 1 &&
                        (~(1L << (shiftDistance - 1)) & roundingBits) != 0;
                if ((leastZero && round && sticky) || (!leastZero && round)) {
                    newSignif++;
                }

                glong signBit = doppel & DoubleConsts::SIGN_BIT_MASK;
                newSignif = signBit | (newSignif << shiftDistance);
                gdouble result = Double::fromLongBits(newSignif);

                if (Double::isInfinite(result)) {
                    // Infinite result generated by rounding
                    return "1.0p1024";
                } else {
                    String res = Double::toHexString(result).subString(2);
                    if (!subnormal)
                        return res;
                    else {
                        // Create a normalized subnormal string.
                        gint idx = res.indexOf('p');
                        if (idx == -1)
                            idx = res.indexOf('P');
                        if (idx == -1) {
                            // No 'p' character in hex string.
                            CORE_ASSERT(false);
                        } else {
                            // Get exponent and append at the end.
                            String exp = res.subString(idx + 1);
                            gint iexp = Integer::parseInt(exp) - 54;
                            return res.subString(0, idx) + "p"
                                    + Integer::toString(iexp);
                        }
                    }
                }
            }
        }

        gint SpecialFragment::adjust(gint width, gint flags, gbool negative) {
            gint newW = width;
            if (newW != -1 && negative && Flags::contains(flags, Flags::PARENTHESES))
                newW--;
            return newW;
        }

        void SpecialFragment::trailingZeros(XString& out, gint nzeros) {
            for (gint i = 0; i < nzeros; i++) {
                out.append('0');
            }
        }

        // void SpecialFragment::leadingZeros(XString& out, gint precision) {}

        XString& SpecialFragment::trailingSign(XString& out, gbool negative) const {
            if (negative && Flags::contains(f, Flags::PARENTHESES))
                out.append(')');
            return out;
        }

        XString& SpecialFragment::leadingSign(XString& out, gbool negative) const {
            if (!negative) {
                if (Flags::contains(f, Flags::PLUS)) {
                    out.append('+');
                } else if (Flags::contains(f, Flags::LEADING_SPACE)) {
                    out.append(' ');
                }
            } else {
                if (Flags::contains(f, Flags::PARENTHESES))
                    out.append('(');
                else
                    out.append('-');
            }
            return out;
        }

        XString& SpecialFragment::magnitude(Formatter& fmt, XString& sb, glong value, gint flags, gint width) {
            return magnitude(fmt, sb, Long::toString(value, 10), 0, flags, width);
        }

        XString& SpecialFragment::magnitude(Formatter& fmt, XString& sb, ULONG value, gint flags, gint width) {
            return magnitude(fmt, sb, Long::toUnsignedString(value, 10), 0, flags, width);
        }

        XString& SpecialFragment::magnitude(Formatter& fmt, XString& sb, CharSequence const& value, gint offset,
                                            gint flags, gint width) {
            gint begin = sb.length();

            gchar zero = fmt.getZeroDigit();

            // determine localized grouping separator and size
            gchar grpSep = '\0';
            gint grpSize = -1;
            gchar decSep = '\0';

            gint len = value.length();
            gint dot = len;
            for (gint j = offset; j < len; j++) {
                if (value.charAt(j) == '.') {
                    dot = j;
                    break;
                }
            }

            if (dot < len) {
                decSep = fmt.getDecimalSeparator();
            }

            if (Flags::contains(flags, Flags::GROUP)) {
                grpSep = fmt.getGroupingSeparator();

                if (fmt.locale.equals(Locale::US)) {
                    grpSize = 3;
                } else {
                    grpSize = fmt.getGroupingSize();
                    // Some locales do not use grouping (the number
                    // pattern for these locales does not contain group, e.g.
                    // ("#0.###")), but specify a grouping separator.
                    // To avoid unnecessary identification of the position of
                    // grouping separator, reset its value with null character
                    if (!fmt.isGroupingUsed() || grpSize == 0) {
                        grpSep = '\0';
                    }
                }
            }

            // localize the digits inserting group separators as necessary
            for (gint j = offset; j < len; j++) {
                if (j == dot) {
                    sb.append(decSep);
                    // no more group separators after the decimal separator
                    grpSep = '\0';
                    continue;
                }

                gchar c = value.charAt(j);
                sb.append((gchar) ((c - '0') + zero));
                if (grpSep != '\0' && j != dot - 1 && ((dot - j) % grpSize == 1)) {
                    sb.append(grpSep);
                }
            }

            // apply zero padding
            if (width > sb.length() && Flags::contains(flags, Flags::ZERO_PAD)) {
                String zeros = String::valueOf(zero).repeat(width - sb.length());
                sb.insert(begin, zeros);
            }

            return sb;
        }

        void SpecialFragment::magnitudeEx(Formatter& fmt, XString& sb, CharArray const& value, gint offset) {
            gchar zero = fmt.getZeroDigit();

            gint len = value.length();
            for (gint j = offset; j < len; j++) {
                gchar c = value[j];
                sb.append((gchar) ((c - '0') + zero));
            }
        }

        gbool SpecialFragment::apply(Formatter& fmt, gbyte b) const {
            glong v = (gint) b;
            if (b < 0 && (c == Conversion::OCTAL_INTEGER || c == Conversion::HEXADECIMAL_INTEGER)) {
                v += (1LL << 8);
            }
            return apply(fmt, v);
        }

        gbool SpecialFragment::apply(Formatter& fmt, gshort b) const {
            glong v = b;
            if (b < 0 && (c == Conversion::OCTAL_INTEGER || c == Conversion::HEXADECIMAL_INTEGER)) {
                v += (1LL << 16);
            }
            return apply(fmt, v);
        }

        gbool SpecialFragment::apply(Formatter& fmt, gint b) const {
            glong v = b;
            if (b < 0 && (c == Conversion::OCTAL_INTEGER || c == Conversion::HEXADECIMAL_INTEGER))
                v += (1LL << 32);
            return apply(fmt, v);
        }

        gbool SpecialFragment::apply(Formatter& fmt, glong value) const {
            XString sb = XString();

            if (c == Conversion::DECIMAL_INTEGER) {
                gbool neg = value < 0;
                String valueStr = Long::toString(value, 10);

                // leading sign indicator
                leadingSign(sb, neg);

                // the value
                magnitude(fmt, sb, valueStr, neg ? 1 : 0, f, adjust(w, f, neg));

                // trailing sign indicator
                trailingSign(sb, neg);
            } else if (c == Conversion::OCTAL_INTEGER) {
                try {
                    checkBadFlags(Flags::PARENTHESES | Flags::LEADING_SPACE | Flags::PLUS);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                String s = Long::toOctalString(value);
                gint len = (Flags::contains(f, Flags::ALTERNATE)
                                ? s.length() + 1
                                : s.length());

                // apply ALTERNATE (radix indicator for octal) before ZERO_PAD
                if (Flags::contains(f, Flags::ALTERNATE))
                    sb.append('0');
                if (Flags::contains(f, Flags::ZERO_PAD))
                    trailingZeros(sb, w - len);
                sb.append(s);
            } else if (c == Conversion::HEXADECIMAL_INTEGER) {
                try {
                    checkBadFlags(Flags::PARENTHESES | Flags::LEADING_SPACE | Flags::PLUS);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                String s = Long::toHexString(value);
                gint len = (Flags::contains(f, Flags::ALTERNATE)
                                ? s.length() + 2
                                : s.length());

                // apply ALTERNATE (radix indicator for hex) before ZERO_PAD
                if (Flags::contains(f, Flags::ALTERNATE))
                    sb.append(Flags::contains(f, Flags::UPPERCASE) ? "0X" : "0x");
                if (Flags::contains(f, Flags::ZERO_PAD))
                    trailingZeros(sb, w - len);
                if (Flags::contains(f, Flags::UPPERCASE))
                    s = s.toUpperCase(fmt.locale);
                sb.append(s);
            }

            // justify based on width
            justify(fmt.out, sb);
            return true;
        }

        gbool SpecialFragment::apply(Formatter& fmt, gfloat f) const {
            return apply(fmt, (gdouble) f);
        }

        gbool SpecialFragment::apply(Formatter& fmt, gdouble value) const {
            XString sb = XString();
            gbool neg = Double::compare(value, 0.0) == -1;
            gbool r = true;
            if (!Double::isNaN(value)) {
                gdouble v = Math::abs(value);

                // leading sign indicator
                leadingSign(sb, neg);

                // the value
                if (!Double::isInfinite(v))
                    r = apply(fmt, sb, v, f, c, p, neg);
                else
                    sb.append(Flags::contains(f, Flags::UPPERCASE) ? "INF" : "inf");

                // trailing sign indicator
                trailingSign(sb, neg);
            } else
                sb.append(Flags::contains(f, Flags::UPPERCASE) ? "NAN" : "NaN");

            // justify based on width
            justify(fmt.out, sb);
            return r;
        }

        gbool SpecialFragment::apply(Formatter& fmt, LiteralInt const& li) const {
            XString sb = XString();
            gbool r = false;

            if (c == Conversion::DECIMAL_INTEGER) {
                String valueStr = Long::toUnsignedString(li.value, 10);

                // leading sign indicator
                leadingSign(sb, false);

                // the value
                magnitude(fmt, sb, valueStr, 0, f, adjust(w, f, false));

                // trailing sign indicator
                trailingSign(sb, false);
            } else if (c == Conversion::OCTAL_INTEGER) {
                try {
                    checkBadFlags(Flags::PARENTHESES | Flags::LEADING_SPACE | Flags::PLUS);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                String s = Long::toOctalString(li.value);
                gint len = (Flags::contains(f, Flags::ALTERNATE)
                                ? s.length() + 1
                                : s.length());

                // apply ALTERNATE (radix indicator for octal) before ZERO_PAD
                if (Flags::contains(f, Flags::ALTERNATE))
                    sb.append('0');
                if (Flags::contains(f, Flags::ZERO_PAD))
                    trailingZeros(sb, w - len);
                sb.append(s);
            } else if (c == Conversion::HEXADECIMAL_INTEGER) {
                try {
                    checkBadFlags(Flags::PARENTHESES | Flags::LEADING_SPACE | Flags::PLUS);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                String s = Long::toHexString(li.value);
                gint len = (Flags::contains(f, Flags::ALTERNATE)
                                ? s.length() + 2
                                : s.length());

                // apply ALTERNATE (radix indicator for hex) before ZERO_PAD
                if (Flags::contains(f, Flags::ALTERNATE))
                    sb.append(Flags::contains(f, Flags::UPPERCASE) ? "0X" : "0x");
                if (Flags::contains(f, Flags::ZERO_PAD))
                    trailingZeros(sb, w - len);
                if (Flags::contains(f, Flags::UPPERCASE))
                    s = s.toUpperCase(fmt.locale);
                sb.append(s);
            }

            // justify based on width
            justify(fmt.out, sb);
            return r;
        }

        gbool SpecialFragment::apply(Formatter& fmt, LiteralFloat const& f) const {
            try { return apply(fmt, (gdouble) f.value); } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool SpecialFragment::apply(Formatter& fmt, Complex const& z) const {
            if (c == Conversion::HEXADECIMAL_FLOAT) {
                // flags '#' will be added
                f = Flags::add(f, Flags::ALTERNATE);
            }
            // flags '+' and '(' will be ignored
            if (Flags::contains(f, Flags::PLUS | Flags::PARENTHESES))
                f = Flags::remove(f, Flags::PLUS | Flags::PARENTHESES);
            gbool r = apply(fmt, z.real());
            if (Math::signum(z.imag()) > 0) {
                r &= apply(fmt, "+");
                r &= apply(fmt, z.imag());
            } else
                r &= apply(fmt, z.imag());
            r &= apply(fmt, "i");
            return r;
        }

        gbool SpecialFragment::apply(Formatter& fmt, Temporal const& t) const {
            XString sb = XString();
            try { apply(fmt, sb, t, c); } catch (Throwable const& ex) { ex.throws($ftrace()); }
            // justify based on width
            if (Flags::contains(f, Flags::UPPERCASE))
                justify(fmt.out, sb.toString().toUpperCase(fmt.locale));
            else
                justify(fmt.out, sb);
            return true;
        }

        XString& SpecialFragment::apply(Formatter& fmt, XString& sb, Temporal const& t, gchar c) const {
            try {
                XString xs;
                switch (c) {
                    case DateTime::HOUR_OF_DAY_0: { // 'H' (00 - 23)
                        glong i = t.getLong(Temporal::HOUR_OF_DAY);
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, 2));
                        break;
                    }
                    case DateTime::HOUR_OF_DAY: { // 'k' (0 - 23) -- like H
                        glong i = t.getLong(Temporal::HOUR_OF_DAY);
                        sb.append(magnitude(fmt, xs, i, Flags::NONE, 2));
                        break;
                    }
                    case DateTime::HOUR_0: { // 'I' (01 - 12)
                        glong i = t.getLong(Temporal::CLOCK_HOUR_OF_AMPM);
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, 2));
                        break;
                    }
                    case DateTime::HOUR: { // 'l' (1 - 12) -- like I
                        glong i = t.getLong(Temporal::CLOCK_HOUR_OF_AMPM);
                        sb.append(magnitude(fmt, xs, i, Flags::NONE, 2));
                        break;
                    }
                    case DateTime::MINUTE: { // 'M' (00 - 59)
                        glong i = t.getLong(Temporal::MINUTE_OF_HOUR);
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, 2));
                        break;
                    }
                    case DateTime::NANOSECOND: { // 'N' (000000000 - 999999999)
                        glong i;
                        try {
                            i = t.getLong(Temporal::NANO_OF_SECOND);
                        } catch (TemporalException const& _) {
                            i = t.getLong(Temporal::MILLI_OF_SECOND) * 1000000;
                        }
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, 9));
                        break;
                    }
                    case DateTime::MILLISECOND: { // 'L' (000 - 999)
                        glong i = t.getLong(Temporal::MILLI_OF_SECOND);
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, 3));
                        break;
                    }
                    case DateTime::MILLISECOND_SINCE_EPOCH: { // 'Q' (0 - 99...?)
                        glong i = t.getLong(Temporal::INSTANT_SECONDS) * 1000L +
                                t.getLong(Temporal::MILLI_OF_SECOND);
                        sb.append(magnitude(fmt, xs, i, Flags::NONE, w));
                        break;
                    }
                    case DateTime::AM_PM: { // 'p' (am or pm)
                        // Calendar.AM = 0, Calendar.PM = 1, LocaleElements defines upper
                        StringArray ampm = {"AM", "PM"};
                        if (fmt.locale != Locale::US)
                            ampm = fmt.getAmPmStrings();
                        const String& s = ampm[t.getLong(Temporal::AMPM_OF_DAY)];
                        sb.append(s.toLowerCase(Locale::getDefault(Locale::FORMAT)));
                        break;
                    }
                    case DateTime::SECONDS_SINCE_EPOCH: { // 's' (0 - 99...?)
                        glong i = t.getLong(Temporal::INSTANT_SECONDS);
                        sb.append(magnitude(fmt, xs, i, Flags::NONE, w));
                        break;
                    }
                    case DateTime::SECOND: { // 'S' (00 - 60 - leap second)
                        glong i = t.getLong(Temporal::SECOND_OF_MINUTE);
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, 2));
                        break;
                    }
                    case DateTime::ZONE_NUMERIC: { // 'z' ({-|+}####) - ls minus?
                        int i = t.getLong(Temporal::OFFSET_SECONDS);
                        gbool neg = i < 0;
                        sb.append(neg ? '-' : '+');
                        if (neg)
                            i = -i;
                        glong min = i / 60;
                        // combine minute and hour into a single integer
                        glong offset = (min / 60) * 100 + (min % 60);
                        sb.append(magnitude(fmt, xs, offset, Flags::ZERO_PAD, 4));
                        break;
                    }
                    case DateTime::ZONE: { // 'Z' (symbol)
                        Optional<ZoneId> opt = (Optional<ZoneId>) t.query(TemporalQuery::ZONE);
                        if (opt.isEmpty()) {
                            IllegalFormatConversionException(c, t).throws($ftrace());
                        }
                        ZoneId& zid = opt.get();
                        if (!Class<ZoneOffset>::hasInstance(zid) && t.isSupported(Temporal::INSTANT_SECONDS)) {
                            // glong instant = t.getLong(Temporal::INSTANT_SECONDS);
                            // sb.append(TimeZone::of(zid.getId())
                            //                   .displayName(zid.getRules()
                            //                       .isDaylightSavings(LocalDateTime::ofEpochSecond(instant/1000, 0)), Temporal::SHORT, fmt.locale));
                            // break;
                        }
                        sb.append(zid.getId());
                        UNSAFE::deleteRegInstance(zid);
                        break;
                    }
                    // Date
                    case DateTime::NAME_OF_DAY_ABBREV: // 'a'
                    case DateTime::NAME_OF_DAY: { // 'A'
                        int i = t.getLong(Temporal::DAY_OF_WEEK) % 7 + 1;
                        i -= 1; // for index
                        if (c == DateTime::NAME_OF_DAY) {
                            StringArray weekdays = fmt.getWeekdays();
                            sb.append(weekdays[i]);
                        } else {
                            StringArray weekdays = fmt.getShortWeekdays();
                            sb.append(weekdays[i]);
                        }
                        break;
                    }
                    case DateTime::NAME_OF_MONTH_ABBREV: // 'b'
                    case DateTime::NAME_OF_MONTH_ABBREV_X: // 'h' -- same b
                    case DateTime::NAME_OF_MONTH: { // 'B'
                        int i = t.getLong(Temporal::MONTH_OF_YEAR) - 1;
                        if (c == DateTime::NAME_OF_MONTH) {
                            StringArray months = fmt.getMonths();
                            sb.append(months[i]);
                        }else {
                            StringArray months = fmt.getShortMonths();
                            sb.append(months[i]);
                        }
                        break;
                    }
                    case DateTime::CENTURY: // 'C' (00 - 99)
                    case DateTime::YEAR_2: // 'y' (00 - 99)
                    case DateTime::YEAR_4: { // 'Y' (0000 - 9999)
                        glong i = t.getLong(Temporal::YEAR_OF_ERA);
                        int size = 2;
                        switch (c) {
                            case DateTime::CENTURY: i /= 100;
                                break;
                            case DateTime::YEAR_2: i %= 100;
                                break;
                            case DateTime::YEAR_4: size = 4;
                                break;
                            default: break;
                        }
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, size));
                        break;
                    }
                    case DateTime::DAY_OF_MONTH_0: // 'd' (01 - 31)
                    case DateTime::DAY_OF_MONTH: { // 'e' (1 - 31) -- like d
                        glong i = t.getLong(Temporal::DAY_OF_MONTH);
                        int flags = (c == DateTime::DAY_OF_MONTH_0
                                         ? Flags::ZERO_PAD
                                         : Flags::NONE);
                        sb.append(magnitude(fmt, xs, i, flags, 2));
                        break;
                    }
                    case DateTime::DAY_OF_YEAR: { // 'j' (001 - 366)
                        glong i = t.getLong(Temporal::DAY_OF_YEAR);
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, 3));
                        break;
                    }
                    case DateTime::MONTH: { // 'm' (01 - 12)
                        glong i = t.getLong(Temporal::MONTH_OF_YEAR);
                        sb.append(magnitude(fmt, xs, i, Flags::ZERO_PAD, 2));
                        break;
                    }

                    // Composites
                    case DateTime::TIME: // 'T' (24 hour hh:mm:ss - %tH:%tM:%tS)
                    case DateTime::TIME_24_HOUR: { // 'R' (hh:mm same as %H:%M)
                        char sep = ':';
                        apply(fmt, sb, t, DateTime::HOUR_OF_DAY_0).append(sep);
                        apply(fmt, sb, t, DateTime::MINUTE);
                        if (c == DateTime::TIME) {
                            sb.append(sep);
                            apply(fmt, sb, t, DateTime::SECOND);
                        }
                        break;
                    }
                    case DateTime::TIME_12_HOUR: { // 'r' (hh:mm:ss [AP]M)
                        gchar sep = ':';
                        apply(fmt, sb, t, DateTime::HOUR_0).append(sep);
                        apply(fmt, sb, t, DateTime::MINUTE).append(sep);
                        apply(fmt, sb, t, DateTime::SECOND).append(' ');
                        // this may be in wrong place for some locales
                        XString tsb = XString();
                        apply(fmt, tsb, t, DateTime::AM_PM);
                        sb.append(tsb.toString().toUpperCase(fmt.locale));
                        break;
                    }
                    case DateTime::DATE_TIME: { // 'c' (Sat Nov 04 12:02:33 EST 1999)
                        gchar sep = ' ';
                        apply(fmt, sb, t, DateTime::NAME_OF_DAY_ABBREV).append(sep);
                        apply(fmt, sb, t, DateTime::NAME_OF_MONTH_ABBREV).append(sep);
                        apply(fmt, sb, t, DateTime::DAY_OF_MONTH_0).append(sep);
                        apply(fmt, sb, t, DateTime::TIME).append(sep);
                        apply(fmt, sb, t, DateTime::ZONE).append(sep);
                        apply(fmt, sb, t, DateTime::YEAR_4);
                        break;
                    }
                    case DateTime::DATE: { // 'D' (mm/dd/yy)
                        gchar sep = '/';
                        apply(fmt, sb, t, DateTime::MONTH).append(sep);
                        apply(fmt, sb, t, DateTime::DAY_OF_MONTH_0).append(sep);
                        apply(fmt, sb, t, DateTime::YEAR_2);
                        break;
                    }
                    case DateTime::ISO_STANDARD_DATE: { // 'F' (%Y-%m-%d)
                        gchar sep = '-';
                        apply(fmt, sb, t, DateTime::YEAR_4).append(sep);
                        apply(fmt, sb, t, DateTime::MONTH).append(sep);
                        apply(fmt, sb, t, DateTime::DAY_OF_MONTH_0);
                        break;
                    }
                    default:
                        CORE_ASSERT(false);
                }
            } catch (DateTimeException const& _) { IllegalFormatConversionException(c, t).throws($ftrace()); }
            return sb;
        }

        FormatFlagsConversionMismatchException SpecialFragment::failMismatch(gint f, gchar c) {
            return FormatFlagsConversionMismatchException(Flags::toString(f), c);
        }

        IllegalFormatConversionException SpecialFragment::failConversion(gchar c, Object const& arg) {
            return IllegalFormatConversionException(c, arg);
        }

        gbool Conversion::isValid(gchar c) {
            switch (c) {
                case BOOLEAN:
                case BOOLEAN_UPPER:
                case STRING:
                case STRING_UPPER:
                case HASHCODE:
                case HASHCODE_UPPER:
                case CHARACTER:
                case CHARACTER_UPPER:
                case DECIMAL_INTEGER:
                case OCTAL_INTEGER:
                case HEXADECIMAL_INTEGER:
                case HEXADECIMAL_INTEGER_UPPER:
                case SCIENTIFIC:
                case SCIENTIFIC_UPPER:
                case GENERAL:
                case GENERAL_UPPER:
                case DECIMAL_FLOAT:
                case HEXADECIMAL_FLOAT:
                case HEXADECIMAL_FLOAT_UPPER:
                case LINE_SEPARATOR:
                case PERCENT_SIGN: return true;
                default: return false;
            }
        }

        gbool Conversion::isGeneral(gchar c) {
            switch (c) {
                case BOOLEAN:
                case BOOLEAN_UPPER:
                case STRING:
                case STRING_UPPER:
                case HASHCODE:
                case HASHCODE_UPPER: return true;
                default: return false;
            }
        }

        gbool Conversion::isCharacter(gchar c) {
            switch (c) {
                case CHARACTER:
                case CHARACTER_UPPER: return true;
                default: return false;
            }
        }

        gbool Conversion::isInteger(gchar c) {
            switch (c) {
                case DECIMAL_INTEGER:
                case OCTAL_INTEGER:
                case HEXADECIMAL_INTEGER:
                case HEXADECIMAL_INTEGER_UPPER: return true;
                default: return false;
            }
        }

        gbool Conversion::isFloat(gchar c) {
            switch (c) {
                case SCIENTIFIC:
                case SCIENTIFIC_UPPER:
                case GENERAL:
                case GENERAL_UPPER:
                case DECIMAL_FLOAT:
                case HEXADECIMAL_FLOAT:
                case HEXADECIMAL_FLOAT_UPPER: return true;
                default: return false;
            }
        }

        gbool Conversion::isText(gchar c) {
            switch (c) {
                case LINE_SEPARATOR:
                case PERCENT_SIGN: return true;
                default: return false;
            }
        }

        gbool DateTime::isValid(gchar c) {
            switch (c) {
                case HOUR_OF_DAY_0:
                case HOUR_0:
                case HOUR_OF_DAY:
                case HOUR:
                case MINUTE:
                case NANOSECOND:
                case MILLISECOND:
                case MILLISECOND_SINCE_EPOCH:
                case AM_PM:
                case SECONDS_SINCE_EPOCH:
                case SECOND:
                case TIME:
                case ZONE_NUMERIC:
                case ZONE:
                // Date
                case NAME_OF_DAY_ABBREV:
                case NAME_OF_DAY:
                case NAME_OF_MONTH_ABBREV:
                case NAME_OF_MONTH:
                case CENTURY:
                case DAY_OF_MONTH_0:
                case DAY_OF_MONTH:
                case NAME_OF_MONTH_ABBREV_X:
                case DAY_OF_YEAR:
                case MONTH:
                case YEAR_2:
                case YEAR_4:
                // Composites
                case TIME_12_HOUR:
                case TIME_24_HOUR:
                case DATE_TIME:
                case DATE:
                case ISO_STANDARD_DATE: return true;
                default: return false;
            }
        }

        void FormatParser::parse(String const& format) {
            gint i = 0;
            gint max = format.length();
            Optional<regex::Matcher> matcher; // create if needed

            while (i < max) {
                gint n = format.indexOf('%', i);
                if (n < 0) {
                    // No more format specifiers, but since
                    // i < max there's some trailing text
                    fragments.add(UNSAFE::newInstance<FixedFragment>(format, i, max));
                    break;
                }
                if (i != n) {
                    // Previous characters were fixed text
                    fragments.add(UNSAFE::newInstance<FixedFragment>(format, i, n));
                }
                i = n + 1;
                if (i >= max) {
                    // Trailing %
                    UnknownFormatConversionException("%").throws($ftrace());
                }
                gchar c = format.charAt(i);
                if (Conversion::isValid(c)) {
                    fragments.add(UNSAFE::newInstance<SpecialFragment>(c));
                    i++;
                } else {
                    if (matcher.isEmpty()) {
                        static String const FORMAT_SPECIFIER = "%(\\d+\\$)?([-#+ 0,(\\<]*)?(\\d+)?(\\.\\d+)?([tT])?([a-zA-Z%])"_Sl;
                        static regex::Pattern FORMAT_SPECIFIER_PATTERN = regex::Pattern::compile(FORMAT_SPECIFIER);
                        matcher = FORMAT_SPECIFIER_PATTERN.matcher(format);
                    }
                    // We have already parsed a '%' at n, so we either have a
                    // match or the specifier at n is invalid
                    if (matcher.get().find(n) && matcher.get().start() == n) {
                        Optional<SpecialFragment> sf;
                        try {
                            fragments.add(UNSAFE::newInstance<SpecialFragment>(format, matcher.get()));
                        } catch (Error const& ex) {
                            ex.cause().throws($ftrace());
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        i = matcher.get().end();
                    } else {
                        UnknownFormatConversionException(String::valueOf(c)).throws($ftrace());
                    }
                }
            }

            complete = true;
        }

        gbool FormatParser::isComplete() const {
            return complete;
        }

        SpecialFormatParser::SpecialFormatParser(ArrayList<FormatFragment> const& fragments, gchar first, gint start,
                                                 String const& str, gint max): fragments(fragments),
                                                                               first(first),
                                                                               start(start),
                                                                               str(str),
                                                                               max(max) {}
    } // ext
} // core
