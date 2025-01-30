//
// Created by bruns on 22/03/2024.
//

#ifndef CORE24_CACHE_H
#define CORE24_CACHE_H

#include <core/misc/Unsafe.h>

namespace core {
    namespace misc {
        class Cache final : public Object {
        public:
            class Node;

            CORE_ALIAS(NODE, Class<Node>::Pointer);
            CORE_ALIAS(OBJECT, Class<Object>::Pointer);

            static CORE_FAST gint LOCKED = 1;
            static CORE_FAST gint UNLOCKED = 0;

            enum ObjectType {
                NORMAL,
                THROWABLE,
                THREAD,
            };

            class Node {
            public:
                OBJECT object = {};
                NODE next = {};
                NODE prev = {};
                ObjectType type = NORMAL;

                CORE_EXPLICIT Node(Object &object, NODE prev, NODE next, ObjectType type);

                ~Node();
            };

            NODE first;
            NODE last;
            gint volatile state = 0;
            glong counter = 0;
            gbool duringClearning = false;

        public:
            CORE_IMPLICIT Cache();

            void tryLock();

            void unlock();

            void addAddress(Object &object, ObjectType type = NORMAL);

            gbool containsAddress(Object const &object);

            gbool removeAddress(Object &object);

            void clear();

            ~Cache() override;

            static glong threadCount();
        };

        extern Cache VarHandleCache;
        extern glong statics[8192];
        extern gint statics_size;
        extern glong initialized;
        extern glong threadNumber;
    }
} // core

#endif //CORE24_CACHE_H
