//
// Created by brunshweeck on 27 août 2024.
//

#ifndef CORE24_APPENDABLE_H
#define CORE24_APPENDABLE_H

#include <core/Object.h>

namespace core {
    /**
     * An object to which @c gchar sequences and values can be appended.  The
     * @c Appendable interface must be implemented by any class whose
     * instances are intended to receive formatted output from a
     * @b util::Formatter.
     *
     * <p> The characters to be appended should be valid Unicode characters as
     * described in <a href="Character.html#unicode">Unicode Character
     * Representation</a>.  Note that supplementary characters may be composed of
     * multiple 16-bit @c gchar values.
     *
     * <p> Appendables are not necessarily safe for multithreaded access.  Thread
     * safety is the responsibility of classes that extend and implement this
     * interface.
     *
     * <p> Since this interface may be implemented by existing classes
     * with different styles of error handling there is no guarantee that
     * errors will be propagated to the invoker.
     */
    class Appendable: public virtual Object {
    public:

        /**
         * Appends the specified character sequence to this @c Appendable.
         *
         * <p> Depending on which class implements the character sequence
         * @c csq, the entire sequence may not be appended.  For
         * instance, if @c csq is an @b io::CharBuffer then
         * the subsequence to append is defined by the buffer's position and limit.
         * <p>
         * The contents of this @c Appendable are unspecified if the @c CharSequence
         * is modified during the method call or an exception is thrown
         * when accessing the @c CharSequence.
         *
         * @param  csq
         *         The character sequence to append.
         *
         * @return  A reference to this @c Appendable
         *
         * @throws  IOException
         *          If an I/O error occurs
         */
        virtual Appendable &append(CharSequence const& csq) = 0;

        /**
         * Appends a subsequence of the specified character sequence to this
         * @c Appendable.
         *
         * <p> An invocation of this method of the form @code out.append(csq, start, end) @endcode
         * when @c csq is not @c null, behaves in exactly the same way as the invocation
         *
         * <pre>
         *     out.append(csq.subSequence(start, end)) </pre>
         *
         * <p>
         * The contents of this @c Appendable are unspecified if the @c CharSequence
         * is modified during the method call or an exception is thrown
         * when accessing the @c CharSequence.
         * @param  csq
         *         The character sequence from which a subsequence will be
         *         appended.
         *
         * @param  start
         *         The index of the first character in the subsequence
         *
         * @param  end
         *         The index of the character following the last character in the
         *         subsequence
         *
         * @return  A reference to this @c Appendable
         *
         * @throws  IndexOutOfBoundsException
         *          If @c start or @c end are negative, @c start
         *          is greater than @c end, or @c end is greater than
         *          @c csq.length()
         *
         * @throws  IOException
         *          If an I/O error occurs
         */
        virtual Appendable &append(CharSequence const& csq, gint start, gint end) = 0;

        /**
         * Appends the specified character to this @c Appendable.
         *
         * @param  c
         *         The character to append
         *
         * @return  A reference to this @c Appendable
         *
         * @throws  IOException
         *          If an I/O error occurs
         */
        virtual Appendable &append(gchar c) = 0;
    };
} // core

#endif //CORE24_APPENDABLE_H
