//
// Created by bruns on 09/05/2024.
//

#ifndef CORE24_NIL_H
#define CORE24_NIL_H

#include <core/Object.h>

namespace core {
    namespace misc {
        /**
         * The class @c NIL define the null pointer address.
         */
        class NIL final : public Object {
            /**
                * Construct new NIL instance
                */
            CORE_IMPLICIT CORE_FAST NIL() = default;

            /**
             * NIL value is not copyable
             */
            NIL(NIL const &) = default;

        public:
            /**
             * The single instance of this class
             */
            static NIL INSTANCE;

            /**
             * Return null pointer
             * @tparam T The type of pointer target type
             */
            template<class T>
            CORE_IMPLICIT CORE_FAST operator T *() const { return CORE_CAST(T*, 0); }

            template<class T>
            friend CORE_FAST gbool operator==(T *p, const NIL &) { return !p; }

            template<class T>
            friend CORE_FAST gbool operator!=(T *p, const NIL &) { return p; }

            template<class T>
            friend CORE_FAST gbool operator==(NIL const &, T *p) { return !p; }

            template<class T>
            friend CORE_FAST gbool operator!=(NIL const &, T *p) { return p; }
        };
    } // misc

    extern misc::NIL &null;
} // core


#endif // CORE24_NIL_H
