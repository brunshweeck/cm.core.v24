//
// Created by brunshweeck on 1 sept. 2024.
//

#ifndef CORE24_ILLEGALCHARSETNAMEEXCEPTION_H
#define CORE24_ILLEGALCHARSETNAMEEXCEPTION_H

#include <core/lang/IllegalArgumentException.h>

namespace core {
  namespace charset {
    /**
     * Unchecked exception thrown when a string that is not a
     * <i>legal charset name</i> is used as such.
     */
    class IllegalCharsetNameException : public virtual IllegalArgumentException {
      /**
       * The illegal charset name.
       */
      String illegalCharsetName;

    public:
      /**
       * Constructs an instance of this class.
       *
       * @param  charsetName
       *         The illegal charset name
       */
      CORE_EXPLICIT IllegalCharsetNameException(String charsetName);

      /**
       * Retrieves the illegal charset name.
       *
       * @return  The illegal charset name
       */
      String charsetName() const;

      Object& clone() const override;

    protected:
      void selfThrow() const override;
    };
  } // io
} // core

#endif //CORE24_ILLEGALCHARSETNAMEEXCEPTION_H
