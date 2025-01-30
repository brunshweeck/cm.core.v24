//
// Created by bruns on 16/10/2024.
//

#include <meta/regex/Pattern.Grapheme.h>
#include <meta/regex/Pattern.IndicConjunctBreak.h>

namespace core {
    namespace regex {
        gbool Pattern::Grapheme::rules[LAST_TYPE + 1][LAST_TYPE + 1] = {};
        gint Pattern::Grapheme::rulesCount = initRules();

        gint Pattern::Grapheme::nextBoundary(CharSequence const &src, gint off, gint limit) {

            CORE_IGNORE(rulesCount);

            gint ch0 = Character::codePointAt(src, off);
            gint ret = off + Character::charCount(ch0);
            // indicates whether gb11 or gb12 is underway
            gint t0 = getType(ch0);
            gint riCount = t0 == RI ? 1 : 0;
            gbool gb11 = t0 == EXTENDED_PICTOGRAPHIC;
            while (ret < limit) {
                gint ch1 = Character::codePointAt(src, ret);
                gint t1 = getType(ch1);

                // GB9c
                if (IndicConjunctBreak::isConsonant(ch0)) {
                    gint advance = checkIndicConjunctBreak(src, ret, limit);
                    if (advance >= 0) {
                        ret += advance;
                        continue;
                    }
                }

                if (gb11 && t0 == ZWJ && t1 == EXTENDED_PICTOGRAPHIC) {
                    // continue for gb11
                } else if (riCount % 2 == 1 && t0 == RI && t1 == RI) {
                    // continue for gb12
                } else if (rules[t0][t1]) {
                    if (ret > off) {
                        break;
                    } else {
                        gb11 = t1 == EXTENDED_PICTOGRAPHIC;
                        riCount = 0;
                    }
                }

                riCount += (t1 == RI) ? 1 : 0;
                ch0 = ch1;
                t0 = t1;

                ret += Character::charCount(ch1);
            }
            return ret;
        }

        gint Pattern::Grapheme::initRules() {
            // GB 999 Any + Any  -> default
            for (gint i = FIRST_TYPE; i <= LAST_TYPE; i++)
                for (gint j = FIRST_TYPE; j <= LAST_TYPE; j++)
                    rules[i][j] = true;
            // GB 6 L x (L | V | LV | VT)
            rules[L][L] = false;
            rules[L][V] = false;
            rules[L][LV] = false;
            rules[L][LVT] = false;
            // GB 7 (LV | V) x (V | T)
            rules[LV][V] = false;
            rules[LV][T] = false;
            rules[V][V] = false;
            rules[V][T] = false;
            // GB 8 (LVT | T) x T
            rules[LVT][T] = false;
            rules[T][T] = false;
            // GB 9 x (Extend|ZWJ)
            // GB 9a x Spacing Mark
            // GB 9b Prepend x
            for (gint i = FIRST_TYPE; i <= LAST_TYPE; i++) {
                rules[i][EXTEND] = false;
                rules[i][ZWJ] = false;
                rules[i][SPACINGMARK] = false;
                rules[PREPEND][i] = false;
            }
            // GB 4  (Control | CR | LF) +
            // GB 5  + (Control | CR | LF)
            for (gint i = FIRST_TYPE; i <= LAST_TYPE; i++)
                for (gint j = CR; j <= CONTROL; j++) {
                    rules[i][j] = true;
                    rules[j][i] = true;
                }
            // GB 3 CR x LF
            rules[CR][LF] = false;
            // GB 11 Exended_Pictographic x (Extend|ZWJ)
            rules[EXTENDED_PICTOGRAPHIC][EXTEND] = false;
            rules[EXTENDED_PICTOGRAPHIC][ZWJ] = false;

            return LAST_TYPE * LAST_TYPE;
        }

        gbool Pattern::Grapheme::isExcludedSpacingMark(gint cp) {
            return cp == 0x102B || cp == 0x102C || cp == 0x1038 ||
                   cp >= 0x1062 && cp <= 0x1064 ||
                   cp >= 0x1067 && cp <= 0x106D ||
                   cp == 0x1083 ||
                   cp >= 0x1087 && cp <= 0x108C ||
                   cp == 0x108F ||
                   cp >= 0x109A && cp <= 0x109C ||
                   cp == 0x1A61 || cp == 0x1A63 || cp == 0x1A64 ||
                   cp == 0xAA7B || cp == 0xAA7D;
        }

        gint Pattern::Grapheme::getType(gint cp) {
            if (cp < 0x007F) {
                // ASCII
                if (cp < 32) {
                    // Control characters
                    if (cp == 0x000D)
                        return CR;
                    if (cp == 0x000A)
                        return LF;
                    return CONTROL;
                }
                return OTHER;
            }

            if (Character::isExtendedPictographic(cp)) {
                return EXTENDED_PICTOGRAPHIC;
            }

            Character::Category type = Character::category(cp);
            switch (type) {
                case Character::Category::UNASSIGNED:
                    // NOTE: #tr29 lists "Unassigned and Default_Ignorable_Code_Point" as Control
                    // but GraphemeBreakTest.txt lists u+0378/reserved-0378 as "Other"
                    // so type it as "Other" to make the test happy
                    if (cp == 0x0378)
                        return OTHER;

                case Character::Category::CONTROL:
                case Character::Category::LINE_SEPARATOR:
                case Character::Category::PARAGRAPH_SEPARATOR:
                case Character::Category::SURROGATE:
                    return CONTROL;
                case Character::Category::FORMAT:
                    if (cp == 0x200C ||
                        cp >= 0xE0020 && cp <= 0xE007F)
                        return EXTEND;
                    if (cp == 0x200D)
                        return ZWJ;
                    if (cp >= 0x0600 && cp <= 0x0605 ||
                        cp == 0x06DD || cp == 0x070F ||
                        cp == 0x0890 || cp == 0x0891 ||
                        cp == 0x08E2 || cp == 0x110BD || cp == 0x110CD)
                        return PREPEND;
                    return CONTROL;
                case Character::Category::NON_SPACING_MARK:
                case Character::Category::ENCLOSING_MARK:
                    // NOTE:
                    // #tr29 "plus a few General_Category = Spacing_Mark needed for
                    // canonical equivalence."
                    // but for "extended grapheme clusters" support, there is no
                    // need actually to diff "extend" and "spackmark" given GB9, GB9a
                    return EXTEND;
                case Character::Category::COMBINING_SPACING_MARK:
                    if (isExcludedSpacingMark(cp))
                        return OTHER;
                // NOTE:
                // 0x11720 and 0x11721 are mentioned in #tr29 as
                // OTHER_LETTER but it appears their category has been updated to
                // COMBING_SPACING_MARK already (verified in ver.8)
                    return SPACINGMARK;
                case Character::Category::OTHER_SYMBOL:
                    if (cp >= 0x1F1E6 && cp <= 0x1F1FF)
                        return RI;
                    return OTHER;
                case Character::Category::MODIFIER_LETTER:
                case Character::Category::MODIFIER_SYMBOL:
                    // WARNING:
                    // not mentioned in #tr29 but listed in GraphemeBreakProperty.txt
                    if (cp == 0xFF9E || cp == 0xFF9F ||
                        cp >= 0x1F3FB && cp <= 0x1F3FF)
                        return EXTEND;
                    return OTHER;
                case Character::Category::OTHER_LETTER: {
                    if (cp == 0x0E33 || cp == 0x0EB3)
                        return SPACINGMARK;
                    // hangul jamo
                    if (cp >= 0x1100 && cp <= 0x11FF) {
                        if (cp <= 0x115F)
                            return L;
                        if (cp <= 0x11A7)
                            return V;
                        return T;
                    }
                    // hangul syllables
                    gint sindex = cp - SYLLABLE_BASE;
                    if (sindex >= 0 && sindex < SCOUNT) {
                        if (sindex % TCOUNT == 0)
                            return LV;
                        return LVT;
                    }
                    //  hangul jamo_extended A
                    if (cp >= 0xA960 && cp <= 0xA97C)
                        return L;
                    //  hangul jamo_extended B
                    if (cp >= 0xD7B0 && cp <= 0xD7C6)
                        return V;
                    if (cp >= 0xD7CB && cp <= 0xD7FB)
                        return T;

                    // Prepend
                    switch (cp) {
                        case 0x0D4E:
                        case 0x111C2:
                        case 0x111C3:
                        case 0x1193F:
                        case 0x11941:
                        case 0x11A3A:
                        case 0x11A84:
                        case 0x11A85:
                        case 0x11A86:
                        case 0x11A87:
                        case 0x11A88:
                        case 0x11A89:
                        case 0x11D46:
                        case 0x11F02:
                            return PREPEND;
                        default: break;
                    }
                }
                default: break;
            }
            return OTHER;
        }

        gint Pattern::Grapheme::checkIndicConjunctBreak(CharSequence const &src, gint index, gint limit) {
            gbool linkerFound = false;
            gint advance = 0;

            while (index + advance < limit) {
                gint ch1 = Character::codePointAt(src, index + advance);
                advance += Character::charCount(ch1);

                if (IndicConjunctBreak::isLinker(ch1)) {
                    linkerFound = true;
                } else if (IndicConjunctBreak::isConsonant(ch1)) {
                    if (linkerFound) {
                        return advance;
                    } else {
                        break;
                    }
                } else if (!IndicConjunctBreak::isExtend(ch1)) {
                    break;
                }
            }
            return -1;
        }
    } // regex
} // core
