//
// Created by bruns on 22/03/2024.
//

#undef BIG_ENDIAN

#include "MemoryHeap.h"

#include <native/System.h>
#include <core/lang/IllegalArgumentException.h>

namespace core {
    CORE_ALIAS(BYTES, Class<gbyte>::Pointer);
    CORE_ALIAS(LONGS, Class<glong>::Pointer);
    CORE_ALIAS(INTS, Class<gint>::Pointer);
    CORE_ALIAS(SHORTS, Class<gshort>::Pointer);

    namespace misc {
        MemoryHeap::MemoryHeap() = default;

        glong MemoryHeap::alignToHeapWordSize(glong bytes) {
            if (bytes >= 0)
                return bytes + ADDRESS_SIZE - 1 & ~(ADDRESS_SIZE - 1);

            return -1;
        }

        glong MemoryHeap::allocateMemory(glong sizeInBytes) {
            size_t size = alignToHeapWordSize(sizeInBytes);
            // if (true) {
            //     // Check if size is valid, and resolve if it isn't.
            //     size_t s = ADDRESS_SIZE;
            //     while (s < size) s <<= 1;
            //     size = s;
            // }
            BYTES bytes = null;
            try {
                // bytes = new gbyte[size];
                // bytes = (BYTES) malloc(size);
                bytes = (BYTES) Malloc(size, UNSAFE::ADDRESS_SIZE);
                if (bytes == null)
                    size = 0U;
            } catch (...) { size = 0U; }
            size_t j = 0;
            if (size - j >= 8) {
                LONGS longs = (LONGS) bytes;
                for (size_t i = j; j < size; i++, j += 8) longs[i] = 0L;
            }
            if (size - j >= 4) {
                INTS ints = (INTS) bytes;
                for (size_t i = j << 1; j < size; i++, j += 4) ints[i] = 0L;
            }
            if (size - j >= 2) {
                SHORTS shorts = (SHORTS) bytes;
                for (size_t i = j << 2; j < size; i++, j += 2) shorts[i] = 0L;
            }
            for (; j < size; j++) bytes[j] = 0L;
            return CORE_CAST(glong, bytes);
        }

        glong MemoryHeap::reallocateMemory(glong addr, glong sizeInBytes) {
            size_t size = alignToHeapWordSize(sizeInBytes);
            // if (true) {
            //     // Check if size is valid, and resolve if it isn't.
            //     size_t s = ADDRESS_SIZE;
            //     while (s < size) s <<= 1;
            //     size = s;
            // }
            BYTES oldBytes = (BYTES) addr;
            BYTES bytes = null;
            try {
                // bytes = new gbyte[size];
                // bytes = (BYTES) realloc(oldBytes, size);
                bytes = (BYTES) ReAlloc((LPVOID) oldBytes, size, UNSAFE::ADDRESS_SIZE);
                if (bytes == null)
                    size = 0U;
            } catch (...) { size = 0U; }
            return CORE_CAST(glong, bytes);
        }

        void MemoryHeap::freeMemory(glong addr) {
            BYTES bytes = (BYTES) addr;
            if (addr != 0) {
                //     delete[] bytes;
                // free(bytes);
                Free((LPVOID) bytes);
            }
        }

        MemoryHeap::~MemoryHeap() = default;

        gbool MemoryHeap::isAbleForAllocate() {
            return true;
        }

        void MemoryHeap::setMemory(Object& obj, glong offset, glong bytes, gbyte value) const {
            CORE_IGNORE(this);
            try {
                glong target = &obj == &null ? 0LL : CORE_CAST(glong, &obj);
                checkPointer(null, target);
#if __has_builtin(__builtin_memset)
                __builtin_memset(CORE_CAST(void*, target + offset), value, bytes);
#else
                memset(CORE_CAST(void*, target + offset), value, bytes);
#endif
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void MemoryHeap::copyMemory(Object const& srcBase, glong srcOffset,
                                    Object& destBase, glong destOffset,
                                    glong bytes) const {
            CORE_IGNORE(this);
            try {
                glong target1 = &srcBase == &null
                                    ? 0LL
                                    : CORE_CAST(glong, &srcBase);
                glong target2 = &destBase == &null
                                    ? 0LL
                                    : CORE_CAST(glong, &destBase);
                checkPointer(null, target1);
                checkPointer(null, target2);

#if __has_builtin(__builtin_memcpy)
                __builtin_memcpy(CORE_CAST(void*, target2), CORE_CAST(void*, target1), bytes);
#else
                memcpy(CORE_CAST(void*, target1), CORE_CAST(void*, target2), bytes);
#endif
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void MemoryHeap::copySwapMemory(Object const& srcBase, glong srcOffset,
                                        Object& destBase, glong destOffset,
                                        glong bytes, glong elemSize) const {
            CORE_IGNORE(this);
            try {
                glong target1 = &srcBase == &null
                                    ? 0LL
                                    : CORE_CAST(glong, &srcBase);
                glong target2 = &destBase == &null
                                    ? 0LL
                                    : CORE_CAST(glong, &destBase);

                checkPointer(null, target1);
                checkPointer(null, target2);

                target1 += srcOffset;
                target2 += destOffset;

                glong size = bytes / elemSize;
                gint i = -1;
                switch (elemSize) {
                    case 2: {
                        CORE_ALIAS(REF, Class<gshort>::Pointer);
                        const REF t1 = CORE_CAST(REF, target1);
                        const REF t2 = CORE_CAST(REF, target2);
                        while (++i < size)
                            t2[i] = Short::reverseBytes(t1[i]);
                        break;
                    }
                    case 4: {
                        CORE_ALIAS(REF, Class<gint>::Pointer);
                        const REF t1 = CORE_CAST(REF, target1);
                        const REF t2 = CORE_CAST(REF, target2);
                        while (++i < size)
                            t2[i] = Integer::reverseBytes(t1[i]);
                        break;
                    }
                    case 8: {
                        CORE_ALIAS(REF, Class<glong>::Pointer);
                        const REF t1 = CORE_CAST(REF, target1);
                        const REF t2 = CORE_CAST(REF, target2);
                        while (++i < size)
                            t2[i] = Long::reverseBytes(t1[i]);
                        break;
                    }
                    default:
                        break;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        MemoryHeap MemoryHeap::INSTANCE;
    } // misc
} // core
