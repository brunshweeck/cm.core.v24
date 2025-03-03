//
// Created by bruns on 22/03/2024.
//

#ifndef CORE24_MEMORYHEAP_H
#define CORE24_MEMORYHEAP_H

#include <core/misc/VarHandle.h>

namespace core {
    namespace misc {
        class MemoryHeap final : public VarHandle {
            glong address;

            CORE_IMPLICIT MemoryHeap();

        public:
            /**
             * Round up allocation size to a multiple of HeapWordSize.
             */
            static glong alignToHeapWordSize(glong bytes);

            static MemoryHeap INSTANCE;

            glong allocateMemory(glong bytes);

            glong reallocateMemory(glong addr, glong bytes);

            void freeMemory(glong addr);

            void setMemory(Object &obj, glong offset, glong bytes, gbyte value) const;

            void copyMemory(Object const &srcBase, glong srcOffset,
                            Object &destBase, glong destOffset,
                            glong bytes) const;

            void copySwapMemory(Object const &srcBase, glong srcOffset,
                                Object &destBase, glong destOffset,
                                glong bytes, glong elemSize) const;

            ~MemoryHeap() override;

            static gbool isAbleForAllocate();

        };
    } // misc
} // core

#endif //CORE24_MEMORYHEAP_H
