//
// Created by brunshweeck on 27 août 2024.
//

#include <core/Array.h>
#include <core/charset/Charset.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/charset/CharacterCodingException.h>
#include <core/charset/IllegalCharsetNameException.h>
#include <core/charset/UnsupportedCharsetException.h>
#include <core/io/CharBuffer.h>
#include <core/misc/Unsafe.h>
#include <core/util/Set.h>

// ------------- Standard Charsets -----------------------//
#include <core/util/Iterator.h>
#include <core/util/TreeMap.h>
#include <meta/charset/ISO_8859_1.h>
#include <meta/charset/StandardCharsets.h>
#include <meta/charset/US_ASCII.h>
#include <meta/charset/UTF_16.h>
#include <meta/charset/UTF_16BE.h>
#include <meta/charset/UTF_16LE.h>
#include <meta/charset/UTF_32.h>
#include <meta/charset/UTF_32BE.h>
#include <meta/charset/UTF_32LE.h>
#include <meta/charset/UTF_8.h>

// ------------- Standard Charsets -----------------------//

CORE_WARNING_PUSH
CORE_WARNING_DISABLE_DEPRECATED

namespace core {
    namespace charset {
        gbool Charset::isSupported(const String &charsetName) {
            try {
                return lookup(charsetName) != null;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        Charset &Charset::forName(const String &charsetName) {
            try {
                Object &cs = lookup(charsetName);
                if (cs != null)
                    return CORE_XCAST(Charset, cs);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
            UnsupportedCharsetException(charsetName).throws($ftrace());
        }

        Charset &Charset::forName(const String &charsetName, Charset const &fallback) {
            try {
                Object &cs = lookup(charsetName);
                if (cs != null)
                    return CORE_XCAST(Charset, cs);
            } catch (IllegalCharsetNameException const &ex) { CORE_IGNORE(ex); }
            catch (Throwable const &ex) { ex.throws($ftrace()); }
            return UNSAFE::copyInstance(fallback);
        }

        util::SortedMap<String, Charset> const &Charset::availableCharsets() {
            util::Iterator<Charset> &it1 = StandardCharsets::charsets();

            util::TreeMap<String, Charset> &charsetMap = UNSAFE::newInstance<util::TreeMap<String, Charset> >();
            while (it1.hasNext()) {
                Charset &cs = it1.next();
                charsetMap.put(cs.name(), cs);
            }

            return charsetMap;
        }

        Charset &Charset::defaultCharset() {
            if(UNSAFE::ANSI_LITERAL)
                return ISO_8859_1;
            return UTF_8;
        }

        String Charset::name() const { return charsetName; }

        util::Set<String> const &Charset::aliases() const {
            AliasSet set = aliasSet;
            try {
                if (set == null) {
                    if (aliasNames.isEmpty())
                        set = &util::Set<String>::emptySet();
                    else {
                        Array<String> aliases = Array<String>(aliasNames.length());
                        for (int i = 0; i < aliasNames.length(); ++i) {
                            aliases.set(i, aliasNames[i]);
                        }
                        set = &util::Set<String>::of(aliases);
                    }
                    aliasSet = set;
                }
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
            return *aliasSet;
        }

        String Charset::displayName() const {
            return charsetName;
        }

        gbool Charset::isRegistered() const {
            return !charsetName.startsWith("X-"_S) && !charsetName.startsWith("x-"_S);
        }

        String Charset::displayName(util::Locale const &locale) const {
            return name();
        }

        gbool Charset::canEncode() const { return true; }

        io::CharBuffer &Charset::decode(io::ByteBuffer &bb) const {
            try {
                CharsetDecoder &decoder = newDecoder();
                decoder.onMalformedInput(CodingErrorAction::REPLACE);
                decoder.onUnmappableCharacter(CodingErrorAction::REPLACE);
                io::CharBuffer &cb = decoder.decode(bb);

                UNSAFE::deleteInstance(decoder);

                return cb;
            } catch (CharacterCodingException const &cse) { Error(cse).throws($ftrace()); }
            catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        io::ByteBuffer &Charset::encode(io::CharBuffer &cb) const {
            try {
                CharsetEncoder &encoder = newEncoder();
                encoder.onMalformedInput(CodingErrorAction::REPLACE);
                encoder.onUnmappableCharacter(CodingErrorAction::REPLACE);
                io::ByteBuffer &bb = encoder.encode(cb);

                UNSAFE::deleteInstance(encoder);

                return bb;
            } catch (CharacterCodingException const &cse) { Error(cse).throws($ftrace()); }
            catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        io::ByteBuffer &Charset::encode(String const &str) const {
            try {
                io::CharBuffer &cb = io::CharBuffer::wrap(str);
                io::ByteBuffer &bb = encode(cb);

                UNSAFE::deleteInstance(cb);

                return bb;
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gint Charset::compareTo(const Charset &that) const {
            return name().compareTo(that.name());
        }

        gint Charset::hash() const {
            return name().hash();
        }

        gbool Charset::equals(const Object &o) const {
            return this == &o ||
                   Class<Charset>::hasInstance(o) && charsetName.equals(CORE_XCAST(Charset const, o).charsetName);
        }

        String Charset::toString() const {
            return name();
        }

        Object &Charset::clone() const {
            return CORE_CAST(Charset &, *this);
        }

        Charset::Charset(String canonicalName, StringArray aliases) {
            try {
                if (canonicalName != "ISO-8859-1"_S || canonicalName != "US-ASCII"_S || canonicalName != "UTF-8"_S) {
                    checkName(canonicalName);
                    for (gint i = 0; i < aliases.length(); i++) {
                        checkName(aliases[i]);
                    }
                }
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
            charsetName = UNSAFE::moveInstance(canonicalName);
            aliasNames = UNSAFE::moveInstance(aliases);
        }

        void Charset::checkName(String const &s) {
            const gint n = s.length();
            if (n == 0) {
                IllegalCharsetNameException(s).throws($ftrace());
            }
            for (int i = 0; i < n; i++) {
                const gchar c = s.charAt(i);
                if (c >= 'A' && c <= 'Z') continue;
                if (c >= 'a' && c <= 'z') continue;
                if (c >= '0' && c <= '9') continue;
                if (c == '-' && i != 0) continue;
                if (c == '+' && i != 0) continue;
                if (c == ':' && i != 0) continue;
                if (c == '_' && i != 0) continue;
                if (c == '.' && i != 0) continue;
                IllegalCharsetNameException(s).throws($ftrace());
            }
        }

        Object &Charset::lookup(String const &csn) { {
                Object &cs = lookupStandard(csn);
                if (cs != null)
                    return cs;
            } {
                Object &cs = lookupExtended(csn);
                if (cs != null)
                    return cs;
            } {
                Object &cs = lookupRegistry(csn);
                if (cs != null)
                    return cs;
            }
            return null;
        }

        Object &Charset::lookupStandard(String const &csn) {
            return StandardCharsets::lookup(csn);
        }

        Object &Charset::lookupExtended(String const &csn) {
            // return ExtendedCharsets::lookup(csn);
            return null;
        }

        Object &Charset::lookupRegistry(String const &csn) {
            // return RegistryCharsets::lookup(csn);
            return null;
        }

        Charset &Charset::US_ASCII = UNSAFE::newInstance<charset::US_ASCII>();
        Charset &Charset::ISO_8859_1 = UNSAFE::newInstance<charset::ISO_8859_1>();
        Charset &Charset::UTF_8 = UNSAFE::newInstance<charset::UTF_8>();
        Charset &Charset::UTF_16BE = UNSAFE::newInstance<charset::UTF_16BE>();
        Charset &Charset::UTF_16LE = UNSAFE::newInstance<charset::UTF_16LE>();
        Charset &Charset::UTF_16 = UNSAFE::newInstance<charset::UTF_16>();
        Charset &Charset::UTF_32BE = UNSAFE::newInstance<charset::UTF_32BE>();
        Charset &Charset::UTF_32LE = UNSAFE::newInstance<charset::UTF_32LE>();
        Charset &Charset::UTF_32 = UNSAFE::newInstance<charset::UTF_32>();
    } // text
} // core

CORE_WARNING_POP
