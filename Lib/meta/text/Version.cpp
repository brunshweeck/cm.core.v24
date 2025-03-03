//
// Created by brunshweeck on 28/11/24.
//

#include "Version.h"

#include "AutoSync.h"
#include <core/concurrent/ReentrantLock.h>
#include <core/util/HashMap.h>

namespace core {
    using namespace util;
    using namespace concurrent;

    namespace text {

        static Map<Integer, Version> &getVersions() {
            static HashMap<Integer, Version> VERSIONS = {};
            return VERSIONS;
        }

        static ReentrantLock &getVersionsLock() {
            static ReentrantLock LOCK = {};
            return LOCK;
        }

        String const Version::INVALID_VERSION_NUMBER = "Invalid version number: Version number may be negative or greater than 255";

        Version const& Version::UNICODE_1_0 = get(1, 0, 0, 0);
        Version const& Version::UNICODE_1_0_1 = get(1, 0, 1, 0);
        Version const& Version::UNICODE_1_1_0 = get(1, 1, 0, 0);
        Version const& Version::UNICODE_1_1_5 = get(1, 1, 5, 0);
        Version const& Version::UNICODE_2_0 = get(2, 0, 0, 0);
        Version const& Version::UNICODE_2_1_2 = get(2, 1, 2, 0);
        Version const& Version::UNICODE_2_1_5 = get(2, 1, 5, 0);
        Version const& Version::UNICODE_2_1_8 = get(2, 1, 8, 0);
        Version const& Version::UNICODE_2_1_9 = get(2, 1, 9, 0);
        Version const& Version::UNICODE_3_0 = get(3, 0, 0, 0);
        Version const& Version::UNICODE_3_0_1 = get(3, 0, 1, 0);
        Version const& Version::UNICODE_3_1_0 = get(3, 1, 0, 0);
        Version const& Version::UNICODE_3_1_1 = get(3, 1, 1, 0);
        Version const& Version::UNICODE_3_2 = get(3, 2, 0, 0);
        Version const& Version::UNICODE_4_0 = get(4, 0, 0, 0);
        Version const& Version::UNICODE_4_0_1 = get(4, 0, 1, 0);
        Version const& Version::UNICODE_4_1 = get(4, 1, 0, 0);
        Version const& Version::UNICODE_5_0 = get(5, 0, 0, 0);
        Version const& Version::UNICODE_5_1 = get(5, 1, 0, 0);
        Version const& Version::UNICODE_5_2 = get(5, 2, 0, 0);
        Version const& Version::UNICODE_6_0 = get(6, 0, 0, 0);
        Version const& Version::UNICODE_6_1 = get(6, 1, 0, 0);
        Version const& Version::UNICODE_6_2 = get(6, 2, 0, 0);
        Version const& Version::UNICODE_6_3 = get(6, 3, 0, 0);
        Version const& Version::UNICODE_7_0 = get(7, 0, 0, 0);
        Version const& Version::UNICODE_8_0 = get(8, 0, 0, 0);
        Version const& Version::UNICODE_9_0 = get(9, 0, 0, 0);
        Version const& Version::UNICODE_10_0 = get(10, 0, 0, 0);
        Version const& Version::UNICODE_11_0 = get(11, 0, 0, 0);
        Version const& Version::UNICODE_12_0 = get(12, 0, 0, 0);
        Version const& Version::UNICODE_12_1 = get(12, 1, 0, 0);
        Version const& Version::UNICODE_13_0 = get(13, 0, 0, 0);
        Version const& Version::UNICODE_14_0 = get(14, 0, 0, 0);
        Version const& Version::UNICODE_15_0 = get(15, 0, 0, 0);
        Version const& Version::UNICODE_15_1 = get(15, 1, 0, 0);
        Version const& Version::UNICODE_16_0 = get(16, 0, 0, 0);

        Version const& Version::ICU_VERSION = get(76, 1, 0, 0);
        Version const& Version::ICU_DATA_VERSION = ICU_VERSION;
        Version const& Version::UNICODE_VERSION = UNICODE_16_0;

        Version const& Version::UCOL_RUNTIME_VERSION = get(9);
        Version const& Version::UCOL_BUILDER_VERSION = get(9);
        Version const& Version::UCOL_TAILORINGS_VERSION = get(1);


        Version::Version(gint compactversion) {
            version = compactversion;
        }

        gint Version::compact(gint major, gint minor, gint milli, gint micro) {
            return (major << 24) | (minor << 16) | (milli << 8) | micro;
        }

        Version& Version::get(const String& version) {
            gint length = version.length();
            IntArray array = {0, 0, 0, 0};
            int count = 0;
            int index = 0;

            while (count < 4 && index < length) {
                gint c = version.charAt(index);
                if (c == '.') {
                    count++;
                }
                else {
                    c -= '0';
                    if (c < 0 || c > 9) {
                        IllegalArgumentException(INVALID_VERSION_NUMBER).throws($ftrace());
                    }
                    array[count] *= 10;
                    array[count] += c;
                }
                index++;
            }
            if (index != length) {
                IllegalArgumentException("Invalid version number: String '"_Sl + version + "' exceeds version format"_Sl)
                    .throws($ftrace());
            }
            for (int i : array) {
                if (i < 0 || i > 255) {
                    IllegalArgumentException(INVALID_VERSION_NUMBER).throws($ftrace());
                }
            }

            try {
                return get(array[0], array[1], array[2], array[3]);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        Version& Version::get(gint major, gint minor, gint milli, gint micro) {
            // checks if it is in the hashmap
            // else
            if (major < 0 || major > 255 || minor < 0 || minor > 255 ||
                milli < 0 || milli > 255 || micro < 0 || micro > 255) {
                IllegalArgumentException(INVALID_VERSION_NUMBER).throws($ftrace());
            }
            AutoSync sync = getVersionsLock();
            gint version = compact(major, minor, milli, micro);
            Integer key = version;
            Object& ver = getVersions().getOrNull(key);
            if (ver == null) {
                Version& v = UNSAFE::newInstance<Version>(version);
                getVersions().putIfAbsent(key, v);
                return v;
            }
            CORE_IGNORE(sync);
            return CORE_XCAST(Version, ver);
        }

        Version& Version::get(gint major, gint minor) {
            try {
                return get(major, minor, 0, 0);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        Version& Version::get(gint major) {
            try {
                return get(major, 0, 0, 0);
            } catch (Throwable const& ex) {
                ex.throws($ftrace());
            }
        }

        String Version::toString() const {
            XString result = XString(7);
            result.append(major());
            result.append('.');
            result.append(minor());
            result.append('.');
            result.append(milli());
            result.append('.');
            result.append(micro());
            return result.toString();
        }

        gint Version::major() const {
            return (version >> 24) & LAST_BYTE_MASK;
        }

        gint Version::minor() const {
            return (version >> 16) & LAST_BYTE_MASK;
        }

        gint Version::milli() const {
            return (version >> 8) & LAST_BYTE_MASK;
        }

        gint Version::micro() const {
            return version & LAST_BYTE_MASK;
        }

        gbool Version::equals(const Object& other) const {
            return this == &other || Class<Version>::hasInstance(other) && CORE_XCAST(Version const, other).version == version;
        }

        gint Version::hash() const {
            return version;
        }

        gint Version::compareTo(const Version& other) const {
            // m_version_ is an int, a signed 32-bit integer.
            // When the major version is >=128, then the version int is negative.
            // Compare it in two steps to simulate an unsigned-int comparison.
            // (Alternatively we could turn each int into a long and reset the upper 32 bits.)
            // Compare the upper bits first, using logical shift right (unsigned).
            int diff = (gint)(((version + 0U) >> 1) - ((other.version + 0U) >> 1));
            if (diff != 0) {
                return diff;
            }
            // Compare the remaining bits.
            return (gint)(((version + 0U) & 1) - ((other.version + 0U) & 1));
        }

        Object& Version::clone() const {
            return (Version &) *this;
        }
    } // text
} // core
