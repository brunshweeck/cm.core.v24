//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {

    /**
     * Classes for matching character sequences against patterns specified
     * by regular expressions.
     *
     * <p>
     * An instance of the @b regex::Pattern class
     * represents a regular expression that is specified in string form in
     * a syntax similar to that used by Perl.
     * </p>
     * <p>
     * Instances of the @b regex::Matcher class are used
     * to match character sequences against a given pattern.  Input is
     * provided to matchers via the @b core::CharSequence
     * interface in order to support matching against characters from a
     * wide variety of input sources.
     * </p>
     * <p>
     * Unless otherwise noted, passing a @c null argument to a
     * method in any class or interface in this package will cause a
     * @b IllegalArgumentException to be thrown.
     * </p>
     */
    namespace regex {}
}

#endif //NAMESPACE_DOC_H
