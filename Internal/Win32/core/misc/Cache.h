//
// Created by bruns on 22/03/2024.
//

#ifndef CORE24_CACHE_H
#define CORE24_CACHE_H

#include <core/misc/Unsafe.h>

namespace core {
    namespace misc {
        class Cache final : public Object {
            class Node;

            CORE_ALIAS(NODE, Class<Node>::Pointer);

            static CORE_FAST gint LOCKED = 1;
            static CORE_FAST gint UNLOCKED = 0;

            class Node final : public Object {
            public:
                Object &object;
                NODE next = null, prev = null;

                CORE_EXPLICIT Node(Object &object, NODE prev, NODE next);

                ~Node() override;
            };

            NODE first;
            NODE last;
            gint volatile state = 0;
            glong current = 0;
            glong nbObjects = 0; // For Debugging

        public:
            CORE_IMPLICIT Cache();

            void tryLock();

            void unlock();

            void addAddress(Object &object);

            gbool containsAddress(Object const &object);

            gbool removeAddress(Object &object);

            void clear();

            ~Cache() override;
        };

        extern Cache VarHandleCache;
        extern glong statics[8192];
        extern gint statics_size;
        extern glong initialized;
    }
} // core

#endif //CORE24_CACHE_H
