//
// Created by brunshweeck on 11 sept. 2024.
//

#ifndef CORE24_MISSINGRESOURCEEXCEPTION_H
#define CORE24_MISSINGRESOURCEEXCEPTION_H

#include <core/lang/RuntimeException.h>

namespace core {
    namespace util {
        /**
         * Signals that a resource is missing.
         *
         * @see core::Exception
         */
        class MissingResourceException : public virtual RuntimeException {
            /**
             * The class name of the resource bundle requested by the user.
             */
            String resourceClass;

            /**
             * The name of the specific resource requested by the user.
             */
            String resourceKey;

        public:
            /**
             * Constructs a MissingResourceException with the specified information.
             * A detail message is a String that describes this particular exception.
             * @param message the detail message
             * @param className the name of the resource class
             * @param key the key for the missing resource.
             */
            CORE_EXPLICIT MissingResourceException(String const &message, String className, String key);

            /**
             * Gets parameter passed by constructor.
             *
             * @return the name of the resource class
             */
            String className() const;

            /**
             * Gets parameter passed by constructor.
             *
             * @return the key for the missing resource
             */
            String key() const;

            Object &clone() const override;

        protected:
            void selfThrow() const override;

        private:
            /**
             * Constructs a @c MissingResourceException with
             * @c message, @c className, @c key,
             * and @c cause.
             *
             * @param message
             *        the detail message
             * @param className
             *        the name of the resource class
             * @param key
             *        the key for the missing resource.
             * @param cause
             *        the cause (which is saved for later retrieval by the
             *        @b Throwable::cause() method).
             */
            CORE_EXPLICIT MissingResourceException(String const &message, String className, String key,
                                                   Throwable const &cause);
        };
    } // util
} // core

#endif //CORE24_MISSINGRESOURCEEXCEPTION_H
