//
// Created by bruns on 22/03/2024.
//

#include <core/misc/Cache.h>
#include <Windows.h>

namespace core {
    namespace misc {
        Cache::Node::Node(Object &object, NODE prev, NODE next) : object(object), next(next), prev(prev) {
            if (next != null)
                next->prev = this;
            if (prev != null)
                prev->next = this;
        }

        Cache::Node::~Node() {
            delete &object;
        }

        void Cache::tryLock() {
            HANDLE handle = CreateEvent(null, FALSE, FALSE, null);
            DWORD d = 5;
            while (!Unsafe::weakCompareAndSetIntAcquire(null, CORE_CAST(glong, &state), 0, 1))
            // #ifdef CORE_COMPILER_MSVC
            //             while (InterlockedCompareExchange(CORE_CAST(volatile LONG*, &state), 1, 0) != 1)
            // #else
            //             LONG i = 0;
            //             while (__atomic_compare_exchange_n(CORE_CAST(volatile LONG*, &state), &i, 1, false, 0, 0))
            // #endif
            {
                WaitForSingleObject(handle, d);
                d += 5;
            }

            current = CORE_CAST(glong, handle);
        }

        void Cache::unlock() {
            HANDLE handle = CORE_CAST(HANDLE, current);
            DWORD d = 5;
            while (!Unsafe::weakCompareAndSetIntRelease(null, CORE_CAST(glong, &state), 1, 0)) {
                WaitForSingleObject(handle, d);
                d += 5;
            }
            CloseHandle(handle);
        }

        Cache::Cache() {
            first = null;
            last = null;
            current = 0;
            state = UNLOCKED;
            if (initialized == 0x123456789ABCDEFLL) {
                // statics[statics_size++] = CORE_CAST(glong, this);
                initialized = CORE_CAST(glong, 0xFEDCBA9876543210LL);
            }
        }

        void Cache::addAddress(Object &object) {
            if (initialized != 0x123456789ABCDEF && initialized != CORE_CAST(glong, 0xFEDCBA9876543210LL))
                return;
            if (initialized == 0x123456789ABCDEF) {
                if (statics_size >= 8000)
                    return;
                if (statics_size > 0)
                    statics[statics_size] = CORE_CAST(
                        glong, new Node(object, null, CORE_CAST(NODE, statics[statics_size-1])));
                else
                    statics[statics_size] = CORE_CAST(glong, new Node(object, null, null));
                statics_size += 1;
                return;
            }
            tryLock();
            if (statics_size > 0 && (first == null || last == null)) {
                first = CORE_CAST(NODE, statics[statics_size-1]);
                last = CORE_CAST(NODE, statics[0]);
                nbObjects = statics_size;
            }
            const NODE node = new Node(object, null, first);
            first = node;
            if (last == null)
                last = node;
            nbObjects++;
            unlock();
        }

        gbool Cache::containsAddress(Object const &object) {
            if (initialized != 0x123456789ABCDEF && initialized != CORE_CAST(glong, 0xFEDCBA9876543210LL))
                return false;
            if (statics_size > 0) {
                for (int i = 0; i < statics_size; ++i) {
                    NODE node = CORE_CAST(NODE, statics[i]);
                    if (node != null && &node->object == &object)
                        return true;
                }
                if (initialized == 0x123456789ABCDEF)
                    return false;
            }

            tryLock();
            NODE f = first;
            NODE l = last;
            while (f != null && l != null) {
                if (&f->object == &object) {
                    unlock();
                    return true;
                }
                if (f == l)
                    break;
                if (&l->object == &object) {
                    unlock();
                    return true;
                }
                f = f->next;
                l = l->prev;
            }
            unlock();
            return false;
        }

        gbool Cache::removeAddress(Object &object) {
            // 0x123456789ABCDEF = 81985529216486895
            // 0xFEDCBA9876543210 = 18364758544493064720 (unsigned) = -81985529216486896 (signed)
            if (initialized != 0x123456789ABCDEF && initialized != CORE_CAST(glong, 0xFEDCBA9876543210LL))
                return false;
            if (initialized == 0x123456789ABCDEF) {
                for (int i = 0; i < statics_size; ++i) {
                    NODE node = CORE_CAST(NODE, statics[i]);
                    if (node != null && &node->object == &object) {
                        NODE prev = node->prev;
                        NODE next = node->next;
                        node->prev = node->next = null;
                        if (prev != null)
                            prev->next = next;
                        if (next != null)
                            next->prev = prev;
                        if (node == first)
                            first = next;
                        if (node == last)
                            last = prev;
                        delete node;
                        return true;
                    }
                }
                if (initialized == 0x123456789ABCDEF)
                    return false;
            }
            tryLock();
            NODE f = first;
            NODE l = last;
            while (f != null && l != null) {
                if (&f->object == &object) {
                    NODE prev = f->prev;
                    NODE next = f->next;
                    f->next = f->prev = null;
                    if (prev != null)
                        prev->next = next;
                    else
                        first = next;
                    if (next != null)
                        next->prev = prev;
                    else
                        last = prev;
                    unlock();
                    delete f;
                    nbObjects--;
                    return true;
                }
                if (f == l)
                    break;
                if (&l->object == &object) {
                    NODE prev = l->prev;
                    NODE next = l->next;
                    l->next = l->prev = null;
                    if (prev != null)
                        prev->next = next;
                    else
                        first = next;
                    if (next != null)
                        next->prev = prev;
                    else
                        last = prev;
                    unlock();
                    delete l;
                    return true;
                }
                f = f->next;
                l = l->prev;
            }
            unlock();
            return false;
        }

        void Cache::clear() {
            if (initialized != 0x123456789ABCDEF && initialized != CORE_CAST(glong, 0xFEDCBA9876543210LL))
                return;
            if (initialized == 0x123456789ABCDEF)
                return;

            tryLock();
            NODE f = first;
            first = last = null;
            glong nb = nbObjects;
            nbObjects = 0;
            unlock();
            while (f != null) {
                NODE cur = f;
                f = f->next;
                cur->next = null;
                if (f != null)
                    f->prev = null;
                delete cur;
                nb--;
            }
        }

        Cache::~Cache() {
            gint oldSize = statics_size;
            clear();
            initialized = 0x123456789ABCDEFLL;
            if (oldSize == statics_size)
                statics_size = 0;
            FillMemory(statics, oldSize * sizeof(glong), 0);
        }
    }
} // core
