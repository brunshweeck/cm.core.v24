//
// Created by admin on 07/01/25.
//

#ifndef CORE24_ATOMICMOVENOTSUPPORTEDEXCEPTION_H
#define CORE24_ATOMICMOVENOTSUPPORTEDEXCEPTION_H

#include "FileSystemException.h"

namespace core {
    namespace io {

        /**
         * Checked exception thrown when a file cannot be moved as an atomic file system
         * operation.
         */
        class AtomicMoveNotSupportedException: public virtual FileSystemException {
        public:

            /**
             * Constructs an instance of this class.
             *
             * @param   source
             *          a string identifying the source file or {@code null} if not known
             * @param   target
             *          a string identifying the target file or {@code null} if not known
             * @param   reason
             *          a reason message with additional information or {@code null}
             */
            CORE_EXPLICIT AtomicMoveNotSupportedException(const String &file,
                                                          const String &other,
                                                          const String &reason);

            Object &clone() const override;

        protected:
            void selfThrow() const override;
        };

    } // io
} // core

#endif //CORE24_ATOMICMOVENOTSUPPORTEDEXCEPTION_H
