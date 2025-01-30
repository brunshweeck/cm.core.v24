//
// Created by brunshweeck on 12 sept. 2024.
//

#include <core/String.h>
#include <meta/util/ASCII.h>

namespace core {
    namespace util {
        gbool ASCII::caseIgnoreMatch(String const& s1, String const& s2) {
            if (&s1 == &s2)
                return true;

            gint n = s1.length();

            if (n != s2.length())
                return false;

            for (int i = 0; i < n; i++) {
                gchar c1 = s1.charAt(i);
                gchar c2 = s2.charAt(i);

                if (c1 != c2 && toLower(c1) != toLower(c2)) {
                    return false;
                }
            }
            return true;
        }

        gbool ASCII::caseIgnoreCompare(String const& s1, String const& s2) {
            if (&s1 == &s2)
                return true;
            return toLowerString(s1).compareTo(toLowerString(s2));
        }

        gchar ASCII::toUpper(gchar c) {
            return isLower(c) ? CORE_CAST(gchar, c - 0x20) : c;
        }

        gchar ASCII::toLower(gchar c) {
            return isUpper(c) ? CORE_CAST(gchar, c + 0x20) : c;
        }

        String ASCII::toLowerString(String const& s) {
            gint n = s.length();
            gint idx = 0;
            for (; idx < n; idx++) {
                if (isUpper(s.charAt(idx)))
                    break;
            }

            if (idx == n)
                return s;

            CharArray ca = CharArray(n);
            for (int i = 0; i < n; ++i) {
                gchar c = s.charAt(i);
                ca[i] = i < idx ? c : toLower(c);
            }
            return String(ca);
        }

        String ASCII::toUpperString(String const& s) {
            gint n = s.length();
            gint idx = 0;
            for (; idx < n; idx++) {
                if (isLower(s.charAt(idx)))
                    break;
            }

            if (idx == n)
                return s;

            CharArray ca = CharArray(n);
            for (int i = 0; i < n; ++i) {
                gchar c = s.charAt(i);
                ca[i] = i < idx ? c : toUpper(c);
            }
            return String(ca);
        }

        String ASCII::toTitleString(String const& s) {
            gint n = s.length();
            if (n == 0)
                return s;
            gint idx = 0;
            if (!isLower(s.charAt(idx))) {
                for (idx = 1; idx < n; idx++) {
                    if (isUpper(s.charAt(idx)))
                        break;
                }
            }

            if (idx == n)
                return s;

            CharArray ca = CharArray(n);
            for (int i = 0; i < n; ++i) {
                gchar c = s.charAt(i);
                if (i == 0 && idx == 0) {
                    ca[i] = toUpper(c);
                } else if (i < idx) {
                    ca[i] = c;
                } else {
                    ca[i] = toLower(c);
                }
            }
            return String(ca);
        }

        gbool ASCII::isAlphaString(String const& s) {
            gint n = s.length();
            for (int i = 0; i < n; i++) {
                if (!isAlpha(s.charAt(i))) {
                    return false;
                }
            }
            return true;
        }

        gbool ASCII::isNumericString(String const& s) {
            gint n = s.length();
            for (gint i = 0; i < n; i++) {
                if (!isNumeric(s.charAt(i))) {
                    return false;
                }
            }
            return true;
        }

        gbool ASCII::isAlphaNumericString(String const& s) {
            int len = s.length();
            for (int i = 0; i < len; i++) {
                if (!isAlphaNumeric(s.charAt(i))) {
                    return false;
                }
            }
            return true;
        }

        gbool ASCII::isEmpty(String const& s) {
            return s.isEmpty();
        }

        gbool ASCII::isUpper(gchar c) {
            return c >= 'A' && c <= 'Z';
        }

        gbool ASCII::isLower(gchar c) {
            return c >= 'a' && c <= 'z';
        }

        gbool ASCII::isAlpha(gchar c) {
            return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
        }

        gbool ASCII::isNumeric(gchar c) {
            return  c >= '0' && c <= '9';
        }

        gbool ASCII::isAlphaNumeric(gchar c) {
            return isAlpha(c) || isNumeric(c);
        }
    } // util
} // core
