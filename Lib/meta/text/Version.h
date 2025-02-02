//
// Created by brunshweeck on 28/11/24.
//

#ifndef CORE24_VERSION_H
#define CORE24_VERSION_H

#include "core/String.h"

namespace core {
    namespace text {
        class Version final : public Comparable<Version> {
            CORE_ADD_GLOBAL_FRIENDS();
            /**
             * Version number stored as a byte for each of the major, minor, milli and
             * micro numbers in the 32 bit int.
             * Most significant for the major and the least significant contains the
             * micro numbers.
             */
            gint version;

            /**
             * Constructor with int
             * @param compactversion a 32 bit int with each byte representing a number
             */
            Version(gint compactversion);

            /**
             * Gets the int from the version numbers
             * @param major non-negative version number
             * @param minor non-negative version number
             * @param milli non-negative version number
             * @param micro non-negative version number
             */
            static gint compact(gint major, gint minor, gint milli, gint micro);

        public:
            /**
             * Returns an instance of VersionInfo with the argument version.
             * @param version version String in the format of "major.minor.milli.micro"
             *                or "major.minor.milli" or "major.minor" or "major",
             *                where major, minor, milli, micro are non-negative numbers
             *                &lt;= 255. If the trailing version numbers are
             *                not specified they are taken as 0s. E.g. Version "3.1" is
             *                equivalent to "3.1.0.0".
             * @return an instance of VersionInfo with the argument version.
             * @exception IllegalArgumentException when the argument version
             *                is not in the right format
             * @stable ICU 2.6
             */
            static Version& get(const String& version);

            /**
             * Returns an instance of VersionInfo with the argument version.
             * @param major major version, non-negative number &lt;= 255.
             * @param minor minor version, non-negative number &lt;= 255.
             * @param milli milli version, non-negative number &lt;= 255.
             * @param micro micro version, non-negative number &lt;= 255.
             * @exception IllegalArgumentException when either arguments are negative or &gt; 255
             * @stable ICU 2.6
             */
            static Version& get(gint major, gint minor, gint milli, gint micro);

            /**
             * Returns an instance of VersionInfo with the argument version.
             * Equivalent to getInstance(major, minor, 0, 0).
             * @param major major version, non-negative number &lt;= 255.
             * @param minor minor version, non-negative number &lt;= 255.
             * @exception IllegalArgumentException when either arguments are
             *                                     negative or &gt; 255
             * @stable ICU 2.6
             */
            static Version& get(gint major, gint minor);

            /**
             * Returns an instance of VersionInfo with the argument version.
             * Equivalent to getInstance(major, 0, 0, 0).
             * @param major major version, non-negative number &lt;= 255.
             * @exception IllegalArgumentException when either arguments are
             *                                     negative or &gt; 255
             * @stable ICU 2.6
             */
            static Version& get(gint major);

            /**
             * Returns the String representative of VersionInfo in the format of
             * "major.minor.milli.micro"
             * @return String representative of VersionInfo
             * @stable ICU 2.6
             */
            String toString() const override;

            /**
             * Returns the major version number
             * @return the major version number
             * @stable ICU 2.6
             */
            gint major() const;

            /**
             * Returns the minor version number
             * @return the minor version number
             * @stable ICU 2.6
             */
            gint minor() const;

            /**
             * Returns the milli version number
             * @return the milli version number
             * @stable ICU 2.6
             */
            gint milli() const;

            /**
             * Returns the micro version number
             * @return the micro version number
             * @stable ICU 2.6
             */
            gint micro() const;

            /**
             * Checks if this version information is equals to the argument version
             * @param other object to be compared
             * @return true if other is equals to this object's version information,
             *         false otherwise
             * @stable ICU 2.6
             */
            gbool equals(const Object& other) const override;

            /**
             * Returns the hash code value for this set.
             *
             * @return the hash code value for this set.
             * @see java.lang.Object#hash()
             * @stable ICU 2.6
             */
            gint hash() const override;

            /**
             * Compares other with this VersionInfo.
             * @param other VersionInfo to be compared
             * @return 0 if the argument is a VersionInfo object that has version
             *           information equals to this object.
             *           Less than 0 if the argument is a VersionInfo object that has
             *           version information greater than this object.
             *           Greater than 0 if the argument is a VersionInfo object that
             *           has version information less than this object.
             * @stable ICU 2.6
             */
            gint compareTo(const Version& o) const override;

            /**
             * Return itself
             * @return itself
             */
            Object& clone() const override;

            /**
             * Unicode 1.0 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_1_0;
            /**
             * Unicode 1.0.1 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_1_0_1;
            /**
             * Unicode 1.1.0 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_1_1_0;
            /**
             * Unicode 1.1.5 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_1_1_5;
            /**
             * Unicode 2.0 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_2_0;
            /**
             * Unicode 2.1.2 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_2_1_2;
            /**
             * Unicode 2.1.5 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_2_1_5;
            /**
             * Unicode 2.1.8 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_2_1_8;
            /**
             * Unicode 2.1.9 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_2_1_9;
            /**
             * Unicode 3.0 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_3_0;
            /**
             * Unicode 3.0.1 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_3_0_1;
            /**
             * Unicode 3.1.0 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_3_1_0;
            /**
             * Unicode 3.1.1 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_3_1_1;
            /**
             * Unicode 3.2 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_3_2;

            /**
             * Unicode 4.0 version
             * @stable ICU 2.6
             */
            static Version const & UNICODE_4_0;

            /**
             * Unicode 4.0.1 version
             * @stable ICU 3.4
             */
            static Version const & UNICODE_4_0_1;

            /**
             * Unicode 4.1 version
             * @stable ICU 3.4
             */
            static Version const & UNICODE_4_1;

            /**
             * Unicode 5.0 version
             * @stable ICU 3.4
             */
            static Version const & UNICODE_5_0;

            /**
             * Unicode 5.1 version
             * @stable ICU 4.2
             */
            static Version const & UNICODE_5_1;

            /**
             * Unicode 5.2 version
             * @stable ICU 4.4
             */
            static Version const & UNICODE_5_2;

            /**
             * Unicode 6.0 version
             * @stable ICU 4.6
             */
            static Version const & UNICODE_6_0;

            /**
             * Unicode 6.1 version
             * @stable ICU 49
             */
            static Version const & UNICODE_6_1;

            /**
             * Unicode 6.2 version
             * @stable ICU 50
             */
            static Version const & UNICODE_6_2;

            /**
             * Unicode 6.3 version
             * @stable ICU 52
             */
            static Version const & UNICODE_6_3;

            /**
             * Unicode 7.0 version
             * @stable ICU 54
             */
            static Version const & UNICODE_7_0;

            /**
             * Unicode 8.0 version
             * @stable ICU 56
             */
            static Version const & UNICODE_8_0;

            /**
             * Unicode 9.0 version
             * @stable ICU 58
             */
            static Version const & UNICODE_9_0;

            /**
             * Unicode 10.0 version
             * @stable ICU 60
             */
            static Version const & UNICODE_10_0;

            /**
             * Unicode 11.0 version
             * @stable ICU 62
             */
            static Version const & UNICODE_11_0;

            /**
             * Unicode 12.0 version
             * @stable ICU 64
             */
            static Version const & UNICODE_12_0;

            /**
             * Unicode 12.1 version
             * @stable ICU 64
             */
            static Version const & UNICODE_12_1;

            /**
             * Unicode 13.0 version
             * @stable ICU 66
             */
            static Version const & UNICODE_13_0;

            /**
             * Unicode 14.0 version
             * @stable ICU 70
             */
            static Version const & UNICODE_14_0;

            /**
             * Unicode 15.0 version
             * @stable ICU 72
             */
            static Version const & UNICODE_15_0;

            /**
             * Unicode 15.1 version
             * @stable ICU 74
             */
            static Version const & UNICODE_15_1;

            /**
             * Unicode 16.0 version
             * @stable ICU 76
             */
            static Version const & UNICODE_16_0;

            /**
             * ICU4J current release version
             * @stable ICU 2.8
             */
            static Version const & ICU_VERSION;

            /**
             * Data version in ICU4J.
             * @internal
             * @deprecated This API is ICU internal only.
             */
            static Version const & ICU_DATA_VERSION;

            /**
             * Collation runtime version (sort key generator, string comparisons).
             * If the version is different, sort keys for the same string could be different.
             * This value may change in subsequent releases of ICU.
             * @stable ICU 2.8
             */
            static Version const & UCOL_RUNTIME_VERSION;

            /**
             * Collation builder code version.
             * When this is different, the same tailoring might result
             * in assigning different collation elements to code points.
             * This value may change in subsequent releases of ICU.
             * @stable ICU 2.8
             */
            static Version const & UCOL_BUILDER_VERSION;

            /**
             * Constant version 1.
             * This was intended to be the version of collation tailorings,
             * but instead the tailoring data carries a version number.
             * @deprecated ICU 54
             */
            static Version const & UCOL_TAILORINGS_VERSION;

            /**
             * Unicode data version used by the current release.
             * Defined here privately for printing by the main() method in this class.
             * Should be the same as {@link com.ibm.icu.lang.UCharacter#getUnicodeVersion()}
             * which gets the version number from a data file.
             * We do not want VersionInfo to have an import dependency on UCharacter.
             */
            static Version const & UNICODE_VERSION;

        private:
            /**
             * Map of singletons
             */
            static util::HashMap<Integer, Version> VERSIONS;
            /**
             * Last byte mask
             */
            static CORE_FAST gint LAST_BYTE_MASK = 0xFF;
            /**
             * Error statement string
             */
            static const String INVALID_VERSION_NUMBER;

            static concurrent::ReentrantLock sync;
        };
    } // text
} // core

#endif //CORE24_VERSION_H
