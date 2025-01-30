//
// Created by brunshweeck on 8 juin 2024.
//

#include <core/IllegalArgumentException.h>
#include <core/OutOfMemoryError.h>
#include <core/Thread.h>
#include <core/misc/Cache.h>
#include <core/misc/Event.h>
#include <core/misc/MemoryHeap.h>
#include <core/misc/Unsafe.h>
#include <core/misc/VarHandle.h>
#include <core/time/LocalDateTime.h>
#include <core/util/Arrays.h>
#include <core/BooleanArray.h>
#include <core/LongArray.h>
#include <core/ShortArray.h>
#include <core/FloatArray.h>
#include <core/DoubleArray.h>

namespace core {
    using namespace time;
    using namespace util;

    namespace misc {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        static CORE_FAST gbyte b2byte(gbool b) { return b ? 1 : 0; }

        static CORE_FAST gbool b2bool(gbyte b) { return b != 0; }

        static CORE_FAST gshort c2s(gchar c) { return CORE_CAST(gshort, c); }

        static CORE_FAST gchar s2c(gshort s) { return CORE_CAST(gchar, s); }

        static gint f2i(gfloat f) { return Float::toRawIntBits(f); }

        static gfloat i2f(gint i) { return Float::fromIntBits(i); }

        static glong d2l(gdouble d) { return Double::toRawLongBits(d); }

        static gdouble l2d(glong l) { return Double::fromLongBits(l); }

        static glong o2l(Object const &o) { return o == null ? 0LL : CORE_CAST(glong, &o); }

        static gint o2i(Object const &o) { return CORE_CAST(gint, o2l(o)); }

        static Object &l2o(glong l) { return (l == 0L) ? null : *CORE_CAST(Class<Object>::Pointer, l); }

        static Object &i2o(gint i) { return l2o(Integer::toUnsignedLong(i)); }

        CORE_ALIAS(VAL, Class<Object>::Pointer);

        gint Unsafe::getInt(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::getInt(null, tryGetDirectAccess(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putInt(Object &o, glong offset, gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putInt(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::getReference(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return l2o(getAddress(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putReference(Object &o, glong offset, Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                putAddress(o, offset, o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getBoolean(Object const &o, glong offset) {
            try {
                return b2bool(getByte(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putBoolean(Object &o, glong offset, gbool x) {
            try {
                putByte(o, offset, b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getByte(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::getByte(null, tryGetDirectAccess(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putByte(Object &o, glong offset, gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putByte(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getShort(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::getShort(null, tryGetDirectAccess(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putShort(Object &o, glong offset, gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putShort(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getChar(Object const &o, glong offset) {
            try {
                return s2c(getShort(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putChar(Object &o, glong offset, gchar x) {
            try {
                putShort(o, offset, c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getLong(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::getLong(null, tryGetDirectAccess(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putLong(Object &o, glong offset, glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putLong(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getFloat(Object const &o, glong offset) {
            try {
                return i2f(getInt(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putFloat(Object &o, glong offset, gfloat x) {
            try {
                putInt(o, offset, f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getDouble(Object const &o, glong offset) {
            try {
                return l2d(getLong(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putDouble(Object &o, glong offset, gdouble x) {
            try {
                putLong(o, offset, d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAddress(Object const &o, glong offset) {
            try {
                return addressSize() == 4
                           ? Integer::toUnsignedLong(getInt(o, offset))
                           : getLong(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putAddress(Object &o, glong offset, glong x) {
            try {
                VarHandle::checkNativeAddress(x);
                addressSize() == 4
                    ? putInt(o, offset, CORE_CAST(gint, x))
                    : putLong(o, offset, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getByte(glong address) {
            try {
                return getByte(null, address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putByte(glong address, gbyte x) {
            try {
                return putByte(null, address, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getShort(glong address) {
            try {
                return getShort(null, address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putShort(glong address, gshort x) {
            try {
                return putShort(null, address, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getChar(glong address) {
            try {
                return getChar(null, address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putChar(glong address, gchar x) {
            try {
                putChar(null, address, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getInt(glong address) {
            try {
                return getInt(null, address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putInt(glong address, gint x) {
            try {
                return putInt(null, address, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getLong(glong address) {
            try {
                return getLong(null, address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putLong(glong address, glong x) {
            try {
                return putLong(null, address, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getFloat(glong address) {
            try {
                return getFloat(null, address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putFloat(glong address, gfloat x) {
            try {
                return putFloat(null, address, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getDouble(glong address) {
            try {
                return getDouble(null, address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putDouble(glong address, gdouble x) {
            try {
                return putDouble(null, address, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAddress(glong address) {
            try {
                return getAddress(null, address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putAddress(glong address, glong x) {
            try {
                return putAddress(null, address, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::allocateMemory(glong bytes) {
            bytes = MemoryHeap::alignToHeapWordSize(bytes > 0 && bytes < Long::MAX_VALUE ? bytes + 1 : bytes);
            try {
                MemoryHeap::checkSize(bytes);
            } catch (Exception const &ex) { ex.throws($ftrace()); }

            if (bytes == 0LL)
                return 0LL;

            glong p = MemoryHeap::INSTANCE.allocateMemory(bytes);
            if (p == 0LL)
                OutOfMemoryError("Unable to allocate "_S + bytes + " bytes"_S).throws($ftrace());

            return p;
        }

        glong Unsafe::reallocateMemory(glong address, glong bytes) {
            bytes = MemoryHeap::alignToHeapWordSize(bytes + 1);

            try {
                VarHandle::checkPointer(null, address);
                MemoryHeap::checkSize(bytes);
            } catch (Exception const &ex) { ex.throws($ftrace()); }

            if (bytes == 0LL) {
                freeMemory(address);
                return 0LL;
            }

            glong p = (address == 0LL)
                          ? MemoryHeap::INSTANCE.allocateMemory(bytes)
                          : MemoryHeap::INSTANCE.reallocateMemory(address, bytes);
            if (p == 0LL)
                OutOfMemoryError("Unable to allocate "_S + bytes + " bytes"_S).throws($ftrace());

            return p;
        }

        void Unsafe::setMemory(Object &o, glong offset, glong bytes, gbyte value) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::checkSize(bytes);
                if (bytes == 0LL)
                    return;

                MemoryHeap::INSTANCE.setMemory(null, tryGetDirectAccess(o, offset), bytes, value);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::setMemory(glong address, glong bytes, gbyte value) {
            try {
                return setMemory(null, address, bytes, value);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::copyMemory(Object const &srcBase, glong srcOffset,
                                Object &destBase, glong destOffset, glong bytes) {
            try {
                VarHandle::checkOffset(srcBase, srcOffset);
                VarHandle::checkOffset(destBase, destOffset);
                VarHandle::checkSize(bytes);
                if (bytes == 0LL) {
                    return;
                }
                MemoryHeap::INSTANCE.copyMemory(null, tryGetDirectAccess(srcBase, srcOffset),
                                                null, tryGetDirectAccess(destBase, destOffset),
                                                bytes);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::copyMemory(glong srcAddress, glong destAddress, glong bytes) {
            try {
                copyMemory(null, srcAddress, null, destAddress, bytes);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::copySwapMemory(Object const &srcBase, glong srcOffset,
                                    Object &destBase, glong destOffset, glong bytes,
                                    glong elemSize) {
            try {
                VarHandle::checkSize(bytes);
            } catch (Exception const &ex) { ex.throws($ftrace()); }

            if (elemSize != 2 && elemSize != 4 && elemSize != 8)
                IllegalArgumentException("Invalid data size for swapping: "_S + elemSize).throws(
                    $ftrace());

            if (bytes % elemSize != 0)
                IllegalArgumentException("Invalid number of bytes"_S).throws($ftrace());

            try {
                VarHandle::checkOffset(srcBase, srcOffset);
                VarHandle::checkOffset(destBase, destOffset);
                if (bytes == 0LL)
                    return;

                MemoryHeap::INSTANCE.copySwapMemory(null, tryGetDirectAccess(srcBase, srcOffset),
                                                    null, tryGetDirectAccess(destBase, destOffset),
                                                    bytes, elemSize);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::copySwapMemory(glong srcAddress, glong destAddress, glong bytes, glong elemSize) {
            try {
                copySwapMemory(null, srcAddress, null, destAddress, bytes, elemSize);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::freeMemory(glong address) {
            try {
                VarHandle::checkPointer(null, address);
                if (address == 0LL)
                    return;

                if (!MemoryHeap::isAbleForAllocate())
                    return;

                MemoryHeap::INSTANCE.freeMemory(address);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::addressSize() { return ADDRESS_SIZE; }

        gint Unsafe::pageSize() { return 1024; }

        gbool Unsafe::compareAndSetReference(Object &o, glong offset,
                                             Object const &expected,
                                             Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? compareAndSetInt(null, tryGetDirectAccess(o, offset),
                                              o2i(expected), o2i(x))
                           : compareAndSetLong(null, tryGetDirectAccess(o, offset),
                                               o2l(expected), o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::compareAndExchangeReference(Object &o, glong offset,
                                                    Object const &expected,
                                                    Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? i2o(compareAndExchangeInt(null, tryGetDirectAccess(o, offset),
                                                       o2i(expected), o2i(x)))
                           : l2o(compareAndExchangeLong(null, tryGetDirectAccess(o, offset),
                                                        o2l(expected), o2l(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::compareAndExchangeReferenceAcquire(Object &o, glong offset,
                                                           Object const &expected,
                                                           Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? i2o(compareAndExchangeIntAcquire(null, tryGetDirectAccess(o, offset),
                                                              o2i(expected), o2i(x)))
                           : l2o(compareAndExchangeLongAcquire(null, tryGetDirectAccess(o, offset),
                                                               o2l(expected), o2l(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::compareAndExchangeReferenceRelease(Object &o, glong offset,
                                                           Object const &expected,
                                                           Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? i2o(compareAndExchangeIntRelease(null, tryGetDirectAccess(o, offset),
                                                              o2i(expected), o2i(x)))
                           : l2o(compareAndExchangeLongRelease(null, tryGetDirectAccess(o, offset),
                                                               o2l(expected), o2l(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetReferencePlain(Object &o, glong offset,
                                                      Object const &expected,
                                                      Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? weakCompareAndSetIntPlain(null, tryGetDirectAccess(o, offset),
                                                       o2i(expected), o2i(x))
                           : weakCompareAndSetLongPlain(null, tryGetDirectAccess(o, offset),
                                                        o2l(expected), o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetReferenceAcquire(Object &o, glong offset,
                                                        Object const &expected,
                                                        Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset),
                                                         o2i(expected), o2i(x))
                           : weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset),
                                                          o2l(expected), o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetReferenceRelease(Object &o, glong offset,
                                                        Object const &expected,
                                                        Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset),
                                                         o2i(expected), o2i(x))
                           : weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset),
                                                          o2l(expected), o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetReference(Object &o, glong offset,
                                                 Object const &expected,
                                                 Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? weakCompareAndSetInt(null, tryGetDirectAccess(o, offset),
                                                  o2i(expected), o2i(x))
                           : weakCompareAndSetLong(null, tryGetDirectAccess(o, offset),
                                                   o2l(expected), o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndSetInt(Object &o, glong offset,
                                       gint expected,
                                       gint x) {
            return compareAndExchangeInt(null, tryGetDirectAccess(o, offset), expected, x) == expected;
        }

        gint Unsafe::compareAndExchangeInt(Object &o, glong offset,
                                           gint expected,
                                           gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeInt(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::compareAndExchangeIntAcquire(Object &o, glong offset,
                                                  gint expected,
                                                  gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeIntAcquire(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::compareAndExchangeIntRelease(Object &o, glong offset,
                                                  gint expected,
                                                  gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeIntRelease(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetIntPlain(Object &o, glong offset,
                                                gint expected,
                                                gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetIntPlain(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetIntAcquire(Object &o, glong offset,
                                                  gint expected,
                                                  gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetIntRelease(Object &o, glong offset,
                                                  gint expected,
                                                  gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetInt(Object &o, glong offset,
                                           gint expected,
                                           gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetInt(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndSetByte(Object &o, glong offset,
                                        gbyte expected,
                                        gbyte x) {
            try {
                return compareAndExchangeByte(null, tryGetDirectAccess(o, offset), expected, x) == expected;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::compareAndExchangeByte(Object &o, glong offset,
                                             gbyte expected,
                                             gbyte x) {
            glong wordOffset = offset & ~3;
            gint shift = CORE_CAST(gint, (offset & 3) << 3);
            if (BIG_ENDIAN) {
                shift = 24 - shift;
            }
            gint mask = 0xFF << shift;
            gint maskedExpected = (expected & 0xFF) << shift;
            gint maskedX = (x & 0xFF) << shift;
            gint fullWord;
            do {
                fullWord = getIntVolatile(o, wordOffset);
                if ((fullWord & mask) != maskedExpected)
                    return CORE_CAST(gbyte, (fullWord & mask) >> shift);
            } while (!weakCompareAndSetInt(o, wordOffset, fullWord, (fullWord & ~mask) | maskedX));

            return expected;
        }

        gbyte Unsafe::compareAndExchangeByteAcquire(Object &o, glong offset,
                                                    gbyte expected,
                                                    gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeByteAcquire(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::compareAndExchangeByteRelease(Object &o, glong offset,
                                                    gbyte expected,
                                                    gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeByteRelease(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetBytePlain(Object &o, glong offset,
                                                 gbyte expected,
                                                 gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetBytePlain(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetByteAcquire(Object &o, glong offset,
                                                   gbyte expected,
                                                   gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetByteAcquire(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetByteRelease(Object &o, glong offset,
                                                   gbyte expected,
                                                   gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetByteRelease(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetByte(Object &o, glong offset,
                                            gbyte expected,
                                            gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetByte(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndSetShort(Object &o, glong offset,
                                         gshort expected,
                                         gshort x) {
            try {
                return compareAndExchangeShort(null, tryGetDirectAccess(o, offset), expected, x) == expected;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::compareAndExchangeShort(Object &o, glong offset,
                                               gshort expected,
                                               gshort x) {
            if ((offset & 3) == 3) {
                IllegalArgumentException("Update spans the word, not supported"_S).throws($ftrace());
            }
            glong wordOffset = offset & ~3;
            gint shift = CORE_CAST(gint, (offset & 3) << 3);
            if (BIG_ENDIAN) {
                shift = 16 - shift;
            }
            gint mask = 0xFFFF << shift;
            gint maskedExpected = (expected & 0xFFFF) << shift;
            gint maskedX = (x & 0xFFFF) << shift;
            gint fullWord;
            do {
                fullWord = getIntVolatile(o, wordOffset);
                if ((fullWord & mask) != maskedExpected) {
                    return CORE_CAST(gshort, (fullWord & mask) >> shift);
                }
            } while (!weakCompareAndSetInt(o, wordOffset, fullWord, (fullWord & ~mask) | maskedX));

            return expected;
        }

        gshort Unsafe::compareAndExchangeShortAcquire(Object &o, glong offset,
                                                      gshort expected,
                                                      gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeShortAcquire(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::compareAndExchangeShortRelease(Object &o, glong offset,
                                                      gshort expected,
                                                      gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeShortRelease(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetShortPlain(Object &o, glong offset,
                                                  gshort expected,
                                                  gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetShortPlain(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetShortAcquire(Object &o, glong offset,
                                                    gshort expected,
                                                    gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetShortAcquire(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetShortRelease(Object &o, glong offset,
                                                    gshort expected,
                                                    gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetShortRelease(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetShort(Object &o, glong offset,
                                             gshort expected,
                                             gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetShort(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndSetChar(Object &o, glong offset,
                                        gchar expected,
                                        gchar x) {
            try {
                return compareAndSetShort(null, tryGetDirectAccess(o, offset),
                                          c2s(expected), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::compareAndExchangeChar(Object &o, glong offset,
                                             gchar expected,
                                             gchar x) {
            try {
                return s2c(compareAndExchangeShort(null, tryGetDirectAccess(o, offset),
                                                   c2s(expected), c2s(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::compareAndExchangeCharAcquire(Object &o, glong offset,
                                                    gchar expected,
                                                    gchar x) {
            try {
                return s2c(compareAndExchangeShortAcquire(null, tryGetDirectAccess(o, offset),
                                                          c2s(expected), c2s(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::compareAndExchangeCharRelease(Object &o, glong offset,
                                                    gchar expected,
                                                    gchar x) {
            try {
                return s2c(compareAndExchangeShortRelease(null, tryGetDirectAccess(o, offset),
                                                          c2s(expected), c2s(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetCharPlain(Object &o, glong offset,
                                                 gchar expected,
                                                 gchar x) {
            try {
                return weakCompareAndSetShortPlain(null, tryGetDirectAccess(o, offset),
                                                   c2s(expected), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetCharAcquire(Object &o, glong offset,
                                                   gchar expected,
                                                   gchar x) {
            try {
                return weakCompareAndSetShortAcquire(null, tryGetDirectAccess(o, offset),
                                                     c2s(expected), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetCharRelease(Object &o, glong offset,
                                                   gchar expected,
                                                   gchar x) {
            try {
                return weakCompareAndSetShortRelease(null, tryGetDirectAccess(o, offset),
                                                     c2s(expected), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetChar(Object &o, glong offset,
                                            gchar expected,
                                            gchar x) {
            try {
                return weakCompareAndSetShort(null, tryGetDirectAccess(o, offset),
                                              c2s(expected), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndSetBoolean(Object &o, glong offset,
                                           gbool expected,
                                           gbool x) {
            try {
                return compareAndSetByte(null, tryGetDirectAccess(o, offset), b2byte(expected), b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndExchangeBoolean(Object &o, glong offset,
                                                gbool expected,
                                                gbool x) {
            try {
                return b2bool(compareAndExchangeByte(null, tryGetDirectAccess(o, offset),
                                                     b2byte(expected), b2byte(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndExchangeBooleanAcquire(Object &o, glong offset,
                                                       gbool expected,
                                                       gbool x) {
            try {
                return b2bool(compareAndExchangeByteAcquire(null, tryGetDirectAccess(o, offset),
                                                            b2byte(expected), b2byte(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndExchangeBooleanRelease(Object &o, glong offset,
                                                       gbool expected,
                                                       gbool x) {
            try {
                return b2bool(compareAndExchangeByteRelease(null, tryGetDirectAccess(o, offset),
                                                            b2byte(expected), b2byte(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetBooleanPlain(Object &o, glong offset,
                                                    gbool expected,
                                                    gbool x) {
            try {
                return weakCompareAndSetBytePlain(null, tryGetDirectAccess(o, offset),
                                                  b2byte(expected), b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetBooleanAcquire(Object &o, glong offset,
                                                      gbool expected,
                                                      gbool x) {
            try {
                return weakCompareAndSetByteAcquire(null, tryGetDirectAccess(o, offset),
                                                    b2byte(expected), b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetBooleanRelease(Object &o, glong offset,
                                                      gbool expected,
                                                      gbool x) {
            try {
                return weakCompareAndSetByteRelease(null, tryGetDirectAccess(o, offset),
                                                    b2byte(expected), b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetBoolean(Object &o, glong offset,
                                               gbool expected,
                                               gbool x) {
            try {
                return weakCompareAndSetByte(null, tryGetDirectAccess(o, offset),
                                             b2byte(expected), b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndSetFloat(Object &o, glong offset,
                                         gfloat expected,
                                         gfloat x) {
            try {
                return compareAndSetInt(null, tryGetDirectAccess(o, offset),
                                        f2i(expected), f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::compareAndExchangeFloat(Object &o, glong offset,
                                               gfloat expected,
                                               gfloat x) {
            try {
                return i2f(compareAndExchangeInt(null, tryGetDirectAccess(o, offset),
                                                 f2i(expected), f2i(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::compareAndExchangeFloatAcquire(Object &o, glong offset,
                                                      gfloat expected,
                                                      gfloat x) {
            try {
                return i2f(compareAndExchangeIntAcquire(null, tryGetDirectAccess(o, offset),
                                                        f2i(expected), f2i(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::compareAndExchangeFloatRelease(Object &o, glong offset,
                                                      gfloat expected,
                                                      gfloat x) {
            try {
                return i2f(compareAndExchangeIntRelease(null, tryGetDirectAccess(o, offset),
                                                        f2i(expected), f2i(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetFloatPlain(Object &o, glong offset,
                                                  gfloat expected,
                                                  gfloat x) {
            try {
                return weakCompareAndSetIntPlain(null, tryGetDirectAccess(o, offset),
                                                 f2i(expected), f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetFloatAcquire(Object &o, glong offset,
                                                    gfloat expected,
                                                    gfloat x) {
            try {
                return weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset),
                                                   f2i(expected), f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetFloatRelease(Object &o, glong offset,
                                                    gfloat expected,
                                                    gfloat x) {
            try {
                return weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset),
                                                   f2i(expected), f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetFloat(Object &o, glong offset,
                                             gfloat expected,
                                             gfloat x) {
            try {
                return weakCompareAndSetInt(null, tryGetDirectAccess(o, offset),
                                            f2i(expected), f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndSetDouble(Object &o, glong offset,
                                          gdouble expected,
                                          gdouble x) {
            try {
                return compareAndSetLong(null, tryGetDirectAccess(o, offset), d2l(expected), d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::compareAndExchangeDouble(Object &o, glong offset,
                                                 gdouble expected,
                                                 gdouble x) {
            try {
                return l2d(compareAndExchangeLong(null, tryGetDirectAccess(o, offset),
                                                  d2l(expected), d2l(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::compareAndExchangeDoubleAcquire(Object &o, glong offset,
                                                        gdouble expected,
                                                        gdouble x) {
            try {
                return l2d(compareAndExchangeLongAcquire(null, tryGetDirectAccess(o, offset),
                                                         d2l(expected), d2l(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::compareAndExchangeDoubleRelease(Object &o, glong offset,
                                                        gdouble expected,
                                                        gdouble x) {
            try {
                return l2d(compareAndExchangeLongRelease(null, tryGetDirectAccess(o, offset),
                                                         d2l(expected), d2l(x)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetDoublePlain(Object &o, glong offset,
                                                   gdouble expected,
                                                   gdouble x) {
            try {
                return weakCompareAndSetLongPlain(null, tryGetDirectAccess(o, offset),
                                                  d2l(expected), d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetDoubleAcquire(Object &o, glong offset,
                                                     gdouble expected,
                                                     gdouble x) {
            try {
                return weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset),
                                                    d2l(expected), d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetDoubleRelease(Object &o, glong offset,
                                                     gdouble expected,
                                                     gdouble x) {
            try {
                return weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset),
                                                    d2l(expected), d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetDouble(Object &o, glong offset,
                                              gdouble expected,
                                              gdouble x) {
            try {
                return weakCompareAndSetLong(null, tryGetDirectAccess(o, offset),
                                             d2l(expected), d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::compareAndSetLong(Object &o, glong offset,
                                        glong expected,
                                        glong x) {
            try {
                return compareAndExchangeLong(null, tryGetDirectAccess(o, offset), expected, x) == expected;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::compareAndExchangeLong(Object &o, glong offset,
                                             glong expected,
                                             glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeLong(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::compareAndExchangeLongAcquire(Object &o, glong offset,
                                                    glong expected,
                                                    glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeLongAcquire(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::compareAndExchangeLongRelease(Object &o, glong offset,
                                                    glong expected,
                                                    glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::compareAndExchangeLongRelease(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetLongPlain(Object &o, glong offset,
                                                 glong expected,
                                                 glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetLongPlain(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetLongAcquire(Object &o, glong offset,
                                                   glong expected,
                                                   glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetLongRelease(Object &o, glong offset,
                                                   glong expected,
                                                   glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::weakCompareAndSetLong(Object &o, glong offset,
                                            glong expected,
                                            glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::weakCompareAndSetLong(null, tryGetDirectAccess(o, offset), expected, x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::getReferenceVolatile(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? i2o(getIntVolatile(o, offset))
                           : l2o(getLongVolatile(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putReferenceVolatile(Object &o, glong offset, Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                (ADDRESS_SIZE == 4)
                    ? putIntVolatile(null, tryGetDirectAccess(o, offset), o2i(x))
                    : putLongVolatile(null, tryGetDirectAccess(o, offset), o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getIntVolatile(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::getIntVolatile(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putIntVolatile(Object &o, glong offset, gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putIntVolatile(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getBooleanVolatile(Object const &o, glong offset) {
            try {
                return b2bool(getByteVolatile(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putBooleanVolatile(Object &o, glong offset, gbool x) {
            try {
                putByteVolatile(null, tryGetDirectAccess(o, offset), b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getByteVolatile(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::getByteVolatile(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putByteVolatile(Object &o, glong offset, gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::putByteVolatile(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getShortVolatile(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::getShortVolatile(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putShortVolatile(Object &o, glong offset, gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putShortVolatile(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getCharVolatile(Object const &o, glong offset) {
            try {
                return s2c(getShortVolatile(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putCharVolatile(Object &o, glong offset, gchar x) {
            try {
                putShortVolatile(null, tryGetDirectAccess(o, offset), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getLongVolatile(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return VarHandle::getLongVolatile(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putLongVolatile(Object &o, glong offset, glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putLongVolatile(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getFloatVolatile(Object const &o, glong offset) {
            try {
                return i2f(getIntVolatile(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putFloatVolatile(Object &o, glong offset, gfloat x) {
            try {
                putIntVolatile(null, tryGetDirectAccess(o, offset), f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getDoubleVolatile(Object const &o, glong offset) {
            try {
                return l2d(getLongVolatile(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putDoubleVolatile(Object &o, glong offset, gdouble x) {
            try {
                putLongVolatile(null, tryGetDirectAccess(o, offset), d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::getReferenceAcquire(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return ADDRESS_SIZE == 4
                           ? i2o(getIntAcquire(o, offset))
                           : l2o(getLongAcquire(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getBooleanAcquire(Object const &o, glong offset) {
            try {
                return b2bool(getByteAcquire(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getByteAcquire(Object const &o, glong offset) {
            try {
                return VarHandle::getByteVolatile(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getShortAcquire(Object const &o, glong offset) {
            try {
                return VarHandle::getShortVolatile(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getCharAcquire(Object const &o, glong offset) {
            try {
                return s2c(getShortAcquire(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getIntAcquire(Object const &o, glong offset) {
            try {
                return VarHandle::getIntVolatile(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getLongAcquire(Object const &o, glong offset) {
            try {
                return VarHandle::getLongVolatile(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getFloatAcquire(Object const &o, glong offset) {
            try {
                return i2f(getIntAcquire(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getDoubleAcquire(Object const &o, glong offset) {
            try {
                return l2d(getLongAcquire(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putReferenceRelease(Object &o, glong offset, Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                (ADDRESS_SIZE == 4)
                    ? putIntRelease(null, tryGetDirectAccess(o, offset), o2i(x))
                    : putLongRelease(null, tryGetDirectAccess(o, offset), o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putBooleanRelease(Object &o, glong offset, gbool x) {
            try {
                putByteRelease(null, tryGetDirectAccess(o, offset), b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putByteRelease(Object &o, glong offset, gbyte x) {
            try {
                putByteVolatile(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putShortRelease(Object &o, glong offset, gshort x) {
            try {
                putShortVolatile(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putCharRelease(Object &o, glong offset, gchar x) {
            try {
                putShortRelease(null, tryGetDirectAccess(o, offset), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putIntRelease(Object &o, glong offset, gint x) {
            try {
                putIntVolatile(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putLongRelease(Object &o, glong offset, glong x) {
            try {
                putLongVolatile(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putFloatRelease(Object &o, glong offset, gfloat x) {
            try {
                putIntRelease(null, tryGetDirectAccess(o, offset), f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putDoubleRelease(Object &o, glong offset, gdouble x) {
            try {
                putLongRelease(null, tryGetDirectAccess(o, offset), d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::getReferenceOpaque(Object const &o, glong offset) {
            try {
                VarHandle::checkOffset(o, offset);
                return (ADDRESS_SIZE == 4)
                           ? i2o(getIntOpaque(o, offset))
                           : l2o(getLongOpaque(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getBooleanOpaque(Object const &o, glong offset) {
            try {
                return b2bool(getByteOpaque(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getByteOpaque(Object const &o, glong offset) {
            try {
                return VarHandle::getByteOpaque(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getShortOpaque(Object const &o, glong offset) {
            try {
                return VarHandle::getShortOpaque(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getCharOpaque(Object const &o, glong offset) {
            try {
                return s2c(getShortOpaque(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getIntOpaque(Object const &o, glong offset) {
            try {
                return VarHandle::getIntOpaque(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getLongOpaque(Object const &o, glong offset) {
            try {
                return VarHandle::getLongOpaque(o, offset);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getFloatOpaque(Object const &o, glong offset) {
            try {
                return i2f(getIntOpaque(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getDoubleOpaque(Object const &o, glong offset) {
            try {
                return l2d(getLongOpaque(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putReferenceOpaque(Object &o, glong offset, Object &x) {
            try {
                VarHandle::checkOffset(o, offset);
                (ADDRESS_SIZE == 4)
                    ? putIntOpaque(null, tryGetDirectAccess(o, offset), o2i(x))
                    : putLongOpaque(null, tryGetDirectAccess(o, offset), o2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putBooleanOpaque(Object &o, glong offset, gbool x) {
            try {
                putByteOpaque(null, tryGetDirectAccess(o, offset), b2byte(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putByteOpaque(Object &o, glong offset, gbyte x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putByteOpaque(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putShortOpaque(Object &o, glong offset, gshort x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putShortOpaque(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putCharOpaque(Object &o, glong offset, gchar x) {
            try {
                putShortOpaque(null, tryGetDirectAccess(o, offset), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putIntOpaque(Object &o, glong offset, gint x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putIntOpaque(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putLongOpaque(Object &o, glong offset, glong x) {
            try {
                VarHandle::checkOffset(o, offset);
                VarHandle::putLongOpaque(null, tryGetDirectAccess(o, offset), x);
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putFloatOpaque(Object &o, glong offset, gfloat x) {
            try {
                putIntOpaque(null, tryGetDirectAccess(o, offset), f2i(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putDoubleOpaque(Object &o, glong offset, gdouble x) {
            try {
                putLongOpaque(null, tryGetDirectAccess(o, offset), d2l(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::unpark(Object &thread) {
            if (thread == null)
                return;
            CORE_ASSERT(Class<Thread>::hasInstance(thread));
            Thread::BLOCKER volatile blocker = CORE_XCAST(Thread, thread).blocker;
            Thread::CACHE volatile event = CORE_XCAST(Thread, thread).event;
            Event::unpark(thread, CORE_CAST(glong, blocker), CORE_CAST(glong, event));
        }

        void Unsafe::park(gbool isAbsolute, glong time) {
            glong timeout = 0;
            if (isAbsolute) {
                LocalDateTime ldt = LocalDateTime::now();
                glong timestamp = ldt.toEpochSecond();
                timestamp *= 1000;
                timestamp += ldt.nano() / 1000000;
                if (ldt.nano() % 1000000 > 499999)
                    timestamp += 1;
                timeout = time - timestamp;
            } else {
                timeout = time;
            }
            Thread &thread = Thread::currentThread();
            Thread::BLOCKER volatile blocker = CORE_XCAST(Thread, thread).blocker;
            Thread::CACHE volatile event = CORE_XCAST(Thread, thread).event;
            Event::park(thread, CORE_CAST(glong, blocker), CORE_CAST(glong, event), timeout);
        }

        gint Unsafe::getAndAddInt(Object &o, glong offset, gint delta) {
            gint v;
            do {
                v = getIntVolatile(o, offset);
            } while (!weakCompareAndSetInt(null, tryGetDirectAccess(o, offset), v, v + delta));
            return v;
        }

        gint Unsafe::getAndAddIntRelease(Object &o, glong offset, gint delta) {
            gint v;
            do {
                v = getInt(o, offset);
            } while (!weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset), v, v + delta));
            return v;
        }

        gint Unsafe::getAndAddIntAcquire(Object &o, glong offset, gint delta) {
            gint v;
            do {
                v = getIntAcquire(o, offset);
            } while (!weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset), v, v + delta));
            return v;
        }

        glong Unsafe::getAndAddLong(Object &o, glong offset, glong delta) {
            glong v;
            do {
                v = getLongVolatile(o, offset);
            } while (!weakCompareAndSetLong(null, tryGetDirectAccess(o, offset), v, v + delta));
            return v;
        }

        glong Unsafe::getAndAddLongRelease(Object &o, glong offset, glong delta) {
            glong v;
            do {
                v = getLong(o, offset);
            } while (!weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset), v, v + delta));
            return v;
        }

        glong Unsafe::getAndAddLongAcquire(Object &o, glong offset, glong delta) {
            glong v;
            do {
                v = getLongAcquire(o, offset);
            } while (!weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset), v, v + delta));
            return v;
        }

        gbyte Unsafe::getAndAddByte(Object &o, glong offset, gbyte delta) {
            gbyte v;
            do {
                v = getByteVolatile(o, offset);
            } while (!weakCompareAndSetByte(null, tryGetDirectAccess(o, offset), v, CORE_CAST(gbyte, v + delta)));
            return v;
        }

        gbyte Unsafe::getAndAddByteRelease(Object &o, glong offset, gbyte delta) {
            gbyte v;
            do {
                v = getByte(o, offset);
            } while (!weakCompareAndSetByteRelease(null, tryGetDirectAccess(o, offset), v, CORE_CAST(gbyte, v + delta)))
            ;
            return v;
        }

        gbyte Unsafe::getAndAddByteAcquire(Object &o, glong offset, gbyte delta) {
            gbyte v;
            do {
                v = getByteAcquire(o, offset);
            } while (!weakCompareAndSetByteAcquire(null, tryGetDirectAccess(o, offset), v, CORE_CAST(gbyte, v + delta)))
            ;
            return v;
        }

        gshort Unsafe::getAndAddShort(Object &o, glong offset, gshort delta) {
            gshort v;
            do {
                v = getShortVolatile(o, offset);
            } while (!weakCompareAndSetShort(null, tryGetDirectAccess(o, offset), v, CORE_CAST(gshort, v + delta)));
            return v;
        }

        gshort Unsafe::getAndAddShortRelease(Object &o, glong offset, gshort delta) {
            gshort v;
            do {
                v = getShort(o, offset);
            } while (!weakCompareAndSetShortRelease(null, tryGetDirectAccess(o, offset), v,
                                                    CORE_CAST(gshort, v + delta)));
            return v;
        }

        gshort Unsafe::getAndAddShortAcquire(Object &o, glong offset, gshort delta) {
            gshort v;
            do {
                v = getShortAcquire(o, offset);
            } while (!weakCompareAndSetShortAcquire(null, tryGetDirectAccess(o, offset), v,
                                                    CORE_CAST(gshort, v + delta)));
            return v;
        }

        gfloat Unsafe::getAndAddFloat(Object &o, glong offset, gfloat delta) {
            try {
                gint expectedBits;
                gfloat v;
                do {
                    // Load and CAS with the raw bits to avoid issues with NaNs and
                    // possible bit conversion from signaling NaNs to quiet NaNs that
                    // may result in the loop not terminating.
                    expectedBits = getIntVolatile(o, offset);
                    v = i2f(expectedBits);
                } while (!weakCompareAndSetInt(null, tryGetDirectAccess(o, offset), expectedBits, f2i((v + delta))));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getAndAddFloatRelease(Object &o, glong offset, gfloat delta) {
            try {
                gint expectedBits;
                gfloat v;
                do {
                    // Load and CAS with the raw bits to avoid issues with NaNs and
                    // possible bit conversion from signaling NaNs to quiet NaNs that
                    // may result in the loop not terminating.
                    expectedBits = getInt(o, offset);
                    v = i2f(expectedBits);
                } while (!weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset), expectedBits,
                                                      f2i((v + delta))));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getAndAddFloatAcquire(Object &o, glong offset, gfloat delta) {
            try {
                gint expectedBits;
                gfloat v;
                do {
                    // Load and CAS with the raw bits to avoid issues with NaNs and
                    // possible bit conversion from signaling NaNs to quiet NaNs that
                    // may result in the loop not terminating.
                    expectedBits = getIntAcquire(o, offset);
                    v = i2f(expectedBits);
                } while (!weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset), expectedBits,
                                                      f2i(v + delta)));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getAndAddDouble(Object &o, glong offset, gdouble delta) {
            try {
                glong expectedBits;
                gdouble v;
                do {
                    // Load and CAS with the raw bits to avoid issues with NaNs and
                    // possible bit conversion from signaling NaNs to quiet NaNs that
                    // may result in the loop not terminating.
                    expectedBits = getLongVolatile(o, offset);
                    v = l2d(expectedBits);
                } while (!weakCompareAndSetLong(null, tryGetDirectAccess(o, offset), expectedBits, d2l(v + delta)));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getAndAddDoubleRelease(Object &o, glong offset, gdouble delta) {
            try {
                glong expectedBits;
                gdouble v;
                do {
                    // Load and CAS with the raw bits to avoid issues with NaNs and
                    // possible bit conversion from signaling NaNs to quiet NaNs that
                    // may result in the loop not terminating.
                    expectedBits = getLong(o, offset);
                    v = l2d(expectedBits);
                } while (!weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset), expectedBits,
                                                       d2l(v + delta)));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getAndAddDoubleAcquire(Object &o, glong offset, gdouble delta) {
            try {
                glong expectedBits;
                gdouble v;
                do {
                    // Load and CAS with the raw bits to avoid issues with NaNs and
                    // possible bit conversion from signaling NaNs to quiet NaNs that
                    // may result in the loop not terminating.
                    expectedBits = getLongAcquire(o, offset);
                    v = l2d(expectedBits);
                } while (!weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset), expectedBits,
                                                       d2l(v + delta)));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndSetInt(Object &o, glong offset, gint newValue) {
            try {
                gint v;
                do {
                    v = getIntVolatile(o, offset);
                } while (!weakCompareAndSetInt(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndSetIntRelease(Object &o, glong offset, gint newValue) {
            try {
                gint v;
                do {
                    v = getInt(o, offset);
                } while (!weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndSetIntAcquire(Object &o, glong offset, gint newValue) {
            try {
                gint v;
                do {
                    v = getIntAcquire(o, offset);
                } while (!weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndSetLong(Object &o, glong offset, glong newValue) {
            try {
                glong v;
                do {
                    v = getLongVolatile(o, offset);
                } while (!weakCompareAndSetLong(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndSetLongRelease(Object &o, glong offset, glong newValue) {
            try {
                glong v;
                do {
                    v = getLong(o, offset);
                } while (!weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndSetLongAcquire(Object &o, glong offset, glong newValue) {
            try {
                glong v;
                do {
                    v = getLongAcquire(o, offset);
                } while (!weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::getAndSetReference(Object &o, glong offset, Object &newValue) {
            try {
                VAL v = null;
                do {
                    v = &getReferenceVolatile(o, offset);
                } while (!weakCompareAndSetReference(null, tryGetDirectAccess(o, offset), *v, newValue));
                return *v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::getAndSetReferenceRelease(Object &o, glong offset, Object &newValue) {
            try {
                VAL v = null;
                do {
                    v = &getReference(o, offset);
                } while (!weakCompareAndSetReferenceRelease(null, tryGetDirectAccess(o, offset), *v, newValue));
                return *v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        Object &Unsafe::getAndSetReferenceAcquire(Object &o, glong offset, Object &newValue) {
            try {
                VAL v = null;
                do {
                    v = &getReferenceAcquire(o, offset);
                } while (!weakCompareAndSetReferenceAcquire(null, tryGetDirectAccess(o, offset), *v, newValue));
                return *v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndSetBoolean(Object &o, glong offset, gbool newValue) {
            try {
                return b2bool(getAndSetByte(null, tryGetDirectAccess(o, offset), b2byte(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndSetBooleanRelease(Object &o, glong offset, gbool newValue) {
            try {
                return b2bool(getAndSetByteRelease(null, tryGetDirectAccess(o, offset), b2byte(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndSetBooleanAcquire(Object &o, glong offset, gbool newValue) {
            try {
                return b2bool(getAndSetByteAcquire(null, tryGetDirectAccess(o, offset), b2byte(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndSetByte(Object &o, glong offset, gbyte newValue) {
            try {
                gbyte v;
                do {
                    v = getByteVolatile(o, offset);
                } while (!weakCompareAndSetByte(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndSetByteRelease(Object &o, glong offset, gbyte newValue) {
            try {
                gbyte v;
                do {
                    v = getByte(o, offset);
                } while (!weakCompareAndSetByteRelease(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndSetByteAcquire(Object &o, glong offset, gbyte newValue) {
            try {
                gbyte v;
                do {
                    v = getByteAcquire(o, offset);
                } while (!weakCompareAndSetByteAcquire(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndSetShort(Object &o, glong offset, gshort newValue) {
            try {
                gshort v;
                do {
                    v = getShortVolatile(o, offset);
                } while (!weakCompareAndSetShort(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndSetShortRelease(Object &o, glong offset, gshort newValue) {
            try {
                gshort v;
                do {
                    v = getShort(o, offset);
                } while (!weakCompareAndSetShortRelease(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndSetShortAcquire(Object &o, glong offset, gshort newValue) {
            try {
                gshort v;
                do {
                    v = getShortAcquire(o, offset);
                } while (!weakCompareAndSetShortAcquire(null, tryGetDirectAccess(o, offset), v, newValue));
                return v;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getAndSetFloat(Object &o, glong offset, gfloat newValue) {
            try {
                return i2f(getAndSetInt(null, tryGetDirectAccess(o, offset), f2i(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getAndSetFloatRelease(Object &o, glong offset, gfloat newValue) {
            try {
                return i2f(getAndSetIntRelease(null, tryGetDirectAccess(o, offset), f2i(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gfloat Unsafe::getAndSetFloatAcquire(Object &o, glong offset, gfloat newValue) {
            try {
                return i2f(getAndSetIntAcquire(null, tryGetDirectAccess(o, offset), f2i(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getAndSetDouble(Object &o, glong offset, gdouble newValue) {
            try {
                return l2d(getAndSetLong(null, tryGetDirectAccess(o, offset), d2l(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getAndSetDoubleRelease(Object &o, glong offset, gdouble newValue) {
            try {
                return l2d(getAndSetLongRelease(null, tryGetDirectAccess(o, offset), d2l(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gdouble Unsafe::getAndSetDoubleAcquire(Object &o, glong offset, gdouble newValue) {
            try {
                return l2d(getAndSetLongAcquire(null, tryGetDirectAccess(o, offset), d2l(newValue)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseOrBoolean(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseOrByte(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseOrBooleanRelease(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseOrByteRelease(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseOrBooleanAcquire(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseOrByteAcquire(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseAndBoolean(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseAndByte(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseAndBooleanRelease(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseAndByteRelease(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseAndBooleanAcquire(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseAndByteAcquire(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseXorBoolean(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseXorByte(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseXorBooleanRelease(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseXorByteRelease(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbool Unsafe::getAndBitwiseXorBooleanAcquire(Object &o, glong offset, gbool mask) {
            try {
                return b2bool(getAndBitwiseXorByteAcquire(null, tryGetDirectAccess(o, offset), b2byte(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseOrByte(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    current = getByteVolatile(o, offset);
                } while (!weakCompareAndSetByte(null, tryGetDirectAccess(o, offset), current,
                                                CORE_CAST(gbyte, current | mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseOrByteRelease(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    current = getByte(o, offset);
                } while (!weakCompareAndSetByteRelease(null, tryGetDirectAccess(o, offset), current,
                                                       CORE_CAST(gbyte, current | mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseOrByteAcquire(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getByte(o, offset);
                } while (!weakCompareAndSetByteAcquire(null, tryGetDirectAccess(o, offset), current,
                                                       CORE_CAST(gbyte, current | mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseAndByte(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    current = getByteVolatile(o, offset);
                } while (!weakCompareAndSetByte(null, tryGetDirectAccess(o, offset), current,
                                                CORE_CAST(gbyte, current & mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseAndByteRelease(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    current = getByte(o, offset);
                } while (!weakCompareAndSetByteRelease(null, tryGetDirectAccess(o, offset), current,
                                                       CORE_CAST(gbyte, current & mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseAndByteAcquire(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getByte(o, offset);
                } while (!weakCompareAndSetByteAcquire(null, tryGetDirectAccess(o, offset), current,
                                                       CORE_CAST(gbyte, current & mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseXorByte(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    current = getByteVolatile(o, offset);
                } while (!weakCompareAndSetByte(null, tryGetDirectAccess(o, offset), current,
                                                CORE_CAST(gbyte, current ^ mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseXorByteRelease(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    current = getByte(o, offset);
                } while (!weakCompareAndSetByteRelease(null, tryGetDirectAccess(o, offset), current,
                                                       CORE_CAST(gbyte, current ^ mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gbyte Unsafe::getAndBitwiseXorByteAcquire(Object &o, glong offset, gbyte mask) {
            try {
                gbyte current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getByte(o, offset);
                } while (!weakCompareAndSetByteAcquire(null, tryGetDirectAccess(o, offset), current,
                                                       CORE_CAST(gbyte, current ^ mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseOrShort(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    current = getShortVolatile(o, offset);
                } while (!weakCompareAndSetShort(null, tryGetDirectAccess(o, offset), current,
                                                 CORE_CAST(gshort, current | mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseOrShortRelease(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    current = getShort(o, offset);
                } while (!weakCompareAndSetShortRelease(null, tryGetDirectAccess(o, offset), current,
                                                        CORE_CAST(gshort, current | mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseOrShortAcquire(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getShort(o, offset);
                } while (!weakCompareAndSetShortAcquire(null, tryGetDirectAccess(o, offset), current,
                                                        CORE_CAST(gshort, current | mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseAndShort(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    current = getShortVolatile(o, offset);
                } while (!weakCompareAndSetShort(null, tryGetDirectAccess(o, offset), current,
                                                 CORE_CAST(gshort, current & mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseAndShortRelease(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    current = getShort(o, offset);
                } while (!weakCompareAndSetShortRelease(null, tryGetDirectAccess(o, offset), current,
                                                        CORE_CAST(gshort, current & mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseAndShortAcquire(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getShort(o, offset);
                } while (!weakCompareAndSetShortAcquire(null, tryGetDirectAccess(o, offset), current,
                                                        CORE_CAST(gshort, current & mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseXorShort(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    current = getShortVolatile(o, offset);
                } while (!weakCompareAndSetShort(null, tryGetDirectAccess(o, offset), current,
                                                 CORE_CAST(gshort, current ^ mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseXorShortRelease(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    current = getShort(o, offset);
                } while (!weakCompareAndSetShortRelease(null, tryGetDirectAccess(o, offset), current,
                                                        CORE_CAST(gshort, current ^ mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getAndBitwiseXorShortAcquire(Object &o, glong offset, gshort mask) {
            try {
                gshort current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getShort(o, offset);
                } while (!weakCompareAndSetShortAcquire(null, tryGetDirectAccess(o, offset), current,
                                                        CORE_CAST(gshort, current ^ mask)));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseOrChar(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseOrShort(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseOrCharRelease(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseOrShortRelease(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseOrCharAcquire(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseOrShortAcquire(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseAndChar(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseAndShort(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseAndCharRelease(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseAndShortRelease(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseAndCharAcquire(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseAndShortAcquire(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseXorChar(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseXorShort(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseXorCharRelease(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseXorShortRelease(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getAndBitwiseXorCharAcquire(Object &o, glong offset, gchar mask) {
            try {
                return s2c(getAndBitwiseXorShortAcquire(null, tryGetDirectAccess(o, offset), c2s(mask)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseOrInt(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    current = getIntVolatile(o, offset);
                } while (!weakCompareAndSetInt(null, tryGetDirectAccess(o, offset), current, current | mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseOrIntRelease(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    current = getInt(o, offset);
                } while (!weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset), current, current | mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseOrIntAcquire(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getInt(o, offset);
                } while (!weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset), current, current | mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseAndInt(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    current = getIntVolatile(o, offset);
                } while (!weakCompareAndSetInt(null, tryGetDirectAccess(o, offset), current, current & mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseAndIntRelease(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    current = getInt(o, offset);
                } while (!weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset), current, current & mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseAndIntAcquire(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getInt(o, offset);
                } while (!weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset), current, current & mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseXorInt(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    current = getIntVolatile(o, offset);
                } while (!weakCompareAndSetInt(null, tryGetDirectAccess(o, offset), current, current ^ mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseXorIntRelease(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    current = getInt(o, offset);
                } while (!weakCompareAndSetIntRelease(null, tryGetDirectAccess(o, offset), current, current ^ mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getAndBitwiseXorIntAcquire(Object &o, glong offset, gint mask) {
            try {
                gint current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getInt(o, offset);
                } while (!weakCompareAndSetIntAcquire(null, tryGetDirectAccess(o, offset), current, current ^ mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseOrLong(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    current = getLong(o, offset);
                } while (!weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset), current, current | mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseOrLongRelease(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    current = getLong(o, offset);
                } while (!weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset), current, current | mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseOrLongAcquire(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getLong(o, offset);
                } while (!weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset), current, current | mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseAndLong(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    current = getLongVolatile(o, offset);
                } while (!weakCompareAndSetLong(null, tryGetDirectAccess(o, offset), current, current & mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseAndLongRelease(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    current = getLong(o, offset);
                } while (!weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset), current, current & mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseAndLongAcquire(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getLong(o, offset);
                } while (!weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset), current, current & mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseXorLong(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    current = getLongVolatile(o, offset);
                } while (!weakCompareAndSetLong(null, tryGetDirectAccess(o, offset), current, current ^ mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseXorLongRelease(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    current = getLong(o, offset);
                } while (!weakCompareAndSetLongRelease(null, tryGetDirectAccess(o, offset), current, current ^ mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getAndBitwiseXorLongAcquire(Object &o, glong offset, glong mask) {
            try {
                glong current;
                do {
                    // Plain read, the value is a hint, the acquire CAS does the work
                    current = getLong(o, offset);
                } while (!weakCompareAndSetLongAcquire(null, tryGetDirectAccess(o, offset), current, current ^ mask));

                return current;
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::loadFence() {
            VarHandle::loadFence();
        }

        void Unsafe::storeFence() {
            VarHandle::storeFence();
        }

        void Unsafe::fullFence() {
            VarHandle::fullFence();
        }

        glong Unsafe::getLongUnaligned(Object const &o, glong offset) {
            try {
                return (offset & 7) == 0
                           ? getLong(o, offset)
                           : (offset & 3) == 0
                                 ? VarHandle::makeLong(getInt(o, offset),
                                                       getInt(o, offset + 4))
                                 : (offset & 1) == 0
                                       ? VarHandle::makeLong(getShort(o, offset),
                                                             getShort(o, offset + 2),
                                                             getShort(o, offset + 4),
                                                             getShort(o, offset + 6))
                                       : VarHandle::makeLong(getByte(o, offset),
                                                             getByte(o, offset + 1),
                                                             getByte(o, offset + 2),
                                                             getByte(o, offset + 3),
                                                             getByte(o, offset + 4),
                                                             getByte(o, offset + 5),
                                                             getByte(o, offset + 6),
                                                             getByte(o, offset + 7));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong Unsafe::getLongUnaligned(Object const &o, glong offset, gbool bigEndian) {
            try {
                return VarHandle::convEndian(bigEndian, getLongUnaligned(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getIntUnaligned(Object const &o, glong offset) {
            try {
                return (offset & 3) == 0
                           ? getInt(o, offset)
                           : (offset & 1) == 0
                                 ? VarHandle::makeInt(getShort(o, offset),
                                                      getShort(o, offset + 2))
                                 : VarHandle::makeInt(getByte(o, offset),
                                                      getByte(o, offset + 1),
                                                      getByte(o, offset + 2),
                                                      getByte(o, offset + 3));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gint Unsafe::getIntUnaligned(Object const &o, glong offset, gbool bigEndian) {
            try {
                return VarHandle::convEndian(bigEndian, getIntUnaligned(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getShortUnaligned(Object const &o, glong offset) {
            try {
                return (offset & 1) == 0
                           ? getShort(o, offset)
                           : VarHandle::makeShort(getByte(o, offset),
                                                  getByte(o, offset + 1));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gshort Unsafe::getShortUnaligned(Object const &o, glong offset, gbool bigEndian) {
            try {
                return VarHandle::convEndian(bigEndian, getShortUnaligned(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getCharUnaligned(Object const &o, glong offset) {
            try {
                return (offset & 1) == 0
                           ? getChar(o, offset)
                           : CORE_CAST(gchar, VarHandle::makeShort(getByte(o, offset), getByte(o, offset + 1)));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        gchar Unsafe::getCharUnaligned(Object const &o, glong offset, gbool bigEndian) {
            try {
                return VarHandle::convEndian(bigEndian, getCharUnaligned(o, offset));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putLongUnaligned(Object &o, glong offset, glong x) {
            try {
                if ((offset & 7) == 0) {
                    putLong(null, tryGetDirectAccess(o, offset), x);
                } else if ((offset & 3) == 0) {
                    VarHandle::putLongParts(null, tryGetDirectAccess(o, offset),
                                            CORE_CAST(gint, (x + 0u) >> 0),
                                            CORE_CAST(gint, (x + 0u) >> 32));
                } else if ((offset & 1) == 0) {
                    VarHandle::putLongParts(null, tryGetDirectAccess(o, offset),
                                            CORE_CAST(gshort, (x + 0u) >> 0),
                                            CORE_CAST(gshort, (x + 0u) >> 16),
                                            CORE_CAST(gshort, (x + 0u) >> 32),
                                            CORE_CAST(gshort, (x + 0u) >> 48));
                } else {
                    VarHandle::putLongParts(null, tryGetDirectAccess(o, offset),
                                            CORE_CAST(gbyte, (x + 0u) >> 0),
                                            CORE_CAST(gbyte, (x + 0u) >> 8),
                                            CORE_CAST(gbyte, (x + 0u) >> 16),
                                            CORE_CAST(gbyte, (x + 0u) >> 24),
                                            CORE_CAST(gbyte, (x + 0u) >> 32),
                                            CORE_CAST(gbyte, (x + 0u) >> 40),
                                            CORE_CAST(gbyte, (x + 0u) >> 48),
                                            CORE_CAST(gbyte, (x + 0u) >> 56));
                }
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putLongUnaligned(Object &o, glong offset, glong x, gbool bigEndian) {
            try {
                putLongUnaligned(null, tryGetDirectAccess(o, offset), VarHandle::convEndian(bigEndian, x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putIntUnaligned(Object &o, glong offset, gint x) {
            try {
                if ((offset & 3) == 0) {
                    putInt(null, tryGetDirectAccess(o, offset), x);
                } else if ((offset & 1) == 0) {
                    VarHandle::putIntParts(null, tryGetDirectAccess(o, offset),
                                           CORE_CAST(gshort, (x + 0u) >> 0),
                                           CORE_CAST(gshort, (x + 0u) >> 16));
                } else {
                    VarHandle::putIntParts(null, tryGetDirectAccess(o, offset),
                                           CORE_CAST(gbyte, (x + 0u) >> 0),
                                           CORE_CAST(gbyte, (x + 0u) >> 8),
                                           CORE_CAST(gbyte, (x + 0u) >> 16),
                                           CORE_CAST(gbyte, (x + 0u) >> 24));
                }
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putIntUnaligned(Object &o, glong offset, gint x, gbool bigEndian) {
            try {
                putIntUnaligned(null, tryGetDirectAccess(o, offset), VarHandle::convEndian(bigEndian, x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putShortUnaligned(Object &o, glong offset, gshort x) {
            try {
                if ((offset & 1) == 0) {
                    putShort(null, tryGetDirectAccess(o, offset), x);
                } else {
                    VarHandle::putShortParts(null, tryGetDirectAccess(o, offset),
                                             CORE_CAST(gbyte, (x + 0u) >> 0),
                                             CORE_CAST(gbyte, (x + 0u) >> 8));
                }
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putShortUnaligned(Object &o, glong offset, gshort x, gbool bigEndian) {
            try {
                putShortUnaligned(null, tryGetDirectAccess(o, offset), VarHandle::convEndian(bigEndian, x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putCharUnaligned(Object &o, glong offset, gchar x) {
            try {
                putShortUnaligned(null, tryGetDirectAccess(o, offset), c2s(x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        void Unsafe::putCharUnaligned(Object &o, glong offset, gchar x, gbool bigEndian) {
            try {
                putCharUnaligned(null, tryGetDirectAccess(o, offset), VarHandle::convEndian(bigEndian, x));
            } catch (Exception const &ex) { ex.throws($ftrace()); }
        }

        glong statics[8192] = {};
        gint statics_size = 0;
        glong threadNumber = 0;
        glong initialized = 0x0123456789ABCDEFLL;
        Cache VarHandleCache = {};

        gbool Unsafe::isAlreadyAllocated(Object const &object) {
            if (object == null || Class<Thread>::hasInstance(object))
                return true;
            return VarHandleCache.containsAddress(object);
        }

        void Unsafe::storeReference(Object &object) {
            Cache::ObjectType type = Cache::NORMAL;
            if (Class<Throwable>::hasInstance(object))
                type = Cache::THROWABLE;
            else if (Class<Thread>::hasInstance(object))
                type = Cache::THREAD;
            VarHandleCache.addAddress(object, type);
        }

        gbool Unsafe::deleteReference(Object &object) {
            return VarHandleCache.removeAddress(object);
        }

        glong Unsafe::tryGetDirectAccess(Object const &o, glong offset) {
            const glong address = o2l(o);

            if (address != 0 && Class<BasicArray>::hasInstance(o))
                return array2DirectAccess(o, offset);

            return address + offset;
        }

        glong Unsafe::array2DirectAccess(Object const &o, glong offset) {
            CORE_FAST glong ARRAY_BASE_OFFSET = ARRAY_BYTE_BASE_OFFSET;

            glong address = o2l(o) + ARRAY_BASE_OFFSET;
            glong i_x_scale = offset - ARRAY_BASE_OFFSET;

            address = CORE_CAST(glong, *CORE_CAST(void**, address));
            return address + i_x_scale;
        }

        glong Unsafe::array2DirectAccess2(Object const &o, glong offset) {
            glong address = o2l(o);
            glong i_x_scale = offset;

            if (Class<ByteArray>::hasInstance(o)) {
                address += ARRAY_BYTE_BASE_OFFSET;
                i_x_scale -= ARRAY_BYTE_BASE_OFFSET;

                address = CORE_CAST(glong, *CORE_CAST(void**, address));
            } else if (Class<CharArray>::hasInstance(o)) {
                address += ARRAY_CHAR_BASE_OFFSET;
                i_x_scale -= ARRAY_CHAR_BASE_OFFSET;
            } else if (Class<ObjectArray>::hasInstance(o)) {
                address += ARRAY_REFERENCE_BASE_OFFSET;
                i_x_scale -= ARRAY_REFERENCE_BASE_OFFSET;
            } else if (Class<IntArray>::hasInstance(o)) {
                address += ARRAY_INT_BASE_OFFSET;
                i_x_scale -= ARRAY_INT_BASE_OFFSET;
            } else if (Class<LongArray>::hasInstance(o)) {
                address += ARRAY_LONG_BASE_OFFSET;
                i_x_scale -= ARRAY_LONG_BASE_OFFSET;
            } else if (Class<ShortArray>::hasInstance(o)) {
                address += ARRAY_SHORT_BASE_OFFSET;
                i_x_scale -= ARRAY_SHORT_BASE_OFFSET;
            } else if (Class<BooleanArray>::hasInstance(o)) {
                address += ARRAY_BOOLEAN_BASE_OFFSET;
                i_x_scale -= ARRAY_BOOLEAN_BASE_OFFSET;
            } else if (Class<FloatArray>::hasInstance(o)) {
                address += ARRAY_FLOAT_BASE_OFFSET;
                i_x_scale -= ARRAY_FLOAT_BASE_OFFSET;
            } else if (Class<DoubleArray>::hasInstance(o)) {
                address += ARRAY_DOUBLE_BASE_OFFSET;
                i_x_scale -= ARRAY_DOUBLE_BASE_OFFSET;
            }

            address = CORE_CAST(glong, *CORE_CAST(void**, address));

            return address + i_x_scale;
        }

        glong Unsafe::threadCount() {
            return Cache::threadCount();
        }

        CORE_WARNING_POP
    } // misc
} // core
