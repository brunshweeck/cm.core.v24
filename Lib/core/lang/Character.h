//
// Created by brunshweeck on 26 mai 2024.
//

#ifndef CORE24_CHARACTER_H
#define CORE24_CHARACTER_H

#include <core/lang/Comparable.h>
#include <core/lang/Byte.h>

namespace core {
  /**
   * The @c Character class wraps a value of the primitive
   * type @c gchar in an object. An object of class
   * @c Character contains a single field whose type is
   * @c gchar.
   * <p>
   * In addition, this class provides a large number of static methods for
   * determining a character's category (lowercase letter, digit, etc.)
   * and for converting characters from uppercase to lowercase and vice
   * versa.
   * </p>
   * <h3> Unicode Conformance </h3>
   * <p>
   * The fields and methods of class @c Character are defined in terms
   * of character information from the Unicode Standard, specifically the
   * <i>UnicodeData</i> file that is part of the Unicode Character Database.
   * This file specifies properties including name and category for every
   * assigned Unicode code point or character range. The file is available
   * from the Unicode Consortium at
   * <a href="http://www.unicode.org">http://www.unicode.org</a>.
   * </p>
   * <p>
   * Character information is based on the Unicode Standard, version 15.1.
   * </p>
   * <h3> Unicode Character Representations </h3>
   *
   * <p>
   * The @c gchar data type (and therefore the value that a
   * @c Character object encapsulates) are based on the
   * original Unicode specification, which defined characters as
   * fixed-width 16-bit entities. The Unicode Standard has since been
   * changed to allow for characters whose representation requires more
   * than 16 bits.  The range of legal <em>code point</em>s is now
   * U+0000 to U+10FFFF, known as <em>Unicode scalar value</em>.
   * (Refer to the <a href="http://www.unicode.org/reports/tr27/#notation"><i>
   * definition</i></a> of the U+<i>n</i> notation in the Unicode
   * Standard.)
   * </p>
   * <p>
   * <a id="BMP">The set of characters from U+0000 to U+FFFF</a> is
   * sometimes referred to as the <em>Basic Multilingual Plane (BMP)</em>.
   * <a id="supplementary">Characters</a> whose code points are greater
   * than U+FFFF are called <em>supplementary character</em>s.  The Java
   * platform uses the UTF-16 representation in @c gchar arrays and
   * in the @c String and @c XString classes. In
   * this representation, supplementary characters are represented as a pair
   * of @c gchar values, the first from the <em>high-surrogates</em>
   * range, (@c '\uD800' - @c '\uDBFF'), the second from the
   * <em>low-surrogates</em> range (@c '\uDC00' - @c '\uDFFF').
   * </p>
   * <p>
   * A @c gchar value, therefore, represents Basic
   * Multilingual Plane (BMP) code points, including the surrogate
   * code points, or code units of the UTF-16 encoding. An
   * @c gint value represents all Unicode code points,
   * including supplementary code points. The lower (least significant)
   * 21 bits of @c gint are used to represent Unicode code
   * points and the upper (most significant) 11 bits must be zero.
   * Unless otherwise specified, the behavior with respect to
   * supplementary characters and surrogate @c gchar values is
   * as follows: <br/>
   *
   * - The methods that only accept a @c gchar value cannot support
   * supplementary characters. They treat @c gchar values from the
   * surrogate ranges as undefined characters. For example,
   * @code Character::isLetter('\D840') @endcode returns @c false, even though
   * this specific value if followed by any low-surrogate value in a string
   * would represent a letter. <br/>
   *
   * - The methods that accept an @c gint value support all
   * Unicode characters, including supplementary characters. For
   * example, @code Character.isLetter(0x2F81A) @endcode returns
   * @c true because the code point value represents a letter
   * (a CJK ideograph). <br/>
   *
   * </p>
   * <p>
   * In the Core24 documentation, <em>Unicode code point</em> is
   * used for character values in the range between U+0000 and U+10FFFF,
   * and <em>Unicode code unit</em> is used for 16-bit
   * @c gchar values that are code units of the <em>UTF-16</em>
   * encoding. For more information on Unicode terminology, refer to the
   * <a href="http://www.unicode.org/glossary/">Unicode Glossary</a>.
   * </p>
   *
   * @see https://www.unicode.org/reports/tr27 Unicode 3.1.0
   */
  class Character final : public Comparable<Character> {
  public:
    /**
     * The minimum radix available for conversion to and from strings.
     * The constant value of this field is the smallest value permitted
     * for the radix argument in radix-conversion methods such as the
     * @c digit method, the @c forDigit method, and the
     * @c toString method of class @c Integer.
     */
    static CORE_FAST gint MIN_RADIX = 2;

    /**
     * The maximum radix available for conversion to and from strings.
     * The constant value of this field is the largest value permitted
     * for the radix argument in radix-conversion methods such as the
     * @c digit method, the @c forDigit method, and the
     * @c toString method of class @c Integer.
     *
                                         */
    static CORE_FAST gint MAX_RADIX = 36;

    /**
     * The constant value of this field is the smallest value of type
     * @c gchar, @c '\u0000'.
     *
     *
     */
    static CORE_FAST gchar MIN_VALUE = u'\u0000';

    /**
     * The constant value of this field is the largest value of type
     * @c gchar, @c '\uFFFF'.
     *
     *
     */
    static CORE_FAST gchar MAX_VALUE = u'\uFFFF';

    /*
     * Normative general types
     */

    /*
     * General character types
     */
    enum Category {
      /**
      * General category "Cn" in the Unicode specification.
      *
      */
      UNASSIGNED = 13,

      /**
      * General category "Lu" in the Unicode specification.
      *
      */
      UPPERCASE_LETTER = 14,

      /**
      * General category "Ll" in the Unicode specification.
      *
      */
      LOWERCASE_LETTER = 15,

      /**
      * General category "Lt" in the Unicode specification.
      *
      */
      TITLECASE_LETTER = 16,

      /**
      * General category "Lm" in the Unicode specification.
      *
      */
      MODIFIER_LETTER = 17,

      /**
      * General category "Lo" in the Unicode specification.
      *
      */
      OTHER_LETTER = 18,

      /**
      * General category "Mn" in the Unicode specification.
      *
      */
      NON_SPACING_MARK = 0,

      /**
      * General category "Me" in the Unicode specification.
      *
      */
      ENCLOSING_MARK = 2,

      /**
      * General category "Mc" in the Unicode specification.
      *
      */
      COMBINING_SPACING_MARK = 1,

      /**
      * General category "Nd" in the Unicode specification.
      *
      */
      DECIMAL_DIGIT_NUMBER = 3,

      /**
      * General category "Nl" in the Unicode specification.
      *
      */
      LETTER_NUMBER = 4,

      /**
      * General category "No" in the Unicode specification.
      *
      */
      OTHER_NUMBER = 5,

      /**
      * General category "Zs" in the Unicode specification.
      *
      */
      SPACE_SEPARATOR = 6,

      /**
      * General category "Zl" in the Unicode specification.
      *
      */
      LINE_SEPARATOR = 7,

      /**
      * General category "Zp" in the Unicode specification.
      *
      */
      PARAGRAPH_SEPARATOR = 8,

      /**
      * General category "Cc" in the Unicode specification.
      *
      */
      CONTROL = 9,

      /**
      * General category "Cf" in the Unicode specification.
      *
      */
      FORMAT = 10,

      /**
      * General category "Co" in the Unicode specification.
      *
      */
      PRIVATE_USE = 12,

      /**
      * General category "Cs" in the Unicode specification.
      *
      */
      SURROGATE = 11,

      /**
      * General category "Pd" in the Unicode specification.
      *
      */
      DASH_PUNCTUATION = 20,

      /**
      * General category "Ps" in the Unicode specification.
      *
      */
      START_PUNCTUATION = 21,

      /**
      * General category "Pe" in the Unicode specification.
      *
      */
      END_PUNCTUATION = 22,

      /**
      * General category "Pc" in the Unicode specification.
      *
      */
      CONNECTOR_PUNCTUATION = 19,

      /**
      * General category "Po" in the Unicode specification.
      *
      */
      OTHER_PUNCTUATION = 25,

      /**
      * General category "Sm" in the Unicode specification.
      *
      */
      MATH_SYMBOL = 26,

      /**
      * General category "Sc" in the Unicode specification.
      *
      */
      CURRENCY_SYMBOL = 27,

      /**
      * General category "Sk" in the Unicode specification.
      *
      */
      MODIFIER_SYMBOL = 28,

      /**
      * General category "So" in the Unicode specification.
      *
      */
      OTHER_SYMBOL = 29,

      /**
      * General category "Pi" in the Unicode specification.
      *
      */
      INITIAL_QUOTE_PUNCTUATION = 23,

      /**
      * General category "Pf" in the Unicode specification.
      *
      */
      FINAL_QUOTE_PUNCTUATION = 24,
    };

    /**
     *  Directionality of character
     */
    enum Directionality {
      /**
      * Undefined Directionality character type. Undefined character
      * values have undefined directionality in the Unicode specification.
      */
      DIRECTIONALITY_UNDEFINED = 0,

      /**
      * Strong Directionality character type "L" in the Unicode specification.
      */
      DIRECTIONALITY_LEFT_TO_RIGHT,

      /**
      * Strong Directionality character type "T" in the Unicode specification.
      */
      DIRECTIONALITY_RIGHT_TO_LEFT,

      /**
      * Strong Directionality character type "AL" in the Unicode specification.
      */
      DIRECTIONALITY_RIGHT_TO_LEFT_ARABIC,

      /**
      * Weak Directionality character type "EN" in the Unicode specification.
      */
      DIRECTIONALITY_EUROPEAN_NUMBER,

      /**
      * Weak Directionality character type "ES" in the Unicode specification.
      */
      DIRECTIONALITY_EUROPEAN_NUMBER_SEPARATOR,

      /**
      * Weak Directionality character type "ET" in the Unicode specification.
      */
      DIRECTIONALITY_EUROPEAN_NUMBER_TERMINATOR,

      /**
      * Weak Directionality character type "AN" in the Unicode specification.
      */
      DIRECTIONALITY_ARABIC_NUMBER,

      /**
      * Weak Directionality character type "CS" in the Unicode specification.
      */
      DIRECTIONALITY_COMMON_NUMBER_SEPARATOR,

      /**
      * Weak Directionality character type "NSM" in the Unicode specification.
      */
      DIRECTIONALITY_NON_SPACING_MARK,

      /**
      * Weak Directionality character type "BN" in the Unicode specification.
      */
      DIRECTIONALITY_BOUNDARY_NEUTRAL,

      /**
      * Neutral Directionality character type "B" in the Unicode specification.
      */
      DIRECTIONALITY_PARAGRAPH_SEPARATOR,

      /**
      * Neutral Directionality character type "S" in the Unicode specification.
      */
      DIRECTIONALITY_SEGMENT_SEPARATOR,

      /**
      * Neutral Directionality character type "WS" in the Unicode specification.
      */
      DIRECTIONALITY_WHITESPACE,

      /**
      * Neutral Directionality character type "ON" in the Unicode specification.
      */
      DIRECTIONALITY_OTHER_NEUTRALS,

      /**
      * Strong Directionality character type "LRE" in the Unicode specification.
      */
      DIRECTIONALITY_LEFT_TO_RIGHT_EMBEDDING,

      /**
      * Strong Directionality character type "LRO" in the Unicode specification.
      */
      DIRECTIONALITY_LEFT_TO_RIGHT_OVERRIDE,

      /**
      * Strong Directionality character type "RLE" in the Unicode specification.
      */
      DIRECTIONALITY_RIGHT_TO_LEFT_EMBEDDING,

      /**
      * Strong Directionality character type "RLO" in the Unicode specification.
      */
      DIRECTIONALITY_RIGHT_TO_LEFT_OVERRIDE,

      /**
      * Weak Directionality character type "PDF" in the Unicode specification.
      */
      DIRECTIONALITY_POP_DIRECTIONAL_FORMAT,

      /**
      * Weak Directionality character type "LRI" in the Unicode specification.
      */
      DIRECTIONALITY_LEFT_TO_RIGHT_ISOLATE,

      /**
      * Weak Directionality character type "RLI" in the Unicode specification.
      */
      DIRECTIONALITY_RIGHT_TO_LEFT_ISOLATE,

      /**
      * Weak Directionality character type "FSI" in the Unicode specification.
      */
      DIRECTIONALITY_FIRST_STRONG_ISOLATE,

      /**
      * Weak Directionality character type "PDI" in the Unicode specification.
      */
      DIRECTIONALITY_POP_DIRECTIONAL_ISOLATE,
    };

    /**
     * The minimum value of a
     * <a href="http://www.unicode.org/glossary/#high_surrogate_code_unit">
     * Unicode high-surrogate code unit</a>
     * in the UTF-16 encoding, constant @c '\uD800'.
     * A high-surrogate is also known as a <i>leading-surrogate</i>.
     *
     *
     */
    static CORE_FAST gchar MIN_HIGH_SURROGATE = 0xD800;

    /**
     * The maximum value of a
     * <a href="http://www.unicode.org/glossary/#high_surrogate_code_unit">
     * Unicode high-surrogate code unit</a>
     * in the UTF-16 encoding, constant @c '\uDBFF'.
     * A high-surrogate is also known as a <i>leading-surrogate</i>.
     *
     *
     */
    static CORE_FAST gchar MAX_HIGH_SURROGATE = 0xDBFF;

    /**
     * The minimum value of a
     * <a href="http://www.unicode.org/glossary/#low_surrogate_code_unit">
     * Unicode low-surrogate code unit</a>
     * in the UTF-16 encoding, constant @c '\uDC00'.
     * A low-surrogate is also known as a <i>trailing-surrogate</i>.
     *
     *
     */
    static CORE_FAST gchar MIN_LOW_SURROGATE = 0xDC00;

    /**
     * The maximum value of a
     * <a href="http://www.unicode.org/glossary/#low_surrogate_code_unit">
     * Unicode low-surrogate code unit</a>
     * in the UTF-16 encoding, constant @c '\uDFFF'.
     * A low-surrogate is also known as a <i>trailing-surrogate</i>.
     *
     *
     */
    static CORE_FAST gchar MAX_LOW_SURROGATE = 0xDFFF;

    /**
     * The minimum value of a Unicode surrogate code unit in the
     * UTF-16 encoding, constant @c '\uD800'.
     *
     *
     */
    static CORE_FAST gchar MIN_SURROGATE = MIN_HIGH_SURROGATE;

    /**
     * The maximum value of a Unicode surrogate code unit in the
     * UTF-16 encoding, constant @c '\uDFFF'.
     *
     *
     */
    static CORE_FAST gchar MAX_SURROGATE = MAX_LOW_SURROGATE;

    /**
     * The minimum value of a
     * <a href="http://www.unicode.org/glossary/#supplementary_code_point">
     * Unicode supplementary code point</a>, constant @c U+10000.
     *
     *
     */
    static CORE_FAST gint MIN_SUPPLEMENTARY = 0x010000;

    /**
     * The minimum value of a
     * <a href="http://www.unicode.org/glossary/#code_point">
     * Unicode code point</a>, constant @c U+0000.
     *
     *
     */
    static CORE_FAST gint MIN_CODE_POINT = 0x000000;

    /**
     * The maximum value of a
     * <a href="http://www.unicode.org/glossary/#code_point">
     * Unicode code point</a>, constant @c U+10FFFF.
     *
     *
     */
    static CORE_FAST gint MAX_CODE_POINT = 0x10FFFF;

    // See http://www.unicode.org/Public/UNIDATA/Blocks.txt
    // for the latest specification of Unicode Blocks.

    /**
     * A family of character subsets representing the character blocks in the
     * Unicode specification. Character blocks generally define characters
     * used for a specific script or purpose. A character is contained by
     * at most one Unicode block.
     *
     *
     */
    enum class UnicodeBlock : gshort {
      /**
       * Constant for the "Basic Latin" Unicode character block.
       */
      BASIC_LATIN,

      /**
       * Constant for the "Latin-1 Supplement" Unicode character block.
       */
      LATIN_1_SUPPLEMENT,

      /**
       * Constant for the "Latin Extended-A" Unicode character block.
       */
      LATIN_EXTENDED_A,

      /**
       * Constant for the "Latin Extended-B" Unicode character block.
       */
      LATIN_EXTENDED_B,

      /**
       * Constant for the "IPA Extensions" Unicode character block.
       */
      IPA_EXTENSIONS,

      /**
       * Constant for the "Spacing Modifier Letters" Unicode character block.
       */
      SPACING_MODIFIER_LETTERS,

      /**
       * Constant for the "Combining Diacritical Marks" Unicode character block.
       */
      COMBINING_DIACRITICAL_MARKS,

      /**
       * Constant for the "Greek and Coptic" Unicode character block.
       * <p>
       * This block was previously known as the "Greek" block.
       * </p>
       */
      GREEK,

      /**
       * Constant for the "Cyrillic" Unicode character block.
       */
      CYRILLIC,

      /**
       * Constant for the "Armenian" Unicode character block.
       */
      ARMENIAN,

      /**
       * Constant for the "Hebrew" Unicode character block.
       */
      HEBREW,

      /**
       * Constant for the "Arabic" Unicode character block.
       */
      ARABIC,

      /**
       * Constant for the "Devanagari" Unicode character block.
       */
      DEVANAGARI,

      /**
       * Constant for the "Bengali" Unicode character block.
       */
      BENGALI,

      /**
       * Constant for the "Gurmukhi" Unicode character block.
       */
      GURMUKHI,

      /**
       * Constant for the "Gujarati" Unicode character block.
       */
      GUJARATI,

      /**
       * Constant for the "Oriya" Unicode character block.
       */
      ORIYA,

      /**
       * Constant for the "Tamil" Unicode character block.
       */
      TAMIL,

      /**
       * Constant for the "Telugu" Unicode character block.
       */
      TELUGU,

      /**
       * Constant for the "Kannada" Unicode character block.
       */
      KANNADA,

      /**
       * Constant for the "Malayalam" Unicode character block.
       */
      MALAYALAM,

      /**
       * Constant for the "Thai" Unicode character block.
       */
      THAI,

      /**
       * Constant for the "Lao" Unicode character block.
       */
      LAO,

      /**
       * Constant for the "Tibetan" Unicode character block.
       */
      TIBETAN,

      /**
       * Constant for the "Georgian" Unicode character block.
       */
      GEORGIAN,

      /**
       * Constant for the "Hangul Jamo" Unicode character block.
       */
      HANGUL_JAMO,

      /**
       * Constant for the "Latin Extended Additional" Unicode character block.
       */
      LATIN_EXTENDED_ADDITIONAL,

      /**
       * Constant for the "Greek Extended" Unicode character block.
       */
      GREEK_EXTENDED,

      /**
       * Constant for the "General Punctuation" Unicode character block.
       */
      GENERAL_PUNCTUATION,

      /**
       * Constant for the "Superscripts and Subscripts" Unicode character
       * block.
       */
      SUPERSCRIPTS_AND_SUBSCRIPTS,

      /**
       * Constant for the "Currency Symbols" Unicode character block.
       */
      CURRENCY_SYMBOLS,

      /**
       * Constant for the "Combining Diacritical Marks for Symbols" Unicode
       * character block.
       * <p>
       * This block was previously known as "Combining Marks for Symbols".
       * </p>
       */
      COMBINING_MARKS_FOR_SYMBOLS,

      /**
       * Constant for the "Letterlike Symbols" Unicode character block.
       */
      LETTERLIKE_SYMBOLS,

      /**
       * Constant for the "Number Forms" Unicode character block.
       */
      NUMBER_FORMS,

      /**
       * Constant for the "Arrows" Unicode character block.
       */
      ARROWS,

      /**
       * Constant for the "Mathematical Operators" Unicode character block.
       */
      MATHEMATICAL_OPERATORS,

      /**
       * Constant for the "Miscellaneous Technical" Unicode character block.
       */
      MISCELLANEOUS_TECHNICAL,

      /**
       * Constant for the "Control Pictures" Unicode character block.
       */
      CONTROL_PICTURES,

      /**
       * Constant for the "Optical Character Recognition" Unicode character block.
       */
      OPTICAL_CHARACTER_RECOGNITION,

      /**
       * Constant for the "Enclosed Alphanumerics" Unicode character block.
       */
      ENCLOSED_ALPHANUMERICS,

      /**
       * Constant for the "Box Drawing" Unicode character block.
       */
      BOX_DRAWING,

      /**
       * Constant for the "Block Elements" Unicode character block.
       */
      BLOCK_ELEMENTS,

      /**
       * Constant for the "Geometric Shapes" Unicode character block.
       */
      GEOMETRIC_SHAPES,

      /**
       * Constant for the "Miscellaneous Symbols" Unicode character block.
       */
      MISCELLANEOUS_SYMBOLS,

      /**
       * Constant for the "Dingbats" Unicode character block.
       */
      DINGBATS,

      /**
       * Constant for the "CJK Symbols and Punctuation" Unicode character block.
       */
      CJK_SYMBOLS_AND_PUNCTUATION,

      /**
       * Constant for the "Hiragana" Unicode character block.
       */
      HIRAGANA,

      /**
       * Constant for the "Katakana" Unicode character block.
       */
      KATAKANA,

      /**
       * Constant for the "Bopomofo" Unicode character block.
       */
      BOPOMOFO,

      /**
       * Constant for the "Hangul Compatibility Jamo" Unicode character block.
       */
      HANGUL_COMPATIBILITY_JAMO,

      /**
       * Constant for the "Kanbun" Unicode character block.
       */
      KANBUN,

      /**
       * Constant for the "Enclosed CJK Letters and Months" Unicode character block.
       */
      ENCLOSED_CJK_LETTERS_AND_MONTHS,

      /**
       * Constant for the "CJK Compatibility" Unicode character block.
       */
      CJK_COMPATIBILITY,

      /**
       * Constant for the "CJK Unified Ideographs" Unicode character block.
       */
      CJK_UNIFIED_IDEOGRAPHS,

      /**
       * Constant for the "Hangul Syllables" Unicode character block.
       */
      HANGUL_SYLLABLES,

      /**
       * Constant for the "private Use Area" Unicode character block.
       */
      PRIVATE_USE_AREA,

      /**
       * Constant for the "CJK Compatibility Ideographs" Unicode character
       * block.
       */
      CJK_COMPATIBILITY_IDEOGRAPHS,

      /**
       * Constant for the "Alphabetic Presentation Forms" Unicode character block.
       */
      ALPHABETIC_PRESENTATION_FORMS,

      /**
       * Constant for the "Arabic Presentation Forms-A" Unicode character
       * block.
       */
      ARABIC_PRESENTATION_FORMS_A,

      /**
       * Constant for the "Combining Half Marks" Unicode character block.
       */
      COMBINING_HALF_MARKS,

      /**
       * Constant for the "CJK Compatibility Forms" Unicode character block.
       */
      CJK_COMPATIBILITY_FORMS,

      /**
       * Constant for the "Small Form Variants" Unicode character block.
       */
      SMALL_FORM_VARIANTS,

      /**
       * Constant for the "Arabic Presentation Forms-B" Unicode character block.
       */
      ARABIC_PRESENTATION_FORMS_B,

      /**
       * Constant for the "Halfwidth and Fullwidth Forms" Unicode character
       * block.
       */
      HALFWIDTH_AND_FULLWIDTH_FORMS,

      /**
       * Constant for the "Specials" Unicode character block.
       */
      SPECIALS,

      /**
       * \deprecated
       * Instead of URROGATES_AREA, use HIGH_SURROGATES,
       * HIGH_PRIVATE_USE_SURROGATES, and LOW_SURROGATES.
       * These constants match the block definitions of the Unicode Standard.
       */
      SURROGATES_AREA,

      /**
       * Constant for the "Syriac" Unicode character block.
       */
      SYRIAC,

      /**
       * Constant for the "Thaana" Unicode character block.
       */
      THAANA,

      /**
       * Constant for the "Sinhala" Unicode character block.
       */
      SINHALA,

      /**
       * Constant for the "Myanmar" Unicode character block.
       */
      MYANMAR,

      /**
       * Constant for the "Ethiopic" Unicode character block.
       */
      ETHIOPIC,

      /**
       * Constant for the "Cherokee" Unicode character block.
       */
      CHEROKEE,

      /**
       * Constant for the "Unified Canadian Aboriginal Syllabics" Unicode character block.
       */
      UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS,

      /**
       * Constant for the "Ogham" Unicode character block.
       */
      OGHAM,

      /**
       * Constant for the "Runic" Unicode character block.
       */
      RUNIC,

      /**
       * Constant for the "Khmer" Unicode character block.
       */
      KHMER,

      /**
       * Constant for the "Mongolian" Unicode character block.
       */
      MONGOLIAN,

      /**
       * Constant for the "Braille Patterns" Unicode character block.
       */
      BRAILLE_PATTERNS,

      /**
       * Constant for the "CJK Radicals Supplement" Unicode character block.
       */
      CJK_RADICALS_SUPPLEMENT,

      /**
       * Constant for the "Kangxi Radicals" Unicode character block.
       */
      KANGXI_RADICALS,

      /**
       * Constant for the "Ideographic Description Characters" Unicode character block.
       */
      IDEOGRAPHIC_DESCRIPTION_CHARACTERS,

      /**
       * Constant for the "Bopomofo Extended" Unicode character block.
       */
      BOPOMOFO_EXTENDED,

      /**
       * Constant for the "CJK Unified Ideographs Extension A" Unicode character block.
       */
      CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A,

      /**
       * Constant for the "Yi Syllables" Unicode character block.
       */
      YI_SYLLABLES,

      /**
       * Constant for the "Yi Radicals" Unicode character block.
       */
      YI_RADICALS,

      /**
       * Constant for the "Cyrillic Supplement" Unicode character block.
       * This block was previously known as the "Cyrillic Supplementary" block.
       *
       */
      CYRILLIC_SUPPLEMENTARY,

      /**
       * Constant for the "Tagalog" Unicode character block.
       */
      TAGALOG,

      /**
       * Constant for the "Hanunoo" Unicode character block.
       */
      HANUNOO,

      /**
       * Constant for the "Buhid" Unicode character block.
       */
      BUHID,

      /**
       * Constant for the "Tagbanwa" Unicode character block.
       */
      TAGBANWA,

      /**
       * Constant for the "Limbu" Unicode character block.
       */
      LIMBU,

      /**
       * Constant for the "Tai Le" Unicode character block.
       */
      TAI_LE,

      /**
       * Constant for the "Khmer Symbols" Unicode character block.
       */
      KHMER_SYMBOLS,

      /**
       * Constant for the "Phonetic Extensions" Unicode character block.
       */
      PHONETIC_EXTENSIONS,

      /**
       * Constant for the "Miscellaneous Mathematical Symbols-A" Unicode character block.
       */
      MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A,

      /**
       * Constant for the "Supplemental Arrows-A" Unicode character block.
       */
      SUPPLEMENTAL_ARROWS_A,

      /**
       * Constant for the "Supplemental Arrows-B" Unicode character block.
       */
      SUPPLEMENTAL_ARROWS_B,

      /**
       * Constant for the "Miscellaneous Mathematical Symbols-B" Unicode
       * character block.
       */
      MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B,

      /**
       * Constant for the "Supplemental Mathematical Operators" Unicode
       * character block.
       */
      SUPPLEMENTAL_MATHEMATICAL_OPERATORS,

      /**
       * Constant for the "Miscellaneous Symbols and Arrows" Unicode character
       * block.
       */
      MISCELLANEOUS_SYMBOLS_AND_ARROWS,

      /**
       * Constant for the "Katakana Phonetic Extensions" Unicode character
       * block.
       */
      KATAKANA_PHONETIC_EXTENSIONS,

      /**
       * Constant for the "Yijing Hexagram Symbols" Unicode character block.
       */
      YIJING_HEXAGRAM_SYMBOLS,

      /**
       * Constant for the "Variation Selectors" Unicode character block.
       */
      VARIATION_SELECTORS,

      /**
       * Constant for the "Linear B Syllabary" Unicode character block.
       */
      LINEAR_B_SYLLABARY,

      /**
       * Constant for the "Linear B Ideograms" Unicode character block.
       */
      LINEAR_B_IDEOGRAMS,

      /**
       * Constant for the "Aegean Numbers" Unicode character block.
       */
      AEGEAN_NUMBERS,

      /**
       * Constant for the "Old Italic" Unicode character block.
       */
      OLD_ITALIC,

      /**
       * Constant for the "Gothic" Unicode character block.
       */
      GOTHIC,

      /**
       * Constant for the "Ugaritic" Unicode character block.
       */
      UGARITIC,

      /**
       * Constant for the "Deseret" Unicode character block.
       */
      DESERET,

      /**
       * Constant for the "Shavian" Unicode character block.
       */
      SHAVIAN,

      /**
       * Constant for the "Osmanya" Unicode character block.
       */
      OSMANYA,

      /**
       * Constant for the "Cypriot Syllabary" Unicode character block.
       */
      CYPRIOT_SYLLABARY,

      /**
       * Constant for the "Byzantine Musical Symbols" Unicode character block.
       */
      BYZANTINE_MUSICAL_SYMBOLS,

      /**
       * Constant for the "Musical Symbols" Unicode character block.
       */
      MUSICAL_SYMBOLS,

      /**
       * Constant for the "Tai Xuan Jing Symbols" Unicode character block.
       */
      TAI_XUAN_JING_SYMBOLS,

      /**
       * Constant for the "Mathematical Alphanumeric Symbols" Unicode
       * character block.
       */
      MATHEMATICAL_ALPHANUMERIC_SYMBOLS,

      /**
       * Constant for the "CJK Unified Ideographs Extension B" Unicode
       * character block.
       */
      CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B,

      /**
       * Constant for the "CJK Compatibility Ideographs Supplement" Unicode character block.
       */
      CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT,

      /**
       * Constant for the "Tags" Unicode character block.
       */
      TAGS,

      /**
       * Constant for the "Variation Selectors Supplement" Unicode character
       * block.
       */
      VARIATION_SELECTORS_SUPPLEMENT,

      /**
       * Constant for the "Supplementary private Use Area-A" Unicode character
       * block.
       */
      SUPPLEMENTARY_PRIVATE_USE_AREA_A,

      /**
       * Constant for the "Supplementary private Use Area-B" Unicode character
       * block.
       */
      SUPPLEMENTARY_PRIVATE_USE_AREA_B,

      /**
       * Constant for the "High Surrogates" Unicode character block.
       * This block represents codepoint values in the high surrogate
       * range: U+D800 through U+DB7F
       *
       *
       */
      HIGH_SURROGATES,

      /**
       * Constant for the "High private Use Surrogates" Unicode character
       * block.
       * This block represents codepoint values in the private use high
       * surrogate range: U+DB80 through U+DBFF
       *
       *
       */
      HIGH_PRIVATE_USE_SURROGATES,

      /**
       * Constant for the "Low Surrogates" Unicode character block.
       * This block represents codepoint values in the low surrogate
       * range: U+DC00 through U+DFFF
       *
       *
       */
      LOW_SURROGATES,

      /**
       * Constant for the "Arabic Supplement" Unicode character block.
       */
      ARABIC_SUPPLEMENT,

      /**
       * Constant for the "NKo" Unicode character block.
       */
      NKO,

      /**
       * Constant for the "Samaritan" Unicode character block.
       */
      SAMARITAN,

      /**
       * Constant for the "Mandaic" Unicode character block.
       */
      MANDAIC,

      /**
       * Constant for the "Ethiopic Supplement" Unicode character block.
       */
      ETHIOPIC_SUPPLEMENT,

      /**
       * Constant for the "Unified Canadian Aboriginal Syllabics Extended"
       * Unicode character block.
       */
      UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED,

      /**
       * Constant for the "New Tai Lue" Unicode character block.
       */
      NEW_TAI_LUE,

      /**
       * Constant for the "Buginese" Unicode character block.
       */
      BUGINESE,

      /**
       * Constant for the "Tai Tham" Unicode character block.
       */
      TAI_THAM,

      /**
       * Constant for the "Balinese" Unicode character block.
       */
      BALINESE,

      /**
       * Constant for the "Sundanese" Unicode character block.
       */
      SUNDANESE,

      /**
       * Constant for the "Batak" Unicode character block.
       */
      BATAK,

      /**
       * Constant for the "Lepcha" Unicode character block.
       */
      LEPCHA,

      /**
       * Constant for the "Ol Chiki" Unicode character block.
       */
      OL_CHIKI,

      /**
       * Constant for the "Vedic Extensions" Unicode character block.
       */
      VEDIC_EXTENSIONS,

      /**
       * Constant for the "Phonetic Extensions Supplement" Unicode character
       * block.
       */
      PHONETIC_EXTENSIONS_SUPPLEMENT,

      /**
       * Constant for the "Combining Diacritical Marks Supplement" Unicode
       * character block.
       */
      COMBINING_DIACRITICAL_MARKS_SUPPLEMENT,

      /**
       * Constant for the "Glagolitic" Unicode character block.
       */
      GLAGOLITIC,

      /**
       * Constant for the "Latin Extended-C" Unicode character block.
       */
      LATIN_EXTENDED_C,

      /**
       * Constant for the "Coptic" Unicode character block.
       */
      COPTIC,

      /**
       * Constant for the "Georgian Supplement" Unicode character block.
       */
      GEORGIAN_SUPPLEMENT,

      /**
       * Constant for the "Tifinagh" Unicode character block.
       */
      TIFINAGH,

      /**
       * Constant for the "Ethiopic Extended" Unicode character block.
       */
      ETHIOPIC_EXTENDED,

      /**
       * Constant for the "Cyrillic Extended-A" Unicode character block.
       */
      CYRILLIC_EXTENDED_A,

      /**
       * Constant for the "Supplemental Punctuation" Unicode character block.
       */
      SUPPLEMENTAL_PUNCTUATION,

      /**
       * Constant for the "CJK Strokes" Unicode character block.
       */
      CJK_STROKES,

      /**
       * Constant for the "Lisu" Unicode character block.
       */
      LISU,

      /**
       * Constant for the "Vai" Unicode character block.
       */
      VAI,

      /**
       * Constant for the "Cyrillic Extended-B" Unicode character block.
       */
      CYRILLIC_EXTENDED_B,

      /**
       * Constant for the "Bamum" Unicode character block.
       */
      BAMUM,

      /**
       * Constant for the "Modifier Tone Letters" Unicode character block.
       */
      MODIFIER_TONE_LETTERS,

      /**
       * Constant for the "Latin Extended-D" Unicode character block.
       */
      LATIN_EXTENDED_D,

      /**
       * Constant for the "Syloti Nagri" Unicode character block.
       */
      SYLOTI_NAGRI,

      /**
       * Constant for the "Common Indic Number Forms" Unicode character block.
       */
      COMMON_INDIC_NUMBER_FORMS,

      /**
       * Constant for the "Phags-pa" Unicode character block.
       */
      PHAGS_PA,

      /**
       * Constant for the "Saurashtra" Unicode character block.
       */
      SAURASHTRA,

      /**
       * Constant for the "Devanagari Extended" Unicode character block.
       */
      DEVANAGARI_EXTENDED,

      /**
       * Constant for the "Kayah Li" Unicode character block.
       */
      KAYAH_LI,

      /**
       * Constant for the "Rejang" Unicode character block.
       */
      REJANG,

      /**
       * Constant for the "Hangul Jamo Extended-A" Unicode character block.
       */
      HANGUL_JAMO_EXTENDED_A,

      /**
       * Constant for the "Javanese" Unicode character block.
       */
      JAVANESE,

      /**
       * Constant for the "Cham" Unicode character block.
       */
      CHAM,

      /**
       * Constant for the "Myanmar Extended-A" Unicode character block.
       */
      MYANMAR_EXTENDED_A,

      /**
       * Constant for the "Tai Viet" Unicode character block.
       */
      TAI_VIET,

      /**
       * Constant for the "Ethiopic Extended-A" Unicode character block.
       */
      ETHIOPIC_EXTENDED_A,

      /**
       * Constant for the "Meetei Mayek" Unicode character block.
       */
      MEETEI_MAYEK,

      /**
       * Constant for the "Hangul Jamo Extended-B" Unicode character block.
       */
      HANGUL_JAMO_EXTENDED_B,

      /**
       * Constant for the "Vertical Forms" Unicode character block.
       */
      VERTICAL_FORMS,

      /**
       * Constant for the "Ancient Greek Numbers" Unicode character block.
       */
      ANCIENT_GREEK_NUMBERS,

      /**
       * Constant for the "Ancient Symbols" Unicode character block.
       */
      ANCIENT_SYMBOLS,

      /**
       * Constant for the "Phaistos Disc" Unicode character block.
       */
      PHAISTOS_DISC,

      /**
       * Constant for the "Lycian" Unicode character block.
       */
      LYCIAN,

      /**
       * Constant for the "Carian" Unicode character block.
       */
      CARIAN,

      /**
       * Constant for the "Old Persian" Unicode character block.
       */
      OLD_PERSIAN,

      /**
       * Constant for the "Imperial Aramaic" Unicode character block.
       */
      IMPERIAL_ARAMAIC,

      /**
       * Constant for the "Phoenician" Unicode character block.
       */
      PHOENICIAN,

      /**
       * Constant for the "Lydian" Unicode character block.
       */
      LYDIAN,

      /**
       * Constant for the "Kharoshthi" Unicode character block.
       */
      KHAROSHTHI,

      /**
       * Constant for the "Old South Arabian" Unicode character block.
       */
      OLD_SOUTH_ARABIAN,

      /**
       * Constant for the "Avestan" Unicode character block.
       */
      AVESTAN,

      /**
       * Constant for the "Inscriptional Parthian" Unicode character block.
       */
      INSCRIPTIONAL_PARTHIAN,

      /**
       * Constant for the "Inscriptional Pahlavi" Unicode character block.
       */
      INSCRIPTIONAL_PAHLAVI,

      /**
       * Constant for the "Old Turkic" Unicode character block.
       */
      OLD_TURKIC,

      /**
       * Constant for the "Rumi Numeral Symbols" Unicode character block.
       */
      RUMI_NUMERAL_SYMBOLS,

      /**
       * Constant for the "Brahmi" Unicode character block.
       */
      BRAHMI,

      /**
       * Constant for the "Kaithi" Unicode character block.
       */
      KAITHI,

      /**
       * Constant for the "Cuneiform" Unicode character block.
       */
      CUNEIFORM,

      /**
       * Constant for the "Cuneiform Numbers and Punctuation" Unicode
       * character block.
       */
      CUNEIFORM_NUMBERS_AND_PUNCTUATION,

      /**
       * Constant for the "Egyptian Hieroglyphs" Unicode character block.
       */
      EGYPTIAN_HIEROGLYPHS,

      /**
       * Constant for the "Bamum Supplement" Unicode character block.
       */
      BAMUM_SUPPLEMENT,

      /**
       * Constant for the "Kana Supplement" Unicode character block.
       */
      KANA_SUPPLEMENT,

      /**
       * Constant for the "Ancient Greek Musical Notation" Unicode character
       * block.
       */
      ANCIENT_GREEK_MUSICAL_NOTATION,

      /**
       * Constant for the "Counting Rod Numerals" Unicode character block.
       */
      COUNTING_ROD_NUMERALS,

      /**
       * Constant for the "Mahjong Tiles" Unicode character block.
       */
      MAHJONG_TILES,

      /**
       * Constant for the "Domino Tiles" Unicode character block.
       */
      DOMINO_TILES,

      /**
       * Constant for the "Playing Cards" Unicode character block.
       */
      PLAYING_CARDS,

      /**
       * Constant for the "Enclosed Alphanumeric Supplement" Unicode character
       * block.
       */
      ENCLOSED_ALPHANUMERIC_SUPPLEMENT,

      /**
       * Constant for the "Enclosed Ideographic Supplement" Unicode character
       * block.
       */
      ENCLOSED_IDEOGRAPHIC_SUPPLEMENT,

      /**
       * Constant for the "Miscellaneous Symbols And Pictographs" Unicode
       * character block.
       */
      MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS,

      /**
       * Constant for the "Emoticons" Unicode character block.
       */
      EMOTICONS,

      /**
       * Constant for the "Transport And Map Symbols" Unicode character block.
       */
      TRANSPORT_AND_MAP_SYMBOLS,

      /**
       * Constant for the "Alchemical Symbols" Unicode character block.
       */
      ALCHEMICAL_SYMBOLS,

      /**
       * Constant for the "CJK Unified Ideographs Extension C" Unicode
       * character block.
       */
      CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C,

      /**
       * Constant for the "CJK Unified Ideographs Extension D" Unicode
       * character block.
       */
      CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D,

      /**
       * Constant for the "Arabic Extended-A" Unicode character block.
       */
      ARABIC_EXTENDED_A,

      /**
       * Constant for the "Sundanese Supplement" Unicode character block.
       */
      SUNDANESE_SUPPLEMENT,

      /**
       * Constant for the "Meetei Mayek Extensions" Unicode character block.
       */
      MEETEI_MAYEK_EXTENSIONS,

      /**
       * Constant for the "Meroitic Hieroglyphs" Unicode character block.
       */
      MEROITIC_HIEROGLYPHS,

      /**
       * Constant for the "Meroitic Cursive" Unicode character block.
       */
      MEROITIC_CURSIVE,

      /**
       * Constant for the "Sora Sompeng" Unicode character block.
       */
      SORA_SOMPENG,

      /**
       * Constant for the "Chakma" Unicode character block.
       */
      CHAKMA,

      /**
       * Constant for the "Sharada" Unicode character block.
       */
      SHARADA,

      /**
       * Constant for the "Takri" Unicode character block.
       */
      TAKRI,

      /**
       * Constant for the "Miao" Unicode character block.
       */
      MIAO,

      /**
       * Constant for the "Arabic Mathematical Alphabetic Symbols" Unicode
       * character block.
       */
      ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS,

      /**
       * Constant for the "Combining Diacritical Marks Extended" Unicode
       * character block.
       */
      COMBINING_DIACRITICAL_MARKS_EXTENDED,

      /**
       * Constant for the "Myanmar Extended-B" Unicode character block.
       */
      MYANMAR_EXTENDED_B,

      /**
       * Constant for the "Latin Extended-E" Unicode character block.
       */
      LATIN_EXTENDED_E,

      /**
       * Constant for the "Coptic Epact Numbers" Unicode character block.
       */
      COPTIC_EPACT_NUMBERS,

      /**
       * Constant for the "Old Permic" Unicode character block.
       */
      OLD_PERMIC,

      /**
       * Constant for the "Elbasan" Unicode character block.
       */
      ELBASAN,

      /**
       * Constant for the "Caucasian Albanian" Unicode character block.
       */
      CAUCASIAN_ALBANIAN,

      /**
       * Constant for the "Linear A" Unicode character block.
       */
      LINEAR_A,

      /**
       * Constant for the "Palmyrene" Unicode character block.
       */
      PALMYRENE,

      /**
       * Constant for the "Nabataean" Unicode character block.
       */
      NABATAEAN,

      /**
       * Constant for the "Old North Arabian" Unicode character block.
       */
      OLD_NORTH_ARABIAN,

      /**
       * Constant for the "Manichaean" Unicode character block.
       */
      MANICHAEAN,

      /**
       * Constant for the "Psalter Pahlavi" Unicode character block.
       */
      PSALTER_PAHLAVI,

      /**
       * Constant for the "Mahajani" Unicode character block.
       */
      MAHAJANI,

      /**
       * Constant for the "Sinhala Archaic Numbers" Unicode character block.
       */
      SINHALA_ARCHAIC_NUMBERS,

      /**
       * Constant for the "Khojki" Unicode character block.
       */
      KHOJKI,

      /**
       * Constant for the "Khudawadi" Unicode character block.
       */
      KHUDAWADI,

      /**
       * Constant for the "Grantha" Unicode character block.
       */
      GRANTHA,

      /**
       * Constant for the "Tirhuta" Unicode character block.
       */
      TIRHUTA,

      /**
       * Constant for the "Siddham" Unicode character block.
       */
      SIDDHAM,

      /**
       * Constant for the "Modi" Unicode character block.
       */
      MODI,

      /**
       * Constant for the "Warang Citi" Unicode character block.
       */
      WARANG_CITI,

      /**
       * Constant for the "Pau Cin Hau" Unicode character block.
       */
      PAU_CIN_HAU,

      /**
       * Constant for the "Mro" Unicode character block.
       */
      MRO,

      /**
       * Constant for the "Bassa Vah" Unicode character block.
       */
      BASSA_VAH,

      /**
       * Constant for the "Pahawh Hmong" Unicode character block.
       */
      PAHAWH_HMONG,

      /**
       * Constant for the "Duployan" Unicode character block.
       */
      DUPLOYAN,

      /**
       * Constant for the "Shorthand Format Controls" Unicode character block.
       */
      SHORTHAND_FORMAT_CONTROLS,

      /**
       * Constant for the "Mende Kikakui" Unicode character block.
       */
      MENDE_KIKAKUI,

      /**
       * Constant for the "Ornamental Dingbats" Unicode character block.
       */
      ORNAMENTAL_DINGBATS,

      /**
       * Constant for the "Geometric Shapes Extended" Unicode character block.
       */
      GEOMETRIC_SHAPES_EXTENDED,

      /**
       * Constant for the "Supplemental Arrows-C" Unicode character block.
       */
      SUPPLEMENTAL_ARROWS_C,

      /**
       * Constant for the "Cherokee Supplement" Unicode character block.
       */
      CHEROKEE_SUPPLEMENT,

      /**
       * Constant for the "Hatran" Unicode character block.
       */
      HATRAN,

      /**
       * Constant for the "Old Hungarian" Unicode character block.
       */
      OLD_HUNGARIAN,

      /**
       * Constant for the "Multani" Unicode character block.
       */
      MULTANI,

      /**
       * Constant for the "Ahom" Unicode character block.
       */
      AHOM,

      /**
       * Constant for the "Early Dynastic Cuneiform" Unicode character block.
       */
      EARLY_DYNASTIC_CUNEIFORM,

      /**
       * Constant for the "Anatolian Hieroglyphs" Unicode character block.
       */
      ANATOLIAN_HIEROGLYPHS,

      /**
       * Constant for the "Sutton SignWriting" Unicode character block.
       */
      SUTTON_SIGNWRITING,

      /**
       * Constant for the "Supplemental Symbols and Pictographs" Unicode
       * character block.
       */
      SUPPLEMENTAL_SYMBOLS_AND_PICTOGRAPHS,

      /**
       * Constant for the "CJK Unified Ideographs Extension E" Unicode
       * character block.
       */
      CJK_UNIFIED_IDEOGRAPHS_EXTENSION_E,

      /**
       * Constant for the "Syriac Supplement" Unicode
       * character block.
       */
      SYRIAC_SUPPLEMENT,

      /**
       * Constant for the "Cyrillic Extended-C" Unicode
       * character block.
       */
      CYRILLIC_EXTENDED_C,

      /**
       * Constant for the "Osage" Unicode
       * character block.
       */
      OSAGE,

      /**
       * Constant for the "Newa" Unicode
       * character block.
       */
      NEWA,

      /**
       * Constant for the "Mongolian Supplement" Unicode
       * character block.
       */
      MONGOLIAN_SUPPLEMENT,

      /**
       * Constant for the "Marchen" Unicode
       * character block.
       */
      MARCHEN,

      /**
       * Constant for the "Ideographic Symbols and Punctuation" Unicode
       * character block.
       */
      IDEOGRAPHIC_SYMBOLS_AND_PUNCTUATION,

      /**
       * Constant for the "Tangut" Unicode
       * character block.
       */
      TANGUT,

      /**
       * Constant for the "Tangut Components" Unicode
       * character block.
       */
      TANGUT_COMPONENTS,

      /**
       * Constant for the "Kana Extended-A" Unicode
       * character block.
       */
      KANA_EXTENDED_A,
      /**
       * Constant for the "Glagolitic Supplement" Unicode
       * character block.
       */
      GLAGOLITIC_SUPPLEMENT,
      /**
       * Constant for the "Adlam" Unicode
       * character block.
       */
      ADLAM,

      /**
       * Constant for the "Masaram Gondi" Unicode
       * character block.
       */
      MASARAM_GONDI,

      /**
       * Constant for the "Zanabazar Square" Unicode
       * character block.
       */
      ZANABAZAR_SQUARE,

      /**
       * Constant for the "Nushu" Unicode
       * character block.
       */
      NUSHU,

      /**
       * Constant for the "Soyombo" Unicode
       * character block.
       */
      SOYOMBO,

      /**
       * Constant for the "Bhaiksuki" Unicode
       * character block.
       */
      BHAIKSUKI,

      /**
       * Constant for the "CJK Unified Ideographs Extension F" Unicode
       * character block.
       */
      CJK_UNIFIED_IDEOGRAPHS_EXTENSION_F,
      /**
       * Constant for the "Georgian Extended" Unicode
       * character block.
       */
      GEORGIAN_EXTENDED,

      /**
       * Constant for the "Hanifi Rohingya" Unicode
       * character block.
       */
      HANIFI_ROHINGYA,

      /**
       * Constant for the "Old Sogdian" Unicode
       * character block.
       */
      OLD_SOGDIAN,

      /**
       * Constant for the "Sogdian" Unicode
       * character block.
       */
      SOGDIAN,

      /**
       * Constant for the "Dogra" Unicode
       * character block.
       */
      DOGRA,

      /**
       * Constant for the "Gunjala Gondi" Unicode
       * character block.
       */
      GUNJALA_GONDI,

      /**
       * Constant for the "Makasar" Unicode
       * character block.
       */
      MAKASAR,

      /**
       * Constant for the "Medefaidrin" Unicode
       * character block.
       */
      MEDEFAIDRIN,

      /**
       * Constant for the "Mayan Numerals" Unicode
       * character block.
       */
      MAYAN_NUMERALS,

      /**
       * Constant for the "Indic Siyaq Numbers" Unicode
       * character block.
       */
      INDIC_SIYAQ_NUMBERS,

      /**
       * Constant for the "Chess Symbols" Unicode
       * character block.
       */
      CHESS_SYMBOLS,

      /**
       * Constant for the "Elymaic" Unicode
       * character block.
       */
      ELYMAIC,

      /**
       * Constant for the "Nandinagari" Unicode
       * character block.
       */
      NANDINAGARI,

      /**
       * Constant for the "Tamil Supplement" Unicode
       * character block.
       */
      TAMIL_SUPPLEMENT,

      /**
       * Constant for the "Egyptian Hieroglyph Format Controls" Unicode
       * character block.
       */
      EGYPTIAN_HIEROGLYPH_FORMAT_CONTROLS,

      /**
       * Constant for the "Small Kana Extension" Unicode
       * character block.
       */
      SMALL_KANA_EXTENSION,

      /**
       * Constant for the "Nyiakeng Puachue Hmong" Unicode
       * character block.
       */
      NYIAKENG_PUACHUE_HMONG,

      /**
       * Constant for the "Wancho" Unicode
       * character block.
       */
      WANCHO,

      /**
       * Constant for the "Ottoman Siyaq Numbers" Unicode
       * character block.
       */
      OTTOMAN_SIYAQ_NUMBERS,

      /**
       * Constant for the "Symbols and Pictographs Extended-A" Unicode
       * character block.
       */
      SYMBOLS_AND_PICTOGRAPHS_EXTENDED_A,

      /**
       * Constant for the "Yezidi" Unicode
       * character block.
       */
      YEZIDI,

      /**
       * Constant for the "Chorasmian" Unicode
       * character block.
       */
      CHORASMIAN,

      /**
       * Constant for the "Dives Akuru" Unicode
       * character block.
       */
      DIVES_AKURU,

      /**
       * Constant for the "Lisu Supplement" Unicode
       * character block.
       */
      LISU_SUPPLEMENT,

      /**
       * Constant for the "Khitan Small Script" Unicode
       * character block.
       */
      KHITAN_SMALL_SCRIPT,

      /**
       * Constant for the "Tangut Supplement" Unicode
       * character block.
       */
      TANGUT_SUPPLEMENT,

      /**
       * Constant for the "Symbols for Legacy Computing" Unicode
       * character block.
       */
      SYMBOLS_FOR_LEGACY_COMPUTING,

      /**
       * Constant for the "CJK Unified Ideographs Extension G" Unicode
       * character block.
       */
      CJK_UNIFIED_IDEOGRAPHS_EXTENSION_G,

      /**
       * Constant for the "Arabic Extended-B" Unicode
       * character block.
       */
      ARABIC_EXTENDED_B,

      /**
       * Constant for the "Vithkuqi" Unicode
       * character block.
       */
      VITHKUQI,

      /**
       * Constant for the "Latin Extended-F" Unicode
       * character block.
       */
      LATIN_EXTENDED_F,

      /**
       * Constant for the "Old Uyghur" Unicode
       * character block.
       */
      OLD_UYGHUR,

      /**
       * Constant for the "Unified Canadian Aboriginal Syllabics Extended-A" Unicode
       * character block.
       */
      UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED_A,

      /**
       * Constant for the "Cypro-Minoan" Unicode
       * character block.
       */
      CYPRO_MINOAN,

      /**
       * Constant for the "Tangsa" Unicode
       * character block.
       */
      TANGSA,

      /**
       * Constant for the "Kana Extended-B" Unicode
       * character block.
       */
      KANA_EXTENDED_B,

      /**
       * Constant for the "Znamenny Musical Notation" Unicode
       * character block.
       */
      ZNAMENNY_MUSICAL_NOTATION,

      /**
       * Constant for the "Latin Extended-G" Unicode
       * character block.
       */
      LATIN_EXTENDED_G,

      /**
       * Constant for the "Toto" Unicode
       * character block.
       */
      TOTO,

      /**
       * Constant for the "Ethiopic Extended-B" Unicode
       * character block.
       */
      ETHIOPIC_EXTENDED_B,

      /**
       * Constant for the "Arabic Extended-C" Unicode
       * character block.
       */
      ARABIC_EXTENDED_C,

      /**
       * Constant for the "Devanagari Extended-A" Unicode
       * character block.
       */
      DEVANAGARI_EXTENDED_A,

      /**
       * Constant for the "Kawi" Unicode
       * character block.
       */
      KAWI,

      /**
       * Constant for the "Kaktovik Numerals" Unicode
       * character block.
       */
      KAKTOVIK_NUMERALS,

      /**
       * Constant for the "Cyrillic Extended-D" Unicode
       * character block.
       */
      CYRILLIC_EXTENDED_D,

      /**
       * Constant for the "Nag Mundari" Unicode
       * character block.
       */
      NAG_MUNDARI,

      /**
       * Constant for the "CJK Unified Ideographs Extension H" Unicode
       * character block.
       */
      CJK_UNIFIED_IDEOGRAPHS_EXTENSION_H,

      /**
       * Constant for the UNASSIGNED
       */
      UNASSIGNED
    };


    /**
     * A family of character subsets representing the character scripts
     * defined in the <a href="http://www.unicode.org/reports/tr24/">
     * <i>Unicode Standard Annex #24: Script Names</i></a>. Every Unicode
     * character is assigned to a single Unicode script, either a specific
     * script, such as @c Character::UnicodeScript::LATIN Latin, or
     * one of the following three special values,
     * @c Character::UnicodeScript::INHERITED Inherited,
     * @c Character::UnicodeScript::COMMON Common or
     * @c Character::UnicodeScript::UNKNOWN Unknown.
     *
     *
     */
    enum UnicodeScript : gshort {
      /**
       * Unicode script "Common".
       */
      COMMON,

      /**
       * Unicode script "Latin".
       */
      LATIN,

      /**
       * Unicode script "Greek".
       */
      GREEK,

      /**
       * Unicode script "Cyrillic".
       */
      CYRILLIC,

      /**
       * Unicode script "Armenian".
       */
      ARMENIAN,

      /**
       * Unicode script "Hebrew".
       */
      HEBREW,

      /**
       * Unicode script "Arabic".
       */
      ARABIC,

      /**
       * Unicode script "Syriac".
       */
      SYRIAC,

      /**
       * Unicode script "Thaana".
       */
      THAANA,

      /**
       * Unicode script "Devanagari".
       */
      DEVANAGARI,

      /**
       * Unicode script "Bengali".
       */
      BENGALI,

      /**
       * Unicode script "Gurmukhi".
       */
      GURMUKHI,

      /**
       * Unicode script "Gujarati".
       */
      GUJARATI,

      /**
       * Unicode script "Oriya".
       */
      ORIYA,

      /**
       * Unicode script "Tamil".
       */
      TAMIL,

      /**
       * Unicode script "Telugu".
       */
      TELUGU,

      /**
       * Unicode script "Kannada".
       */
      KANNADA,

      /**
       * Unicode script "Malayalam".
       */
      MALAYALAM,

      /**
       * Unicode script "Sinhala".
       */
      SINHALA,

      /**
       * Unicode script "Thai".
       */
      THAI,

      /**
       * Unicode script "Lao".
       */
      LAO,

      /**
       * Unicode script "Tibetan".
       */
      TIBETAN,

      /**
       * Unicode script "Myanmar".
       */
      MYANMAR,

      /**
       * Unicode script "Georgian".
       */
      GEORGIAN,

      /**
       * Unicode script "Hangul".
       */
      HANGUL,

      /**
       * Unicode script "Ethiopic".
       */
      ETHIOPIC,

      /**
       * Unicode script "Cherokee".
       */
      CHEROKEE,

      /**
       * Unicode script "Canadian_Aboriginal".
       */
      CANADIAN_ABORIGINAL,

      /**
       * Unicode script "Ogham".
       */
      OGHAM,

      /**
       * Unicode script "Runic".
       */
      RUNIC,

      /**
       * Unicode script "Khmer".
       */
      KHMER,

      /**
       * Unicode script "Mongolian".
       */
      MONGOLIAN,

      /**
       * Unicode script "Hiragana".
       */
      HIRAGANA,

      /**
       * Unicode script "Katakana".
       */
      KATAKANA,

      /**
       * Unicode script "Bopomofo".
       */
      BOPOMOFO,

      /**
       * Unicode script "Han".
       */
      HAN,

      /**
       * Unicode script "Yi".
       */
      YI,

      /**
       * Unicode script "Old_Italic".
       */
      OLD_ITALIC,

      /**
       * Unicode script "Gothic".
       */
      GOTHIC,

      /**
       * Unicode script "Deseret".
       */
      DESERET,

      /**
       * Unicode script "Inherited".
       */
      INHERITED,

      /**
       * Unicode script "Tagalog".
       */
      TAGALOG,

      /**
       * Unicode script "Hanunoo".
       */
      HANUNOO,

      /**
       * Unicode script "Buhid".
       */
      BUHID,

      /**
       * Unicode script "Tagbanwa".
       */
      TAGBANWA,

      /**
       * Unicode script "Limbu".
       */
      LIMBU,

      /**
       * Unicode script "Tai_Le".
       */
      TAI_LE,

      /**
       * Unicode script "Linear_B".
       */
      LINEAR_B,

      /**
       * Unicode script "Ugaritic".
       */
      UGARITIC,

      /**
       * Unicode script "Shavian".
       */
      SHAVIAN,

      /**
       * Unicode script "Osmanya".
       */
      OSMANYA,

      /**
       * Unicode script "Cypriot".
       */
      CYPRIOT,

      /**
       * Unicode script "Braille".
       */
      BRAILLE,

      /**
       * Unicode script "Buginese".
       */
      BUGINESE,

      /**
       * Unicode script "Coptic".
       */
      COPTIC,

      /**
       * Unicode script "New_Tai_Lue".
       */
      NEW_TAI_LUE,

      /**
       * Unicode script "Glagolitic".
       */
      GLAGOLITIC,

      /**
       * Unicode script "Tifinagh".
       */
      TIFINAGH,

      /**
       * Unicode script "Syloti_Nagri".
       */
      SYLOTI_NAGRI,

      /**
       * Unicode script "Old_Persian".
       */
      OLD_PERSIAN,

      /**
       * Unicode script "Kharoshthi".
       */
      KHAROSHTHI,

      /**
       * Unicode script "Balinese".
       */
      BALINESE,

      /**
       * Unicode script "Cuneiform".
       */
      CUNEIFORM,

      /**
       * Unicode script "Phoenician".
       */
      PHOENICIAN,

      /**
       * Unicode script "Phags_Pa".
       */
      PHAGS_PA,

      /**
       * Unicode script "Nko".
       */
      NKO,

      /**
       * Unicode script "Sundanese".
       */
      SUNDANESE,

      /**
       * Unicode script "Batak".
       */
      BATAK,

      /**
       * Unicode script "Lepcha".
       */
      LEPCHA,

      /**
       * Unicode script "Ol_Chiki".
       */
      OL_CHIKI,

      /**
       * Unicode script "Vai".
       */
      VAI,

      /**
       * Unicode script "Saurashtra".
       */
      SAURASHTRA,

      /**
       * Unicode script "Kayah_Li".
       */
      KAYAH_LI,

      /**
       * Unicode script "Rejang".
       */
      REJANG,

      /**
       * Unicode script "Lycian".
       */
      LYCIAN,

      /**
       * Unicode script "Carian".
       */
      CARIAN,

      /**
       * Unicode script "Lydian".
       */
      LYDIAN,

      /**
       * Unicode script "Cham".
       */
      CHAM,

      /**
       * Unicode script "Tai_Tham".
       */
      TAI_THAM,

      /**
       * Unicode script "Tai_Viet".
       */
      TAI_VIET,

      /**
       * Unicode script "Avestan".
       */
      AVESTAN,

      /**
       * Unicode script "Egyptian_Hieroglyphs".
       */
      EGYPTIAN_HIEROGLYPHS,

      /**
       * Unicode script "Samaritan".
       */
      SAMARITAN,

      /**
       * Unicode script "Mandaic".
       */
      MANDAIC,

      /**
       * Unicode script "Lisu".
       */
      LISU,

      /**
       * Unicode script "Bamum".
       */
      BAMUM,

      /**
       * Unicode script "Javanese".
       */
      JAVANESE,

      /**
       * Unicode script "Meetei_Mayek".
       */
      MEETEI_MAYEK,

      /**
       * Unicode script "Imperial_Aramaic".
       */
      IMPERIAL_ARAMAIC,

      /**
       * Unicode script "Old_South_Arabian".
       */
      OLD_SOUTH_ARABIAN,

      /**
       * Unicode script "Inscriptional_Parthian".
       */
      INSCRIPTIONAL_PARTHIAN,

      /**
       * Unicode script "Inscriptional_Pahlavi".
       */
      INSCRIPTIONAL_PAHLAVI,

      /**
       * Unicode script "Old_Turkic".
       */
      OLD_TURKIC,

      /**
       * Unicode script "Brahmi".
       */
      BRAHMI,

      /**
       * Unicode script "Kaithi".
       */
      KAITHI,

      /**
       * Unicode script "Meroitic Hieroglyphs".
       *
       */
      MEROITIC_HIEROGLYPHS,

      /**
       * Unicode script "Meroitic Cursive".
       *
       */
      MEROITIC_CURSIVE,

      /**
       * Unicode script "Sora Sompeng".
       *
       */
      SORA_SOMPENG,

      /**
       * Unicode script "Chakma".
       *
       */
      CHAKMA,

      /**
       * Unicode script "Sharada".
       *
       */
      SHARADA,

      /**
       * Unicode script "Takri".
       *
       */
      TAKRI,

      /**
       * Unicode script "Miao".
       *
       */
      MIAO,

      /**
       * Unicode script "Caucasian Albanian".
       *
       */
      CAUCASIAN_ALBANIAN,

      /**
       * Unicode script "Bassa Vah".
       *
       */
      BASSA_VAH,

      /**
       * Unicode script "Duployan".
       *
       */
      DUPLOYAN,

      /**
       * Unicode script "Elbasan".
       *
       */
      ELBASAN,

      /**
       * Unicode script "Grantha".
       *
       */
      GRANTHA,

      /**
       * Unicode script "Pahawh Hmong".
       *
       */
      PAHAWH_HMONG,

      /**
       * Unicode script "Khojki".
       *
       */
      KHOJKI,

      /**
       * Unicode script "Linear A".
       *
       */
      LINEAR_A,

      /**
       * Unicode script "Mahajani".
       *
       */
      MAHAJANI,

      /**
       * Unicode script "Manichaean".
       *
       */
      MANICHAEAN,

      /**
       * Unicode script "Mende Kikakui".
       *
       */
      MENDE_KIKAKUI,

      /**
       * Unicode script "Modi".
       *
       */
      MODI,

      /**
       * Unicode script "Mro".
       *
       */
      MRO,

      /**
       * Unicode script "Old North Arabian".
       *
       */
      OLD_NORTH_ARABIAN,

      /**
       * Unicode script "Nabataean".
       *
       */
      NABATAEAN,

      /**
       * Unicode script "Palmyrene".
       *
       */
      PALMYRENE,

      /**
       * Unicode script "Pau Cin Hau".
       *
       */
      PAU_CIN_HAU,

      /**
       * Unicode script "Old Permic".
       *
       */
      OLD_PERMIC,

      /**
       * Unicode script "Psalter Pahlavi".
       *
       */
      PSALTER_PAHLAVI,

      /**
       * Unicode script "Siddham".
       *
       */
      SIDDHAM,

      /**
       * Unicode script "Khudawadi".
       *
       */
      KHUDAWADI,

      /**
       * Unicode script "Tirhuta".
       *
       */
      TIRHUTA,

      /**
       * Unicode script "Warang Citi".
       *
       */
      WARANG_CITI,

      /**
       * Unicode script "Ahom".
       *
       */
      AHOM,

      /**
       * Unicode script "Anatolian Hieroglyphs".
       *
       */
      ANATOLIAN_HIEROGLYPHS,

      /**
       * Unicode script "Hatran".
       *
       */
      HATRAN,

      /**
       * Unicode script "Multani".
       *
       */
      MULTANI,

      /**
       * Unicode script "Old Hungarian".
       *
       */
      OLD_HUNGARIAN,

      /**
       * Unicode script "SignWriting".
       *
       */
      SIGNWRITING,

      /**
       * Unicode script "Adlam".
       *
       */
      ADLAM,

      /**
       * Unicode script "Bhaiksuki".
       *
       */
      BHAIKSUKI,

      /**
       * Unicode script "Marchen".
       *
       */
      MARCHEN,

      /**
       * Unicode script "Newa".
       *
       */
      NEWA,

      /**
       * Unicode script "Osage".
       *
       */
      OSAGE,

      /**
       * Unicode script "Tangut".
       *
       */
      TANGUT,

      /**
       * Unicode script "Masaram Gondi".
       *
       */
      MASARAM_GONDI,

      /**
       * Unicode script "Nushu".
       *
       */
      NUSHU,

      /**
       * Unicode script "Soyombo".
       *
       */
      SOYOMBO,

      /**
       * Unicode script "Zanabazar Square".
       *
       */
      ZANABAZAR_SQUARE,

      /**
       * Unicode script "Hanifi Rohingya".
       *
       */
      HANIFI_ROHINGYA,

      /**
       * Unicode script "Old Sogdian".
       *
       */
      OLD_SOGDIAN,

      /**
       * Unicode script "Sogdian".
       *
       */
      SOGDIAN,

      /**
       * Unicode script "Dogra".
       *
       */
      DOGRA,

      /**
       * Unicode script "Gunjala Gondi".
       *
       */
      GUNJALA_GONDI,

      /**
       * Unicode script "Makasar".
       *
       */
      MAKASAR,

      /**
       * Unicode script "Medefaidrin".
       *
       */
      MEDEFAIDRIN,

      /**
       * Unicode script "Elymaic".
       *
       */
      ELYMAIC,

      /**
       * Unicode script "Nandinagari".
       *
       */
      NANDINAGARI,

      /**
       * Unicode script "Nyiakeng Puachue Hmong".
       *
       */
      NYIAKENG_PUACHUE_HMONG,

      /**
       * Unicode script "Wancho".
       *
       */
      WANCHO,

      /**
       * Unicode script "Yezidi".
       *
       */
      YEZIDI,

      /**
       * Unicode script "Chorasmian".
       *
       */
      CHORASMIAN,

      /**
       * Unicode script "Dives Akuru".
       *
       */
      DIVES_AKURU,

      /**
       * Unicode script "Khitan Small Script".
       *
       */
      KHITAN_SMALL_SCRIPT,

      /**
       * Unicode script "Vithkuqi".
       *
       */
      VITHKUQI,

      /**
       * Unicode script "Old Uyghur".
       *
       */
      OLD_UYGHUR,

      /**
       * Unicode script "Cypro Minoan".
       *
       */
      CYPRO_MINOAN,

      /**
       * Unicode script "Tangsa".
       *
       */
      TANGSA,

      /**
       * Unicode script "Toto".
       *
       */
      TOTO,

      /**
       * Unicode script "Kawi".
       *
       */
      KAWI,

      /**
       * Unicode script "Nag Mundari".
       *
       */
      NAG_MUNDARI,

      /**
       * Unicode script "Unknown".
       */
      UNKNOWN
    };

  private:
    /**
     * The value of the @c Character.
     */
    gchar value = 0;

  public:
    /**
     * Construct new @c Character instance
     * representing the null (U+0000) character
     */
    Character() = default;

    /**
     * Constructs a newly allocated @c Character object that
     * represents the specified @c gchar value.
     *
     * @param  value   the value to be represented by the
     *                  @c Character object.
     *
     * @deprecated
     * It is rarely appropriate to use this constructor. The static factory
     * @b valueOf(gchar) is generally a better choice, as it is
     * likely to yield significantly better space and time performance.
     */
    CORE_IMPLICIT Character(gchar value);

    /**
     * Returns a @c Character instance representing the specified
     * @c gchar value.
     * If a new @c Character instance is not required, this method
     * should generally be used in preference to the constructor
     * @b Character(gchar), as this method is likely to yield
     * significantly better space and time performance by caching
     * frequently requested values.
     *
     * This method will always cache values in the range
     * @c '\u0000' to @c '\u007F', inclusive, and may
     * cache other values outside of this range.
     *
     * @param  c a gchar value.
     * @return a @c Character instance representing @c c.
     *
     */
    static Character valueOf(gchar c);

    /**
     * Returns the value of this @c Character object.
     * @return  the primitive @c gchar value represented by
     *          this object.
     */
    gchar charValue() const;

    /**
     * Returns a hash code for this @c Character; equal to the result
     * of invoking @c charValue().
     *
     * @return a hash code value for this @c Character
     */
    gint hash() const override;

    /**
     * Returns a hash code for a @c gchar value; compatible with
     * @c Character::hash().
     *
     *
     *
     * @param value The @c gchar for which to return a hash code.
     * @return a hash code value for a @c gchar value.
     */
    static gint hash(gchar value);

    /**
     * Compares this object against the specified object.
     * The result is @c true if and only if the argument is not
     * @c null and is a @c Character object that
     * represents the same @c gchar value as this object.
     *
     * @param   obj   the object to compare with.
     * @return  @c true if the objects are the same;
     *          @c false otherwise.
     */
    gbool equals(Object const& obj) const override;

    /**
     * Returns a @c String object representing this
     * @c Character's value.  The result is a string of
     * length 1 whose sole component is the primitive
     * @c gchar value represented by this
     * @c Character object.
     *
     * @return  a string representation of this object.
     */
    String toString() const override;

    /**
     * Returns a @c String object representing the
     * specified @c gchar.  The result is a string of length
     * 1 consisting solely of the specified @c gchar.
     *
     * @apiNote This method cannot handle <i> supplementary characters </i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::toString(gint) method.
     *
     * @param c the @c gchar to be converted
     * @return the string representation of the specified @c gchar
     *
     */
    static String toString(gchar c);

    /**
     * Returns a @c String object representing the
     * specified character (Unicode code point).  The result is a string of
     * length 1 or 2, consisting solely of the specified @c codePoint.
     *
     * @param codePoint the @c codePoint to be converted
     * @return the string representation of the specified @c codePoint
     * @throws IllegalArgumentException if the specified
     *      @c codePoint is not a valid Unicode code point.
     *
     */
    static String toString(gint codePoint);

    /**
     * Determines whether the specified code point is a valid
     * <a href="http://www.unicode.org/glossary/#code_point">
     * Unicode code point value</a>.
     *
     * @param  codePoint the Unicode code point to be tested
     * @return @c true if the specified code point value is between
     *         @b MIN_CODE_POINT and
     *         @b MAX_CODE_POINT inclusive;
     *         @c false otherwise.
     *
     */
    static gbool isValidCodePoint(gint codePoint);

    /**
     * Determines whether the specified character (Unicode code point)
     * is in the <i> Basic Multilingual Plane (BMP)</i>.
     * Such code points can be represented using a single @c gchar.
     *
     * @param  codePoint the character (Unicode code point) to be tested
     * @return @c true if the specified code point is between
     *         @b MIN_VALUE and @b MAX_VALUE inclusive;
     *         @c false otherwise.
     *
     */
    static gbool isBmpCodePoint(gint codePoint);

    /**
     * Determines whether the specified character (Unicode code point)
     * is in the <i>supplementary character</i> range.
     *
     * @param  codePoint the character (Unicode code point) to be tested
     * @return @c true if the specified code point is between
     *         @b MIN_SUPPLEMENTARY_CODE_POINT and
     *         @b MAX_CODE_POINT inclusive;
     *         @c false otherwise.
     *
     */
    static gbool isSupplementary(gint codePoint);

    /**
     * Determines if the given @c gchar value is a
     * <a href="http://www.unicode.org/glossary/#high_surrogate_code_unit">
     * Unicode high-surrogate code unit</a>
     * (also known as <i>leading-surrogate code unit</i>).
     *
     * <p>
     * Such values do not represent characters by themselves,
     * but are used in the representation of
     * <i>supplementary characters</i>
     * in the UTF-16 encoding.
     * </p>
     * @param  ch the @c gchar value to be tested.
     * @return @c true if the @c gchar value is between
     *         @b MIN_HIGH_SURROGATE and
     *         @b MAX_HIGH_SURROGATE inclusive;
     *         @c false otherwise.
                       *
     */
    static gbool isHighSurrogate(gchar ch);

    /**
     * Determines if the given @c gchar value is a
     * <a href="http://www.unicode.org/glossary/#low_surrogate_code_unit">
     * Unicode low-surrogate code unit</a>
     * (also known as <i>trailing-surrogate code unit</i>).
     *
     * <p>
     * Such values do not represent characters by themselves,
     * but are used in the representation of
     * <i>supplementary characters</i> in the UTF-16 encoding.
     * </p>
     * @param  ch the @c gchar value to be tested.
     * @return @c true if the @c gchar value is between
     *         @b MIN_LOW_SURROGATE and
     *         @b MAX_LOW_SURROGATE inclusive;
     *         @c false otherwise.
     *
     */
    static gbool isLowSurrogate(gchar ch);

    /**
     * Determines if the given @c gchar value is a Unicode
     * <i>surrogate code unit</i>.
     *
     * <p>
     * Such values do not represent characters by themselves,
     * but are used in the representation of
     * <i>supplementary characters</i> in the UTF-16 encoding.
     * </p>
     * <p>
     * A gchar value is a surrogate code unit if and only if it is either
     * a low-surrogate code unit or a high-surrogate code unit.
     * </p>
     * @param  ch the @c gchar value to be tested.
     * @return @c true if the @c gchar value is between
     *         @b MIN_SURROGATE and
     *         @b MAX_SURROGATE inclusive;
     *         @c false otherwise.
     *
     */
    static gbool isSurrogate(gchar ch);

    /**
     * Determines whether the specified pair of @c gchar
     * values is a valid
     * <a href="http://www.unicode.org/glossary/#surrogate_pair">
     * Unicode surrogate pair</a>.
     *
     * <p>
     * This method is equivalent to the expression:
     * @code isHighSurrogate(high) && isLowSurrogate(low) @endcode
     * </p>
     * @param  high the high-surrogate code value to be tested
     * @param  low the low-surrogate code value to be tested
     * @return @c true if the specified high and
     * low-surrogate code values represent a valid surrogate pair;
     * @c false otherwise.
     *
     */
    static gbool isSurrogatePair(gchar high, gchar low);

    /**
     * Determines the number of @c gchar values needed to
     * represent the specified character (Unicode code point). If the
     * specified character is equal to or greater than 0x10000, then
     * the method returns 2. Otherwise, the method returns 1.
     *
     * <p>
     * This method doesn't validate the specified character to be a
     * valid Unicode code point. The caller must validate the
     * character value using @c isValidCodePoint
     * if necessary.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  2 if the character is a valid supplementary character; 1 otherwise.
     *
     */
    static gint charCount(gint codePoint);

    /**
     * Converts the specified surrogate pair to its supplementary code
     * point value. This method does not validate the specified
     * surrogate pair. The caller must validate it using @c
     * Character::isSurrogatePair(gchar, gchar) isSurrogatePair if necessary.
     *
     * @param  high the high-surrogate code unit
     * @param  low the low-surrogate code unit
     * @return the supplementary code point composed from the
     *         specified surrogate pair.
     *
     */
    static gint toCodePoint(gchar high, gchar low);

    /**
     * Returns the code point at the given index of the
     * @c CharSequence. If the @c gchar value at
     * the given index in the @c CharSequence is in the
     * high-surrogate range, the following index is less than the
     * length of the @c CharSequence, and the
     * @c gchar value at the following index is in the
     * low-surrogate range, then the supplementary code point
     * corresponding to this surrogate pair is returned. Otherwise,
     * the @c gchar value at the given index is returned.
     *
     * @param seq a sequence of @c gchar values (Unicode code
     * units)
     * @param index the index to the @c gchar values (Unicode
     * code units) in @c seq to be converted
     * @return the Unicode code point at the given index
     *
     * @throws IndexOutOfBoundsException if the value
     * @c index is negative or not less than @b seq.length().
     *
     */
    static gint codePointAt(CharSequence const& seq, gint index);

    /**
     * Returns the code point at the given index of the
     * @c gchar array. If the @c gchar value at
     * the given index in the @c gchar array is in the
     * high-surrogate range, the following index is less than the
     * length of the @c gchar array, and the
     * @c gchar value at the following index is in the
     * low-surrogate range, then the supplementary code point
     * corresponding to this surrogate pair is returned. Otherwise,
     * the @c gchar value at the given index is returned.
     *
     * @param a the @c gchar array
     * @param index the index to the @c gchar values (Unicode
     * code units) in the @c gchar array to be converted
     * @return the Unicode code point at the given index
     *
     * @throws IndexOutOfBoundsException if the value
     * @c index is negative or not less than
     * the length of the @c gchar array.
     *
     */
    static gint codePointAt(CharArray const& a, gint index);

    /**
     * Returns the code point at the given index of the
     * @c gchar array, where only array elements with
     * @c index less than @c limit can be used. If
     * the @c gchar value at the given index in the
     * @c gchar array is in the high-surrogate range, the
     * following index is less than the @c limit, and the
     * @c gchar value at the following index is in the
     * low-surrogate range, then the supplementary code point
     * corresponding to this surrogate pair is returned. Otherwise,
     * the @c gchar value at the given index is returned.
     *
     * @param a the @c gchar array
     * @param index the index to the @c gchar values (Unicode
     * code units) in the @c gchar array to be converted
     * @param limit the index after the last array element that
     * can be used in the @c gchar array
     * @return the Unicode code point at the given index
     *
     * @throws IndexOutOfBoundsException if the @c index
     * argument is negative or not less than the @c limit
     * argument, or if the @c limit argument is negative or
     * greater than the length of the @c gchar array.
     *
     */
    static gint codePointAt(CharArray const& a, gint index, gint limit);

    /**
     * Returns the code point preceding the given index of the
     * @c CharSequence. If the @c gchar value at
     * @c (index - 1) in the @c CharSequence is in
     * the low-surrogate range, @c (index - 2) is not
     * negative, and the @c gchar value at @c (index - 2)
     * in the @c CharSequence is in the
     * high-surrogate range, then the supplementary code point
     * corresponding to this surrogate pair is returned. Otherwise,
     * the @c gchar value at @c (index - 1) is
     * returned.
     *
     * @param seq the @c CharSequence instance
     * @param index the index following the code point that should be returned
     * @return the Unicode code point value before the given index.
     *
     * @throws IndexOutOfBoundsException if the @c index
     * argument is less than 1 or greater than @b seq.length().
     *
     */
    static gint codePointBefore(CharSequence const& seq, gint index);

    /**
     * Returns the code point preceding the given index of the
     * @c gchar array. If the @c gchar value at
     * @c (index - 1) in the @c gchar array is in
     * the low-surrogate range, @c (index - 2) is not
     * negative, and the @c gchar value at @c (index - 2)
     * in the @c gchar array is in the
     * high-surrogate range, then the supplementary code point
     * corresponding to this surrogate pair is returned. Otherwise,
     * the @c gchar value at @c (index - 1) is
     * returned.
     *
     * @param a the @c gchar array
     * @param index the index following the code point that should be returned
     * @return the Unicode code point value before the given index.
     *
     * @throws IndexOutOfBoundsException if the @c index
     * argument is less than 1 or greater than the length of the
     * @c gchar array
     *
     */
    static gint codePointBefore(CharArray const& a, gint index);

    /**
     * Returns the code point preceding the given index of the
     * @c gchar array, where only array elements with
     * @c index greater than or equal to @c start
     * can be used. If the @c gchar value at @c (index - 1)
     * in the @c gchar array is in the
     * low-surrogate range, @c (index - 2) is not less than
     * @c start, and the @c gchar value at
     * @c (index - 2) in the @c gchar array is in
     * the high-surrogate range, then the supplementary code point
     * corresponding to this surrogate pair is returned. Otherwise,
     * the @c gchar value at @c (index - 1) is
     * returned.
     *
     * @param a the @c gchar array
     * @param index the index following the code point that should be returned
     * @param start the index of the first array element in the
     * @c gchar array
     * @return the Unicode code point value before the given index.
     *
     * @throws IndexOutOfBoundsException if the @c index
     * argument is not greater than the @c start argument or
     * is greater than the length of the @c gchar array, or
     * if the @c start argument is negative or not less than
     * the length of the @c gchar array.
     *
     */
    static gint codePointBefore(CharArray const& a, gint index, gint start);

    /**
     * Returns the leading surrogate (a
     * <a href="http://www.unicode.org/glossary/#high_surrogate_code_unit">
     * high surrogate code unit</a>) of the
     * <a href="http://www.unicode.org/glossary/#surrogate_pair">
     * surrogate pair</a>
     * representing the specified supplementary character (Unicode
     * code point) in the UTF-16 encoding.  If the specified character
     * is not a <i>supplementary character</i>, an unspecified @c gchar is returned.
     *
     * <p>
     * If @c isSupplementaryCodePoint(x) is @c true, then
     * @c isHighSurrogate@c (highSurrogate(x)) and
     * @c toCodePoint@c (highSurrogate(x), @c lowSurrogate@c (x)) == x
     * are also always @c true.
     * </p>
     * @param   codePoint a supplementary character (Unicode code point)
     * @return  the leading surrogate code unit used to represent the
     *          character in the UTF-16 encoding
     *
     */
    static gchar highSurrogate(gint codePoint);

    /**
     * Returns the trailing surrogate (a
     * <a href="http://www.unicode.org/glossary/#low_surrogate_code_unit">
     * low surrogate code unit</a>) of the
     * <a href="http://www.unicode.org/glossary/#surrogate_pair">
     * surrogate pair</a>
     * representing the specified supplementary character (Unicode
     * code point) in the UTF-16 encoding.  If the specified character
     * is not a
     * <i>supplementary character</i>,
     * an unspecified @c gchar is returned.
     *
     * <p>
     * If
     * @c isSupplementaryCodePoint(x) is @c true, then
     * @c isLowSurrogate@c (lowSurrogate(x)) and
     * @c Character::toCodePoint@c (@c highSurrogate(x),lowSurrogate(x)) == x
     * are also always @c true.
     * </p>
     * @param   codePoint a supplementary character (Unicode code point)
     * @return  the trailing surrogate code unit used to represent the
     *          character in the UTF-16 encoding
     *
     */
    static gchar lowSurrogate(gint codePoint);

    /**
     * Converts the specified character (Unicode code point) to its
     * UTF-16 representation. If the specified code point is a BMP
     * (Basic Multilingual Plane or Plane 0) value, the same value is
     * stored in @c dst[dstIndex], and 1 is returned. If the
     * specified code point is a supplementary character, its
     * surrogate values are stored in @c dst[dstIndex]
     * (high-surrogate) and @c dst[dstIndex+1]
     * (low-surrogate), and 2 is returned.
     *
     * @param  codePoint the character (Unicode code point) to be converted.
     * @param  dst an array of @c gchar in which the
     * @c codePoint's UTF-16 value is stored.
     * @param dstIndex the start index into the @c dst
     * array where the converted value is stored.
     * @return 1 if the code point is a BMP code point, 2 if the
     * code point is a supplementary code point.
     * @throws IllegalArgumentException if the specified
     * @c codePoint is not a valid Unicode code point.
     *
     * @throws IndexOutOfBoundsException if @c dstIndex
     * is negative or not less than @c dst.length, or if
     * @c dst at @c dstIndex doesn't have enough
     * array element(s) to store the resulting @c gchar
     * value(s). (If @c dstIndex is equal to
     * @c dst.length-1 and the specified
     * @c codePoint is a supplementary character, the
     * high-surrogate value is not stored in
     * @c dst[dstIndex].)
     *
     */
    static gint toChars(gint codePoint, CharArray& dst, gint dstIndex);

    /**
     * Converts the specified character (Unicode code point) to its
     * UTF-16 representation stored in a @c gchar array. If
     * the specified code point is a BMP (Basic Multilingual Plane or
     * Plane 0) value, the resulting @c gchar array has
     * the same value as @c codePoint. If the specified code
     * point is a supplementary code point, the resulting
     * @c gchar array has the corresponding surrogate pair.
     *
     * @param  codePoint a Unicode code point
     * @return a @c gchar array having
     *         @c codePoint's UTF-16 representation.
     * @throws IllegalArgumentException if the specified
     * @c codePoint is not a valid Unicode code point.
     *
     */
    static CharArray toChars(gint codePoint);

    /**
     * Returns the number of Unicode code points in the text range of
     * the specified gchar sequence. The text range begins at the
     * specified @c beginIndex and extends to the
     * @c gchar at index @c endIndex - 1. Thus the
     * length (in @c gchars) of the text range is
     * @c endIndex-beginIndex. Unpaired surrogates within
     * the text range count as one code point each.
     *
     * @param seq the gchar sequence
     * @param beginIndex the index to the first @c gchar of
     * the text range.
     * @param endIndex the index after the last @c gchar of
     * the text range.
     * @return the number of Unicode code points in the specified text
     * range
     *
     * @throws IndexOutOfBoundsException if the
     * @c beginIndex is negative, or @c endIndex
     * is larger than the length of the given sequence, or
     * @c beginIndex is larger than @c endIndex.
     *
     */
    static gint codePointCount(CharSequence const& seq, gint beginIndex, gint endIndex);

    /**
     * Returns the number of Unicode code points in a subarray of the
     * @c gchar array argument. The @c offset
     * argument is the index of the first @c gchar of the
     * subarray and the @c count argument specifies the
     * length of the subarray in @c gchars. Unpaired
     * surrogates within the subarray count as one code point each.
     *
     * @param a the @c gchar array
     * @param offset the index of the first @c gchar in the
     * given @c gchar array
     * @param count the length of the subarray in @c gchars
     * @return the number of Unicode code points in the specified subarray
     *
     * @throws IndexOutOfBoundsException if @c offset or
     * @c count is negative, or if @c offset +
     * count is larger than the length of the given array.
     *
     */
    static gint codePointCount(CharArray const& a, gint offset, gint count);

    /**
     * Returns the index within the given gchar sequence that is offset
     * from the given @c index by @c codePointOffset
     * code points. Unpaired surrogates within the text range given by
     * @c index and @c codePointOffset count as
     * one code point each.
     *
     * @param seq the gchar sequence
     * @param index the index to be offset
     * @param codePointOffset the offset in code points
     * @return the index within the gchar sequence
     *
     * @throws IndexOutOfBoundsException if @c index
     *   is negative or larger than the length of the gchar sequence,
     *   or if @c codePointOffset is positive and the
     *   subsequence starting with @c index has fewer than
     *   @c codePointOffset code points, or if
     *   @c codePointOffset is negative and the subsequence
     *   before @c index has fewer than the absolute value
     *   of @c codePointOffset code points.
     *
     */
    static gint offsetByCodePoints(CharSequence const& seq, gint index, gint codePointOffset);

    /**
     * Returns the index within the given @c gchar subarray
     * that is offset from the given @c index by
     * @c codePointOffset code points. The
     * @c start and @c count arguments specify a
     * subarray of the @c gchar array. Unpaired surrogates
     * within the text range given by @c index and
     * @c codePointOffset count as one code point each.
     *
     * @param a the @c gchar array
     * @param start the index of the first @c gchar of the
     * subarray
     * @param count the length of the subarray in @c gchars
     * @param index the index to be offset
     * @param codePointOffset the offset in code points
     * @return the index within the subarray
     *
     * @throws IndexOutOfBoundsException
     *   if @c start or @c count is negative,
     *   or if @c start + count is larger than the length of
     *   the given array,
     *   or if @c index is less than @c start or
     *   larger then @c start + count,
     *   or if @c codePointOffset is positive and the text range
     *   starting with @c index and ending with @c start + count - 1
     *   has fewer than @c codePointOffset code
     *   points,
     *   or if @c codePointOffset is negative and the text range
     *   starting with @c start and ending with @c index - 1
     *   has fewer than the absolute value of
     *   @c codePointOffset code points.
     *
     */
    static gint offsetByCodePoints(CharArray const& a, gint start, gint count,
                                   gint index, gint codePointOffset);

    /**
     * Determines if the specified character is a lowercase character.
     * <p>
     * A character is lowercase if its general category type, provided
     * by @c Character::category(ch), is
     * @c LOWERCASE_LETTER, or it has contributory property
     * Other_Lowercase as defined by the Unicode Standard.
     * </p>
     * <p>
     * The following are examples of lowercase characters:
     * </p>
     * <p>
     * @c 'a' @c 'b' @c 'c' @c 'd' @c 'e' @c 'f' @c 'g' @c 'h' @c 'i'
     * @c 'j' @c 'k' @c 'l' @c 'm' @c 'n' @c 'o' @c 'p' @c 'q' @c 'r'
     * @c 's' @c 't' @c 'u' @c 'v' @c 'w' @c 'x' @c 'y' @c 'z'
     * @c '\u00DF' @c '\u00E0' @c '\u00E1' @c '\u00E2' @c '\u00E3' @c '\u00E4' @c '\u00E5' @c '\u00E6'
     * @c '\u00E7' @c '\u00E8' @c '\u00E9' @c '\u00EA' @c '\u00EB' @c '\u00EC' @c '\u00ED' @c '\u00EE'
     * @c '\u00EF' @c '\u00F0' @c '\u00F1' @c '\u00F2' @c '\u00F3' @c '\u00F4' @c '\u00F5' @c '\u00F6'
     * @c '\u00F8' @c '\u00F9' @c '\u00FA' @c '\u00FB' @c '\u00FC' @c '\u00FD' @c '\u00FE' @c '\u00FF'
     * </p>
     * <p>
     * Many other Unicode characters are lowercase too.
     * </p>
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isLowerCase(gint) method.
     * </p>
     * @param   ch   the character to be tested.
     * @return  @c true if the character is lowercase;
     *          @c false otherwise.
     *
     */
    static gbool isLowerCase(gchar ch);

    /**
     * Determines if the specified character (Unicode code point) is a
     * lowercase character.
     * <p>
     * A character is lowercase if its general category type, provided
     * by @c Character::category category(codePoint), is
     * @c LOWERCASE_LETTER, or it has contributory property
     * Other_Lowercase as defined by the Unicode Standard.
     * </p>
     * <p>
     * The following are examples of lowercase characters:
     * <br/>
     * @c 'a' @c 'b' @c 'c' @c 'd' @c 'e' @c 'f' @c 'g' @c 'h' @c 'i'
     * @c 'j' @c 'k' @c 'l' @c 'm' @c 'n' @c 'o' @c 'p' @c 'q' @c 'r'
     * @c 's' @c 't' @c 'u' @c 'v' @c 'w' @c 'x' @c 'y' @c 'z'
     * @c '\u00DF' @c '\u00E0' @c '\u00E1' @c '\u00E2' @c '\u00E3' @c '\u00E4' @c '\u00E5' @c '\u00E6'
     * @c '\u00E7' @c '\u00E8' @c '\u00E9' @c '\u00EA' @c '\u00EB' @c '\u00EC' @c '\u00ED' @c '\u00EE'
     * @c '\u00EF' @c '\u00F0' @c '\u00F1' @c '\u00F2' @c '\u00F3' @c '\u00F4' @c '\u00F5' @c '\u00F6'
     * @c '\u00F8' @c '\u00F9' @c '\u00FA' @c '\u00FB' @c '\u00FC' @c '\u00FD' @c '\u00FE' @c '\u00FF'
     * <p>
     * Many other Unicode characters are lowercase too.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is lowercase;
     *          @c false otherwise.
     *
     */
    static gbool isLowerCase(gint codePoint);

    /**
     * Determines if the specified character is an uppercase character.
     * <p>
     * A character is uppercase if its general category type, provided by
     * @c Character::category(ch), is @c UPPERCASE_LETTER.
     * or it has contributory property Other_Uppercase as defined by the Unicode Standard.
     * </p>
     * <p>
     * The following are examples of uppercase characters:
     * <br/>
     * @c 'A' @c 'B' @c 'C' @c 'D' @c 'E' @c 'F' @c 'G' @c 'H' @c 'I'
     * @c 'J' @c 'K' @c 'L' @c 'M' @c 'N' @c 'O' @c 'P' @c 'Q' @c 'R'
     * @c 'S' @c 'T' @c 'U' @c 'V' @c 'W' @c 'X' @c 'Y' @c 'Z'
     * @c '\u00C0' @c '\u00C1' @c '\u00C2' @c '\u00C3' @c '\u00C4' @c '\u00C5' @c '\u00C6' @c '\u00C7'
     * @c '\u00C8' @c '\u00C9' @c '\u00CA' @c '\u00CB' @c '\u00CC' @c '\u00CD' @c '\u00CE' @c '\u00CF'
     * @c '\u00D0' @c '\u00D1' @c '\u00D2' @c '\u00D3' @c '\u00D4' @c '\u00D5' @c '\u00D6' @c '\u00D8'
     * @c '\u00D9' @c '\u00DA' @c '\u00DB' @c '\u00DC' @c '\u00DD' @c '\u00DE'
     * </p>
     * <p>
     * Many other Unicode characters are uppercase too.
     * </p>
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isUpperCase(gint) method.
     * </p>
     * @param   ch   the character to be tested.
     * @return  @c true if the character is uppercase;
     *          @c false otherwise.
     *
     */
    static gbool isUpperCase(gchar ch);

    /**
     * Determines if the specified character (Unicode code point) is an uppercase character.
     * <p>
     * A character is uppercase if its general category type, provided by
     * @c Character::category(codePoint), is @c UPPERCASE_LETTER,
     * or it has contributory property Other_Uppercase as defined by the Unicode Standard.
     * <p>
     * The following are examples of uppercase characters:
     * <br/>
     * A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
     * @c '\u00C0' @c '\u00C1' @c '\u00C2' @c '\u00C3' @c '\u00C4' @c '\u00C5' @c '\u00C6' @c '\u00C7'
     * @c '\u00C8' @c '\u00C9' @c '\u00CA' @c '\u00CB' @c '\u00CC' @c '\u00CD' @c '\u00CE' @c '\u00CF'
     * @c '\u00D0' @c '\u00D1' @c '\u00D2' @c '\u00D3' @c '\u00D4' @c '\u00D5' @c '\u00D6' @c '\u00D8'
     * @c '\u00D9' @c '\u00DA' @c '\u00DB' @c '\u00DC' @c '\u00DD' @c '\u00DE'
     * </p>
     * <p>
     * Many other Unicode characters are uppercase too.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is uppercase;
     *          @c false otherwise.
                                         *
     */
    static gbool isUpperCase(gint codePoint);

    /**
     * Determines if the specified character is a titlecase character.
     * <p>
     * A character is a titlecase character if its general
     * category type, provided by @c Character::category(ch),
     * is @c TITLECASE_LETTER.
     * </p>
     * <p>
     * Some characters look like pairs of Latin letters. For example, there
     * is an uppercase letter that looks like "LJ" and has a corresponding
     * lowercase letter that looks like "lj". A third form, which looks like "Lj",
     * is the appropriate form to use when rendering a word in lowercase
     * with initial capitals, as for a book title.
     * </p>
     * <p>
     * These are some of the Unicode characters for which this method returns
     * @c true: <br/>
     *
     * - @c LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON <br/>
     * - @c LATIN CAPITAL LETTER L WITH SMALL LETTER J <br/>
     * - @c LATIN CAPITAL LETTER N WITH SMALL LETTER J <br/>
     * - @c LATIN CAPITAL LETTER D WITH SMALL LETTER Z
     *
     * <p>
     * Many other Unicode characters are titlecase too.
     * </p>
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isTitleCase(gint) method.
     * </p>
     * @param   ch   the character to be tested.
     * @return  @c true if the character is titlecase;
     *          @c false otherwise.
     *
     */
    static gbool isTitleCase(gchar ch);

    /**
     * Determines if the specified character (Unicode code point) is a titlecase character.
     * <p>
     * A character is a titlecase character if its general
     * category type, provided by @c Character::category(gint) category(codePoint),
     * is @c TITLECASE_LETTER.
     * </p>
     * <p>
     * Some characters look like pairs of Latin letters. For example, there
     * is an uppercase letter that looks like "LJ" and has a corresponding
     * lowercase letter that looks like "lj". A third form, which looks like "Lj",
     * is the appropriate form to use when rendering a word in lowercase
     * with initial capitals, as for a book title.
     * </p>
     * <p>
     * These are some of the Unicode characters for which this method returns
     * @c true: <br/>
     *
     * - @c LATIN CAPITAL LETTER D WITH SMALL LETTER Z WITH CARON <br/>
     * - @c LATIN CAPITAL LETTER L WITH SMALL LETTER J <br/>
     * - @c LATIN CAPITAL LETTER N WITH SMALL LETTER J <br/>
     * - @c LATIN CAPITAL LETTER D WITH SMALL LETTER Z
     * </p>
     * <p>
     * Many other Unicode characters are titlecase too.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is titlecase;
     *          @c false otherwise.
                                         *
     */
    static gbool isTitleCase(gint codePoint);

    /**
     * Determines if the specified character is a digit.
     * <p>
     * A character is a digit if its general category type, provided
     * by @c Character::category(ch), is @c DECIMAL_DIGIT_NUMBER.
     * </p>
     * <p>
     * Some Unicode character ranges that contain digits: <br/>
     *
     * - @c '\u0030' through @c '\u0039',
     *     ISO-LATIN-1 digits (@c '0' through @c '9') <br/>
     * - @c '\u0660' through @c '\u0669',
     *     Arabic-Indic digits <br/>
     * - @c '\u06F0' through @c '\u06F9',
     *     Extended Arabic-Indic digits <br/>
     * - @c '\u0966' through @c '\u096F',
     *     Devanagari digits <br/>
     * - @c '\uFF10' through @c '\uFF19',
     *     Fullwidth digits <br/>
     *
     * Many other character ranges contain digits as well.
     * </p>
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isDigit(gint) method.
     * </p>
     * @param   ch   the character to be tested.
     * @return  @c true if the character is a digit;
     *          @c false otherwise.
     */
    static gbool isDigit(gchar ch);

    /**
     * Determines if the specified character (Unicode code point) is a digit.
     * <p>
     * A character is a digit if its general category type, provided
     * by @c Character::category(gint) category(codePoint), is
     * @c DECIMAL_DIGIT_NUMBER.
     * </p>
     * <p>
     * Some Unicode character ranges that contain digits:
     *
     * - @c '\u0030' through @c '\u0039', ISO-LATIN-1 digits (@c '0' through @c '9') <br/>
     * - @c '\u0660' through @c '\u0669', Arabic-Indic digits <br/>
     * - @c '\u06F0' through @c '\u06F9', Extended Arabic-Indic digits <br/>
     * - @c '\u0966' through @c '\u096F', Devanagari digits <br/>
     * - @c '\uFF10' through @c '\uFF19', Fullwidth digits
     *
     * Many other character ranges contain digits as well.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is a digit;
     *          @c false otherwise.
                       *
     */
    static gbool isDigit(gint codePoint);

    /**
     * Determines if a character is defined in Unicode.
     * <p>
     * A character is defined if at least one of the following is true:
     *
     * - It has an entry in the UnicodeData file. <br/>
     * - It has a value in a range defined by the UnicodeData file. <br/>
     * </p>
     *
     * <p>
     * @b Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isDefined(gint) method.
     * </p>
     * @param   ch   the character to be tested
     * @return  @c true if the character has a defined meaning
     *          in Unicode; @c false otherwise.
                                                           *
     */
    static gbool isDefined(gchar ch);

    /**
     * Determines if a character (Unicode code point) is defined in Unicode.
     * <p>
     * A character is defined if at least one of the following is true: <br/>
     *
     * - It has an entry in the UnicodeData file. <br/>
     * - It has a value in a range defined by the UnicodeData file. <br/>
     * </p>
     *
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character has a defined meaning
     *          in Unicode; @c false otherwise.
                                                           *
     */
    static gbool isDefined(gint codePoint);

    /**
     * Determines if the specified character is a letter.
     * <p>
     * A character is considered to be a letter if its general
     * category type, provided by @c Character::category(ch),
     * is any of the following: <br/>
     *
     * -  @c UPPERCASE_LETTER <br/>
     * -  @c LOWERCASE_LETTER <br/>
     * -  @c TITLECASE_LETTER <br/>
     * -  @c MODIFIER_LETTER <br/>
     * -  @c OTHER_LETTER
     *
     *
     * Not all letters have case. Many characters are
     * letters but are neither uppercase nor lowercase nor titlecase.
     * </p>
     * <p>
     * @b Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isLetter(gint) method.
     * </p>
     * @param   ch   the character to be tested.
     * @return  @c true if the character is a letter;
     *          @c false otherwise.
                                                                                      */
    static gbool isLetter(gchar ch);

    /**
     * Determines if the specified character (Unicode code point) is a letter.
     * <p>
     * A character is considered to be a letter if its general
     * category type, provided by @c Character::category(gint) category(codePoint),
     * is any of the following: <br/>
     *
     * -  @c UPPERCASE_LETTER <br/>
     * -  @c LOWERCASE_LETTER <br/>
     * -  @c TITLECASE_LETTER <br/>
     * -  @c MODIFIER_LETTER <br/>
     * -  @c OTHER_LETTER
     *
     *
     * Not all letters have case. Many characters are
     * letters but are neither uppercase nor lowercase nor titlecase.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is a letter;
     *          @c false otherwise.
                                                                    *
     */
    static gbool isLetter(gint codePoint);

    /**
     * Determines if the specified character is a letter or digit.
     * <p>
     * A character is considered to be a letter or digit if either
     * @c Character::isLetter(gchar ch) or
     * @c Character::isDigit(gchar ch) returns
     * @c true for the character.
     * </p>
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isLetterOrDigit(gint) method.
     * </p>
     * @param   ch   the character to be tested.
     * @return  @c true if the character is a letter or digit;
     *          @c false otherwise.
                                                           *
     */
    static gbool isLetterOrDigit(gchar ch);

    /**
     * Determines if the specified character (Unicode code point) is a letter or digit.
     * <p>
     * A character is considered to be a letter or digit if either
     * @c isLetter(codePoint) or
     * @c isDigit(codePoint) returns
     * @c true for the character.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is a letter or digit;
     *          @c false otherwise.
                                         *
     */
    static gbool isLetterOrDigit(gint codePoint);

    /**
     * Determines if the specified character (Unicode code point) is alphabetic.
     * <p>
     * A character is considered to be alphabetic if its general category type,
     * provided by @c Character::category(gint) category(codePoint), is any of
     * the following: <br/>
     *
     * -  @c UPPERCASE_LETTER <br/>
     * -  @c LOWERCASE_LETTER <br/>
     * -  @c TITLECASE_LETTER <br/>
     * -  @c MODIFIER_LETTER <br/>
     * -  @c OTHER_LETTER <br/>
     * -  @c LETTER_NUMBER
     *
     * or it has contributory property Other_Alphabetic as defined by the
     * Unicode Standard.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is a Unicode alphabet
     *          character, @c false otherwise.
     *
     */
    static gbool isAlphabetic(gint codePoint);

    /**
     * Determines if the specified character (Unicode code point) is a CJKV
     * (Chinese, Japanese, Korean and Vietnamese) ideograph, as defined by
     * the Unicode Standard.
     *
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is a Unicode ideograph
     *          character, @c false otherwise.
     *
     */
    static gbool isIdeographic(gint codePoint);

    /**
     * Determines if the specified character (Unicode code point) is an Emoji.
     * <p>
     * A character is considered to be an Emoji if and only if it has the @c Emoji
     * property, defined in
     * <a href="https://unicode.org/reports/tr51/#Emoji_Properties_and_Data_Files">
     * Unicode Emoji (Technical Standard #51)</a>.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is an Emoji;
     *          @c false otherwise.
     */
    static gbool isEmoji(gint codePoint);

    /**
     * Determines if the specified character (Unicode code point) has the
     * Emoji Presentation property by default.
     * <p>
     * A character is considered to have the Emoji Presentation property if and
     * only if it has the @c Emoji_Presentation property, defined in
     * <a href="https://unicode.org/reports/tr51/#Emoji_Properties_and_Data_Files">
     * Unicode Emoji (Technical Standard #51)</a>.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character has the Emoji Presentation
     *          property; @c false otherwise.
     */
    static gbool isEmojiPresentation(gint codePoint);

    /**
     * Determines if the specified character (Unicode code point) is an
     * Emoji Modifier.
     * <p>
     * A character is considered to be an Emoji Modifier if and only if it has
     * the @c Emoji_Modifier property, defined in
     * <a href="https://unicode.org/reports/tr51/#Emoji_Properties_and_Data_Files">
     * Unicode Emoji (Technical Standard #51)</a>.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is an Emoji Modifier;
     *          @c false otherwise.
     */
    static gbool isEmojiModifier(gint codePoint);

    /**
     * Determines if the specified character (Unicode code point) is an
     * Emoji Modifier Base.
     * <p>
     * A character is considered to be an Emoji Modifier Base if and only if it has
     * the @c Emoji_Modifier_Base property, defined in
     * <a href="https://unicode.org/reports/tr51/#Emoji_Properties_and_Data_Files">
     * Unicode Emoji (Technical Standard #51)</a>.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is an Emoji Modifier Base;
     *          @c false otherwise.
     */
    static gbool isEmojiModifierBase(gint codePoint);

    /**
     * Determines if the specified character (Unicode code point) is an
     * Emoji Component.
     * <p>
     * A character is considered to be an Emoji Component if and only if it has
     * the @c Emoji_Component property, defined in
     * <a href="https://unicode.org/reports/tr51/#Emoji_Properties_and_Data_Files">
     * Unicode Emoji (Technical Standard #51)</a>.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is an Emoji Component;
     *          @c false otherwise.
     */
    static gbool isEmojiComponent(gint codePoint);

    /**
     * Determines if the specified character (Unicode code point) is
     * an Extended Pictographic.
     * <p>
     * A character is considered to be an Extended Pictographic if and only if it has
     * the @c Extended_Pictographic property, defined in
     * <a href="https://unicode.org/reports/tr51/#Emoji_Properties_and_Data_Files">
     * Unicode Emoji (Technical Standard #51)</a>.
     * </p>
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is an Extended Pictographic;
     *          @c false otherwise.
     */
    static gbool isExtendedPictographic(gint codePoint);

    /**
     * Converts the character argument to lowercase using case
     * mapping information from the UnicodeData file.
     * <p>
     * Note that
     * @c Character::isLowerCase(Character::toLowerCase(ch))
     * does not always return @c true for some ranges of
     * characters, particularly those that are symbols or ideographs.
     * </p>
     * <p>
     * In general, @c String::toLowerCase() should be used to map
     * characters to lowercase. @c String case mapping methods
     * have several benefits over @c Character case mapping methods.
     * @c String case mapping methods can perform locale-sensitive
     * mappings, context-sensitive mappings, and 1:M character mappings, whereas
     * the @c Character case mapping methods cannot.
     * </p>
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::toLowerCase(gint) method.
     * </p>
     * @param   ch   the character to be converted.
     * @return  the lowercase equivalent of the character, if any;
     *          otherwise, the character itself.
                       */
    static gchar toLowerCase(gchar ch);

    /**
     * Converts the character (Unicode code point) argument to
     * lowercase using case mapping information from the UnicodeData
     * file.
     *
     * <p>
     * Note that
     * @c Character::isLowerCase(Character::toLowerCase(codePoint))
     * does not always return @c true for some ranges of
     * characters, particularly those that are symbols or ideographs.
     * </p>
     * <p>
     * In general, @c String::toLowerCase() should be used to map
     * characters to lowercase. @c String case mapping methods
     * have several benefits over @c Character case mapping methods.
     * @c String case mapping methods can perform locale-sensitive
     * mappings, context-sensitive mappings, and 1:M character mappings, whereas
     * the @c Character case mapping methods cannot.
     * </p>
     * @param   codePoint   the character (Unicode code point) to be converted.
     * @return  the lowercase equivalent of the character (Unicode code
     *          point), if any; otherwise, the character itself.
                       *
     *
     */
    static gint toLowerCase(gint codePoint);

    /**
     * Converts the character argument to uppercase using case mapping
     * information from the UnicodeData file.
     * <p>
     * Note that
     * @c Character::isUpperCase(Character::toUpperCase(ch))
     * does not always return @c true for some ranges of
     * characters, particularly those that are symbols or ideographs.
     * </p>
     * <p>
     * In general, @c String::toUpperCase() should be used to map
     * characters to uppercase. @c String case mapping methods
     * have several benefits over @c Character case mapping methods.
     * @c String case mapping methods can perform locale-sensitive
     * mappings, context-sensitive mappings, and 1:M character mappings, whereas
     * the @c Character case mapping methods cannot.
     * </p>
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::toUpperCase(gint) method.
     * </p>
     * @param   ch   the character to be converted.
     * @return  the uppercase equivalent of the character, if any;
     *          otherwise, the character itself.
     */
    static gchar toUpperCase(gchar ch);

    /**
     * Converts the character (Unicode code point) argument to
     * uppercase using case mapping information from the UnicodeData
     * file.
     *
     * <p>
     * Note that
     * @c Character::isUpperCase(Character::toUpperCase(codePoint))
     * does not always return @c true for some ranges of
     * characters, particularly those that are symbols or ideographs.
     * </p>
     * <p>
     * In general, @c String::toUpperCase() should be used to map
     * characters to uppercase. @c String case mapping methods
     * have several benefits over @c Character case mapping methods.
     * @c String case mapping methods can perform locale-sensitive
     * mappings, context-sensitive mappings, and 1:M character mappings, whereas
     * the @c Character case mapping methods cannot.
     * </p>
     * @param   codePoint   the character (Unicode code point) to be converted.
     * @return  the uppercase equivalent of the character, if any;
     *          otherwise, the character itself.
     */
    static gint toUpperCase(gint codePoint);

    /**
     * Converts the character argument to titlecase using case mapping
     * information from the UnicodeData file. If a character has no
     * explicit titlecase mapping and is not itself a titlecase gchar
     * according to UnicodeData, then the uppercase mapping is
     * returned as an equivalent titlecase mapping. If the
     * @c gchar argument is already a titlecase
     * @c gchar, the same @c gchar value will be
     * returned.
     * <p>
     * Note that
     * @c Character::isTitleCase(Character::toTitleCase(ch))
     * does not always return @c true for some ranges of
     * characters.
     * </p>
     * @note This method cannot handle <a href="#supplementary">
     * supplementary characters</a>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::toTitleCase(gint) method.
     *
     * @param   ch   the character to be converted.
     * @return  the titlecase equivalent of the character, if any;
     *          otherwise, the character itself.
     */
    static gchar toTitleCase(gchar ch);

    /**
     * Converts the character (Unicode code point) argument to titlecase using case mapping
     * information from the UnicodeData file. If a character has no
     * explicit titlecase mapping and is not itself a titlecase gchar
     * according to UnicodeData, then the uppercase mapping is
     * returned as an equivalent titlecase mapping. If the
     * character argument is already a titlecase
     * character, the same character value will be
     * returned.
     *
     * <p>
     * Note that @c Character::isTitleCase(Character::toTitleCase(codePoint))
     * does not always return @c true for some ranges of
     * characters.
     * </p>
     * @param   codePoint   the character (Unicode code point) to be converted.
     * @return  the titlecase equivalent of the character, if any;
     *          otherwise, the character itself.
     */
    static gint toTitleCase(gint codePoint);

    /**
     * Returns the numeric value of the character @c ch in the
     * specified radix.
     * <p>
     * If the radix is not in the range @c MIN_RADIX &le;
     * @c radix &le; @c MAX_RADIX or if the
     * value of @c ch is not a valid digit in the specified
     * radix, @c -1 is returned. A character is a valid digit
     * if at least one of the following is true:
     *
     * - The method @c isDigit is @c true of the character
     *     and the Unicode decimal digit value of the character (or its
     *     single-character decomposition) is less than the specified radix.
     *     In this case the decimal digit value is returned. <br/>
     * - The character is one of the uppercase Latin letters
     *     @c 'A' through @c 'Z' and its code is less than
     *     @c radix + 'A' - 10.
     *     In this case, @c ch - 'A' + 10
     *     is returned. <br/>
     * - The character is one of the lowercase Latin letters
     *     @c 'a' through @c 'z' and its code is less than
     *     @c radix + 'a' - 10.
     *     In this case, @c ch - 'a' + 10
     *     is returned. <br/>
     * - The character is one of the fullwidth uppercase Latin letters A
     *     (@c '\uFF21') through Z (@c '\uFF3A')
     *     and its code is less than
     *     @c radix + '\uFF21' - 10.
     *     In this case, @c ch - '\uFF21' + 10
     *     is returned. <br/>
     * - The character is one of the fullwidth lowercase Latin letters a
     *     (@c '\uFF41') through z (@c '\uFF5A')
     *     and its code is less than
     *     @c radix + '\uFF41' - 10.
     *     In this case, @c ch - '\uFF41' + 10
     *     is returned.
     * </p>
     *
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @b digit(gint, gint) method.
     * </p>
     * @param   ch      the character to be converted.
     * @param   radix   the radix.
     * @return  the numeric value represented by the character in the
     *          specified radix.
     */
    static gint digit(gchar ch, gint radix);

    /**
     * Returns the numeric value of the specified character (Unicode
     * code point) in the specified radix.
     *
     * <p>
     * If the radix is not in the range @c MIN_RADIX &le;
     * @c radix &le; @c MAX_RADIX or if the
     * character is not a valid digit in the specified
     * radix, @c -1 is returned. A character is a valid digit
     * if at least one of the following is true: <br/>
     *
     * - The method @c isDigit(codePoint) is @c true of the character
     *     and the Unicode decimal digit value of the character (or its
     *     single-character decomposition) is less than the specified radix.
     *     In this case the decimal digit value is returned. <br/>
     * - The character is one of the uppercase Latin letters
     *     @c 'A' through @c 'Z' and its code is less than
     *     @c radix + 'A' - 10. In this case, @c codePoint - 'A' + 10 is returned. <br/>
     * - The character is one of the lowercase Latin letters
     *     @c 'a' through @c 'z' and its code is less than
     *     @c radix + 'a' - 10. In this case, @c codePoint - 'a' + 10 is returned. <br/>
     * - The character is one of the fullwidth uppercase Latin letters A
     *     (@c '\uFF21') through Z (@c '\uFF3A')
     *     and its code is less than @c radix + '\uFF21' - 10.
     *     In this case, @c codePoint - '\uFF21' + 10 is returned. <br/>
     * - The character is one of the fullwidth lowercase Latin letters a
     *     (@c '\uFF41') through z (@c '\uFF5A') and its code is less than
     *     @c radix + '\uFF41'- 10. In this case, @c codePoint - '\uFF41' + 10 is returned. <br/>
     * </p>
     *
     * @param   codePoint the character (Unicode code point) to be converted.
     * @param   radix   the radix.
     * @return  the numeric value represented by the character in the
     *          specified radix.
     */
    static gint digit(gint codePoint, gint radix);

    /**
     * Returns the @c gint value that the specified Unicode
     * character represents. For example, the character
     * @c '\u216C' (the roman numeral fifty) will return
     * an gint with a value of 50.
     * <p>
     * The letters A-Z in their uppercase (@c '\u0041' through
     * @c '\u005A'), lowercase
     * (@c '\u0061' through @c '\u007A'), and
     * full width variant (@c '\uFF21' through
     * @c '\uFF3A' and @c '\uFF41' through
     * @c '\uFF5A') forms have numeric values from 10
     * through 35. This is independent of the Unicode specification,
     * which does not assign numeric values to these @c gchar
     * values.
     * </p>
     * <p>
     * If the character does not have a numeric value, then -1 is returned.
     * If the character has a numeric value that cannot be represented as a
     * non-negative integer (for example, a fractional value), then -2
     * is returned.
     * </p>
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @b numericValue(gint) method.
     * </p>
     * @param   ch      the character to be converted.
     * @return  the numeric value of the character, as a non-negative @c gint
     *          value; -2 if the character has a numeric value but the value
     *          can not be represented as a non-negative @c gint value;
     *          -1 if the character has no numeric value.
     */
    static gint numericValue(gchar ch);

    /**
     * Returns the @c gint value that the specified
     * character (Unicode code point) represents. For example, the character
     * @c '\u216C' (the Roman numeral fifty) will return
     * an @c gint with a value of 50.
     * <p>
     * The letters A-Z in their uppercase (@c '\u0041' through
     * @c '\u005A'), lowercase
     * (@c '\u0061' through @c '\u007A'), and
     * full width variant (@c '\uFF21' through
     * @c '\uFF3A' and @c '\uFF41' through
     * @c '\uFF5A') forms have numeric values from 10
     * through 35. This is independent of the Unicode specification,
     * which does not assign numeric values to these @c gchar values.
     * </p>
     * <p>
     * If the character does not have a numeric value, then -1 is returned.
     * If the character has a numeric value that cannot be represented as a
     * non-negative integer (for example, a fractional value), then -2
     * is returned.
     * </p>
     * @param   codePoint the character (Unicode code point) to be converted.
     * @return  the numeric value of the character, as a non-negative @c gint
     *          value; -2 if the character has a numeric value but the value
     *          can not be represented as a non-negative @c gint value;
     *          -1 if the character has no numeric value.
     */
    static gint numericValue(gint codePoint);

    /**
     * Determines if the specified character is ISO-LATIN-1 white space.
     * This method returns @c true for the following five
     * characters only: <br/>
     * -  Character ........Code ........Name <br/>
     * -  '\\t' ............U+0009 ......HORIZONTAL TABULATION <br/>
     * -  '\\n' ............U+000A ......NEW LINE <br/>
     * -  '\\f' ............U+000C ......FORM FEED <br/>
     * -  '\\r' ............U+000D ......CARRIAGE RETURN <br/>
     * -  ' ' ..............U+0020 ......SPACE
     *
     * @param      ch   the character to be tested.
     * @return     @c true if the character is ISO-LATIN-1 white space; @c false otherwise.
     * @deprecated Replaced by isWhitespace(gchar).
     */
    CORE_XDEPRECATED("Use Character::isWhitespace(gchar)")

    static gbool isSpace(gchar ch);


    /**
     * Determines if the specified character is a Unicode space character.
     * A character is considered to be a space character if and only if
     * it is specified to be a space character by the Unicode Standard. This
     * method returns true if the character's general category type is any of
     * the following: <br/>
     *
     * -  @c SPACE_SEPARATOR <br/>
     * -  @c LINE_SEPARATOR <br/>
     * -  @c PARAGRAPH_SEPARATOR
     *
     *
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isSpace(gint) method.
     * </p>
     * @param   ch      the character to be tested.
     * @return  @c true if the character is a space character;
     *          @c false otherwise.
     */
    static gbool isUnicodeSpace(gchar ch);

    /**
     * Determines if the specified character (Unicode code point) is a
     * Unicode space character.  A character is considered to be a
     * space character if and only if it is specified to be a space
     * character by the Unicode Standard. This method returns true if
     * the character's general category type is any of the following: <br/>
     *
     * -  @b SPACE_SEPARATOR <br/>
     * -  @b LINE_SEPARATOR <br/>
     * -  @b PARAGRAPH_SEPARATOR <br/>
     *
     *
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is a space character;
     *          @c false otherwise.
     */
    static gbool isUnicodeSpace(gint codePoint);

    /**
     * Determines if the specified character is white space according to Java.
     * A character is a Java whitespace character if and only if it satisfies
     * one of the following criteria: <br/>
     *
     * -  It is a Unicode space character (@c SPACE_SEPARATOR,
     *      @c LINE_SEPARATOR, or @c PARAGRAPH_SEPARATOR)
     *      but is not also a non-breaking space (@c '\u00A0',
     *      @c '\u2007', @c '\u202F'). <br/>
     * -  It is @c '\\t', U+0009 HORIZONTAL TABULATION. <br/>
     * -  It is @c '\\n', U+000A LINE FEED. <br/>
     * -  It is @c '\u000B', U+000B VERTICAL TABULATION. <br/>
     * -  It is @c '\\f', U+000C FORM FEED. <br/>
     * -  It is @c '\\r', U+000D CARRIAGE RETURN. <br/>
     * -  It is @c '\u001C', U+001C FILE SEPARATOR. <br/>
     * -  It is @c '\u001D', U+001D GROUP SEPARATOR. <br/>
     * -  It is @c '\u001E', U+001E RECORD SEPARATOR. <br/>
     * -  It is @c '\u001F', U+001F UNIT SEPARATOR.
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isWhitespace(gint) method.
     * </p>
     * @param   ch the character to be tested.
     * @return  @c true if the character is a Java whitespace
     *          character; @c false otherwise.
              *
     */
    static gbool isWhitespace(gchar ch);

    /**
     * Determines if the specified character (Unicode code point) is
     * white space according to Java.  A character is a Java
     * whitespace character if and only if it satisfies one of the
     * following criteria: <br/>
     *
     * -  It is a Unicode space character (@b SPACE_SEPARATOR,
     *      @b LINE_SEPARATOR, or @b PARAGRAPH_SEPARATOR)
     *      but is not also a non-breaking space (@c '\u00A0',
     *      @c '\u2007', @c '\u202F'). <br/>
     * -  It is @c '\\t', U+0009 HORIZONTAL TABULATION. <br/>
     * -  It is @c '\\n', U+000A LINE FEED. <br/>
     * -  It is @c '\u000B', U+000B VERTICAL TABULATION. <br/>
     * -  It is @c '\\f', U+000C FORM FEED. <br/>
     * -  It is @c '\\r', U+000D CARRIAGE RETURN. <br/>
     * -  It is @c '\u001C', U+001C FILE SEPARATOR. <br/>
     * -  It is @c '\u001D', U+001D GROUP SEPARATOR. <br/>
     * -  It is @c '\u001E', U+001E RECORD SEPARATOR. <br/>
     * -  It is @c '\u001F', U+001F UNIT SEPARATOR.
     *
     *
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is a Java whitespace
     *          character; @c false otherwise.
              *
     */
    static gbool isWhitespace(gint codePoint);

    /**
     * Determines if the specified character is an ISO control
     * character.  A character is considered to be an ISO control
     * character if its code is in the range @c '\u0000'
     * through @c '\u001F' or in the range
     * @c '\u007F' through @c '\u009F'.
     *
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isISOControl(gint) method.
     * </p>
     * @param   ch      the character to be tested.
     * @return  @c true if the character is an ISO control character;
     *          @c false otherwise.
     */
    static gbool isISOControl(gchar ch);

    /**
     * Determines if the referenced character (Unicode code point) is an ISO control
     * character.  A character is considered to be an ISO control
     * character if its code is in the range @c '\u0000'
     * through @c '\u001F' or in the range
     * @c '\u007F' through @c '\u009F'.
     *
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is an ISO control character;
     *          @c false otherwise.
                       *
     */
    static gbool isISOControl(gint codePoint);

    /**
     * Returns a value indicating a character's general category.
     *
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @b category(gint) method.
     * </p>
     * @param   ch      the character to be tested.
     * @return  a value of type @c gint representing the
     *          character's general category.
     */
    static Category category(gchar ch);

    /**
     * Returns a value indicating a character's general category.
     *
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  a value of type @c gint representing the
     *          character's general category.
     */
    static Category category(gint codePoint);

    /**
     * Determines the character representation for a specific digit in
     * the specified radix. If the value of @c radix is not a
     * valid radix, or the value of @c digit is not a valid
     * digit in the specified radix, the null character
     * (@c '\u0000') is returned.
     * <p>
     * The @c radix argument is valid if it is greater than or
     * equal to @c MIN_RADIX and less than or equal to
     * @c MAX_RADIX. The @c digit argument is valid if
     * @c 0 <= digit < radix.
     * </p>
     * <p>
     * If the digit is less than 10, then
     * @c '0' + digit is returned. Otherwise, the value
     * @c 'a' + digit - 10 is returned.
     * </p>
     *
     * @param   digit   the number to convert to a character.
     * @param   radix   the radix.
     * @return  the @c gchar representation of the specified digit
     *          in the specified radix.
                                */
    static gchar forDigit(gint digit, gint radix);

    /**
     * Returns the Unicode directionality property for the given
     * character.  Character directionality is used to calculate the
     * visual ordering of text. The directionality value of undefined
     * @c gchar values is @c DIRECTIONALITY_UNDEFINED.
     *
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @b directionality(gint) method.
     * </p>
     * @param  ch @c gchar for which the directionality property
     *            is requested.
     * @return the directionality property of the @c gchar value.
     *
     */
    static Directionality directionality(gchar ch);

    /**
     * Returns the Unicode directionality property for the given
     * character (Unicode code point).  Character directionality is
     * used to calculate the visual ordering of text. The
     * directionality value of undefined character is
     * @b DIRECTIONALITY_UNDEFINED.
     *
     * @param   codePoint the character (Unicode code point) for which
     *          the directionality property is requested.
     * @return the directionality property of the character.
     *
     */
    static Directionality directionality(gint codePoint);

    /**
     * Determines whether the character is mirrored according to the
     * Unicode specification.  Mirrored characters should have their
     * glyphs horizontally mirrored when displayed in text that is
     * right-to-left.  For example, @c '\u0028' LEFT
     * PARENTHESIS is semantically defined to be an <i>opening
     * parenthesis</i>.  This will appear as a "(" in text that is
     * left-to-right but as a ")" in text that is right-to-left.
     *
     * <p>
     * @c Note: This method cannot handle <i> supplementary characters</i>. To support
     * all Unicode characters, including supplementary characters, use
     * the @c Character::isMirrored(gint) method.
     * </p>
     * @param  ch @c gchar for which the mirrored property is requested
     * @return @c true if the gchar is mirrored, @c false
     *         if the @c gchar is not mirrored or is not defined.
     *
     */
    static gbool isMirrored(gchar ch);

    /**
     * Determines whether the specified character (Unicode code point)
     * is mirrored according to the Unicode specification.  Mirrored
     * characters should have their glyphs horizontally mirrored when
     * displayed in text that is right-to-left.  For example,
     * @c '\u0028' LEFT PARENTHESIS is semantically
     * defined to be an <i>opening parenthesis</i>.  This will appear
     * as a "(" in text that is left-to-right but as a ")" in text
     * that is right-to-left.
     *
     * @param   codePoint the character (Unicode code point) to be tested.
     * @return  @c true if the character is mirrored, @c false
     *          if the character is not mirrored or is not defined.
     *
     */
    static gbool isMirrored(gint codePoint);

    /**
     * Compares two @c Character objects numerically.
     *
     * @param   anotherCharacter   the @c Character to be compared.
     * @return  the value @c 0 if the argument @c Character
     *          is equal to this @c Character; a value less than
     *          @c 0 if this @c Character is numerically less
     *          than the @c Character argument; and a value greater than
     *          @c 0 if this @c Character is numerically greater
     *          than the @c Character argument (unsigned comparison).
     *          Note that this is strictly a numerical comparison; it is not
     *          locale-dependent.
     */
    gint compareTo(Character const& anotherCharacter) const override;

    /**
     * Compares two @c gchar values numerically.
     * The value returned is identical to what would be returned by:
     * <pre>
     *    Character::valueOf(x).compareTo(Character::valueOf(y))
     * </pre>
     *
     * @param  x the first @c gchar to compare
     * @param  y the second @c gchar to compare
     * @return the value @c 0 if @c x == y;
     *         a value less than @c 0 if @c x < y; and
     *         a value greater than @c 0 if @c x > y
     */
    static gint compare(gchar x, gchar y);

    /**
     * The number of bits used to represent a @c gchar value in unsigned
     * binary form, constant @c 16.
     */
    static CORE_FAST gint SIZE = 16;

    /**
     * The number of bytes used to represent a @c gchar value in unsigned
     * binary form.
     */
    static CORE_FAST gint BYTES = SIZE / Byte::SIZE;

    /**
     * Returns the value obtained by reversing the order of the bytes in the
     * specified @c gchar value.
     *
     * @param ch The @c gchar of which to reverse the byte order.
     * @return the value obtained by reversing (or, equivalently, swapping)
     *     the bytes in the specified @c gchar value.
     *
     */
    static gchar reverseBytes(gchar ch);

    /**
     * Returns the name of the specified character
     * @c codePoint, or null if the code point is
     * @b unassigned.
     * <p>
     * If the specified character is not assigned a name by
     * the <i>UnicodeData</i> file (part of the Unicode Character
     * Database maintained by the Unicode Consortium), the returned
     * name is the same as the result of the expression:
     *
     * @code
     *     Character::plane(codePoint).toString().replace('_', ' ')
     *     + " "
     *     + Integer::toHexString(codePoint).toUpperCase();
     * @endcode.
     *
     * For the @c codePoints in the <i>UnicodeData</i> file, the name
     * returned by this method follows the naming scheme in the
     * "Unicode Name Property" section of the Unicode Standard. For other
     * code points, such as Hangul/Ideographs, The name generation rule above
     * differs from the one defined in the Unicode Standard.
     * </p>
     * @param  codePoint the character (Unicode code point)
     *
     * @return the name of the specified character, or null if
     *         the code point is unassigned.
     *
     * @throws IllegalArgumentException if the specified
     *            @c codePoint is not a valid Unicode
     *            code point.
     *
     *
     */
    static String name(gint codePoint);

    /**
     * Returns the code point value of the Unicode character specified by
     * the given character name.
     * <p>
     * If a character is not assigned a name by the <i>UnicodeData</i>
     * file (part of the Unicode Character Database maintained by the Unicode
     * Consortium), its name is defined as the result of the expression:
     *
     * @code
     *     Character::UnicodeBlock.of(codePoint).toString().replace('_', ' ')
     *     + " "
     *     + Integer::toHexString(codePoint).toUpperCase(Locale.ROOT);
     * @endcode
     * </p>
     * <p>
     * The @c name matching is case-insensitive, with any leading and
     * trailing whitespace character removed.
     *
     * For the code points in the <i>UnicodeData</i> file, this method
     * recognizes the name which conforms to the name defined in the
     * "Unicode Name Property" section in the Unicode Standard. For other
     * code points, this method recognizes the name generated with
     * @b getName(gint) method.
     * </p>
     * @param  name the character name
     *
     * @return the code point value of the character specified by its name.
     *
     * @throws IllegalArgumentException if the specified @c name
     *         is not a valid character name.
     */
    static gint codePointOf(String const& name);

    /**
     * Return shaodow copy of this object.
     *
     * @return the shadow copy of this object.
     */
    Object& clone() const override;
  };
} // core

#endif //CORE24_CHARACTER_H
