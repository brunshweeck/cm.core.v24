//
// Created by brunshweeck on 6 juil. 2024.
//

#ifndef CORE24_ARRAYLIST_H
#define CORE24_ARRAYLIST_H

#include <core/function/UnaryOperator.h>
#include <core/util/List.h>

namespace core {
    namespace util {
        /**
         * Resizable-array implementation of the @c List interface.  Implements
         * all optional list operations, and permits all elements.
         * In addition to implementing the @c List interface,
         * this class provides methods to manipulate the size of the array that is
         * used internally to store the list.  (This class is roughly equivalent to
         * @c Vector, except that it is unsynchronized.)
         * <p>
         * The @c size, @c isEmpty, @c get, @c set, @c firstElement, @c lastElement, @c removeLast,
         * @c iterator, @c listIterator, and @c reversed operations run in constant time.
         * The @c add, and @c addLast operations runs in <i>amortized constant time</i>,
         * that is, adding n elements requires O(n) time.  All the other operations run in
         * linear time (roughly speaking).  The constant factor is low compared to that for
         * the @c LinkedList implementation.
         * </p>
         * <p>
         * Each @c ArrayList instance has a <i>capacity</i>.  The capacity is
         * the size of the array used to store the elements in the list.  It is always
         * at least as large as the list size.  As elements are added to an ArrayList,
         * its capacity grows automatically.  The details of the growth policy are not
         * specified beyond the fact that adding an element has constant amortized
         * time cost.
         * </p>
         * <p>
         * An application can increase the capacity of an @c ArrayList instance
         * before adding a large number of elements using the @c ensureCapacity
         * operation.  This may reduce the amount of incremental reallocation.
         * </p>
         * <p>
         * <strong>Note that this implementation is not synchronized.</strong>
         * If multiple threads access an @c ArrayList instance concurrently,
         * and at least one of the threads modifies the list structurally, it
         * <i>must</i> be synchronized externally.  (A structural modification is
         * any operation that adds or deletes one or more elements, or explicitly
         * resizes the backing array; merely setting the value of an element is not
         * a structural modification.)  This is typically accomplished by
         * synchronizing on some object that naturally encapsulates the list.
         *
         * If no such object exists, the list should be "wrapped" using the
         * @c Collections::synchronizedList method.  This is best done at creation time,
         * to prevent accidental unsynchronized access to the list:
         * @code
         *   auto& list = Collections::synchronizedList(Unsafe::newInstance<ArrayList<Object>>(...));
         * @endcode
         * </p>
         * <p>
         * The iterators returned by this class's @c iterator and @c listIterator methods are
         * <em>fail-fast</em>: if the list is structurally modified at any time after the iterator
         * is created, in any way except through the iterator's own
         * @c ListIterator::remove or @c ListIterator::add methods, the iterator will throw a
         * @c ConcurrentModificationException.  Thus, in the face of concurrent modification, the iterator
         * fails quickly and cleanly, rather than risking arbitrary, non-deterministic behavior at an undetermined
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
         *
         * @tparam E the type of elements in this list
         *
         * @see     Collection
         * @see     List
         * @see     LinkedList
         * @see     Vector
         */
        template <class E>
        class ArrayList final : public virtual List<E>, public virtual RandomAccess {
            CORE_ALIAS(UNSAFE, misc::Unsafe);
            CORE_IMPORT_FIELD_OR_METHOD(List<E>, reverse);

            /**
             * Default initial capacity.
             */
            static CORE_FAST gint DEFAULT_CAPACITY = 10;

            /**
             * The array buffer into which the elements of the ArrayList are stored.
             * The capacity of the ArrayList is the length of this array buffer.
             */
            Array<> /*mutable*/ data;

            /**
             * The size of the ArrayList (the number of elements it contains).
             */
            gint count = 0;

            CORE_IMPORT_FIELD_OR_METHOD(List<E>, modCount);

        public:
            /**
             * Constructs an empty list with the specified initial capacity.
             *
             * @param  initialCapacity  the initial capacity of the list
             * @throws IllegalArgumentException if the specified initial capacity
             *         is negative
             */
            CORE_EXPLICIT ArrayList(gint initialCapacity) {
                if (initialCapacity > 0) {
                    data = Array<Object>(initialCapacity, null);
                } else if (initialCapacity == 0) {
                    data = { };
                } else {
                    IllegalArgumentException().throws($ftrace());
                }
            }

            /**
             * Constructs an empty list with an initial capacity of ten.
             */
            CORE_IMPLICIT ArrayList() {
                CORE_IGNORE(this);
            }

            /**
             * Constructs a list containing the elements of the specified
             * collection, in the order they are returned by the collection's
             * iterator.
             *
             * @param c the collection whose elements are to be placed into this list
             */
            CORE_EXPLICIT ArrayList(Collection<E> const& c) {
                Array<E> a = c.toArray();
                if ((count = a.length()) != 0) {
                    data = Arrays::copyOf<Object>(a, count);
                }
            }

            ArrayList(ArrayList const& c): data(c.data), count(c.count) {}

            ArrayList(ArrayList&& c) CORE_NOTHROW: data(UNSAFE::moveInstance(c.data)) {
                UNSAFE::swapValues(count, c.count);
                UNSAFE::swapValues(modCount, c.modCount);
            }

            ArrayList& operator=(ArrayList const& c) {
                if (this != &c) {
                    data = c.data;
                    count = c.count;
                    modCount = c.modCount;
                }
                return *this;
            }

            ArrayList& operator=(ArrayList&& c) CORE_NOTHROW {
                if (this != &c) {
                    data = UNSAFE::moveInstance(c.data);
                    UNSAFE::swapValues(count, c.count);
                    UNSAFE::swapValues(modCount, c.modCount);
                }
                return *this;
            }

            ~ArrayList() CORE_NOTHROW override {
                count = 0;
                if (reverse != null) {
                    UNSAFE::deleteRegInstance(*reverse);
                    reverse = null;
                }
            }

            /**
             * Trims the capacity of this @c ArrayList instance to be the
             * list's current size.  An application can use this operation to minimize
             * the storage of an @c ArrayList instance.
             */
            virtual void trimToSize() {
                modCount += 1;
                if (count < data.length()) {
                    data = Arrays::copyOf(data, count);
                }
            }

            /**
             * Increases the capacity of this @c ArrayList instance, if
             * necessary, to ensure that it can hold at least the number of elements
             * specified by the minimum capacity argument.
             *
             * @param minCapacity the desired minimum capacity
             */
            virtual void ensureCapacity(gint minCapacity) {
                if (minCapacity > data.length()) {
                    modCount += 1;
                    resize(minCapacity);
                }
            }

        private:
            /**
             * Increases the capacity to ensure that it can hold at least the
             * number of elements specified by the minimum capacity argument.
             *
             * @param minCapacity the desired minimum capacity
             * @throws OutOfMemoryError if minCapacity is less than zero
             */
            void resize(gint minCapacity) {
                gint oldCapacity = data.length();
                if (oldCapacity > 0) {
                    gint newCapacity = Arrays::newLength(oldCapacity,
                                                         minCapacity - oldCapacity,
                                                         oldCapacity >> 1);
                    data = Arrays::copyOf(data, newCapacity);
                } else {
                    data = Array<>(minCapacity, null);
                }
            }

            void resize() {
                try {
                    resize(count + 1);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

        public:
            /**
             * Returns the number of elements in this list.
             *
             * @return the number of elements in this list
             */
            gint size() const override {
                return count;
            }

            /**
             * Returns @c true if this list contains no elements.
             *
             * @return @c true if this list contains no elements
             */
            gbool isEmpty() const override {
                return count == 0;
            }

            /**
             * Returns @c true if this list contains the specified element.
             * More formally, returns @c true if and only if this list contains
             * at least one element @c e such that @c o.equals(o).
             *
             * @param val element whose presence in this list is to be tested
             * @return @c true if this list contains the specified element
             */
            gbool contains(const E& val) const override {
                return indexOf(val) >= 0;
            }

            /**
             * Returns the index of the first occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the lowest index @c i such that
             * @c Objects.equals(o, get(i)), or -1 if there is no such index.
             */
            gint indexOf(const E& val) const override {
                return indexOfRange(val, 0, count);
            }

        private:
            gint indexOfRange(E const& val, gint startIndex, gint endIndex) const {
                for (int i = startIndex; i < endIndex; ++i) {
                    if (data[i] == val) {
                        return i;
                    }
                }
                return -1;
            }

        public:
            /**
             * Returns the index of the last occurrence of the specified element
             * in this list, or -1 if this list does not contain the element.
             * More formally, returns the highest index @c i such that
             * @c Objects.equals(o, get(i)), or -1 if there is no such index.
             */
            gint lastIndexOf(E const& val) const override {
                return lastIndexOfRange(val, 0, count);
            }

        private:
            gint lastIndexOfRange(E const& val, gint startIndex, gint endIndex) const {
                for (int i = endIndex - 1; i >= startIndex; --i) {
                    if (data[i] == val) {
                        return i;
                    }
                }
                return -1;
            }

        public:
            /**
             * Returns a shallow copy of this @c ArrayList instance.  (The
             * elements themselves are not copied.)
             *
             * @return a clone of this @c ArrayList instance
             */
            Object& clone() const override {
                try {
                    ArrayList& clone = UNSAFE::newInstance<ArrayList>();
                    clone.data = data;
                    clone.count = count;
                    clone.modCount = 0;
                    return clone;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns an array containing all the elements in this list
             * in proper sequence (from first to last element).
             * <p>
             * The returned array will be "safe" in that no references to it are
             * maintained by this list.  (In other words, this method must allocate
             * a new array).  The caller is thus free to modify the returned array.
             * </p>
             * <p>
             * This method acts as bridge between array-based and collection-based
             * APIs.
             * </p>
             * @return an array containing all the elements in this list in
             *         proper sequence
             */
            Array<E> toArray() const override {
                Array<> const& es = data;
                return Arrays::copyOf<E>(es, count);
            }

        private:
            static E& elementAt(Array<Object>& data, gint index) {
                return CORE_XCAST(E, data[index]);
            }

            static E const& elementAt(Array<Object> const& data, gint index) {
                return CORE_XCAST(E const, data[index]);
            }

        public:
            /**
             * Returns the element at the specified position in this list.
             *
             * @param  index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexOutOfBoundsException
             */
            E& get(gint index) override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    return elementAt(data, index);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns the element at the specified position in this list.
             *
             * @param  index index of the element to return
             * @return the element at the specified position in this list
             * @throws IndexOutOfBoundsException
             */
            const E& get(gint index) const override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    return elementAt(data, index);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            E& firstElement() override {
                if (count != 0) {
                    return elementAt(data, 0);
                }
                NoSuchElementException().throws($ftrace());
            }

            const E& firstElement() const override {
                if (count != 0) {
                    return elementAt(data, 0);
                }
                NoSuchElementException().throws($ftrace());
            }

            E& lastElement() override {
                if (count != 0) {
                    return elementAt(data, count - 1);
                }
                NoSuchElementException().throws($ftrace());
            }

            const E& lastElement() const override {
                if (count != 0) {
                    return elementAt(data, count - 1);
                }
                NoSuchElementException().throws($ftrace());
            }

            /**
             * Replaces the element at the specified position in this list with
             * the specified element.
             *
             * @param index index of the element to replace
             * @param newValue element to be stored at the specified position
             * @return the element previously at the specified position
             * @throws IndexOutOfBoundsException
             */
            E& set(gint index, const E& newValue) override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    E& oldValue = elementAt(data, index);
                    data.set(index, newValue);
                    return oldValue;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Appends the specified element to the end of this list.
             *
             * @param val element to be appended to this list
             * @return @c true (as specified by @c Collection::add)
             */
            gbool add(const E& val) override {
                try {
                    modCount += 1;
                    Array<>& es = data;
                    if (count == es.length())
                        resize();
                    es.set(count++, val);
                    return true;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Inserts the specified element at the specified position in this
             * list. Shifts the element currently at that position (if any) and
             * any subsequent elements to the right (adds one to their indices).
             *
             * @param index index at which the specified element is to be inserted
             * @param val element to be inserted
             * @throws IndexOutOfBoundsException
             */
            void add(gint index, const E& val) override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    modCount += 1;
                    gint n = count;
                    if (count == data.length())
                        resize();
                    Arrays::copy(data, index, data, index + 1, n - index);
                    data.set(index, val);
                    count = n + 1;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            void addFirst(const E& val) override {
                try {
                    add(0, val);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            void addLast(const E& val) override {
                try {
                    add(val);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Removes the element at the specified position in this list.
             * Shifts any subsequent elements to the left (subtracts one from their
             * indices).
             *
             * @param index the index of the element to be removed
             * @return the element that was removed from the list
             * @throws IndexOutOfBoundsException
             */
            E& remove(gint index) override {
                try {
                    misc::Preconditions::checkIndex(index, count);
                    E& oldValue = elementAt(data, index);
                    removeAt(index);
                    return oldValue;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            E& removeFirst() override {
                if (count == 0)
                    NoSuchElementException().throws($ftrace());
                try {
                    return remove(0);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            E& removeLast() override {
                if (count == 0)
                    NoSuchElementException().throws($ftrace());
                try {
                    return remove(count - 1);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

        private:
            void checkCoModification(gint lastModCount) const {
                if ((lastModCount != modCount))
                    ConcurrentModificationException().throws($ftrace());
            }

            gbool equalToListForRange(List<E> const& list, gint fromIndex, gint toIndex) const {
                if ((toIndex > data.length())) {
                    ConcurrentModificationException().throws($ftrace());
                }
                try {
                    Iterator2<E>& it = list.iterator();
                    for (; fromIndex < toIndex; ++fromIndex)
                        if (!it.hasNext() || data[fromIndex] != it.next())
                            return false;
                    return !it.hasNext();
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            gbool equalToArrayList(ArrayList const& list) const {
                gint lastModCount = list.modCount;
                gint size = count;
                gbool equals;
                if ((equals = (size == list.count))) {
                    if ((size > data.length() || size > list.data.length())) {
                        ConcurrentModificationException().throws($ftrace());
                    }
                    for (int i = 0; i < size; ++i) {
                        if (data[i] != list.data[i]) {
                            equals = false;
                            break;
                        }
                    }
                }
                list.checkCoModification(lastModCount);
                return equals;
            }

        public:
            gbool equals(const Object& o) const override {
                try {
                    if (this == &o) {
                        return true;
                    }
                    if (!Class<List<E>>::hasInstance(o)) {
                        return false;
                    }
                    gint const lastModCount = modCount;
                    // ArrayList can be subclassed and given arbitrary behavior, but we can
                    // still deal with the common case where o is ArrayList precisely
                    gbool const equals = (Class<ArrayList>::hasInstance(o))
                                             ? equalToArrayList(CORE_XCAST(ArrayList const, o))
                                             : equalToListForRange(CORE_XCAST(List<E> const, o), 0, count);
                    checkCoModification(lastModCount);
                    return equals;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

        private:
            gint hashForRange(gint fromIndex, gint toIndex) const {
                if ((toIndex > data.length())) {
                    ConcurrentModificationException().throws($ftrace());
                }
                gint hash = 0;
                for (gint i = fromIndex; i < toIndex; ++i) {
                    hash = 31 * hash + data[i].hash();
                }
                return hash;
            }

        public:
            gint hash() const override {
                try {
                    gint const lastModCount = modCount;
                    gint const hash = hashForRange(0, count);
                    checkCoModification(lastModCount);
                    return hash;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

        private:
            void removeAt(gint index) {
                modCount += 1;
                gint const newSize = count - 1;
                if (newSize > index) {
                    Arrays::copy(data, index + 1, data, index, newSize - index);
                }
                data.set(count = newSize, null);
            }

        public:
            /**
             * Removes the first occurrence of the specified element from this list,
             * if it is present.  If the list does not contain the element, it is
             * unchanged.  More formally, removes the element with the lowest index
             * @c i such that
             * @c Objects.equals(o, get(i))
             * (if such an element exists).  Returns @c true if this list
             * contained the specified element (or equivalently, if this list
             * changed as a result of the call).
             *
             * @param val element to be removed from this list, if present
             * @return @c true if this list contained the specified element
             */
            gbool remove(const E& val) override {
                try {
                    gint const size = count;
                    gint i = 0;
                    for (; i < size; ++i) {
                        if (val == data[i]) {
                            goto FOUND;
                        }
                    }
                    return false;
                FOUND:
                    removeAt(i);
                    return true;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Removes all the elements from this list.  The list will
             * be empty after this call returns.
             */
            void clear() override {
                modCount += 1;
                for (int n = count, i = count = 0; i < n; ++i) {
                    data.set(i, null);
                }
            }

            /**
             * Appends all the elements in the specified collection to the end of
             * this list, in the order that they are returned by the
             * specified collection's Iterator.  The behavior of this operation is
             * undefined if the specified collection is modified while the operation
             * is in progress.  (This implies that the behavior of this call is
             * undefined if the specified collection is this list, and this
             * list is nonempty.)
             *
             * @param c collection containing elements to be added to this list
             * @return @c true if this list changed as a result of the call
             */
            gbool addAll(const Collection<E>& c) override {
                try {
                    Array<E> array = c.toArray();
                    modCount += 1;
                    gint size = array.length();
                    if (size == 0) {
                        return false;
                    }
                    gint oldSize = count;
                    if (size > data.length() - oldSize) {
                        resize(oldSize + size);
                    }
                    Arrays::copy(array, 0, data, oldSize, size);
                    count = oldSize + size;
                    return true;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Inserts all the elements in the specified collection into this
             * list, starting at the specified position.  Shifts the element
             * currently at that position (if any) and any subsequent elements to
             * the right (increases their indices).  The new elements will appear
             * in the list in the order that they are returned by the
             * specified collection's iterator.
             *
             * @param index index at which to insert the first element from the
             *              specified collection
             * @param c collection containing elements to be added to this list
             * @return @c true if this list changed as a result of the call
             * @throws IndexOutOfBoundsException
             */
            gbool addAll(gint index, const Collection<E>& c) override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    Array<E> array = c.toArray();
                    modCount += 1;
                    gint size = array.length();
                    if (size == 0) {
                        return false;
                    }
                    gint oldSize = count;
                    if (size > data.length() - oldSize) {
                        resize(oldSize + size);
                    }
                    gint n = oldSize - index;
                    if (n > 0) {
                        Arrays::copy(array, index, data, index + size, n);
                    }
                    Arrays::copy(array, 0, data, oldSize, size);
                    count = oldSize + size;
                    return true;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

        protected:
            /**
             * Removes from this list all the elements whose index is between
             * @c fromIndex, inclusive, and @c toIndex, exclusive.
             * Shifts any succeeding elements to the left (reduces their index).
             * This call shortens the list by @code (toIndex - fromIndex) @endcode elements.
             * (If @code toIndex==fromIndex @endcode , this operation has no effect.)
             *
             * @throws IndexOutOfBoundsException if @c fromIndex or
             *         @c toIndex is out of range
             *         (@code fromIndex < 0 ||
             *          toIndex > size() ||
             *          toIndex < fromIndex
             *          @endcode )
             */
            void removeRange(gint fromIndex, gint toIndex) {
                try {
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, count);
                    modCount += 1;
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

        private:
            /** Erases the gap from lo to hi, by sliding down following elements. */
            void shift(gint lo, gint hi) {
                Arrays::copy(data, hi, data, lo, count - hi);
                for (int to = count, i = (count -= hi - lo); i < to; i++) {
                    data.set(i, null);
                }
            }

        public:
            /**
             * Removes from this list all of its elements that are contained in the
             * specified collection.
             *
             * @param c collection containing elements to be removed from this list
             * @return @c true if this list changed as a result of the call
             * @see Collection::contains(Object)
             */
            gbool removeAll(const Collection<E>& c) override {
                try {
                    return removeAllForRange(c, 0, count, true);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Retains only the elements in this list that are contained in the
             * specified collection.  In other words, removes from this list all
             * of its elements that are not contained in the specified collection.
             *
             * @param c collection containing elements to be retained in this list
             * @return @c true if this list changed as a result of the call
             * @see Collection::contains(Object)
             */
            gbool retainAll(const Collection<E>& c) override {
                try {
                    return removeAllForRange(c, 0, count, false);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

        private:
            gbool removeAllForRange(Collection<E> const& c, gint from, gint to, gbool existing) {
                gint r;
                gint w = -1;
                Array<>& es = data;
                try {
                    // Optimize for initial run of survivors
                    for (r = from; ; r++) {
                        if (r == to)
                            return false;
                        if (c.contains(elementAt(es, r)) == existing)
                            break;
                    }
                    w = r++;
                    for (; r < to; r++) {
                        E const& val = elementAt(es, r);
                        if (c.contains(val) == existing)
                            data.set(w++, val);
                    }
                    modCount += to - w;
                    shift(w, to);
                } catch (Throwable const& ex) {
                    // Preserve behavioral compatibility with AbstractCollection,
                    // even if c.contains() throws.
                    if (w != -1) {
                        Arrays::copy(data, r, data, w, to - r);
                        w += to - r;
                        modCount += to - w;
                        shift(w, to);
                    }
                    ex.throws($ftrace());
                }
                return true;
            }

        public:
            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to @c ListIterator::next.
             * An initial call to @c ListIterator::previous would
             * return the element with the specified index minus one.
             *
             * <p>The returned list iterator is <i>fail-fast</i>.
             *
             * @throws IndexOutOfBoundsException
             */
            ListIterator<E>& listIterator(gint index) override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    return UNSAFE::newInstance<AscendingListIterator>(*this, index);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence), starting at the specified position in the list.
             * The specified index indicates the first element that would be
             * returned by an initial call to @c ListIterator::next.
             * An initial call to @c ListIterator::previous would
             * return the element with the specified index minus one.
             *
             * <p>The returned list iterator is <i>fail-fast</i>.
             *
             * @throws IndexOutOfBoundsException
             */
            ListIterator2<E>& listIterator(gint index) const override {
                try {
                    misc::Preconditions::checkIndexForAdding(index, count);
                    return UNSAFE::newInstance<AscendingListIterator2>(*this, index);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence).
             *
             * <p>The returned list iterator is <i>fail-fast</i>.
             *
             * @see #listIterator(int)
             */
            ListIterator<E>& listIterator() override {
                try {
                    return UNSAFE::newInstance<AscendingListIterator>(*this, 0);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns a list iterator over the elements in this list (in proper
             * sequence).
             *
             * <p>The returned list iterator is <i>fail-fast</i>.
             *
             * @see #listIterator(int)
             */
            ListIterator2<E>& listIterator() const override {
                try {
                    return UNSAFE::newInstance<AscendingListIterator2>(*this, 0);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns an iterator over the elements in this list in proper sequence.
             *
             * <p>The returned iterator is <i>fail-fast</i>.
             *
             * @return an iterator over the elements in this list in proper sequence
             */
            Iterator<E>& iterator() override {
                try {
                    return UNSAFE::newInstance<AscendingIterator>(*this);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns an iterator over the elements in this list in proper sequence.
             *
             * <p>The returned iterator is <i>fail-fast</i>.
             *
             * @return an iterator over the elements in this list in proper sequence
             */
            Iterator2<E>& iterator() const override {
                try {
                    return UNSAFE::newInstance<AscendingIterator2>(*this);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

        private:
            /**
             * An optimized version of List::AscendingIterator
             */
            class AscendingIterator : public virtual Iterator<E> {
            protected:
                ArrayList& list;
                gint cursor = 0;
                gint lastIndex = -1;
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT AscendingIterator(ArrayList& list): list(list), lastModCount(list.modCount) {}

                gbool hasNext() const override {
                    return cursor < list.count;
                }

                E& next() override {
                    if ((lastModCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    gint i = cursor;
                    if (i >= list.count)
                        NoSuchElementException().throws($ftrace());
                    Array<Object>& es = list.data;
                    if ((i >= es.length()))
                        ConcurrentModificationException().throws($ftrace());
                    cursor = i + 1;
                    lastIndex = i;
                    return elementAt(es, i);
                }

                void remove() override {
                    if (lastIndex < 0)
                        IllegalStateException().throws($ftrace());
                    if ((lastModCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        list.remove(lastIndex);
                        cursor = lastIndex;
                        lastIndex = -1;
                        lastModCount = list.modCount;
                    } catch (IndexOutOfBoundsException const& ex) {
                        CORE_IGNORE(ex);
                        ConcurrentModificationException().throws($ftrace());
                    }
                    catch (Throwable const& ex) {
                        ex.throws($ftrace());
                    }
                }

                void forEach(const function::Consumer<E&>& action) override {
                    gint size = list.count;
                    gint i = cursor;
                    Array<Object>& es = list.data;
                    if (i < size) {
                        if ((i > es.length()))
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            for (; i < size && lastModCount == list.modCount; i++)
                                action.accept(elementAt(es, i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        // update once at end to reduce heap write traffic
                        cursor = i;
                        lastIndex = i - 1;
                        if ((lastModCount != list.modCount)) {
                            ConcurrentModificationException().throws($ftrace());
                        }
                    }
                }

                gbool equals(const Object& obj) const override {
                    if (this == &obj)
                        return true;
                    if (Class<AscendingIterator>::hasInstance(obj)) {
                        AscendingIterator const& it = CORE_XCAST(AscendingIterator const, obj);
                        return &list == &it.list && cursor == it.cursor;
                    }
                    return false;
                }
            };

            /**
             * An optimized version of List::AscendingIterator2
             */
            class AscendingIterator2 : public virtual Iterator2<E> {
            protected:
                ArrayList const& list;
                gint cursor = 0;
                gint lastIndex = -1;
                gint lastModCount = 0;

            public:
                CORE_EXPLICIT AscendingIterator2(ArrayList const& list): list(list), lastModCount(list.modCount) {}

                gbool hasNext() const override {
                    return cursor < list.count;
                }

                E const& next() override {
                    if ((lastModCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    gint i = cursor;
                    if (i >= list.count)
                        NoSuchElementException().throws($ftrace());
                    Array<Object> const& es = list.data;
                    if ((i >= es.length()))
                        ConcurrentModificationException().throws($ftrace());
                    cursor = i + 1;
                    lastIndex = i;
                    return elementAt(es, i);
                }

                void forEach(const function::Consumer<E>& action) override {
                    gint size = list.count;
                    gint i = cursor;
                    Array<Object> const& es = list.data;
                    if (i < size) {
                        if ((i > es.length()))
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            for (; i < size && lastModCount == list.modCount; i++)
                                action.accept(elementAt(es, i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        // update once at end to reduce heap write traffic
                        cursor = i;
                        lastIndex = i - 1;
                        if ((lastModCount != list.modCount)) {
                            ConcurrentModificationException().throws($ftrace());
                        }
                    }
                }

                gbool equals(const Object& obj) const override {
                    if (this == &obj)
                        return true;
                    if (Class<AscendingIterator2>::hasInstance(obj)) {
                        AscendingIterator2 const& it = CORE_XCAST(AscendingIterator2 const, obj);
                        return &list == &it.list && cursor == it.cursor;
                    }
                    return false;
                }
            };

            /**
             * An optimized version of List::AscendingListIterator
             */
            class AscendingListIterator : public virtual AscendingIterator, public virtual ListIterator<E> {
                CORE_IMPORT_FIELD_OR_METHOD(AscendingIterator, list);
                CORE_IMPORT_FIELD_OR_METHOD(AscendingIterator, cursor);
                CORE_IMPORT_FIELD_OR_METHOD(AscendingIterator, lastIndex);
                CORE_IMPORT_FIELD_OR_METHOD(AscendingIterator, lastModCount);

            public:
                CORE_EXPLICIT AscendingListIterator(ArrayList& list, gint index) : AscendingIterator(list) {
                    cursor = index;
                }

                gbool hasPrevious() const override {
                    return cursor != 0;
                }

                gbool hasNext() const override {
                    return AscendingIterator::hasNext();
                }

                E& next() override {
                    return AscendingIterator::next();
                }

                void remove() override {
                    AscendingIterator::remove();
                }

                E& previous() override {
                    if ((lastModCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    gint i = cursor - 1;
                    if (i < 0)
                        NoSuchElementException().throws($ftrace());
                    Array<Object>& es = list.data;
                    if ((i >= es.length()))
                        ConcurrentModificationException().throws($ftrace());
                    cursor = lastIndex = i;
                    return elementAt(es, i);
                }

                gint nextIndex() const override {
                    return cursor;
                }

                gint previousIndex() const override {
                    return cursor - 1;
                }

                void set(const E& e) override {
                    if (lastIndex < 0)
                        IllegalStateException().throws($ftrace());
                    if ((lastModCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        list.set(lastIndex, e);
                    } catch (IndexOutOfBoundsException const& _) {
                        ConcurrentModificationException().throws($ftrace());
                    } catch (Throwable const& ex) {
                        ex.throws($ftrace());
                    }
                }

                void add(const E& e) override {
                    if ((lastModCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        gint i = cursor;
                        list.add(i, e);
                        cursor = i + 1;
                        lastIndex = -1;
                    } catch (IndexOutOfBoundsException const& _) {
                        ConcurrentModificationException().throws($ftrace());
                    }
                    catch (Throwable const& ex) {
                        ex.throws($ftrace());
                    }
                }
            };

            /**
             * An optimized version of List::AscendingListIterator2
             */
            class AscendingListIterator2 : public virtual AscendingIterator2, public virtual ListIterator2<E> {
                CORE_IMPORT_FIELD_OR_METHOD(AscendingIterator2, list);
                CORE_IMPORT_FIELD_OR_METHOD(AscendingIterator2, cursor);
                CORE_IMPORT_FIELD_OR_METHOD(AscendingIterator2, lastIndex);
                CORE_IMPORT_FIELD_OR_METHOD(AscendingIterator2, lastModCount);

            public:
                CORE_EXPLICIT AscendingListIterator2(ArrayList const& list, gint index): AscendingIterator2(list) {
                    cursor = index;
                }

                gbool hasNext() const override {
                    return AscendingIterator2::hasNext();
                }

                const E& next() override {
                    return AscendingIterator2::next();
                }

                void remove() override {
                    AscendingIterator2::remove();
                }

                gbool hasPrevious() const override {
                    return cursor != 0;
                }

                E const& previous() override {
                    if ((lastModCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    gint i = cursor - 1;
                    if (i < 0)
                        NoSuchElementException().throws($ftrace());
                    Array<Object> const& es = list.data;
                    if ((i >= es.length()))
                        ConcurrentModificationException().throws($ftrace());
                    cursor = lastIndex = i;
                    return elementAt(es, i);
                }

                gint nextIndex() const override {
                    return cursor;
                }

                gint previousIndex() const override {
                    return cursor - 1;
                }
            };

        public:
            /**
             * Returns a view of the portion of this list between the specified
             * @c fromIndex, inclusive, and @c toIndex, exclusive.  (If
             * @c fromIndex and @c toIndex are equal, the returned list is
             * empty.)  The returned list is backed by this list, so non-structural
             * changes in the returned list are reflected in this list, and vice versa.
             * The returned list supports all the optional list operations.
             *
             * <p>This method eliminates the need for explicit range operations (of
             * the sort that commonly exist for arrays).  Any operation that expects
             * a list can be used as a range operation by passing a subList view
             * instead of a whole list.  For example, the following idiom
             * removes a range of elements from a list:
             * @code
             *      list.subList(from, to).clear();
             * @endcode
             * Similar idioms may be constructed for @c indexOf and
             * @c lastIndexOf(Object), and all the algorithms in the
             * @c Collections class can be applied to a subList.
             *
             * <p>The semantics of the list returned by this method become undefined if
             * the backing list (i.e., this list) is <i>structurally modified</i> in
             * any way other than via the returned list.  (Structural modifications are
             * those that change the size of this list, or otherwise perturb it in such
             * a fashion that iterations in progress may yield incorrect results.)
             *
             * @throws IndexOutOfBoundsException
             * @throws IllegalArgumentException
             */
            const List<E>& subList(gint fromIndex, gint toIndex) const override {
                try {
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, count);
                    return UNSAFE::newInstance<SubList>(CORE_CAST(ArrayList &, *this), fromIndex, toIndex);
                    // return *new SubList(*this, fromIndex, toIndex);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns a view of the portion of this list between the specified
             * @c fromIndex, inclusive, and @c toIndex, exclusive.  (If
             * @c fromIndex and @c toIndex are equal, the returned list is
             * empty.)  The returned list is backed by this list, so non-structural
             * changes in the returned list are reflected in this list, and vice versa.
             * The returned list supports all the optional list operations.
             *
             * <p>This method eliminates the need for explicit range operations (of
             * the sort that commonly exist for arrays).  Any operation that expects
             * a list can be used as a range operation by passing a subList view
             * instead of a whole list.  For example, the following idiom
             * removes a range of elements from a list:
             * @code
             *      list.subList(from, to).clear();
             * @endcode
             * Similar idioms may be constructed for @c indexOf and
             * @c lastIndexOf(Object), and all the algorithms in the
             * @c Collections class can be applied to a subList.
             *
             * <p>The semantics of the list returned by this method become undefined if
             * the backing list (i.e., this list) is <i>structurally modified</i> in
             * any way other than via the returned list.  (Structural modifications are
             * those that change the size of this list, or otherwise perturb it in such
             * a fashion that iterations in progress may yield incorrect results.)
             *
             * @throws IndexOutOfBoundsException
             * @throws IllegalArgumentException
             */
            List<E>& subList(gint fromIndex, gint toIndex) override {
                try {
                    misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, count);
                    return UNSAFE::newInstance<SubList>(*this, fromIndex, toIndex);
                    // return *new SubList(*this, fromIndex, toIndex);
                } catch (Throwable const& ex) {
                    ex.throws($ftrace());
                }
            }

        private:
            class SubList : public virtual List<E>, public virtual RandomAccess {
                SubList const& parent;
                ArrayList& list;
                gint offset = 0;
                gint count = 0;

                CORE_IMPORT_FIELD_OR_METHOD(List<E>, modCount);

                void synchronize(gint n) {
                    CORE_ALIAS(SL, typename Class<SubList>::Pointer);
                    SL self = this;
                    do {
                        self->count += n;
                        self->modCount = list.modCount;
                        self = CORE_CAST(SL, &self->parent);
                    } while (self != this);
                }

                void checkCoModification() const {
                    if ((modCount != list.modCount)) {
                        ConcurrentModificationException().throws($ftrace());
                    }
                }

            public:
                CORE_EXPLICIT SubList(ArrayList& list, gint fromIndex, gint toIndex): parent(*this), list(list),
                    offset(fromIndex), count(toIndex - fromIndex) {}

                SubList(SubList& parent, gint fromIndex, gint toIndex)
                    : parent(parent), list(parent.list), offset(fromIndex), count(toIndex - fromIndex) {}

                gint size() const override {
                    return count;
                }

                E& get(gint index) override {
                    try {
                        misc::Preconditions::checkIndex(index, count);
                        Array<Object>& es = list.data;
                        return elementAt(es, offset + index);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                const E& get(gint index) const override {
                    try {
                        misc::Preconditions::checkIndex(index, count);
                        Array<Object> const& es = list.data;
                        return elementAt(es, offset + index);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                E& set(gint index, const E& newValue) override {
                    try {
                        misc::Preconditions::checkIndex(index, count);
                        Array<Object>& es = list.data;
                        E& oldValue = elementAt(es, offset + index);
                        es.set(offset + index, newValue);
                        return oldValue;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gbool contains(const E& val) const override {
                    try {
                        return indexOf(val) >= 0;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                void forEach(const function::Consumer<E&>& action) override {
                    Array<Object>& es = list.data;
                    for (int i = 0, n = count; i < n; ++i) {
                        try {
                            action.accept(elementAt(es, offset + i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if ((modCount != list.modCount))
                            ConcurrentModificationException().throws($ftrace());
                    }
                }

                void forEach(const function::Consumer<E>& action) const override {
                    Array<Object> const& es = list.data;
                    for (int i = 0, n = count; i < n; ++i) {
                        try {
                            action.accept(elementAt(es, offset + i));
                        } catch (Throwable const& ex) { ex.throws($ftrace()); }
                        if ((modCount != list.modCount))
                            ConcurrentModificationException().throws($ftrace());
                    }
                }

                gbool isEmpty() const override {
                    return count == 0;
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

            private:
                class AscendingListIterator : public virtual ListIterator<E> {
                    SubList& self;
                    gint cursor = 0;
                    gint lastIndex = -1;
                    gint lastModCount = 0;

                public:
                    CORE_EXPLICIT AscendingListIterator(SubList& list, gint index) : self(list), cursor(index),
                        lastModCount(list.modCount) {}

                    gbool hasNext() const override {
                        return cursor != self.count;
                    }

                    E& next() override {
                        if ((lastModCount != self.modCount)) {
                            ConcurrentModificationException().throws($ftrace());
                        }
                        gint i = cursor;
                        if (i >= self.count) {
                            NoSuchElementException().throws(
                                $ftrace());
                        }
                        Array<>& es = self.list.data;
                        if ((self.offset + i >= es.length())) {
                            ConcurrentModificationException().throws($ftrace());
                        }
                        cursor = i + 1;
                        lastIndex = i;
                        return elementAt(es, i);
                    }

                    gbool hasPrevious() const override {
                        return cursor != 0;
                    }

                    E& previous() override {
                        if ((lastModCount != self.modCount))
                            ConcurrentModificationException().throws($ftrace());
                        gint i = cursor - 1;
                        if (i < 0)
                            NoSuchElementException().throws(
                                $ftrace());
                        Array<>& es = self.list.data;
                        if ((self.offset + i >= es.length()))
                            ConcurrentModificationException().throws($ftrace());
                        cursor = lastIndex = i;
                        return elementAt(es, self.offset + i);
                    }

                    gint nextIndex() const override {
                        return cursor;
                    }

                    gint previousIndex() const override {
                        return cursor - 1;
                    }

                    void set(const E& e) override {
                        if (lastIndex < 0)
                            NoSuchElementException().throws(
                                $ftrace());
                        if ((lastModCount != self.list.modCount))
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            self.remove(lastIndex);
                            cursor = lastIndex;
                            lastIndex = -1;
                            lastModCount = self.modCount;
                        } catch (IndexOutOfBoundsException const& _) {
                            ConcurrentModificationException().throws($ftrace());
                        }
                        catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    gbool equals(const Object& obj) const override {
                        if (this == &obj) {
                            return true;
                        }
                        if (Class<AscendingListIterator>::hasInstance(obj)) {
                            AscendingListIterator const& it = CORE_XCAST(AscendingListIterator const, obj);
                            return &self != &it.self && cursor == it.cursor;
                        }
                        return false;
                    }

                    void forEach(const function::Consumer<E&>& action) override {
                        Array<>& es = self.list.data;
                        gint count = self.count;
                        for (gint i = 0; i < count; i++) {
                            try {
                                action.accept(elementAt(es, self.offset + i));
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                            if ((lastModCount != self.list.modCount))
                                ConcurrentModificationException().throws($ftrace());
                        }
                    }

                    void remove() override {
                        if (lastIndex < 0)
                            IllegalStateException().throws($ftrace());
                        if ((lastModCount != self.list.modCount))
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            self.remove(lastIndex);
                            cursor = lastIndex;
                            lastIndex = -1;
                            lastModCount = self.modCount;
                        } catch (IndexOutOfBoundsException const& ex) {
                            CORE_IGNORE(ex);
                            ConcurrentModificationException().throws($ftrace());
                        }
                        catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }

                    void add(const E& e) override {
                        if ((lastModCount != self.list.modCount))
                            ConcurrentModificationException().throws($ftrace());
                        try {
                            gint i = cursor;
                            self.add(i, e);
                            cursor = i + 1;
                            lastIndex = -1;
                            lastModCount = self.modCount;
                        } catch (IndexOutOfBoundsException const& ex) {
                            CORE_IGNORE(ex);
                            ConcurrentModificationException().throws($ftrace());
                        }
                        catch (Throwable const& ex) { ex.throws($ftrace()); }
                    }
                };

                class AscendingListIterator2 : public virtual ListIterator2<E> {
                    SubList const& self;
                    gint cursor = 0;
                    gint lastIndex = -1;
                    gint lastModCount = 0;

                public:
                    CORE_EXPLICIT AscendingListIterator2(SubList const& list, gint index) : self(list), cursor(index),
                        lastModCount(list.modCount) {}

                    gbool hasNext() const override {
                        return cursor != self.count;
                    }

                    E const& next() override {
                        if ((lastModCount != self.modCount)) {
                            ConcurrentModificationException().throws($ftrace());
                        }
                        gint i = cursor;
                        if (i >= self.count) {
                            NoSuchElementException().throws(
                                $ftrace());
                        }
                        Array<Object> const& es = self.list.data;
                        if ((self.offset + i >= es.length())) {
                            ConcurrentModificationException().throws($ftrace());
                        }
                        cursor = i + 1;
                        lastIndex = i;
                        return elementAt(es, i);
                    }

                    gbool hasPrevious() const override {
                        return cursor != 0;
                    }

                    E const& previous() override {
                        if ((lastModCount != self.modCount))
                            ConcurrentModificationException().throws($ftrace());
                        gint i = cursor - 1;
                        if (i < 0)
                            NoSuchElementException().throws(
                                $ftrace());
                        Array<Object> const& es = self.list.data;
                        if ((self.offset + i >= es.length()))
                            ConcurrentModificationException().throws($ftrace());
                        cursor = lastIndex = i;
                        return elementAt(es, self.offset + i);
                    }

                    gint nextIndex() const override {
                        return cursor;
                    }

                    gint previousIndex() const override {
                        return cursor - 1;
                    }

                    gbool equals(const Object& obj) const override {
                        if (this == &obj) {
                            return true;
                        }
                        if (Class<AscendingListIterator2>::hasInstance(obj)) {
                            AscendingListIterator2 const& it = CORE_XCAST(AscendingListIterator2 const, obj);
                            return &self != &it.self && cursor == it.cursor;
                        }
                        return false;
                    }

                    void forEach(const function::Consumer<E>& action) override {
                        Array<Object>& es = self.list.data;
                        gint count = self.count;
                        for (gint i = 0; i < count; i++) {
                            try {
                                action.accept(elementAt(es, self.offset + i));
                            } catch (Throwable const& ex) { ex.throws($ftrace()); }
                            if ((lastModCount != self.list.modCount))
                                ConcurrentModificationException().throws($ftrace());
                        }
                    }
                };

            public:
                ListIterator<E>& listIterator(gint index) override {
                    try {
                        misc::Preconditions::checkIndex(index, count);
                        return UNSAFE::newInstance<AscendingListIterator>(*this, index);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                ListIterator2<E>& listIterator(gint index) const override {
                    try {
                        misc::Preconditions::checkIndex(index, count);
                        return UNSAFE::newInstance<AscendingListIterator2>(*this, index);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gbool add(const E& val) override {
                    if ((modCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        list.add(offset + count, val);
                        synchronize(+1);
                        return true;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gbool remove(const E& val) override {
                    if ((modCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        gint const i = indexOf(val);
                        if (i >= 0)
                            remove(i);
                        return true;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gbool addAll(const Collection<E>& c) override {
                    if ((modCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        return addAll(count, c);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gbool addAll(gint index, const Collection<E>& c) override {
                    if ((modCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        gint n = c.size();
                        if (list.addAll(index, c)) {
                            synchronize(+n);
                            return true;
                        }
                        return false;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                void replaceAll(const function::UnaryOperator<E>& op) override {
                    if ((modCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    try {
                        Array<Object>& es = list.data;
                        for (int i = 0; i < count; ++i) {
                            es.set(offset + i, op.apply(elementAt(es, offset + i)));
                            checkCoModification();
                        }
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                void sort(const Comparator<E>& c) override {
                    if ((modCount != list.modCount))
                        ConcurrentModificationException().throws($ftrace());
                    Array<Object>& es = list.data;
                    Arrays::sort(es,
                                 offset,
                                 offset + count,
                                 Comparator<Object>::comparing<E>(
                                     [](Object const& o) -> E const& { return CORE_XCAST(E const, o); }, c)
                    );
                }

                void add(gint index, const E& val) override {
                    try {
                        misc::Preconditions::checkIndex(index, count);
                        checkCoModification();
                        list.add(offset + index, val);
                        synchronize(+1);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                E& remove(gint index) override {
                    try {
                        misc::Preconditions::checkIndex(index, count);
                        checkCoModification();
                        E& oldValue = list.remove(offset + index);
                        synchronize(-1);
                        return oldValue;
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gint indexOf(const E& val) const override {
                    try {
                        checkCoModification();
                        return list.indexOfRange(val, offset, offset + count);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gint lastIndexOf(const E& val) const override {
                    try {
                        checkCoModification();
                        return list.lastIndexOfRange(val, offset, offset + count);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                void clear() override {
                    try {
                        checkCoModification();
                        list.removeRange(offset, offset + count);
                        synchronize(count);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gint hash() const override {
                    try {
                        checkCoModification();
                        return list.hashForRange(offset, offset + count);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                List<E>& subList(gint fromIndex, gint toIndex) const override {
                    try {
                        misc::Preconditions::checkIndexFromRange(fromIndex, toIndex, count);
                        checkCoModification();
                        return UNSAFE::newInstance<SubList>(CORE_CAST(SubList &, *this), fromIndex, toIndex);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                Array<E> toArray() const override {
                    try {
                        checkCoModification();
                        const Array<>& es = list.data;
                        return Arrays::copyOfRange<E>(es, offset, offset + count);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gbool removeAll(const Collection<E>& c) override {
                    try {
                        checkCoModification();
                        return list.removeAllForRange(c, offset, offset + count, true);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                gbool retainAll(const Collection<E>& c) override {
                    try {
                        checkCoModification();
                        return list.removeAllForRange(c, offset, offset + count, false);
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }

                String toString() const override {
                    try {
                        checkCoModification();
                        XString str;
                        Array<>& es = list.data;
                        for (int i = 0; ; ++i) {
                            Object const& val = es[offset + i];
                            (this == &val) ? str.append("{..}"_S) : str.append(val);
                            if (i < count - 1) str.append(u',').append(u' ');
                            else return str.append(u']').toString();
                        }
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                }
            };

        public:
            void forEach(const function::Consumer<E&>& action) override {
                gint lastModCount = modCount;
                Array<>& es = data;
                for (int i = 0; i < count; ++i) {
                    E& val = elementAt(es, i);
                    try { action.accept(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    if (lastModCount != modCount)
                        ConcurrentModificationException().throws($ftrace());
                }
            }

            void forEach(const function::Consumer<E>& action) const override {
                gint lastModCount = modCount;
                Array<> const& es = data;
                for (int i = 0; i < count; ++i) {
                    E const& val = elementAt(es, i);
                    try { action.accept(val); } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    if (lastModCount != modCount)
                        ConcurrentModificationException().throws($ftrace());
                }
            }

            gbool removeIf(const function::Predicate<E>& filter) override {
                gint size = count;
                gint count = 0;
                gint lastModCount = modCount;
                Array<>& es = data;
                for (int i = 0; i < size; ++i) {
                    E& val = elementAt(es, i);
                    gbool b;
                    try {
                        b = filter.test(val);
                    } catch (Throwable const& ex) {
                        // Shift remainder from -count places
                        for (int j = i; j < count; ++j)
                            Arrays::swap(es, j, j - count);
                        ArrayList::count = size - count;
                        ex.throws($ftrace());
                    }
                    if (lastModCount != modCount) {
                        if (count > 0) {
                            // Shift remainder from -count places
                            for (int j = i; j < count; ++j)
                                Arrays::swap(es, j, j - count);
                            ArrayList::count = size - count;
                        }
                        ConcurrentModificationException().throws($ftrace());
                    }
                    if (b) {
                        count += 1;
                        continue;
                    }
                    if (count > 0) {
                        // Shifty the current element from -count places.
                        // es[i] <=> es[i-count]
                        modCount = lastModCount += 1;
                        Arrays::swap(es, i, i - count);
                    }
                }
                ArrayList::count = size - count;
                if (count > 0)
                    Arrays::fill(es, size - count, size, CORE_XCAST(Object, null));
                return count > 0;
            }

            String toString() const override {
                XString xs;
                Array<> const& es = data;
                for (int i = 0; ; ++i) {
                    Object const& val = elementAt(es, i);
                    if (this == &val)
                        xs.append("{..}"_S);
                    else
                        xs.append(val);
                    if (i < count - 1)
                        xs.append(u',').append(u' ');
                    else
                        return xs.append(u']').toString();
                }
            }

            void replaceAll(const function::UnaryOperator<E>& op) override {
                gint lastModCount = modCount;
                Array<>& es = data;
                for (int i = 0; i < count; ++i) {
                    E& val = elementAt(es, i);

                    try {
                        es.set(i, op.apply(val));
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    if (lastModCount != modCount)
                        ConcurrentModificationException().throws($ftrace());
                }
            }

            void sort(const Comparator<E>& c) override {
                try {
                    Array<>& es = data;
                    Arrays::sort(es,
                                 0,
                                 count,
                                 Comparator<>::comparing<E>([](Object const& o) -> E const& {
                                     return CORE_XCAST(E const, o);
                                 }, c));
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }

            gbool containsAll(const Collection<E>& c) const override {
                if (this == &c)
                    return true;

                try {
                    for (E const& val : c)
                        if (!contains(val))
                            return false;

                    return true;
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
            }
        };
    } // util
} // core

#endif //CORE24_ARRAYLIST_H
