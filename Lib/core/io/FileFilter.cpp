//
// Created by brunshweeck on 15 sept. 2024.
//

#include <core/io/FileFilter.h>

namespace core {
    namespace io {
        gbool FileFilter::accept(File const &pathname) const {
            try {
                return test(pathname);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gbool FileFilter::equals(const Object &obj) const {
            return this == &obj || Predicate::equals(obj);
        }

        Object &FileFilter::clone() const {
            try {
                return UNSAFE::newInstance<FileFilter>(*this);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        gbool FileFilter::test(File const &arg) const CORE_NOTHROW {
            return Predicate::test(arg);
        }
    } // io
} // core
