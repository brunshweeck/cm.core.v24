//
// Created by bruns on 22/03/2024.
//

#include <Windows.h>
#include <core/IllegalArgumentException.h>
#include <core/misc/VarHandle.h>

#ifdef CORE_COMPILER_MSVC
#include <intrin.h>
#endif


namespace core {
    namespace misc {
#define Compiler_barrier() _ReadWriteBarrier()

#if defined(CORE_PROCESSOR_ARM) || defined(CORE_PROCESSOR_ARM_32) || defined(CORE_PROCESSOR_ARM_64)
#define Memory_barrier()             __dmb(0xB) // inner shared data memory barrier
#define Compiler_or_Memory_barrier() Memory_barrier()
#elif defined(CORE_PROCESSOR_X86_32) || defined(CORE_PROCESSOR_X86_64)
        // x86/x64 hardware only emits memory barriers inside _Interlocked intrinsics
#define Compiler_or_Memory_barrier() Compiler_barrier()
#else // ^^^ x86/x64 / unsupported hardware vvv
#error Unsupported hardware
#endif // hardware

        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        static glong o2l(Object const &o) { return o == null ? 0LL : CORE_CAST(glong, &o); }

        gbool VarHandle::is32BitClean(glong value) { return (value + 0uLL) >> 32 == 0; }

        void VarHandle::checkSize(glong size) {
            if (Unsafe::addressSize() == 4) {
                // Note: this will also check for negative sizes
                if (!is32BitClean(size))
                    IllegalArgumentException("Size value is too large"_S).throws($ftrace());
            } else if (size < 0)
                IllegalArgumentException("Negative size"_S).throws($ftrace());
        }

        void VarHandle::checkNativeAddress(glong address) {
            if (Unsafe::addressSize() == 4) {
                // Accept both zero and sign extended pointers. A valid
                // pointer will, after the +1 below, either have produced
                // the value 0x0 or 0x1. Masking off the low bit allows
                // for testing against 0.
                if ((((address >> 32) + 1) & ~1) != 0)
                    IllegalArgumentException("Unsupported memory address"_S).throws($ftrace());
            }
        }

        void VarHandle::checkOffset(Object const &o, glong offset) {
            if (Unsafe::addressSize() == 4) {
                // Note: this will also check for negative offsets
                if (!is32BitClean(offset)) {
                    IllegalArgumentException("Invalid address offset"_S).throws($ftrace());
                }
            } else if (offset < 0) {
                IllegalArgumentException("Negative address offset"_S).throws($ftrace());
            }
        }

        void VarHandle::checkPointer(Object const &o, glong offset) {
            try {
                if (&o == &null)
                    checkNativeAddress(offset);
                else
                    checkOffset(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte VarHandle::getByte(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            CHAR load8 = __iso_volatile_load8(CORE_CAST(CHAR volatile *, address));
#else
            CHAR load8 = __atomic_load_n(CORE_CAST(CHAR volatile *, address), 5);
#endif
            Compiler_or_Memory_barrier();
            return load8;
        }

        gbyte VarHandle::getByteVolatile(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            CHAR load8 = __iso_volatile_load8(CORE_CAST(CHAR volatile *, address));
#else
            CHAR load8 = __atomic_load_n(CORE_CAST(CHAR volatile *, address), 2);
#endif
            Compiler_or_Memory_barrier();
            return load8;
        }

        gbyte VarHandle::getByteOpaque(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            CHAR load8 = __iso_volatile_load8(CORE_CAST(CHAR volatile *, address));
#else
            CHAR load8 = __atomic_load_n(CORE_CAST(CHAR volatile *, address), 0);
#endif
            // no barrier.
            return load8;
        }

        gshort VarHandle::getShort(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            SHORT load16 = __iso_volatile_load16(CORE_CAST(SHORT volatile *, address));
#else
            SHORT load16 = __atomic_load_n(CORE_CAST(SHORT volatile *, address), 5);
#endif
            Compiler_or_Memory_barrier();
            return load16;
        }

        gshort VarHandle::getShortVolatile(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            SHORT load16 = __iso_volatile_load16(CORE_CAST(SHORT volatile *, address));
#else
            SHORT load16 = __atomic_load_n(CORE_CAST(SHORT volatile *, address), 2);
#endif
            Compiler_or_Memory_barrier();
            return load16;
        }

        gshort VarHandle::getShortOpaque(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            SHORT load16 = __iso_volatile_load16(CORE_CAST(SHORT volatile *, address));
#else
            SHORT load16 = __atomic_load_n(CORE_CAST(SHORT volatile *, address), 0);
#endif
            // no barrier.
            return load16;
        }

        gint VarHandle::getInt(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            __int32 load32 = __iso_volatile_load32(CORE_CAST(__int32 volatile *, address));
#else
            LONG load32 = __atomic_load_n(CORE_CAST(LONG volatile *, address), 5);
#endif
            Compiler_or_Memory_barrier();
            return load32;
        }

        gint VarHandle::getIntVolatile(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            __int32 load32 = __iso_volatile_load32(CORE_CAST(__int32 volatile *, address));
#else
            LONG load32 = __atomic_load_n(CORE_CAST(LONG volatile *, address), 2);
#endif
            Compiler_or_Memory_barrier();
            return load32;
        }

        gint VarHandle::getIntOpaque(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            __int32 load32 = __iso_volatile_load32(CORE_CAST(__int32 volatile *, address));
#else
            LONG load32 = __atomic_load_n(CORE_CAST(LONG volatile *, address), 0);
#endif
            // no barrier.
            return load32;
        }

        glong VarHandle::getLong(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
#if defined(CORE_PROCESSOR_ARM) || defined(CORE_PROCESSOR_ARM_32) || defined(CORE_PROCESSOR_ARM_64)
            __int64 load64 = __ldrexd(CORE_CAST(__int64 volatile *, address));
#else
            __int64 load64 = __iso_volatile_load64(CORE_CAST(__int64 volatile *, address));
#endif
            Compiler_or_Memory_barrier();
#else
            glong load64 = __atomic_load_n((glong volatile *) address, 5);
#endif
            return load64;
        }

        glong VarHandle::getLongVolatile(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
#if defined(CORE_PROCESSOR_ARM) || defined(CORE_PROCESSOR_ARM_32) || defined(CORE_PROCESSOR_ARM_64)
            __int64 load64 = __ldrexd(CORE_CAST(__int64 volatile *, address));
#else
            __int64 load64 = __iso_volatile_load64(CORE_CAST(__int64 volatile *, address));
#endif
            Compiler_or_Memory_barrier();
#else
            glong load64 = __atomic_load_n((glong volatile *) address, 2);
#endif
            return load64;
        }

        glong VarHandle::getLongOpaque(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
#if defined(CORE_PROCESSOR_ARM) || defined(CORE_PROCESSOR_ARM_32) || defined(CORE_PROCESSOR_ARM_64)
            __int64 load64 = __ldrexd(CORE_CAST(__int64 volatile *, address));
#else
            __int64 load64 = __iso_volatile_load64(CORE_CAST(__int64 volatile *, address));
#endif
            // no barrier.
#else
            glong load64 = __atomic_load_n((glong volatile *) address, 0);
#endif
            return load64;
        }

        void VarHandle::putByte(Object &o, glong offset, gbyte x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            InterlockedExchange8(CORE_CAST(CHAR volatile *, address), x);
#else
            return __atomic_store_n(CORE_CAST(CHAR volatile *, address), x, 5);
#endif
        }

        void VarHandle::putByteVolatile(Object &o, glong offset, gbyte x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            Compiler_or_Memory_barrier();
            InterlockedExchange8(CORE_CAST(CHAR volatile *, address), x);
#else
            return __atomic_store_n(CORE_CAST(CHAR volatile *, address), x, 3);
#endif
        }

        void VarHandle::putByteOpaque(Object &o, glong offset, gbyte x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            InterlockedExchange8(CORE_CAST(CHAR volatile *, address), x);
#else
            return __atomic_store_n(CORE_CAST(CHAR volatile *, address), x, 0);
#endif
        }

        void VarHandle::putShort(Object &o, glong offset, gshort x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            InterlockedExchange16(CORE_CAST(SHORT volatile *, address), x);
#else
            return __atomic_store_n(CORE_CAST(SHORT volatile *, address), x, 5);
#endif
        }

        void VarHandle::putShortVolatile(Object &o, glong offset, gshort x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            Compiler_or_Memory_barrier();
            InterlockedExchange16(CORE_CAST(SHORT volatile *, address), x);
#else
            return __atomic_store_n(CORE_CAST(SHORT volatile *, address), x, 3);
#endif
        }

        void VarHandle::putShortOpaque(Object &o, glong offset, gshort x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            InterlockedExchange16(CORE_CAST(SHORT volatile *, address), x);
#else
            return __atomic_store_n(CORE_CAST(SHORT volatile *, address), x, 0);
#endif
        }

        void VarHandle::putInt(Object &o, glong offset, gint x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            InterlockedExchange(CORE_CAST(LONG volatile *, address), x);
#else
            return __atomic_store_n(CORE_CAST(LONG volatile *, address), x, 5);
#endif
        }

        void VarHandle::putIntVolatile(Object &o, glong offset, gint x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            InterlockedExchange(CORE_CAST(LONG volatile *, address), x);
            Compiler_or_Memory_barrier();
#else
            return __atomic_store_n(CORE_CAST(LONG volatile *, address), x, 3);
#endif
        }

        void VarHandle::putIntOpaque(Object &o, glong offset, gint x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            InterlockedExchange(CORE_CAST(LONG volatile *, address), x);
#else
            return __atomic_store_n(CORE_CAST(LONG volatile *, address), x, 0);
#endif
        }

        void VarHandle::putLong(Object &o, glong offset, glong x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
#if defined(CORE_PROCESSOR_ARM) || defined(CORE_PROCESSOR_ARM_32) || defined(CORE_PROCESSOR_ARM_64)
            __iso_volatile_store64(CORE_CAST(__int64 volatile *, address), x);
            Compiler_or_Memory_barrier();
#else
            InterlockedExchange64(CORE_CAST(__int64 volatile *, address), x);
            Compiler_or_Memory_barrier();
            *CORE_CAST(__int64 volatile *, address) = x;
#endif
#else
            return __atomic_store_n((glong volatile *) address, x, 5);
#endif
        }

        void VarHandle::putLongVolatile(Object &o, glong offset, glong x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            Compiler_or_Memory_barrier();
            __iso_volatile_store64(CORE_CAST(__int64 volatile *, address), x);
#else
            return __atomic_store_n((glong volatile *) address, x, 3);
#endif
        }

        void VarHandle::putLongOpaque(Object &o, glong offset, glong x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            InterlockedExchange64(CORE_CAST(__int64 volatile *, address), x);
            // no barrier.
#else
            return __atomic_store_n((glong volatile *) address, x, 0);
#endif
        }

        gint VarHandle::pickPos(gint top, gint pos) { return Unsafe::BIG_ENDIAN ? top - pos : pos; }

        glong VarHandle::makeLong(gbyte i0, gbyte i1, gbyte i2, gbyte i3, gbyte i4, gbyte i5, gbyte i6, gbyte i7) {
            return ((toUnsignedLong(i0) << pickPos(56, 0))
                    | (toUnsignedLong(i1) << pickPos(56, 8))
                    | (toUnsignedLong(i2) << pickPos(56, 16))
                    | (toUnsignedLong(i3) << pickPos(56, 24))
                    | (toUnsignedLong(i4) << pickPos(56, 32))
                    | (toUnsignedLong(i5) << pickPos(56, 40))
                    | (toUnsignedLong(i6) << pickPos(56, 48))
                    | (toUnsignedLong(i7) << pickPos(56, 56)));
        }

        glong VarHandle::makeLong(gint i0, gint i1) {
            return (toUnsignedLong(i0) << pickPos(32, 0))
                   | (toUnsignedLong(i1) << pickPos(32, 32));
        }

        glong VarHandle::makeLong(gshort i0, gshort i1, gshort i2, gshort i3) {
            return ((toUnsignedLong(i0) << pickPos(48, 0))
                    | (toUnsignedLong(i1) << pickPos(48, 16))
                    | (toUnsignedLong(i2) << pickPos(48, 32))
                    | (toUnsignedLong(i3) << pickPos(48, 48)));
        }

        gint VarHandle::makeInt(gbyte i0, gbyte i1, gbyte i2, gbyte i3) {
            return ((toUnsignedInt(i0) << pickPos(24, 0))
                    | (toUnsignedInt(i1) << pickPos(24, 8))
                    | (toUnsignedInt(i2) << pickPos(24, 16))
                    | (toUnsignedInt(i3) << pickPos(24, 24)));
        }

        gshort VarHandle::makeShort(gbyte i0, gbyte i1) {
            return CORE_CAST(gshort, (toUnsignedInt(i0) << pickPos(8, 0))
                             | (toUnsignedInt(i1) << pickPos(8, 8)));
        }

        gint VarHandle::makeInt(gshort i0, gshort i1) {
            return (toUnsignedInt(i0) << pickPos(16, 0))
                   | (toUnsignedInt(i1) << pickPos(16, 16));
        }

        gbyte VarHandle::pick(gbyte le, gbyte be) { return BIG_ENDIAN ? be : le; }

        gshort VarHandle::pick(gshort le, gshort be) { return BIG_ENDIAN ? be : le; }

        gint VarHandle::pick(gint le, gint be) { return BIG_ENDIAN ? be : le; }

        void VarHandle::putLongParts(Object &o, glong offset,
                                     gbyte i0, gbyte i1, gbyte i2, gbyte i3, gbyte i4,
                                     gbyte i5, gbyte i6, gbyte i7) {
            Unsafe::putByte(o, offset + 0, pick(i0, i7));
            Unsafe::putByte(o, offset + 1, pick(i1, i6));
            Unsafe::putByte(o, offset + 2, pick(i2, i5));
            Unsafe::putByte(o, offset + 3, pick(i3, i4));
            Unsafe::putByte(o, offset + 4, pick(i4, i3));
            Unsafe::putByte(o, offset + 5, pick(i5, i2));
            Unsafe::putByte(o, offset + 6, pick(i6, i1));
            Unsafe::putByte(o, offset + 7, pick(i7, i0));
        }

        void VarHandle::putLongParts(Object &o, glong offset, gshort i0, gshort i1, gshort i2, gshort i3) {
            Unsafe::putShort(o, offset + 0, pick(i0, i3));
            Unsafe::putShort(o, offset + 2, pick(i1, i2));
            Unsafe::putShort(o, offset + 4, pick(i2, i1));
            Unsafe::putShort(o, offset + 6, pick(i3, i0));
        }

        void VarHandle::putLongParts(Object &o, glong offset, gint i0, gint i1) {
            Unsafe::putInt(o, offset + 0, pick(i0, i1));
            Unsafe::putInt(o, offset + 4, pick(i1, i0));
        }

        void VarHandle::putIntParts(Object &o, glong offset, gbyte i0, gbyte i1, gbyte i2, gbyte i3) {
            Unsafe::putByte(o, offset + 0, pick(i0, i3));
            Unsafe::putByte(o, offset + 1, pick(i1, i2));
            Unsafe::putByte(o, offset + 2, pick(i2, i1));
            Unsafe::putByte(o, offset + 3, pick(i3, i0));
        }

        void VarHandle::putShortParts(Object &o, glong offset, gbyte i0, gbyte i1) {
            Unsafe::putByte(o, offset + 0, pick(i0, i1));
            Unsafe::putByte(o, offset + 1, pick(i1, i0));
        }

        gint VarHandle::toUnsignedInt(gbyte n) { return n & 0xff; }

        gint VarHandle::toUnsignedInt(gshort n) { return n & 0xffff; }

        glong VarHandle::toUnsignedLong(gbyte n) { return n & 0xffl; }

        glong VarHandle::toUnsignedLong(gshort n) { return n & 0xffffl; }

        glong VarHandle::toUnsignedLong(gint n) { return n & 0xffffffffl; }

        gchar VarHandle::convEndian(gbool big, gchar n) { return big == BIG_ENDIAN ? n : Character::reverseBytes(n); }

        gshort VarHandle::convEndian(gbool big, gshort n) { return big == BIG_ENDIAN ? n : Short::reverseBytes(n); }

        gint VarHandle::convEndian(gbool big, gint n) { return big == BIG_ENDIAN ? n : Integer::reverseBytes(n); }

        glong VarHandle::convEndian(gbool big, glong n) { return big == BIG_ENDIAN ? n : Long::reverseBytes(n); }

        void VarHandle::putIntParts(Object &o, glong offset, gshort i0, gshort i1) {
            Unsafe::putShort(o, offset + 0, pick(i0, i1));
            Unsafe::putShort(o, offset + 2, pick(i1, i0));
        }

        gbyte VarHandle::compareAndExchangeByte(Object &o, glong offset, gbyte expected, gbyte x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            return _InterlockedCompareExchange8(CORE_CAST(CHAR volatile *, address), x, expected);
#else
            return __atomic_compare_exchange_n(CORE_CAST(CHAR volatile *, address), (CHAR*)&expected, x, false, 0, 0) ?
            expected : (gbyte) __atomic_load_n(CORE_CAST(CHAR volatile *, address), 5);
#endif
        }

        gbyte VarHandle::compareAndExchangeByteAcquire(Object &o, glong offset, gbyte expected, gbyte x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndExchangeByte(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(CHAR volatile *, address), (CHAR*)&expected, x, false, 2, 2) ?
                   expected : (gbyte) __atomic_load_n(CORE_CAST(CHAR volatile *, address), 2);
#endif
        }

        gbyte VarHandle::compareAndExchangeByteRelease(Object &o, glong offset, gbyte expected, gbyte x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndExchangeByte(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(CHAR volatile *, address), (CHAR*)&expected, x, false, 3, 0) ?
                   expected : (gbyte) __atomic_load_n(CORE_CAST(CHAR volatile *, address), 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetByte(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(CHAR volatile *, address), (CHAR*)&expected, x, true, 0, 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetByte(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(CHAR volatile *, address), (CHAR*)&expected, x, true, 2, 2);
#endif
        }

        gbool VarHandle::weakCompareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetByte(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(CHAR volatile *, address), (CHAR*)&expected, x, true, 3, 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetBytePlain(Object &o, glong offset, gbyte expected, gbyte x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetByte(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(CHAR volatile *, address), (CHAR*)&expected, x, true, 5, 5);
#endif
        }

        gshort VarHandle::compareAndExchangeShort(Object &o, glong offset, gshort expected, gshort x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            return InterlockedCompareExchange16(CORE_CAST(SHORT volatile *, address), x, expected);
#else
            return __atomic_compare_exchange_n(CORE_CAST(SHORT volatile *, address), (SHORT*)&expected, x, false, 0, 0) ?
                   expected : (SHORT) __atomic_load_n(CORE_CAST(SHORT volatile *, address), 5);
#endif
        }

        gshort VarHandle::compareAndExchangeShortAcquire(Object &o, glong offset, gshort expected, gshort x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndExchangeShort(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(SHORT volatile *, address), (SHORT*)&expected, x, false, 2, 2) ?
                   expected : (SHORT) __atomic_load_n(CORE_CAST(SHORT volatile *, address), 5);
#endif
        }

        gshort VarHandle::compareAndExchangeShortRelease(Object &o, glong offset, gshort expected, gshort x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndExchangeShort(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(SHORT volatile *, address), (SHORT*)&expected, x, false, 3, 0) ?
                   expected : (SHORT) __atomic_load_n(CORE_CAST(SHORT volatile *, address), 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetShort(Object &o, glong offset, gshort expected, gshort x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetShort(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(SHORT volatile *, address), (SHORT*)&expected, x, true, 0, 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetShort(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(SHORT volatile *, address), (SHORT*)&expected, x, true, 2, 2);
#endif
        }

        gbool VarHandle::weakCompareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetShort(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(SHORT volatile *, address), (SHORT*)&expected, x, true, 3, 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetShortPlain(Object &o, glong offset, gshort expected, gshort x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetShort(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(SHORT volatile *, address), (SHORT*)&expected, x, true, 5, 5);
#endif
        }

        gint VarHandle::compareAndExchangeInt(Object &o, glong offset, gint expected, gint x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            return InterlockedCompareExchange(CORE_CAST(LONG volatile *, address), x, expected);
#else
            return __atomic_compare_exchange_n(CORE_CAST(LONG volatile *, address), (LONG*)&expected, x, false, 0, 0) ?
                   expected : (LONG) __atomic_load_n(CORE_CAST(LONG volatile *, address), 5);
#endif
        }

        gint VarHandle::compareAndExchangeIntAcquire(Object &o, glong offset, gint expected, gint x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndExchangeInt(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(LONG volatile *, address), (LONG*)&expected, x, false, 2, 2) ?
                   expected : (LONG) __atomic_load_n(CORE_CAST(LONG volatile *, address), 0);
#endif
        }

        gint VarHandle::compareAndExchangeIntRelease(Object &o, glong offset, gint expected, gint x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndExchangeInt(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(LONG volatile *, address), (LONG*)&expected, x, false, 3, 0) ?
                   expected : (LONG) __atomic_load_n(CORE_CAST(LONG volatile *, address), 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetInt(Object &o, glong offset, gint expected, gint x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetInt(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(LONG volatile *, address), (LONG*)&expected, x, true, 0, 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetInt(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(LONG volatile *, address), (LONG*)&expected, x, true, 2, 2);
#endif
        }

        gbool VarHandle::weakCompareAndSetIntRelease(Object &o, glong offset, gint expected, gint x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetInt(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(LONG volatile *, address), (LONG*)&expected, x, true, 3, 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetIntPlain(Object &o, glong offset, gint expected, gint x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetInt(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(CORE_CAST(LONG volatile *, address), (LONG*)&expected, x, true, 5, 5);
#endif
        }

        glong VarHandle::compareAndExchangeLong(Object &o, glong offset, glong expected, glong x) {
            glong address = o2l(o) + offset;
#ifdef CORE_COMPILER_MSVC
            return InterlockedCompareExchange64(CORE_CAST(__int64 volatile *, address), x, expected);
#else
            return __atomic_compare_exchange_n((glong volatile *) address, (glong*)&expected, x, false, 0, 0) ?
                   expected : (glong) __atomic_load_n((glong volatile *) address, 5);
#endif
        }

        glong VarHandle::compareAndExchangeLongAcquire(Object &o, glong offset, glong expected, glong x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndExchangeLong(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n((glong volatile *) address, (glong*)&expected, x, false, 2, 2) ?
                   expected : (glong) __atomic_load_n((glong volatile *) address, 5);
#endif
        }

        glong VarHandle::compareAndExchangeLongRelease(Object &o, glong offset, glong expected, glong x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndExchangeLong(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n((glong volatile *) address, (glong*)&expected, x, false, 3, 0) ?
                   expected : (glong) __atomic_load_n((glong volatile *) address, 5);
#endif
        }

        gbool VarHandle::weakCompareAndSetLong(Object &o, glong offset, glong expected, glong x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetLong(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n((glong volatile *) address, (glong*)&expected, x, true, 0, 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetLong(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n((glong volatile *) address, (glong*)&expected, x, true, 2, 2);
#endif
        }

        gbool VarHandle::weakCompareAndSetLongRelease(Object &o, glong offset, glong expected, glong x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetLong(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n((glong volatile *) address, (glong*)&expected, x, true, 3, 0);
#endif
        }

        gbool VarHandle::weakCompareAndSetLongPlain(Object &o, glong offset, glong expected, glong x) {
#ifdef CORE_COMPILER_MSVC
            return Unsafe::compareAndSetLong(o, offset, expected, x);
#else
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n((glong volatile *) address, (glong*)&expected, x, true, 5, 5);
#endif
        }

        void VarHandle::loadFence() {
#ifdef CORE_COMPILER_MSVC
            LoadFence();
#else
            __atomic_thread_fence(2);
#endif
        }

        void VarHandle::storeFence() {
#ifdef CORE_COMPILER_MSVC
            StoreFence();
#else
            __atomic_thread_fence(3);
#endif
        }

        void VarHandle::fullFence() {
#ifdef CORE_COMPILER_MSVC
            MemoryFence();
#else
            __atomic_thread_fence(5);
#endif
        }

        CORE_WARNING_POP
    } // misc
} // core
