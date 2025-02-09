//
// Created by brunshweeck on 27 août 2024.
//

#ifndef CORE24_CHARSET_H
#define CORE24_CHARSET_H

#include <core/StringArray.h>

#undef BIG_ENDIAN
#undef LITTLE_ENDIAN
#undef NATIVE_ENDIAN

namespace core {
    namespace charset {
        /**
         * A named mapping between sequences of sixteen-bit Unicode <b>code units</b>
         * and sequences of bytes.  This class defines methods for creating decoders
         * and encoders and for retrieving the various names associated with a charset.
         * Instances of this class are immutable.
         *
         * <p>
         * This class also defines static methods for testing whether a particular
         * charset is supported, for locating charset instances by name, and for
         * constructing a map that contains every charset for which support is
         * available.
         * </p>
         *
         * <p>
         * All the methods defined in this class are safe for use by multiple
         * concurrent threads.
         * </p>
         *
         * <h3>Charset names</h3>
         *
         * <p>
         * Charsets are named by strings composed of the following characters:<br/>
         *
         *  - The uppercase letters @c 'A' through @c 'Z'
         *        (@c '\u0041'  through @c '\u005a' ),<br/>
         *
         *  - The lowercase letters @c 'a' through @c 'z'
         *        (@c '\u0061'  through @c '\u007a' ),<br/>
         *
         *  - The digits @c '0' through @c '9'
         *        (@c '\u0030'  through @c '\u0039' ),<br/>
         *
         *  - The dash character @c '-'
         *        (@c '\u002d' , <small>HYPHEN-MINUS</small>),<br/>
         *
         *  - The plus character @c '+'
         *        (@c '\u002b' , <small>PLUS SIGN</small>),<br/>
         *
         *  - The period character @c '.'
         *        (@c '\u002e' , <small>FULL STOP</small>),<br/>
         *
         *  - The colon character @c ':'
         *        (@c '\u003a' , <small>COLON</small>), and<br/>
         *
         *  - The underscore character @c '_'
         *        (@c '\u005f' , <small>LOW LINE</small>).<br/>
         *
         * A charset name must begin with either a letter or a digit.  The empty string
         * is not a legal charset name.  Charset names are not case-sensitive; that is,
         * case is always ignored when comparing charset names.  Charset names
         * generally follow the conventions documented in
         * <a href="http://www.ietf.org/rfc/rfc2278.txt">RFC 2278: IANA Charset
         * Registration Procedures</a>.
         * </p>
         *
         * <p>
         * Every charset has a <i>canonical name</i> and may also have one or more
         * <i>aliases</i>.  The canonical name is returned by the @b name method
         * of this class.  Canonical names are, by convention, usually in upper case.
         * The aliases of a charset are returned by the @b aliases' method.
         * </p>
         *
         * <p>
         * If a charset listed in the <a href="http://www.iana.org/assignments/character-sets">
         * IANA Charset Registry</a> is supported by an implementation of the Core24 then
         * its canonical name must be the name listed in the registry. Many charsets
         * are given more than one name in the registry, in which case the registry
         * identifies one of the names as <i>MIME-preferred</i>.  If a charset has more
         * than one registry name then its canonical name must be the MIME-preferred
         * name and the other names in the registry must be valid aliases.  If a
         * supported charset is not listed in the IANA registry then its canonical name
         * must begin with one of the strings @c "X-" or @c "x-".
         * </p>
         *
         * <p>
         * The IANA charset registry does change over time, and so the canonical
         * name and the aliases of a particular charset may also change over time.  To
         * ensure compatibility it is recommended that no alias ever be removed from a
         * charset, and that if the canonical name of a charset is changed then its
         * previous canonical name be made into an alias.
         * </p>
         * <h3>Standard charsets</h3>
         * <p>
         * Every implementation is required to support the following standard charsets.
         * Consult the release documentation for your implementation to see if any other
         * charsets are supported.  The behavior of such optional charsets may differ
         * between implementations. <br>
         * <b>Description of standard charsets</b> <br/>
         *
         * - @c "US-ASCII" = Seven-bit ASCII, a.k.a. @c ISO646-US, a.k.a. the Basic Latin
         *                   block of the Unicode character set <br/>
         * - @c "ISO-8859-1" = ISO Latin Alphabet No. 1, a.k.a. @c ISO-LATIN-1 <br/>
         * - @c "UTF-8" = Eight-bit UCS Transformation Format <br/>
         * - @c "UTF-16BE" = Sixteen-bit UCS Transformation Format, big-endian byte order <br/>
         * - @c "UTF-16LE" = Sixteen-bit UCS Transformation Format, little-endian byte order <br/>
         * - @c "UTF-16" = Sixteen-bit UCS Transformation Format, byte order identified by
         *                  an optional byte-order mark <br/>
         * - @c "UTF-32BE" = Thirty-two-bit UCS Transformation Format, big-endian byte order <br/>
         * - @c "UTF-32LE" = Thirty-two-bit UCS Transformation Format, little-endian byte order <br/>
         * - @c "UTF-32" = Thirty-two-bit UCS Transformation Format, byte order identified by an
         *                  optional byte-order mark <br/>
         *
         * </p>
         * <p>
         * The @c UTF-8 charset is specified by
         * <a href="http://www.ietf.org/rfc/rfc2279.txt">RFC 2279</a>;
         * the transformation format upon which it is based is specified in
         * ISO 10646-1 and is also described in the
         * <a href="http://www.unicode.org/standard/standard.html">Unicode Standard</a>.
         * </p>
         * <p>
         * The @c UTF-16 charsets are specified by
         * <a href="http://www.ietf.org/rfc/rfc2781.txt"> RFC 2781</a>;
         * the transformation formats upon which they are based are specified in
         * ISO 10646-1 and are also described in the
         * <a href="http://www.unicode.org/standard/standard.html">Unicode Standard</a>.
         * </p>
         * <p> The @c UTF-32 charsets are based upon transformation formats which are specified in
         * ISO 10646-1 and are also described in the
         * <a href="http://www.unicode.org/standard/standard.html"> Unicode Standard</a>.
         * </p>
         * <p>
         * The @c UTF-16 and @c UTF-32 charsets use sixteen-bit and thirty-two-bit
         * quantities respectively, and are therefore sensitive to byte order.
         * In these encodings the byte order of a stream may be indicated by an initial <i>byte-order mark</i>
         * represented by the Unicode character @c U+FEFF.  Byte-order marks are handled as follows: <br/>
         *
         *   - When decoding, the @c UTF-16BE, @c UTF-16LE, @c UTF-32BE, and @c UTF-32LE
         *   charsets interpret the initial byte-order marks as a <small>ZERO-WIDTH
         *   NON-BREAKING SPACE</small>; when encoding, they do not write
         *   byte-order marks. <br/>
         *
         *   - When decoding, the @c UTF-16 and @c UTF-32 charsets interpret the
         *   byte-order mark at the beginning of the input stream to indicate the
         *   byte-order of the stream but defaults to big-endian if there is no
         *   byte-order mark; when encoding, it uses big-endian byte order and writes
         *   a big-endian byte-order mark. <br/>
         *
         * In any case, byte order marks occurring after the first element of an
         * input sequence are not omitted since the same code is used to represent
         * <small>ZERO-WIDTH NON-BREAKING SPACE</small>.
         * </p>
         * <p>
         * Every instance has a default charset, which is @c UTF-8 unless changed in
         * an implementation specific manner. Refer to @b defaultCharset() for more detail.
         * </p>
         * <h3>Terminology</h3>
         * <p>
         * The name of this class is taken from the terms used in
         * <a href="http://www.ietf.org/rfc/rfc2278.txt">RFC 2278</a>.
         * In that document a <i>charset</i> is defined as the combination of
         * one or more coded character sets and a character-encoding scheme.
         * (This definition is confusing; some other software systems define
         * <i>charset</i> as a synonym for <i>coded character set</i>.)
         * </p>
         * <p>
         * A <i>coded character set</i> is a mapping between a set of abstract
         * characters and a set of integers.  US-ASCII, ISO 8859-1,
         * JIS X 0201, and Unicode are examples of coded character sets.
         * </p>
         * <p>
         * Some standards have defined a <i>character set</i> to be simply a
         * set of abstract characters without an associated assigned numbering.
         * An alphabet is an example of such a character set.  However, the subtle
         * distinction between <i>character set</i> and <i>coded character set</i>
         * is rarely used in practice; the former has become a short form for the
         * latter.
         * </p>
         * <p>
         * A <i>character-encoding scheme</i> is a mapping between one or more
         * coded character sets and a set of octet (eight-bit byte) sequences.
         * UTF-8, UTF-16, ISO 2022, and EUC are examples of
         * character-encoding schemes.  Encoding schemes are often associated with
         * a particular coded character set; UTF-8, for example, is used only to
         * encode Unicode.  Some schemes, however, are associated with multiple
         * coded character sets; EUC, for example, can be used to encode
         * characters in a variety of Asian coded character sets.
         * </p>
         * <p>
         * When a coded character set is used exclusively with a single
         * character-encoding scheme then the corresponding charset is usually
         * named for the coded character set; otherwise a charset is usually named
         * for the encoding scheme and, possibly, the locale of the coded
         * character sets that it supports.  Hence, @c US-ASCII is both the
         * name of a coded character set and of the charset that encodes it, while
         * @c EUC-JP is the name of the charset that encodes the
         * JIS X 0201, JIS X 0208, and JIS X 0212
         * coded character sets for the Japanese language.
         * </p>
         * <p>
         * The native character encoding is UTF-16.
         * A charset therefore defines a mapping between sequences of sixteen-bit
         * UTF-16 code units (that is, sequences of chars) and sequences of bytes.
         * </p>
         *
         * @see http://www.iana.org/assignments/character-sets Character Sets
         * @see https://www.rfc-editor.org/info/rfc2278 RFC 2278: IANA Charset Registration Procedures
         * @see https://www.rfc-editor.org/info/rfc2279 RFC 2279: UTF-8, a transformation format of ISO 10646
         * @see https://www.rfc-editor.org/info/rfc2781 RFC 2781: UTF-16, an encoding of ISO 10646
         *
         * @see CharsetDecoder
         * @see CharsetEncoder
         * @see core::Character
         */
        class /* abstract */ Charset : public Comparable<Charset> {
            CORE_ALIAS(Aliases, Class<Array<String>>::Pointer);
            CORE_ALIAS(AliasSet, Class<util::Set<String>>::Pointer);

            String charsetName;

            StringArray aliasNames;

            AliasSet mutable aliasSet = null;

        public:
            /**
             * Tells whether the named charset is supported.
             *
             * @param  charsetName
             *         The name of the requested charset; may be either
             *         a canonical name or an alias
             *
             * @return  @c true if, and only if, support for the named charset
             *          is available.
             *
             * @throws IllegalCharsetNameException
             *         If the given charset name is illegal
             */
            static gbool isSupported(const String& charsetName);

            /**
             * Returns a charset object for the named charset.
             *
             * @param  charsetName
             *         The name of the requested charset; may be either
             *         a canonical name or an alias
             *
             * @return  A charset object for the named charset
             *
             * @throws  IllegalCharsetNameException
             *          If the given charset name is illegal
             *
             * @throws  UnsupportedCharsetException
             *          If no support for the named charset is available
             *          in this instance.
             */
            static Charset& forName(const String& charsetName);

            /**
             * Returns a charset object for the named charset. If the charset object
             * for the named charset is not available or @c charsetName is not a
             * legal charset name, then @c fallback is returned.
             *
             * @param  charsetName
             *         The name of the requested charset; may be either
             *         a canonical name or an alias
             *
             * @param  fallback
             *         fallback charset in case the charset object for the named
             *         charset is not available or @c charsetName is not a legal
             *         charset name.
             *
             * @return  A charset object for the named charset, or @c fallback
             *          in case the charset object for the named charset is not
             *          available or @c charsetName is not a legal charset name
             */
            static Charset& forName(const String& charsetName, Charset const& fallback);

            /**
             * Constructs a sorted map from canonical charset names to charset objects.
             *
             * <p>
             * The map returned by this method will have one entry for each charset
             * for which support is available.  If
             * two or more supported charsets have the same canonical name then the
             * resulting map will contain just one of them; which one it will contain
             * is not specified.
             * </p>
             * <p>
             * The invocation of this method, and the subsequent use of the
             * resulting map, may cause time-consuming disk or network I/O operations
             * to occur.  This method is provided for applications that need to
             * enumerate all the available charsets, for example to allow user
             * charset selection.  This method is not used by the
             * @b forName method, which instead employs an efficient incremental lookup
             * algorithm.
             * </p>
             * <p>
             * This method may return different results at different times if new
             * charset providers are dynamically made available.
             * In the absence of such changes, the charsets returned
             * by this method are exactly those that can be retrieved via the
             * @b forName method.
             * </p>
             *
             * @return An immutable, case-insensitive map from canonical charset names
             *         to charset objects
             */
            static util::SortedMap<String, Charset> const& availableCharsets();

            /**
             * Returns the default charset.
             *
             * <p>
             * The default charset is @c UTF-8, unless changed in an
             * implementation specific manner.
             * </p>
             *
             * @note If the compiler property Unsafe::ANSI_LITERAL is
             * predefined with value true (by default on MSVC). the
             * charset returned is ISO-8859-1.
             *
             * @return  A charset object for the default charset
             */
            static Charset& defaultCharset();

            /**
             * Returns this charset's canonical name.
             *
             * @return  The canonical name of this charset
             */
            String name() const;

            /**
             * Returns a set containing this charset's aliases.
             *
             * @return  An immutable set of this charset's aliases
             */
            util::Set<String> const& aliases() const;

            /**
             * Returns this charset's human-readable name for the default locale.
             *
             * <p>
             * The default implementation of this method simply returns this
             * charset's canonical name.  Concrete subclasses of this class may
             * override this method in order to provide a localized display name.
             * </p>
             *
             * @return  The display name of this charset in the default locale
             */
            virtual String displayName() const;

            /**
             * Tells whether this charset is registered in the
             * <a href="http://www.iana.org/assignments/character-sets">
             * IANA Charset Registry</a>.
             *
             * @return  @c true if, and only if, this charset is known by its
             *          implementor to be registered with the IANA
             * @spec http://www.iana.org/assignments/character-sets Character Sets
             */
            virtual gbool isRegistered() const;

            /**
             * Returns this charset's human-readable name for the given locale.
             *
             * <p>
             * The default implementation of this method simply returns this
             * charset's canonical name.  Concrete subclasses of this class may
             * override this method in order to provide a localized display name.
             * </p>
             *
             * @param  locale
             *         The locale for which the display name is to be retrieved
             *
             * @return  The display name of this charset in the given locale
             */
            virtual String displayName(util::Locale const& locale) const;

            // -----------------

            /**
             * Tells whether this charset contains the given charset.
             *
             * <p>
             * A charset <i>C</i> is said to <i>contain</i> a charset <i>D</i> if,
             * and only if, every character representable in <i>D</i> is also
             * representable in <i>C</i>.  If this relationship holds then it is
             * guaranteed that every string that can be encoded in <i>D</i> can also be
             * encoded in <i>C</i> without performing any replacements.
             * </p>
             * <p>
             * That <i>C</i> contains <i>D</i> does not imply that each character
             * representable in <i>C</i> by a particular byte sequence is represented
             * in <i>D</i> by the same byte sequence, although sometimes this is the
             * case.
             * </p>
             * <p>
             * Every charset contains itself.
             * </p>
             * <p>
             * This method computes an approximation of the containment relation:
             * If it returns @c true then the given charset is known to be
             * contained by this charset; if it returns @c false, however, then
             * it is not necessarily the case that the given charset is not contained
             * in this charset.
             * </p>
             * @param   cs
             *          The given charset
             *
             * @return  @c true if the given charset is contained in this charset
             */
            virtual gbool contains(Charset const& cs) const = 0;

            /**
             * Constructs a new decoder for this charset.
             *
             * @return  A new decoder for this charset
             */
            virtual CharsetDecoder& newDecoder() const = 0;

            /**
             * Constructs a new encoder for this charset.
             *
             * @return  A new encoder for this charset
             *
             * @throws  UnsupportedOperationException
             *          If this charset does not support encoding
             */
            virtual CharsetEncoder& newEncoder() const = 0;

            /**
             * Tells whether this charset supports encoding.
             *
             * <p>
             * Nearly all charsets support encoding.  The primary exceptions are
             * special-purpose <i>auto-detect</i> charsets whose decoders can determine
             * which of several possible encoding schemes is in use by examining the
             * input byte sequence.  Such charsets do not support encoding because
             * there is no way to determine which encoding should be used on output.
             * Implementations of such charsets should override this method to return
             * @c false.
             * </p>
             *
             * @return  @c true if, and only if, this charset supports encoding
             */
            virtual gbool canEncode() const;

            /**
             * Convenience method that decodes bytes in this charset into Unicode
             * characters.
             *
             * <p>
             * An invocation of this method upon a charset @c cs returns the
             * same result as the expression
             *
             * @code
             *     cs.newDecoder()
             *       .onMalformedInput(CodingErrorAction::REPLACE)
             *       .onUnmappableCharacter(CodingErrorAction::REPLACE)
             *       .decode(bb);
             * @endcode
             *
             * except that it is potentially more efficient because it can cache
             * decoders between successive invocations.
             * </p>
             * <p>
             * This method always replaces malformed-input and unmappable-character
             * sequences with this charset's default replacement byte array.  In order
             * to detect such sequences, use the @b CharsetDecoder::decode(ByteBuffer)
             * method directly.
             * </p>
             *
             * @param  bb  The byte buffer to be decoded
             *
             * @return  A char buffer containing the decoded characters
             */
            io::CharBuffer& decode(io::ByteBuffer& bb) const;

            /**
             * Convenience method that encodes Unicode characters into bytes in this
             * charset.
             *
             * <p>
             * An invocation of this method upon a charset @c cs returns the
             * same result as the expression
             *
             * @code
             *     cs.newEncoder()
             *       .onMalformedInput(CodingErrorAction::REPLACE)
             *       .onUnmappableCharacter(CodingErrorAction::REPLACE)
             *       .encode(bb);
             * @endcode
             *
             * except that it is potentially more efficient because it can cache
             * encoders between successive invocations.
             * </p>
             * <p>
             * This method always replaces malformed-input and unmappable-character
             * sequences with this charset's default replacement string.  In order to
             * detect such sequences, use the @b CharsetEncoder::encode(io::CharBuffer)
             * method directly.
             * </p>
             *
             * @param  cb  The char buffer to be encoded
             *
             * @return  A byte buffer containing the encoded characters
             */
            io::ByteBuffer& encode(io::CharBuffer& cb) const;

            /**
             * Convenience method that encodes a string into bytes in this charset.
             *
             * <p>
             * An invocation of this method upon a charset @c cs returns the
             * same result as the expression
             *
             * @code
             *     cs.encode(CharBuffer::wrap(s));
             * @endcode
             * </p>
             * @param  str  The string to be encoded
             *
             * @return  A byte buffer containing the encoded characters
             */
            io::ByteBuffer& encode(String const& str) const;

            /**
             * Compares this charset to another.
             *
             * <p>
             * Charsets are ordered by their canonical names, without regard to
             * case.
             * </p>
             *
             * @param  that
             *         The charset to which this charset is to be compared
             *
             * @return A negative integer, zero, or a positive integer as this charset
             *         is less than, equal to, or greater than the specified charset
             */
            gint compareTo(const Charset& that) const final;

            /**
             * @return the hash code for this charset
             */
            gint hash() const final;

            /**
             * Tells whether this object is equal to another.
             *
             * <p>
             * Two charsets are equal if, and only if, they have the same canonical
             * names.  A charset is never equal to any other type of object.
             * </p>
             *
             * @return  @c true if, and only if, this charset is equal to the
             *          given object
             */
            gbool equals(const Object& o) const final;

            /**
             * @return a string describing this charset
             */
            String toString() const final;

            /**
             * All classes deriving from Charset must have
             * single and unique instance.
             * @return itself
             */
            Object& clone() const final;

            /**
             * A typesafe enumeration for byte orders.
             */
            enum ByteOrder {
                /**
                 * Constant denoting big-endian byte order.  In this order, the toBytes of a
                 * multibyte value are ordered from most significant to least significant.
                 */
                BIG_ENDIAN,

                /**
                 * Constant denoting little-endian byte order.  In this order, the toBytes of
                 * a multibyte value are ordered from least significant to most
                 * significant.
                 */
                LITTLE_ENDIAN,

                // Retrieve the private byte order. It's used early during bootstrap, and
                // must be initialized after BIG_ENDIAN and LITTLE_ENDIAN.
                NATIVE_ENDIAN = (CORE_BYTE_ORDER == CORE_LITTLE_ENDIAN) ? LITTLE_ENDIAN : BIG_ENDIAN,
            };


            /**
             * A typesafe enumeration for coding-error actions.
             *
             * <p>
             * Instances of this class are used to specify how malformed-input and
             * unmappable-character errors are to be handled by charset @em decoders
             * and @em encoders.
             * </p>
             */
            enum CodingErrorAction {
                /**
                 * Action indicating that a coding error is to be handled by dropping the
                 * erroneous input and resuming the coding operation.
                 */
                IGNORE,

                /**
                 * Action indicating that a coding error is to be handled by dropping the
                 * erroneous input, appending the coder's replacement value to the output
                 * buffer, and resuming the coding operation.
                 */
                REPLACE,

                /**
                 * Action indicating that a coding error is to be reported, either by
                 * returning a @b CoderResult object or by throwing a
                 * @b CharacterCodingException, whichever is appropriate for the method
                 * implementing the coding process.
                 */
                REPORT,
            };

            // ---------------------------- Standards Charsets ---------------------------------- //

            /**
             * Seven-bit ASCII, also known as ISO646-US, also known as the
             * Basic Latin block of the Unicode character set.
             */
            static Charset& US_ASCII;

            /**
             * ISO Latin Alphabet <em> No. 1</em>, also known as ISO-LATIN-1.
             */
            static Charset& ISO_8859_1;

            /**
             * Eight-bit UCS Transformation Format.
             */
            static Charset& UTF_8;

            /**
             * Sixteen-bit UCS Transformation Format, big-endian byte order.
             */
            static Charset& UTF_16BE;

            /**
             * Sixteen-bit UCS Transformation Format, little-endian byte order.
             */
            static Charset& UTF_16LE;

            /**
             * Sixteen-bit UCS Transformation Format, byte order identified by an
             * optional byte-order mark.
             */
            static Charset& UTF_16;

            /**
             * Thirty-two-bit UCS Transformation Format, big-endian byte order.
             */
            static Charset& UTF_32BE;

            /**
             * Thirty-two-bit UCS Transformation Format, little-endian byte order.
             */
            static Charset& UTF_32LE;

            /**
             * Thirty-two-bit UCS Transformation Format, byte order identified by an
             * optional byte-order mark.
             */
            static Charset& UTF_32;

        protected:
            /**
             * Initializes a new charset with the given canonical name and alias
             * set.
             *
             * @param  canonicalName
             *         The canonical name of this charset
             *
             * @param  aliases
             *         An array of this charset's aliases, or null if it has no aliases
             *
             * @throws IllegalCharsetNameException
             *         If the canonical name or any of the aliases are illegal
             */
            CORE_EXPLICIT Charset(String canonicalName, StringArray aliases);

            CORE_ALIAS(UNSAFE, misc::Unsafe);

        private:
            /**
             * <p>
             * Checks that the given string is a legal charset name.
             * </p>
             *
             * @param  s
             *         A purported charset name
             *
             * @throws  IllegalCharsetNameException
             *          If the given name is not a legal charset name
             */
            static void checkName(String const& s);

            static Object& lookup(String const& csn);

            static Object& lookupStandard(String const& csn);

            static Object& lookupExtended(String const& csn);

            static Object& lookupRegistry(String const& csn);
        };
    } // text
} // core

#endif //CORE24_CHARSET_H
