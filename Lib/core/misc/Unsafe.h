//
// Created by brunshweeck on 8 juin 2024.
//

#ifndef CORE24_UNSAFE_H
#define CORE24_UNSAFE_H

#include <core/AssertionError.h>
#include <core/CloneNotSupportedException.h>
#include <core/misc/Nil.h>
#include <core/misc/ObjectArray.h>
#include <core/misc/PrimitiveArray.h>

#include <core/Boolean.h>
#include <core/Double.h>
#include <core/Float.h>
#include <core/Long.h>
#include <core/Short.h>

namespace core {
    namespace misc {
        CORE_WARNING_PUSH
        CORE_WARNING_DISABLE_DEPRECATED

        class Unsafe final : public Object {
            /**
             * This Constructor is called on time only.
             */
            CORE_EXPLICIT Unsafe() = default;

        public:
            /// peek and poke operations
            /// (compilers should optimize these to memory ops)

            /**
             * Fetches a value from a given class variable.
             * More specifically, fetches a field or array element within the given
             * object @c o at the given offset, or (if @c o is null)
             * from the memory address whose numerical value is the given offset.
             * <p>
             * The results are undefined unless one of the following cases is true:
             *
             * @li The offset was obtained from @a offsetof of some class field and the
             * object referred to by @c o is of a compatible with that field's class.
             *
             * @li The object referred to by @c o is an array, and the offset
             * is an integer of the form @c B+N*S, where @c N is
             * a valid index into the array, and @c B and @c S are
             * the values obtained by @c ARRAY_X_BASE_OFFSET and
             * @c Unsafe::ARRAY_X_INDEX_SCALE (respectively, where X represent the type of array element)
             * from the array's class.  The value referred to is the @c N<em>th</em> element of the array.
             *
             * <p>
             * If one of the above cases is true, the call references a specific class
             * variable (field or C-array element).  However, the results are undefined
             * if that variable is not in fact of the type returned by this method. </p>
             * <p>
             * This method refers to a variable by means of two parameters, and so
             * it provides (in effect) a <em>double-register</em> addressing mode
             * for class variables.  When the object reference is null, this method
             * uses its offset as an absolute address.  This is similar in operation
             * to methods such as @c Unsafe::getInt(long), which provide (in effect) a
             * <em>single-register</em> addressing mode for non-class variables.
             * However, because class variables may have a different layout in memory
             * from non-class variables, programmers should not assume that these
             * two addressing modes are ever equivalent.  Also, programmers should
             * remember that offsets from the double-register addressing mode cannot
             * be portability confused with longs used in the single-register addressing
             * mode. </p>
             *
             * @param obj class heap object in which the variable resides, if any, else
             *        null
             * @param offset indication of where the variable resides in a class heap
             *        object, if any, else a memory address locating the variable
             *        statically
             * @return the value fetched from the indicated class variable
             * @throws RuntimeException No defined exceptions are thrown
             */
            static gint getInt(Object const& obj, glong offset);

            /**
             * Stores a value into a given class variable.
             * <p>
             * The first two parameters are interpreted exactly as with
             * @c Unsafe::getInt(Object, long) to refer to a specific
             * class variable (field or array element).  The given value
             * is stored into that variable. </p>
             * <p>
             * The variable must be of the same type as the method
             * parameter @c x. </p>
             *
             * @param o class heap object in which the variable resides, if any, else
             *        null
             * @param offset indication of where the variable resides in a class heap
             *        object, if any, else a memory address locating the variable
             *        statically
             * @param x the value to store into the indicated class variable
             * @throws RuntimeException No defined exceptions are thrown
             */
            static void putInt(Object& o, glong offset, gint x);

            /**
             * Fetches a reference value from a given class variable.
             * @see Unsafe::getInt(Object, long)
             */
            static Object& getReference(Object const& obj, glong offset);

            /**
             * Stores a reference value into a given class variable.
             * <p>
             * Unless the reference @c x being stored is either null
             * or matches the field type, the results are undefined.
             * If the reference @c o is non-null, card marks or
             * other store barriers for that object (if the VM requires them)
             * are updated. </p>
             *
             * @see Unsafe::putInt(Object, long, int)
             */
            static void putReference(Object& obj, glong offset, Object& x);

            /** @see Unsafe::getInt(Object, long) */
            static gbool getBoolean(Object const& obj, glong offset);

            /** @see Unsafe::putInt(Object, long, int) */
            static void putBoolean(Object& obj, glong offset, gbool x);

            /** @see Unsafe::getInt(Object, long) */
            static gbyte getByte(Object const& obj, glong offset);

            /** @see Unsafe::putInt(Object, long, int) */
            static void putByte(Object& obj, glong offset, gbyte x);

            /** @see Unsafe::getInt(Object, long) */
            static gshort getShort(Object const& obj, glong offset);

            /** @see Unsafe::putInt(Object, long, int) */
            static void putShort(Object& obj, glong offset, gshort x);

            /** @see Unsafe::getInt(Object, long) */
            static gchar getChar(Object const& obj, glong offset);

            /** @see Unsafe::putInt(Object, long, int) */
            static void putChar(Object& obj, glong offset, gchar x);

            /** @see Unsafe::getInt(Object, long) */
            static glong getLong(Object const& obj, glong offset);

            /** @see Unsafe::putInt(Object, long, int) */
            static void putLong(Object& obj, glong offset, glong x);

            /** @see Unsafe::getInt(Object, long) */
            static gfloat getFloat(Object const& obj, glong offset);

            /** @see Unsafe::putInt(Object, long, int) */
            static void putFloat(Object& obj, glong offset, gfloat x);

            /** @see Unsafe::getInt(Object, long) */
            static gdouble getDouble(Object const& obj, glong offset);

            /** @see Unsafe::putInt(Object, long, int) */
            static void putDouble(Object& obj, glong offset, gdouble x);

            /**
             * Fetches a native pointer from a given memory address.  If the address is
             * zero, or does not point into a block obtained from
             * @c Unsafe::allocateMemory, the results are undefined.
             *
             * <p>If the native pointer is less than 64 bits wide, it is extended as
             * an unsigned number to a class long.  The pointer may be indexed by any
             * given byte offset, simply by adding that offset (as a simple integer) to
             * the long representing the pointer.  The number of bytes actually read
             * from the target address may be determined by consulting
             * @c Unsafe::addressSize. </p>
             *
             * @see Unsafe::allocateMemory
             * @see Unsafe::getInt(Object, long)
             */
            static glong getAddress(Object const& obj, glong offset);

            /**
             * Stores a native pointer into a given memory address.  If the address is
             * zero, or does not point into a block obtained from
             * @c Unsafe::allocateMemory, the results are undefined.
             *
             * <p>The number of bytes actually written at the target address may be
             * determined by consulting @c Unsafe::addressSize. </p>
             *
             * @see Unsafe::allocateMemory
             * @see Unsafe::putInt(Object, long, int)
             */
            static void putAddress(Object& obj, glong offset, glong x);

            // These work on values in the C heap.

            /**
             * Fetches a value from a given memory address.  If the address is zero, or
             * does not point into a block obtained from @c Unsafe::allocateMemory, the
             * results are undefined.
             *
             * @see Unsafe::allocateMemory
             */
            static gbyte getByte(glong address);

            /**
             * Stores a value into a given memory address.  If the address is zero, or
             * does not point into a block obtained from @c Unsafe::allocateMemory, the
             * results are undefined.
             *
             * @see Unsafe::getByte(long)
             */
            static void putByte(glong address, gbyte x);

            /** @see Unsafe::getByte(long) */
            static gshort getShort(glong address);

            /** @see Unsafe::putByte(long, byte) */
            static void putShort(glong address, gshort x);

            /** @see Unsafe::getByte(long) */
            static gchar getChar(glong address);

            /** @see Unsafe::putByte(long, byte) */
            static void putChar(glong address, gchar x);

            /** @see Unsafe::getByte(long) */
            static gint getInt(glong address);

            /** @see Unsafe::putByte(long, byte) */
            static void putInt(glong address, gint x);

            /** @see Unsafe::getByte(long) */
            static glong getLong(glong address);

            /** @see Unsafe::putByte(long, byte) */
            static void putLong(glong address, glong x);

            /** @see Unsafe::getByte(long) */
            static gfloat getFloat(glong address);

            /** @see Unsafe::putByte(long, byte) */
            static void putFloat(glong address, gfloat x);

            /** @see Unsafe::getByte(long) */
            static gdouble getDouble(glong address);

            /** @see Unsafe::putByte(long, byte) */
            static void putDouble(glong address, gdouble x);

            /** @see Unsafe::getAddress(Object, long) */
            static glong getAddress(glong address);

            /** @see Unsafe::putAddress(Object, long, long) */
            static void putAddress(glong address, glong x);

            /// wrappers for malloc, reallocate, free:

            /**
             * Allocates a new block of native memory, of the given size in bytes.  The
             * contents of the memory are uninitialized; they will generally be
             * garbage.  The resulting native pointer will never be zero, and will be
             * aligned for all value types.  Dispose of this memory by calling
             * @c Unsafe::freeMemory, or resize it with @c Unsafe::reallocateMemory.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are the best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if the size is negative or too large
             *         for the native size_t type
             *
             * @throws OutOfMemoryError if the allocation is refused by the system
             *
             * @see Unsafe::getByte(long)
             * @see Unsafe::putByte(long, byte)
             */
            static glong allocateMemory(glong bytes);

            /**
             * Resizes a new block of native memory, to the given size in bytes.  The
             * contents of the new block past the size of the old block are
             * uninitialized; they will generally be garbage.  The resulting native
             * pointer will be zero if and only if the requested size is zero.  The
             * resulting native pointer will be aligned for all value types.  Dispose
             * of this memory by calling @c Unsafe::freeMemory, or resize it with
             * @c Unsafe::reallocateMemory.  The address passed to this method may be null, in
             * which case an allocation will be performed.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are the best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if the size is negative or too large
             *         for the native size_t type
             *
             * @throws OutOfMemoryError if the allocation is refused by the system
             *
             * @see Unsafe::allocateMemory
             */
            static glong reallocateMemory(glong address, glong bytes);

            /**
             * Sets all bytes in a given block of memory to a fixed value
             * (usually zero).
             *
             * <p>This method determines a block's base address by means of two parameters,
             * and so it provides (in effect) a <em>double-register</em> addressing mode,
             * as discussed in @c Unsafe::getInt(Object,long).  When the object reference is null,
             * the offset supplies an absolute base address. </p>
             *
             * <p>The stores are in coherent (atomic) units of a size determined
             * by the address and length parameters.  If the effective address and
             * length are all even modulo 8, the stores take place in 'long' units.
             * If the effective address and length are (resp.) even modulo 4 or 2,
             * the stores take place in units of 'int' or 'short'. </p>
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are the best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if any of the arguments is invalid
             */
            static void setMemory(Object& obj, glong offset, glong bytes, gbyte value);

            /**
             * Sets all bytes in a given block of memory to a fixed value
             * (usually zero).  This provides a <em>single-register</em> addressing mode,
             * as discussed in @c Unsafe::getInt(Object,long).
             *
             * <p>Equivalent to @c setMemory(null, address, bytes, value). </p>
             */
            static void setMemory(glong address, glong bytes, gbyte value);

            /**
             * Sets all bytes in a given block of memory to a copy of another
             * block.
             *
             * <p>This method determines each block's base address by means of two parameters,
             * and so it provides (in effect) a <em>double-register</em> addressing mode,
             * as discussed in @c Unsafe::getInt(Object,long).  When the object reference is null,
             * the offset supplies an absolute base address. </p>
             *
             * <p>The transfers are in coherent (atomic) units of a size determined
             * by the address and length parameters.  If the effective addresses and
             * length are all even modulo 8, the transfer takes place in 'long' units.
             * If the effective addresses and length are (resp.) even modulo 4 or 2,
             * the transfer takes place in units of 'int' or 'short'. </p>
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are the best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if any of the arguments is invalid
             */
            static void copyMemory(Object const& srcBase, glong srcOffset,
                                   Object& destBase, glong destOffset,
                                   glong bytes);

            /**
             * Sets all bytes in a given block of memory to a copy of another
             * block.  This provides a <em>single-register</em> addressing mode,
             * as discussed in @c Unsafe::getInt(Object,long).
             *
             * Equivalent to @c copyMemory(null, srcAddress, null, destAddress, bytes).
             */
            static void copyMemory(glong srcAddress, glong destAddress, glong bytes);

            /**
             * Copies all elements from one block of memory to another block,
             * *unconditionally* byte swapping the elements on the fly.
             *
             * <p>This method determines each block's base address by means of two parameters,
             * and so it provides (in effect) a <em>double-register</em> addressing mode,
             * as discussed in @c Unsafe::getInt(Object,long).  When the object reference is null,
             * the offset supplies an absolute base address. </p>
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are the best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if any of the arguments is invalid
             */
            static void copySwapMemory(Object const& srcBase, glong srcOffset,
                                       Object& destBase, glong destOffset,
                                       glong bytes, glong elemSize);

            /**
             * Copies all elements from one block of memory to another block, byte swapping the
             * elements on the fly.
             *
             * This provides a <em>single-register</em> addressing mode, as
             * discussed in @c Unsafe::getInt(Object,long).
             *
             * Equivalent to @c copySwapMemory(null, srcAddress, null, destAddress, bytes, elemSize).
             */
            static void copySwapMemory(glong srcAddress, glong destAddress, glong bytes, glong elemSize);

            /**
             * Disposes of a block of native memory, as obtained from
             * @c Unsafe::allocateMemory or @c Unsafe::reallocateMemory.  The address passed to
             * this method may be null, in which case no action is taken.
             *
             * <em>Note:</em> It is the responsibility of the caller to make
             * sure arguments are checked before the methods are called. While
             * some rudimentary checks are performed on the input, the checks
             * are the best effort and when performance is an overriding priority,
             * as when methods of this class are optimized by the runtime
             * compiler, some or all checks (if any) may be elided. Hence, the
             * caller must not rely on the checks and corresponding
             * exceptions!
             *
             * @throws RuntimeException if any of the arguments is invalid
             *
             * @see Unsafe::allocateMemory
             */
            static void freeMemory(glong address);

            /// random queries

            static CORE_FAST gint ADDRESS_SIZE = CORE_ADDRESS_SIZE;

            /**
             * This constant differs from all results that will ever be returned from
             * @c Unsafe::offsetof.
             */
            static CORE_FAST gint INVALID_FIELD_OFFSET = -1;

            CORE_WARNING_PUSH
            CORE_WARNING_DISABLE_INVALID_OFFSETOF

            static CORE_FAST gint ARRAY_BOOLEAN_BASE_OFFSET = CORE_FIELD_OFFSET(PrimitiveArray<Boolean>, value);
            static CORE_FAST gint ARRAY_BYTE_BASE_OFFSET = CORE_FIELD_OFFSET(PrimitiveArray<Byte>, value);
            static CORE_FAST gint ARRAY_SHORT_BASE_OFFSET = CORE_FIELD_OFFSET(PrimitiveArray<Short>, value);
            static CORE_FAST gint ARRAY_CHAR_BASE_OFFSET = CORE_FIELD_OFFSET(PrimitiveArray<Character>, value);
            static CORE_FAST gint ARRAY_INT_BASE_OFFSET = CORE_FIELD_OFFSET(PrimitiveArray<Integer>, value);
            static CORE_FAST gint ARRAY_LONG_BASE_OFFSET = CORE_FIELD_OFFSET(PrimitiveArray<Long>, value);
            static CORE_FAST gint ARRAY_FLOAT_BASE_OFFSET = CORE_FIELD_OFFSET(PrimitiveArray<Float>, value);
            static CORE_FAST gint ARRAY_DOUBLE_BASE_OFFSET = CORE_FIELD_OFFSET(PrimitiveArray<Double>, value);
            static CORE_FAST gint ARRAY_REFERENCE_BASE_OFFSET = CORE_FIELD_OFFSET(ObjectArray, value);

            CORE_WARNING_POP

            static CORE_FAST gint ARRAY_BOOLEAN_INDEX_SCALE = Boolean::BYTES;
            static CORE_FAST gint ARRAY_BYTE_INDEX_SCALE = Byte::BYTES;
            static CORE_FAST gint ARRAY_SHORT_INDEX_SCALE = Short::BYTES;
            static CORE_FAST gint ARRAY_CHAR_INDEX_SCALE = Character::BYTES;
            static CORE_FAST gint ARRAY_INT_INDEX_SCALE = Integer::BYTES;
            static CORE_FAST gint ARRAY_LONG_INDEX_SCALE = Long::BYTES;
            static CORE_FAST gint ARRAY_FLOAT_INDEX_SCALE = Float::BYTES;
            static CORE_FAST gint ARRAY_DOUBLE_INDEX_SCALE = Double::BYTES;
            static CORE_FAST gint ARRAY_REFERENCE_INDEX_SCALE = ADDRESS_SIZE;

            /**
             * Reports the size in bytes of a native pointer, as stored via
             * @c Unsafe::putAddress.  This value will be either 4 or 8.  Note that the sizes of
             * other primitive types (as stored in native memory blocks) is determined
             * fully by their information content.
             */
            static gint addressSize();

            /**
             * Reports the size in bytes of a native memory page (whatever that is).
             * This value will always be a power of two.
             */
            static gint pageSize();

        private:
            template <class T>
            CORE_ALIAS(Unref, typename Class<T>::NRef);

            template <class T>
            CORE_ALIAS(Unref2, typename Class<Unref<T>>::NConst);

        public:
            /**
             * Allocates an instance but does not run any constructor.
             * Initializes the class if it has not yet been.
             *
             * @note It's necessary that it exist +1 public corresponding constructor.
             */
            template <class T, class... Args>
            static Unref2<T>& newInstance(Args&&... args) {
                // basic check for T
                CORE_FULL_CHECK_SLIMMED_TYPE(Unref2<T>)
                // T mustn't be incomplete type (all incomplete types are not sizable)
                CORE_FAST_ASSERT(Class<T>::isComplete());
                // T mustn't be abstract type
                CORE_FAST_ASSERT(!Class<T>::isAbstract());
                // T must have constructor compatible, callable with given arguments.
                CORE_FAST_ASSERT(Class<T>::template isConstructible<Args...>());
                CORE_ALIAS(REF, typename Class<Unref2<T>>::Pointer);

                glong address = 0;
                REF newRef = { };
                try {
                    // allocate sufficient memory space to store new instance of T
                    address = allocateMemory(sizeof(T));

                    // construct desired object at memory address allocated previously.
                    newRef = new((REF)address) Unref2<T>(forwardInstance<Args>(args)...);

                    // Store, allocated object.
                    return Store<Unref2<T>>::store(*newRef);
                } catch (Throwable const& cse) {
                    // Storage failed.
                    if (newRef) return *newRef;
                    // Construction failed.
                    if (address != 0 && !newRef) freeMemory(address);

                    Error("Unable to create new object instance"_Sl, cse).throws($ftrace());
                } catch (std::exception const& cse) {
                    // Storage failed. (legacy code)
                    if (newRef) return *newRef;
                    // Construction failed.
                    if (address != 0 && !newRef) freeMemory(address);

                    Error("Unable to create new object instance"_Sl, Exception(String(cse.what())))
                            .throws($ftrace());
                } catch (...) {
                    // Construction failed.
                    if (address != 0 && !newRef) freeMemory(address);

                    Error("Unable to create new object instance"_Sl).throws($ftrace());
                }
            }

            /**
             * Allocates a redefine-able instance, but does not run any constructor.
             * Initializes the class if it has not yet been.
             * @note
             *  Use Unsafe::replaceInstance to replace create instance with another <br/>
             * Example:
             * @code
             *      auto& var1 = Unsafe::newReplaceableInstance<String>(sizeof(XString))
             *      // assert sizeof(XString) >= sizeof(String)
             *      auto& var2 = Unsafe::replaceInstance<XString>(var1);
             *      &var1 == &var2 // true
             *      var1 == var2 // true
             *      ClassOf(var2)::hasInstance(var1) // true
             * @endcode
             *
             * @note It's necessary that it exist +1 public corresponding constructor.
             */
            template <class T, class... Args>
            static Unref2<T>& newReplaceableInstance(gint requiredSpace, Args&&... args) {
                // basic check for T
                CORE_FULL_CHECK_SLIMMED_TYPE(Unref2<T>)
                // T mustn't be incomplete type (all incomplete types are not sizable)
                CORE_FAST_ASSERT(Class<T>::isComplete());
                // T mustn't be abstract type
                CORE_FAST_ASSERT(!Class<T>::isAbstract());
                // T must have constructor compatible, callable with given arguments.
                CORE_FAST_ASSERT(Class<T>::template isConstructible<Args...>());
                CORE_ALIAS(REF, typename Class<Unref2<T>>::Pointer);

                glong address = 0;
                REF newRef = { };
                try {
                    // allocate sufficient memory space to store new instance of T
                    address = allocateMemory(sizeof(T));

                    // construct desired object at memory address allocated previously.
                    newRef = new((REF)address) Unref2<T>(forwardInstance<Args>(args)...);

                    // Store, allocated object.
                    return Store<Unref2<T>>::store(*newRef);
                } catch (Throwable const& cse) {
                    // Storage failed.
                    if (newRef) return *newRef;
                    // Construction failed.
                    if (address != 0 && !newRef) freeMemory(address);

                    Error("Unable to create new object instance"_Sl, cse).throws($ftrace());
                } catch (std::exception const& cse) {
                    // Storage failed. (legacy code)
                    if (newRef) return *newRef;
                    // Construction failed.
                    if (address != 0 && !newRef) freeMemory(address);

                    Error("Unable to create new object instance"_Sl, Exception(String(cse.what())))
                            .throws($ftrace());
                } catch (...) {
                    // Construction failed.
                    if (address != 0 && !newRef) freeMemory(address);

                    Error("Unable to create new object instance"_Sl).throws($ftrace());
                }
            }

            /**
             * Allocates an instance at same memory address with specified instance, but does not run any constructor.
             * Initializes the class if it has not yet been.
             * @note  the oldInstance address must be writable (not constexpr)
             *
             * @note It's necessary that it exist +1 public corresponding constructor.
             */
            template <class T, class... Args>
            static Unref2<T>& replaceInstance(Object& replaceableObj, Args&&... args) {
                // basic check for T
                CORE_FULL_CHECK_SLIMMED_TYPE(Unref2<T>)
                // T mustn't be incomplete type (all incomplete types are not sizable)
                CORE_FAST_ASSERT(Class<T>::isComplete());
                // T mustn't be abstract type
                CORE_FAST_ASSERT(!Class<T>::isAbstract());
                // T must have constructor compatible, callable with given arguments.
                CORE_FAST_ASSERT(Class<T>::template isConstructible<Args...>());
                CORE_ASSERT(!ClassOf(null)::hasInstance(replaceableObj));
                try {
                    // construct desired object at memory address of replaceObj.
                    return *new(&replaceableObj) Unref2<T>(forwardInstance<Args>(args)...);
                } catch (Throwable const& cse) {
                    Error("Unable to create new object instance"_Sl, cse).throws($ftrace());
                } catch (std::exception const& cse) {
                    Error("Unable to create new object instance"_Sl, Exception(String(cse.what())))
                            .throws($ftrace());
                } catch (...) {
                    Error("Unable to create new object instance"_Sl).throws($ftrace());
                }
            }

            /**
             * Duplicate given instance or Return himself if given instance is always copy of another instance and
             * second argument is true.
             * The method is mostly used to create unique reference of desired object.
             * - for instances @c x and @c y, such as:
             *    @code
             *      y = Unsafe::copyInstance(x, false)
             *    @endcode ensure that:
             *   - @c x and @c y have similar address if @c x has been previously created with @c Unsafe::newInstance or @c Unsafe::copyInstance method.
             *   - @c x equals to @c y and, x and y haven't same address in otherwise
             * - for instances @c x and @c y, such as:
             *    @code
             *      y = Unsafe::copyInstance(x, true)
             *    @endcode ensure that:
             *    - @c x equals to @c y and, @c x and @c y have not similar address in otherwise
             * @tparam  T The type of original value.
             * @param original The instance to be copied
             * @param duplicate boolean value used to force this operation to create new instance.
             * @throws Error If duplication failed
             */
            template <class T>
            static Unref2<T>& copyInstance(T&& original, gbool duplicate = false) {
                CORE_CHECK_TYPE(Unref2<T>)
                if (Class<NIL>::isSame<T>() || Class<Thread>::isSame<T>() || original == null)
                    return CORE_CAST(Unref2<T> &, original);

                if (!duplicate && Class<Object>::hasInstance(original)) {
                    // Preserve old object if it already stored in backend.
                    Object const& object = CORE_XCAST(Object const, original);
                    if (isAlreadyAllocated(object))
                        return CORE_CAST(Unref2<T> &, original);

                    // Enable duplication because given object not exist into the database
                    // duplicate = true;
                }

                try {
                    return Instance<Unref2<T>>::newCopy(forwardInstance<T>(original));
                } catch (Throwable const& cse) {
                    Error("Unable to duplicate object of type "_Sl + typeName(original), cse).throws($ftrace());
                }
            }

            /**
             * Convert given value to rvalue reference.
             *
             * @param var The given value.
             */
            template <class T>
            static CORE_FAST Unref<T>&& moveInstance(T&& var) {
                return CORE_CAST(Unref<T>&&, var);
            }

            /**
             * Perfect forwarding.
             *
             * @param var The given value.
             */
            template <class T>
            static CORE_FAST T&& forwardInstance(Unref<T>& var) CORE_NOTHROW {
                return CORE_CAST(T&&, var);
            }

            /**
             * Perfect forwarding.
             *
             * @param var The given value.
             */
            template <class T>
            static CORE_FAST T&& forwardInstance(Unref<T>&& var) CORE_NOTHROW {
                CORE_FAST_XASSERT(!Class<T>::isLvalue(), "Forwarding is not supported by lvalue reference");
                return CORE_CAST(T&&, var);
            }

            /**
             * Swap two values
             */
            template <class From, class To = From>
            static void swapValues(From& from, To& to) {
                CORE_FAST_ASSERT(!Class<From>::isConstant());
                CORE_FAST_ASSERT(!Class<To>::isConstant());
                CORE_FAST_ASSERT(
                    Class<To>::template isAssignable<From>() || Class<From>::template isConstructible<To>());
                CORE_FAST_ASSERT(
                    Class<From>::template isAssignable<To>() || Class<To>::template isConstructible<From>());
                From f = moveInstance(from);
                from = moveInstance(to);
                to = moveInstance(f);
            }

            /**
             * Destroy and free instance created dynamically.
             * @note the given instance must be allocated by calling of Unsafe::newInstance() method.
             */
            template <class T>
            static gbool deleteInstance(T&& var, gbool optional = false) CORE_NOTHROW {
                CORE_FAST_ASSERT(!Class<T>::isPrimitive());
                if (!optional) {
                    Destructor<T>::destroy(forwardInstance<T>(var));
                    return true;
                }
                if (FORCE_COLLECTION_SMALL_MEMORY_USAGE) {
                    Destructor<T>::destroy(forwardInstance<T>(var));
                    return true;
                }
                return false;
            }

            /**
             * Destroy and free instance created dynamically.
             * @note the given instance must be allocated by calling of Unsafe::newInstance() method.
             */
            template <class T>
            static gbool deleteRegInstance(T&& var, gbool optional = false) CORE_NOTHROW {
                CORE_FAST_ASSERT(!Class<T>::isPrimitive());
                if (!optional) {
                    return Destructor<T>::destroyRegistered(forwardInstance<T>(var));
                }
                if (FORCE_COLLECTION_SMALL_MEMORY_USAGE)
                    return Destructor<T>::destroyRegistered(forwardInstance<T>(var));
                return false;
            }

            /**
             * Atomically updates class variable to @c x if it is currently
             * holding @c expected.
             *
             * <p>This operation has memory semantics of a @c volatile read
             * and write.  Corresponds to C11 atomic_compare_exchange_strong. </p>
             *
             * @return @c true if successful
             */
            static gbool compareAndSetReference(Object& o, glong offset,
                                                Object const& expected,
                                                Object& x);

            static Object& compareAndExchangeReference(Object& o, glong offset,
                                                       Object const& expected,
                                                       Object& x);

            static Object& compareAndExchangeReferenceAcquire(Object& o, glong offset,
                                                              Object const& expected,
                                                              Object& x);

            static Object& compareAndExchangeReferenceRelease(Object& o, glong offset,
                                                              Object const& expected,
                                                              Object& x);

            static gbool weakCompareAndSetReferencePlain(Object& o, glong offset,
                                                         Object const& expected,
                                                         Object& x);

            static gbool weakCompareAndSetReferenceAcquire(Object& o, glong offset,
                                                           Object const& expected,
                                                           Object& x);

            static gbool weakCompareAndSetReferenceRelease(Object& o, glong offset,
                                                           Object const& expected,
                                                           Object& x);

            static gbool weakCompareAndSetReference(Object& o, glong offset,
                                                    Object const& expected,
                                                    Object& x);

            /**
             * Atomically updates class variable to @c x if it is currently
             * holding @c expected.
             *
             * <p>This operation has memory semantics of a @c volatile read
             * and write.  Corresponds to C11 atomic_compare_exchange_strong. </p>
             *
             * @return @c true if successful
             */
            static gbool compareAndSetInt(Object& o, glong offset,
                                          gint expected,
                                          gint x);

            static gint compareAndExchangeInt(Object& o, glong offset,
                                              gint expected,
                                              gint x);

            static gint compareAndExchangeIntAcquire(Object& o, glong offset,
                                                     gint expected,
                                                     gint x);

            static gint compareAndExchangeIntRelease(Object& o, glong offset,
                                                     gint expected,
                                                     gint x);

            static gbool weakCompareAndSetIntPlain(Object& o, glong offset,
                                                   gint expected,
                                                   gint x);

            static gbool weakCompareAndSetIntAcquire(Object& o, glong offset,
                                                     gint expected,
                                                     gint x);

            static gbool weakCompareAndSetIntRelease(Object& o, glong offset,
                                                     gint expected,
                                                     gint x);

            static gbool weakCompareAndSetInt(Object& o, glong offset,
                                              gint expected,
                                              gint x);

            static gbool compareAndSetByte(Object& o, glong offset,
                                           gbyte expected,
                                           gbyte x);

            static gbyte compareAndExchangeByte(Object& o, glong offset,
                                                gbyte expected,
                                                gbyte x);

            static gbyte compareAndExchangeByteAcquire(Object& o, glong offset,
                                                       gbyte expected,
                                                       gbyte x);

            static gbyte compareAndExchangeByteRelease(Object& o, glong offset,
                                                       gbyte expected,
                                                       gbyte x);

            static gbool weakCompareAndSetBytePlain(Object& o, glong offset,
                                                    gbyte expected,
                                                    gbyte x);

            static gbool weakCompareAndSetByteAcquire(Object& o, glong offset,
                                                      gbyte expected,
                                                      gbyte x);

            static gbool weakCompareAndSetByteRelease(Object& o, glong offset,
                                                      gbyte expected,
                                                      gbyte x);

            static gbool weakCompareAndSetByte(Object& o, glong offset,
                                               gbyte expected,
                                               gbyte x);

            static gbool compareAndSetShort(Object& o, glong offset,
                                            gshort expected,
                                            gshort x);

            static gshort compareAndExchangeShort(Object& o, glong offset,
                                                  gshort expected,
                                                  gshort x);

            static gshort compareAndExchangeShortAcquire(Object& o, glong offset,
                                                         gshort expected,
                                                         gshort x);

            static gshort compareAndExchangeShortRelease(Object& o, glong offset,
                                                         gshort expected,
                                                         gshort x);

            static gbool weakCompareAndSetShortPlain(Object& o, glong offset,
                                                     gshort expected,
                                                     gshort x);

            static gbool weakCompareAndSetShortAcquire(Object& o, glong offset,
                                                       gshort expected,
                                                       gshort x);

            static gbool weakCompareAndSetShortRelease(Object& o, glong offset,
                                                       gshort expected,
                                                       gshort x);

            static gbool weakCompareAndSetShort(Object& o, glong offset,
                                                gshort expected,
                                                gshort x);

            static gbool compareAndSetChar(Object& o, glong offset,
                                           gchar expected,
                                           gchar x);

            static gchar compareAndExchangeChar(Object& o, glong offset,
                                                gchar expected,
                                                gchar x);

            static gchar compareAndExchangeCharAcquire(Object& o, glong offset,
                                                       gchar expected,
                                                       gchar x);

            static gchar compareAndExchangeCharRelease(Object& o, glong offset,
                                                       gchar expected,
                                                       gchar x);

            static gbool weakCompareAndSetCharPlain(Object& o, glong offset,
                                                    gchar expected,
                                                    gchar x);

            static gbool weakCompareAndSetCharAcquire(Object& o, glong offset,
                                                      gchar expected,
                                                      gchar x);

            static gbool weakCompareAndSetCharRelease(Object& o, glong offset,
                                                      gchar expected,
                                                      gchar x);

            static gbool weakCompareAndSetChar(Object& o, glong offset,
                                               gchar expected,
                                               gchar x);

            static gbool compareAndSetBoolean(Object& o, glong offset,
                                              gbool expected,
                                              gbool x);

            static gbool compareAndExchangeBoolean(Object& o, glong offset,
                                                   gbool expected,
                                                   gbool x);

            static gbool compareAndExchangeBooleanAcquire(Object& o, glong offset,
                                                          gbool expected,
                                                          gbool x);

            static gbool compareAndExchangeBooleanRelease(Object& o, glong offset,
                                                          gbool expected,
                                                          gbool x);

            static gbool weakCompareAndSetBooleanPlain(Object& o, glong offset,
                                                       gbool expected,
                                                       gbool x);

            static gbool weakCompareAndSetBooleanAcquire(Object& o, glong offset,
                                                         gbool expected,
                                                         gbool x);

            static gbool weakCompareAndSetBooleanRelease(Object& o, glong offset,
                                                         gbool expected,
                                                         gbool x);

            static gbool weakCompareAndSetBoolean(Object& o, glong offset,
                                                  gbool expected,
                                                  gbool x);

            static gbool compareAndSetFloat(Object& o, glong offset,
                                            gfloat expected,
                                            gfloat x);

            static gfloat compareAndExchangeFloat(Object& o, glong offset,
                                                  gfloat expected,
                                                  gfloat x);

            static gfloat compareAndExchangeFloatAcquire(Object& o, glong offset,
                                                         gfloat expected,
                                                         gfloat x);

            static gfloat compareAndExchangeFloatRelease(Object& o, glong offset,
                                                         gfloat expected,
                                                         gfloat x);

            static gbool weakCompareAndSetFloatPlain(Object& o, glong offset,
                                                     gfloat expected,
                                                     gfloat x);

            static gbool weakCompareAndSetFloatAcquire(Object& o, glong offset,
                                                       gfloat expected,
                                                       gfloat x);

            static gbool weakCompareAndSetFloatRelease(Object& o, glong offset,
                                                       gfloat expected,
                                                       gfloat x);

            static gbool weakCompareAndSetFloat(Object& o, glong offset,
                                                gfloat expected,
                                                gfloat x);

            static gbool compareAndSetDouble(Object& o, glong offset,
                                             gdouble expected,
                                             gdouble x);

            static gdouble compareAndExchangeDouble(Object& o, glong offset,
                                                    gdouble expected,
                                                    gdouble x);

            static gdouble compareAndExchangeDoubleAcquire(Object& o, glong offset,
                                                           gdouble expected,
                                                           gdouble x);

            static gdouble compareAndExchangeDoubleRelease(Object& o, glong offset,
                                                           gdouble expected,
                                                           gdouble x);

            static gbool weakCompareAndSetDoublePlain(Object& o, glong offset,
                                                      gdouble expected,
                                                      gdouble x);

            static gbool weakCompareAndSetDoubleAcquire(Object& o, glong offset,
                                                        gdouble expected,
                                                        gdouble x);

            static gbool weakCompareAndSetDoubleRelease(Object& o, glong offset,
                                                        gdouble expected,
                                                        gdouble x);

            static gbool weakCompareAndSetDouble(Object& o, glong offset,
                                                 gdouble expected,
                                                 gdouble x);

            /**
             * Atomically updates class variable to @c x if it is currently
             * holding @c expected.
             *
             * <p>This operation has memory semantics of a @c volatile read
             * and write.  Corresponds to C11 atomic_compare_exchange_strong. </p>
             *
             * @return @c true if successful
             */
            static gbool compareAndSetLong(Object& o, glong offset,
                                           glong expected,
                                           glong x);

            static glong compareAndExchangeLong(Object& o, glong offset,
                                                glong expected,
                                                glong x);

            static glong compareAndExchangeLongAcquire(Object& o, glong offset,
                                                       glong expected,
                                                       glong x);

            static glong compareAndExchangeLongRelease(Object& o, glong offset,
                                                       glong expected,
                                                       glong x);

            static gbool weakCompareAndSetLongPlain(Object& o, glong offset,
                                                    glong expected,
                                                    glong x);

            static gbool weakCompareAndSetLongAcquire(Object& o, glong offset,
                                                      glong expected,
                                                      glong x);

            static gbool weakCompareAndSetLongRelease(Object& o, glong offset,
                                                      glong expected,
                                                      glong x);

            static gbool weakCompareAndSetLong(Object& o, glong offset,
                                               glong expected,
                                               glong x);

            /**
             * Fetches a reference value from a given class variable, with volatile
             * load semantics. Otherwise, identical to @c Unsafe::getReference(Object, long)
             */
            static Object& getReferenceVolatile(Object const& o, glong offset);

            /**
             * Stores a reference value into a given class variable, with
             * volatile store semantics. Otherwise, identical to @c Unsafe::putReference(Object, long, Object)
             */
            static void putReferenceVolatile(Object& o, glong offset, Object& x);

            /** Volatile version of @c Unsafe::getInt(Object, long)  */
            static gint getIntVolatile(Object const& o, glong offset);

            /** Volatile version of @c Unsafe::putInt(Object, long, int)  */
            static void putIntVolatile(Object& o, glong offset, gint x);

            /** Volatile version of @c Unsafe::getBoolean(Object, long)  */
            static gbool getBooleanVolatile(Object const& o, glong offset);

            /** Volatile version of @c Unsafe::putBoolean(Object, long, bool)  */
            static void putBooleanVolatile(Object& o, glong offset, gbool x);

            /** Volatile version of @c Unsafe::getByte(Object, long)  */
            static gbyte getByteVolatile(Object const& o, glong offset);

            /** Volatile version of @c Unsafe::putByte(Object, long, byte)  */
            static void putByteVolatile(Object& o, glong offset, gbyte x);

            /** Volatile version of @c Unsafe::getShort(Object, long)  */
            static gshort getShortVolatile(Object const& o, glong offset);

            /** Volatile version of @c Unsafe::putShort(Object, long, short)  */
            static void putShortVolatile(Object& o, glong offset, gshort x);

            /** Volatile version of @c Unsafe::getChar(Object, long)  */
            static gchar getCharVolatile(Object const& o, glong offset);

            /** Volatile version of @c Unsafe::putChar(Object, long, char)  */
            static void putCharVolatile(Object& o, glong offset, gchar x);

            /** Volatile version of @c Unsafe::getLong(Object, long)  */
            static glong getLongVolatile(Object const& o, glong offset);

            /** Volatile version of @c Unsafe::putLong(Object, long, long)  */
            static void putLongVolatile(Object& o, glong offset, glong x);

            /** Volatile version of @c Unsafe::getFloat(Object, long)  */
            static gfloat getFloatVolatile(Object const& o, glong offset);

            /** Volatile version of @c Unsafe::putFloat(Object, long, float)  */
            static void putFloatVolatile(Object& o, glong offset, gfloat x);

            /** Volatile version of @c Unsafe::getDouble(Object, long)  */
            static gdouble getDoubleVolatile(Object const& o, glong offset);

            /** Volatile version of @c Unsafe::putDouble(Object, long, double)  */
            static void putDoubleVolatile(Object& o, glong offset, gdouble x);

            /** Acquire version of @c Unsafe::getReferenceVolatile(Object, long) */
            static Object& getReferenceAcquire(Object const& o, glong offset);

            /** Acquire version of @c Unsafe::getBooleanVolatile(Object, long) */
            static gbool getBooleanAcquire(Object const& o, glong offset);

            /** Acquire version of @c Unsafe::getByteVolatile(Object, long) */
            static gbyte getByteAcquire(Object const& o, glong offset);

            /** Acquire version of @c Unsafe::getShortVolatile(Object, long) */
            static gshort getShortAcquire(Object const& o, glong offset);

            /** Acquire version of @c Unsafe::getCharVolatile(Object, long) */
            static gchar getCharAcquire(Object const& o, glong offset);

            /** Acquire version of @c Unsafe::getIntVolatile(Object, long) */
            static gint getIntAcquire(Object const& o, glong offset);

            /** Acquire version of @c Unsafe::getLongVolatile(Object, long) */
            static glong getLongAcquire(Object const& o, glong offset);

            /** Acquire version of @c Unsafe::getFloatVolatile(Object, long) */
            static gfloat getFloatAcquire(Object const& o, glong offset);

            /** Acquire version of @c Unsafe::getDoubleVolatile(Object, long) */
            static gdouble getDoubleAcquire(Object const& o, glong offset);

            /*
             * Versions of @c Unsafe::putReferenceVolatile(Object, long, Object)
             * that do not guarantee immediate visibility of the store to
             * other threads. This method is generally only useful if the
             * underlying field is a class volatile (or if an array cell, one
             * that is otherwise only accessed using volatile accesses).
             *
             * Corresponds to C11 atomic_store_explicit(..., memory_order_release).
             */

            /** Release version of @c Unsafe::putReferenceVolatile(Object, long, Object) */
            static void putReferenceRelease(Object& o, glong offset, Object& x);

            /** Release version of @c Unsafe::putBooleanVolatile(Object, long, boolean) */
            static void putBooleanRelease(Object& o, glong offset, gbool x);

            /** Release version of @c Unsafe::putByteVolatile(Object, long, byte) */
            static void putByteRelease(Object& o, glong offset, gbyte x);

            /** Release version of @c Unsafe::putShortVolatile(Object, long, short) */
            static void putShortRelease(Object& o, glong offset, gshort x);

            /** Release version of @c Unsafe::putCharVolatile(Object, long, char) */
            static void putCharRelease(Object& o, glong offset, gchar x);

            /** Release version of @c Unsafe::putIntVolatile(Object, long, int) */
            static void putIntRelease(Object& o, glong offset, gint x);

            /** Release version of @c Unsafe::putLongVolatile(Object, long, long) */
            static void putLongRelease(Object& o, glong offset, glong x);

            /** Release version of @c Unsafe::putFloatVolatile(Object, long, float) */
            static void putFloatRelease(Object& o, glong offset, gfloat x);

            /** Release version of @c Unsafe::putDoubleVolatile(Object, long, double) */
            static void putDoubleRelease(Object& o, glong offset, gdouble x);

            // ------------------------------ Opaque --------------------------------------

            /** Opaque version of @c Unsafe::getReferenceVolatile(Object, long) */
            static Object& getReferenceOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::getBooleanVolatile(Object, long) */
            static gbool getBooleanOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::getByteVolatile(Object, long) */
            static gbyte getByteOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::getShortVolatile(Object, long) */
            static gshort getShortOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::getCharVolatile(Object, long) */
            static gchar getCharOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::getIntVolatile(Object, long) */
            static gint getIntOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::getLongVolatile(Object, long) */
            static glong getLongOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::getFloatVolatile(Object, long) */
            static gfloat getFloatOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::getDoubleVolatile(Object, long) */
            static gdouble getDoubleOpaque(Object const& o, glong offset);

            /** Opaque version of @c Unsafe::putReferenceVolatile(Object, long, Object) */
            static void putReferenceOpaque(Object& o, glong offset, Object& x);

            /** Opaque version of @c Unsafe::putBooleanVolatile(Object, long, boolean) */
            static void putBooleanOpaque(Object& o, glong offset, gbool x);

            /** Opaque version of @c Unsafe::putByteVolatile(Object, long, byte) */
            static void putByteOpaque(Object& o, glong offset, gbyte x);

            /** Opaque version of @c Unsafe::putShortVolatile(Object, long, short) */
            static void putShortOpaque(Object& o, glong offset, gshort x);

            /** Opaque version of @c Unsafe::putCharVolatile(Object, long, char) */
            static void putCharOpaque(Object& o, glong offset, gchar x);

            /** Opaque version of @c Unsafe::putIntVolatile(Object, long, int) */
            static void putIntOpaque(Object& o, glong offset, gint x);

            /** Opaque version of @c Unsafe::putLongVolatile(Object, long, long) */
            static void putLongOpaque(Object& o, glong offset, glong x);

            /** Opaque version of @c Unsafe::putFloatVolatile(Object, long, float) */
            static void putFloatOpaque(Object& o, glong offset, gfloat x);

            /** Opaque version of @c Unsafe::putDoubleVolatile(Object, long, double) */
            static void putDoubleOpaque(Object& o, glong offset, gdouble x);

            /**
             * Unblocks the given thread blocked on @c park, or, if it is
             * not blocked, causes the subsequent call to @c park not to
             * block.  Note: this operation is "unsafe" solely because the
             * caller must somehow ensure that the thread has not been
             * destroyed. Nothing special is usually required to ensure this
             * when called from special methods (in which there will ordinarily be a live
             * reference to the thread) but this is not nearly-automatically
             * so when calling from native code.
             *
             * @param thread the thread to unpark.
             */
            static void unpark(Object& thread);

            /**
             * Blocks current thread, returning when a balancing
             * @c unpark occurs, or a balancing @c unpark has
             * already occurred, or the thread is interrupted, or, if not
             * absolute and time is not zero, the given time nanoseconds have
             * elapsed, or if absolute, the given deadline in milliseconds
             * since Epoch has passed, or spuriously (i.e., returning for no
             * "reason"). Note: This operation is in the Unsafe class only
             * because @c unpark is, so it would be strange to place it
             * elsewhere.
             */
            static void park(gbool isAbsolute, glong time);

            // The following contain CAS-based implementations used on
            // platforms not supporting native instructions

            /**
             * Atomically adds the given value to the current value of a field
             * or array element within the given object @c o
             * at the given @c offset.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param delta the value to add
             * @return the previous value
             */
            static gint getAndAddInt(Object& o, glong offset, gint delta);

            static gint getAndAddIntRelease(Object& o, glong offset, gint delta);

            static gint getAndAddIntAcquire(Object& o, glong offset, gint delta);

            /**
             * Atomically adds the given value to the current value of a field
             * or array element within the given object @c o
             * at the given @c offset.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param delta the value to add
             * @return the previous value
             */
            static glong getAndAddLong(Object& o, glong offset, glong delta);

            static glong getAndAddLongRelease(Object& o, glong offset, glong delta);

            static glong getAndAddLongAcquire(Object& o, glong offset, glong delta);

            static gbyte getAndAddByte(Object& o, glong offset, gbyte delta);

            static gbyte getAndAddByteRelease(Object& o, glong offset, gbyte delta);

            static gbyte getAndAddByteAcquire(Object& o, glong offset, gbyte delta);

            static gshort getAndAddShort(Object& o, glong offset, gshort delta);

            static gshort getAndAddShortRelease(Object& o, glong offset, gshort delta);

            static gshort getAndAddShortAcquire(Object& o, glong offset, gshort delta);

            static gfloat getAndAddFloat(Object& o, glong offset, gfloat delta);

            static gfloat getAndAddFloatRelease(Object& o, glong offset, gfloat delta);

            static gfloat getAndAddFloatAcquire(Object& o, glong offset, gfloat delta);

            static gdouble getAndAddDouble(Object& o, glong offset, gdouble delta);

            static gdouble getAndAddDoubleRelease(Object& o, glong offset, gdouble delta);

            static gdouble getAndAddDoubleAcquire(Object& o, glong offset, gdouble delta);

            /**
             * Atomically exchanges the given value with the current value of
             * a field or array element within the given object @c o
             * at the given @c offset.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param newValue new value
             * @return the previous value
             */
            static gint getAndSetInt(Object& o, glong offset, gint newValue);

            static gint getAndSetIntRelease(Object& o, glong offset, gint newValue);

            static gint getAndSetIntAcquire(Object& o, glong offset, gint newValue);

            /**
             * Atomically exchanges the given value with the current value of
             * a field or array element within the given object @c o
             * at the given @c offset.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param newValue new value
             * @return the previous value
             */
            static glong getAndSetLong(Object& o, glong offset, glong newValue);

            static glong getAndSetLongRelease(Object& o, glong offset, glong newValue);

            static glong getAndSetLongAcquire(Object& o, glong offset, glong newValue);

            /**
             * Atomically exchanges the given reference value with the current
             * reference value of a field or array element within the given
             * object @c o at the given @c offset.
             *
             * @param o object/array to update the field/element in
             * @param offset field/element offset
             * @param newValue new value
             * @return the previous value
             */
            static Object& getAndSetReference(Object& o, glong offset, Object& newValue);

            static Object& getAndSetReferenceRelease(Object& o, glong offset, Object& newValue);

            static Object& getAndSetReferenceAcquire(Object& o, glong offset, Object& newValue);

            static gbool getAndSetBoolean(Object& o, glong offset, gbool newValue);

            static gbool getAndSetBooleanRelease(Object& o, glong offset, gbool newValue);

            static gbool getAndSetBooleanAcquire(Object& o, glong offset, gbool newValue);

            static gbyte getAndSetByte(Object& o, glong offset, gbyte newValue);

            static gbyte getAndSetByteRelease(Object& o, glong offset, gbyte newValue);

            static gbyte getAndSetByteAcquire(Object& o, glong offset, gbyte newValue);

            static gshort getAndSetShort(Object& o, glong offset, gshort newValue);

            static gshort getAndSetShortRelease(Object& o, glong offset, gshort newValue);

            static gshort getAndSetShortAcquire(Object& o, glong offset, gshort newValue);

            static gfloat getAndSetFloat(Object& o, glong offset, gfloat newValue);

            static gfloat getAndSetFloatRelease(Object& o, glong offset, gfloat newValue);

            static gfloat getAndSetFloatAcquire(Object& o, glong offset, gfloat newValue);

            static gdouble getAndSetDouble(Object& o, glong offset, gdouble newValue);

            static gdouble getAndSetDoubleRelease(Object& o, glong offset, gdouble newValue);

            static gdouble getAndSetDoubleAcquire(Object& o, glong offset, gdouble newValue);


            // The following contain CAS-based implementations used on
            // platforms not supporting native instructions

            static gbool getAndBitwiseOrBoolean(Object& o, glong offset, gbool mask);

            static gbool getAndBitwiseOrBooleanRelease(Object& o, glong offset, gbool mask);

            static gbool getAndBitwiseOrBooleanAcquire(Object& o, glong offset, gbool mask);

            static gbool getAndBitwiseAndBoolean(Object& o, glong offset, gbool mask);

            static gbool getAndBitwiseAndBooleanRelease(Object& o, glong offset, gbool mask);

            static gbool getAndBitwiseAndBooleanAcquire(Object& o, glong offset, gbool mask);

            static gbool getAndBitwiseXorBoolean(Object& o, glong offset, gbool mask);

            static gbool getAndBitwiseXorBooleanRelease(Object& o, glong offset, gbool mask);

            static gbool getAndBitwiseXorBooleanAcquire(Object& o, glong offset, gbool mask);

            static gbyte getAndBitwiseOrByte(Object& o, glong offset, gbyte mask);

            static gbyte getAndBitwiseOrByteRelease(Object& o, glong offset, gbyte mask);

            static gbyte getAndBitwiseOrByteAcquire(Object& o, glong offset, gbyte mask);

            static gbyte getAndBitwiseAndByte(Object& o, glong offset, gbyte mask);

            static gbyte getAndBitwiseAndByteRelease(Object& o, glong offset, gbyte mask);

            static gbyte getAndBitwiseAndByteAcquire(Object& o, glong offset, gbyte mask);

            static gbyte getAndBitwiseXorByte(Object& o, glong offset, gbyte mask);

            static gbyte getAndBitwiseXorByteRelease(Object& o, glong offset, gbyte mask);

            static gbyte getAndBitwiseXorByteAcquire(Object& o, glong offset, gbyte mask);

            static gshort getAndBitwiseOrShort(Object& o, glong offset, gshort mask);

            static gshort getAndBitwiseOrShortRelease(Object& o, glong offset, gshort mask);

            static gshort getAndBitwiseOrShortAcquire(Object& o, glong offset, gshort mask);

            static gshort getAndBitwiseAndShort(Object& o, glong offset, gshort mask);

            static gshort getAndBitwiseAndShortRelease(Object& o, glong offset, gshort mask);

            static gshort getAndBitwiseAndShortAcquire(Object& o, glong offset, gshort mask);

            static gshort getAndBitwiseXorShort(Object& o, glong offset, gshort mask);

            static gshort getAndBitwiseXorShortRelease(Object& o, glong offset, gshort mask);

            static gshort getAndBitwiseXorShortAcquire(Object& o, glong offset, gshort mask);

            static gchar getAndBitwiseOrChar(Object& o, glong offset, gchar mask);

            static gchar getAndBitwiseOrCharRelease(Object& o, glong offset, gchar mask);

            static gchar getAndBitwiseOrCharAcquire(Object& o, glong offset, gchar mask);

            static gchar getAndBitwiseAndChar(Object& o, glong offset, gchar mask);

            static gchar getAndBitwiseAndCharRelease(Object& o, glong offset, gchar mask);

            static gchar getAndBitwiseAndCharAcquire(Object& o, glong offset, gchar mask);

            static gchar getAndBitwiseXorChar(Object& o, glong offset, gchar mask);

            static gchar getAndBitwiseXorCharRelease(Object& o, glong offset, gchar mask);

            static gchar getAndBitwiseXorCharAcquire(Object& o, glong offset, gchar mask);

            static gint getAndBitwiseOrInt(Object& o, glong offset, gint mask);

            static gint getAndBitwiseOrIntRelease(Object& o, glong offset, gint mask);

            static gint getAndBitwiseOrIntAcquire(Object& o, glong offset, gint mask);

            static gint getAndBitwiseAndInt(Object& o, glong offset, gint mask);

            static gint getAndBitwiseAndIntRelease(Object& o, glong offset, gint mask);

            static gint getAndBitwiseAndIntAcquire(Object& o, glong offset, gint mask);

            static gint getAndBitwiseXorInt(Object& o, glong offset, gint mask);

            static gint getAndBitwiseXorIntRelease(Object& o, glong offset, gint mask);

            static gint getAndBitwiseXorIntAcquire(Object& o, glong offset, gint mask);

            static glong getAndBitwiseOrLong(Object& o, glong offset, glong mask);

            static glong getAndBitwiseOrLongRelease(Object& o, glong offset, glong mask);

            static glong getAndBitwiseOrLongAcquire(Object& o, glong offset, glong mask);

            static glong getAndBitwiseAndLong(Object& o, glong offset, glong mask);

            static glong getAndBitwiseAndLongRelease(Object& o, glong offset, glong mask);

            static glong getAndBitwiseAndLongAcquire(Object& o, glong offset, glong mask);

            static glong getAndBitwiseXorLong(Object& o, glong offset, glong mask);

            static glong getAndBitwiseXorLongRelease(Object& o, glong offset, glong mask);

            static glong getAndBitwiseXorLongAcquire(Object& o, glong offset, glong mask);

            /**
             * Ensures that loads before the fence will not be reordered with loads and
             * stores after the fence; a "LoadLoad plus LoadStore barrier".
             *
             * Corresponds to C11 atomic_thread_fence(memory_order_acquire)
             * (an "acquire fence").
             *
             * Provides a LoadLoad barrier followed by a LoadStore barrier.
             */
            static void loadFence();

            /**
             * Ensures that loads and stores before the fence will not be reordered with
             * stores after the fence; a "StoreStore plus LoadStore barrier".
             *
             * Corresponds to C11 atomic_thread_fence(memory_order_release)
             * (a "release fence").
             *
             * Provides a StoreStore barrier followed by a LoadStore barrier.
             */
            static void storeFence();

            /**
             * Ensures that loads and stores before the fence will not be reordered
             * with loads and stores after the fence.  Implies the effects of both
             * loadFence() and storeFence(), and in addition, the effect of a StoreLoad
             * barrier.
             *
             * Corresponds to C11 atomic_thread_fence(memory_order_seq_cst).
             */
            static void fullFence();

#undef BIG_ENDIAN

            /**
             * Flag whose value is true if and only if the native endianness
             * of this platform is big.
             *
             * @implNote
             * The actual value for this field is injected by the JVM.
             */
            static CORE_FAST gbool BIG_ENDIAN = CORE_BYTE_ORDER == CORE_BIG_ENDIAN;

            /**
             * Fetches a value at some byte offset into a given class object.
             * More specifically, fetches a value within the given object
             * <code>o</code> at the given offset, or (if <code>o</code> is
             * null) from the memory address whose numerical value is the
             * given offset.
             * <p>
             * The specification of this method is the same as
             * @c Unsafe::getLong(Object, long) except that the offset does not need to
             * have been obtained from @c Unsafe::offsetof
             * of some class field.  The value
             * in memory is raw data, and need not correspond to any class
             * variable.  Unless <code>o</code> is null, the value accessed
             * must be entirely within the allocated object.  The endianness
             * of the value in memory is the endianness of the native platform. </p>
             *
             * <p> The read will be atomic with respect to the largest power
             * of two that divides the GCD of the offset and the storage size.
             * For example, getLongUnaligned will make atomic reads of 2-, 4-,
             * or 8-byte storage units if the offset is zero mod 2, 4, or 8,
             * respectively.  There are no other guarantees of atomicity. </p>
             * <p>
             * 8-byte atomicity is only guaranteed on platforms on which
             * support atomic accesses to longs. </p>
             *
             * @param o class heap object in which the value resides, if any, else
             *        null
             * @param offset The offset in bytes from the start of the object
             * @return the value fetched from the indicated object
             * @throws RuntimeException No defined exceptions are thrown
             */
            static glong getLongUnaligned(Object const& o, glong offset);

            /**
             * As @c Unsafe::getLongUnaligned(Object, long) but with an
             * additional argument which specifies the endianness of the value
             * as stored in memory.
             *
             * @param o class heap object in which the variable resides
             * @param offset The offset in bytes from the start of the object
             * @param bigEndian The endianness of the value
             * @return the value fetched from the indicated object
             */
            static glong getLongUnaligned(Object const& o, glong offset, gbool bigEndian);

            static gint getIntUnaligned(Object const& o, glong offset);

            static gint getIntUnaligned(Object const& o, glong offset, gbool bigEndian);

            static gshort getShortUnaligned(Object const& o, glong offset);

            static gshort getShortUnaligned(Object const& o, glong offset, gbool bigEndian);

            static gchar getCharUnaligned(Object const& o, glong offset);

            static gchar getCharUnaligned(Object const& o, glong offset, gbool bigEndian);

            /**
             * Stores a value at some byte offset into a given class object.
             * <p>
             * The specification of this method is the same as
             * @c Unsafe::getLong(Object, long) except that the offset does not need to
             * have been obtained from @c Unsafe::offsetof
             * of some class field.  The value
             * in memory is raw data, and need not correspond to any class
             * variable.  The endianness of the value in memory is the
             * endianness of the native platform. </p>
             * <p>
             * The writing will be atomic with respect to the largest power of
             * two that divides the GCD of the offset and the storage size.
             * For example, putLongUnaligned will make atomic writes of 2-, 4-,
             * or 8-byte storage units if the offset is zero mod 2, 4, or 8,
             * respectively.  There are no other guarantees of atomicity. </p>
             * <p>
             * 8-byte atomicity is only guaranteed on platforms on which
             * support atomic accesses to longs. </p>
             *
             * @param o class heap object in which the value resides, if any, else
             *        null
             * @param offset The offset in bytes from the start of the object
             * @param x the value to store
             * @throws RuntimeException No defined exceptions are thrown
             */
            static void putLongUnaligned(Object& o, glong offset, glong x);

            /**
             * As @c Unsafe::putLongUnaligned(Object, long, long) but with an additional
             * argument which specifies the endianness of the value as stored in memory.
             * @param o class heap object in which the value resides
             * @param offset The offset in bytes from the start of the object
             * @param x the value to store
             * @param bigEndian The endianness of the value
             * @throws RuntimeException No defined exceptions are thrown
             */
            static void putLongUnaligned(Object& o, glong offset, glong x, gbool bigEndian);

            /** @see Unsafe::putLongUnaligned(Object, long, long) */
            static void putIntUnaligned(Object& o, glong offset, gint x);

            /** @see Unsafe::putLongUnaligned(Object, long, long, boolean) */
            static void putIntUnaligned(Object& o, glong offset, gint x, gbool bigEndian);

            /** @see Unsafe::putLongUnaligned(Object, long, long) */
            static void putShortUnaligned(Object& o, glong offset, gshort x);

            /** @see Unsafe::putLongUnaligned(Object, long, long, boolean) */
            static void putShortUnaligned(Object& o, glong offset, gshort x, gbool bigEndian);

            /** @see Unsafe::putLongUnaligned(Object, long, long) */
            static void putCharUnaligned(Object& o, glong offset, gchar x);

            /** @see Unsafe::putLongUnaligned(Object, long, long, boolean) */
            static void putCharUnaligned(Object& o, glong offset, gchar x, gbool bigEndian);

        private:
            /**
             * Check if address of object exist on the caches.
             */
            static gbool isAlreadyAllocated(Object const& object);

            /**
             * Store allocated object address for reuse.
             */
            static void storeReference(Object& object);

            static gbool deleteReference(Object& object);

            template <class T,
                      gbool isCloneable = Class<Object>::isSuper<T>(),
                      gbool isCopyable = Class<T>::template isConstructible<T const&>()>
            class Instance final : public Object {
            public:
                CORE_FAST_ASSERT(isCloneable | isCopyable);

                static T& newCopy(T const& original) { CORE_UNREACHABLE(); }

                static T& newCopy(T&& original) { CORE_UNREACHABLE(); }
            };

            template <class T>
            class Instance<T, true, true> final : public Object {
            public:
                static T& newCopy(T const& original) CORE_NOTHROW {
                    // try copy instance by cloning.
                    try {
                        return Instance<T, true, false>::newCopy(original);
                    } catch (CloneNotSupportedException const&) {
                        // try copy instance by copy constructor.
                        try {
                            return Instance<T, false, true>::newCopy(original);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); };
                    }
                    catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                static T& newCopy(T&& original) CORE_NOTHROW {
                    // try copy instance by move constructor.
                    try {
                        return Instance<T, false, true>::newCopy(moveInstance(original));
                    } catch (CloneNotSupportedException const& _) {
                        // try copy instance by cloning.
                        try {
                            return Instance<T, true, false>::newCopy(original);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); };
                    }
                    catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            };

            template <class T>
            class Instance<T, false, true> final : public Object {
            public:
                static T& newCopy(T const& original) CORE_NOTHROW {
                    // try copy instance by duplication
                    try {
                        return newInstance<T>(original);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                static T& newCopy(T&& original) CORE_NOTHROW {
                    try {
                        return newInstance<T>(moveInstance(original));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            };

            template <class T>
            class Instance<T, true, false> final : public Object {
            public:
                static T& newCopy(T const& original) {
                    // try cast via Class.hasInstance to evict exception during dynamic cast.
                    if (Class<Object>::hasInstance(original)) {
                        Object const& obj = CORE_XCAST(Object const, original);
                        Object& copy = obj.clone();

                        // assert(Class<T>::hasInstance(copy));
                        return CORE_XCAST(T, copy);
                    }
                    // cast fail.
                    CloneNotSupportedException().throws($ftrace());
                }
            };

            /**
             * Return the direct access address
             *
             * @param o the target object.
             * @param offset the offset
             * @return the direct access address.
             */
            static glong tryGetDirectAccess(Object const& o, glong offset);

            static glong array2DirectAccess(Object const& o, glong offset);

            static glong array2DirectAccess2(Object const& o, glong offset);

            template <class T, gbool isObject = Class<Object>::isSuper<T>()>
            class Store final {
            public:
                static CORE_FAST T& store(T& obj) { return obj; }
            };

            template <class T>
            class Store<T, true> final {
            public:
                static T& store(T& obj) {
                    storeReference(CORE_XCAST(Object, obj));
                    return obj;
                }
            };

            template <class T, gbool isObject = Class<Object>::isSuper<T>()>
            class Destructor final {
            public:
                static void destroy(T&& obj) { delete &obj; }

                static gbool destroyRegistered(T&&) { return false; }
            };

            template <class T>
            class Destructor<T, true> final {
            public:
                static void destroy(T&& obj) {
                    Unref2<T>& target = CORE_CAST(Unref2<T> &, obj);
                    if (!deleteReference(target)) delete &target;
                }

                static gbool destroyRegistered(T&& obj) {
                    Unref2<T>& target = CORE_CAST(Unref2<T> &, obj);
                    return deleteReference(target);
                }
            };

        public:
#if defined(CORE_COMPILER_MSVC) and !defined(CORE_UNICODE_MSVC)
            static CORE_FAST gint ANSI_LITERAL = 1;
#else
            static CORE_FAST gint ANSI_LITERAL = 0;
#endif

            /**
             * The property used to determine if after any collection
             * operations, all allocated instance must be destroyed.
             * In other-word if FORCE_COLLECTION_SMALL_MEMORY_USAGE is
             * false the programs will be destroyed all allocated instance
             * after the end of execution of program. if it's true
             * the instance allocated into function/methods will be
             * destroyed after execution of function/methods like
             * local variables.
             *
             * Example:
             * @li case 1: FORCE_COLLECTION_SMALL_MEMORY_USAGE = false
             * @code
             *      class MyClass: public Collection<String> {
             *          Collection<String> &c;
             *
             *      public:
             *          String toString() const {
             *              Iterator<String> &it = c.iterator();
             *              XString xs = XString();
             *              while(it.hasNext()) xs.append(it.next());
             *              return xs.toString();
             *          }
             *      };
             * @endcode
             * @li case 2: FORCE_COLLECTION_SMALL_MEMORY_USAGE = true
             * @code
             *      class MyClass: public Collection<String> {
             *          Collection<String> &c;
             *
             *      public:
             *          String toString() const {
             *              Iterator<String> &it = c.iterator();
             *              XString xs = XString();
             *              while(it.hasNext()) xs.append(it.next());
             *              Unsafe::deleteInstance(it);
             *              return xs.toString();
             *          }
             *      };
             * @endcode
             *
             * @see List::of
             * @see Set::of
             * @see Map::of
             * @see Map::ofEntries
             */
            static gint CORE_FAST FORCE_COLLECTION_SMALL_MEMORY_USAGE = CORE_FORCE_SMALL_MEMORY_USAGE;

            /**
             * The property used select charset version.
             */
            static gint CORE_FAST USE_CHARSET_GB18030_2000 = CORE_CHARSET_GB18030_VERSION == 2000;

            static gint CORE_FAST USE_LOCALE_OLD_ISO_CODES = CORE_USE_LOCALE_OLD_ISO_CODES;

        private:
            CORE_ADD_AS_FRIEND(Thread);

            static glong threadCount();
        };

        CORE_WARNING_POP
    } // misc
} // core

#endif //CORE24_UNSAFE_H
