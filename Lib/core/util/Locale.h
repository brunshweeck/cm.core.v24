//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_LOCALE_H
#define CORE24_LOCALE_H

#include <core/util/MissingResourceException.h>

namespace core {
    namespace util {
        /**
         * A @c Locale object represents a specific geographical, political,
         * or cultural region. An operation that requires a @c Locale to perform
         * its task is called <em>locale-sensitive</em> and uses the @c Locale
         * to tailor information for the user. For example, displaying a number
         * is a locale-sensitive operation-; the number should be formatted
         * according to the customs and conventions of the user's native country,
         * region, or culture.
         *
         * <p>
         * The @c Locale class implements IETF BCP 47 which is composed of
         * <a href="https://tools.ietf.org/html/rfc4647"> RFC 4647 "Matching of Language Tags" </a> and
         * <a href="https://tools.ietf.org/html/rfc5646"> RFC 5646 "Tags for Identifying Languages" </a>
         * with support for the LDML (UTS#35, "Unicode Locale Data Markup Language") BCP 47-compatible
         * extensions for locale data exchange.
         * </p>
         * <p>
         * A @c Locale object logically consists of the fields
         * described below.
         * </p>
         *
         * @li <b>Language</b>
         *   <p>
         *   ISO 639 alpha-2 or alpha-3 language code, or registered
         *   language subtags up to 8 alpha letters (for future enhancements).
         *   When a language has both an alpha-2 code and an alpha-3 code, the
         *   alpha-2 code must be used.  You can find a full list of valid
         *   language codes in the IANA Language Subtag Registry (search for
         *   "Type: language").  The language field is case-insensitive, but
         *   @c Locale always canonicalizes to lower case.
         *   </p>
         *   <p>
         *   Well-formed language values have the form
         *   <code>[a-zA-Z]{2,8}</code>.  Note that this is not the full
         *   BCP47 language production, since it excludes extlang.  They are
         *   not needed since modern three-letter language codes replace
         *   them.
         *   </p>
         *   <p>
         *   Example: "en" (English), "ja" (Japanese), "kok" (Konkani)
         *   </p>
         *
         *  @li <b>Script</b>
         *   <p>
         *   ISO 15924 alpha-4 script code.  You can find a full list of
         *   valid script codes in the IANA Language Subtag Registry (search
         *   for "Type: script").  The script field is case-insensitive, but
         *   @c Locale always canonicalizes to title case (the first
         *   letter is upper case and the rest of the letters are lower
         *   case).
         *   </p>
         *   <p>
         *   Well-formed script values have the form <code>[a-zA-Z]{4}</code>
         *   </p>
         *   <p>
         *   Example: "Latn" (Latin), "Cyrl" (Cyrillic)
         *   </p>
         *
         *   @li <b>Country (Region)</b>
         *   <p>
         *   ISO 3166 alpha-2 country code or UN M.49 numeric-3 area code.
         *   You can find a full list of valid country and region codes in the
         *   IANA Language Subtag Registry (search for "Type: region").  The
         *   country (region) field is case-insensitive, but
         *   @c Locale always canonicalizes to upper case.
         *   </p>
         *   <p>
         *   Well-formed country/region values have
         *   the form <code>[a-zA-Z]{2} | [0-9]{3}</code>
         *   </p>
         *   <p>
         *   Example: "US" (United States), "FR" (France), "029" (Caribbean)
         *   </p>
         *
         *  @li <b>Variant</b>
         *   <p>
         *   Any arbitrary value used to indicate a variation of a
         *   @c Locale.  Where there are two or more variant values
         *   each indicating its own semantics, these values should be ordered
         *   by importance, with most important first, separated by
         *   underscore('_').  The variant field is case-sensitive.
         *   </p>
         *   <p>
         *   Note: IETF BCP 47 places syntactic restrictions on variant
         *   subtags.  Also, BCP 47 subtags are strictly used to indicate
         *   additional variations that define a language or its dialects that
         *   are not covered by any combinations of language, script and
         *   region subtags.  You can find a full list of valid variant codes
         *   in the IANA Language Subtag Registry (search for "Type: variant").
         *   </p>
         *   <p>
         *   However, the variant field in @c Locale has
         *   historically been used for any kind of variation, not just
         *   language variations.  For example, some supported variants
         *   available in Java SE Runtime Environments indicate alternative
         *   cultural behaviors such as calendar type or number script.  In
         *   BCP 47 this kind of information, which does not identify the
         *   language, is supported by extension subtags or private use
         *   subtags.
         *   </p>
         *   <p>
         *   Well-formed variant values have the form <code>SUBTAG
         *   (('_'|'-') SUBTAG)*</code> where <code>SUBTAG =
         *   [0-9][0-9a-zA-Z]{3} | [0-9a-zA-Z]{5,8}</code>. (Note: BCP 47 only
         *   uses hyphen ('-') as a delimiter, this is more lenient).
         *   </p>
         *   <p>
         *   Example: "polyton" (Polytonic Greek), "POSIX"
         *   </p>
         *
         * <p>
         * <b>Note:</b> Although BCP 47 requires field values to be registered
         * in the IANA Language Subtag Registry, the @c Locale class
         * does not provide any validation features.  The @em builder
         * only checks if an individual field satisfies the syntactic
         * requirement (is well-formed), but does not validate the value
         * itself.
         * </p>
         *
         * <h3>Obtaining a Locale</h3>
         *
         * <p>
         * There are several ways to obtain a @c Locale
         * object.
         * </p>
         *
         * <h3>Factory Methods</h3>
         *
         * <p>
         * The method @b forLanguageTag obtains a @c Locale
         * object for a well-formed BCP 47 language tag. The method
         * </p>
         *
         * <h3>Locale Constants</h3>
         *
         * <p>
         * The @c Locale class provides a number of convenient constants
         * that you can use to obtain @c Locale objects for commonly used
         * locales. For example, @c Locale::US is the @c Locale object
         * for the United States.
         * </p>
         *
         * <h3>Default Locale</h3>
         *
         * <p>
         * The default Locale is provided for any locale-sensitive methods if no
         * @c Locale is explicitly specified as an argument, such as
         * @b DateFormat::getInstance(). The default Locale is determined at startup
         * of the Process and established in the following three phases:
         * @li The locale-related system properties listed below are established from the
         * host environment. Some system properties (except for @c 'user.language') may
         * not have values from the host environment.
         * </p>
         * <p>
         * There are finer-grained default Locales specific for each @b Locale::Category.
         * These category specific default Locales can be queried by @b getDefault(Category),
         * and set by <b> setDefault(Category, Locale)</b>. Construction of these category
         * specific default Locales are determined by the corresponding system properties,
         * which consist of the base system properties as listed above, suffixed by either
         * @c ".display" or @c ".format" depending on the category. For example,
         * the value of the @c 'user.language'.display system property will be used in the
         * @c language part of the default Locale for the @b Locale::Category::DISPLAY
         * category. In the absence of category specific system properties, the "category-less"
         * system properties are used, such as @c 'user.language' in the previous example.
         * </p>
         * <h3>Locale Matching</h3>
         *
         * <p>
         * If an application or a system is internationalized and provides localized
         * resources for multiple locales, it sometimes needs to find one or more
         * locales (or language tags) which meet each user's specific preferences. Note
         * that a term "language tag" is used interchangeably with "locale" in this
         * locale matching documentation.
         * </p>
         * <p>
         * In order to do matching a user's preferred locales to a set of language
         * tags, <a href="https://tools.ietf.org/html/rfc4647">RFC 4647 Matching of
         * Language Tags</a> defines two mechanisms: filtering and lookup.
         * <em>Filtering</em> is used to get all matching locales, whereas
         * <em>lookup</em> is to choose the best matching locale.
         * Matching is done case-insensitively. These matching mechanisms are described
         * in the following sections.
         * </p>
         *
         * <h3>Filtering</h3>
         *
         * <p>
         * The filtering operation returns all matching language tags. It is defined
         * in RFC 4647 as follows:
         * "In filtering, each language range represents the least specific language
         * tag (that is, the language tag with the fewest number of subtags) that is an
         * acceptable match. All the language tags in the matching set of tags will
         * have an equal or greater number of subtags than the language range. Every
         * non-wildcard subtag in the language range will appear in every one of the
         * matching language tags."
         * </p>
         * <p>
         * There are two types of filtering: filtering for basic language ranges
         * (called "basic filtering") and filtering for extended language ranges
         * (called "extended filtering"). They may return different results by what
         * kind of language ranges are included in the given Language Priority List.
         * @b Locale::FilteringMode is a parameter to specify how filtering should
         * be done.
         * </p>
         * <h3>Lookup</h3>
         *
         * <p>
         * The lookup operation returns the best matching language tags. It is
         * defined in RFC 4647 as follows:
         * "By contrast with filtering, each language range represents the most
         * specific tag that is an acceptable match.  The first matching tag found,
         * according to the user's priority, is considered the closest match and is the
         * item returned."
         * </p>
         * <p>
         * For example, if a Language Priority List consists of two language ranges,
         * @c "zh-Hant-TW" and @c "en-US", in prioritized order, lookup
         * method progressively searches the language tags below in order to find the
         * best matching language tag.
         *
         * @li zh-Hant-TW
         * @li zh-Hant
         * @li zh
         * @li en-US
         * @li en
         *
         * If there is a language tag which matches completely to a language range
         * above, the language tag is returned.
         * </p>
         *
         * <h3>Use of Locale</h3>
         *
         * <p>
         * Once you've obtained a @c Locale you can query it for information
         * about itself. Use @c country to get the country (or region)
         * code and @c language to get the language code.
         * You can use @c displayCountry to get the
         * name of the country suitable for displaying to the user. Similarly,
         * you can use @c displayLanguage to get the name of
         * the language suitable for displaying to the user. Interestingly,
         * the @c displayXXX methods are themselves locale-sensitive
         * and have two versions: one that uses the default
         * @b Locale::Category::DISPLAY locale and one
         * that uses the locale specified as an argument.
         * </p>
         * <p>
         * The Platform provides a number of classes that perform locale-sensitive
         * operations. For example, the @c NumberFormat class formats
         * numbers, currency, and percentages in a locale-sensitive manner. Classes
         * such as @c NumberFormat have several convenience methods
         * for creating a default object of that type. For example, the
         * @c NumberFormat class provides these three convenience methods
         * for creating a default @c NumberFormat object:
         * @code
         *     NumberFormat::newInstance();
         *     NumberFormat::newCurrencyInstance();
         *     NumberFormat::newPercentInstance();
         * @endcode
         * Each of these methods has two variants; one with an explicit locale
         * and one without; the latter uses the default
         * @b Locale::Category::FORMAT locale:
         * @code
         *     NumberFormat::newInstance(myLocale);
         *     NumberFormat::newCurrencyInstance(myLocale);
         *     NumberFormat::newPercentInstance(myLocale);
         * @endcode
         * A @c Locale is the mechanism for identifying the kind of object
         * (@c NumberFormat) that you would like to get. The locale is
         * <b>just</b> a mechanism for identifying objects,
         * <b>not</b> a container for the objects themselves.
         * </p>
         *
         * @see https://www.rfc-editor.org/info/rfc4647 RFC 4647: Matching of Language Tags
         * @see https://www.rfc-editor.org/info/rfc5646 RFC 5646: Tags for Identifying Languages
         *
         * @see text::Format
         * @see text::NumberFormat
         * @see text::Collator
         */
        class Locale final : public Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_ADD_AS_FRIEND(time::ZoneOffset);
            CORE_ADD_AS_FRIEND(time::ZoneId);

            /**
             * The Locale properties class
             */
            class Holder final : public Object {
                String language;
                String script;
                String region;
                String variant;

                CORE_ADD_AS_FRIEND(Locale);

                static Holder DEFAULT_LOCALE;
                static Holder DEFAULT_DISPLAY_LOCALE;
                static Holder DEFAULT_FORMAT_LOCALE;

            public:
                static String const SEP; // "_"
                static String const TAG_SEP; // "-"
                static String const PRIVATEUSE; // "x"
                static String const UNDETERMINED; // "und"
                static String const PRIVUSE_VARIANT_PREFIX; // "lvariant"

                CORE_IMPLICIT Holder();

                CORE_EXPLICIT Holder(String const &language,
                                     String const &script,
                                     String const &region,
                                     String const &variant);

                gbool equals(const Object &o) const override;

                gint hash() const override;

                String toString() const override;

                static String convertOldISOCode(String const &language);

                //
                // Language subtag syntax checking methods
                //
                static gbool isLanguage(String const &s);

                static gbool isExtLang(String const &s);

                static gbool isScript(String const &s);

                static gbool isRegion(String const &s);

                static gbool isVariant(String const &s);

                static gbool isExtensionSingleton(String const &s);

                static gbool isExtensionSingletonChar(gchar c);

                static gbool isExtensionSubtag(String const &s);

                static gbool isPrivateUsePrefix(String const &s);

                static gbool isPrivateUsePrefixChar(gchar c);

                static gbool isPrivateUseSubtag(String const &s);

                static String searchAlpha2Language(String const &alpha3Language);

                static String searchAlpha3Language(String const &alpha2Language);

                static String searchAlpha2Country(String const &alpha3Country);

                static String searchAlpha3Country(String const &alpha2Country);

                static gbool searchScript(String const &script);

                static void initDefault();

                static void setDefault(gbool display, Holder const &newLocale);

                //
                // Language subtag canonicalization methods
                //
                static String canonicalizeLanguage(String const &s);

                static String canonicalizeExtLanguage(String const &s);

                static String canonicalizeScript(String const &s);

                static String canonicalizeRegion(String const &s);

                static String canonicalizeVariant(String const &s);

                static String canonicalizeExtension(String const &s);

                static String canonicalizeExtensionSingleton(String const &s);

                static String canonicalizeExtensionSubtag(String const &s);

                static String canonicalizePrivateUse(String const &s);

                static String canonicalizePrivateSubtag(String const &s);

                /*
                 * BNF in RFC5646
                 *
                 * Language-Tag  = langtag             ; normal language tags
                 *               / privateuse          ; private use tag
                 *               / grandfathered       ; grandfathered tags
                 *
                 *
                 * langtag       = language
                 *                 ["-" script]
                 *                 ["-" region]
                 *                 *("-" variant)
                 *                 *("-" extension)
                 *                 ["-" privateuse]
                 *
                 * language      = 2*3ALPHA            ; shortest ISO 639 code
                 *                 ["-" extlang]       ; sometimes followed by
                 *                                     ; extended language subtags
                 *               / 4ALPHA              ; or reserved for future use
                 *               / 5*8ALPHA            ; or registered language subtag
                 *
                 * extlang       = 3ALPHA              ; selected ISO 639 codes
                 *                 *2("-" 3ALPHA)      ; permanently reserved
                 *
                 * script        = 4ALPHA              ; ISO 15924 code
                 *
                 * region        = 2ALPHA              ; ISO 3166-1 code
                 *               / 3DIGIT              ; UN M.49 code
                 *
                 * variant       = 5*8alphanum         ; registered variants
                 *               / (DIGIT 3alphanum)
                 *
                 * extension     = singleton 1*("-" (2*8alphanum))
                 *
                 *                                     ; Single alphanumerics
                 *                                     ; "x" reserved for private use
                 * singleton     = DIGIT               ; 0 - 9
                 *               / %x41-57             ; A - W
                 *               / %x59-5A             ; Y - Z
                 *               / %x61-77             ; a - w
                 *               / %x79-7A             ; y - z
                 *
                 * privateuse    = "x" 1*("-" (1*8alphanum))
                 *
                 */
                static Holder parse(String const &tag, gbool strict);

                //
                // Language subtag parsers
                //
                gbool parseLanguage(StringTokenizer &token, gbool &isError);

                gbool parseExtLangs(StringTokenizer &token, gbool &isError);

                gbool parseScript(StringTokenizer &token, gbool &isError);

                gbool parseRegion(StringTokenizer &token, gbool &isError);

                gbool parseVariants(StringTokenizer &token, gbool &isError);

                gbool parseExtensions(StringTokenizer &token, gbool &isError) const;

                gbool parsePrivateUse(StringTokenizer &token, gbool &isError);

                static Holder parseLocale(Holder const &base);

                /**
                 * Resolve this locale for translation
                 */
                IntArray resolve(gbool ignoreVariant) const;
            };

            /**
             * The locale properties field.
             */
            Holder base;

            /**
             * The cache for last returned language tag.
             */
            String mutable languageTag;

            /**
             * The internal constructor.
             *
             * @param holder the locale holder.
             */
            CORE_IMPLICIT Locale(Holder holder);

        public:
            /**
             * Enum for locale categories.  These locale categories are used to get/set
             * the default locale for the specific functionality represented by the
             * category.
             *
             * @see getDefault(Locale::Category)
             * @see setDefault(Locale::Category, Locale)
             */
            enum Category {
                /**
                 * Category used to represent the default locale for
                 * displaying user interfaces.
                 */
                DISPLAY,

                /**
                 * Category used to represent the default locale for
                 * formatting dates, numbers, and/or currencies.
                 */
                FORMAT,
            };

            /**
             * Enum for specifying the type defined in ISO 3166. This enum is used to
             * retrieve the two-letter ISO3166-1 alpha-2, three-letter ISO3166-1
             * alpha-3, four-letter ISO3166-3 country codes.
             *
             * @see isoCountries(Locale::CountryCode)
             */
            enum class CountryCode {
                /**
                 * PART1_ALPHA2 is used to represent the ISO3166-1 alpha-2 two letters
                 * country codes.
                 */
                PART1_ALPHA2,

                /**
                 *
                 * PART1_ALPHA3 is used to represent the ISO3166-1 alpha-3 three letters
                 * country codes.
                 */
                PART1_ALPHA3,

                /**
                 * PART3 is used to represent the ISO3166-3 four letters country codes.
                 */
                PART3
            };

            /**
             * Useful constant for language.
             */
            static const Locale ENGLISH;

            /**
             * Useful constant for language.
             */
            static const Locale FRENCH;

            /**
             * Useful constant for language.
             */
            static const Locale GERMAN;

            /**
             * Useful constant for language.
             */
            static const Locale ITALIAN;

            /**
             * Useful constant for language.
             */
            static const Locale JAPANESE;

            /**
             * Useful constant for language.
             */
            static const Locale KOREAN;

            /**
             * Useful constant for language.
             */
            static const Locale CHINESE;

            /**
             * Useful constant for language.
             */
            static const Locale SIMPLIFIED_CHINESE;

            /**
             * Useful constant for language.
             */
            static const Locale TRADITIONAL_CHINESE;

            /**
             * Useful constant for country.
             */
            static const Locale FRANCE;

            /**
             * Useful constant for country.
             */
            static const Locale GERMANY;

            /**
             * Useful constant for country.
             */
            static const Locale ITALY;

            /**
             * Useful constant for country.
             */
            static const Locale JAPAN;

            /**
             * Useful constant for country.
             */
            static const Locale KOREA;

            /**
             * Useful constant for country.
             */
            static const Locale UK;

            /**
             * Useful constant for country.
             */
            static const Locale US;

            /**
             * Useful constant for country.
             */
            static const Locale CANADA;

            /**
             * Useful constant for country.
             */
            static const Locale CANADA_FRENCH;

            /**
             * Useful constant for the root locale.  The root locale is the locale whose
             * language, country, and script are empty ("") strings.  This is regarded
             * as the base locale of all locales, and is used as the language/country
             * neutral locale for the locale sensitive operations.
             */
            static const Locale ROOT;

            /**
             * Useful constant for country.
             */
            static const Locale CHINA;

            /**
             * Useful constant for country.
             */
            static const Locale PRC;

            /**
             * Useful constant for country.
             */
            static const Locale TAIWAN;

            /**
             * Construct a locale from a language code.
             * This constructor normalizes the language value to lowercase.
             * @note Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms.
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the @c Locale class description about
             * valid language values.
             */
            CORE_EXPLICIT Locale(String const &language);

            /**
             * Construct a locale from language and country.
             * This constructor normalizes the language value to lowercase and
             * the country value to uppercase.
             *
             * @note Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms.
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the @c Locale class description about
             * valid language values.
             * @param country An ISO 3166 alpha-2 country code or a UN M.49 numeric-3 area code.
             * See the @c Locale class description about valid country values.
             */
            CORE_EXPLICIT Locale(String const &language,
                                 String const &country);

            /**
             * Construct a locale from language, script and country.
             * This constructor normalizes the language value to lowercase, script value
             * composed by capital followed by three lowercase and the country value to uppercase.
             *
             * @note Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms.
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the @c Locale class description about
             * valid language values.
             * @param script ISO 15924 alpha-4 script code. See the @c Locale class description
             * about valid script values.
             * @param country An ISO 3166 alpha-2 country code or a UN M.49 numeric-3 area code.
             * See the @c Locale class description about valid country values.
             */
            CORE_EXPLICIT Locale(String const &language,
                                 String const &script,
                                 String const &country);

            /**
             * Construct a locale from language, script and country.
             * This constructor normalizes the language value to lowercase, script value
             * composed by capital followed by three lowercase and the country value to uppercase.
             *
             * @note Obsolete ISO 639 codes ("iw", "ji", and "in") are mapped to
             * their current forms.
             *
             * @param language An ISO 639 alpha-2 or alpha-3 language code, or a language subtag
             * up to 8 characters in length.  See the @c Locale class description about
             * valid language values.
             * @param script ISO 15924 alpha-4 script code. See the @c Locale class description
             * about valid script values.
             * @param country An ISO 3166 alpha-2 country code or a UN M.49 numeric-3 area code.
             * See the @c Locale class description about valid country values.
             * @param variant Any arbitrary value used to indicate a variation of a @c Locale.
             * See the @c Locale class description for the details.
             */
            CORE_EXPLICIT Locale(String const &language,
                                 String const &script,
                                 String const &country,
                                 String const &variant);

            /**
             * Gets the current value of the <b> default locale</b> for this instance of process.
             * <p>
             * The Process sets the default locale during startup
             * based on the host environment. It is used by many locale-sensitive
             * methods if no locale is explicitly specified.
             * It can be changed using the @b setDefault(Locale) method.
             * </p>
             * @return the default locale for this instance of the Process.
             */
            static Locale getDefault();

            /**
             * Gets the current value of the <b> default locale</b> for
             * the specified Category for this instance of the Process.
             * <p>
             * The Process sets the default locale during startup based
             * on the host environment. It is used by many locale-sensitive methods
             * if no locale is explicitly specified. It can be changed using the
             * <b> setDefault(Locale::Category, Locale)</b> method.
             * </p>
             * @param category the specified category to get the default locale
             * @return the default locale for the specified Category for this instance
             *     of the Process
             *
             * @see setDefault(Locale::Category, Locale)
             */
            static Locale getDefault(Category category);

            /**
             * Sets the <b> default locale</b> for
             * this instance of the Process. This does not affect the
             * host locale.
             * <p>
             * The Process sets the default locale during startup
             * based on the host environment. It is used by many locale-sensitive
             * methods if no locale is explicitly specified.
             * </p>
             * <p>
             * Since changing the default locale may affect many different areas
             * of functionality, this method should only be used if the caller
             * is prepared to reinitialize locale-sensitive code running
             * within the same Process.
             * </p>
             * <p>
             * By setting the default locale with this method, all the default
             * locales for each Category are also set to the specified default locale.
             * </p>
             *
             * @param newLocale the new default locale
             */
            static void setDefault(Locale const &newLocale);

            /**
             * Sets the <b> default locale</b> for the specified
             * Category for this instance of the Process. This does
             * not affect the host locale.
             * <p>
             * The Process sets the default locale during startup based
             * on the host environment. It is used by many locale-sensitive methods
             * if no locale is explicitly specified.
             * </p>
             * <p>
             * Since changing the default locale may affect many different areas of
             * functionality, this method should only be used if the caller is
             * prepared to reinitialize locale-sensitive code running within the
             * same Process.
             * </p>
             * @param category the specified category to set the default locale
             * @param newLocale the new default locale
             *
             * @see getDefault(Locale::Category)
             */
            static void setDefault(Category category, Locale const &newLocale);

            /**
             * @return an array of installed locales
             *
             * The returned array represents the union of locales supported.
             * At a minimum, the returned array must contain a
             * @c Locale instance equal to @b Locale::ROOT and
             * a @c Locale instance equal to @b Locale::US.
             */
            static Array<Locale> availableLocales();

            /**
             * Returns a list of all 2-letter country codes defined in ISO 3166.
             * Can be used to obtain Locales.
             * This method is equivalent to <b> availableISOCountry(Locale.CountryCode type)</b>
             * with @c type  @b CountryCode::PART1_ALPHA2.
             * <p>
             * <b>Note:</b> The @c Locale class also supports other codes for
             * country (region), such as 3-letter numeric UN M.49 area codes.
             * Therefore, the list returned by this method does not contain ALL valid
             * codes that can be used to obtain Locales.
             * </p>
             * <p>
             * Note that this method does not return obsolete 2-letter country codes.
             * ISO3166-3 codes which designate country codes for those obsolete codes,
             * can be retrieved from <b> #availableISOCountry(Locale.CountryCode type)</b> with
             * @c type  @b CountryCode::PART3.
             * @return An array of ISO 3166 two-letter country codes.
             */
            static StringArray getISOCountries();

            /**
             * @return a @c Set of ISO3166 country codes for the specified type
             * @param type @b Locale.IsoCountryCode specified ISO code type.
             * @see Locale::CountryCode
             */
            static StringArray getISOCountries(CountryCode type);

            /**
             * Returns a list of all 2-letter language codes defined in ISO 639.
             * Can be used to obtain Locales.
             * @note
             * @li ISO 639 is not a stable standard-; some languages' codes have changed.
             * The list this function returns includes both the new and the old codes for the
             * languages whose codes have changed.
             * @li The @c Locale class also supports language codes up to
             * 8 characters in length.  Therefore, the list returned by this method does
             * not contain ALL valid codes that can be used to obtain Locales.
             *
             * @return An array of ISO 639 two-letter language codes.
             */
            static StringArray getISOLanguages();

            /**
             * Returns the language code of this Locale.
             *
             * @note This method returns the new forms for the obsolete ISO 639
             * codes ("iw", "ji", and "in"). See <b>Legacy language codes</b>
             * for more information.
             *
             * @return The language code, or the empty string if none is defined.
             * @see displayLanguage
             */
            String language() const;

            /**
             * Returns the script for this locale, which should
             * either be the empty string or an ISO 15924 4-letter script
             * code. The first letter is uppercase and the rest are
             * lowercase, for example, 'Latn', 'Cyrl'.
             *
             * @return The script code, or the empty string if none is defined.
             * @see displayScript
             */
            String script() const;

            /**
             * Returns the country/region code for this locale, which should
             * either be the empty string, an uppercase ISO 3166 2-letter code,
             * or a UN M.49 3-digit code.
             *
             * @return The country/region code, or the empty string if none is defined.
             * @see displayCountry
             */
            String country() const;

            /**
             * Returns the variant code for this locale.
             *
             * @return The variant code, or the empty string if none is defined.
             * @see #displayVariant
             */
            String variant() const;

            /**
             * Returns a string representation of this @c Locale
             * object, consisting of language, country, variant, script,
             * and extensions as below:
             * @code
             * language + "_" + country ( + "_#" + script)
             * @endcode
             *
             * Language is always lower case, country is always upper case, script is always title
             * case.
             *
             * <p>
             * If both the language and country fields are missing, this function will return
             * the empty string, even if the script is present (you
             * can't have a locale with just a variant, the variant must accompany a well-formed
             * language or country code).
             * </p>
             *
             * <p>
             * Examples:
             * @li @c en
             * @li @c en_US
             * @li @c _US
             * @li @c en_#Latn
             * @li @c zh_CN_#Hans
             * </p>
             * @return A string representation of the Locale, for debugging.
             * @see displayName
             * @see toLanguageTag
             */
            String toString() const override;

            /**
             * Returns a well-formed IETF BCP 47 language tag representing
             * this locale.
             *
             * <p>
             * If this @c Locale has a language, country, or
             * variant that does not satisfy the IETF BCP 47 language tag
             * syntax requirements, this method handles these fields as
             * described below:
             * </p>
             * <p>
             * <b>Language:</b> If language is empty, or not <em> well-formed</em>
             * (for example "a" or "e2"), it will be emitted as "und" (Undetermined).
             * </p>
             * <p>
             * <b>Country:</b> If country is not <em> well-formed</em>
             * (for example "12" or "USA"), it will be omitted.
             * </p>
             * <p>
             * <b>Special Conversions:</b> Core24 supports some old locale
             * representations, including deprecated ISO language codes,
             * for compatibility. This method performs the following
             * conversions:
             * @li Deprecated ISO language codes "iw", "ji", and "in" are
             * converted to "he", "yi", and "id", respectively.
             *
             * @li A locale with language "no", country "NO", and variant
             * "NY", representing Norwegian Nynorsk (Norway), is converted
             * to a language tag "nn-NO".
             * </p>
             * @note Although the language tag obtained by this
             * method is well-formed (satisfies the syntax requirements
             * defined by the IETF BCP 47 specification), it is not
             * necessarily a valid BCP 47 language tag.  For example,
             * @code
             *   Locale::forLanguageTag("xx-YY"_S).toLanguageTag();
             * @endcode
             *
             * will return "xx-YY", but the language subtag "xx" and the
             * region subtag "YY" are invalid because they are not registered
             * in the IANA Language Subtag Registry.
             *
             * @return a BCP47 language tag representing the locale
             * @see forLanguageTag(String)
             */
            String toLanguageTag() const;

            /**
             * This method formats a language tag into one with case convention
             * that adheres to section 2.1.1. Formatting of Language Tags of RFC5646.
             * This format is defined as: <i>All subtags, including extension and private
             * use subtags, use lowercase letters with two exceptions: two-letter
             * and four-letter subtags that neither appear at the start of the tag
             * nor occur after singletons. Such two-letter subtags are all
             * uppercase (as in the tags "en-CA-x-ca" or "sgn-BE-FR") and four-
             * letter subtags are titlecase (as in the tag "az-Latn-x-latn").</i> As
             * legacy tags, (defined as "grandfathered" in RFC5646) are not always well-formed, this method
             * will simply case fold a legacy tag to match the exact case convention
             * for the particular tag specified in the respective
             * <b> Legacy tags</b> table.
             *
             * <p>
             * <b>Special Exceptions</b>
             * </p>
             * <p>
             * To maintain consistency with @b variant
             * which is case-sensitive, this method will neither case fold variant
             * subtags nor case fold private use subtags prefixed by @c lvariant.
             * </p>
             * <p>
             * For example,
             * @code
             * String tag = "ja-kana-jp-x-lvariant-Oracle";
             * Locale::caseFoldLanguageTag(tag); // returns "ja-Kana-JP-Oracle"
             * String tag2 = "ja-kana-jp-x-Oracle";
             * Locale::caseFoldLanguageTag(tag2); // returns "ja-Kana-JP"
             * @endcode
             * </p>
             * <p>
             * Excluding case folding, this method makes no modifications to the tag itself.
             * Case convention of language tags does not carry meaning, and is simply
             * recommended as it corresponds with various ISO standards, including:
             * ISO639-1, ISO15924, and ISO3166-1.
             * </p>
             * <p>
             * As the formatting of the case convention is dependent on the
             * positioning of certain subtags, callers of this method should ensure
             * that the language tag is well-formed, (conforming to section 2.1. Syntax
             * of RFC5646).
             * </p>
             * @param languageTag the IETF BCP 47 language tag.
             * @return a case folded IETF BCP 47 language tag
             * @throws IllformedLocaleException if @c languageTag is not well-formed
             * @see https://www.rfc-editor.org/rfc/rfc5646.html#section-2.1
             *       RFC5646 2.1. Syntax
             * @see https://www.rfc-editor.org/rfc/rfc5646#section-2.1.1
             *       RFC5646 2.1.1. Formatting of Language Tags
             */
            static String caseFoldLanguageTag(String const &languageTag);

            /**
             * Returns a locale for the specified IETF BCP 47 language tag string.
             *
             * <p>
             * If the specified language tag contains any ill-formed subtags,
             * the first such subtag and all following subtags are ignored.
             * </p>
             * <p>
             * The following <b>conversions</b> are performed:
             * @li The language code "und" is mapped to language "".
             *
             * @li The language codes "iw", "ji", and "in" are mapped to "he",
             * "yi", and "id" respectively. (This is the same canonicalization
             * that's done in Locale's constructors.) See <em>Legacy language codes</em>
             * for more information.
             *
             * @li When the languageTag argument contains an extlang subtag,
             * the first such subtag is used as the language, and the primary
             * language subtag and other extlang subtags are ignored:
             *
             * @code
             *     Locale.forLanguageTag("ar-aao").getLanguage(); // returns "aao"
             *     Locale.forLanguageTag("en-abc-def-us").toString(); // returns "abc_US"
             * @endcode
             *
             * @li Case is normalized except for variant tags, which are left
             * unchanged.  Language is normalized to lower case, script to
             * title case, country to upper case, and extensions to lower
             * case.
             * </p>
             * <p>
             * This implements the 'Language-Tag' production of BCP47, and
             * so supports legacy (regular and irregular, referred to as
             * "Type: grandfathered" in BCP47) as well as
             * private use language tags.  Standalone private use tags are
             * represented as empty language and extension 'x-whatever',
             * and legacy tags are converted to their canonical replacements
             * where they exist.
             * </p>
             * <p>
             * Legacy tags with canonical replacements are as follows:
             * <b> Legacy tags with canonical replacements</b> <br/>
             * <b> legacy tag --> modern replacement </b>
             * @li art-lojban --> jbo
             * @li i-ami --> ami
             * @li i-bnn --> bnn
             * @li i-hak --> hak
             * @li i-klingon --> tlh
             * @li i-lux --> lb
             * @li i-navajo --> nv
             * @li i-pwn --> pwn
             * @li i-tao --> tao
             * @li i-tay --> tay
             * @li i-tsu --> tsu
             * @li no-bok --> nb
             * @li no-nyn --> nn
             * @li sgn-BE-FR --> sfb
             * @li sgn-BE-NL --> vgt
             * @li sgn-CH-DE --> sgg
             * @li zh-guoyu --> cmn
             * @li zh-hakka --> hak
             * @li zh-min-nan --> nan
             * @li zh-xiang --> hsn
             * </p>
             * <p>
             * Legacy tags with no modern replacement will be
             * converted as follows:
             * <b>Legacy tags with no modern replacement</b> <br/>
             * <b>legacy tag --> converts to </b> <br/>
             * @li cel-gaulish --> xtg-x-cel-gaulish
             * @li en-GB-oed --> en-GB-x-oed
             * @li i-default --> en-x-i-default
             * @li i-enochian --> und-x-i-enochian
             * @li i-mingo --> see-x-i-mingo
             * @li zh-min --> nan-x-zh-min
             * </p>
             * <p>
             * For a list of all legacy tags, see the
             * IANA Language Subtag Registry (search for "Type: grandfathered").
             * </p>
             *
             * @note there is no guarantee that @c toLanguageTag
             * and @c forLanguageTag will round-trip.
             *
             * @param languageTag the language tag
             * @return The locale that best represents the language tag.
             *
             * @see toLanguageTag()
             */
            static Locale forLanguageTag(String const &languageTag);

            /**
             * @return a three-letter abbreviation of this locale's language
             *
             * If the language matches an ISO 639-1 two-letter code, the
             * corresponding ISO 639-2/T three-letter lowercase code is
             * returned.  The ISO 639-2 language codes can be found on-line,
             * see "Codes for the Representation of Names of Languages Part 2:
             * Alpha-3 Code".  If the locale specifies a three-letter
             * language, the language is returned as is.  If the locale does
             * not specify a language the empty string is returned.
             *
             * @throws MissingResourceException Throws MissingResourceException if
             * three-letter language abbreviation is not available for this locale.
             */
            String getISO3Language() const;

            /**
             * @return a three-letter abbreviation of this locale's country
             *
             * If the country matches an ISO 3166-1 alpha-2 code, the
             * corresponding ISO 3166-1 alpha-3 uppercase code is returned.
             * If the locale doesn't specify a country, this will be the empty
             * string.
             *
             * <p>
             * The ISO 3166-1 codes can be found on-line.
             * </p>
             * @throws  MissingResourceException Throws MissingResourceException if the
             * three-letter country abbreviation is not available for this locale.
             */
            String getISO3Country() const;

            /**
             * Returns a name for the locale's language that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized for the default
             * @b DISPLAY locale.
             * For example, if the locale is fr_FR and the default
             * @b DISPLAY locale
             * is en_US, getDisplayLanguage() will return "French"; if the locale is en_US and
             * the default @b DISPLAY locale is fr_FR,
             * getDisplayLanguage() will return "anglais".
             * If the name returned cannot be localized for the default
             * @b DISPLAY locale,
             * (say, we don't have a Japanese name for Croatian),
             * this function falls back on the English name, and uses the ISO code as a last-resort
             * value.  If the locale doesn't specify a language, this function returns the empty string.
             *
             * @return The name of the display language.
             */
            String displayLanguage() const;

            /**
             * Returns a name for the locale's language that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized according to inLocale.
             * For example, if the locale is fr_FR and inLocale
             * is en_US, getDisplayLanguage() will return "French"; if the locale is en_US and
             * inLocale is fr_FR, getDisplayLanguage() will return "anglais".
             * If the name returned cannot be localized according to inLocale,
             * (say, we don't have a Japanese name for Croatian),
             * this function falls back on the English name, and finally
             * on the ISO code as a last-resort value.  If the locale doesn't specify a language,
             * this function returns the empty string.
             *
             * @param inLocale The locale for which to retrieve the display language.
             * @return The name of the display language appropriate to the given locale.
             */
            String displayLanguage(Locale const &inLocale) const;

            /**
             * Returns a name for the locale's script that is appropriate for display to
             * the user. If possible, the name will be localized for the default
             * @b DISPLAY locale.  Returns
             * the empty string if this locale doesn't specify a script code.
             *
             * @return the display name of the script code for the current default
             *     @b DISPLAY locale
             */
            String displayScript() const;

            /**
             * Returns a name for the locale's script that is appropriate
             * for display to the user. If possible, the name will be
             * localized for the given locale. Returns the empty string if
             * this locale doesn't specify a script code.
             *
             * @param inLocale The locale for which to retrieve the display script.
             * @return the display name of the script code for the current default
             * @b DISPLAY locale
             */
            String displayScript(Locale const &inLocale) const;

            /**
             * Returns a name for the locale's country that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized for the default
             * @b DISPLAY locale.
             * For example, if the locale is fr_FR and the default
             * @b DISPLAY, locale is en_US, getDisplayCountry() will return "France";
             * if the locale is en_US and the default @b DISPLAY locale is fr_FR,
             * getDisplayCountry() will return "Etats-Unis".
             * If the name returned cannot be localized for the default
             * @b DISPLAY locale,
             * (say, we don't have a Japanese name for Croatia),
             * this function falls back on the English name, and uses the ISO code as a last-resort
             * value.  If the locale doesn't specify a country, this function returns the empty string.
             *
             * @return The name of the country appropriate to the locale.
             */
            String displayCountry() const;

            /**
             * Returns a name for the locale's country that is appropriate for display to the
             * user.
             * If possible, the name returned will be localized according to inLocale.
             * For example, if the locale is fr_FR and inLocale
             * is en_US, getDisplayCountry() will return "France"; if the locale is en_US and
             * inLocale is fr_FR, getDisplayCountry() will return "Etats-Unis".
             * If the name returned cannot be localized according to inLocale.
             * (say, we don't have a Japanese name for Croatia),
             * this function falls back on the English name, and finally
             * on the ISO code as a last-resort value.  If the locale doesn't specify a country,
             * this function returns the empty string.
             *
             * @param inLocale The locale for which to retrieve the display country.
             * @return The name of the country appropriate to the given locale.
             */
            String displayCountry(Locale const &inLocale) const;

            /**
             * Returns a name for the locale's variant code that is appropriate for display to the
             * user.  If possible, the name will be localized for the default
             * @b DISPLAY locale.  If the locale
             * doesn't specify a variant code, this function returns the empty string.
             *
             * @return The name of the display variant code appropriate to the locale.
             */
            String displayVariant() const;

            /**
             * Returns a name for the locale's variant code that is appropriate for display to the
             * user.  If possible, the name will be localized for inLocale.  If the locale
             * doesn't specify a variant code, this function returns the empty string.
             *
             * @param inLocale The locale for which to retrieve the display variant code.
             * @return The name of the display variant code appropriate to the given locale.
             */
            String displayVariant(Locale const &inLocale) const;

            /**
             * Returns a name for the locale that is appropriate for display to the
             * user. This will be the values returned by getDisplayLanguage(),
             * getDisplayScript(), getDisplayCountry(), getDisplayVariant() and
             * optional <b> Unicode extensions</b>
             * assembled into a single string. The non-empty values are used in order, with
             * the second and subsequent names in parentheses.  For example:
             * <blockquote>
             * language (script, country, variant(, extension)*)<br>
             * language (country(, extension)*)<br>
             * language (variant(, extension)*)<br>
             * script (country(, extension)*)<br>
             * country (extension)*<br>
             * </blockquote>
             * depending on which fields are specified in the locale. The field
             * separator in the above parentheses, denoted as a comma character, may
             * be localized depending on the locale. If the language, script, country,
             * and variant fields are all empty, this function returns the empty string.
             *
             * @return The name of the locale appropriate to display.
             */
            String displayName() const;

            /**
             * Returns a name for the locale that is appropriate for display
             * to the user.  This will be the values returned by
             * getDisplayLanguage(), getDisplayScript(), getDisplayCountry(),
             * getDisplayVariant(), and optional <em>Unicode extensions</em>
             * assembled into a single string. The non-empty
             * values are used in order, with the second and subsequent names in
             * parentheses.  For example:
             * <blockquote>
             * language (script, country, variant)<br>
             * language (country)<br>
             * language (variant)<br>
             * script (country, variant)<br>
             * script (variant)<br>
             * country (variant)<br>
             * </blockquote>
             * depending on which fields are specified in the locale. The field
             * separator in the above parentheses, denoted as a comma character, may
             * be localized depending on the locale. If the language, script, country,
             * and variant fields are all empty, this function returns the empty string.
             *
             * @param inLocale The locale for which to retrieve the display name.
             * @return The name of the locale appropriate to display.
             */
            String displayName(Locale const &inLocale) const;

            /**
             * Return the shadow copy of this locale
             *
             * @return shadow copy
             */
            Object &clone() const override;

            /**
             * Return the hash code of this locale
             *
             * @return the hash code.
             */
            gint hash() const override;

            /**
             * Returns true if this Locale is equal to another object.  A Locale is
             * deemed equal to another Locale with identical language, script, country,
             * variant and extensions, and unequal to all other objects.
             *
             * @return true if this Locale is equal to the specified object.
             */
            gbool equals(const Object &obj) const override;
        };
    } // util
} // core

#endif //CORE24_LOCALE_H
