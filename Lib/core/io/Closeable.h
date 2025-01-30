//
// Created by bruns on 12/10/2024.
//

#ifndef CORE24_CLOSEABLE_H
#define CORE24_CLOSEABLE_H

#include <core/AutoCloseable.h>

namespace core {
    namespace io {
        /**
         * A @c Closeable is a source or destination of data that can be closed.
         * The close method is invoked to release resources that the object is
         * holding (such as open files).
         */
        class Closeable : public AutoCloseable {
        public:
            /**
             * Closes this stream and releases any system resources associated
             * with it. If the stream is already closed then invoking this
             * method has no effect.
             *
             * <p>
             * As noted in @b AutoCloseable::close(), cases where the
             * close may fail require careful attention. It is strongly advised
             * to relinquish the underlying resources and to internally
             * <em>mark</em> the @c Closeable as closed, prior to throwing
             * the @c IOException.
             * </p>
             * @throws IOException if an I/O error occurs
             */
            void close() override = 0;
        };
    } // io
} // core

#endif //CORE24_CLOSEABLE_H
