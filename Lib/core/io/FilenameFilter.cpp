//
// Created by brunshweeck on 15 sept. 2024.
//

#include <core/io/FilenameFilter.h>

namespace core {
    namespace io {
        gbool FilenameFilter::accept(File const &dir, String const &name) const {
            try {
                return test(dir, name);
            }catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool FilenameFilter::equals(const Object &obj) const {
            return this == &obj || Class<FilenameFilter>::hasInstance(obj) && BiPredicate::equals(obj);
        }

        Object & FilenameFilter::clone() const {
            try {
                return UNSAFE::newInstance<FilenameFilter>(*this);
            }catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool FilenameFilter::test(File const &a, String const &b) const noexcept {
            return BiPredicate::test(a, b);
        }
    } // io
} // core
