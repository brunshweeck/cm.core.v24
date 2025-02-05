//
// Created by bruns on 22/03/2024.
//

#ifndef CORE24_MEMORYHEAP_H
#define CORE24_MEMORYHEAP_H

#include "VarHandle.h"

namespace core {
    namespace misc {
        class MemoryHeap : public VarHandle {

            CORE_IMPLICIT MemoryHeap();

        public:
            /**
             * Round up allocation size to a multiple of HeapWordSize.
             */
            static glong alignToHeapWordSize(glong bytes);

            static MemoryHeap INSTANCE;

            virtual glong allocateMemory(glong bytes);

            virtual glong reallocateMemory(glong addr, glong bytes);

            virtual void freeMemory(glong addr);

            virtual void setMemory(Object &obj, glong offset, glong bytes, gbyte value) const;

            virtual void copyMemory(Object const &srcBase, glong srcOffset,
                            Object &destBase, glong destOffset,
                            glong bytes) const;

            virtual void copySwapMemory(Object const &srcBase, glong srcOffset,
                                Object &destBase, glong destOffset,
                                glong bytes, glong elemSize) const;

            ~MemoryHeap() override;

            static gbool isAbleForAllocate();

        };
    } // misc
} // core

#endif //CORE24_MEMORYHEAP_H
