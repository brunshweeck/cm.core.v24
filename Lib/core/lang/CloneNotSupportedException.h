//
// Created by brunshweeck on 8 juin 2024.
//

#ifndef CORE24_CLONENOTSUPPORTEDEXCEPTION_H
#define CORE24_CLONENOTSUPPORTEDEXCEPTION_H

#include <core/lang/RuntimeException.h>

namespace core {
  /**
   * Thrown to indicate that the @c clone method in class
   * @c Object has been called to clone an object, but that
   * the object's class does not support this operation.
   *
   * <p>
   * Applications that override the @c clone method can also
   * throw this exception to indicate that an object could not or
   * should not be cloned.
   */
  class CloneNotSupportedException final : public virtual RuntimeException {
  public:
    /**
     * Constructs a @c CloneNotSupportedException with no
     * detail message.
     */
    CORE_IMPLICIT CloneNotSupportedException() = default;

    /**
     * Constructs a @c CloneNotSupportedException with the
     * specified detail message.
     *
     * @param message the detail message.
     */
    CORE_EXPLICIT CloneNotSupportedException(String const& message) CORE_NOTHROW;

    Object& clone() const override;

  protected:
    void selfThrow() const override;
  };
} // core

#endif //CORE24_CLONENOTSUPPORTEDEXCEPTION_H
