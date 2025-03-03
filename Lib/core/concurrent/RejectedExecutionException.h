//
// Created by bruns on 01/10/2024.
//

#ifndef CORE24_REJECTEDEXECUTIONEXCEPTION_H
#define CORE24_REJECTEDEXECUTIONEXCEPTION_H
#include <core/lang/RuntimeException.h>

namespace core {
  namespace concurrent {
    /**
     * Exception thrown by an @b Executor when a task cannot be
     * accepted for execution.
     */
    class RejectedExecutionException : public virtual RuntimeException {
    public:
      /**
       * Constructs a @c RejectedExecutionException with no detail message.
       */
      CORE_IMPLICIT RejectedExecutionException() = default;

      /**
       * Constructs a @c RejectedExecutionException with the
       * specified detail message.
       *
       * @param message the detail message
       */
      CORE_EXPLICIT RejectedExecutionException(String const& message);

      /**
       * Constructs a @c RejectedExecutionException with the
       * specified detail message and cause.
       *
       * @param  message the detail message
       * @param  cause the cause (which is saved for later retrieval by the
       *         @b cause() method)
       */
      CORE_EXPLICIT RejectedExecutionException(String const& message, Throwable const& cause);

      /**
       * Constructs a @c RejectedExecutionException with the
       * specified cause.  The detail message is set to @c cause.toString()
       * (which typically contains the class and detail message of @c cause).
       *
       * @param  cause the cause (which is saved for later retrieval by the
       *         @b cause() method)
       */
      CORE_EXPLICIT RejectedExecutionException(Throwable const& cause);

      Object& clone() const override;

    protected:
      void selfThrow() const override;
    };
  } // concurrent
} // core

#endif //CORE24_REJECTEDEXECUTIONEXCEPTION_H
