//
// Created by brunshweeck
//


#include <core/lang/Array.h>
#include <core/lang/StringArray.h>
#include <core/lang/XString.h>
#include <core/concurrent/ReentrantLock.h>
#include <core/util/Locale.h>
#include <core/util/StringJoiner.h>
#include <core/util/StringTokenizer.h>
#include <meta/util/ASCII.h>
#include <meta/util/LocaleIsoData.h>
#include <meta/util/cldr/Cldr.h>
#include <meta/util/cldr/LikelySubtag.h>

namespace core {
using namespace locales;

    namespace util {
        Locale::Holder::Holder() {
            if (this == &DEFAULT_FORMAT_LOCALE)
                initDefault();
        }

        Locale::Holder::Holder(String const &language,
                               String const &script,
                               String const &region,
                               String const &variant)
            : language(ASCII::toLowerString(language)), script(script),
              region(ASCII::toUpperString(region)), variant(variant) {
        }

        gbool Locale::Holder::equals(const Object &o) const {
            if (this == &o)
                return true;

            if (!Class<Holder>::hasInstance(o))
                return false;

            Holder const &holder = CORE_XCAST(Holder const, o);

            if (language != holder.language)
                return false;

            if (script != holder.script)
                return false;

            if (region != holder.region)
                return false;

            if (variant != holder.variant)
                return false;

            return true;
        }

        gint Locale::Holder::hash() const {
            gint h = 0;
            h = h * 31 + language.hash();
            h = h * 31 + script.hash();
            h = h * 31 + region.hash();
            h = h * 31 + variant.hash();
            return h;
        }

        String Locale::Holder::toString() const {
            return Object::toString()
                   + R"([language=")"_S + language
                   + R"(", script=")"_S + script
                   + R"(", region=")"_S + region
                   + R"(", variant=")"_S + variant
                   + R"("])"_S;
        }

        String Locale::Holder::convertOldISOCode(String const &language) {
            if (UNSAFE::USE_LOCALE_OLD_ISO_CODES) {
                if (language.equals("iw"_S)) return "he"_S;
                if (language.equals("in"_S)) return "id"_S;
                if (language.equals("ji"_S)) return "yi"_S;
            } else {
                if (language.equals("he"_S)) return "iw"_S;
                if (language.equals("id"_S)) return "in"_S;
                if (language.equals("yi"_S)) return "ji"_S;
            }
            return language;
        }

        gbool Locale::Holder::isLanguage(String const &s) {
            // language      = 2*3ALPHA            ; shortest ISO 639 code
            //                 ["-" extlang]       ; sometimes followed by
            //                                     ;   extended language subtags
            //               / 4ALPHA              ; or reserved for future use
            //               / 5*8ALPHA            ; or registered language subtag
            gint n = s.length();
            return (n >= 2 && n <= 8) && ASCII::isAlphaString(s);
        }

        gbool Locale::Holder::isExtLang(String const &s) {
            // extlang       = 3ALPHA              ; selected ISO 639 codes
            //                 *2("-" 3ALPHA)      ; permanently reserved
            return s.length() == 3 && ASCII::isAlphaString(s);
        }

        gbool Locale::Holder::isScript(String const &s) {
            // script        = 4ALPHA              ; ISO 15924 code
            return s.length() == 4 && ASCII::isAlphaString(s);
        }

        gbool Locale::Holder::isRegion(String const &s) {
            // region        = 2ALPHA              ; ISO 3166-1 code
            //               / 3DIGIT              ; UN M.49 code
            return (s.length() == 2) && ASCII::isAlphaString(s)
                   || (s.length() == 3) && ASCII::isAlphaNumericString(s);
        }

        gbool Locale::Holder::isVariant(String const &s) {
            // variant       = 5*8alphanum         ; registered variants
            //               / (DIGIT 3alphanum)
            gint n = s.length();
            if (n >= 5 && n <= 8)
                return ASCII::isAlphaNumericString(s);

            if (n == 4)
                return ASCII::isNumeric(s.charAt(0))
                       && ASCII::isAlphaNumeric(s.charAt(1))
                       && ASCII::isAlphaNumeric(s.charAt(2))
                       && ASCII::isAlphaNumeric(s.charAt(3));

            return false;
        }

        gbool Locale::Holder::isExtensionSingleton(String const &s) {
            // singleton     = DIGIT               ; 0 - 9
            //               / %x41-57             ; A - W
            //               / %x59-5A             ; Y - Z
            //               / %x61-77             ; a - w
            //               / %x79-7A             ; y - z

            return (s.length() == 1)
                   && ASCII::isAlphaString(s)
                   && !ASCII::caseIgnoreMatch(PRIVATEUSE, s);
        }

        gbool Locale::Holder::isExtensionSingletonChar(gchar c) {
            return isExtensionSingleton(String::valueOf(c));
        }

        gbool Locale::Holder::isExtensionSubtag(String const &s) {
            // extension     = singleton 1*("-" (2*8alphanum))
            gint n = s.length();
            return (n >= 2) && (n <= 8) && ASCII::isAlphaNumericString(s);
        }

        gbool Locale::Holder::isPrivateUsePrefix(String const &s) {
            // privateuse    = "x" 1*("-" (1*8alphanum))
            return (s.length() == 1)
                   && ASCII::caseIgnoreMatch(PRIVATEUSE, s);
        }

        gbool Locale::Holder::isPrivateUsePrefixChar(gchar c) {
            return (ASCII::caseIgnoreMatch(PRIVATEUSE, String::valueOf(c)));
        }

        gbool Locale::Holder::isPrivateUseSubtag(String const &s) {
            // privateuse    = "x" 1*("-" (1*8alphanum))
            gint n = s.length();
            return (n >= 1) && (n <= 8) && ASCII::isAlphaNumericString(s);
        }

        String Locale::Holder::searchAlpha2Language(String const &alpha3Language) {
            if (alpha3Language.length() != 3)
                return alpha3Language;

            String const &db = LocaleISOData::ISO639Languages;

            gchar const c1 = alpha3Language.charAt(0);
            gchar const c2 = alpha3Language.charAt(1);
            gchar const c3 = alpha3Language.charAt(2);

            for (gint index = 0; index < db.length(); index += 5) {
                gchar c = db.charAt(index + 2);
                if (c > c1) break;
                if (c < c1) continue;
                c = db.charAt(index + 2 + 1);
                if (c > c2) break;
                if (c < c2) continue;
                c = db.charAt(index + 2 + 2);
                if (c > c3) break;
                if (c < c3) continue;
                return alpha3Language.subString(index, index + 2);
            }

            return String();
        }

        String Locale::Holder::searchAlpha3Language(String const &alpha2Language) {
            if (alpha2Language.length() != 2)
                return alpha2Language;

            String const &db = LocaleISOData::ISO639Languages;

            gchar const c1 = alpha2Language.charAt(0);
            gchar const c2 = alpha2Language.charAt(1);

            for (gint index = 0; index < db.length(); index += 5) {
                gchar c = db.charAt(index);
                if (c > c1) break;
                if (c < c1) continue;
                c = db.charAt(index + 1);
                if (c > c2) break;
                if (c < c2) continue;
                return alpha2Language.subString(index + 2, index + 5);
            }

            return String();
        }

        String Locale::Holder::searchAlpha2Country(String const &alpha3Country) {
            if (alpha3Country.length() != 3)
                return alpha3Country;

            String const &db = LocaleISOData::ISO3166Countries;

            gchar const c1 = alpha3Country.charAt(0);
            gchar const c2 = alpha3Country.charAt(1);
            gchar const c3 = alpha3Country.charAt(2);

            for (gint index = 0; index < db.length(); index += 5) {
                gchar c = db.charAt(index + 2);
                if (c > c1) break;
                if (c < c1) continue;
                c = db.charAt(index + 2 + 1);
                if (c > c2) break;
                if (c < c2) continue;
                c = db.charAt(index + 2 + 2);
                if (c > c3) break;
                if (c < c3) continue;
                return alpha3Country.subString(index, index + 2);
            }

            return String();
        }

        String Locale::Holder::searchAlpha3Country(String const &alpha2Country) {
            if (alpha2Country.length() != 2)
                return alpha2Country;

            String const &db = LocaleISOData::ISO3166Countries;

            gchar const c1 = alpha2Country.charAt(0);
            gchar const c2 = alpha2Country.charAt(1);

            for (gint index = 0; index < db.length(); index += 5) {
                gchar c = db.charAt(index);
                if (c > c1) break;
                if (c < c1) continue;
                c = db.charAt(index + 1);
                if (c > c2) break;
                if (c < c2) continue;
                return alpha2Country.subString(index + 2, index + 5);
            }

            return String();
        }

        gbool Locale::Holder::searchScript(String const &script) {
            if (script.length() != 4)
                return false;

            String const &db = LocaleISOData::ISOScripts;
            gint shift = 4;

            gchar const c1 = script.charAt(0);
            gchar const c2 = script.charAt(1);
            gchar const c3 = script.charAt(2);
            gchar const c4 = script.charAt(3);

            for (gint index = 0; index < db.length(); index += shift) {
                gchar c = db.charAt(index + 0);
                if (c > c1) break;
                if (c < c2) continue;
                c = db.charAt(index + 1);
                if (c > c2) break;
                if (c < c2) continue;
                c = db.charAt(index + 2);
                if (c > c3) break;
                if (c < c3) continue;
                c = db.charAt(index + 2);
                if (c > c4) break;
                if (c < c4) continue;
                return true;
            }

            return false;
        }

        void Locale::Holder::setDefault(gbool display, Holder const &newLocale) {
        }

        String Locale::Holder::canonicalizeLanguage(String const &s) {
            return ASCII::toLowerString(s);
        }

        String Locale::Holder::canonicalizeExtLanguage(String const &s) {
            return ASCII::toLowerString(s);
        }

        String Locale::Holder::canonicalizeScript(String const &s) {
            return ASCII::toTitleString(s);
        }

        String Locale::Holder::canonicalizeRegion(String const &s) {
            return ASCII::toUpperString(s);
        }

        String Locale::Holder::canonicalizeVariant(String const &s) {
            return ASCII::toUpperString(s);
        }

        String Locale::Holder::canonicalizeExtension(String const &s) {
            return ASCII::toLowerString(s);
        }

        String Locale::Holder::canonicalizeExtensionSingleton(String const &s) {
            return ASCII::toLowerString(s);
        }

        String Locale::Holder::canonicalizeExtensionSubtag(String const &s) {
            return ASCII::toLowerString(s);
        }

        String Locale::Holder::canonicalizePrivateUse(String const &s) {
            return ASCII::toLowerString(s);
        }

        String Locale::Holder::canonicalizePrivateSubtag(String const &s) {
            return ASCII::toLowerString(s);
        }

        Locale::Holder Locale::Holder::parse(String const &languageTag, gbool strict) {
            StringTokenizer st = StringTokenizer(languageTag, TAG_SEP);

            Holder tag = Holder();
            gbool isError = false;

            // langTag must start with either language or privateuse
            if (tag.parseLanguage(st, isError)) {
                tag.parseExtLangs(st, isError);
                tag.parseScript(st, isError);
                tag.parseRegion(st, isError);
                tag.parseVariants(st, isError);
                tag.parseExtensions(st, isError);
            }
            tag.parsePrivateUse(st, isError);
            if (st.hasMoreTokens() && !isError) {
                String const current = st.nextToken();
                if (strict) {
                    if (current.isEmpty())
                        IllegalArgumentException("Empty subtag"_S).throws($ftrace());
                    else
                        IllegalArgumentException("Invalid subtag: "_S).throws($ftrace());
                }
            }
            return tag;
        }

        gbool Locale::Holder::parseLanguage(StringTokenizer &token, gbool &isError) {
            if (!token.hasMoreTokens() || isError)
                return false;

            gbool found = false;
            String s = token.nextToken();
            if (isLanguage(s)) {
                found = true;
                if (!s.equals(UNDETERMINED))
                    language = UNSAFE::moveInstance(s);
            } else
                token.goBack();
            return found;
        }

        gbool Locale::Holder::parseExtLangs(StringTokenizer &token, gbool &isError) {
            if (!token.hasMoreTokens() || isError)
                return false;

            gbool found = false;
            gint count = 0;

            String extLang;

            while (token.hasMoreTokens()) {
                String s = token.nextToken();
                if (!isExtLang(s)) {
                    token.goBack();
                    break;
                }

                found = true;
                count += 1;
                if (count == 1)
                    extLang = UNSAFE::moveInstance(s);

                if (count > 3)
                    break;
            }

            if (!extLang.isEmpty())
                language = UNSAFE::moveInstance(extLang);

            return found;
        }

        gbool Locale::Holder::parseScript(StringTokenizer &token, gbool &isError) {
            if (!token.hasMoreTokens() || isError)
                return false;

            gbool found = false;

            String s = token.nextToken();
            if (isScript(s)) {
                found = true;
                script = UNSAFE::moveInstance(s);
            } else
                token.goBack();

            return found;
        }

        gbool Locale::Holder::parseRegion(StringTokenizer &token, gbool &isError) {
            if (!token.hasMoreTokens() || isError)
                return false;

            gbool found = false;

            String s = token.nextToken();
            if (isRegion(s)) {
                found = true;
                region = UNSAFE::moveInstance(s);
            } else
                token.goBack();

            return found;
        }

        gbool Locale::Holder::parseVariants(StringTokenizer &token, gbool &isError) {
            if (!token.hasMoreTokens() || isError)
                return false;

            gbool found = false;
            StringJoiner variants = StringJoiner(SEP);
            while (token.hasMoreTokens()) {
                String s = token.nextToken();
                if (!isVariant(s)) {
                    token.goBack();
                    break;
                }
                found = true;
                variants.add(s);
            }

            if (found)
                variant = variants.toString();

            return found;
        }

        gbool Locale::Holder::parseExtensions(StringTokenizer &token, gbool &isError) const {
            if (!token.hasMoreTokens() || isError)
                return false;

            gbool found = false;
            StringJoiner extensions = StringJoiner(TAG_SEP);
            while (token.hasMoreTokens()) {
                String s = token.nextToken();
                if (isExtensionSingleton(s)) {
                    String singleton = s;
                    XString xs = XString(singleton);

                    while (token.hasMoreTokens()) {
                        s = token.nextToken();
                        if (isExtensionSubtag(s))
                            xs.append(TAG_SEP).append(s);
                        else {
                            token.goBack();
                            isError = true;
                            break;
                        }
                    }

                    extensions.add(xs);
                    found = true;
                } else {
                    token.goBack();
                    break;
                }
            }

            CORE_IGNORE(variant);

            return found;
        }

        gbool Locale::Holder::parsePrivateUse(StringTokenizer &token, gbool &isError) {
            if (!token.hasMoreTokens() || isError)
                return false;

            gbool found = false;
            String s = token.nextToken();

            String privateUse;
            if (isPrivateUsePrefix(s)) {
                XString xs = XString();
                gint cnt = 0;
                gbool var = false;
                while (token.hasMoreTokens()) {
                    s = token.nextToken();
                    if (!var && s.equals(PRIVUSE_VARIANT_PREFIX)) {
                        var = true;
                        continue;
                    }
                    if (!isPrivateUseSubtag(s) || !var) {
                        token.goBack();
                        break;
                    }
                    if (xs.isEmpty())
                        xs.append(s);
                    else
                        xs.append(SEP).append(s);
                    cnt += 1;
                }

                if (cnt == 0)
                    isError = true;
                else {
                    privateUse = xs.toString();
                    found = true;
                }
            } else
                token.goBack();

            if (found) {
                if (variant.isEmpty())
                    variant = /* PRIVUSE_VARIANT_PREFIX + */ privateUse;
                else
                    variant += SEP + /* PRIVUSE_VARIANT_PREFIX +  */privateUse;
            }

            CORE_IGNORE(privateUse);
            return found;
        }

        Locale::Holder Locale::Holder::parseLocale(Holder const &base) {
            Holder tag = Holder();

            String language = base.language;
            String script = base.script;
            String region = base.region;
            String variant = base.variant;

            gbool hasSubtag = false;
            String privUseVar; // store ill-formed variant subtags

            if (isLanguage(language)) {
                // Convert a deprecated language code to new code
                if (language.equals("iw"_S))
                    language = "he"_S;
                else if (language.equals("ji"_S))
                    language = "yi"_S;
                else if (language.equals("in"_S))
                    language = "id"_S;
                tag.language = language;
            }

            if (isScript(script)) {
                tag.script = canonicalizeScript(script);
                hasSubtag = true;
            }

            if (isRegion(region)) {
                tag.region = canonicalizeRegion(region);
                hasSubtag = true;
            }

            // Special handling for no_NO_NY - use nn_NO for language tag
            if (tag.language.equals("no"_S) && tag.region.equals("NO"_S) && variant.equals("NY"_S)) {
                tag.language = "nn"_S;
                variant = ""_S;
            }

            if (!variant.isEmpty()) {
                StringTokenizer token = StringTokenizer(variant, SEP);
                StringJoiner variants = StringJoiner(TAG_SEP);
                gint cnt = 0;
                while (token.hasMoreTokens()) {
                    String var = token.nextToken();
                    if (!isVariant(var)) {
                        token.goBack();
                        break;
                    }
                    variants.add(var); // Do not canonicalize!
                    cnt += 1;
                }
                if (cnt != 0) {
                    tag.variant = variants.toString();
                    hasSubtag = true;
                }
                if (token.hasMoreTokens()) {
                    // ill-formed variant subtags
                    StringJoiner sj = StringJoiner(TAG_SEP);
                    while (token.hasMoreTokens()) {
                        String prv = token.nextToken();
                        if (!isPrivateUseSubtag(prv)) {
                            // cannot use private use subtag - truncated
                            token.goBack();
                            break;
                        }
                        sj.add(prv);
                    };
                    if (sj.length() > 0)
                        privUseVar = sj.toString();
                }
            }

            // append ill-formed variant subtags to private use
            String privateuse;
            if (!privUseVar.isEmpty()) {
                privateuse = PRIVUSE_VARIANT_PREFIX + TAG_SEP + privUseVar;
                if (tag.variant.isEmpty())
                    tag.variant = privateuse;
                else
                    tag.variant += TAG_SEP + privateuse;
            }

            if (tag.language.isEmpty() && (hasSubtag || privateuse.isEmpty())) {
                // use lang "und" when 1) no language is available AND
                // 2) any of other subtags other than private use are available or
                // no private use tag is available
                tag.language = UNDETERMINED;
            }

            return tag;
        }

        IntArray Locale::Holder::resolve(gbool ignoreVariant) const {
            String L = language;
            String R = region;
            String S = script;
            String V = variant;

            // Resolve target locale.
            if (!L.isEmpty() && !Holder::isLanguage(L))
                L = String();

            if (!S.isEmpty() && !Holder::isScript(S))
                S = String();

            if (!R.isEmpty() && !Holder::isRegion(R))
                R = String();

            if (ignoreVariant || !Holder::isVariant(V))
                V = String();

            gint LID = -1;
            gint RID = -1;
            gint SID = -1;
            gint VID = -1;

            if (!L.isEmpty())
                LID = locales::Cldr::languageToID(Holder::canonicalizeLanguage(L));

            if (!S.isEmpty())
                SID = locales::Cldr::scriptToID(Holder::canonicalizeScript(S));

            if (!R.isEmpty())
                RID = locales::Cldr::regionToID(Holder::canonicalizeRegion(R));

            if (!V.isEmpty())
                VID = locales::Cldr::variantToID(ASCII::toUpperString(V));

            if (LID < 0)
                LID = locales::Cldr::languageToID("und"_S);

            if (LID < 0 || RID < 0 || SID < 0) {
                // Resolve invalid locale identifier with Likely Subtag IDs.
                IntArray IDs = locales::LikelySubtag::resolve(LID, SID, RID);
                if (IDs.length() == 3) {
                    LID = IDs[0];
                    SID = IDs[1];
                    RID = IDs[2];
                }
            }

            return IntArray::of(LID, SID, RID, VID);
        }

        Locale::Locale(Holder holder) {
            base.language = UNSAFE::moveInstance(holder.language);
            base.script = UNSAFE::moveInstance(holder.script);
            base.region = UNSAFE::moveInstance(holder.region);
            base.variant = UNSAFE::moveInstance(holder.variant);
        }

        Locale::Locale(String const &language): Locale(language, String(), String(), String()) {
        }

        Locale::Locale(String const &language, String const &country): Locale(language, String(), country, String()) {
        }

        Locale::Locale(String const &language, String const &script, String const &country)
            : Locale(language, script, country, String()) {
        }

        Locale::Locale(String const &language, String const &script, String const &country, String const &variant)
            : base(Holder::convertOldISOCode(language), script, country, variant) {
        }

        static concurrent::ReentrantLock monitor;

        Locale Locale::getDefault() {
            return Locale(Holder::DEFAULT_LOCALE);
        }

        Locale Locale::getDefault(Category category) {
            if (category == Category::DISPLAY)
                return Locale(Holder::DEFAULT_DISPLAY_LOCALE);

            return Locale(Holder::DEFAULT_FORMAT_LOCALE);
        }

        void Locale::setDefault(Locale const &newLocale) {
            monitor.lock();
            setDefault(Category::DISPLAY, newLocale);
            setDefault(Category::FORMAT, newLocale);
            Holder::DEFAULT_LOCALE = newLocale.base;
            monitor.unlock();
        }

        void Locale::setDefault(Category category, Locale const &newLocale) {
            monitor.lock();
            if (category == Category::DISPLAY)
                Holder::DEFAULT_DISPLAY_LOCALE = newLocale.base;
            else
                Holder::DEFAULT_FORMAT_LOCALE = newLocale.base;
            monitor.unlock();
        }

        Array<Locale> Locale::availableLocales() {
            return {};
        }

        StringArray Locale::getISOCountries() {
            String const &db = LocaleISOData::ISO3166Countries;
            StringArray result = StringArray(db.length() / 5);
            for (gint index = 0; index < db.length(); index += 5) {
                result[index] = db.subString(index, index + 2);
            }
            return result;
        }

        StringArray Locale::getISOCountries(CountryCode type) {
            switch (type) {
                case CountryCode::PART1_ALPHA2:
                    return getISOCountries();
                case CountryCode::PART1_ALPHA3: {
                    String const &db = LocaleISOData::ISO3166Countries;
                    StringArray result = StringArray(db.length() / 5);
                    for (gint index = 0; index < db.length(); index += 5) {
                        result[index] = db.subString(index + 2, index + 5);
                    }
                    return result;
                }
                case CountryCode::PART3: {
                    String const &db = LocaleISOData::ISO3166Regions;
                    StringArray result = StringArray(db.length() / 4);
                    for (gint index = 0; index < db.length(); index += 4) {
                        result[index] = db.subString(index, index + 4);
                    }
                    return result;
                }
            }
            CORE_ASSERT2(false, "Invalid country code type");
        }

        StringArray Locale::getISOLanguages() {
            String const &db = LocaleISOData::ISO639Languages;
            StringArray result = StringArray(db.length() / 5);
            for (gint index = 0; index < db.length(); index += 5) {
                result[index] = db.subString(index, index + 2);
            }
            return result;
        }

        String Locale::language() const {
            return base.language;
        }

        String Locale::script() const {
            return base.script;
        }

        String Locale::country() const {
            return base.region;
        }

        String Locale::variant() const {
            return base.variant;
        }

        String Locale::toString() const {
            gbool const l = !base.language.isEmpty();
            gbool const s = !base.script.isEmpty();
            gbool const r = !base.region.isEmpty();
            gbool const v = !base.variant.isEmpty();

            XString result = XString(base.language);

            if (s && (l || r)) {
                result.append(Holder::SEP)
                        .append(base.script); // This may just append '_'
            }

            if (r || (l && (v || s))) {
                result.append(Holder::SEP)
                        .append(base.region); // This may just append '_'
            }
            if (v && (l || r)) {
                result.append(Holder::SEP)
                        .append(base.variant);
            }

            return result.toString();
        }

        String Locale::toLanguageTag() const {
            String lTag = languageTag;
            if (!lTag.isEmpty())
                return lTag;

            Holder tag = Holder::parseLocale(base);
            XString result;

            String subtag = tag.language;
            if (!subtag.isEmpty())
                result.append(Holder::canonicalizeLanguage(subtag));

            subtag = tag.script;
            if (!subtag.isEmpty()) {
                result.append(Holder::TAG_SEP);
                result.append(Holder::canonicalizeScript(subtag));
            }

            subtag = tag.region;
            if (!subtag.isEmpty()) {
                result.append(Holder::TAG_SEP);
                result.append(Holder::canonicalizeScript(subtag));
            }

            subtag = tag.variant;
            if (!subtag.isEmpty()) {
                result.append(Holder::TAG_SEP);
                result.append(Holder::canonicalizeVariant(subtag));
            }

            String langTag = result.toString();
            monitor.lock();
            if (languageTag.isEmpty())
                languageTag = langTag;
            monitor.unlock();

            return langTag;
        }

        String Locale::caseFoldLanguageTag(String const &languageTag) {
            try {
                Holder tag = Holder::parse(languageTag, true);
                StringJoiner sj = StringJoiner(Holder::TAG_SEP);
                if (!tag.language.isEmpty())
                    sj.add(Holder::canonicalizeLanguage(tag.language));
                if (!tag.script.isEmpty())
                    sj.add(Holder::canonicalizeScript(tag.script));
                if (!tag.region.isEmpty())
                    sj.add(Holder::canonicalizeRegion(tag.region));
                if (!tag.variant.isEmpty())
                    sj.add(tag.variant.replace(Holder::SEP.charAt(0), Holder::TAG_SEP.charAt(0)));
                return sj.toString();
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        Locale Locale::forLanguageTag(String const &languageTag) {
            try {
                Holder tag = Holder::parse(languageTag, false);
                return Locale(tag);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        String Locale::getISO3Language() const {
            String alpha3 = Holder::searchAlpha3Language(base.language);
            if (alpha3.isEmpty())
                MissingResourceException("Couldn't find 3-letter language code for "_S + base.language,
                                         "FromData_"_S + toString(),
                                         "ShortLanguage"_S).throws($ftrace());
            return alpha3;
        }

        String Locale::getISO3Country() const {
            String iso3 = Holder::searchAlpha3Language(base.region);
            if (iso3.isEmpty())
                MissingResourceException("Couldn't find 3-letter country code for "_S + base.region,
                                         "FromData_"_S + toString(),
                                         "ShortCountry"_S).throws($ftrace());
            return iso3;
        }

        String Locale::displayLanguage() const {
            return displayLanguage(getDefault(Category::DISPLAY));
        }

        String Locale::displayLanguage(Locale const &inLocale) const {
            if (!Holder::isLanguage(base.language))
                return String();

            Holder target = inLocale.base;
            gbool useVar = !target.variant.isEmpty();
            if (useVar && target.variant.indexOf(Holder::SEP) > 0) {
                // Multiple variants. (auto disable variant dependency)
                useVar = false;
            }
            if (useVar && !Holder::isVariant(target.variant))
                useVar = false;

            // Find id for this locale language
            gint langID = locales::Cldr::languageToID(Holder::canonicalizeLanguage(base.language));

            if (langID < 0)
                // Missing resources.
                return String();

            gint scrID = locales::Cldr::scriptToID(Holder::canonicalizeScript(base.script));
            if (scrID < 0)
                scrID = -1;

            gint regID = locales::Cldr::regionToID(Holder::canonicalizeRegion(base.region));

            // Resolve target locale.
            IntArray IDs = target.resolve(!useVar);
            gint LID = IDs[0];
            gint SID = IDs[1];
            gint RID = IDs[2];
            gint VID = IDs[3];

            /* Find the translation class */
            locales::Cldr &error = locales::Cldr::forLocale(-1, -1, -1, -1);

            // The analysis status
            gbool LAS = false;
            gbool SAS = false;
            gbool RAS = false;
            gbool VAS = false;

            // language-script-region-variant
            if (VID >= 0 && !VAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, RID, VID);
                if (cldr != error) {
                    String display = cldr.displayLanguage(langID, scrID, regID);
                    if (!display.isEmpty())
                        return display;

                    VAS = cldr.hasVariant();
                    RAS = cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-script-region-?
            if (RID >= 0 && !RAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, RID, -1);
                if (cldr != error) {
                    String display = cldr.displayLanguage(langID, scrID, regID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS = cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-script-?-?
            if (SID >= 0 && !SAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayLanguage(langID, scrID, regID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-?-?-?
            if (LID >= 0 && !LAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, -1, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayLanguage(langID, scrID, regID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // ?-?-?-? (fallback to en-?-?-?)
            {
                static gint EN = locales::Cldr::languageToID("en"_S);
                static locales::Cldr &cldr = locales::Cldr::forLocale(EN, -1, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayLanguage(langID, scrID, regID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            return String();
        }

        String Locale::displayScript() const {
            return displayScript(getDefault(Category::DISPLAY));
        }

        String Locale::displayScript(Locale const &inLocale) const {
            if (!Holder::isScript(base.script))
                return String();

            Holder target = inLocale.base;
            gbool useVar = !target.variant.isEmpty();
            if (useVar && target.variant.indexOf(Holder::SEP) > 0) {
                // Multiple variants. (auto disable variant dependency)
                useVar = false;
            }
            if (useVar && !Holder::isVariant(target.variant))
                useVar = false;

            // Find id for this locale script
            gint scrID = locales::Cldr::scriptToID(Holder::canonicalizeScript(base.script));
            if (scrID < 0)
                return String();

            // Resolve target locale.
            IntArray IDs = target.resolve(!useVar);
            gint LID = IDs[0];
            gint SID = IDs[1];
            gint RID = IDs[2];
            gint VID = IDs[3];

            /* Find the translation class */
            locales::Cldr &error = locales::Cldr::forLocale(-1, -1, -1, -1);

            // The analysis status
            gbool LAS = false;
            gbool SAS = false;
            gbool RAS = false;
            gbool VAS = false;

            // language-script-region-variant
            if (VID >= 0 && !VAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, RID, VID);
                if (cldr != error) {
                    String display = cldr.displayScript(scrID);
                    if (!display.isEmpty())
                        return display;

                    VAS = cldr.hasVariant();
                    RAS = cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-script-region-?
            if (RID >= 0 && !RAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, RID, -1);
                if (cldr != error) {
                    String display = cldr.displayScript(scrID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS = cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-script-?-?
            if (SID >= 0 && !SAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayScript(scrID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-?-?-?
            if (LID >= 0 && !LAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, -1, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayScript(scrID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // ?-?-?-? (fallback to en-?-?-?)
            {
                static gint EN = locales::Cldr::languageToID("en"_S);
                static locales::Cldr &cldr = locales::Cldr::forLocale(EN, -1, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayScript(scrID);
                    if (!display.isEmpty())
                        return display;
                }
            }

            return String();
        }

        String Locale::displayCountry() const {
            return displayCountry(getDefault(Category::DISPLAY));
        }

        String Locale::displayCountry(Locale const &inLocale) const {
            if (!Holder::isRegion(base.region))
                return String();

            Holder target = inLocale.base;
            gbool useVar = !target.variant.isEmpty();
            if (useVar && target.variant.indexOf(Holder::SEP) > 0) {
                // Multiple variants. (auto disable variant dependency)
                useVar = false;
            }
            if (useVar && !Holder::isVariant(target.variant))
                useVar = false;

            // Find id for this locale region
            gint regID = locales::Cldr::regionToID(Holder::canonicalizeRegion(base.region));
            if (regID < 0)
                return String();

            // Resolve target locale.
            IntArray IDs = target.resolve(!useVar);
            gint LID = IDs[0];
            gint SID = IDs[1];
            gint RID = IDs[2];
            gint VID = IDs[3];

            /* Find the translation class */
            locales::Cldr &error = locales::Cldr::forLocale(-1, -1, -1, -1);

            // The analysis status
            gbool LAS = false;
            gbool SAS = false;
            gbool RAS = false;
            gbool VAS = false;

            // language-script-region-variant
            if (VID >= 0 && !VAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, RID, VID);
                if (cldr != error) {
                    String display = cldr.displayCountry(regID);
                    if (!display.isEmpty())
                        return display;

                    VAS = cldr.hasVariant();
                    RAS = cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-script-region-?
            if (RID >= 0 && !RAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, RID, -1);
                if (cldr != error) {
                    String display = cldr.displayCountry(regID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS = cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-script-?-?
            if (SID >= 0 && !SAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayCountry(regID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-?-?-?
            if (LID >= 0 && !LAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, -1, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayCountry(regID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // ?-?-?-? (fallback to en-?-?-?)
            {
                static gint EN = locales::Cldr::languageToID("en"_S);
                static locales::Cldr &cldr = locales::Cldr::forLocale(EN, -1, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayCountry(regID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            return String();
        }

        String Locale::displayVariant() const {
            return displayVariant(getDefault(Category::DISPLAY));
        }

        String Locale::displayVariant(Locale const &inLocale) const {
            // The variant should be multiple values linked by specified separator (by default is '_')
            // Retrieve the display name of this locale variant consist of split it and join the
            // display names of any part with semicolon followed by one space (", ") using the string joiner.
            if (base.variant.indexOf(Holder::SEP) > 0) {
                StringTokenizer st = StringTokenizer(base.variant, Holder::SEP);
                StringJoiner sj = StringJoiner(", "_S);
                gint cnt = 0;
                while (st.hasMoreTokens()) {
                    String var = st.nextToken();
                    if (!Holder::isVariant(var)) {
                        if (Holder::isPrivateUsePrefix(var) && cnt == 0)
                            cnt += 1;
                        else
                            sj.add(var);
                        continue;
                    }

                    Locale locale = Locale(Holder());
                    locale.base.variant = var;
                    String variant = locale.displayVariant(inLocale);
                    if (!variant.isEmpty())
                        sj.add(variant);
                    else
                        sj.add(var);
                }

                return sj.toString();
            }

            if (!Holder::isVariant(base.variant))
                return String();

            Holder target = inLocale.base;
            gbool useVar = !target.variant.isEmpty();
            if (useVar && target.variant.indexOf(Holder::SEP) > 0) {
                // Multiple variants. (auto disable variant dependency)
                useVar = false;
            }
            if (useVar && !Holder::isVariant(target.variant))
                useVar = false;

            // Find identifier for this locale variant
            gint ID = locales::Cldr::variantToID(Holder::canonicalizeVariant(base.variant));
            if (ID < 0)
                return String();

            IntArray IDs = target.resolve(!useVar);
            gint LID = IDs[0];
            gint SID = IDs[1];
            gint RID = IDs[2];
            gint VID = IDs[3];

            /* Find the translation class */
            locales::Cldr &error = locales::Cldr::forLocale(-1, -1, -1, -1);

            // The analysis status
            gbool LAS = false;
            gbool SAS = false;
            gbool RAS = false;
            gbool VAS = false;

            // language-script-region-variant
            if (VID >= 0 && !VAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, RID, VID);
                if (cldr != error) {
                    String display = cldr.displayVariant(ID);
                    if (!display.isEmpty())
                        return display;

                    VAS = cldr.hasVariant();
                    RAS = cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-script-region-?
            if (RID >= 0 && !RAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, RID, -1);
                if (cldr != error) {
                    String display = cldr.displayVariant(ID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS = cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-script-?-?
            if (SID >= 0 && !SAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, SID, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayVariant(ID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS = cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // language-?-?-?
            if (LID >= 0 && !LAS) {
                locales::Cldr &cldr = locales::Cldr::forLocale(LID, -1, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayVariant(ID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            // ?-?-?-? (fallback to en-?-?-?)
            {
                static gint EN = locales::Cldr::languageToID("en"_S);
                static locales::Cldr &cldr = locales::Cldr::forLocale(EN, -1, -1, -1);
                if (cldr != error) {
                    String display = cldr.displayVariant(ID);
                    if (!display.isEmpty())
                        return display;

                    VAS |= cldr.hasVariant();
                    RAS |= cldr.hasRegion();
                    SAS |= cldr.hasScript();
                    LAS |= !VAS && !RAS && !SAS;
                }
            }

            return String();
        }

        String Locale::displayName() const {
            return displayName(getDefault(Category::DISPLAY));
        }

        String Locale::displayName(Locale const &inLocale) const {
            String language = displayLanguage(inLocale);
            String script = displayScript(inLocale);
            String region = displayCountry(inLocale);
            String variant = displayVariant(inLocale);

            StringArray display = StringArray(4);
            gint n = 0;
            if (!language.isEmpty())
                display[n++] = UNSAFE::moveInstance(language);
            if (!script.isEmpty())
                display[n++] = UNSAFE::moveInstance(script);
            if (!region.isEmpty())
                display[n++] = UNSAFE::moveInstance(region);
            if (!variant.isEmpty())
                display[n++] = UNSAFE::moveInstance(variant);

            gint index = 0;

            XString result;

            while (result.isEmpty() && index < n)
                result.append(display[index++]);

            StringJoiner sj = StringJoiner(", "_S, "("_S, ")"_S);
            while (index < n)
                sj.add(display[index++]);

            if (sj.length() > 2)
                result.append(u' ');
            result.append(sj.toString());
            return result.toString();
        }

        Object &Locale::clone() const {
            try {
                return UNSAFE::newInstance<Locale>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Locale::hash() const {
            return base.hash() ^ 123454321;
        }

        gbool Locale::equals(const Object &obj) const {
            return this == &obj ||
                   Class<Locale>::hasInstance(obj) && base.equals(CORE_XCAST(Locale const, obj).base);
        }

        // Statics Variables

        Locale::Holder Locale::Holder::DEFAULT_LOCALE = {};
        Locale::Holder Locale::Holder::DEFAULT_DISPLAY_LOCALE = {};
        Locale::Holder Locale::Holder::DEFAULT_FORMAT_LOCALE = {};

        String const Locale::Holder::SEP = "_"_S;
        String const Locale::Holder::TAG_SEP = "-"_S;
        String const Locale::Holder::PRIVATEUSE = "x"_S;
        String const Locale::Holder::UNDETERMINED = "und"_S;
        String const Locale::Holder::PRIVUSE_VARIANT_PREFIX = "lvariant"_S;

        Locale const Locale::ENGLISH = Holder("en"_S, String(), String(), String());
        Locale const Locale::FRENCH = Holder("fr"_S, String(), String(), String());
        Locale const Locale::GERMAN = Holder("de"_S, String(), String(), String());
        Locale const Locale::ITALIAN = Holder("it"_S, String(), String(), String());
        Locale const Locale::JAPANESE = Holder("ja"_S, String(), String(), String());
        Locale const Locale::KOREAN = Holder("ko"_S, String(), String(), String());
        Locale const Locale::CHINESE = Holder("zh"_S, String(), String(), String());
        Locale const Locale::SIMPLIFIED_CHINESE = Holder("zh"_S, String(), "CN"_S, String());
        Locale const Locale::TRADITIONAL_CHINESE = Holder("zh"_S, String(), "TW"_S, String());
        Locale const Locale::FRANCE = Holder("fr"_S, String(), "FR"_S, String());
        Locale const Locale::GERMANY = Holder("de"_S, String(), "DE"_S, String());
        Locale const Locale::ITALY = Holder("it"_S, String(), "IT"_S, String());
        Locale const Locale::JAPAN = Holder("ja"_S, String(), "JP"_S, String());
        Locale const Locale::KOREA = Holder("ko"_S, String(), "KR"_S, String());
        Locale const Locale::UK = Holder("en"_S, String(), "GB"_S, String());
        Locale const Locale::US = Holder("en"_S, String(), "US"_S, String());
        Locale const Locale::CANADA = Holder("en"_S, String(), "CA"_S, String());
        Locale const Locale::CANADA_FRENCH = Holder("fr"_S, String(), "CA"_S, String());
        Locale const Locale::CHINA = SIMPLIFIED_CHINESE;
        Locale const Locale::TAIWAN = TRADITIONAL_CHINESE;
        Locale const Locale::ROOT = Holder();
    }
}
