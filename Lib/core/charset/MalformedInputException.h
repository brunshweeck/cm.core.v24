//
// Created by brunshweeck on 1 sept. 2024.
//

#ifndef CORE24_MALFORMEDINPUTEXCEPTION_H
#define CORE24_MALFORMEDINPUTEXCEPTION_H
#include <core/charset/CharacterCodingException.h>

namespace core {
  namespace charset {
    /**
     * Checked exception thrown when an input byte sequence is not legal for given
     * charset, or an input character sequence is not a legal sixteen-bit Unicode
     * sequence.
     */
    class MalformedInputException final : public virtual CharacterCodingException {
      /**
       * The length of the input.
       */
      gint inputLength;

    public:
      /**
       * Constructs an @c MalformedInputException with the given
       * length.
       * @param inputLength the length of the input
       */
      CORE_EXPLICIT MalformedInputException(gint inputLength);

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

#endif //CORE24_MALFORMEDINPUTEXCEPTION_H
