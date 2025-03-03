//
// Created by admin on 19/01/25.
//

#ifndef CORE24_VECTOR_H
#define CORE24_VECTOR_H

#include <core/concurrent/ReentrantLock.h>
#include <core/lang/LongArray.h>
#include <core/util/Optional.h>
#include <core/util/List.h>

namespace core {
    namespace util {
        /**
         * The @c Vector class implements a growable array of
         * objects. Like an array, it contains components that can be
         * accessed using an integer index. However, the size of a
         * @c Vector can grow or shrink as needed to accommodate
         * adding and removing items after the @c Vector has been created.
         *
         * <p>
         * Each vector tries to optimize storage management by maintaining a
         * @c capacity and a @c capacityIncrement. The
         * @c capacity is always at least as large as the vector
         * size; it is usually larger because as components are added to the
         * vector, the vector's storage increases in chunks the size of
         * @c capacityIncrement. An application can increase the
         * capacity of a vector before inserting a large number of
         * components; this reduces the amount of incremental reallocation.
         * </p>
         * <p>
         * The iterators returned by this class's @b iterator and
         * @b listIterator methods are <em>fail-fast</em>:
         * if the vector is structurally modified at any time after the iterator is
         * created, in any way except through the iterator's own
         * @b remove or @b add methods, the iterator will throw a
         * @b ConcurrentModificationException.  Thus, in the face of
         * concurrent modification, the iterator fails quickly and cleanly, rather
         * than risking arbitrary, non-deterministic behavior at an undetermined
         * time in the future.
         * </p>
         * <p>
         * Note that the fail-fast behavior of an iterator cannot be guaranteed
         * as it is, generally speaking, impossible to make any hard guarantees in the
         * presence of unsynchronized concurrent modification.  Fail-fast iterators
         * throw @c ConcurrentModificationException on a best-effort basis.
         * Therefore, it would be wrong to write a program that depended on this
         * exception for its correctness:  <i>the fail-fast behavior of iterators
         * should be used only to detect bugs.</i>
         * </p>
         * <p>
         * Unlike the new collection implementations, @c Vector is synchronized.
         * If a thread-safe implementation is not needed, it is recommended to use
         * @b ArrayList in place of @c Vector.
         * </p>
         * @tparam E Type of component elements
         *
         * @see Collection
         * @see LinkedList
         */
        template <class E>
        class Vector final : public virtual List<E>, public virtual RandomAccess {
            /**
             * The array buffer into which the components of the vector are
             * stored. The capacity of the vector is the length of this array buffer,
             * and is at least large enough to contain all the vector's elements.
             *
             * <p>
             * Any array elements following the last element in the Vector are null.
             * </p>
             */
            Array<> data;

            /**
             * The number of valid components in this @c Vector object.
             * Components @c data[0] through @c data[count-1] are the actual items.
             */
            gint count = 0;

            /**
             * The amount by which the capacity of the vector is automatically
             * incremented when its size becomes greater than its capacity.  If
             * the capacity increment is less than or equal to zero, the capacity
             * of the vector is doubled each time it needs to grow.
             */
            gint increment = 0;

            CORE_IMPORT_FIELD_OR_METHOD(List<E>, modCount);
            CORE_IMPORT_FIELD_OR_METHOD(List<E>, reverse);

        public:
            /**
             * Constructs an empty vector with the specified initial capacity and
             * capacity increment.
             *
             * @param   initialCapacity     the initial capacity of the vector
             * @param   capacityIncrement   the amount by which the capacity is
             *                              increased when the vector overflows
             * @throws IllegalArgumentException if the specified initial capacity
             *         is negative
             */
            CORE_EXPLICIT Vector(gint initialCapacity, gint capacityIncrement) {
                if (initialCapacity < 0)
                    IllegalArgumentException("Illegal Capacity: "_Sl + initialCapacity).throws($ftrace());
                data = Array<>(initialCapacity);
                increment = capacityIncrement;
            }

            /**
             * Constructs an empty vector with the specified initial capacity and
             * with its capacity increment equal to zero.
             *
             * @param   initialCapacity   the initial capacity of the vector
             * @throws IllegalArgumentException if the specified initial capacity
             *         is negative
             */
            CORE_EXPLICIT Vector(gint initialCapacity): Vector(initialCapacity, 0) {}

            /**
             * Constructs an empty vector so that its internal data array
             * has size @c 10 and its standard capacity increment is
             * zero.
             */
            Vector(): Vector(10) {}

            /**
             * Constructs a vector containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param c the collection whose elements are to be placed into this
             *       vector
             */
            CORE_EXPLICIT Vector(Collection<E> const& c) {
                try {
                    data = (Array<>) c.toArray();
                    count = data.length();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Constructs a vector containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param c the collection whose elements are to be placed into this
             *       vector
             */
            template <class X = Object, ClassOf(1)::OnlyIf<Class<E>::template isSuper<X>()>  = 1>
            CORE_EXPLICIT Vector(Collection<X> const& c) {
                try {
                    data = (Array<>) c.toArray();
                    count = data.length();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Copies the components of this vector into the specified array.
             * The item at index @c k in this vector is copied into
             * component @c k of @c dst.
             *
             * @param  dst the array into which the components get copied
             * @throws IndexOutOfBoundsException if the specified array is not
             *         large enough to hold all the components of this vector
             * @throws ClassCastException if a component of this vector is not of
             *         a runtime type that can be stored in the specified array
             * @see toArray(Object[])
             */
            template <class X = Object>
            CORE_SYNC void copyTo(Array<X>& dst) const {
                try {
                    AutoSync sync = AutoSync(*this);
                    Arrays::copy(data, 0, dst, 0, count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Trims the capacity of this vector to be the vector's current
             * size. If the capacity of this vector is larger than its current
             * size, then the capacity is changed to equal the size by replacing
             * its internal data array, kept in the field @c data,
             * with a smaller one. An application can use this operation to
             * minimize the storage of a vector.
             */
            CORE_SYNC void trimToSize() {
                try {
                    AutoSync sync = AutoSync(*this);
                    modCount += 1;
                    gint capacity = data.length();
                    if (count < capacity)
                        data = Arrays::copyOf(data, count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Increases the capacity of this vector, if necessary, to ensure
             * that it can hold at least the number of components specified by
             * the minimum capacity argument.
             *
             * <p>
             * If the current capacity of this vector is less than
             * @c minCapacity, then its capacity is increased by replacing its
             * internal data array, kept in the field @c data, with a
             * larger one.  The size of the new data array will be the old size plus
             * @c increment, unless the value of
             * @c increment is less than or equal to zero, in which case
             * the new capacity will be twice the old capacity; but if this new size
             * is still smaller than @c minCapacity, then the new capacity will
             * be @c minCapacity.
             * </p>
             * @param minCapacity the desired minimum capacity
             */
            CORE_SYNC void ensureCapacity(gint minCapacity) {
                if (minCapacity > 0) {
                    try {
                        AutoSync sync = AutoSync(*this);
                        if (minCapacity > data.length()) {
                            modCount += 1;
                            resize(minCapacity);
                        }
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            }

            /**
             * Sets the size of this vector. If the new size is greater than the
             * current size, new @c null items are added to the end of
             * the vector. If the new size is less than the current size, all
             * components at index @c newSize and greater are discarded.
             *
             * @param  newSize   the new size of this vector
             * @throws IndexOutOfBoundsException if the new size is negative
             */
            CORE_SYNC void setSize(gint newSize) {
                try {
                    AutoSync sync = AutoSync(*this);
                    if (newSize > data.length())
                        resize(newSize);
                    for (gint to = count, i = newSize; i < to; i += 1)
                        data.set(i, null);
                    count = newSize;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the current capacity of this vector.
             *
             * @return  the current capacity (the length of its internal
             *          data array, kept in the field @c data
             *          of this vector)
             */
            CORE_SYNC gint capacity() const {
                try {
                    AutoSync sync = AutoSync(*this);
                    return data.length();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the number of components in this vector.
             *
             * @return  the number of components in this vector
             */
            CORE_SYNC gint size() const {
                try {
                    AutoSync sync = AutoSync(*this);
                    return count;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Tests if this vector has no components.
             *
             * @return  @c true if and only if this vector has
             *          no components, that is, its size is zero;
             *          @c false otherwise.
             */
            CORE_SYNC gbool isEmpty() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return count == 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns @c true if this vector contains the specified element.
             * More formally, returns @c true if and only if this vector
             * contains at least one element @c e such that
             * @c Objects::equals(o, e).
             *
             * @param o element whose presence in this vector is to be tested
             * @return @c true if this vector contains the specified element
             */
            gbool contains(E const& o) const override {
                try {
                    return indexOf(o) >= 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the index of the first occurrence of the specified element
             * in this vector, or -1 if this vector does not contain the element.
             * More formally, returns the lowest index @c i such that
             * @c Objects.equals(o, get(i)),
             * or -1 if there is no such index.
             *
             * @param o element to search for
             * @return the index of the first occurrence of the specified element in
             *         this vector, or -1 if this vector does not contain the element
             */
            gint indexOf(E const& o) const override {
                try {
                    return indexOf(o, 0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the index of the first occurrence of the specified element in
             * this vector, searching forwards from @c index, or returns -1 if
             * the element is not found.
             * More formally, returns the lowest index @c i such that
             * @code (i >= index && Objects.equals(o, get(i))) @endcode ,
             * or -1 if there is no such index.
             *
             * @param o element to search for
             * @param index index to start searching from
             * @return the index of the first occurrence of the element in
             *         this vector at position @c index or later in the vector;
             *         @c -1 if the element is not found.
             * @throws IndexOutOfBoundsException if the specified index is negative
             * @see     Object::equals(Object)
             */
            CORE_SYNC gint indexOf(E const& o, gint index) const {
                try {
                    AutoSync sync = AutoSync(*this);
                    for (gint i = index; i < count; i += 1)
                        if (o.equals(data.getOrNull(i)))
                            return i;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return -1;
            }

            /**
             * Returns the index of the last occurrence of the specified element
             * in this vector, or -1 if this vector does not contain the element.
             * More formally, returns the highest index @c i such that
             * @code Objects::equals(o, get(i)) @endcode ,
             * or -1 if there is no such index.
             *
             * @param o element to search for
             * @return the index of the last occurrence of the specified element in
             *         this vector, or -1 if this vector does not contain the element
             */
            gint lastIndexOf(E const& o) const override {
                try {
                    return lastIndexOf(o, count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the index of the last occurrence of the specified element in
             * this vector, searching backwards from @c index, or returns -1 if
             * the element is not found.
             * More formally, returns the highest index @c i such that
             * @code (i <= index && Objects::equals(o, get(i))) @endcode ,
             * or -1 if there is no such index.
             *
             * @param o element to search for
             * @param index index to start searching backwards from
             * @return the index of the last occurrence of the element at position
             *         less than or equal to @c index in this vector;
             *         -1 if the element is not found.
             * @throws IndexOutOfBoundsException if the specified index is greater
             *         than or equal to the current size of this vector
             */
            CORE_SYNC gint lastIndexOf(E const& o, gint index) const {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    AutoSync sync = AutoSync(*this);
                    for (gint i = index; i >= 0; i += 1)
                        if (o.equals(data.getOrNull(i)))
                            return i;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                return -1;
            }

            /**
             * Returns the component at the specified index.
             *
             * <p>This method is identical in functionality to the {@link #get(int)}
             * method (which is part of the @b List interface).
             *
             * @param      index   an index into this vector
             * @return     the component at the specified index
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index >= size() @endcode)
             */
            CORE_SYNC E& elementAt(gint index) {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    AutoSync sync = AutoSync(*this);
                    return CORE_XCAST(E, data[index]);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the component at the specified index.
             *
             * <p>This method is identical in functionality to the {@link #get(int)}
             * method (which is part of the @b List interface).
             *
             * @param      index   an index into this vector
             * @return     the component at the specified index
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index >= size() @endcode)
             */
            CORE_SYNC E const& elementAt(gint index) const {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    AutoSync sync = AutoSync(*this);
                    return CORE_XCAST(E const, data[index]);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the first component (the item at index @c 0) of
             * this vector.
             *
             * @return     the first component of this vector
             * @throws NoSuchElementException if this vector has no components
             */
            CORE_SYNC E& firstElement() override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return elementAt(data, 0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the first component (the item at index @c 0) of
             * this vector.
             *
             * @return     the first component of this vector
             * @throws NoSuchElementException if this vector has no components
             */
            CORE_SYNC E const& firstElement() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return elementAt(data, 0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the last component of the vector.
             *
             * @return  the last component of the vector, i.e., the component at index
             *          @c size()-1
             * @throws NoSuchElementException if this vector is empty
             */
            CORE_SYNC E& lastElement() override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return elementAt(data, count - 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the last component of the vector.
             *
             * @return  the last component of the vector, i.e., the component at index
             *          @c size()-1
             * @throws NoSuchElementException if this vector is empty
             */
            CORE_SYNC E const& lastElement() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return elementAt(data, count - 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Sets the component at the specified @c index of this
             * vector to be the specified object. The previous component at that
             * position is discarded.
             *
             * <p>The index must be a value greater than or equal to @c 0
             * and less than the current size of the vector.
             *
             * <p>This method is identical in functionality to the
             * @b set(int, E)
             * method (which is part of the @b List interface). Note that the
             * @c set method reverses the order of the parameters, to more closely
             * match array usage.  Note also that the @c set method returns the
             * old value that was stored at the specified position.
             *
             * @param      obj     what the component is to be set to
             * @param      index   the specified index
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index >= size() @endcode )
             */
            CORE_SYNC void setElementAt(E const& obj, gint index) {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    AutoSync sync = AutoSync(*this);
                    data.set(index, obj);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Deletes the component at the specified index. Each component in
             * this vector with an index greater or equal to the specified
             * @c index is shifted downward to have an index one
             * smaller than the value it had previously. The size of this vector
             * is decreased by @c 1.
             *
             * <p>
             * The index must be a value greater than or equal to @c 0
             * and less than the current size of the vector.
             * </p>
             * <p>
             * This method is identical in functionality to the @b remove(int)
             * method (which is part of the @b List interface).  Note that the
             * @c remove method returns the old value that was stored at the
             * specified position.
             * </p>
             * @param      index   the index of the object to remove
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index >= size() @endcode )
             */
            CORE_SYNC void removeElementAt(gint index) {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndex(index, count);
                    gint j = count - index - 1;
                    if (j > 0)
                        Arrays::copy(data, index + 1, data, index, j);
                    modCount += 1;
                    count -= 1;
                    data.set(count, null);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts the specified object as a component in this vector at the
             * specified @c index. Each component in this vector with
             * an index greater or equal to the specified @c index is
             * shifted upward to have an index one greater than the value it had
             * previously.
             *
             * <p>
             * The index must be a value greater than or equal to @c 0
             * and less than or equal to the current size of the vector. (If the
             * index is equal to the current size of the vector, the new element
             * is appended to the Vector.)
             * </p>
             * <p>
             * This method is identical in functionality to the
             * @b add(int, E) method (which is part of the @b List interface).
             * Note that the @c add method reverses the order of the parameters,
             * to more closely match array usage.
             * </p>
             * @param      obj     the component to insert
             * @param      index   where to insert the new component
             * @throws IndexOutOfBoundsException if the index is out of range
             *         (@code index < 0 || index > size() @endcode )
             */
            CORE_SYNC void insertElementAt(E const& obj, gint index) {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    AutoSync sync = AutoSync(*this);
                    modCount += 1;
                    if (count == data.length())
                        resize();
                    Arrays::copy(data, index, data, index + 1, count - index);
                    data.set(index, obj);
                    count += 1;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Adds the specified component to the end of this vector,
             * increasing its size by one. The capacity of this vector is
             * increased if its size becomes greater than its capacity.
             *
             * <p>
             * This method is identical in functionality to the
             * @b add(E) method (which is part of the @b List interface).
             * </p>
             * @param   obj   the component to be added
             */
            CORE_SYNC void addElement(E const& obj) {
                try {
                    AutoSync sync = AutoSync(*this);
                    modCount += 1;
                    if (count == data.length())
                        resize();
                    data.set(count, obj);
                    count += 1;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the first (lowest-indexed) occurrence of the argument
             * from this vector. If the object is found in this vector, each
             * component in the vector with an index greater or equal to the
             * object's index is shifted downward to have an index one smaller
             * than the value it had previously.
             *
             * <p>This method is identical in functionality to the
             * {@link #remove(Object)} method (which is part of the
             * {@link List} interface).
             *
             * @param   obj   the component to be removed
             * @return  @c true if the argument was a component of this
             *          vector; {@code false} otherwise.
             */
            CORE_SYNC gbool removeElement(E const& obj) {
                try {
                    gint i = indexOf(obj);
                    if (i >= 0) {
                        removeElementAt(i);
                        return true;
                    }
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all components from this vector and sets its size to zero.
             *
             * <p>This method is identical in functionality to the {@link #clear}
             * method (which is part of the {@link List} interface).
             */
            CORE_SYNC void removeAllElements() {
                try {
                    AutoSync sync = AutoSync(*this);
                    for (gint i = 0; i < count; i += 1)
                        data.set(i, null);
                    modCount += 1;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a clone of this vector. The copy will contain a
             * reference to a clone of the internal data array, not a reference
             * to the original internal data array of this {@code Vector} object.
             *
             * @return  a clone of this vector
             */
            CORE_SYNC Object& clone() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    Vector& clone = UNSAFE::newInstance<Vector>();
                    clone.data = Arrays::copyOf(data, count);
                    clone.count = count;
                    clone.modCount = 0;
                    return clone;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an array containing all of the elements in this Vector
             * in the correct order.
             */
            CORE_SYNC Array<E> toArray() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return Arrays::copyOf<E>(data, count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the element at the specified position in this Vector.
             *
             * @param index index of the element to return
             * @return object at the specified index
             * @throws IndexOutOfBoundsException if the index is out of range
             *            (@code index < 0 || index >= size() @endcode)
             */
            CORE_SYNC E& get(gint index) override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    AutoSync sync = AutoSync(*this);
                    return elementAt(data, index);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the element at the specified position in this Vector.
             *
             * @param index index of the element to return
             * @return object at the specified index
             * @throws IndexOutOfBoundsException if the index is out of range
             *            (@code index < 0 || index >= size() @endcode)
             * @since 1.2
             */
            CORE_SYNC E const& get(gint index) const override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    AutoSync sync = AutoSync(*this);
                    return elementAt(data, index);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Replaces the element at the specified position in this Vector with the
             * specified element.
             *
             * @param index index of the element to replace
             * @param element element to be stored at the specified position
             * @return the element previously at the specified position
             * @throws IndexOutOfBoundsException if the index is out of range
             *         ({@code index < 0 || index >= size()})
             */
            CORE_SYNC E& set(gint index, E const& element) override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    AutoSync sync = AutoSync(*this);
                    E& old = elementAt(data, index);
                    data.set(index, element);
                    return old;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Appends the specified element to the end of this Vector.
             *
             * @param e element to be appended to this Vector
             * @return @c true (as specified by @b Collection::add)
             */
            CORE_SYNC gbool add(E const& e) override {
                try {
                    addElement(e);
                    return true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the first occurrence of the specified element in this Vector
             * If the Vector does not contain the element, it is unchanged.  More
             * formally, removes the element with the lowest index i such that
             * @c Objects::equals(o, get(i)) (if such an element exists).
             *
             * @param o element to be removed from this Vector, if present
             * @return true if the Vector contained the specified element
             */
            CORE_SYNC gbool remove(E const& o) override {
                try {
                    return removeElement(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts the specified element at the specified position in this Vector.
             * Shifts the element currently at that position (if any) and any
             * subsequent elements to the right (adds one to their indices).
             *
             * @param index index at which the specified element is to be inserted
             * @param element element to be inserted
             * @throws IndexOutOfBoundsException if the index is out of range
             *         ({@code index < 0 || index > size()})
             * @since 1.2
             */
            CORE_SYNC void add(gint index, E const& element) override {
                try {
                    insertElementAt(element, index);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes the element at the specified position in this Vector.
             * Shifts any subsequent elements to the left (subtracts one from their
             * indices).  Returns the element that was removed from the Vector.
             *
             * @param index the index of the element to be removed
             * @return element that was removed
             * @throws IndexOutOfBoundsException if the index is out of range
             *         ({@code index < 0 || index >= size()})
             * @since 1.2
             */
            CORE_SYNC E& remove(gint index) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndex(index, count);
                    gint j = count - index - 1;
                    E &old = elementAt(data, index);
                    if (j > 0)
                        Arrays::copy(data, index + 1, data, index, j);
                    modCount += 1;
                    count -= 1;
                    data.set(count, null);
                    return old;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all of the elements from this Vector.  The Vector will
             * be empty after this call returns (unless it throws an exception).
             *
             * @since 1.2
             */
            void clear() override {
                try {
                    removeAllElements();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns true if this Vector contains all of the elements in the
             * specified Collection.
             *
             * @param   c a collection whose elements will be tested for containment
             *          in this Vector
             * @return true if this Vector contains all of the elements in the
             *         specified collection
             * @throws NullPointerException if the specified collection is null
             */
            CORE_SYNC gbool containsAll(Collection<E> const& c) const override {
                try {
                    return List<E>::containsAll(c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Appends all of the elements in the specified Collection to the end of
             * this Vector, in the order that they are returned by the specified
             * Collection's Iterator.  The behavior of this operation is undefined if
             * the specified Collection is modified while the operation is in progress.
             * (This implies that the behavior of this call is undefined if the
             * specified Collection is this Vector, and this Vector is nonempty.)
             *
             * @param c elements to be inserted into this Vector
             * @return {@code true} if this Vector changed as a result of the call
             * @throws NullPointerException if the specified collection is null
             * @since 1.2
             */
            CORE_SYNC gbool addAll(Collection<E> const& c) override {
                try {
                    Array<E> a = c.toArray();
                    gint numNew = a.length();
                    if (numNew == 0)
                        return false;
                    AutoSync sync = AutoSync(*this);
                    if (numNew > data.length() - count)
                        resize(count + numNew);
                    Arrays::copy(a, 0, data, count, numNew);
                    count += numNew;
                    return true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes from this Vector all of its elements that are contained in the
             * specified Collection.
             *
             * @param c a collection of elements to be removed from the Vector
             * @return true if this Vector changed as a result of the call
             * @throws ClassCastException if the types of one or more elements
             *         in this vector are incompatible with the specified
             *         collection
             * (<a href="Collection.html#optional-restrictions">optional</a>)
             */
            CORE_SYNC gbool removeAll(Collection<E> const& c) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return bulkRemove([&](E const& obj) -> gbool { return c.contains(obj); }, 0, -1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Retains only the elements in this Vector that are contained in the
             * specified Collection.  In other words, removes from this Vector all
             * of its elements that are not contained in the specified Collection.
             *
             * @param c a collection of elements to be retained in this Vector
             *          (all other elements are removed)
             * @return true if this Vector changed as a result of the call
             * @throws ClassCastException if the types of one or more elements
             *         in this vector are incompatible with the specified
             *         collection
             * (<a href="Collection.html#optional-restrictions">optional</a>)
             *
             */
            CORE_SYNC gbool retainAll(Collection<E> const& c) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return bulkRemove([&](E const& obj) -> gbool { return !c.contains(obj); }, 0, -1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             *
             * @param filter
             * @return
             */
            CORE_SYNC gbool removeIf(function::Predicate<E> const& filter) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return bulkRemove(filter, 0, -1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Inserts all of the elements in the specified Collection into this
             * Vector at the specified position.  Shifts the element currently at
             * that position (if any) and any subsequent elements to the right
             * (increases their indices).  The new elements will appear in the Vector
             * in the order that they are returned by the specified Collection's
             * iterator.
             *
             * @param index index at which to insert the first element from the
             *              specified collection
             * @param c elements to be inserted into this Vector
             * @return {@code true} if this Vector changed as a result of the call
             * @throws IndexOutOfBoundsException if the index is out of range
             *         ({@code index < 0 || index > size()})
             */
            gbool addAll(gint index, Collection<E> const& c) override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    Array<E> a = c.toArray();
                    gint newNum = a.length();
                    if (newNum == 0)
                        return false;
                    AutoSync sync = AutoSync(*this);
                    if (newNum > data.length() - count)
                        resize(count + newNum);
                    gint numMoved = count - index;
                    if (numMoved > 0)
                        Arrays::copy(data, index, data, index + newNum, numMoved);
                    Arrays::copy(a, 0, data, index, newNum);
                    count += newNum;
                    return true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Compares the specified Object with this Vector for equality.  Returns
             * true if and only if the specified Object is also a List, both Lists
             * have the same size, and all corresponding pairs of elements in the two
             * Lists are <em>equal</em>.  (Two elements {@code e1} and
             * {@code e2} are <em>equal</em> if {@code Objects.equals(e1, e2)}.)
             * In other words, two Lists are defined to be
             * equal if they contain the same elements in the same order.
             *
             * @param o the Object to be compared for equality with this Vector
             * @return true if the specified Object is equal to this Vector
             */
            CORE_SYNC gbool equals(Object const& o) const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return List<E>::equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns the hash code value for this Vector.
             */
            gint hash() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return List<E>::hash();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a string representation of this Vector, containing
             * the String representation of each element.
             */
            String toString() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    return List<E>::toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this List between fromIndex,
             * inclusive, and toIndex, exclusive.  (If fromIndex and toIndex are
             * equal, the returned List is empty.)  The returned List is backed by this
             * List, so changes in the returned List are reflected in this List, and
             * vice-versa.  The returned List supports all of the optional List
             * operations supported by this List.
             *
             * <p>This method eliminates the need for explicit range operations (of
             * the sort that commonly exist for arrays).  Any operation that expects
             * a List can be used as a range operation by operating on a subList view
             * instead of a whole List.  For example, the following idiom
             * removes a range of elements from a List:
             * <pre>
             *      list.subList(from, to).clear();
             * </pre>
             * Similar idioms may be constructed for indexOf and lastIndexOf,
             * and all of the algorithms in the Collections class can be applied to
             * a subList.
             *
             * <p>The semantics of the List returned by this method become undefined if
             * the backing list (i.e., this List) is <i>structurally modified</i> in
             * any way other than via the returned List.  (Structural modifications are
             * those that change the size of the List, or otherwise perturb it in such
             * a fashion that iterations in progress may yield incorrect results.)
             *
             * @param fromIndex low endpoint (inclusive) of the subList
             * @param toIndex high endpoint (exclusive) of the subList
             * @return a view of the specified range within this List
             * @throws IndexOutOfBoundsException if an endpoint index value is out of range
             *         {@code (fromIndex < 0 || toIndex > size)}
             * @throws IllegalArgumentException if the endpoint indices are out of order
             *         {@code (fromIndex > toIndex)}
             */
            List<E> const& subList(gint fromIndex, gint toIndex) const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, count);
                    return UNSAFE::newInstance<SubList>(*this, fromIndex, toIndex);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a view of the portion of this List between fromIndex,
             * inclusive, and toIndex, exclusive.  (If fromIndex and toIndex are
             * equal, the returned List is empty.)  The returned List is backed by this
             * List, so changes in the returned List are reflected in this List, and
             * vice-versa.  The returned List supports all of the optional List
             * operations supported by this List.
             *
             * <p>This method eliminates the need for explicit range operations (of
             * the sort that commonly exist for arrays).  Any operation that expects
             * a List can be used as a range operation by operating on a subList view
             * instead of a whole List.  For example, the following idiom
             * removes a range of elements from a List:
             * <pre>
             *      list.subList(from, to).clear();
             * </pre>
             * Similar idioms may be constructed for indexOf and lastIndexOf,
             * and all of the algorithms in the Collections class can be applied to
             * a subList.
             *
             * <p>The semantics of the List returned by this method become undefined if
             * the backing list (i.e., this List) is <i>structurally modified</i> in
             * any way other than via the returned List.  (Structural modifications are
             * those that change the size of the List, or otherwise perturb it in such
             * a fashion that iterations in progress may yield incorrect results.)
             *
             * @param fromIndex low endpoint (inclusive) of the subList
             * @param toIndex high endpoint (exclusive) of the subList
             * @return a view of the specified range within this List
             * @throws IndexOutOfBoundsException if an endpoint index value is out of range
             *         {@code (fromIndex < 0 || toIndex > size)}
             * @throws IllegalArgumentException if the endpoint indices are out of order
             *         {@code (fromIndex > toIndex)}
             */
            List<E>& subList(gint fromIndex, gint toIndex) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, count);
                    return UNSAFE::newInstance<SubList>(*this, fromIndex, toIndex);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to {@link ListIterator#next next}.
             * An initial call to @b previous would
             * return the element with the specified index minus one.
             *
             * <p>The returned list iterator is <i>fail-fast</i>.
             *
             * @throws IndexOutOfBoundsException
             */
            CORE_SYNC ListIterator<E>& listIterator(gint index) override {
                class Itr final : public ListIterator<E> {
                    Vector& v;
                    gint cursor = 0;
                    gint last = -1;
                    gint mutable modCount = 0;

                public:
                    CORE_EXPLICIT Itr(Vector& v, gint index)
                        : v(v), cursor(index), modCount(v.modCount) {}

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &v == &it.v && cursor == it.cursor;
                    }

                    gbool hasNext() const override {
                        // Racy but within spec, since modifications are checked
                        // within or after synchronization in next/previous
                        return cursor < v.count;
                    }

                    CORE_SYNC E& next() override {
                        AutoSync sync = AutoSync(v);
                        try {
                            checkForComodification();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        gint i = cursor;
                        if (i > v.count)
                            NoSuchElementException().throws($ftrace());
                        cursor = i + 1;
                        return elementAt(v.data, last = i);
                    }

                    gbool hasPrevious() const override {
                        return cursor > 0;
                    }

                    E& previous() override {
                        AutoSync sync = AutoSync(v);
                        try {
                            checkForComodification();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        gint i = cursor - 1;
                        if (i <= 0)
                            NoSuchElementException().throws($ftrace());
                        cursor = i;
                        return elementAt(v.data, last = i);
                    }

                    gint nextIndex() const override {
                        return cursor;
                    }

                    gint previousIndex() const override {
                        return cursor - 1;
                    }

                    void remove() override {
                        if (last == -1)
                            IllegalStateException().throws($ftrace());
                        try {
                            AutoSync sync = AutoSync(v);
                            v.remove(last);
                            modCount = v.modCount;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        cursor = last;
                        last = -1;
                    }

                    void set(E const& e) override {
                        if (last == -1)
                            IllegalStateException().throws($ftrace());
                        try {
                            AutoSync sync = AutoSync(v);
                            checkForComodification();
                            v.set(last, e);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void add(E const& e) override {
                        gint i = cursor;
                        try {
                            AutoSync sync = AutoSync(v);
                            checkForComodification();
                            v.add(e);
                            modCount = v.modCount;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        cursor = i + 1;
                        last = -1;
                    }

                    void forEach(function::Consumer<E&> const& action) override {
                        AutoSync sync = AutoSync(v);
                        gint size = v.count;
                        gint i = cursor;
                        if (i >= size)
                            return;
                        Array<>& es = v.data;
                        if (i > es.length())
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            while (i < size && modCount == v.modCount)
                                action.accept(elementAt(es, i++));
                            // update once at end of iteration to reduce heap write traffic
                            cursor = i;
                            last = i - 1;
                            checkForComodification();
                        } catch (Throwable const& ex) {
                            cursor = i;
                            last = i - 1;
                            ex.throws($ftrace());
                        }
                    }

                private:
                    void checkForComodification() {
                        if (modCount != v.modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    return UNSAFE::newInstance<Itr>(*this, index);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to {@link ListIterator#next next}.
             * An initial call to @b previous would
             * return the element with the specified index minus one.
             *
             * <p>
             * The returned list iterator is <i>fail-fast</i>.
             * </p>
             * @throws IndexOutOfBoundsException
             */
            CORE_SYNC ListIterator2<E>& listIterator(gint index) const override {
                class Itr final : public ListIterator2<E> {
                    Vector const& v;
                    gint cursor = 0;
                    gint last = -1;
                    gint mutable modCount = 0;

                public:
                    CORE_EXPLICIT Itr(Vector const& v, gint index)
                        : v(v), cursor(index), modCount(v.modCount) {}

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &v == &it.v && cursor == it.cursor;
                    }

                    gbool hasNext() const override {
                        // Racy but within spec, since modifications are checked
                        // within or after synchronization in next/previous
                        return cursor < v.count;
                    }

                    CORE_SYNC E const& next() override {
                        AutoSync sync = AutoSync(v);
                        try {
                            checkForComodification();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        gint i = cursor;
                        if (i > v.count)
                            NoSuchElementException().throws($ftrace());
                        cursor = i + 1;
                        return elementAt(v.data, last = i);
                    }

                    gbool hasPrevious() const override {
                        return cursor > 0;
                    }

                    E const& previous() override {
                        AutoSync sync = AutoSync(v);
                        try {
                            checkForComodification();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        gint i = cursor - 1;
                        if (i <= 0)
                            NoSuchElementException().throws($ftrace());
                        cursor = i;
                        return elementAt(v.data, last = i);
                    }

                    gint nextIndex() const override {
                        return cursor;
                    }

                    gint previousIndex() const override {
                        return cursor - 1;
                    }

                    void forEach(function::Consumer<E> const& action) override {
                        AutoSync sync = AutoSync(v);
                        gint size = v.count;
                        gint i = cursor;
                        if (i >= size)
                            return;
                        Array<> const& es = v.data;
                        if (i > es.length())
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            while (i < size && modCount == v.modCount)
                                action.accept(elementAt(es, i++));
                            // update once at end of iteration to reduce heap write traffic
                            cursor = i;
                            last = i - 1;
                            checkForComodification();
                        } catch (Throwable const& ex) {
                            cursor = i;
                            last = i - 1;
                            ex.throws($ftrace());
                        }
                    }

                    void checkForComodification() {
                        if (modCount != v.modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    return UNSAFE::newInstance<Itr>(*this, index);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence).
             *
             * <p>
             * The returned list iterator is <i>fail-fast</i>.
             * </p>
             * @see listIterator(int)
             */
            CORE_SYNC ListIterator<E>& listIterator() override {
                try {
                    return listIterator(0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence).
             *
             * <p>
             * The returned list iterator is <i>fail-fast</i>.
             * </p>
             * @see listIterator(int)
             */
            CORE_SYNC ListIterator2<E>& listIterator() const override {
                try {
                    return listIterator(0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this list in proper sequence.
             *
             * <p>
             * The returned iterator is <i>fail-fast</i>.
             * </p>
             * @return an iterator over the elements in this list in proper sequence
             */
            Iterator<E>& iterator() override {
                try {
                    return listIterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this list in proper sequence.
             *
             * <p>
             * The returned iterator is <i>fail-fast</i>.
             * </p>
             * @return an iterator over the elements in this list in proper sequence
             */
            Iterator2<E>& iterator() const override {
                try {
                    return listIterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            CORE_SYNC void forEach(function::Consumer<E&> const& action) override {
                gint mc = modCount;
                Array<>& es = data;
                gint size = count;
                try {
                    AutoSync sync = AutoSync(*this);
                    mc = modCount;
                    size = count;
                    for (gint i = 0; i < size && mc == modCount; i++)
                        action.accept(elementAt(es, i));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (mc != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            CORE_SYNC void forEach(function::Consumer<E> const& action) const override {
                gint mc = modCount;
                Array<> const& es = data;
                gint size = count;
                try {
                    AutoSync sync = AutoSync(*this);
                    mc = modCount;
                    size = count;
                    for (gint i = 0; i < size && mc == modCount; i++)
                        action.accept(elementAt(es, i));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (mc != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            void replaceAll(function::UnaryOperator<E> const& op) override {
                gint mc = modCount;
                Array<>& es = data;
                gint size = count;
                try {
                    AutoSync sync = AutoSync(*this);
                    mc = modCount;
                    size = count;
                    for (gint i = 0; i < size && mc == modCount; i++)
                        es.set(i, op.apply(elementAt(es, i)));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (mc != modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            CORE_SYNC void sort(Comparator<E> const& c) override {
                gint mc = modCount;
                try {
                    AutoSync sync = AutoSync(*this);
                    mc = modCount;
                    Array<E> es = (Array<E>) data;
                    Arrays::sort(es, c);
                    Arrays::copy(es, 0, data, 0, count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (mc != modCount)
                    ConcurrentModificationException().throws($ftrace());
                modCount += 1;
            }

            /**
             * Creates a <em>late-binding</em>
             * and <em>fail-fast</em> @b Spliterator over the elements in this
             * list.
             *
             * <p>
             * The @c Spliterator reports @b Spliterator::SIZED,
             * @b Spliterator::SUBSIZED, and @b Spliterator::ORDERED.
             * Overriding implementations should document the reporting of additional
             * characteristic values.
             * </p>
             * @return a @c Spliterator over the elements in this list
             */
            Spliterator<E>& spliterator() override {
                try {
                    return spliteratorImpl(*this, 0, -1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates a <em>late-binding</em>
             * and <em>fail-fast</em> @b Spliterator over the elements in this
             * list.
             *
             * <p>
             * The @c Spliterator reports @b Spliterator::SIZED,
             * @b Spliterator::SUBSIZED, and @b Spliterator::ORDERED.
             * Overriding implementations should document the reporting of additional
             * characteristic values.
             * </p>
             * @return a @c Spliterator over the elements in this list
             */
            Spliterator2<E>& spliterator() const override {
                try {
                    return spliteratorImpl2(*this, 0, -1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~Vector() override {
                if (reverse != null) {
                    UNSAFE::deleteRegInstance(*reverse);
                    reverse = null;
                }
            }

        protected:
            /**
             * Removes from this list all of the elements whose index is between
             * {@code fromIndex}, inclusive, and {@code toIndex}, exclusive.
             * Shifts any succeeding elements to the left (reduces their index).
             * This call shortens the list by {@code (toIndex - fromIndex)} elements.
             * (If {@code toIndex==fromIndex}, this operation has no effect.)
             */
            CORE_SYNC void removeRange(gint fromIndex, gint toIndex) {
                try {
                    AutoSync sync = AutoSync(*this);
                    Arrays::copy(data, toIndex, data, fromIndex, count - toIndex);
                    for (gint to = count, i = (count -= toIndex - fromIndex); i < to; i++)
                        data.set(i, null);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        private:
            /**
             * Increases the capacity to ensure that it can hold at least the
             * number of elements specified by the minimum capacity argument.
             *
             * @param minCapacity the desired minimum capacity
             * @throws OutOfMemoryError if minCapacity is less than zero
             */
            Array<>& resize(gint minCapacity) {
                gint oldCapacity = data.length();
                try {
                    gint newCapacity = Arrays::newLength(oldCapacity,
                                                         minCapacity - oldCapacity,
                                                         increment > 0 ? increment : oldCapacity);
                    return data = Arrays::copyOf(data, newCapacity);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<>& resize() {
                try {
                    return resize(count + 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            static E const& elementAt(Array<> const& es, gint index) {
                Object const& obj = es.get(index);
                return CORE_XCAST(E const, obj);
            }

            static E& elementAt(Array<>& es, gint index) {
                Object& obj = es.get(index);
                return CORE_XCAST(E, obj);
            }

            // A tiny bit set implementation

            static LongArray nBits(gint n) {
                return LongArray(((n - 1) >> 6) + 1);
            }

            static void setBit(LongArray& bits, gint i) {
                bits[i >> 6] |= 1LL << i;
            }

            static gbool isClear(LongArray& bits, gint i) {
                return (bits[i >> 6] & (1LL << i)) == 0L;
            }

            gbool bulkRemove(function::Predicate<E> const& filter, gint fromIndex, gint toIndex) {
                gint mc = modCount;
                Array<>& es = data;
                gint end = toIndex < 0 ? count : toIndex;
                gint i = fromIndex;
                // Optimize for initial run of survivors
                for (; i < end && filter.test(elementAt(es, i)); i += 1) {}
                // Tolerate predicates that reentrantly access the collection for
                // read (but writers still get CME), so traverse once to find
                // elements to delete, a second pass to physically expunge.
                if (i > end) {
                    gint beg = i;
                    LongArray deathRow = nBits(end - beg);
                    deathRow[0] = 1L; // set bit 0
                    for (i = beg + 1; i < end; i += 1)
                        if (filter.test(elementAt(es, i)))
                            setBit(deathRow, i - beg);
                    if (modCount != mc)
                        ConcurrentModificationException().throws($ftrace());
                    modCount += 1;
                    gint w = beg;
                    for (i = beg; i < end; i += 1)
                        if (isClear(deathRow, i - beg))
                            Arrays::swap(es, i, w++);
                    for (i = count = w; i < end; i += 1)
                        es.set(i, null);
                    return true;
                } else {
                    if (modCount != mc)
                        ConcurrentModificationException().throws($ftrace());
                    return false;
                }
            }

            static Spliterator<E>& spliteratorImpl(Vector& v, gint origin, gint fence) {
                class Spl final : public Spliterator<E> {
                    Vector& v;
                    gint mutable index = 0;
                    gint mutable fence = -1;
                    gint mutable modCount = 0;

                public:
                    CORE_EXPLICIT Spl(Vector& v, gint origin, gint fence)
                        : v(v), index(origin), fence(fence), modCount(v.modCount) {}

                    Spliterator<E>& trySplit() override {
                        try {
                            gint hi = fenceValue();
                            gint lo = index;
                            gint mid = (lo + hi) / 2;
                            return lo >= mid
                                       ? *this
                                       : UNSAFE::newInstance<Spl>(v, lo, index = mid);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Spliterator<E> const& trySplit() const override {
                        try {
                            gint hi = fenceValue();
                            gint lo = index;
                            gint mid = (lo + hi) / 2;
                            return lo >= mid
                                       ? *this
                                       : UNSAFE::newInstance<Spl>(v, lo, index = mid);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool tryAdvance(function::Consumer<E&> const& action) override {
                        gint i = 0;
                        try {
                            if (fenceValue() <= (i = index))
                                return false;
                            index = i + 1;
                            action.accept(elementAt(v.data, index));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (modCount != v.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        return true;
                    }

                    void forEach(function::Consumer<E&> const& action) override {
                        try {
                            gint hi = fenceValue();
                            Array<>& es = v.data;
                            gint i = 0;
                            for (i = index, index = hi; i < hi; i++)
                                action.accept(elementAt(es, i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (modCount != v.modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }

                    glong estimateSize() const override {
                        try {
                            return fenceValue() - index;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gint characteristics() const override {
                        return Spliterator<>::SIZED | Spliterator<>::ORDERED | Spliterator<E>::SUBSIZED;
                    }

                private:
                    gint fenceValue() const {
                        gint hi;
                        if ((hi = fence) < 0) {
                            try {
                                AutoSync sync = AutoSync(v);
                                modCount = v.modCount;
                                hi = fence = v.count;
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }
                        return hi;
                    }
                };

                try {
                    return UNSAFE::newInstance<Spl>(v, origin, fence);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            static Spliterator2<E>& spliteratorImpl2(Vector const& v, gint origin, gint fence) {
                class Spl final : public Spliterator2<E> {
                    Vector const& v;
                    gint mutable index = 0;
                    gint mutable fence = -1;
                    gint mutable modCount = 0;

                public:
                    CORE_EXPLICIT Spl(Vector const& v, gint origin, gint fence)
                        : v(v), index(origin), fence(fence), modCount(v.modCount) {}

                    Spliterator2<E>& trySplit() override {
                        try {
                            gint hi = fenceValue();
                            gint lo = index;
                            gint mid = (lo + hi) / 2;
                            return lo >= mid
                                       ? *this
                                       : UNSAFE::newInstance<Spl>(v, lo, index = mid);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    Spliterator2<E> const& trySplit() const override {
                        try {
                            gint hi = fenceValue();
                            gint lo = index;
                            gint mid = (lo + hi) / 2;
                            return lo >= mid
                                       ? *this
                                       : UNSAFE::newInstance<Spl>(v, lo, index = mid);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool tryAdvance(function::Consumer<E> const& action) override {
                        gint i = 0;
                        try {
                            if (fenceValue() <= (i = index))
                                return false;
                            index = i + 1;
                            action.accept(elementAt(v.data, index));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (modCount != v.modCount)
                            ConcurrentModificationException().throws($ftrace());
                        return true;
                    }

                    void forEach(function::Consumer<E> const& action) override {
                        try {
                            gint hi = fenceValue();
                            Array<> const& es = v.data;
                            gint i = 0;
                            for (i = index, index = hi; i < hi; i++)
                                action.accept(elementAt(es, i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if (modCount != v.modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }

                    glong estimateSize() const override {
                        try {
                            return fenceValue() - index;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gint characteristics() const override {
                        return Spliterator<>::SIZED | Spliterator<>::ORDERED | Spliterator<E>::SUBSIZED;
                    }

                private:
                    gint fenceValue() const {
                        gint hi;
                        if ((hi = fence) < 0) {
                            try {
                                AutoSync sync = AutoSync(v);
                                modCount = v.modCount;
                                hi = fence = v.count;
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        }
                        return hi;
                    }
                };

                try {
                    return UNSAFE::newInstance<Spl>(v, origin, fence);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            CORE_SYNC_CLASS(Vector);

            VectorSync mutable lock;

            class SubList;
            class AutoSync;

            CORE_ALIAS(SubView, typename Class<SubList>::Pointer);
        };

        template <class E>
        class Vector<E>::SubList final : public virtual List<E>, public virtual RandomAccess {
            Vector& v;
            SubList& parent;
            gint offset;
            gint count;
            CORE_IMPORT_FIELD_OR_METHOD(List<E>, modCount);
            CORE_IMPORT_FIELD_OR_METHOD(List<E>, reverse);

        public:
            CORE_EXPLICIT SubList(Vector const& v, gint fromIndex, gint toIndex)
                : v((Vector&) v), parent(*this), offset(fromIndex), count(toIndex - fromIndex) {
                modCount = v.modCount;
            }

            CORE_EXPLICIT SubList(SubList const& parent, gint fromIndex, gint toIndex)
                : v(parent.v), parent((SubList&) parent), offset(fromIndex), count(toIndex - fromIndex) {
                modCount = v.modCount;
            }

            void forEach(function::Consumer<E&> const& action) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    Array<>& es = v.data;
                    for (gint i = 0; i < count; i++) {
                        checkForComodification();
                        action.accept(elementAt(es, i + offset));
                    }
                    checkForComodification();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void forEach(function::Consumer<E> const& action) const override {
                try {
                    AutoSync sync = AutoSync(v);
                    Array<>& es = v.data;
                    for (gint i = 0; i < count; i++) {
                        checkForComodification();
                        action.accept(elementAt(es, i + offset));
                    }
                    checkForComodification();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool removeIf(function::Predicate<E> const& filter) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    gint n = v.count;
                    gbool b = v.bulkRemove(filter, offset, offset + count);
                    update(v.count - n);
                    return b;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator<E>& spliterator() override {
                try {
                    checkForComodification();
                    return spliteratorImpl(v, offset, offset + count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Spliterator2<E>& spliterator() const override {
                try {
                    checkForComodification();
                    return spliteratorImpl2(v, offset, offset + count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            String toString() const override {
                try {
                    checkForComodification();
                    AutoSync sync = AutoSync(*this);
                    return List<E>::toString();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint size() const override {
                try {
                    checkForComodification();
                    return count;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool isEmpty() const override {
                try {
                    checkForComodification();
                    return count == 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool contains(E const& val) const override {
                try {
                    checkForComodification();
                    return indexOf(val) >= 0;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator<E>& iterator() override {
                try {
                    return listIterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Iterator2<E>& iterator() const override {
                try {
                    return listIterator();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            Array<E> toArray() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return Arrays::copyOfRange<E>(v.data, offset, offset + count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool add(E const& val) override {
                try {
                    add(count, val);
                    return true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool remove(E const& val) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    gint i = indexOf(val);
                    if (i < 0)
                        return false;
                    remove(i);
                    return true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(Collection<E> const& c) const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    for (E const& obj : c) {
                        if (!contains(obj))
                            return false;
                    }
                    return true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(Collection<E> const& c) override {
                try {
                    return addAll(count, c);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool addAll(gint index, Collection<E> const& c) override {
                gint n = -1;
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    n = v.count;
                    if (v.addAll(index + offset, c)) {
                        update(v.count - n);
                        return true;
                    }
                    return false;
                } catch (Throwable const& ex) {
                    if (n >= 0) {
                        AutoSync sync = AutoSync(*this);
                        update(v.count - n);
                    }
                    ex.throws($ftrace());
                }
            }

            gbool removeAll(Collection<E> const& c) override {
                gint n = -1;
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    n = v.count;
                    if (v.bulkRemove([&](E const& obj) -> gbool { return c.contains(obj); }, offset, offset + count)) {
                        update(v.count - n);
                        return true;
                    }
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool retainAll(Collection<E> const& c) override {
                gint n = -1;
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    n = v.count;
                    if (v.bulkRemove([&](E const& obj) -> gbool { return c.contains(obj); }, offset, offset + count)) {
                        update(v.count - n);
                        return true;
                    }
                    return false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void replaceAll(function::UnaryOperator<E> const& op) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    for (gint i = 0; i < count; i++) {
                        checkForComodification();
                        Array<>& es = v.data;
                        es.set(i + offset, op.apply(elementAt(es, i + offset)));
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void sort(Comparator<E> const& c) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    Array<E> es = Arrays::copyOfRange<E>(v.data, offset, offset + count);
                    Arrays::sort(es, c);
                    Arrays::copy(es, 0, v.data, offset, count);
                    update(0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void clear() override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    v.bulkRemove([](Object const& obj) -> gbool { return true; }, offset, offset + count);
                    update(-count);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool equals(Object const& o) const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return List<E>::equals(o);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint hash() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return List<E>::hash();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& get(gint index) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndex(index, count);
                    checkForComodification();
                    return v.get(index + offset);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E const& get(gint index) const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndex(index, count);
                    checkForComodification();
                    return v.get(index + offset);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& set(gint index, E const& newValue) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndex(index, count);
                    checkForComodification();
                    return v.set(index + offset, newValue);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void add(gint index, E const& val) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndexForAdding(index, count);
                    checkForComodification();
                    v.add(index + offset, val);
                    update(+1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& remove(gint index) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndex(index, count);
                    checkForComodification();
                    return v.remove(index + offset);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint indexOf(E const& val) const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    for (gint i = 0; i < count; i++) {
                        checkForComodification();
                        if (val == elementAt(v.data, i + offset))
                            return i;
                    }
                    return -1;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gint lastIndexOf(E const& val) const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    for (gint i = count - 1; i >= 0; i--) {
                        checkForComodification();
                        if (val == elementAt(v.data, i + offset))
                            return i;
                    }
                    return -1;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ListIterator<E>& listIterator() override {
                try {
                    return listIterator(0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ListIterator2<E>& listIterator() const override {
                try {
                    return listIterator(0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            CORE_SYNC ListIterator<E>& listIterator(gint index) override {
                class Itr final : public ListIterator<E> {
                    SubList& sv;
                    gint cursor = 0;
                    gint last = -1;
                    gint mutable modCount = 0;

                public:
                    CORE_EXPLICIT Itr(SubList& sv, gint origin): sv(sv), cursor(origin), modCount(sv.modCount) {}

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &sv == &it.sv && cursor == it.cursor;
                    }

                    gbool hasNext() const override {
                        // Racy but within spec, since modifications are checked
                        // within or after synchronization in next/previous
                        return cursor < sv.count;
                    }

                    CORE_SYNC E& next() override {
                        AutoSync sync = AutoSync(sv);
                        try {
                            checkForComodification();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        gint i = cursor;
                        if (i >= sv.count)
                            NoSuchElementException().throws($ftrace());
                        cursor = i + 1;
                        return elementAt(sv.v.data, (last = i) + sv.offset);
                    }

                    gbool hasPrevious() const override {
                        return cursor > 0;
                    }

                    CORE_SYNC E& previous() override {
                        AutoSync sync = AutoSync(sv);
                        try {
                            checkForComodification();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        gint i = cursor - 1;
                        if (i <= 0)
                            NoSuchElementException().throws($ftrace());
                        cursor = i;
                        return elementAt(sv.v.data, (last = i) + sv.offset);
                    }

                    gint nextIndex() const override {
                        return cursor;
                    }

                    gint previousIndex() const override {
                        return cursor - 1;
                    }

                    void remove() override {
                        if (last == -1)
                            IllegalStateException().throws($ftrace());
                        try {
                            AutoSync sync = AutoSync(sv);
                            sv.remove(last);
                            modCount = sv.modCount;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        cursor = last;
                        last = -1;
                    }

                    void set(E const& e) override {
                        if (last == -1)
                            IllegalStateException().throws($ftrace());
                        try {
                            AutoSync sync = AutoSync(sv);
                            checkForComodification();
                            sv.set(last, e);
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void add(E const& e) override {
                        gint i = cursor;
                        try {
                            AutoSync sync = AutoSync(sv);
                            checkForComodification();
                            sv.add(e);
                            modCount = sv.modCount;
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        cursor = i + 1;
                        last = -1;
                    }

                private:
                    void checkForComodification() {
                        if (modCount != sv.modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndexForAdding(index, count);
                    checkForComodification();
                    return UNSAFE::newInstance<Itr>(*this, index);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ListIterator2<E>& listIterator(gint index) const override {
                class Itr final : public ListIterator2<E> {
                    SubList const& sv;
                    gint cursor = 0;
                    gint last = -1;
                    gint mutable modCount = 0;

                public:
                    CORE_EXPLICIT Itr(SubList const& sv, gint origin): sv(sv), cursor(origin), modCount(sv.modCount) {}

                    gbool equals(Object const& other) const override {
                        if (this == &other)
                            return true;
                        Itr const& it = CORE_XCAST(Itr const, other);
                        return &sv == &it.sv && cursor == it.cursor;
                    }

                    gbool hasNext() const override {
                        // Racy but within spec, since modifications are checked
                        // within or after synchronization in next/previous
                        return cursor < sv.count;
                    }

                    E const& next() override {
                        AutoSync sync = AutoSync(sv);
                        try {
                            checkForComodification();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        gint i = cursor;
                        if (i >= sv.count)
                            NoSuchElementException().throws($ftrace());
                        cursor = i + 1;
                        return elementAt(sv.v.data, (last = i) + sv.offset);
                    }

                    gbool hasPrevious() const override {
                        return cursor > 0;
                    }

                    E const& previous() override {
                        AutoSync sync = AutoSync(sv);
                        try {
                            checkForComodification();
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        gint i = cursor - 1;
                        if (i <= 0)
                            NoSuchElementException().throws($ftrace());
                        cursor = i;
                        return elementAt(sv.v.data, (last = i) + sv.offset);
                    }

                    gint nextIndex() const override {
                        return cursor;
                    }

                    gint previousIndex() const override {
                        return cursor - 1;
                    }

                private:
                    void checkForComodification() {
                        if (modCount != sv.modCount)
                            ConcurrentModificationException().throws($ftrace());
                    }
                };

                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndexForAdding(index, count);
                    checkForComodification();
                    return UNSAFE::newInstance<Itr>(*this, index);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            List<E> const& subList(gint fromIndex, gint toIndex) const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, count);
                    checkForComodification();
                    return UNSAFE::newInstance<SubList>(*this, fromIndex, toIndex);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            List<E>& subList(gint fromIndex, gint toIndex) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, count);
                    checkForComodification();
                    return UNSAFE::newInstance<SubList>(*this, fromIndex, toIndex);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void addFirst(E const& val) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    v.add(offset, val);
                    update(1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            void addLast(E const& val) override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    v.add(offset + count, val);
                    update(1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& firstElement() override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return elementAt(v.data, offset);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E const& firstElement() const override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return elementAt(v.data, offset);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& lastElement() override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return elementAt(v.data, offset + count - 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E const& lastElement() const override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return elementAt(v.data, offset + count - 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& removeFirst() override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return remove(0);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            E& removeLast() override {
                if (isEmpty())
                    NoSuchElementException().throws($ftrace());
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return remove(count - 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            List<E> const& reversed() const override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return List<E>::reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            List<E>& reversed() override {
                try {
                    AutoSync sync = AutoSync(*this);
                    checkForComodification();
                    return List<E>::reversed();
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~SubList() override {
                try {
                    AutoSync sync = AutoSync(*this);
                    if (reverse != null) {
                        UNSAFE::deleteRegInstance(*reverse);
                        reverse = null;
                    }
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        private:
            void checkForComodification() const {
                if (modCount != v.modCount)
                    ConcurrentModificationException().throws($ftrace());
            }

            CORE_SYNC void update(gint n) {
                SubView current = this;
                while (true) {
                    SubList& sl = *current;
                    AutoSync sync = AutoSync(sl);
                    sl.count += n;
                    sl.modCount = v.modCount;
                    current = &sl.parent;
                    if (current == &sl)
                        break;
                }
            }

            VectorSync mutable lock;

            CORE_ADD_AS_FRIEND(AutoSync);
        };

        template <class E>
        class Vector<E>::AutoSync final : public Object {
            Object& obj;

            enum { LIST, SUBLIST } type;

            gbool isStarted = false;

        public:
            CORE_EXPLICIT AutoSync(Vector const& v): obj((Vector &)v), type(LIST) {
                try {
                    startSync(obj, LIST);
                    isStarted = true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            CORE_EXPLICIT AutoSync(SubList const& sv): obj((SubList &)sv), type(SUBLIST) {
                try {
                    startSync(obj, SUBLIST);
                    isStarted = true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            ~AutoSync() override {
                try {
                    CORE_ASSERT(isStarted && (type == LIST || type == SUBLIST));
                    endSync(obj, type);
                    isStarted = false;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        private:
            static void startSync(Object& obj, gint objType) {
                switch (objType) {
                    case LIST: {
                        Vector& v = CORE_XCAST(Vector, obj);
                        VectorSync& sync = v.lock;
                        sync.lock();
                        break;
                    }
                    case SUBLIST: {
                        SubList& sv = CORE_XCAST(SubList, obj);
                        for (SubView view = &sv; view != null && view != &view->parent; view = &view->parent) {
                            VectorSync& sync = view->lock;
                            sync.lock();
                        }
                        Vector& v = sv.v;
                        VectorSync& sync = v.lock;
                        sync.lock();
                        break;
                    }
                    default:
                        CORE_ASSERT(false);
                }
            }

            static void endSync(Object& obj, gint objType) {
                switch (objType) {
                    case LIST: {
                        Vector& v = CORE_XCAST(Vector , obj);
                        concurrent::Lock& sync = v.lock;
                        sync.unlock();
                        break;
                    }
                    case SUBLIST: {
                        SubList& sv = CORE_XCAST(SubList, obj);
                        for (SubView view = &sv; view != null && view != &view->parent; view = &view->parent) {
                            concurrent::Lock& sync = view->lock;
                            sync.unlock();
                        }
                        Vector& v = sv.v;
                        concurrent::Lock& sync = v.lock;
                        sync.unlock();
                        break;
                    }
                    default:
                        CORE_ASSERT(false);
                }
            }
        };
    } // util
} // core

#endif //CORE24_VECTOR_H
