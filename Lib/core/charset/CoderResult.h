//
// Created by brunshweeck on 2 sept. 2024.
//

#ifndef CORE24_CODERRESULT_H
#define CORE24_CODERRESULT_H

#include <core/lang/Object.h>

#undef UNDERFLOW
#undef OVERFLOW

namespace core {
  namespace charset {
    /**
     * A description of the result state of a coder.
     *
     * <p>
     * A charset coder, that is, either a decoder or an encoder, consumes bytes
     * (or characters) from an input buffer, translates them, and writes the
     * resulting characters (or bytes) to an output buffer.  A coding process
     * terminates for one of four categories of reasons, which are described by
     * instances of this class: <br/>
     *
     *   -  <i>Underflow</i> is reported when there is no more input to be
     *   processed, or there is insufficient input and additional input is
     *   required.  This condition is represented by the unique result object
     *   @b UNDERFLOW, whose @b isUnderflow method returns @c true. <br/>
     *
     *   -  <i>Overflow</i> is reported when there is insufficient room
     *   remaining in the output buffer.  This condition is represented by the
     *   unique result object @b OVERFLOW, whose @b isOverflow method returns @c true. <br/>
     *
     *   -  A <i>malformed-input error</i> is reported when a sequence of
     *   input units is not well-formed.  Such errors are described by instances of
     *   this class whose @b isMalformed method returns
     *   @c true and whose @b length method returns the length
     *   of the malformed sequence.  There is one unique instance of this class for
     *   all malformed-input errors of a given length. <br/>
     *
     *   -  An <i>unmappable-character error</i> is reported when a sequence
     *   of input units denotes a character that cannot be represented in the
     *   output charset.  Such errors are described by instances of this class
     *   whose @b isUnmappable method returns @c true and
     *   whose @b length method returns the length of the input
     *   sequence denoting the unmappable character.  There is one unique instance
     *   of this class for all unmappable-character errors of a given length.
     *
     * </p>
     * <p>
     * For convenience, the @b isError method returns @c true
     * for result objects that describe malformed-input and unmappable-character
     * errors but @c false for those that describe underflow or overflow
     * conditions.
     * </p>
     */
    class CoderResult final : public Object {
      enum class ErrorType {
        NO_ERROR,

        UNDERFLOW  = 1 << 0,
        OVERFLOW   = 1 << 1,
        MALFORMED  = 1 << 2,
        UNMAPPABLE = 1 << 3,

        ERROR_MIN = MALFORMED,
      };

      ErrorType type;
      gint count;

      CORE_EXPLICIT CoderResult(ErrorType type, gint length);

    public:
      CoderResult(CoderResult const& other) CORE_NOTHROW;

      CoderResult& operator=(CoderResult const& other) CORE_NOTHROW;

      /**
       * Returns a string describing this coder result.
       *
       * @return  A descriptive string
       */
      String toString() const override;

      /**
       * Tells whether this object describes an underflow condition.
       *
       * @return  @c true if, and only if, this object denotes underflow
       */
      gbool isUnderflow() const;

      /**
       * Tells whether this object describes an overflow condition.
       *
       * @return  @c true if, and only if, this object denotes overflow
       */
      gbool isOverflow() const;

      /**
       * Tells whether this object describes an error condition.
       *
       * @return  @c true if, and only if, this object denotes either a
       *          malformed-input error or an unmappable-character error
       */
      gbool isError() const;

      /**
       * Tells whether this object describes an error condition.
       *
       * @return  @c true if, and only if, this object denotes either a
       *          malformed-input error or an unmappable-character error
       */
      gbool isMalformed() const;

      /**
       * Tells whether this object describes an unmappable-character
       * error.
       *
       * @return  @c true if, and only if, this object denotes an
       *          unmappable-character error
       */
      gbool isUnmappable() const;

      /**
       * Returns the length of the erroneous input described by this
       * object&nbsp;&nbsp;<i>(optional operation)</i>.
       *
       * @return  The length of the erroneous input, a positive integer
       *
       * @throws  UnsupportedOperationException
       *          If this object does not describe an error condition, that is,
       *          if the @b isError does not return @c true
       */
      gint length() const;

      /**
       * Special value
       */
      static CoderResult const UNDEFINED;

      /**
       * Result object indicating underflow, meaning that either the input buffer
       * has been completely consumed or, if the input buffer is not yet empty,
       * that additional input is required.
       */
      static CoderResult const UNDERFLOW;

      /**
       * Result object indicating overflow, meaning that there is insufficient
       * room in the output buffer.
       */
      static CoderResult const OVERFLOW;

      /**
       * Static factory method that returns the unique object describing a
       * malformed-input error of the given length.
       *
       * @param   length
       *          The given length
       *
       * @return  The requested coder-result object
       */
      static CoderResult malformedForLength(gint length);

      /**
       * Static factory method that returns the unique result object describing
       * an unmappable-character error of the given length.
       *
       * @param   length
       *          The given length
       *
       * @return  The requested coder-result object
       */
      static CoderResult unmappableForLength(gint length);

      /**
       * Throws an exception appropriate to the result described by this object.
       *
       * @throws  BufferUnderflowException
       *          If this object is @b UNDERFLOW
       *
       * @throws  BufferOverflowException
       *          If this object is @b OVERFLOW
       *
       * @throws  MalformedInputException
       *          If this object represents a malformed-input error; the
       *          exception's length value will be that of this object
       *
       * @throws  UnmappableCharacterException
       *          If this object represents an unmappable-character error; the
       *          exception's length value will be that of this object
       *
       * @throws  CharacterCodingException
       *          @c MalformedInputException if this object represents a
       *          malformed-input error; @c UnmappableCharacterException
       *          if this object represents an unmappable-character error
       */
      void throwException() const;

      Object& clone() const override;
    };
  } // charset
} // core

#endif //CORE24_CODERRESULT_H
