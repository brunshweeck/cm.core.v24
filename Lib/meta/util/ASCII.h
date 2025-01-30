//
// Created by brunshweeck on 12 sept. 2024.
//

#ifndef CORE24_ASCII_H
#define CORE24_ASCII_H

#include <core/String.h>

namespace core {
    namespace util {
        /**
         * Collection of static utility methods for Locale support. The
         * methods which manipulate characters or strings support ASCII only.
         */
        class ASCII final : public Object {
        public:
            /**
             * Compares two ASCII Strings s1 and s2, ignoring case.
             */
            static gbool caseIgnoreMatch(String const& s1, String const& s2);

            static gbool caseIgnoreCompare(String const& s1, String const& s2);

            static gchar toUpper(gchar c);

            static gchar toLower(gchar c);

            /**
             * Converts the given ASCII String to lower-case.
             */
            static String toLowerString(String const& s);

            /**
             * Converts the given ASCII String to upper-case.
             */
            static String toUpperString(String const& s);

            /**
             * Converts the given ASCII String to capital-case.
             */
            static String toTitleString(String const& s);

            static gbool isAlphaString(String const& s);

            static gbool isNumericString(String const& s);

            static gbool isAlphaNumericString(String const& s);

            static gbool isEmpty(String const& s);

        private:
            static gbool isUpper(gchar c);

            static gbool isLower(gchar c);

            static gbool isAlpha(gchar c);

            static gbool isNumeric(gchar c);

            static gbool isAlphaNumeric(gchar c);

            CORE_ADD_AS_FRIEND(Locale);
        };
    } // util
} // core

#endif //CORE24_ASCII_H
