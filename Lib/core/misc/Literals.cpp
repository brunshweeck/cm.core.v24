//
// Created by bruns on 05/05/2024.
//

#include <core/AssertionError.h>
#include <core/IllegalArgumentException.h>
#include <core/OutOfMemoryError.h>
#include <core/String.h>
#include <core/Complex.h>
#include <core/misc/Literals.h>
#include <core/misc/Unsafe.h>

#include <core/charset/Charset.h>

// #line CORE_LINE "Literal.c24"

namespace core {
    CORE_ALIAS(UNSAFE, misc::Unsafe);

    using namespace charset;
    using namespace util;

    inline namespace literals {
        String operator ""_S(const char literal[], size_t len) {
            try {
                if (UNSAFE::ANSI_LITERAL)
                    return operator""_Sl(literal, len);

                return operator""_Su(literal, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator ""_S(const char16_t literal[], size_t len) {
            try {
                return operator""_Su(literal, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator ""_S(const char32_t literal[], size_t len) {
            try {
                return operator""_Su(literal, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator ""_S(const wchar_t literal[], size_t len) {
            try {
                return operator""_Su(literal, len);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator ""_Su(const char literal[], size_t len) {
            if (!literal)
                IllegalArgumentException("null literal").throws($ftrace());
            if (len > Integer::MAX_VALUE)
                OutOfMemoryError("literal too large").throws($ftrace());
            if (len == 0)
                return String();

            gint length = CORE_CAST(gint, len);
            ByteArray bytes = ByteArray(length);
            for (gint i = 0; i < length; i++) bytes[i] = literal[i];

            return String(bytes, Charset::UTF_8);
        }

        String operator ""_Su(const char16_t literal[], size_t len) {
            if (!literal)
                IllegalArgumentException("null literal").throws($ftrace());
            if (len > Integer::MAX_VALUE)
                OutOfMemoryError("literal too large").throws($ftrace());
            if (len == 0)
                return String();

            gint length = CORE_CAST(gint, len);
            CharArray chars = CharArray(length);

            for (gint i = 0; i < length; i++) chars[i] = literal[i];

            return String(chars);
        }

        String operator ""_Su(const char32_t literal[], size_t len) {
            if (!literal)
                IllegalArgumentException("null literal").throws($ftrace());
            if (len > Integer::MAX_VALUE)
                OutOfMemoryError("literal too large").throws($ftrace());
            if (len == 0)
                return String();

            gint length = CORE_CAST(gint, len);
            IntArray codepoints = IntArray(length);

            for (gint i = 0; i < length; i++) codepoints[i] = CORE_CAST(gint, literal[i]);

            return String(codepoints, 0, length);
        }

        String operator ""_Su(const wchar_t literal[], size_t len) {
            if (!literal)
                IllegalArgumentException("null literal").throws($ftrace());
            if (len > Integer::MAX_VALUE)
                OutOfMemoryError("literal too large").throws($ftrace());
            if (len == 0)
                return String();

            gint length = CORE_CAST(gint, len);

            if (sizeof(wchar_t) == sizeof(char16_t)) {
                CharArray chars = CharArray(length);

                for (gint i = 0; i < length; i++) chars[i] = literal[i];

                return String(chars);
            } else {
                IntArray codepoints = IntArray(length);

                for (gint i = 0; i < length; i++) codepoints[i] = CORE_CAST(gint, literal[i]);

                return String(codepoints, 0, length);
            }
        }

        String operator ""_Sl(const char literal[], size_t len) {
            if (!literal)
                IllegalArgumentException("null literal").throws($ftrace());
            if (len > Integer::MAX_VALUE)
                OutOfMemoryError("literal too large").throws($ftrace());
            if (len == 0)
                return String();
            gint length = CORE_CAST(gint, len);
            ByteArray bytes = ByteArray(length);
            for (gint i = 0; i < length; i++)
                bytes[i] = literal[i];

            return String(bytes, 0);
        }

        String operator ""_Sl(const char16_t literal[], size_t len) {
            if (!literal)
                IllegalArgumentException("null literal").throws($ftrace());
            if (len > Integer::MAX_VALUE)
                OutOfMemoryError("literal too large").throws($ftrace());
            if (len == 0)
                return String();
            gint length = CORE_CAST(gint, len);
            try {
                ByteArray bytes = ByteArray(length);
                for (gint i = 0; i < len; i++)
                    bytes[i] = literal[i] & 0xFF;
                return String(bytes, 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator ""_Sl(const char32_t literal[], size_t len) {
            if (!literal)
                IllegalArgumentException("null literal").throws($ftrace());
            if (len > Integer::MAX_VALUE)
                OutOfMemoryError("literal too large").throws($ftrace());
            if (len == 0)
                return String();
            gint length = CORE_CAST(gint, len);
            try {
                ByteArray bytes = ByteArray(length);
                for (gint i = 0; i < len; i++)
                    bytes[i] = literal[i] & 0xFF;
                return String(bytes, 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator ""_Sl(const wchar_t literal[], size_t len) {
            if (!literal)
                IllegalArgumentException("null literal").throws($ftrace());
            if (len > Integer::MAX_VALUE)
                OutOfMemoryError("literal too large").throws($ftrace());
            if (len == 0)
                return String();
            gint length = CORE_CAST(gint, len);
            try {
                ByteArray bytes = ByteArray(length);
                for (gint i = 0; i < len; i++)
                    bytes[i] = literal[i] & 0xFF;
                return String(bytes, 0);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Complex operator ""_i(unsigned long long imag) { return Complex(0, (gdouble) imag); }

        Complex operator ""_i(long double imag) { return Complex(0, (gdouble) imag); }

        Complex operator ""_j(unsigned long long imag) { return Complex(0, (gdouble) imag); }

        Complex operator ""_j(long double imag) { return Complex(0, (gdouble) imag); }

#if __cplusplus >= 201103L

        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_UDL

        Complex operator ""i(unsigned long long imag) { return Complex(0, (gdouble) imag); }

        Complex operator ""i(long double imag) { return Complex(0, (gdouble) imag); }

        Complex operator ""j(unsigned long long imag) { return Complex(0, (gdouble) imag); }

        Complex operator ""j(long double imag) { return Complex(0, (gdouble) imag); }

        CORE_WARNING_POP
#endif


        String operator +(String const& a, String const& b) {
            try {
                return a.concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, Object const& b) {
            try {
                if (Class<String>::hasInstance(b))
                    return a.concat(CORE_XCAST(String const, b));

                return a.concat(String::valueOf(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(Object const& a, String const& b) {
            try {
                if (Class<String>::hasInstance(a))
                    return CORE_XCAST(String const, a).concat(b);

                return String::valueOf(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, short b) {
            try {
                return a.concat(Short::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(short a, String const& b) {
            try {
                return Short::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, bool b) {
            try {
                return a.concat(Boolean::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(bool a, String const& b) {
            try {
                return Boolean::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, char b) {
            try {
                return a.concat(Character::toString(b & 0xFF));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(char a, String const& b) {
            try {
                return Character::toString(a & 0xFF).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, unsigned char b) {
            try {
                return a.concat(Character::toString(b & 0xFF));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(unsigned char a, String const& b) {
            try {
                return Character::toString(a & 0xFF).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, char16_t b) {
            try {
                return a.concat(Character::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(char16_t a, String const& b) {
            try {
                return Character::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, char32_t b) {
            try {
                return a.concat(Character::toString(CORE_CAST(gint, b)));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(char32_t a, String const& b) {
            try {
                return Character::toString(CORE_CAST(gint, a)).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, wchar_t b) {
            try {
                return a.concat(Character::toString(CORE_CAST(gint, b)));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(wchar_t a, String const& b) {
            try {
                return Character::toString(CORE_CAST(gint, a)).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, unsigned short b) {
            try {
                return a.concat(Integer::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(unsigned short a, String const& b) {
            try {
                return Integer::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, int b) {
            try {
                return a.concat(Integer::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(int a, String const& b) {
            try {
                return Integer::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, unsigned int b) {
            try {
                return a.concat(Integer::toUnsignedString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(unsigned int a, String const& b) {
            try {
                return Integer::toUnsignedString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, long b) {
            try {
                return a.concat(Long::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(long a, String const& b) {
            try {
                return Long::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, unsigned long b) {
            try {
                return a.concat(Long::toUnsignedString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(unsigned long a, String const& b) {
            try {
                return Long::toUnsignedString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, long long b) {
            try {
                return a.concat(Long::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(long long a, String const& b) {
            try {
                return Long::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, unsigned long long b) {
            try {
                return a.concat(Long::toUnsignedString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(unsigned long long a, String const& b) {
            try {
                return Long::toUnsignedString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, float b) {
            try {
                return a.concat(Float::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(float a, String const& b) {
            try {
                return Float::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, double b) {
            try {
                return a.concat(Double::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(double a, String const& b) {
            try {
                return Double::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(String const& a, long double b) {
            try {
                return a.concat(Double::toString(b));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String operator +(long double a, String const& b) {
            try {
                return Double::toString(a).concat(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, String const& b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, bool b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, char b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, char16_t b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, char32_t b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, wchar_t b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, short b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, unsigned short b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, int b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, unsigned int b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, long b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, unsigned long b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, long long b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, unsigned long long b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, float b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, double b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String& operator+=(String& a, long double b) {
            try {
                return a = a + b;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }


        gbool operator ==(String const& a, String const& b) {
            return &a == &b || a.equals(b);
        }

        gbool operator !=(String const& a, String const& b) {
            try {
                return !(a == b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool operator ==(String const& a, Object const& b) {
            try {
                return Class<String>::hasInstance(b) && a.equals(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool operator ==(Object const& a, String const& b) {
            try {
                return Class<String>::hasInstance(a) && a.equals(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool operator !=(String const& a, Object const& b) {
            try {
                return !(a == b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool operator !=(Object const& a, String const& b) {
            try {
                return !(a == b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Complex operator +(Complex const& a, Complex const& b) { return a.plus(b); }

        Complex operator +(Complex const& a, short b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(short a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, unsigned short b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(unsigned short a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, int b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(int a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, unsigned int b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(unsigned int a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, long b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(long a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, unsigned long b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(unsigned long a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, long long b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(long long a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, unsigned long long b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(unsigned long long a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, float b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(float a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, double b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(double a, Complex const& b) { return b + a; }

        Complex operator +(Complex const& a, long double b) { return a.plus(CORE_CAST(gdouble, b)); }

        Complex operator +(long double a, Complex const& b) { return b + a; }

        Complex operator -(Complex const& a, Complex const& b) { return a.plus(b.negate()); }

        Complex operator -(Complex const& a, short b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(short a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, unsigned short b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(unsigned short a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, int b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(int a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, unsigned int b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(unsigned int a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, long b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(long a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, unsigned long b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(unsigned long a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, long long b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(long long a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, unsigned long long b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(unsigned long long a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, float b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(float a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, double b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(double a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator -(Complex const& a, long double b) { return a.plus(-CORE_CAST(gdouble, b)); }

        Complex operator -(long double a, Complex const& b) { return b + -CORE_CAST(gdouble, a); }

        Complex operator *(Complex const& a, Complex const& b) { return a.multiply(b); }

        Complex operator *(Complex const& a, short b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(short a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, unsigned short b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(unsigned short a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, int b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(int a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, unsigned int b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(unsigned int a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, long b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(long a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, unsigned long b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(unsigned long a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, long long b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(long long a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, unsigned long long b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(unsigned long long a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, float b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(float a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, double b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(double a, Complex const& b) { return b * a; }

        Complex operator *(Complex const& a, long double b) { return a.multiply(CORE_CAST(gdouble, b)); }

        Complex operator *(long double a, Complex const& b) { return b * a; }

        Complex operator /(Complex const& a, Complex const& b) { return a.divide(b); }

        Complex operator /(Complex const& a, short b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(short a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, unsigned short b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(unsigned short a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, int b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(int a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, unsigned int b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(unsigned int a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, long b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(long a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, unsigned long b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(unsigned long a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, long long b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(long long a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, unsigned long long b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(unsigned long long a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, float b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(float a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, double b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(double a, Complex const& b) { return Complex(0.0, a).divide(b); }

        Complex operator /(Complex const& a, long double b) { return a.divide(CORE_CAST(gdouble, b)); }

        Complex operator /(long double a, Complex const& b) { return Complex(0.0, a).divide(b); }

        gbool operator ==(Complex const& a, Complex const& b) { return a.equals(b); }

        gbool operator ==(Complex const& a, short b) { return a.isReal() && a.real() == b; }

        gbool operator ==(short a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, unsigned short b) { return a.isReal() && a.real() == b; }

        gbool operator ==(unsigned short a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, int b) { return a.isReal() && a.real() == b; }

        gbool operator ==(int a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, unsigned int b) { return a.isReal() && a.real() == b; }

        gbool operator ==(unsigned int a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, long b) { return a.isReal() && a.real() == b; }

        gbool operator ==(long a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, unsigned long b) { return a.isReal() && a.real() == b; }

        gbool operator ==(unsigned long a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, long long b) { return a.isReal() && a.real() == b; }

        gbool operator ==(long long a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, unsigned long long b) { return a.isReal() && a.real() == b; }

        gbool operator ==(unsigned long long a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, float b) { return a.isReal() && Double::compare(a.real(), b) == 0; }

        gbool operator ==(float a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, double b) { return a.isReal() && Double::compare(a.real(), b) == 0; }

        gbool operator ==(double a, Complex const& b) { return b == a; }

        gbool operator ==(Complex const& a, long double b) { return a.isReal() && Double::compare(a.real(), b) == 0; }

        gbool operator ==(long double a, Complex const& b) { return b == a; }

        gbool operator !=(Complex const& a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, short b) { return !(a == b); }

        gbool operator !=(short a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, unsigned short b) { return !(a == b); }

        gbool operator !=(unsigned short a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, int b) { return !(a == b); }

        gbool operator !=(int a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, unsigned int b) { return !(a == b); }

        gbool operator !=(unsigned int a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, long b) { return !(a == b); }

        gbool operator !=(long a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, unsigned long b) { return !(a == b); }

        gbool operator !=(unsigned long a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, long long b) { return !(a == b); }

        gbool operator !=(long long a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, unsigned long long b) { return !(a == b); }

        gbool operator !=(unsigned long long a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, float b) { return !(a == b); }

        gbool operator !=(float a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, double b) { return !(a == b); }

        gbool operator !=(double a, Complex const& b) { return !(a == b); }

        gbool operator !=(Complex const& a, long double b) { return !(a == b); }

        gbool operator !=(long double a, Complex const& b) { return !(a == b); }

        Complex& operator +=(Complex& a, Complex const& b) { return a = a + b; }

        Complex& operator +=(Complex& a, short b) { return a = a + b; }

        Complex& operator +=(Complex& a, unsigned short b) { return a = a + b; }

        Complex& operator +=(Complex& a, int b) { return a = a + b; }

        Complex& operator +=(Complex& a, unsigned int b) { return a = a + b; }

        Complex& operator +=(Complex& a, long b) { return a = a + b; }

        Complex& operator +=(Complex& a, unsigned long b) { return a = a + b; }

        Complex& operator +=(Complex& a, long long b) { return a = a + b; }

        Complex& operator +=(Complex& a, unsigned long long b) { return a = a + b; }

        Complex& operator +=(Complex& a, float b) { return a = a + b; }

        Complex& operator +=(Complex& a, double b) { return a = a + b; }

        Complex& operator +=(Complex& a, long double b) { return a = a + b; }

        Complex& operator -=(Complex& a, Complex const& b) { return a = a + b; }

        Complex& operator -=(Complex& a, short b) { return a = a - b; }

        Complex& operator -=(Complex& a, unsigned short b) { return a = a - b; }

        Complex& operator -=(Complex& a, int b) { return a = a - b; }

        Complex& operator -=(Complex& a, unsigned int b) { return a = a - b; }

        Complex& operator -=(Complex& a, long b) { return a = a - b; }

        Complex& operator -=(Complex& a, unsigned long b) { return a = a - b; }

        Complex& operator -=(Complex& a, long long b) { return a = a - b; }

        Complex& operator -=(Complex& a, unsigned long long b) { return a = a - b; }

        Complex& operator -=(Complex& a, float b) { return a = a - b; }

        Complex& operator -=(Complex& a, double b) { return a = a - b; }

        Complex& operator -=(Complex& a, long double b) { return a = a - b; }

        Complex& operator *=(Complex& a, Complex const& b) { return a = a * b; }

        Complex& operator *=(Complex& a, short b) { return a = a * b; }

        Complex& operator *=(Complex& a, unsigned short b) { return a = a * b; }

        Complex& operator *=(Complex& a, int b) { return a = a * b; }

        Complex& operator *=(Complex& a, unsigned int b) { return a = a * b; }

        Complex& operator *=(Complex& a, long b) { return a = a * b; }

        Complex& operator *=(Complex& a, unsigned long b) { return a = a * b; }

        Complex& operator *=(Complex& a, long long b) { return a = a * b; }

        Complex& operator *=(Complex& a, unsigned long long b) { return a = a * b; }

        Complex& operator *=(Complex& a, float b) { return a = a * b; }

        Complex& operator *=(Complex& a, double b) { return a = a * b; }

        Complex& operator *=(Complex& a, long double b) { return a = a * b; }

        Complex& operator /=(Complex& a, Complex const& b) { return a = a / b; }

        Complex& operator /=(Complex& a, short b) { return a = a / b; }

        Complex& operator /=(Complex& a, unsigned short b) { return a = a / b; }

        Complex& operator /=(Complex& a, int b) { return a = a / b; }

        Complex& operator /=(Complex& a, unsigned int b) { return a = a / b; }

        Complex& operator /=(Complex& a, long b) { return a = a / b; }

        Complex& operator /=(Complex& a, unsigned long b) { return a = a / b; }

        Complex& operator /=(Complex& a, long long b) { return a = a / b; }

        Complex& operator /=(Complex& a, unsigned long long b) { return a = a / b; }

        Complex& operator /=(Complex& a, float b) { return a = a / b; }

        Complex& operator /=(Complex& a, double b) { return a = a / b; }

        Complex& operator /=(Complex& a, long double b) { return a = a / b; }

        Complex operator +(Complex const& b) { return b; }

        Complex operator -(Complex const& b) { return b.negate(); }

        Complex operator ~(Complex const& b) { return b.conjugate(); }

        gbool operator ==(Object const& a, Object const& b) {
            try {
                return &a == &b or &a != &null && &b != &null && a.equals(b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool operator !=(Object const& a, Object const& b) {
            try {
                return !(a == b);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    }
} // core

#ifdef CORE_DEFAULT_ALLOC_FUNCTIONS
//
#else
using namespace core;

// no inline, required by [replacement.functions]/3
void* operator new(size_t sizeInBytes) {
    if (sizeInBytes == 0)
        return null;

    if (sizeInBytes > Long::MAX_VALUE)
        OutOfMemoryError("Increasing memory size to larger than long").throws($ftrace());

    try {
        glong bytes = CORE_CAST(glong, sizeInBytes);
        glong ptr = UNSAFE::allocateMemory(bytes);
        UNSAFE::setMemory(null, ptr, bytes, 0);
        return CORE_CAST(void*, ptr);
    } catch (Throwable const& _) {
        OutOfMemoryError("Could not allocate memory for new").throws($ftrace());
    }
}

// no inline, required by [replacement.functions]/3
void* operator new[](size_t sizeInBytes) {
    if (sizeInBytes == 0)
        return null;

    if (sizeInBytes > Long::MAX_VALUE)
        OutOfMemoryError("Increasing memory size to larger than long").throws($ftrace());

    try {
        glong bytes = CORE_CAST(glong, sizeInBytes);
        glong ptr = UNSAFE::allocateMemory(bytes);
        UNSAFE::setMemory(null, ptr, bytes, 0);
        return CORE_CAST(void*, ptr);
    } catch (Throwable const& _) {
        OutOfMemoryError("Could not allocate memory for new").throws($ftrace());
    }
}

void operator delete(void* cAddress) {
    if (!cAddress)
        return;

    glong ptr = CORE_CAST(glong, cAddress);
    UNSAFE::freeMemory(ptr);
}

void operator delete[](void* cAddress) {
    if (!cAddress)
        return;

    glong ptr = CORE_CAST(glong, cAddress);
    UNSAFE::freeMemory(ptr);
}

void operator delete(void* cAddress, size_t sizeInBytes) {
    if (!cAddress)
        return;

    glong ptr = CORE_CAST(glong, cAddress);
    glong bytes = CORE_CAST(glong, sizeInBytes);
    UNSAFE::setMemory(null, ptr, bytes, 0);
    UNSAFE::freeMemory(ptr);
}

void operator delete[](void* cAddress, size_t sizeInBytes) {
    if (!cAddress)
        return;

    glong ptr = CORE_CAST(glong, cAddress);
    glong bytes = CORE_CAST(glong, sizeInBytes);
    UNSAFE::setMemory(null, ptr, bytes, 0);
    UNSAFE::freeMemory(ptr);
}
#endif
