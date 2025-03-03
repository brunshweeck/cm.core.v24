//
// Created by brunshweeck on 9 sept. 2024.
//

#ifndef CORE24_STANDARDCHARSETS_H
#define CORE24_STANDARDCHARSETS_H

#include <core/lang/Object.h>

namespace core {
    namespace charset {
        class StandardCharsets final : public virtual Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            // Maps alias names to canonical names
            static util::HashMap<String, String> aliases;

            // Maps canonical names to cached instances
            static util::HashMap<String, Charset> cache;

            static gint level;

            static void initialize();

            static void initializeAliases();

            static void initializeCache();

            static String canonicalize(const String& csn);

            static String toLower(String const& s);

        public:
            static Object& lookup(const String& csn);

            static util::Iterator<Charset>& charsets();
        };
    } // charset
} // core

#endif //CORE24_STANDARDCHARSETS_H
