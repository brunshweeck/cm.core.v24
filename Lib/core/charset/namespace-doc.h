//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {
    /**
     * Defines charsets, decoders, and encoders, for translating between
     * bytes and Unicode characters.
     * - <b> Class name </b> | <b> Description </b> <br>
     * - @c charset::Charset | A named mapping between characters and bytes <br>
     * - @c charset::CharsetDecoder | Decodes bytes into characters <br>
     * - @c charset::CharsetEncoder | Encodes characters into bytes <br>
     * - @c charset::CoderResult | Describes coder results <br>
     * - @c charset::Charset::CodingErrorAction | Describes actions to take when coding errors are detected <br>
     *
     * <p>
     * A <i>charset</i> is named mapping between sequences of
     * sixteen-bit Unicode characters and sequences of bytes, in the sense
     * defined in <a href="http://www.ietf.org/rfc/rfc2278.txt">RFC 2278</a>.
     * A <i>decoder</i> is an engine which transforms bytes in a specific
     * charset into characters, and an <i>encoder</i> is an engine which
     * transforms characters into bytes.  Encoders and decoders operate on
     * byte and character buffers.  They are collectively referred to as
     * <i>coders</i>.
     * </p>
     * <p>
     * The @b charset::Charset class defines methods for
     * creating coders for a given charset and for retrieving the various
     * names associated with a charset.  It also defines static methods
     * for testing whether a particular charset is supported, for locating
     * charset instances by name, and for constructing a map that contains
     * every charset for which support is available in the current Java
     * virtual machine.
     * </p>
     * <p>
     * Most users will not use these classes directly; instead they
     * will use the existing charset-related constructors and methods in
     * the @b core::String class, together with the existing
     * @b core::io::InputStreamReader and @b core::io::OutputStreamWriter classes,
     * all of whose implementations
     * have been reworked to make use of the charset facilities defined in
     * this package.  A small number of changes have been made to the
     * @b core::io::InputStreamReader and @b core::io::OutputStreamWriter classes in order to allow explicit
     * charset objects to be specified in the construction of instances of
     * those classes.
     * </p>
     *
     *
     * @see https://www.rfc-editor.org/info/rfc2278 RFC 2278: IANA Charset Registration Procedures
     */
    namespace charset {}
}

#endif //NAMESPACE_DOC_H
