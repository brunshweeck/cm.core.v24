//
// Created by bruns on 13/10/2024.
//

#ifndef CORE24_FLUSHABLE_H
#define CORE24_FLUSHABLE_H

#include <core/io/IOException.h>

namespace core {
    namespace io {

        /**
         * A @c Flushable is a destination of data that can be flushed.  The
         * flush method is invoked to write any buffered output to the underlying
         * stream.
         */
        class Flushable: public virtual Object {
        public:

            /**
             * Flushes this stream by writing any buffered output to the underlying
             * stream.
             *
             * @throws IOException If an I/O error occurs
             */
            virtual void flush() = 0;
        };
    } // io
} // core

#endif //CORE24_FLUSHABLE_H
