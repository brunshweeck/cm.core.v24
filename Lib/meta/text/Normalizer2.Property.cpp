//
// Created by brunshweeck on 28/11/24.
//

#include "Normalizer2.Property.h"


namespace core {
    namespace text {
        static CORE_FAST Property ERROR = {0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0};
        static CORE_FAST Property properties[] = {
#include "Normalizer2.Property.nrm"
        };

        static CORE_FAST gchar indexes[] = {
#include "Normalizer2.Property.idx"
        };

        Property const& Property::getProperty(gint c) {
            if (Character::isValidCodePoint(c))
                return properties[getPropertyIndex(c)];
            return ERROR;
        }

        gint Property::getPropertyIndex(gint c) {
            if (!Character::isValidCodePoint(c))
                return 0xFFFF;
            if (c < 0x11000)
                return indexes[indexes[c >> 5] + (c & 0x1f)];

            return indexes[indexes[((c - 0x11000) >> 8) + 0x880] + (c & 0xff)];
        }

        Version const& Property::getVersion() const {
            switch (version) {
            case 1: return Version::UNICODE_1_1_0;
            case 2: return Version::UNICODE_2_0;
            case 3: return Version::UNICODE_2_1_2;
            case 4: return Version::UNICODE_3_0;
            case 5: return Version::UNICODE_3_1_0;
            case 6: return Version::UNICODE_3_2;
            case 7: return Version::UNICODE_4_0;
            case 8: return Version::UNICODE_4_1;
            case 9: return Version::UNICODE_5_0;
            case 10: return Version::UNICODE_5_1;
            case 11: return Version::UNICODE_5_2;
            case 12: return Version::UNICODE_6_0;
            case 13: return Version::UNICODE_6_1;
            case 14: return Version::UNICODE_6_2;
            case 15: return Version::UNICODE_6_3;
            case 16: return Version::UNICODE_7_0;
            case 17: return Version::UNICODE_8_0;
            case 18: return Version::UNICODE_9_0;
            case 19: return Version::UNICODE_10_0;
            case 20: return Version::UNICODE_11_0;
            case 21: return Version::UNICODE_12_0;
            case 22: return Version::UNICODE_12_1;
            case 23: return Version::UNICODE_13_0;
            case 24: return Version::UNICODE_14_0;
            case 25: return Version::UNICODE_15_0;
            case 26: return Version::UNICODE_16_0;
            default: return Version::UNICODE_VERSION;
            }
        }

        gint Property::getCombiningClass() const {
            return ccc;
        }

        gint Property::getType() const {
            return type;
        }

        static CORE_FAST gchar indexes2[] = {
#include "Normalizer2.Decomposition.idx"
        };
        static CORE_FAST gchar decompositions[] = {
#include "Normalizer2.Decomposition.nrm"
        };

        gint Property::getDecompositionIndex(gint c) {
            if (!Character::isValidCodePoint(c))
                return 0xFFFF;
            if (c < 0x3400)
                return indexes2[indexes2[c >> 4] + (c & 0xf)];
            if (c < 0x30000)
                return indexes2[indexes2[((c - 0x3400) >> 8) + 0x340] + (c & 0xff)];
            return 0xFFFF;
        }

        glong Property::getDecomposition(gint c) {
            gint index = getDecompositionIndex(c);
            if (index == 0xFFFF)
                return 0;
            return (glong) (decompositions + index);
        }

        static CORE_FAST gchar indexes3[] = {
#include "Normalizer2.Ligature.idx"
        };
        static CORE_FAST gchar ligatures[] = {
#include "Normalizer2.Ligature.nrm"
        };

        gint Property::getLigatureIndex(gint c) {
            if (!Character::isValidCodePoint(c))
                return 0xFFFF;
            if (c < 0x3100)
                return indexes3[indexes3[c >> 5] + (c & 0x1f)];
            if (c < 0x12000)
                return indexes3[indexes3[((c - 0x3100) >> 8) + 0x188] + (c & 0xff)];
            return 0xFFFF;
        }

        glong Property::getLigature(gint c) {
            gint index = getLigatureIndex(c);
            if (index == 0xFFFF)
                return 0L;
            return (glong) (ligatures + index);
        }
    }
}
