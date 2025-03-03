//
// Created by brunshweeck on 1 sept. 2024.
//

#ifndef CORE24_UNMAPPABLECHARACTEREXCEPTION_H
#define CORE24_UNMAPPABLECHARACTEREXCEPTION_H

#include <core/charset/CharacterCodingException.h>

namespace core {
  namespace charset {
    /**
     * Checked exception thrown when an input character (or byte) sequence
     * is valid but cannot be mapped to an output byte (or character)
     * sequence.
     */
    class UnmappableCharacterException final : public virtual CharacterCodingException {
      /**
       * The length of the input.
       */
      gint inputLength;

    public:
      /**
       * Constructs an @c UnmappableCharacterException with the given
       * length.
       * @param inputLength the length of the input
       */
      CORE_EXPLICIT UnmappableCharacterException(gint inputLength);

      /**
       * Returns the length of the input.
       * @return the length of the input
       */
      gint length() const;

      Object& clone() const override;

    protected:
      void selfThrow() const override;
    };
  } // charset
} // core

#endif //CORE24_UNMAPPABLECHARACTEREXCEPTION_H
