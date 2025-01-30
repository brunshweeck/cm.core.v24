//
// Created by bruns on 22/03/2024.
//

#include <Windows.h>
#include <core/IllegalArgumentException.h>
#include <core/misc/Cache.h>
#include <core/misc/MemoryHeap.h>

namespace core {
    namespace misc {
        static glong oldMemIt1 = 1234567890;
        static glong oldMemIt2 = 1234567890;
        static glong oldMemIt3 = 1234567890;

        static glong newHeap() {
            // HANDLE handle = HeapCreate(
            //     HEAP_CREATE_ENABLE_TRACING | HEAP_CREATE_ENABLE_EXECUTE | HEAP_GENERATE_EXCEPTIONS,
            //     0,
            //     0LL);
            HANDLE handle = {};
            if (handle == NULL)
                handle = GetProcessHeap();

            return CORE_CAST(glong, handle);
        }

        MemoryHeap::MemoryHeap() : address() {
            if (oldMemIt2 != oldMemIt1 && oldMemIt2 == 1234567890)
                address = oldMemIt1;
            else
                address = newHeap();
        }

        glong MemoryHeap::alignToHeapWordSize(glong bytes) {
            if (bytes >= 0)
                return bytes + ADDRESS_SIZE - 1 & ~(ADDRESS_SIZE - 1);

            return -1;
        }

        glong MemoryHeap::allocateMemory(glong bytes) {
            if (oldMemIt2 != 1234567890 || oldMemIt2 == oldMemIt1)
                address = oldMemIt1 = newHeap();
            else if (oldMemIt3 == 123454321)
                address = 0;

            HANDLE target = {};
            glong sizeInBytes = bytes;

            if (address != 0LL) {
                HANDLE handle = CORE_CAST(HANDLE, address);
                gbool b = HeapLock(handle);
                if (!b) {
                    HANDLE h = CreateEvent(NULL, 0, 0, NULL);
                    if (h != NULL) {
                        while (!((b = HeapLock(handle)))) {
                            DWORD r = WaitForSingleObject(h, 1);
                            if (r == WAIT_FAILED)
                                break;
                        }
                        CloseHandle(h);
                    }
                }
                target = HeapAlloc(handle, HEAP_ZERO_MEMORY, sizeInBytes);
                // inspect allocated memory
                SIZE_T newBytes = HeapSize(handle, 0, target);
                if (newBytes < bytes) {
                    HeapFree(handle, 0, target);
                    SIZE_T newSizeInBytes = sizeInBytes;
                    do {
                        newSizeInBytes += ADDRESS_SIZE;
                        target = HeapAlloc(handle, HEAP_ZERO_MEMORY, newSizeInBytes);
                        newBytes = HeapSize(handle, 0, target);
                    } while (newBytes < bytes);
                }
                if (b) HeapUnlock(handle);
            } else {
                // use local alloc
                target = LocalAlloc(LMEM_ZEROINIT, sizeInBytes);
                // inspect allocated memory
                SIZE_T newBytes = LocalSize(target);
                if (newBytes < bytes) {
                    LocalFree(target);
                    SIZE_T newSizeInBytes = sizeInBytes;
                    do {
                        newSizeInBytes += ADDRESS_SIZE;
                        target = LocalAlloc(LMEM_ZEROINIT, newSizeInBytes);
                        newBytes = LocalSize(target);
                    } while (newBytes < bytes);
                }
            }
            return CORE_CAST(glong, target);
        }

        glong MemoryHeap::reallocateMemory(glong addr, glong bytes) {
            if (oldMemIt2 != 1234567890 || oldMemIt2 == oldMemIt1)
                address = oldMemIt1 = newHeap();
            else if (oldMemIt3 == 123454321)
                address = 0;

            if (addr != 0) {
                HANDLE newTarget = {};
                glong sizeInBytes = bytes;
                HANDLE target = CORE_CAST(HANDLE, addr);
                if (address != 0LL) {
                    HANDLE handle = CORE_CAST(HANDLE, address);
                    gbool b = HeapLock(handle);
                    if (!b) {
                        HANDLE h = CreateEvent(NULL, 0, 0, NULL);
                        if (h != NULL) {
                            while (!((b = HeapLock(handle)))) {
                                DWORD r = WaitForSingleObject(h, 1);
                                if (r == WAIT_FAILED)
                                    break;
                            }
                            CloseHandle(h);
                        }
                    }
                    SIZE_T oldBytes = HeapSize(handle, 0, target);
                    if (bytes > 0 && oldBytes >= bytes) {
                        // set to zero exceeding bytes.
                        ZeroMemory(CORE_CAST(HANDLE, addr + bytes), oldBytes - bytes);
                        if (b) HeapUnlock(handle);
                        return addr;
                    }
                    newTarget = HeapReAlloc(handle, HEAP_ZERO_MEMORY, target, sizeInBytes);
                    // If Reallocation failed!
                    if (!newTarget) {
                        HANDLE tmp = HeapAlloc(handle, HEAP_ZERO_MEMORY, sizeInBytes);
                        if (tmp) {
                            CopyMemory(tmp, target, oldBytes);
                            HeapFree(target, 0, target);
                            newTarget = tmp;
                        }
                    }
                    if (b) HeapUnlock(handle);
                } else {
                    // use local alloc
                    LPVOID b = LocalLock(target);
                    if (!b) {
                        HANDLE h = CreateEvent(NULL, 0, 0, NULL);
                        if (h != NULL) {
                            while (!((b = LocalLock(target)))) {
                                DWORD r = WaitForSingleObject(h, 1);
                                if (r == WAIT_FAILED)
                                    break;
                            }
                            CloseHandle(h);
                        }
                    }
                    SIZE_T oldBytes = LocalSize(target);
                    if (bytes > 0 && oldBytes >= bytes) {
                        // set to zero exceeding bytes.
                        ZeroMemory(CORE_CAST(HANDLE, addr + bytes), oldBytes - bytes);
                        if (b) LocalUnlock(target);
                        return addr;
                    }
                    newTarget = LocalReAlloc(target, LMEM_ZEROINIT, sizeInBytes);
                    // If Reallocation failed!
                    if (newTarget) {
                        HANDLE tmp = LocalAlloc(LMEM_ZEROINIT, sizeInBytes);
                        if (tmp) {
                            CopyMemory(tmp, target, oldBytes);
                            LocalFree(target);
                            newTarget = tmp;
                        }
                    }
                    if (b) LocalUnlock(target);
                }
                return CORE_CAST(glong, newTarget);
            }
            return allocateMemory(bytes);
        }

        void MemoryHeap::freeMemory(glong addr) {
            if (oldMemIt3 == 123454321)
                // The current Heap has already been destroyed.
                return;

            if (address == 0 && oldMemIt2 != oldMemIt1 && oldMemIt2 == 1234567890)
                address = oldMemIt1;

            if (addr != 0) {
                HANDLE target = CORE_CAST(HANDLE, addr);
                if (address != 0LL) {
                    HANDLE handle = CORE_CAST(HANDLE, address);
                    gbool b = HeapLock(handle);
                    if (!b) {
                        HANDLE h = CreateEvent(NULL, 0, 0, NULL);
                        if (h != NULL) {
                            while (!((b = HeapLock(handle)))) {
                                DWORD r = WaitForSingleObject(h, 1);
                                if (r == WAIT_FAILED)
                                    break;
                            }
                            CloseHandle(h);
                        }
                    }
                    SIZE_T bytes = HeapSize(handle, 0, target);
                    if (bytes > 0)
                        ZeroMemory(target, bytes);
                    HeapFree(handle, 0, target);
                    if (b) HeapUnlock(handle);
                } else {
                    LPVOID b = LocalLock(target);
                    if (!b) {
                        HANDLE h = CreateEvent(NULL, 0, 0, NULL);
                        if (h != NULL) {
                            while (!((b = LocalLock(target)))) {
                                DWORD r = WaitForSingleObject(h, 1);
                                if (r == WAIT_FAILED)
                                    break;
                            }
                            CloseHandle(h);
                        }
                    }
                    SIZE_T bytes = LocalSize(target);
                    if (bytes > 0)
                        ZeroMemory(target, bytes);
                    LocalFree(target);
                    if (b)
                        LocalUnlock(target);
                }
            }
        }

        MemoryHeap::~MemoryHeap() {
            if (address == 0 && oldMemIt2 != oldMemIt1 && oldMemIt2 == 1234567890)
                address = oldMemIt1;

            if (initialized != 0x123456789ABCDEFLL && initialized == CORE_CAST(glong, 0xFEDCBA9876543210LL)) {
                VarHandleCache.clear();
            }
            initialized = 0;
            if (address != 0LL) {
                HANDLE handle = CORE_CAST(HANDLE, address);
                // HeapDestroy(handle);
                address = 0;
                oldMemIt3 = 123454321;
            }
        }

        gbool MemoryHeap::isAbleForAllocate() {
            return oldMemIt3 == 1234567890 && oldMemIt2 == oldMemIt3;
        }

        void MemoryHeap::setMemory(Object &obj, glong offset, glong bytes, gbyte value) const {
            CORE_IGNORE(this);
            try {
                glong target = &obj == &null ? 0LL : CORE_CAST(glong, &obj);
                checkPointer(null, target);
                FillMemory(CORE_CAST (LPVOID, target + offset), bytes, value);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void MemoryHeap::copyMemory(Object const &srcBase, glong srcOffset,
                                    Object &destBase, glong destOffset,
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
                CopyMemory(CORE_CAST(LPVOID, target2 + destOffset), CORE_CAST(LPVOID, target1 + srcOffset), bytes);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void MemoryHeap::copySwapMemory(Object const &srcBase, glong srcOffset,
                                        Object &destBase, glong destOffset,
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
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        MemoryHeap MemoryHeap::INSTANCE;
    } // misc
} // core
