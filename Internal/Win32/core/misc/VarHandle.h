//
// Created by bruns on 22/03/2024.
//

#ifndef CORE24_VARHANDLE_H
#define CORE24_VARHANDLE_H

#include <core/misc/Unsafe.h>

namespace core {
    namespace misc {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        class VarHandle : public Object {
        public:
            static CORE_FAST gint ADDRESS_SIZE = Unsafe::ADDRESS_SIZE;
            static CORE_FAST gint BIG_ENDIAN = Unsafe::BIG_ENDIAN;

            CORE_ALIAS(REF, Class<Object>::Pointer);

            /**
             * Check if a value is 32-bit clean (32 MSB are all zero)
             *
             * @param value the 64-bit value to check
             *
             * @return true if the value is 32-bit clean
             */
            static gbool is32BitClean(glong value);

            /**
             * Check the validity of a size (the equivalent of a size_t)
             *
             * @throws RuntimeException if the size is invalid
             *         (<em>Note:</em> after optimization, invalid inputs may
             *         go undetected, which will lead to unpredictable
             *         behavior)
             */
            static void checkSize(glong size);

            /**
             * Check the validity of a native address (the equivalent of void*)
             *
             * @throws RuntimeException if the address is invalid
             *         (<em>Note:</em> after optimization, invalid inputs may
             *         go undetected, which will lead to unpredictable
             *         behavior)
             */
            static void checkNativeAddress(glong address);

            /**
             * Check the validity of an offset, relative to a base object
             *
             * @param o the base object
             * @param offset the offset to check
             *
             * @throws RuntimeException if the size is invalid
             *         (<em>Note:</em> after optimization, invalid inputs may
             *         go undetected, which will lead to unpredictable
             *         behavior)
             */
            static void checkOffset(Object const &o, glong offset);

            /**
             * Check the validity of a double-register pointer
             *
             * Note: This code deliberately does *not* check for NPE for (at
             * least) three reasons:
             *
             * 1) NPE is not just NULL/0 - there is a range of values all
             * resulting in an NPE, which is not trivial to check for
             *
             * 2) It is the responsibility of the callers of Unsafe methods
             * to verify the input, so throwing an exception here is not really
             * useful - passing in a NULL pointer is a critical error and the
             * must not expect an exception to be thrown anyway.
             *
             * 3) the actual operations will detect NULL pointers anyway by
             * means of traps and signals (like SIGSEGV).
             *
             * @param o Java heap object, or null
             * @param offset indication of where the variable resides in a Java heap
             *        object, if any, else a memory address locating the variable
             *        statically
             *
             * @throws RuntimeException if the pointer is invalid
             *         (<em>Note:</em> after optimization, invalid inputs may
             *         go undetected, which will lead to unpredictable
             *         behavior)
             */
            static void checkPointer(Object const &o, glong offset);

            static gbyte getByte(Object const &o, glong offset);

            static gshort getShort(Object const &o, glong offset);

            static gint getInt(Object const &o, glong offset);

            static glong getLong(Object const &o, glong offset);

            static gbyte getByteVolatile(Object const &o, glong offset);

            static gshort getShortVolatile(Object const &o, glong offset);

            static gint getIntVolatile(Object const &o, glong offset);

            static glong getLongVolatile(Object const &o, glong offset);

            static gbyte getByteOpaque(Object const &o, glong offset);

            static gshort getShortOpaque(Object const &o, glong offset);

            static gint getIntOpaque(Object const &o, glong offset);

            static glong getLongOpaque(Object const &o, glong offset);

            static void putByte(Object &o, glong offset, gbyte x);

            static void putShort(Object &o, glong offset, gshort x);

            static void putInt(Object &o, glong offset, gint x);

            static void putLong(Object &o, glong offset, glong x);

            static void putByteVolatile(Object &o, glong offset, gbyte x);

            static void putShortVolatile(Object &o, glong offset, gshort x);

            static void putIntVolatile(Object &o, glong offset, gint x);

            static void putLongVolatile(Object &o, glong offset, glong x);

            static void putByteOpaque(Object &o, glong offset, gbyte x);

            static void putShortOpaque(Object &o, glong offset, gshort x);

            static void putIntOpaque(Object &o, glong offset, gint x);

            static void putLongOpaque(Object &o, glong offset, glong x);


            static gint pickPos(gint top, gint pos);

            // These methods construct integers from bytes.  The byte ordering
            // is the native endianness of this platform.
            static glong makeLong(gbyte i0, gbyte i1, gbyte i2, gbyte i3, gbyte i4, gbyte i5, gbyte i6, gbyte i7);

            static glong makeLong(gshort i0, gshort i1, gshort i2, gshort i3);

            static glong makeLong(gint i0, gint i1);

            static gint makeInt(gshort i0, gshort i1);

            static gint makeInt(gbyte i0, gbyte i1, gbyte i2, gbyte i3);

            static gshort makeShort(gbyte i0, gbyte i1);

            static gbyte pick(gbyte le, gbyte be);

            static gshort pick(gshort le, gshort be);

            static gint pick(gint le, gint be);

            // These methods write integers to memory from smaller parts
            // provided by their caller.  The ordering in which these parts
            // are written is the native endianness of this platform.
            static void putLongParts(Object &o, glong offset, gbyte i0, gbyte i1, gbyte i2,
                                     gbyte i3, gbyte i4, gbyte i5, gbyte i6, gbyte i7);

            static void putLongParts(Object &o, glong offset, gshort i0, gshort i1, gshort i2, gshort i3);

            static void putLongParts(Object &o, glong offset, gint i0, gint i1);

            static void putIntParts(Object &o, glong offset, gshort i0, gshort i1);

            static void putIntParts(Object &o, glong offset, gbyte i0, gbyte i1, gbyte i2, gbyte i3);

            static void putShortParts(Object &o, glong offset, gbyte i0, gbyte i1);

            // Zero-extend an integer
            static gint toUnsignedInt(gbyte n);

            static gint toUnsignedInt(gshort n);

            static glong toUnsignedLong(gbyte n);

            static glong toUnsignedLong(gshort n);

            static glong toUnsignedLong(gint n);

            // Maybe byte-reverse an integer
            static gchar convEndian(gbool big, gchar n);

            static gshort convEndian(gbool big, gshort n);

            static gint convEndian(gbool big, gint n);

            static glong convEndian(gbool big, glong n);

            static gbyte compareAndExchangeByte(Object &o, glong offset, gbyte expected, gbyte x);

            static gshort compareAndExchangeShort(Object &o, glong offset, gshort expected, gshort x);

            static gint compareAndExchangeInt(Object &o, glong offset, gint expected, gint x);

            static glong compareAndExchangeLong(Object &o, glong offset, glong expected, glong x);

            static gbyte compareAndExchangeByteAcquire(Object &o, glong offset, gbyte expected, gbyte x);

            static gshort compareAndExchangeShortAcquire(Object &o, glong offset, gshort expected, gshort x);

            static gint compareAndExchangeIntAcquire(Object &o, glong offset, gint expected, gint x);

            static glong compareAndExchangeLongAcquire(Object &o, glong offset, glong expected, glong x);

            static gbyte compareAndExchangeByteRelease(Object &o, glong offset, gbyte expected, gbyte x);

            static gshort compareAndExchangeShortRelease(Object &o, glong offset, gshort expected, gshort x);

            static gint compareAndExchangeIntRelease(Object &o, glong offset, gint expected, gint x);

            static glong compareAndExchangeLongRelease(Object &o, glong offset, glong expected, glong x);

            static gbool weakCompareAndSetByte(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool weakCompareAndSetShort(Object &o, glong offset, gshort expected, gshort x);

            static gbool weakCompareAndSetInt(Object &o, glong offset, gint expected, gint x);

            static gbool weakCompareAndSetLong(Object &o, glong offset, glong expected, glong x);

            static gbool weakCompareAndSetByteAcquire(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool weakCompareAndSetShortAcquire(Object &o, glong offset, gshort expected, gshort x);

            static gbool weakCompareAndSetIntAcquire(Object &o, glong offset, gint expected, gint x);

            static gbool weakCompareAndSetLongAcquire(Object &o, glong offset, glong expected, glong x);

            static gbool weakCompareAndSetByteRelease(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool weakCompareAndSetShortRelease(Object &o, glong offset, gshort expected, gshort x);

            static gbool weakCompareAndSetIntRelease(Object &o, glong offset, gint expected, gint x);

            static gbool weakCompareAndSetLongRelease(Object &o, glong offset, glong expected, glong x);

            static gbool weakCompareAndSetBytePlain(Object &o, glong offset, gbyte expected, gbyte x);

            static gbool weakCompareAndSetShortPlain(Object &o, glong offset, gshort expected, gshort x);

            static gbool weakCompareAndSetIntPlain(Object &o, glong offset, gint expected, gint x);

            static gbool weakCompareAndSetLongPlain(Object &o, glong offset, glong expected, glong x);

            static void loadFence();

            static void storeFence();

            static void fullFence();
        };

        CORE_WARNING_POP
    } // misc
} // core

#endif //CORE24_VARHANDLE_H
