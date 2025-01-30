//
// Created by bruns on 22/03/2024.
//

#include <unistd.h>
#include <core/misc/Cache.h>

namespace core {
    namespace misc {

        static glong minAddr = 0L;
        static glong maxAddr = 0L;

        Cache::Node::Node(Object& object, NODE prev, NODE next, ObjectType type) {
            Node::next = next;
            Node::prev = prev;
            Node::type = type;
            Node::object = &object;
            if (next)
                next->prev = this;
            if (prev)
                prev->next = this;
            if (type == THREAD) {
                if (threadNumber < 0 || threadNumber > Integer::MAX_VALUE)
                    threadNumber = 0;
                threadNumber++;
            }
        }

        Cache::Node::~Node() {
            if (next)
                next->prev = prev;
            if (prev)
                prev->next = next;
            if (type == NORMAL)
                delete object;
            object = { };
        }

        void Cache::tryLock() {
            gint d = 5;
            while (!Unsafe::weakCompareAndSetIntAcquire(null, CORE_CAST(glong, &state), 0, 1)) {
                sleep(d);
                d += 5;
            }
        }

        void Cache::unlock() {
            gint d = 5;
            while (!Unsafe::weakCompareAndSetIntRelease(null, CORE_CAST(glong, &state), 1, 0)) {
                sleep(d);
                d += 5;
            }
        }

        Cache::Cache() {
            first = { };
            last = { };
            state = UNLOCKED;
            if (initialized == 0x123456789ABCDEFLL)
                initialized = CORE_CAST(glong, 0xFEDCBA9876543210LL);
        }

        void Cache::addAddress(Object& object, ObjectType type) {
            if (initialized != 0x123456789ABCDEF && initialized != CORE_CAST(glong, 0xFEDCBA9876543210LL)) return;
            if (initialized == 0x123456789ABCDEF) {
                if (statics_size >= 8000) return;
                NODE prev = null;
                NODE next = null;
                if (statics_size > 0)
                    next = (NODE) statics[statics_size - 1];
                NODE node = new Node(object, prev, next, type);
                statics[statics_size++] = (glong) node;
                if (statics_size == 1) {
                    minAddr = statics[statics_size - 1];
                    maxAddr = minAddr;
                } else {
                    minAddr = Math::min(minAddr, statics[statics_size - 1]);
                    maxAddr = Math::max(maxAddr, statics[statics_size - 1]);
                }
                return;
            }
            tryLock();
            if (statics_size > 0 && (!first || !last)) {
                first = (NODE) statics[statics_size - 1];
                last = (NODE) statics[0];
                counter = statics_size;
                for (glong i = statics_size - 1; i >= 0; i--) statics[i] = 0;
                statics_size = 0;
            }
            NODE node = new Node(object, null, first, type);
            if (counter == 0) {
                minAddr = (glong) node;
                maxAddr = minAddr;
            } else {
                minAddr = Math::min(minAddr, (glong) node);
                maxAddr = Math::max(maxAddr, (glong) node);
            }
            first = node;
            if (!last) last = node;
            counter++;
            unlock();
        }

        gbool Cache::containsAddress(Object const& object) {
            if (initialized != 0x123456789ABCDEF && initialized != CORE_CAST(glong, 0xFEDCBA9876543210LL))
                return false;
            glong cnt = 0;
            if (initialized == 0x123456789ABCDEF) {
                cnt = statics_size;
                for (glong i = cnt - 1; i >= 0; --i) {
                    NODE node = CORE_CAST(NODE, statics[i]);
                    if (node && node->object == &object)
                        return true;
                    cnt -= 1;
                }
                return false;
            }

            tryLock();
            NODE f = first;
            NODE l = last;
            while (f && l && cnt < counter) {
                if ((glong)f < minAddr || (glong)f > maxAddr)
                    break;
                if (f->object == &object) {
                    unlock();
                    return true;
                }
                if (f == l || f->next == l)
                    break;
                // if (l->object == &object) {
                //     unlock();
                //     return true;
                // }
                f = f->next;
                // l = l->prev;
                cnt += 1;
            }
            unlock();
            return false;
        }

        gbool Cache::removeAddress(Object& object) {
            // 0x123456789ABCDEF = 81985529216486895
            // 0xFEDCBA9876543210 = 18364758544493064720 (unsigned) = -81985529216486896 (signed)
            if (initialized != 0x123456789ABCDEF && initialized != CORE_CAST(glong, 0xFEDCBA9876543210LL))
                return false;
            glong cnt = 0;
            if (initialized == 0x123456789ABCDEF) {
                cnt = statics_size;
                for (glong i = cnt - 1; i >= 0; i--) {
                    NODE node = CORE_CAST(NODE, statics[i]);
                    if (node && node->object == &object) {
                        NODE prev = node->prev;
                        NODE next = node->next;
                        node->prev = node->next = null;
                        if (prev)
                            prev->next = next;
                        if (next)
                            next->prev = prev;
                        if (node == first)
                            first = next;
                        if (node == last)
                            last = prev;
                        delete node;
                        return true;
                    }
                    cnt -= 1;
                }
                return false;
            }
            tryLock();
            NODE f = first;
            NODE l = last;
            while (f && l && cnt < counter) {
                if ((glong)f < minAddr || (glong)f > maxAddr)
                    break;
                if (f->object == &object) {
                    NODE prev = f->prev;
                    NODE next = f->next;
                    f->next = f->prev = null;
                    if (prev)
                        prev->next = next;
                    else
                        first = next;
                    if (next)
                        next->prev = prev;
                    else
                        last = prev;
                    counter--;
                    unlock();
                    delete f;
                    return true;
                }
                if (f == l || f->next == l)
                    break;
                f = f->next;
                cnt += 1;
            }
            unlock();
            return duringClearning;
        }

        void Cache::clear() {
            if (initialized != 0x123456789ABCDEF && initialized != CORE_CAST(glong, 0xFEDCBA9876543210LL)) return;
            if (initialized == 0x123456789ABCDEF) return;

            tryLock();
            NODE f = first;
            glong count = counter;
            first = last = { };
            counter = 0;
            duringClearning = true;
            unlock();
            while (f && count > 0) {
                if ((glong)f < minAddr || (glong)f > maxAddr)
                    break;
                NODE cur = f;
                f = f->next;
                cur->next = { };
                if ((glong)f < minAddr || (glong)f > maxAddr)
                    break;
                if (f)
                    f->prev = { };
                count--;
                if (cur->object)
                    delete cur;
            }

            tryLock();
            duringClearning = false;
            unlock();
        }

        Cache::~Cache() {
            gint oldSize = statics_size;
            clear();
            initialized = 0x123456789ABCDEFLL;
            if (oldSize == statics_size)
                statics_size = 0;
            for (gint i = 0; i < oldSize; ++i)
                statics[i] = 0;
        }

        glong Cache::threadCount() {
            return threadNumber;
        }
    }
} // core
