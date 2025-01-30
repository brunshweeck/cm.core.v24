//
// Created by brunshweeck on 2 sept. 2024.
//

#include <core/IllegalArgumentException.h>
#include <core/charset/CoderResult.h>
#include <core/charset/MalformedInputException.h>
#include <core/charset/UnmappableCharacterException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace charset {
        CoderResult const CoderResult::UNDEFINED = CoderResult(ErrorType::NO_ERROR, 0);

        CoderResult const CoderResult::UNDERFLOW = CoderResult(ErrorType::UNDERFLOW, 0);

        CoderResult const CoderResult::OVERFLOW = CoderResult(ErrorType::UNDERFLOW, 0);

        CoderResult::CoderResult(ErrorType type, gint length): type(type), count(length) {
        }

        CoderResult::CoderResult(CoderResult const &other) CORE_NOTHROW {
            if (&other == &UNDERFLOW && UNDERFLOW.type != ErrorType::UNDERFLOW) {
                type = CORE_CAST(CoderResult&, UNDERFLOW).type = ErrorType::UNDERFLOW;
                count = CORE_CAST(CoderResult&, UNDERFLOW).count = 0;
            } else if (&other == &OVERFLOW && OVERFLOW.type != ErrorType::OVERFLOW) {
                type = CORE_CAST(CoderResult&, OVERFLOW).type = ErrorType::OVERFLOW;
                count = CORE_CAST(CoderResult&, OVERFLOW).count = 0;
            } else if (&other == &UNDEFINED && UNDEFINED.type != ErrorType::NO_ERROR) {
                type = CORE_CAST(CoderResult&, UNDEFINED).type = ErrorType::NO_ERROR;
                count = CORE_CAST(CoderResult&, UNDEFINED).count = 0;
            } else {
                type = other.type;
                count = other.count;
            }
        }

        CoderResult &CoderResult::operator=(CoderResult const &other) CORE_NOTHROW {
            if (&other == &UNDERFLOW && UNDERFLOW.type != ErrorType::UNDERFLOW) {
                type = CORE_CAST(CoderResult&, UNDERFLOW).type = ErrorType::UNDERFLOW;
                count = CORE_CAST(CoderResult&, UNDERFLOW).count = 0;
            } else if (&other == &OVERFLOW && OVERFLOW.type != ErrorType::OVERFLOW) {
                type = CORE_CAST(CoderResult&, OVERFLOW).type = ErrorType::OVERFLOW;
                count = CORE_CAST(CoderResult&, OVERFLOW).count = 0;
            } else if (&other == &UNDEFINED && UNDEFINED.type != ErrorType::NO_ERROR) {
                type = CORE_CAST(CoderResult&, UNDEFINED).type = ErrorType::NO_ERROR;
                count = CORE_CAST(CoderResult&, UNDEFINED).count = 0;
            } else {
                type = other.type;
                count = other.count;
            }
            return *this;
        }

        String CoderResult::toString() const {
            switch (type) {
                case ErrorType::UNDERFLOW: return "UNDERFLOW"_S;
                case ErrorType::OVERFLOW: return "OVERFLOW"_S;
                case ErrorType::MALFORMED: return "MALFORMED"_S;
                case ErrorType::UNMAPPABLE: return "UNMAPPABLE"_S;
                default:
                    CORE_ASSERT(false);
            }
        }

        gbool CoderResult::isUnderflow() const { return type == ErrorType::UNDERFLOW; }

        gbool CoderResult::isOverflow() const { return type == ErrorType::OVERFLOW; }

        gbool CoderResult::isError() const { return type >= ErrorType::ERROR_MIN; }

        gbool CoderResult::isMalformed() const { return type == ErrorType::MALFORMED; }

        gbool CoderResult::isUnmappable() const { return type == ErrorType::UNMAPPABLE; }

        gint CoderResult::length() const { return count; }

        CoderResult CoderResult::malformedForLength(gint length) {
            if (length < 0)
                IllegalArgumentException("Non-positive length"_S).throws($ftrace());
            return CoderResult(ErrorType::MALFORMED, length);
        }

        CoderResult CoderResult::unmappableForLength(gint length) {
            if (length < 0)
                IllegalArgumentException("Non-positive length"_S).throws($ftrace());
            return CoderResult(ErrorType::UNMAPPABLE, length);
        }

        void CoderResult::throwException() const {
            switch (type) {
                case ErrorType::UNDERFLOW:
                    io::BufferUnderflowException().throws($ftrace());
                case ErrorType::OVERFLOW:
                    io::BufferOverflowException().throws($ftrace());
                case ErrorType::MALFORMED:
                    MalformedInputException(count).throws($ftrace());
                case ErrorType::UNMAPPABLE:
                    UnmappableCharacterException(count).throws($ftrace());
                default:
                    CORE_ASSERT(false);
            }
        }

        Object &CoderResult::clone() const {
            try {
                return UNSAFE::newInstance<CoderResult>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }
    } // charset
} // core
