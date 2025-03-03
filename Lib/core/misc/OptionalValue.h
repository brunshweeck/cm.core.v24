//
// Created by brunshweeck on 22/11/24.
//

#ifndef CORE24_OPTIONALVALUE_H
#define CORE24_OPTIONALVALUE_H

#include <core/lang/Class.h>

namespace core {
    namespace misc {

        class OptionalValue : public Object {
        protected:
            CORE_FAST OptionalValue() = default;

        private:
            virtual Object &getContent() = 0;

            virtual Object const &getContent() const = 0;

            virtual gbool hasContent() const = 0;

        public:
            gbool equals(const Object &o) const override;

            gint hash() const override;
        };

    } // misc
} // core

#endif //CORE24_OPTIONALVALUE_H
