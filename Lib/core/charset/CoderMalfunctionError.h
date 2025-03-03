//
// Created by brunshweeck on 1 sept. 2024.
//

#ifndef CORE24_CODERMALFUNCTIONERROR_H
#define CORE24_CODERMALFUNCTIONERROR_H

#include <core/lang/Error.h>

namespace core {
  namespace charset {
    /**
     * Error thrown when the @b decodeLoop method of
     * a @b CharsetDecoder, or the @b encodeLoop method of a
     * @b CharsetEncoder, throws an unexpected exception.
     */
    class CoderMalfunctionError : public virtual Error {
    public:
      /**
       * Initializes an instance of this class.
       *
       * @param  cause
       *         The unexpected exception that was thrown
       */
      CORE_EXPLICIT CoderMalfunctionError(Exception const& cause);

      Object& clone() const override;

    protected:
      void selfThrow() const override;
    };
  } // charset
} // core

#endif //CORE24_CODERMALFUNCTIONERROR_H
