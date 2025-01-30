//
// Created by bruns on 12/05/2024.
//

#include "Preconditions.h"

#include <core/IndexOutOfBoundsException.h>
#include <core/UnsupportedOperationException.h>

namespace core {
    namespace misc {
        Preconditions::Preconditions() {
            UnsupportedOperationException().throws($ftrace());
        }

        gint Preconditions::checkIndex(gint index, gint length) {
            if ((index | length) < 0 || index >= length) {
                IndexOutOfBoundsException("Index "_S + index
                    + " out of bounds for length "_S + length).throws($ftrace());
            }
            return index;
        }

        gint Preconditions::checkIndexFromRange(gint start, gint end, gint length) {
            if ((start | end | length) < 0 || end < start || length < end) {
                IndexOutOfBoundsException("Range ["_S + start + ", "_S + end
                    + ") out of bounds for length "_S + length).throws($ftrace());
            }
            return start;
        }

        gint Preconditions::checkIndexFromSize(gint start, gint size, gint length) {
            if ((start | size | length) < 0 || length < size + start) {
                IndexOutOfBoundsException("Range ["_S + start + ", " + start + "+" + size
                    + ") out of bounds for length " + length).throws($ftrace());
            }
            return start;
        }

        gint Preconditions::checkIndexForAdding(gint index, gint length) {
            if (index < 0 || index > length) {
                IndexOutOfBoundsException("Index "_S + index + " out of adding bounds for length "_S
                    + length).throws($ftrace());
            }
            return index;
        }
    } // misc
} // core
