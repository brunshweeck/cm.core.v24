//
// Created by admin on 03/12/24.
//

#ifndef FORMATPARSER_H
#define FORMATPARSER_H

#include <core/util/ArrayList.h>

#include "FormatFlagsConversionMismatchException.h"
#include "IllegalFormatConversionException.h"

namespace core {
    using namespace util;
    using namespace time;
    CORE_ALIAS(ULONG, unsigned long long);
    CORE_ALIAS(LDOUBLE, long double);

    namespace ext {
        class Formatter;

        class Flags final : public Object {
        public:
            static CORE_FAST int NONE = 0; // ''

            // duplicate declarations from Formattable.java
            static CORE_FAST int LEFT_JUSTIFY = 1 << 0; // '-'
            static CORE_FAST int UPPERCASE = 1 << 1; // '^'
            static CORE_FAST int ALTERNATE = 1 << 2; // '#'

            // numerics
            static CORE_FAST int PLUS = 1 << 3; // '+'
            static CORE_FAST int LEADING_SPACE = 1 << 4; // ' '
            static CORE_FAST int ZERO_PAD = 1 << 5; // '0'
            static CORE_FAST int GROUP = 1 << 6; // ','
            static CORE_FAST int PARENTHESES = 1 << 7; // '('

            // indexing
            static CORE_FAST int PREVIOUS = 1 << 8; // '<'

            static gbool contains(gint flags, gint flag);

            static gbool containsAny(gint flags, gint flag);

            static gint add(gint flags, gint flag);

            static gint remove(gint flags, gint flag);

            static gint parse(String const& s, gint start, gint end);

            static String toString(gint flag);

            static gint parse(gchar c);
        };

        class FormatFragment : public Object {
        public:
            virtual gint index() const = 0;

            virtual gbool apply(Formatter& fmt, Object const& arg) = 0;

            String toString() const override = 0;

            Object& clone() const override;
        };

        class FixedFragment final : public FormatFragment {
        public:
            String const& str;
            gint start;
            gint end;

            FixedFragment(String const& str, gint start, gint end);

            gint index() const override;

            gbool apply(Formatter& fmt, Object const& arg) override;

            String toString() const override;
        };

        class LiteralInt final : public Object {
        public:
            glong value;
            CORE_EXPLICIT LiteralInt(glong value) : value(value) {}
        };

        class LiteralFloat final : public Object {
        public:
            long double value;

            CORE_EXPLICIT LiteralFloat(long double value): value(value) {}
        };

        class SpecialFragment final : public FormatFragment {
            gint i = 0; // index
            gint mutable f = Flags::NONE; // flags
            gint w = -1; // width
            gint p = -1; // precision
            gbool dt = false;
            gchar c = 0; // conversion

        public:
            CORE_EXPLICIT SpecialFragment(gchar conversion);

            SpecialFragment(String const& str, regex::Matcher const& matcher);

            gint index() const override;

            gbool apply(Formatter& fmt, Object const& arg) override;

            String toString() const override;

            void index(String const& str, gint start, gint end);

            void flags(String const& str, gint start, gint end);

            void width(String const& str, gint start, gint end);

            void precision(String const& str, gint start, gint end);

            void conversion(gchar conv);

            // gbool check();

            void checkGeneral() const;

            void checkDate() const;

            void checkCharacter() const;

            void checkInteger() const;

            void checkFloat() const;

            void checkNumeric() const;

            void checkText() const;

            void checkBadFlags(gint badFlags) const;

            gbool applyInteger(Formatter& fmt, Object const& arg) const;

            gbool applyFloat(Formatter& fmt, Object const& arg) const;

            gbool applyCharacter(Formatter& fmt, Object const& arg) const;

            gbool applyString(Formatter& fmt, Object const& arg) const;

            gbool applyBoolean(Formatter& fmt, Object const& arg) const;

            gbool applyDate(Formatter& fmt, Object const& arg) const;

            gbool applyHash(Formatter& fmt, Object const& arg) const;

            gbool apply(Formatter& fmt, String const& arg) const;

            gbool apply(Formatter& fmt, XString& sb, gdouble const& arg, gint flag, gchar c, gint precision,
                        gbool negative) const;

            static void addZeros(XString& out, gint precision);

            void justify(XString& out, CharSequence const& cs) const;

            static String hex(gdouble arg, gint precision);

            static gint adjust(gint width, gint flags, gbool negative);

            static void trailingZeros(XString& out, gint precision);

            // void leadingZeros(XString& out, gint precision);

            XString& trailingSign(XString& out, gbool negative) const;

            XString& leadingSign(XString& out, gbool negative) const;

            static XString& magnitude(Formatter& fmt, XString& sb, glong arg, gint flags, gint width);

            static XString& magnitude(Formatter& fmt, XString& sb, ULONG arg, gint flags, gint width);

            static XString& magnitude(Formatter& fmt, XString& sb, CharSequence const& arg, gint offset,
                                      gint flags, gint width);

            static void magnitudeEx(Formatter& fmt, XString& sb, CharArray const& arg, gint offset);

            gbool apply(Formatter& fmt, gbyte b) const;

            gbool apply(Formatter& fmt, gshort i) const;

            gbool apply(Formatter& fmt, gint i) const;

            gbool apply(Formatter& fmt, glong i) const;

            gbool apply(Formatter& fmt, gfloat f) const;

            gbool apply(Formatter& fmt, gdouble f) const;

            gbool apply(Formatter& fmt, LiteralInt const& i) const;

            gbool apply(Formatter& fmt, LiteralFloat const& f) const;

            gbool apply(Formatter& fmt, Complex const& z) const;

            gbool apply(Formatter& fmt, Temporal const& t) const;

            XString& apply(Formatter& fmt, XString& sb, Temporal const& t, gchar c) const;

            static FormatFlagsConversionMismatchException failMismatch(gint f, gchar c);

            static IllegalFormatConversionException failConversion(gchar c, Object const& arg);
        };

        class Conversion final : public Object {
        public:
            // Byte, Short, Integer, Long, BigInteger
            // (and associated primitives due to autoboxing)
            static CORE_FAST gchar DECIMAL_INTEGER = 'd';
            static CORE_FAST gchar OCTAL_INTEGER = 'o';
            static CORE_FAST gchar HEXADECIMAL_INTEGER = 'x';
            static CORE_FAST gchar HEXADECIMAL_INTEGER_UPPER = 'X';

            // Float, Double, BigDecimal
            // (and associated primitives due to autoboxing)
            static CORE_FAST gchar SCIENTIFIC = 'e';
            static CORE_FAST gchar SCIENTIFIC_UPPER = 'E';
            static CORE_FAST gchar GENERAL = 'g';
            static CORE_FAST gchar GENERAL_UPPER = 'G';
            static CORE_FAST gchar DECIMAL_FLOAT = 'f';
            static CORE_FAST gchar HEXADECIMAL_FLOAT = 'a';
            static CORE_FAST gchar HEXADECIMAL_FLOAT_UPPER = 'A';

            // Character, Byte, Short, Integer
            // (and associated primitives due to autoboxing)
            static CORE_FAST gchar CHARACTER = 'c';
            static CORE_FAST gchar CHARACTER_UPPER = 'C';

            // ZonedDateTime, Calendar, long
            static CORE_FAST gchar DATE_TIME = 't';
            static CORE_FAST gchar DATE_TIME_UPPER = 'T';

            // if (arg.TYPE != boolean) return boolean
            // if (arg != null) return true; else return false;
            static CORE_FAST gchar BOOLEAN = 'b';
            static CORE_FAST gchar BOOLEAN_UPPER = 'B';
            // if (arg instanceof Formattable) arg.formatTo()
            // else arg.toString();
            static CORE_FAST gchar STRING = 's';
            static CORE_FAST gchar STRING_UPPER = 'S';
            // arg.hash()
            static CORE_FAST gchar HASHCODE = 'h';
            static CORE_FAST gchar HASHCODE_UPPER = 'H';

            static CORE_FAST gchar LINE_SEPARATOR = 'n';
            static CORE_FAST gchar PERCENT_SIGN = '%';

            static gbool isValid(gchar c);

            // Returns true iff the Conversion is applicable to all objects.
            static gbool isGeneral(gchar c);

            // Returns true iff the Conversion is applicable to character.
            static gbool isCharacter(gchar c);

            // Returns true iff the Conversion is an integer type.
            static gbool isInteger(gchar c);

            // Returns true iff the Conversion is a floating-point type.
            static gbool isFloat(gchar c);

            // Returns true iff the Conversion does not require an argument
            static gbool isText(gchar c);
        };

        class DateTime final : public Object {
        public:
            static CORE_FAST gchar HOUR_OF_DAY_0 = 'H'; // (00 - 23)
            static CORE_FAST gchar HOUR_0 = 'I'; // (01 - 12)
            static CORE_FAST gchar HOUR_OF_DAY = 'k'; // (0 - 23) -- like H
            static CORE_FAST gchar HOUR = 'l'; // (1 - 12) -- like I
            static CORE_FAST gchar MINUTE = 'M'; // (00 - 59)
            static CORE_FAST gchar NANOSECOND = 'N'; // (000000000 - 999999999)
            static CORE_FAST gchar MILLISECOND = 'L'; // not in gnu (000 - 999)
            static CORE_FAST gchar MILLISECOND_SINCE_EPOCH = 'Q'; // (0 - 99...?)
            static CORE_FAST gchar AM_PM = 'p'; // (am or pm)
            static CORE_FAST gchar SECONDS_SINCE_EPOCH = 's'; // (0 - 99...?)
            static CORE_FAST gchar SECOND = 'S'; // (00 - 60 - leap second)
            static CORE_FAST gchar TIME = 'T'; // (24 hour hh:mm:ss)
            static CORE_FAST gchar ZONE_NUMERIC = 'z'; // (-1200 - +1200) - ls minus?
            static CORE_FAST gchar ZONE = 'Z'; // (symbol)

            // Date
            static CORE_FAST gchar NAME_OF_DAY_ABBREV = 'a'; // 'a'
            static CORE_FAST gchar NAME_OF_DAY = 'A'; // 'A'
            static CORE_FAST gchar NAME_OF_MONTH_ABBREV = 'b'; // 'b'
            static CORE_FAST gchar NAME_OF_MONTH = 'B'; // 'B'
            static CORE_FAST gchar CENTURY = 'C'; // (00 - 99)
            static CORE_FAST gchar DAY_OF_MONTH_0 = 'd'; // (01 - 31)
            static CORE_FAST gchar DAY_OF_MONTH = 'e'; // (1 - 31) -- like d
            static CORE_FAST gchar NAME_OF_MONTH_ABBREV_X = 'h'; // -- same b
            static CORE_FAST gchar DAY_OF_YEAR = 'j'; // (001 - 366)
            static CORE_FAST gchar MONTH = 'm'; // (01 - 12)
            static CORE_FAST gchar YEAR_2 = 'y'; // (00 - 99)
            static CORE_FAST gchar YEAR_4 = 'Y'; // (0000 - 9999)

            // Composites
            static CORE_FAST gchar TIME_12_HOUR = 'r'; // (hh:mm:ss [AP]M)
            static CORE_FAST gchar TIME_24_HOUR = 'R'; // (hh:mm same as %H:%M)
            static CORE_FAST gchar DATE_TIME = 'c';
            // (Sat Nov 04 12:02:33 EST 1999)
            static CORE_FAST gchar DATE = 'D'; // (mm/dd/yy)
            static CORE_FAST gchar ISO_STANDARD_DATE = 'F'; // (%Y-%m-%d)

            static gbool isValid(gchar c);
        };

        class FormatParser final : public Object {
        public:
            ArrayList<FormatFragment> fragments;
            gbool complete = false;

            FormatParser() = default;

            void parse(String const& format);

            gbool isComplete() const;
        };

        class SpecialFormatParser final : public Object {
            ArrayList<FormatFragment> fragments;
            gchar first = 0;
            gint start = 0;
            String const& str;
            gint max = 0;
            gint end = 0;
            gchar c = 0;
            gint aSize = 0;
            gint fSize = 0;
            gint wSize = 0;

        public:
            SpecialFormatParser(ArrayList<FormatFragment> const& fragments, gchar first,
                                gint start, String const& str, gint max);
        };
    } // ext
} // core

#endif //FORMATPARSER_H
