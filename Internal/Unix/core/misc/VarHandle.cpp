//
// Created by bruns on 22/03/2024.
//

#include <stdlib.h>

#ifdef BIG_ENDIAN
#undef BIG_ENDIAN
#endif

#include "VarHandle.h"

#include <core/IllegalArgumentException.h>


namespace core {
    namespace misc {
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
            CORE_IGNORE(o);
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
#if __has_builtin(__atomic_load_n)
            gbyte load8 = __atomic_load_n(CORE_CAST(gbyte volatile *, address), __ATOMIC_SEQ_CST);
#elif __has_builtin(__builtin_memcpy)
            gbyte load8 = 0;
            __builtin_memcpy(&load8, CORE_CAST(void *, address), 1);
#else
            gbyte load8 = *CORE_CAST(gbyte volatile *, address);
#endif
            return load8;
        }

        gbyte VarHandle::getByteVolatile(Object const &o, glong offset) {
#if __has_builtin(__atomic_load_n)
            glong address = o2l(o) + offset;
            return __atomic_load_n(CORE_CAST(gbyte volatile *, address), __ATOMIC_ACQUIRE);
#else
            return getByte(o, offset);
#endif
        }

        gbyte VarHandle::getByteOpaque(Object const &o, glong offset) {
#if __has_builtin(__atomic_load_n)
            glong address = o2l(o) + offset;
            return __atomic_load_n(CORE_CAST(gbyte volatile *, address), __ATOMIC_RELAXED);
#else
            return getByte(o, offset);
#endif
        }

        gshort VarHandle::getShort(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_load_n)
            gshort load16 = __atomic_load_n(CORE_CAST(gshort volatile *, address), __ATOMIC_SEQ_CST);
#elif __has_builtin(__builtin_memcpy)
            gshort load16 = 0;
            __builtin_memcpy(&load16, CORE_CAST(void *, address), 2);
#else
            gshort load16 = *CORE_CAST(gshort volatile *, address);
#endif
            return load16;
        }

        gshort VarHandle::getShortVolatile(Object const &o, glong offset) {
#if __has_builtin(__atomic_load_n)
            glong address = o2l(o) + offset;
            return __atomic_load_n(CORE_CAST(gshort volatile *, address), __ATOMIC_ACQUIRE);
#else
            return getShort(o, offset);
#endif
        }

        gshort VarHandle::getShortOpaque(Object const &o, glong offset) {
#if __has_builtin(__atomic_load_n)
            glong address = o2l(o) + offset;
            return __atomic_load_n(CORE_CAST(gshort volatile *, address), __ATOMIC_RELAXED);
#else
            return getShort(o, offset);
#endif
        }

        gint VarHandle::getInt(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_load_n)
            gint load32 = __atomic_load_n(CORE_CAST(gint volatile *, address), __ATOMIC_SEQ_CST);
#elif __has_builtin(__builtin_memcpy)
            gint load32 = 0;
            __builtin_memcpy(&load32, CORE_CAST(void *, address), 4);
#else
            gint load32 = *CORE_CAST(gint volatile *, address);
#endif
            return load32;
        }

        gint VarHandle::getIntVolatile(Object const &o, glong offset) {
#if __has_builtin(__atomic_load_n)
            glong address = o2l(o) + offset;
            return __atomic_load_n(CORE_CAST(gint volatile *, address), __ATOMIC_ACQUIRE);
#else
            return getInt(o, offset);
#endif
        }

        gint VarHandle::getIntOpaque(Object const &o, glong offset) {
#if __has_builtin(__atomic_load_n)
            glong address = o2l(o) + offset;
            return __atomic_load_n(CORE_CAST(gint volatile *, address), __ATOMIC_RELAXED);
#else
            return getInt(o, offset);
#endif
        }

        glong VarHandle::getLong(Object const &o, glong offset) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_load_n)
            glong load64 = __atomic_load_n(CORE_CAST(glong volatile *, address), __ATOMIC_SEQ_CST);
#elif __has_builtin(__builtin_memcpy)
            glong load64 = 0;
            __builtin_memcpy(&load64, CORE_CAST(void *, address), 1);
#else
            glong load64 = *CORE_CAST(glong volatile *, address);
#endif
            return load64;
        }

        glong VarHandle::getLongVolatile(Object const &o, glong offset) {
#if __has_builtin(__atomic_load_n)
            glong address = o2l(o) + offset;
            return __atomic_load_n(CORE_CAST(glong volatile *, address), __ATOMIC_ACQUIRE);
#else
            return getLong(o, offset);
#endif
        }

        glong VarHandle::getLongOpaque(Object const &o, glong offset) {
#if __has_builtin(__atomic_load_n)
            glong address = o2l(o) + offset;
            return  __atomic_load_n(CORE_CAST(glong volatile *, address), __ATOMIC_RELAXED);
#else
            return getLong(o, offset);
#endif
        }

        void VarHandle::putByte(Object &o, glong offset, gbyte x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gbyte volatile *, address), x, __ATOMIC_SEQ_CST);
#elif __has_builtin(__builtin_memset)
            __builtin_memset(CORE_CAST(void*, address), x, 1);
#else
            *CORE_CAST(gbyte volatile *, address) = x;
#endif
        }

        void VarHandle::putByteVolatile(Object &o, glong offset, gbyte x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gbyte volatile *, address), x, 3);
#else
            putByte(o, offset, x);
#endif
        }

        void VarHandle::putByteOpaque(Object &o, glong offset, gbyte x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gbyte volatile *, address), x, __ATOMIC_RELAXED);
#else
            putByte(o, offset, x);
#endif
        }

        void VarHandle::putShort(Object &o, glong offset, gshort x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gshort volatile *, address), x, __ATOMIC_SEQ_CST);
#elif __has_builtin(__builtin_memcpy)
            __builtin_memcpy(CORE_CAST(void*, address), &x, 2);
#else
            *CORE_CAST(gshort volatile *, address) = x;
#endif
        }

        void VarHandle::putShortVolatile(Object &o, glong offset, gshort x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gshort volatile *, address), x, 3);
#else
            putShort(o, offset, x);
#endif
        }

        void VarHandle::putShortOpaque(Object &o, glong offset, gshort x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gshort volatile *, address), x, __ATOMIC_RELAXED);
#else
            putShort(o, offset, x);
#endif
        }

        void VarHandle::putInt(Object &o, glong offset, gint x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gint volatile *, address), x, __ATOMIC_SEQ_CST);
#elif __has_builtin(__builtin_memcpy)
            __builtin_memcpy(CORE_CAST(void*, address), &x, 4);
#else
            *CORE_CAST(gint volatile *, address) = x;
#endif
        }

        void VarHandle::putIntVolatile(Object &o, glong offset, gint x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gint volatile *, address), x, 3);
#else
            putInt(o, offset, x);
#endif
        }

        void VarHandle::putIntOpaque(Object &o, glong offset, gint x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(gint volatile *, address), x, __ATOMIC_RELAXED);
#else
            putInt(o, offset, x);
#endif
        }

        void VarHandle::putLong(Object &o, glong offset, glong x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(glong volatile *, address), x, __ATOMIC_SEQ_CST);
#elif __has_builtin(__builtin_memcpy)
            __builtin_memcpy(CORE_CAST(void*, address), &x, 8);
#else
            *CORE_CAST(glong volatile *, address) = x;
#endif
        }

        void VarHandle::putLongVolatile(Object &o, glong offset, glong x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(glong volatile *, address), x, __ATOMIC_RELEASE);
#else
            putLong(o, offset, x);
#endif
        }

        void VarHandle::putLongOpaque(Object &o, glong offset, glong x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_store_n)
            __atomic_store_n(CORE_CAST(glong volatile *, address), x, __ATOMIC_RELAXED);
#else
            putLong(o, offset, x);
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

        glong VarHandle::toUnsignedLong(gbyte n) { return n & 0xffL; }

        glong VarHandle::toUnsignedLong(gshort n) { return n & 0xffffL; }

        glong VarHandle::toUnsignedLong(gint n) { return n & 0xffffffffL; }

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
#if __has_builtin(__atomic_compare_exchange_n)
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gbyte volatile *, address), &expected, x, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
            return b ? expected : __atomic_load_n(CORE_CAST(gbyte volatile *, address), __ATOMIC_RELAXED);
#elif __has_builtin(__builtin_memcpy)
            gbyte oldValue = 0;
            __builtin_memcpy(&oldValue, CORE_CAST(void*, address), 1);
            if(oldValue == expected)
                __builtin_memcpy(CORE_CAST(void*, address), &x, 1);

            return oldValue;
#else
            gbyte oldValue = getByte(o, offset);
            if(oldValue == expected)
                putByte(o, offset + 1, x);

            return oldValue;
#endif
        }

        gbyte VarHandle::compareAndExchangeByteAcquire(Object &o, glong offset, gbyte expected, gbyte x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gbyte volatile *, address), &expected, x, false, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
            return b ? expected : __atomic_load_n(CORE_CAST(gbyte volatile *, address), __ATOMIC_ACQUIRE);
#else
            return compareAndExchangeByte(o, offset, expected, x);
#endif
        }

        gbyte VarHandle::compareAndExchangeByteRelease(Object &o, glong offset, gbyte expected, gbyte x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gbyte volatile *, address), &expected, x, false, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
            return b ? expected : __atomic_load_n(CORE_CAST(gbyte volatile *, address), __ATOMIC_RELAXED);
#else
            return compareAndExchangeByte(o, offset, expected, x);
#endif
        }

        gbool VarHandle::weakCompareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gbyte volatile *, address), &expected, x, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
#else
            return compareAndExchangeByte(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gbyte volatile *, address), &expected, x, true, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
#else
            return compareAndExchangeByte(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gbyte volatile *, address), &expected, x, true, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
#else
            return compareAndExchangeByte(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetBytePlain(Object &o, glong offset, gbyte expected, gbyte x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gbyte volatile *, address), &expected, x, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#else
            return compareAndExchangeByte(o, offset, expected, x) == expected;
#endif
        }

        gshort VarHandle::compareAndExchangeShort(Object &o, glong offset, gshort expected, gshort x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_compare_exchange_n)
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gshort volatile *, address), &expected, x, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
            return b ? expected : __atomic_load_n(CORE_CAST(gshort volatile *, address), __ATOMIC_RELAXED);
#elif __has_builtin(__builtin_memcpy)
            gshort oldValue = 0;
            __builtin_memcpy(&oldValue, CORE_CAST(void*, address), __ATOMIC_ACQUIRE);
            if(oldValue == expected)
                __builtin_memcpy(CORE_CAST(void*, address), &x, __ATOMIC_ACQUIRE);

            return oldValue;
#else
            gshort oldValue = getShort(o, offset);
            if(oldValue == expected)
                putShort(o, offset + 1, x);

            return oldValue;
#endif
        }

        gshort VarHandle::compareAndExchangeShortAcquire(Object &o, glong offset, gshort expected, gshort x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gshort volatile *, address), &expected, x, false, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
            return b ? expected : __atomic_load_n(CORE_CAST(gshort volatile *, address), __ATOMIC_ACQUIRE);
#else
            return compareAndExchangeShort(o, offset, expected, x);
#endif
        }

        gshort VarHandle::compareAndExchangeShortRelease(Object &o, glong offset, gshort expected, gshort x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gshort volatile *, address), &expected, x, false, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
            return b ? expected : __atomic_load_n(CORE_CAST(gshort volatile *, address), __ATOMIC_RELAXED);
#else
            return compareAndExchangeShort(o, offset, expected, x);
#endif
        }

        gbool VarHandle::weakCompareAndSetShort(Object &o, glong offset, gshort expected, gshort x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gshort volatile *, address), &expected, x, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
#else
            return compareAndExchangeShort(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gshort volatile *, address), &expected, x, true, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
#else
            return compareAndExchangeShort(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gshort volatile *, address), &expected, x, true, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
#else
            return compareAndExchangeShort(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetShortPlain(Object &o, glong offset, gshort expected, gshort x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gshort volatile *, address), &expected, x, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#else
            return compareAndExchangeShort(o, offset, expected, x) == expected;
#endif
        }


        gint VarHandle::compareAndExchangeInt(Object &o, glong offset, gint expected, gint x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_compare_exchange_n)
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gint volatile *, address), &expected, x, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
            return b ? expected : __atomic_load_n(CORE_CAST(gint volatile *, address), __ATOMIC_RELAXED);
#elif __has_builtin(__builtin_memcpy)
            gint oldValue = 0;
            __builtin_memcpy(&oldValue, CORE_CAST(void*, address), 4);
            if(oldValue == expected)
                __builtin_memcpy(CORE_CAST(void*, address), &x, 4);

            return oldValue;
#else
            gint oldValue = getInt(o, offset);
            if(oldValue == expected)
                putInt(o, offset + 1, x);

            return oldValue;
#endif
        }

        gint VarHandle::compareAndExchangeIntAcquire(Object &o, glong offset, gint expected, gint x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gint volatile *, address), &expected, x, false, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
            return b ? expected : __atomic_load_n(CORE_CAST(gint volatile *, address), __ATOMIC_ACQUIRE);
#else
            return compareAndExchangeInt(o, offset, expected, x);
#endif
        }

        gint VarHandle::compareAndExchangeIntRelease(Object &o, glong offset, gint expected, gint x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(gint volatile *, address), &expected, x, false, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
            return b ? expected : __atomic_load_n(CORE_CAST(gint volatile *, address), __ATOMIC_RELAXED);
#else
            return compareAndExchangeInt(o, offset, expected, x);
#endif
        }

        gbool VarHandle::weakCompareAndSetInt(Object &o, glong offset, gint expected, gint x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gint volatile *, address), &expected, x, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
#else
            return compareAndExchangeInt(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gint volatile *, address), &expected, x, true, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
#else
            return compareAndExchangeInt(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetIntRelease(Object &o, glong offset, gint expected, gint x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gint volatile *, address), &expected, x, true, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
#else
            return compareAndExchangeInt(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetIntPlain(Object &o, glong offset, gint expected, gint x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(gint volatile *, address), &expected, x, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#else
            return compareAndExchangeInt(o, offset, expected, x) == expected;
#endif
        }

        glong VarHandle::compareAndExchangeLong(Object &o, glong offset, glong expected, glong x) {
            glong address = o2l(o) + offset;
#if __has_builtin(__atomic_compare_exchange_n)
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(glong volatile *, address), &expected, x, false, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
            return b ? expected : __atomic_load_n(CORE_CAST(glong volatile *, address), __ATOMIC_RELAXED);
#elif __has_builtin(__builtin_memcpy)
            glong oldValue = 0;
            __builtin_memcpy(&oldValue, CORE_CAST(void*, address), 8);
            if(oldValue == expected)
                __builtin_memcpy(CORE_CAST(void*, address), &x, 8);

            return oldValue;
#else
            glong oldValue = getLong(o, offset);
            if(oldValue == expected)
                putLong(o, offset + 1, x);

            return oldValue;
#endif
        }

        glong VarHandle::compareAndExchangeLongAcquire(Object &o, glong offset, glong expected, glong x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(glong volatile *, address), &expected, x, false, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
            return b ? expected : __atomic_load_n(CORE_CAST(glong volatile *, address), __ATOMIC_ACQUIRE);
#else
            return compareAndExchangeLong(o, offset, expected, x);
#endif
        }

        glong VarHandle::compareAndExchangeLongRelease(Object &o, glong offset, glong expected, glong x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            gbool b = __atomic_compare_exchange_n(
                CORE_CAST(glong volatile *, address), &expected, x, false, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
            return b ? expected : __atomic_load_n(CORE_CAST(glong volatile *, address), __ATOMIC_RELAXED);
#else
            return compareAndExchangeLong(o, offset, expected, x);
#endif
        }

        gbool VarHandle::weakCompareAndSetLong(Object &o, glong offset, glong expected, glong x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(glong volatile *, address), &expected, x, true, __ATOMIC_RELAXED, __ATOMIC_RELAXED);
#else
            return compareAndExchangeLong(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(glong volatile *, address), &expected, x, true, __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE);
#else
            return compareAndExchangeLong(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetLongRelease(Object &o, glong offset, glong expected, glong x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(glong volatile *, address), &expected, x, true, __ATOMIC_RELEASE, __ATOMIC_RELAXED);
#else
            return compareAndExchangeLong(o, offset, expected, x) == expected;
#endif
        }

        gbool VarHandle::weakCompareAndSetLongPlain(Object &o, glong offset, glong expected, glong x) {
#if __has_builtin(__atomic_compare_exchange_n)
            glong address = o2l(o) + offset;
            return __atomic_compare_exchange_n(
                CORE_CAST(glong volatile *, address), &expected, x, true, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
#else
            return compareAndExchangeLong(o, offset, expected, x) == expected;
#endif
        }

        void VarHandle::loadFence() {
#if __has_builtin(__atomic_thread_fence)
            __atomic_thread_fence(__ATOMIC_ACQUIRE);
#endif
        }

        void VarHandle::storeFence() {
#if __has_builtin(__atomic_thread_fence)
            __atomic_thread_fence(__ATOMIC_RELEASE);
#endif
        }

        void VarHandle::fullFence() {
#if __has_builtin(__atomic_thread_fence)
            __atomic_thread_fence(__ATOMIC_SEQ_CST);
#endif
        }

        CORE_WARNING_POP
    } // misc
} // core
