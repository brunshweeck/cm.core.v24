//
// Created by admin on 09/01/25.
//

#include "Enum.h"

#include <core/charset/Charset.h>
#include <core/io/File.h>
#include <core/regex/Pattern.h>
#include <core/text/Normalizer.h>
#include <core/time/LocalDate.h>
#include <core/time/ZoneOffsetTransitionRule.h>

namespace core {
    using namespace charset;
    using namespace io;
    using namespace regex;
    using namespace text;
    using namespace time;
    using namespace util;

#define LABEL(CLASS, NAME) setDefaultLabel(#NAME ## _Sl, CLASS::NAME)
#define ENUM_CLASS(CLASS, ENUM) \
    class CLASS ## _ ## ENUM ## Enum final: public Enum<CLASS::ENUM> { \
    public:\
        CLASS ## _ ## ENUM ## Enum (String const &label, CLASS::ENUM value): Enum/*<CLASS::ENUM>*/(label, (gint)value) {}\
        static gbool init();\
    };\
    static gint $_ ## CLASS ## _ ## ENUM ## Enum = CLASS ## _ ## ENUM ## Enum::init(); \
    gbool CLASS ## _ ## ENUM ## Enum::init()


    ENUM_CLASS(Character, Category) {
        Enum e = (Character::Category) 0;
        static Object& table = labels(null);
        LABEL(Character::Category, UNASSIGNED);
        LABEL(Character::Category, UPPERCASE_LETTER);
        LABEL(Character::Category, LOWERCASE_LETTER);
        LABEL(Character::Category, TITLECASE_LETTER);
        LABEL(Character::Category, MODIFIER_LETTER);
        LABEL(Character::Category, OTHER_LETTER);
        LABEL(Character::Category, NON_SPACING_MARK);
        LABEL(Character::Category, ENCLOSING_MARK);
        LABEL(Character::Category, COMBINING_SPACING_MARK);
        LABEL(Character::Category, DECIMAL_DIGIT_NUMBER);
        LABEL(Character::Category, LETTER_NUMBER);
        LABEL(Character::Category, OTHER_NUMBER);
        LABEL(Character::Category, SPACE_SEPARATOR);
        LABEL(Character::Category, LINE_SEPARATOR);
        LABEL(Character::Category, PARAGRAPH_SEPARATOR);
        LABEL(Character::Category, CONTROL);
        LABEL(Character::Category, FORMAT);
        LABEL(Character::Category, PRIVATE_USE);
        LABEL(Character::Category, SURROGATE);
        LABEL(Character::Category, DASH_PUNCTUATION);
        LABEL(Character::Category, START_PUNCTUATION);
        LABEL(Character::Category, END_PUNCTUATION);
        LABEL(Character::Category, CONNECTOR_PUNCTUATION);
        LABEL(Character::Category, OTHER_PUNCTUATION);
        LABEL(Character::Category, MATH_SYMBOL);
        LABEL(Character::Category, CURRENCY_SYMBOL);
        LABEL(Character::Category, MODIFIER_SYMBOL);
        LABEL(Character::Category, OTHER_SYMBOL);
        LABEL(Character::Category, INITIAL_QUOTE_PUNCTUATION);
        LABEL(Character::Category, FINAL_QUOTE_PUNCTUATION);
        labels(table);
        return true;
    }

    ENUM_CLASS(Character, Directionality) {
        Enum e = (Character::Directionality) 0;
        static Object& table = labels(null);
        LABEL(Character::Directionality, DIRECTIONALITY_UNDEFINED);
        LABEL(Character::Directionality, DIRECTIONALITY_LEFT_TO_RIGHT);
        LABEL(Character::Directionality, DIRECTIONALITY_RIGHT_TO_LEFT);
        LABEL(Character::Directionality, DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC);
        LABEL(Character::Directionality, DIRECTIONALITY_EUROPEAN_NUMBER);
        LABEL(Character::Directionality, DIRECTIONALITY_EUROPEAN_NUMBER_SEPARATOR);
        LABEL(Character::Directionality, DIRECTIONALITY_EUROPEAN_NUMBER_TERMINATOR);
        LABEL(Character::Directionality, DIRECTIONALITY_ARABIC_NUMBER);
        LABEL(Character::Directionality, DIRECTIONALITY_COMMON_NUMBER_SEPARATOR);
        LABEL(Character::Directionality, DIRECTIONALITY_NON_SPACING_MARK);
        LABEL(Character::Directionality, DIRECTIONALITY_BOUNDARY_NEUTRAL);
        LABEL(Character::Directionality, DIRECTIONALITY_PARAGRAPH_SEPARATOR);
        LABEL(Character::Directionality, DIRECTIONALITY_SEGMENT_SEPARATOR);
        LABEL(Character::Directionality, DIRECTIONALITY_WHITESPACE);
        LABEL(Character::Directionality, DIRECTIONALITY_OTHER_NEUTRALS);
        LABEL(Character::Directionality, DIRECTIONALITY_LEFT_TO_RIGHT_EMBEDDING);
        LABEL(Character::Directionality, DIRECTIONALITY_LEFT_TO_RIGHT_OVERRIDE);
        LABEL(Character::Directionality, DIRECTIONALITY_RIGHT_TO_LEFT_EMBEDDING);
        LABEL(Character::Directionality, DIRECTIONALITY_RIGHT_TO_LEFT_OVERRIDE);
        LABEL(Character::Directionality, DIRECTIONALITY_POP_DIRECTIONAL_FORMAT);
        LABEL(Character::Directionality, DIRECTIONALITY_LEFT_TO_RIGHT_ISOLATE);
        LABEL(Character::Directionality, DIRECTIONALITY_RIGHT_TO_LEFT_ISOLATE);
        LABEL(Character::Directionality, DIRECTIONALITY_FIRST_STRONG_ISOLATE);
        LABEL(Character::Directionality, DIRECTIONALITY_POP_DIRECTIONAL_ISOLATE);
        labels(table);
        return true;
    }

    ENUM_CLASS(Character, UnicodeBlock) {
        Enum e = (Character::UnicodeBlock) 0;
        static Object& table = labels(null);
        LABEL(Character::UnicodeBlock, BASIC_LATIN);
        LABEL(Character::UnicodeBlock, LATIN_1_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, LATIN_EXTENDED_A);
        LABEL(Character::UnicodeBlock, LATIN_EXTENDED_B);
        LABEL(Character::UnicodeBlock, IPA_EXTENSIONS);
        LABEL(Character::UnicodeBlock, SPACING_MODIFIER_LETTERS);
        LABEL(Character::UnicodeBlock, COMBINING_DIACRITICAL_MARKS);
        LABEL(Character::UnicodeBlock, GREEK);
        LABEL(Character::UnicodeBlock, CYRILLIC);
        LABEL(Character::UnicodeBlock, ARMENIAN);
        LABEL(Character::UnicodeBlock, HEBREW);
        LABEL(Character::UnicodeBlock, ARABIC);
        LABEL(Character::UnicodeBlock, DEVANAGARI);
        LABEL(Character::UnicodeBlock, BENGALI);
        LABEL(Character::UnicodeBlock, GURMUKHI);
        LABEL(Character::UnicodeBlock, GUJARATI);
        LABEL(Character::UnicodeBlock, ORIYA);
        LABEL(Character::UnicodeBlock, TAMIL);
        LABEL(Character::UnicodeBlock, TELUGU);
        LABEL(Character::UnicodeBlock, KANNADA);
        LABEL(Character::UnicodeBlock, MALAYALAM);
        LABEL(Character::UnicodeBlock, THAI);
        LABEL(Character::UnicodeBlock, LAO);
        LABEL(Character::UnicodeBlock, TIBETAN);
        LABEL(Character::UnicodeBlock, GEORGIAN);
        LABEL(Character::UnicodeBlock, HANGUL_JAMO);
        LABEL(Character::UnicodeBlock, LATIN_EXTENDED_ADDITIONAL);
        LABEL(Character::UnicodeBlock, GREEK_EXTENDED);
        LABEL(Character::UnicodeBlock, GENERAL_PUNCTUATION);
        LABEL(Character::UnicodeBlock, SUPERSCRIPTS_AND_SUBSCRIPTS);
        LABEL(Character::UnicodeBlock, CURRENCY_SYMBOLS);
        LABEL(Character::UnicodeBlock, COMBINING_MARKS_FOR_SYMBOLS);
        LABEL(Character::UnicodeBlock, LETTERLIKE_SYMBOLS);
        LABEL(Character::UnicodeBlock, NUMBER_FORMS);
        LABEL(Character::UnicodeBlock, ARROWS);
        LABEL(Character::UnicodeBlock, MATHEMATICAL_OPERATORS);
        LABEL(Character::UnicodeBlock, MISCELLANEOUS_TECHNICAL);
        LABEL(Character::UnicodeBlock, CONTROL_PICTURES);
        LABEL(Character::UnicodeBlock, OPTICAL_CHARACTER_RECOGNITION);
        LABEL(Character::UnicodeBlock, ENCLOSED_ALPHANUMERICS);
        LABEL(Character::UnicodeBlock, BOX_DRAWING);
        LABEL(Character::UnicodeBlock, BLOCK_ELEMENTS);
        LABEL(Character::UnicodeBlock, GEOMETRIC_SHAPES);
        LABEL(Character::UnicodeBlock, MISCELLANEOUS_SYMBOLS);
        LABEL(Character::UnicodeBlock, DINGBATS);
        LABEL(Character::UnicodeBlock, CJK_SYMBOLS_AND_PUNCTUATION);
        LABEL(Character::UnicodeBlock, HIRAGANA);
        LABEL(Character::UnicodeBlock, KATAKANA);
        LABEL(Character::UnicodeBlock, BOPOMOFO);
        LABEL(Character::UnicodeBlock, HANGUL_COMPATIBILITY_JAMO);
        LABEL(Character::UnicodeBlock, KANBUN);
        LABEL(Character::UnicodeBlock, ENCLOSED_CJK_LETTERS_AND_MONTHS);
        LABEL(Character::UnicodeBlock, CJK_COMPATIBILITY);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS);
        LABEL(Character::UnicodeBlock, HANGUL_SYLLABLES);
        LABEL(Character::UnicodeBlock, PRIVATE_USE_AREA);
        LABEL(Character::UnicodeBlock, CJK_COMPATIBILITY_IDEOGRAPHS);
        LABEL(Character::UnicodeBlock, ALPHABETIC_PRESENTATION_FORMS);
        LABEL(Character::UnicodeBlock, ARABIC_PRESENTATION_FORMS_A);
        LABEL(Character::UnicodeBlock, COMBINING_HALF_MARKS);
        LABEL(Character::UnicodeBlock, CJK_COMPATIBILITY_FORMS);
        LABEL(Character::UnicodeBlock, SMALL_FORM_VARIANTS);
        LABEL(Character::UnicodeBlock, ARABIC_PRESENTATION_FORMS_B);
        LABEL(Character::UnicodeBlock, HALFWIDTH_AND_FULLWIDTH_FORMS);
        LABEL(Character::UnicodeBlock, SPECIALS);
        LABEL(Character::UnicodeBlock, SURROGATES_AREA);
        LABEL(Character::UnicodeBlock, SYRIAC);
        LABEL(Character::UnicodeBlock, THAANA);
        LABEL(Character::UnicodeBlock, SINHALA);
        LABEL(Character::UnicodeBlock, MYANMAR);
        LABEL(Character::UnicodeBlock, ETHIOPIC);
        LABEL(Character::UnicodeBlock, CHEROKEE);
        LABEL(Character::UnicodeBlock, UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS);
        LABEL(Character::UnicodeBlock, OGHAM);
        LABEL(Character::UnicodeBlock, RUNIC);
        LABEL(Character::UnicodeBlock, KHMER);
        LABEL(Character::UnicodeBlock, MONGOLIAN);
        LABEL(Character::UnicodeBlock, BRAILLE_PATTERNS);
        LABEL(Character::UnicodeBlock, CJK_RADICALS_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, KANGXI_RADICALS);
        LABEL(Character::UnicodeBlock, IDEOGRAPHIC_DESCRIPTION_CHARACTERS);
        LABEL(Character::UnicodeBlock, BOPOMOFO_EXTENDED);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A);
        LABEL(Character::UnicodeBlock, YI_SYLLABLES);
        LABEL(Character::UnicodeBlock, YI_RADICALS);
        LABEL(Character::UnicodeBlock, CYRILLIC_SUPPLEMENTARY);
        LABEL(Character::UnicodeBlock, TAGALOG);
        LABEL(Character::UnicodeBlock, HANUNOO);
        LABEL(Character::UnicodeBlock, BUHID);
        LABEL(Character::UnicodeBlock, TAGBANWA);
        LABEL(Character::UnicodeBlock, LIMBU);
        LABEL(Character::UnicodeBlock, TAI_LE);
        LABEL(Character::UnicodeBlock, KHMER_SYMBOLS);
        LABEL(Character::UnicodeBlock, PHONETIC_EXTENSIONS);
        LABEL(Character::UnicodeBlock, MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A);
        LABEL(Character::UnicodeBlock, SUPPLEMENTAL_ARROWS_A);
        LABEL(Character::UnicodeBlock, SUPPLEMENTAL_ARROWS_B);
        LABEL(Character::UnicodeBlock, MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B);
        LABEL(Character::UnicodeBlock, SUPPLEMENTAL_MATHEMATICAL_OPERATORS);
        LABEL(Character::UnicodeBlock, MISCELLANEOUS_SYMBOLS_AND_ARROWS);
        LABEL(Character::UnicodeBlock, KATAKANA_PHONETIC_EXTENSIONS);
        LABEL(Character::UnicodeBlock, YIJING_HEXAGRAM_SYMBOLS);
        LABEL(Character::UnicodeBlock, VARIATION_SELECTORS);
        LABEL(Character::UnicodeBlock, LINEAR_B_SYLLABARY);
        LABEL(Character::UnicodeBlock, LINEAR_B_IDEOGRAMS);
        LABEL(Character::UnicodeBlock, AEGEAN_NUMBERS);
        LABEL(Character::UnicodeBlock, OLD_ITALIC);
        LABEL(Character::UnicodeBlock, GOTHIC);
        LABEL(Character::UnicodeBlock, UGARITIC);
        LABEL(Character::UnicodeBlock, DESERET);
        LABEL(Character::UnicodeBlock, SHAVIAN);
        LABEL(Character::UnicodeBlock, OSMANYA);
        LABEL(Character::UnicodeBlock, CYPRIOT_SYLLABARY);
        LABEL(Character::UnicodeBlock, BYZANTINE_MUSICAL_SYMBOLS);
        LABEL(Character::UnicodeBlock, MUSICAL_SYMBOLS);
        LABEL(Character::UnicodeBlock, TAI_XUAN_JING_SYMBOLS);
        LABEL(Character::UnicodeBlock, MATHEMATICAL_ALPHANUMERIC_SYMBOLS);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B);
        LABEL(Character::UnicodeBlock, CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, TAGS);
        LABEL(Character::UnicodeBlock, VARIATION_SELECTORS_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, SUPPLEMENTARY_PRIVATE_USE_AREA_A);
        LABEL(Character::UnicodeBlock, SUPPLEMENTARY_PRIVATE_USE_AREA_B);
        LABEL(Character::UnicodeBlock, HIGH_SURROGATES);
        LABEL(Character::UnicodeBlock, HIGH_PRIVATE_USE_SURROGATES);
        LABEL(Character::UnicodeBlock, LOW_SURROGATES);
        LABEL(Character::UnicodeBlock, ARABIC_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, NKO);
        LABEL(Character::UnicodeBlock, SAMARITAN);
        LABEL(Character::UnicodeBlock, MANDAIC);
        LABEL(Character::UnicodeBlock, ETHIOPIC_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED);
        LABEL(Character::UnicodeBlock, NEW_TAI_LUE);
        LABEL(Character::UnicodeBlock, BUGINESE);
        LABEL(Character::UnicodeBlock, TAI_THAM);
        LABEL(Character::UnicodeBlock, BALINESE);
        LABEL(Character::UnicodeBlock, SUNDANESE);
        LABEL(Character::UnicodeBlock, BATAK);
        LABEL(Character::UnicodeBlock, LEPCHA);
        LABEL(Character::UnicodeBlock, OL_CHIKI);
        LABEL(Character::UnicodeBlock, VEDIC_EXTENSIONS);
        LABEL(Character::UnicodeBlock, PHONETIC_EXTENSIONS_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, COMBINING_DIACRITICAL_MARKS_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, GLAGOLITIC);
        LABEL(Character::UnicodeBlock, LATIN_EXTENDED_C);
        LABEL(Character::UnicodeBlock, COPTIC);
        LABEL(Character::UnicodeBlock, GEORGIAN_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, TIFINAGH);
        LABEL(Character::UnicodeBlock, ETHIOPIC_EXTENDED);
        LABEL(Character::UnicodeBlock, CYRILLIC_EXTENDED_A);
        LABEL(Character::UnicodeBlock, SUPPLEMENTAL_PUNCTUATION);
        LABEL(Character::UnicodeBlock, CJK_STROKES);
        LABEL(Character::UnicodeBlock, LISU);
        LABEL(Character::UnicodeBlock, VAI);
        LABEL(Character::UnicodeBlock, CYRILLIC_EXTENDED_B);
        LABEL(Character::UnicodeBlock, BAMUM);
        LABEL(Character::UnicodeBlock, MODIFIER_TONE_LETTERS);
        LABEL(Character::UnicodeBlock, LATIN_EXTENDED_D);
        LABEL(Character::UnicodeBlock, SYLOTI_NAGRI);
        LABEL(Character::UnicodeBlock, COMMON_INDIC_NUMBER_FORMS);
        LABEL(Character::UnicodeBlock, PHAGS_PA);
        LABEL(Character::UnicodeBlock, SAURASHTRA);
        LABEL(Character::UnicodeBlock, DEVANAGARI_EXTENDED);
        LABEL(Character::UnicodeBlock, KAYAH_LI);
        LABEL(Character::UnicodeBlock, REJANG);
        LABEL(Character::UnicodeBlock, HANGUL_JAMO_EXTENDED_A);
        LABEL(Character::UnicodeBlock, JAVANESE);
        LABEL(Character::UnicodeBlock, CHAM);
        LABEL(Character::UnicodeBlock, MYANMAR_EXTENDED_A);
        LABEL(Character::UnicodeBlock, TAI_VIET);
        LABEL(Character::UnicodeBlock, ETHIOPIC_EXTENDED_A);
        LABEL(Character::UnicodeBlock, MEETEI_MAYEK);
        LABEL(Character::UnicodeBlock, HANGUL_JAMO_EXTENDED_B);
        LABEL(Character::UnicodeBlock, VERTICAL_FORMS);
        LABEL(Character::UnicodeBlock, ANCIENT_GREEK_NUMBERS);
        LABEL(Character::UnicodeBlock, ANCIENT_SYMBOLS);
        LABEL(Character::UnicodeBlock, PHAISTOS_DISC);
        LABEL(Character::UnicodeBlock, LYCIAN);
        LABEL(Character::UnicodeBlock, CARIAN);
        LABEL(Character::UnicodeBlock, OLD_PERSIAN);
        LABEL(Character::UnicodeBlock, IMPERIAL_ARAMAIC);
        LABEL(Character::UnicodeBlock, PHOENICIAN);
        LABEL(Character::UnicodeBlock, LYDIAN);
        LABEL(Character::UnicodeBlock, KHAROSHTHI);
        LABEL(Character::UnicodeBlock, OLD_SOUTH_ARABIAN);
        LABEL(Character::UnicodeBlock, AVESTAN);
        LABEL(Character::UnicodeBlock, INSCRIPTIONAL_PARTHIAN);
        LABEL(Character::UnicodeBlock, INSCRIPTIONAL_PAHLAVI);
        LABEL(Character::UnicodeBlock, OLD_TURKIC);
        LABEL(Character::UnicodeBlock, RUMI_NUMERAL_SYMBOLS);
        LABEL(Character::UnicodeBlock, BRAHMI);
        LABEL(Character::UnicodeBlock, KAITHI);
        LABEL(Character::UnicodeBlock, CUNEIFORM);
        LABEL(Character::UnicodeBlock, CUNEIFORM_NUMBERS_AND_PUNCTUATION);
        LABEL(Character::UnicodeBlock, EGYPTIAN_HIEROGLYPHS);
        LABEL(Character::UnicodeBlock, BAMUM_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, KANA_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, ANCIENT_GREEK_MUSICAL_NOTATION);
        LABEL(Character::UnicodeBlock, COUNTING_ROD_NUMERALS);
        LABEL(Character::UnicodeBlock, MAHJONG_TILES);
        LABEL(Character::UnicodeBlock, DOMINO_TILES);
        LABEL(Character::UnicodeBlock, PLAYING_CARDS);
        LABEL(Character::UnicodeBlock, ENCLOSED_ALPHANUMERIC_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, ENCLOSED_IDEOGRAPHIC_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS);
        LABEL(Character::UnicodeBlock, EMOTICONS);
        LABEL(Character::UnicodeBlock, TRANSPORT_AND_MAP_SYMBOLS);
        LABEL(Character::UnicodeBlock, ALCHEMICAL_SYMBOLS);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D);
        LABEL(Character::UnicodeBlock, ARABIC_EXTENDED_A);
        LABEL(Character::UnicodeBlock, SUNDANESE_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, MEETEI_MAYEK_EXTENSIONS);
        LABEL(Character::UnicodeBlock, MEROITIC_HIEROGLYPHS);
        LABEL(Character::UnicodeBlock, MEROITIC_CURSIVE);
        LABEL(Character::UnicodeBlock, SORA_SOMPENG);
        LABEL(Character::UnicodeBlock, CHAKMA);
        LABEL(Character::UnicodeBlock, SHARADA);
        LABEL(Character::UnicodeBlock, TAKRI);
        LABEL(Character::UnicodeBlock, MIAO);
        LABEL(Character::UnicodeBlock, ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS);
        LABEL(Character::UnicodeBlock, COMBINING_DIACRITICAL_MARKS_EXTENDED);
        LABEL(Character::UnicodeBlock, MYANMAR_EXTENDED_B);
        LABEL(Character::UnicodeBlock, LATIN_EXTENDED_E);
        LABEL(Character::UnicodeBlock, COPTIC_EPACT_NUMBERS);
        LABEL(Character::UnicodeBlock, OLD_PERMIC);
        LABEL(Character::UnicodeBlock, ELBASAN);
        LABEL(Character::UnicodeBlock, CAUCASIAN_ALBANIAN);
        LABEL(Character::UnicodeBlock, LINEAR_A);
        LABEL(Character::UnicodeBlock, PALMYRENE);
        LABEL(Character::UnicodeBlock, NABATAEAN);
        LABEL(Character::UnicodeBlock, OLD_NORTH_ARABIAN);
        LABEL(Character::UnicodeBlock, MANICHAEAN);
        LABEL(Character::UnicodeBlock, PSALTER_PAHLAVI);
        LABEL(Character::UnicodeBlock, MAHAJANI);
        LABEL(Character::UnicodeBlock, SINHALA_ARCHAIC_NUMBERS);
        LABEL(Character::UnicodeBlock, KHOJKI);
        LABEL(Character::UnicodeBlock, KHUDAWADI);
        LABEL(Character::UnicodeBlock, GRANTHA);
        LABEL(Character::UnicodeBlock, TIRHUTA);
        LABEL(Character::UnicodeBlock, SIDDHAM);
        LABEL(Character::UnicodeBlock, MODI);
        LABEL(Character::UnicodeBlock, WARANG_CITI);
        LABEL(Character::UnicodeBlock, PAU_CIN_HAU);
        LABEL(Character::UnicodeBlock, MRO);
        LABEL(Character::UnicodeBlock, BASSA_VAH);
        LABEL(Character::UnicodeBlock, PAHAWH_HMONG);
        LABEL(Character::UnicodeBlock, DUPLOYAN);
        LABEL(Character::UnicodeBlock, SHORTHAND_FORMAT_CONTROLS);
        LABEL(Character::UnicodeBlock, MENDE_KIKAKUI);
        LABEL(Character::UnicodeBlock, ORNAMENTAL_DINGBATS);
        LABEL(Character::UnicodeBlock, GEOMETRIC_SHAPES_EXTENDED);
        LABEL(Character::UnicodeBlock, SUPPLEMENTAL_ARROWS_C);
        LABEL(Character::UnicodeBlock, CHEROKEE_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, HATRAN);
        LABEL(Character::UnicodeBlock, OLD_HUNGARIAN);
        LABEL(Character::UnicodeBlock, MULTANI);
        LABEL(Character::UnicodeBlock, AHOM);
        LABEL(Character::UnicodeBlock, EARLY_DYNASTIC_CUNEIFORM);
        LABEL(Character::UnicodeBlock, ANATOLIAN_HIEROGLYPHS);
        LABEL(Character::UnicodeBlock, SUTTON_SIGNWRITING);
        LABEL(Character::UnicodeBlock, SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E);
        LABEL(Character::UnicodeBlock, SYRIAC_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, CYRILLIC_EXTENDED_C);
        LABEL(Character::UnicodeBlock, OSAGE);
        LABEL(Character::UnicodeBlock, NEWA);
        LABEL(Character::UnicodeBlock, MONGOLIAN_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, MARCHEN);
        LABEL(Character::UnicodeBlock, IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION);
        LABEL(Character::UnicodeBlock, TANGUT);
        LABEL(Character::UnicodeBlock, TANGUT_COMPONENTS);
        LABEL(Character::UnicodeBlock, KANA_EXTENDED_A);
        LABEL(Character::UnicodeBlock, GLAGOLITIC_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, ADLAM);
        LABEL(Character::UnicodeBlock, MASARAM_GONDI);
        LABEL(Character::UnicodeBlock, ZANABAZAR_SQUARE);
        LABEL(Character::UnicodeBlock, NUSHU);
        LABEL(Character::UnicodeBlock, SOYOMBO);
        LABEL(Character::UnicodeBlock, BHAIKSUKI);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F);
        LABEL(Character::UnicodeBlock, GEORGIAN_EXTENDED);
        LABEL(Character::UnicodeBlock, HANIFI_ROHINGYA);
        LABEL(Character::UnicodeBlock, OLD_SOGDIAN);
        LABEL(Character::UnicodeBlock, SOGDIAN);
        LABEL(Character::UnicodeBlock, DOGRA);
        LABEL(Character::UnicodeBlock, GUNJALA_GONDI);
        LABEL(Character::UnicodeBlock, MAKASAR);
        LABEL(Character::UnicodeBlock, MEDEFAIDRIN);
        LABEL(Character::UnicodeBlock, MAYAN_NUMERALS);
        LABEL(Character::UnicodeBlock, INDIC_SIYAQ_NUMBERS);
        LABEL(Character::UnicodeBlock, CHESS_SYMBOLS);
        LABEL(Character::UnicodeBlock, ELYMAIC);
        LABEL(Character::UnicodeBlock, NANDINAGARI);
        LABEL(Character::UnicodeBlock, TAMIL_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS);
        LABEL(Character::UnicodeBlock, SMALL_KANA_EXTENSION);
        LABEL(Character::UnicodeBlock, NYIAKENG_PUACHUE_HMONG);
        LABEL(Character::UnicodeBlock, WANCHO);
        LABEL(Character::UnicodeBlock, OTTOMAN_SIYAQ_NUMBERS);
        LABEL(Character::UnicodeBlock, SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A);
        LABEL(Character::UnicodeBlock, YEZIDI);
        LABEL(Character::UnicodeBlock, CHORASMIAN);
        LABEL(Character::UnicodeBlock, DIVES_AKURU);
        LABEL(Character::UnicodeBlock, LISU_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, KHITAN_SMALL_SCRIPT);
        LABEL(Character::UnicodeBlock, TANGUT_SUPPLEMENT);
        LABEL(Character::UnicodeBlock, SYMBOLS_FOR_LEGACY_COMPUTING);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G);
        LABEL(Character::UnicodeBlock, ARABIC_EXTENDED_B);
        LABEL(Character::UnicodeBlock, VITHKUQI);
        LABEL(Character::UnicodeBlock, LATIN_EXTENDED_F);
        LABEL(Character::UnicodeBlock, OLD_UYGHUR);
        LABEL(Character::UnicodeBlock, UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED_A);
        LABEL(Character::UnicodeBlock, CYPRO_MINOAN);
        LABEL(Character::UnicodeBlock, TANGSA);
        LABEL(Character::UnicodeBlock, KANA_EXTENDED_B);
        LABEL(Character::UnicodeBlock, ZNAMENNY_MUSICAL_NOTATION);
        LABEL(Character::UnicodeBlock, LATIN_EXTENDED_G);
        LABEL(Character::UnicodeBlock, TOTO);
        LABEL(Character::UnicodeBlock, ETHIOPIC_EXTENDED_B);
        LABEL(Character::UnicodeBlock, ARABIC_EXTENDED_C);
        LABEL(Character::UnicodeBlock, DEVANAGARI_EXTENDED_A);
        LABEL(Character::UnicodeBlock, KAWI);
        LABEL(Character::UnicodeBlock, KAKTOVIK_NUMERALS);
        LABEL(Character::UnicodeBlock, CYRILLIC_EXTENDED_D);
        LABEL(Character::UnicodeBlock, NAG_MUNDARI);
        LABEL(Character::UnicodeBlock, CJK_UNIFIED_IDEOGRAPHS_EXTENSION_H);
        LABEL(Character::UnicodeBlock, UNASSIGNED);
        labels(table);
        return true;
    }

    ENUM_CLASS(Character, UnicodeScript) {
        Enum e = (Character::UnicodeScript) 0;
        static Object& table = labels(null);
        LABEL(Character::UnicodeScript, COMMON);
        LABEL(Character::UnicodeScript, LATIN);
        LABEL(Character::UnicodeScript, GREEK);
        LABEL(Character::UnicodeScript, CYRILLIC);
        LABEL(Character::UnicodeScript, ARMENIAN);
        LABEL(Character::UnicodeScript, HEBREW);
        LABEL(Character::UnicodeScript, ARABIC);
        LABEL(Character::UnicodeScript, SYRIAC);
        LABEL(Character::UnicodeScript, THAANA);
        LABEL(Character::UnicodeScript, DEVANAGARI);
        LABEL(Character::UnicodeScript, BENGALI);
        LABEL(Character::UnicodeScript, GURMUKHI);
        LABEL(Character::UnicodeScript, GUJARATI);
        LABEL(Character::UnicodeScript, ORIYA);
        LABEL(Character::UnicodeScript, TAMIL);
        LABEL(Character::UnicodeScript, TELUGU);
        LABEL(Character::UnicodeScript, KANNADA);
        LABEL(Character::UnicodeScript, MALAYALAM);
        LABEL(Character::UnicodeScript, SINHALA);
        LABEL(Character::UnicodeScript, THAI);
        LABEL(Character::UnicodeScript, LAO);
        LABEL(Character::UnicodeScript, TIBETAN);
        LABEL(Character::UnicodeScript, MYANMAR);
        LABEL(Character::UnicodeScript, GEORGIAN);
        LABEL(Character::UnicodeScript, HANGUL);
        LABEL(Character::UnicodeScript, ETHIOPIC);
        LABEL(Character::UnicodeScript, CHEROKEE);
        LABEL(Character::UnicodeScript, CANADIAN_ABORIGINAL);
        LABEL(Character::UnicodeScript, OGHAM);
        LABEL(Character::UnicodeScript, RUNIC);
        LABEL(Character::UnicodeScript, KHMER);
        LABEL(Character::UnicodeScript, MONGOLIAN);
        LABEL(Character::UnicodeScript, HIRAGANA);
        LABEL(Character::UnicodeScript, KATAKANA);
        LABEL(Character::UnicodeScript, BOPOMOFO);
        LABEL(Character::UnicodeScript, HAN);
        LABEL(Character::UnicodeScript, YI);
        LABEL(Character::UnicodeScript, OLD_ITALIC);
        LABEL(Character::UnicodeScript, GOTHIC);
        LABEL(Character::UnicodeScript, DESERET);
        LABEL(Character::UnicodeScript, INHERITED);
        LABEL(Character::UnicodeScript, TAGALOG);
        LABEL(Character::UnicodeScript, HANUNOO);
        LABEL(Character::UnicodeScript, BUHID);
        LABEL(Character::UnicodeScript, TAGBANWA);
        LABEL(Character::UnicodeScript, LIMBU);
        LABEL(Character::UnicodeScript, TAI_LE);
        LABEL(Character::UnicodeScript, LINEAR_B);
        LABEL(Character::UnicodeScript, UGARITIC);
        LABEL(Character::UnicodeScript, SHAVIAN);
        LABEL(Character::UnicodeScript, OSMANYA);
        LABEL(Character::UnicodeScript, CYPRIOT);
        LABEL(Character::UnicodeScript, BRAILLE);
        LABEL(Character::UnicodeScript, BUGINESE);
        LABEL(Character::UnicodeScript, COPTIC);
        LABEL(Character::UnicodeScript, NEW_TAI_LUE);
        LABEL(Character::UnicodeScript, GLAGOLITIC);
        LABEL(Character::UnicodeScript, TIFINAGH);
        LABEL(Character::UnicodeScript, SYLOTI_NAGRI);
        LABEL(Character::UnicodeScript, OLD_PERSIAN);
        LABEL(Character::UnicodeScript, KHAROSHTHI);
        LABEL(Character::UnicodeScript, BALINESE);
        LABEL(Character::UnicodeScript, CUNEIFORM);
        LABEL(Character::UnicodeScript, PHOENICIAN);
        LABEL(Character::UnicodeScript, PHAGS_PA);
        LABEL(Character::UnicodeScript, NKO);
        LABEL(Character::UnicodeScript, SUNDANESE);
        LABEL(Character::UnicodeScript, BATAK);
        LABEL(Character::UnicodeScript, LEPCHA);
        LABEL(Character::UnicodeScript, OL_CHIKI);
        LABEL(Character::UnicodeScript, VAI);
        LABEL(Character::UnicodeScript, SAURASHTRA);
        LABEL(Character::UnicodeScript, KAYAH_LI);
        LABEL(Character::UnicodeScript, REJANG);
        LABEL(Character::UnicodeScript, LYCIAN);
        LABEL(Character::UnicodeScript, CARIAN);
        LABEL(Character::UnicodeScript, LYDIAN);
        LABEL(Character::UnicodeScript, CHAM);
        LABEL(Character::UnicodeScript, TAI_THAM);
        LABEL(Character::UnicodeScript, TAI_VIET);
        LABEL(Character::UnicodeScript, AVESTAN);
        LABEL(Character::UnicodeScript, EGYPTIAN_HIEROGLYPHS);
        LABEL(Character::UnicodeScript, SAMARITAN);
        LABEL(Character::UnicodeScript, MANDAIC);
        LABEL(Character::UnicodeScript, LISU);
        LABEL(Character::UnicodeScript, BAMUM);
        LABEL(Character::UnicodeScript, JAVANESE);
        LABEL(Character::UnicodeScript, MEETEI_MAYEK);
        LABEL(Character::UnicodeScript, IMPERIAL_ARAMAIC);
        LABEL(Character::UnicodeScript, OLD_SOUTH_ARABIAN);
        LABEL(Character::UnicodeScript, INSCRIPTIONAL_PARTHIAN);
        LABEL(Character::UnicodeScript, INSCRIPTIONAL_PAHLAVI);
        LABEL(Character::UnicodeScript, OLD_TURKIC);
        LABEL(Character::UnicodeScript, BRAHMI);
        LABEL(Character::UnicodeScript, KAITHI);
        LABEL(Character::UnicodeScript, MEROITIC_HIEROGLYPHS);
        LABEL(Character::UnicodeScript, MEROITIC_CURSIVE);
        LABEL(Character::UnicodeScript, SORA_SOMPENG);
        LABEL(Character::UnicodeScript, CHAKMA);
        LABEL(Character::UnicodeScript, SHARADA);
        LABEL(Character::UnicodeScript, TAKRI);
        LABEL(Character::UnicodeScript, MIAO);
        LABEL(Character::UnicodeScript, CAUCASIAN_ALBANIAN);
        LABEL(Character::UnicodeScript, BASSA_VAH);
        LABEL(Character::UnicodeScript, DUPLOYAN);
        LABEL(Character::UnicodeScript, ELBASAN);
        LABEL(Character::UnicodeScript, GRANTHA);
        LABEL(Character::UnicodeScript, PAHAWH_HMONG);
        LABEL(Character::UnicodeScript, KHOJKI);
        LABEL(Character::UnicodeScript, LINEAR_A);
        LABEL(Character::UnicodeScript, MAHAJANI);
        LABEL(Character::UnicodeScript, MANICHAEAN);
        LABEL(Character::UnicodeScript, MENDE_KIKAKUI);
        LABEL(Character::UnicodeScript, MODI);
        LABEL(Character::UnicodeScript, MRO);
        LABEL(Character::UnicodeScript, OLD_NORTH_ARABIAN);
        LABEL(Character::UnicodeScript, NABATAEAN);
        LABEL(Character::UnicodeScript, PALMYRENE);
        LABEL(Character::UnicodeScript, PAU_CIN_HAU);
        LABEL(Character::UnicodeScript, OLD_PERMIC);
        LABEL(Character::UnicodeScript, PSALTER_PAHLAVI);
        LABEL(Character::UnicodeScript, SIDDHAM);
        LABEL(Character::UnicodeScript, KHUDAWADI);
        LABEL(Character::UnicodeScript, TIRHUTA);
        LABEL(Character::UnicodeScript, WARANG_CITI);
        LABEL(Character::UnicodeScript, AHOM);
        LABEL(Character::UnicodeScript, ANATOLIAN_HIEROGLYPHS);
        LABEL(Character::UnicodeScript, HATRAN);
        LABEL(Character::UnicodeScript, MULTANI);
        LABEL(Character::UnicodeScript, OLD_HUNGARIAN);
        LABEL(Character::UnicodeScript, SIGNWRITING);
        LABEL(Character::UnicodeScript, ADLAM);
        LABEL(Character::UnicodeScript, BHAIKSUKI);
        LABEL(Character::UnicodeScript, MARCHEN);
        LABEL(Character::UnicodeScript, NEWA);
        LABEL(Character::UnicodeScript, OSAGE);
        LABEL(Character::UnicodeScript, TANGUT);
        LABEL(Character::UnicodeScript, MASARAM_GONDI);
        LABEL(Character::UnicodeScript, NUSHU);
        LABEL(Character::UnicodeScript, SOYOMBO);
        LABEL(Character::UnicodeScript, ZANABAZAR_SQUARE);
        LABEL(Character::UnicodeScript, HANIFI_ROHINGYA);
        LABEL(Character::UnicodeScript, OLD_SOGDIAN);
        LABEL(Character::UnicodeScript, SOGDIAN);
        LABEL(Character::UnicodeScript, DOGRA);
        LABEL(Character::UnicodeScript, GUNJALA_GONDI);
        LABEL(Character::UnicodeScript, MAKASAR);
        LABEL(Character::UnicodeScript, MEDEFAIDRIN);
        LABEL(Character::UnicodeScript, ELYMAIC);
        LABEL(Character::UnicodeScript, NANDINAGARI);
        LABEL(Character::UnicodeScript, NYIAKENG_PUACHUE_HMONG);
        LABEL(Character::UnicodeScript, WANCHO);
        LABEL(Character::UnicodeScript, YEZIDI);
        LABEL(Character::UnicodeScript, CHORASMIAN);
        LABEL(Character::UnicodeScript, DIVES_AKURU);
        LABEL(Character::UnicodeScript, KHITAN_SMALL_SCRIPT);
        LABEL(Character::UnicodeScript, VITHKUQI);
        LABEL(Character::UnicodeScript, OLD_UYGHUR);
        LABEL(Character::UnicodeScript, CYPRO_MINOAN);
        LABEL(Character::UnicodeScript, TANGSA);
        LABEL(Character::UnicodeScript, TOTO);
        LABEL(Character::UnicodeScript, KAWI);
        LABEL(Character::UnicodeScript, NAG_MUNDARI);
        LABEL(Character::UnicodeScript, UNKNOWN);
        labels(table);
        return true;
    }

    ENUM_CLASS(Charset, ByteOrder) {
        Enum e = (Charset::ByteOrder) 0;
        static Object& table = labels(null);
        LABEL(Charset::ByteOrder, NATIVE_ENDIAN);
        LABEL(Charset::ByteOrder, LITTLE_ENDIAN);
        LABEL(Charset::ByteOrder, BIG_ENDIAN);
        labels(table);
        return true;
    }

    ENUM_CLASS(Charset, CodingErrorAction) {
        Enum e = (Charset::CodingErrorAction) 0;
        static Object& table = labels(null);
        LABEL(Charset::CodingErrorAction, REPORT);
        LABEL(Charset::CodingErrorAction, IGNORE);
        LABEL(Charset::CodingErrorAction, REPLACE);
        labels(table);
        return true;
    }

    ENUM_CLASS(File, CopyOption) {
        Enum e = (File::CopyOption) 0;
        static Object& table = labels(null);
        LABEL(File::CopyOption, DEFAULT_OPTION);
        LABEL(File::CopyOption, REPLACE_EXISTING);
        LABEL(File::CopyOption, COPY_ATTRIBUTES);
        LABEL(File::CopyOption, ATOMIC_MOVE);
        LABEL(File::CopyOption, NOFOLLOW_LINKS);
        LABEL(File::CopyOption, MOVE_BY_COPY);
        labels(table);
        return true;
    }

    ENUM_CLASS(File, Permission) {
        Enum e = (File::Permission) 0;
        static Object& table = labels(null);
        LABEL(File::Permission, OWNER_READ);
        LABEL(File::Permission, OWNER_WRITE);
        LABEL(File::Permission, OWNER_EXECUTE);
        LABEL(File::Permission, GROUP_READ);
        LABEL(File::Permission, GROUP_WRITE);
        LABEL(File::Permission, OWNER_EXECUTE);
        LABEL(File::Permission, OTHERS_READ);
        LABEL(File::Permission, OTHERS_WRITE);
        LABEL(File::Permission, OTHERS_EXECUTE);
        LABEL(File::Permission, OWNER_READ_WRITE);
        LABEL(File::Permission, OWNER_READ_EXECUTE);
        LABEL(File::Permission, OWNER_WRITE_EXECUTE);
        LABEL(File::Permission, OWNER_ALL);
        LABEL(File::Permission, GROUP_READ_WRITE);
        LABEL(File::Permission, GROUP_READ_EXECUTE);
        LABEL(File::Permission, GROUP_WRITE_EXECUTE);
        LABEL(File::Permission, GROUP_ALL);
        LABEL(File::Permission, OTHERS_READ_WRITE);
        LABEL(File::Permission, OTHERS_READ_EXECUTE);
        LABEL(File::Permission, OTHERS_WRITE_EXECUTE);
        LABEL(File::Permission, OTHERS_ALL);
        LABEL(File::Permission, ALL);
        labels(table);
        return true;
    }

    ENUM_CLASS(File, AccessMode) {
        Enum e = (File::AccessMode) 0;
        static Object& table = labels(null);
        LABEL(File::AccessMode, READ);
        LABEL(File::AccessMode, WRITE);
        LABEL(File::AccessMode, EXECUTE);
        LABEL(File::AccessMode, READ_WRITE);
        LABEL(File::AccessMode, READ_EXECUTE);
        LABEL(File::AccessMode, WRITE_EXECUTE);
        LABEL(File::AccessMode, FULL);
        labels(table);
        return true;
    }

    ENUM_CLASS(misc, REQUEST) {
        Enum e = (misc::REQUEST) 0;
        static Object& table = labels(null);
        LABEL(misc::REQUEST, REMOVE_QLF);
        LABEL(misc::REQUEST, CONST_QLF);
        LABEL(misc::REQUEST, VOL_QLF);
        LABEL(misc::REQUEST, REF_QLF);
        LABEL(misc::REQUEST, PTR_QLF);
        LABEL(misc::REQUEST, RVAL_QLF);
        LABEL(misc::REQUEST, ARRAY_QLF);
        LABEL(misc::REQUEST, FUNCTION_QLF);
        LABEL(misc::REQUEST, CLASS_QLF);
        LABEL(misc::REQUEST, METH_QLF);
        LABEL(misc::REQUEST, PRIME_QLF);
        LABEL(misc::REQUEST, OBJECT_QLF);
        LABEL(misc::REQUEST, SAME_FLAG);
        LABEL(misc::REQUEST, TEMPL_FLAG);
        LABEL(misc::REQUEST, MEMBER_FLAG);
        LABEL(misc::REQUEST, ENUM_QLF);
        LABEL(misc::REQUEST, ABSTRACT_FLAG);
        LABEL(misc::REQUEST, CTOR_FLAG);
        LABEL(misc::REQUEST, ASSIGN_FLAG);
        LABEL(misc::REQUEST, SUPER_FLAG);
        LABEL(misc::REQUEST, CONVERT_FLAG);
        LABEL(misc::REQUEST, COMPLET_FLAG);
        LABEL(misc::REQUEST, INT_FLAG);
        LABEL(misc::REQUEST, FLT_FLAG);
        LABEL(misc::REQUEST, CHR_FLAG);
        LABEL(misc::REQUEST, STR_FLAG);
        LABEL(misc::REQUEST, BOOLEAN_FLAG);
        LABEL(misc::REQUEST, CALL_FLAG);
        LABEL(misc::REQUEST, EQ_FLAG);
        LABEL(misc::REQUEST, LT_FLAG);
        LABEL(misc::REQUEST, DTOR_FLAG);
        LABEL(misc::REQUEST, FINAL_QLF);
        LABEL(misc::REQUEST, EMPTY_FLAG);
        LABEL(misc::REQUEST, UNION_QLF);
        LABEL(misc::REQUEST, POLY_FLAG);
        LABEL(misc::REQUEST, LITERAL_FLAG);
        LABEL(misc::REQUEST, TRIVIAL_FLAG);
        LABEL(misc::REQUEST, VOID_FLAG);
        LABEL(misc::REQUEST, SLIM_FLAG);
        LABEL(misc::REQUEST, SIZE_FLAG);
        labels(table);
        return true;
    }

    ENUM_CLASS(Pattern, Flags) {
        Enum e = (Pattern::Flags) 0;
        static Object& table = labels(null);
        LABEL(Pattern::Flags, DOTALL);
        LABEL(Pattern::Flags, CANON_EQ);
        LABEL(Pattern::Flags, LITERAL);
        LABEL(Pattern::Flags, COMMENTS);
        LABEL(Pattern::Flags, MULTILINE);
        LABEL(Pattern::Flags, UNIX_LINES);
        LABEL(Pattern::Flags, UNICODE_CASE);
        LABEL(Pattern::Flags, CASE_INSENSITIVE);
        LABEL(Pattern::Flags, UNICODE_CHARACTER_CLASS);
        labels(table);
        return true;
    }

    ENUM_CLASS(Normalizer, Form) {
        Enum e = (Normalizer::Form) 0;
        static Object& table = labels(null);
        LABEL(Normalizer::Form, NFC);
        LABEL(Normalizer::Form, NFD);
        LABEL(Normalizer::Form, NFKC);
        LABEL(Normalizer::Form, NFKD);
        labels(table);
        return true;
    }

    ENUM_CLASS(Temporal, ChronoField) {
        Enum e = (Temporal::ChronoField) 0;
        static Object& table = labels(null);
        LABEL(Temporal::ChronoField, ERA);
        LABEL(Temporal::ChronoField, YEAR);
        LABEL(Temporal::ChronoField, EPOCH_DAY);
        LABEL(Temporal::ChronoField, AMPM_OF_DAY);
        LABEL(Temporal::ChronoField, DAY_OF_WEEK);
        LABEL(Temporal::ChronoField, DAY_OF_YEAR);
        LABEL(Temporal::ChronoField, HOUR_OF_DAY);
        LABEL(Temporal::ChronoField, NANO_OF_DAY);
        LABEL(Temporal::ChronoField, YEAR_OF_ERA);
        LABEL(Temporal::ChronoField, DAY_OF_MONTH);
        LABEL(Temporal::ChronoField, HOUR_OF_AMPM);
        LABEL(Temporal::ChronoField, MICRO_OF_DAY);
        LABEL(Temporal::ChronoField, MILLI_OF_DAY);
        LABEL(Temporal::ChronoField, MINUTE_OF_DAY);
        LABEL(Temporal::ChronoField, MONTH_OF_YEAR);
        LABEL(Temporal::ChronoField, SECOND_OF_DAY);
        LABEL(Temporal::ChronoField, MINUTE_OF_HOUR);
        LABEL(Temporal::ChronoField, NANO_OF_SECOND);
        LABEL(Temporal::ChronoField, MICRO_OF_SECOND);
        LABEL(Temporal::ChronoField, MILLI_OF_SECOND);
        LABEL(Temporal::ChronoField, OFFSET_SECONDS);
        LABEL(Temporal::ChronoField, CLOCK_HOUR_OF_DAY);
        LABEL(Temporal::ChronoField, INSTANT_SECONDS);
        LABEL(Temporal::ChronoField, PROLEPTIC_MONTH);
        LABEL(Temporal::ChronoField, SECOND_OF_MINUTE);
        LABEL(Temporal::ChronoField, CLOCK_HOUR_OF_AMPM);
        LABEL(Temporal::ChronoField, ALIGNED_WEEK_OF_YEAR);
        LABEL(Temporal::ChronoField, ALIGNED_WEEK_OF_MONTH);
        LABEL(Temporal::ChronoField, ALIGNED_DAY_OF_WEEK_IN_YEAR);
        LABEL(Temporal::ChronoField, ALIGNED_DAY_OF_WEEK_IN_MONTH);
        labels(table);
        return true;
    }

    ENUM_CLASS(Temporal, ChronoUnit) {
        Enum e = (Temporal::ChronoUnit) 0;
        static Object& table = labels(null);
        LABEL(Temporal::ChronoUnit, DAYS);
        LABEL(Temporal::ChronoUnit, ERAS);
        LABEL(Temporal::ChronoUnit, HOURS);
        LABEL(Temporal::ChronoUnit, NANOS);
        LABEL(Temporal::ChronoUnit, WEEKS);
        LABEL(Temporal::ChronoUnit, YEARS);
        LABEL(Temporal::ChronoUnit, MICROS);
        LABEL(Temporal::ChronoUnit, MILLIS);
        LABEL(Temporal::ChronoUnit, MONTHS);
        LABEL(Temporal::ChronoUnit, DECADES);
        LABEL(Temporal::ChronoUnit, FOREVER);
        LABEL(Temporal::ChronoUnit, MINUTES);
        LABEL(Temporal::ChronoUnit, SECONDS);
        LABEL(Temporal::ChronoUnit, HALF_DAYS);
        LABEL(Temporal::ChronoUnit, CENTURIES);
        LABEL(Temporal::ChronoUnit, MILLENNIA);
        labels(table);
        return true;
    }

    ENUM_CLASS(Temporal, FormatStyle) {
        Enum e = (Temporal::FormatStyle) 0;
        static Object& table = labels(null);
        LABEL(Temporal::FormatStyle, FULL);
        LABEL(Temporal::FormatStyle, LONG);
        LABEL(Temporal::FormatStyle, SHORT);
        LABEL(Temporal::FormatStyle, MEDIUM);
        labels(table);
        return true;
    }

    ENUM_CLASS(Temporal, Constant) {
        Enum e = (Temporal::Constant) 0;
        static Object& table = labels(null);
        LABEL(Temporal::Constant, MAX_YEAR);
        LABEL(Temporal::Constant, MAX_MONTH);
        LABEL(Temporal::Constant, MAX_DAY_OF_YEAR);
        LABEL(Temporal::Constant, MAX_DAY_OF_MONTH);
        LABEL(Temporal::Constant, MAX_DAY_OF_WEEK);
        LABEL(Temporal::Constant, MAX_HOUR);
        LABEL(Temporal::Constant, MAX_MINUTE);
        LABEL(Temporal::Constant, MAX_SECOND);
        LABEL(Temporal::Constant, MAX_MILLI);
        LABEL(Temporal::Constant, MAX_NANOSECOND);
        LABEL(Temporal::Constant, MAX_OFFSET_SECONDS);
        LABEL(Temporal::Constant, MIN_YEAR);
        LABEL(Temporal::Constant, MIN_MONTH);
        LABEL(Temporal::Constant, MIN_DAY_OF_YEAR);
        LABEL(Temporal::Constant, MIN_DAY_OF_MONTH);
        LABEL(Temporal::Constant, MIN_DAY_OF_WEEK);
        LABEL(Temporal::Constant, MIN_HOUR);
        LABEL(Temporal::Constant, MIN_MINUTE);
        LABEL(Temporal::Constant, MIN_SECOND);
        LABEL(Temporal::Constant, MIN_MILLI);
        LABEL(Temporal::Constant, MIN_NANOSECOND);
        LABEL(Temporal::Constant, MIN_OFFSET_SECONDS);
        labels(table);
        return true;
    }

    ENUM_CLASS(LocalDate, Month) {
        Enum e = (LocalDate::Month) 0;
        static Object& table = labels(null);
        LABEL(LocalDate::Month, JANUARY);
        LABEL(LocalDate::Month, FEBRUARY);
        LABEL(LocalDate::Month, MARCH);
        LABEL(LocalDate::Month, APRIL);
        LABEL(LocalDate::Month, MAY);
        LABEL(LocalDate::Month, JUNE);
        LABEL(LocalDate::Month, JULY);
        LABEL(LocalDate::Month, AUGUST);
        LABEL(LocalDate::Month, SEPTEMBER);
        LABEL(LocalDate::Month, OCTOBER);
        LABEL(LocalDate::Month, NOVEMBER);
        LABEL(LocalDate::Month, DECEMBER);
        labels(table);
        return true;
    }

    ENUM_CLASS(LocalDate, DayOfWeek) {
        Enum e = (LocalDate::DayOfWeek) 0;
        static Object& table = labels(null);
        LABEL(LocalDate::DayOfWeek, MONDAY);
        LABEL(LocalDate::DayOfWeek, TUESDAY);
        LABEL(LocalDate::DayOfWeek, WEDNESDAY);
        LABEL(LocalDate::DayOfWeek, TUESDAY);
        LABEL(LocalDate::DayOfWeek, FRIDAY);
        LABEL(LocalDate::DayOfWeek, SATURDAY);
        LABEL(LocalDate::DayOfWeek, SUNDAY);
        labels(table);
        return true;
    }

    ENUM_CLASS(ZoneOffsetTransitionRule, TimeDefinition) {
        Enum e = (ZoneOffsetTransitionRule::TimeDefinition) 0;
        static Object& table = labels(null);
        LABEL(ZoneOffsetTransitionRule::TimeDefinition, UTC);
        LABEL(ZoneOffsetTransitionRule::TimeDefinition, WALL);
        LABEL(ZoneOffsetTransitionRule::TimeDefinition, STANDARD);
        labels(table);
        return true;
    }

    ENUM_CLASS(Locale, Category) {
        Enum e = (Locale::Category) 0;
        static Object& table = labels(null);
        LABEL(Locale::Category, FORMAT);
        LABEL(Locale::Category, DISPLAY);
        labels(table);
        return true;
    }

    ENUM_CLASS(Locale, CountryCode) {
        Enum e = (Locale::CountryCode) 0;
        static Object& table = labels(null);
        LABEL(Locale::CountryCode, PART3);
        LABEL(Locale::CountryCode, PART1_ALPHA2);
        LABEL(Locale::CountryCode, PART1_ALPHA3);
        labels(table);
        return true;
    }

#undef LABEL
#undef ENUM_CLASS
} // core
