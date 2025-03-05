//
// Created by brunshweeck on 11 sept. 2024.
//

#ifndef UNICODE
#define UNICODE
#endif

#include <core/lang/Windows.h>
#include <core/util/Locale.h>

namespace core {
    namespace util {
        namespace {
            CORE_FAST struct Win32CodeToISO639 {
                gshort code;
                char const iso639[6];
            } EquivList[] = {
                {0x0401, "ar_SA"},
                {0x0402, "bg\0  "},
                {0x0403, "ca\0  "},
                {0x0404, "zh_TW"},
                {0x0405, "cs\0  "},
                {0x0406, "da\0  "},
                {0x0407, "de\0  "},
                {0x0408, "el\0  "},
                {0x0409, "en_US"},
                {0x040a, "es\0  "},
                {0x040b, "fi\0  "},
                {0x040c, "fr\0  "},
                {0x040d, "he\0  "},
                {0x040e, "hu\0  "},
                {0x040f, "is\0  "},
                {0x0410, "it\0  "},
                {0x0411, "ja\0  "},
                {0x0412, "ko\0  "},
                {0x0413, "nl\0  "},
                {0x0414, "no\0  "},
                {0x0415, "pl\0  "},
                {0x0416, "pt_BR"},
                {0x0418, "ro\0  "},
                {0x0419, "ru\0  "},
                {0x041a, "hr\0  "},
                {0x041c, "sq\0  "},
                {0x041d, "sv\0  "},
                {0x041e, "th\0  "},
                {0x041f, "tr\0  "},
                {0x0420, "ur\0  "},
                {0x0421, "in\0  "},
                {0x0422, "uk\0  "},
                {0x0423, "be\0  "},
                {0x0425, "et\0  "},
                {0x0426, "lv\0  "},
                {0x0427, "lt\0  "},
                {0x0429, "fa\0  "},
                {0x042a, "vi\0  "},
                {0x042d, "eu\0  "},
                {0x042f, "mk\0  "},
                {0x0436, "af\0  "},
                {0x0438, "fo\0  "},
                {0x0439, "hi\0  "},
                {0x043e, "ms\0  "},
                {0x0458, "mt\0  "},
                {0x0801, "ar_IQ"},
                {0x0804, "zh_CN"},
                {0x0807, "de_CH"},
                {0x0809, "en_GB"},
                {0x080a, "es_MX"},
                {0x080c, "fr_BE"},
                {0x0810, "it_CH"},
                {0x0812, "ko\0  "},
                {0x0813, "nl_BE"},
                {0x0814, "no\0  "},
                {0x0816, "pt\0  "},
                {0x081a, "sr\0  "},
                {0x081d, "sv_FI"},
                {0x0c01, "ar_EG"},
                {0x0c04, "zh_HK"},
                {0x0c07, "de_AT"},
                {0x0c09, "en_AU"},
                {0x0c0a, "es\0  "},
                {0x0c0c, "fr_CA"},
                {0x0c1a, "sr\0  "},
                {0x1001, "ar_LY"},
                {0x1004, "zh_SG"},
                {0x1007, "de_LU"},
                {0x1009, "en_CA"},
                {0x100a, "es_GT"},
                {0x100c, "fr_CH"},
                {0x1401, "ar_DZ"},
                {0x1407, "de_LI"},
                {0x1409, "en_NZ"},
                {0x140a, "es_CR"},
                {0x140c, "fr_LU"},
                {0x1801, "ar_MA"},
                {0x1809, "en_IE"},
                {0x180a, "es_PA"},
                {0x1c01, "ar_TN"},
                {0x1c09, "en_ZA"},
                {0x1c0a, "es_DO"},
                {0x2001, "ar_OM"},
                {0x2009, "en_JM"},
                {0x200a, "es_VE"},
                {0x2401, "ar_YE"},
                {0x2409, "en\0  "},
                {0x240a, "es_CO"},
                {0x2801, "ar_SY"},
                {0x2809, "en_BZ"},
                {0x280a, "es_PE"},
                {0x2c01, "ar_JO"},
                {0x2c09, "en_TT"},
                {0x2c0a, "es_AR"},
                {0x3001, "ar_LB"},
                {0x300a, "es_EC"},
                {0x3401, "ar_KW"},
                {0x340a, "es_CL"},
                {0x3801, "ar_AE"},
                {0x380a, "es_UY"},
                {0x3c01, "ar_BH"},
                {0x3c0a, "es_PY"},
                {0x4001, "ar_QA"},
                {0x400a, "es_BO"},
                {0x440a, "es_SV"},
                {0x480a, "es_HN"},
                {0x4c0a, "es_NI"},
                {0x500a, "es_PR"}
            };
        }

        void Locale::BaseLocale::initDefault() {
            LCID lcid = 0;
            lcid = GetUserDefaultLCID();
            CharArray out = CharArray(256);
            String lang;
            String script;
            String country;

            // do binary search on EquivList
            CORE_FAST gint n = ClassOf(EquivList)::count();
            gint from = 0;
            gint to = n - 1;
            gint m = 0;
            gbool success = false;
            while (from < to) {
                m = (from + to) >> 1;
                if (EquivList[m].code < lcid)
                    from = m + 1;
                else if (EquivList[m].code > lcid)
                    to = m - 1;
                else {
                    success = true;
                    if (EquivList[m].iso639[2] != '_')
                        lang = EquivList[m].iso639;
                    else {
                        lang = String(CharArray::of(EquivList[m].iso639[0], EquivList[m].iso639[1]));
                        country = EquivList[m].iso639 + 3;
                    }
                    break;
                }
            }

            if (!success) {
                // Get language
                gint r = GetLocaleInfo(lcid, LOCALE_SISO639LANGNAME, CORE_CAST(LPWSTR, &out[0]), 255);
                lang = String(out, 0, r - 1);

                // Get Script
                r = GetLocaleInfo(lcid, LOCALE_SSCRIPTS, CORE_CAST(LPWSTR, &out[0]), 255);
                for (gint i = 0; i < r; i++)
                    if (out[i] == ';') {
                        script = String(out, 0, i);
                        break;
                    }

                // Get Country/Region
                r = GetLocaleInfo(lcid, LOCALE_SISO3166CTRYNAME, CORE_CAST(LPWSTR, &out[0]), 255);
                country = String(out, 0, r - 1);
            }

            DEFAULT_LOCALE.language = lang;
            DEFAULT_LOCALE.script = script;
            DEFAULT_LOCALE.region = country;

            DEFAULT_DISPLAY_LOCALE = DEFAULT_LOCALE;
            DEFAULT_FORMAT_LOCALE = DEFAULT_LOCALE;
        }
    }
}
