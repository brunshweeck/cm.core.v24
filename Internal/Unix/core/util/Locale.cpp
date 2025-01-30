//
// Created by brunshweeck on 11 sept. 2024.
//

#ifndef UNICODE
#define UNICODE
#endif

#include <stdlib.h>
#include <core/util/Locale.h>
#include <core/util/StringTokenizer.h>

namespace core {
    namespace util {
        void Locale::Holder::initDefault() {
            char const *LC_ALL = getenv("LC_ALL");
            if (!LC_ALL || *LC_ALL == '\0') LC_ALL = NULL;

            char const *LC_NUMERIC = !LC_ALL ? getenv("LC_NUMERIC") : NULL;
            if (!LC_NUMERIC || *LC_NUMERIC == '\0') LC_NUMERIC = NULL;

            char const *LC_TIME = !LC_ALL ? getenv("LC_TIME") : NULL;
            if (!LC_TIME || *LC_TIME == '\0') LC_TIME = NULL;

            char const *LC_MESSAGES = !LC_ALL ? getenv("LC_MESSAGES") : NULL;
            if (!LC_MESSAGES || *LC_MESSAGES == '\0') LC_MESSAGES = NULL;

            char const *LC_MEASUREMENT = !LC_ALL ? getenv("LC_MEASUREMENT") : NULL;
            if (!LC_MEASUREMENT || *LC_MEASUREMENT == '\0') LC_MEASUREMENT = NULL;

            char const *LC_COLLATE = !LC_ALL ? getenv("LC_COLLATE") : NULL;
            if (!LC_COLLATE || *LC_COLLATE == '\0') LC_COLLATE = NULL;

            char const *LC_MONETARY = !LC_ALL ? getenv("LC_MONETARY") : NULL;
            if (!LC_MONETARY || *LC_MONETARY == '\0') LC_MONETARY = NULL;

            char const *LANG = getenv("LANG");
            if (!LANG || *LANG == '\0' || *LANG == 'C') LANG = NULL;

            DEFAULT_LOCALE = {};
            DEFAULT_FORMAT_LOCALE = {};
            DEFAULT_DISPLAY_LOCALE = {};

            if (LANG != NULL) {
                StringTokenizer st = StringTokenizer(LANG, "_."_S);
                if (st.hasMoreTokens())
                    DEFAULT_LOCALE.language = st.nextToken();
                if (st.hasMoreTokens())
                    DEFAULT_LOCALE.region = st.nextToken();
            }

            if (LC_NUMERIC != NULL) {
                StringTokenizer st = StringTokenizer(String(LC_NUMERIC), "_."_S);
                if (st.hasMoreTokens())
                    DEFAULT_FORMAT_LOCALE.language = st.nextToken();
                if (st.hasMoreTokens())
                    DEFAULT_LOCALE.region = st.nextToken();
            } else if (LC_MONETARY != NULL) {
                StringTokenizer st = StringTokenizer(String(LC_MONETARY), "_."_S);
                if (st.hasMoreTokens())
                    DEFAULT_FORMAT_LOCALE.language = st.nextToken();
                if (st.hasMoreTokens())
                    DEFAULT_LOCALE.region = st.nextToken();
            } else if (LC_TIME != NULL) {
                StringTokenizer st = StringTokenizer(String(LC_TIME), "_."_S);
                if (st.hasMoreTokens())
                    DEFAULT_FORMAT_LOCALE.language = st.nextToken();
                if (st.hasMoreTokens())
                    DEFAULT_FORMAT_LOCALE.region = st.nextToken();
            }else
                DEFAULT_FORMAT_LOCALE = DEFAULT_LOCALE;

            if(LC_MESSAGES != NULL) {
                StringTokenizer st = StringTokenizer(String(LC_NUMERIC), "_."_S);
                if(st.hasMoreTokens())
                    DEFAULT_DISPLAY_LOCALE.language = st.nextToken();
                if(st.hasMoreTokens())
                    DEFAULT_DISPLAY_LOCALE.region = st.nextToken();
            }else if(LC_COLLATE != NULL) {
                StringTokenizer st = StringTokenizer(String(LC_COLLATE), "_."_S);
                if(st.hasMoreTokens())
                    DEFAULT_DISPLAY_LOCALE.language = st.nextToken();
                if(st.hasMoreTokens())
                    DEFAULT_DISPLAY_LOCALE.region = st.nextToken();
            }else
                DEFAULT_DISPLAY_LOCALE = DEFAULT_LOCALE;

            if(!isLanguage(DEFAULT_LOCALE.language))
                DEFAULT_LOCALE.language = String();

            if(!isLanguage(DEFAULT_DISPLAY_LOCALE.language))
                DEFAULT_LOCALE.language = String();

            if(!isLanguage(DEFAULT_FORMAT_LOCALE.language))
                DEFAULT_LOCALE.language = String();

            if(!isRegion(DEFAULT_LOCALE.region))
                DEFAULT_LOCALE.region = String();

            if(!isRegion(DEFAULT_DISPLAY_LOCALE.region))
                DEFAULT_LOCALE.region = String();

            if(!isRegion(DEFAULT_FORMAT_LOCALE.region))
                DEFAULT_LOCALE.region = String();
        }
    }
}
