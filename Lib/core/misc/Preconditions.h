//
// Created by bruns on 12/05/2024.
//

#ifndef CORE24_PRECONDITION_H
#define CORE24_PRECONDITION_H

#include <core/lang/Object.h>

namespace core {
    namespace misc {
        /**
         * The class @c Precondition contains utility methods
         * to check if state or arguments are correct.
         */
        class Preconditions final : public Object {
            Preconditions();

        public:
            static gint checkIndex(gint index, gint length);

            static gint checkIndexFromRange(gint start, gint end, gint length);

            static gint checkIndexFromSize(gint start, gint size, gint length);

            static gint checkIndexForAdding(gint index, gint length);
        };
    } // misc
} // core

#endif // CORE24_PRECONDITION_H
