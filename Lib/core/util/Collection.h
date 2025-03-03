//
// Created by brunshweeck on 21 juin 2024.
//

#ifndef CORE24_COLLECTION_H
#define CORE24_COLLECTION_H

#include "Arrays.h"

#include <core/function/Predicate.h>
#include <core/lang/Iterable.h>

namespace core {
    namespace util {
        /**
         * The root interface in the <i>collection hierarchy</i>.  A collection
         * represents a group of objects, known as its <i>elements</i>.  Some
         * collections allow duplicate elements and others do not.  Some are ordered,
         * and others are unordered. Collections that have a defined encounter order
         * are generally subtypes of the @c SequencedCollection interface.
         * <p>
         * <i>Bags</i> or <i>multi-sets</i> (unordered collections that may contain
         * duplicate elements) should implement this interface directly.
         * </p>
         * <p>
         * All general-purpose @c Collection implementation classes (which typically
         * implement @c Collection indirectly through one of its sub-interfaces) should
         * provide two "standard" constructors: a void (no arguments) constructor, which
         * creates an empty collection, and a constructor with a single argument of type
         * @c Collection, which creates a new collection with the same elements as its argument.
         * In effect, the latter constructor allows the user to copy any collection, producing
         * an equivalent collection of the desired implementation type.
         * </p>
         * <p>
         * Certain methods are specified to be <i>optional</i>. If a collection implementation
         * doesn't implement a particular operation, it should define the corresponding method
         * to throw @c UnsupportedOperationException. Such methods are marked "optional operation"
         * in method specifications of the collections interfaces.
         * </p>
         * <p> Some collection implementations have restrictions on the elements that they may contain.
         * For example, some implementations have restrictions on the types of their elements. Attempting
         * to query the presence of an ineligible element may throw an exception, or it may simply return false;
         * some implementations will exhibit the former behavior and some will exhibit the latter.  More generally,
         * attempting an operation on an ineligible element whose completion would not result in
         * the insertion of an ineligible element into the collection may throw an exception, or it may succeed,
         * at the option of the implementation. Such exceptions are marked as "optional" in the specification
         * for this interface.
         * </p>
         * <p>
         * It is up to each collection to determine its own synchronization policy.  In the absence of a stronger
         * guarantee by the implementation, undefined behavior may result from the invocation of any method on a
         * collection that is being mutated by another thread; this includes direct invocations, passing the collection
         * to a method that might perform invocations, and using an existing iterator to examine the collection.
         * </p>
         * <p>
         * Many methods in Collections Framework interfaces are defined in terms of the @c Object::equals method.
         * For example, the specification for the @c Collection::contains method says: <<returns @c true if
         * and only if this collection contains at least one element @c e such that @c o.equals(e)>>.
         * This specification should <i>not</i> be construed to imply that invoking @c Collection::contains
         * with an argument @c o will cause @c o.equals(e) to be invoked for any element @c e.  Implementations
         * are free to implement optimizations whereby the @c Collection::equals invocation is avoided, for
         * example, by first comparing the hash codes of the two elements.  (The @c Object::hash specification
         * guarantees that two objects with unequal hash codes cannot be equal.)  More generally, implementations
         * of the various Collections Framework interfaces are free to take advantage of the specified behavior of
         * underlying @c Object methods wherever the implementor deems it appropriate.
         * </p>
         * <p>
         * Some collection operations which perform recursive traversal of the collection may fail with an exception
         * for self-referential instances where the collection directly or indirectly contains itself. This includes
         * the @c Collection::clone, @c Collection::equals, @c Collection::hash and @c Collection::toString methods.
         * Implementations may optionally handle the self-referential scenario, however most current implementations
         * do not do so.
         * </p>
         * <h2>Views Collections</h2>
         * <p>
         * Most collections manage storage for elements they contain. By contrast, <i>view collections</i> themselves
         * do not store elements, but instead they rely on a backing collection to store the actual elements.
         * Operations that are not handled by the view collection itself are delegated to the backing collection.
         * Examples of view collections include collections that provide a different representation of the same
         * elements, for example, as provided by @c List::subList, @c NavigableSet::subSet, @c Map::entrySet, or
         * @c SequencedCollection::reversed. Any changes made to the backing collection are visible in the view
         * collection. Correspondingly, any changes made to the view collection &mdash; if changes are permitted &mdash;
         * are written through to the backing collection. Although they technically aren't collections, instances of
         * @c Iterator and @c ListIterator can also allow modifications to be written through to the backing collection,
         * and in some cases, modifications to the backing collection will be visible to the Iterator during iteration.
         * </p>
         * <h2>Unmodifiable Collections</h2>
         * <p>
         * Certain methods of this interface are considered "destructive" and are called "mutator" methods in that
         * they modify the group of objects contained within the collection on which they operate. They can be
         * specified to throw @c UnsupportedOperationException if this collection implementation does not support
         * the operation. Such methods should (but are not required to) throw an @c UnsupportedOperationException
         * if the invocation would have no effect on the collection. For example, consider a collection that
         * does not support the @c Collection::add operation. What will happen if the @c Collection::addAll method
         * is invoked on this collection, with an empty collection as the argument? The addition of zero elements
         * has no effect, so it is permissible for this collection simply to do nothing and not to throw
         * an exception. However, it is recommended that such cases throw an exception unconditionally,
         * as throwing only in certain cases can lead to programming errors.
         * </p>
         * <p>
         * An <i>unmodifiable collection</i> is a collection, all of whose mutator methods (as defined above)
         * are specified to throw @c UnsupportedOperationException. Such a collection thus cannot be
         * modified by calling any methods on it. For a collection to be properly unmodifiable, any view collections
         * derived from it must also be unmodifiable. For example, if a List is unmodifiable, the List returned by
         * @c List::subList is also unmodifiable.
         * </p>
         * <p>
         * An unmodifiable collection is not necessarily immutable. If the contained elements are mutable,
         * the entire collection is clearly mutable, even though it might be unmodifiable. For example,
         * consider two unmodifiable lists containing mutable elements. The result of calling
         * @c list1.equals(list2) might differ from one call to the next if the elements had been mutated,
         * even though both lists are unmodifiable. However, if an unmodifiable collection contains all immutable
         * elements, it can be considered effectively immutable.
         * </p>
         * <h2>Unmodifiable View Collections</h2>
         * <p>
         * An <i>unmodifiable view collection</i> is a collection that is unmodifiable and that is also a view onto
         * a backing collection. Its mutator methods throw @c UnsupportedOperationException, as described above, while
         * reading and querying methods are delegated to the backing collection. The effect is to provide read-only
         * access to the backing collection. This is useful for a component to provide users with read access to
         * an internal collection, while preventing them from modifying such collections unexpectedly.
         * </p>
         * <p>
         * Note that changes to the backing collection might still be possible, and if they occur, they are
         * visible through the unmodifiable view. Thus, an unmodifiable view collection is not necessarily
         * immutable. However, if the backing collection of an unmodifiable view is effectively immutable,
         * or if the only reference to the backing collection is through an unmodifiable view, the view can
         * be considered effectively immutable.
         * </p>
         * <h2>Serializability of Collections</h2>
         * <p>
         * Serializability of collections is optional. As such, none of the collections interfaces are declared
         * to implement the @c io::Serializable interface. However, serializability is regarded as being generally
         * useful, so most collection implementations are serializable.
         * </p>
         * <p>
         * The collection implementations that are classes (such as @c ArrayList or @c HashMap) are declared to
         * implement the @c Serializable interface if they are in fact serializable. Some collections implementations
         * are not public classes, such as the unmodifiable collections. In such cases, the serializability of such
         * collections is described in the specification of the method that creates them, or in some other suitable
         * place. In cases where the serializability of a collection is not specified, there is no guarantee about
         * the serializability of such collections. In particular, many view collections are
         * not serializable, even if the original collection is serializable.
         * </p>
         * <p>
         * A collection implementation that implements the @c Serializable interface cannot
         * be guaranteed to be serializable. The reason is that in general, collections
         * contain elements of other types, and it is not possible to determine statically
         * whether instances of some element type are actually serializable. For example, consider
         * a serializable @c Collection<E>, where @c E does not implement the @c Serializable interface.
         * The collection may be serializable, if it contains only elements of some serializable subtype
         * of @c E, or if it is empty. Collections are thus said to be <i>conditionally serializable,</i> as
         * the serializability of the collection as a whole depends on whether the collection itself is serializable
         * and on whether all contained elements are also serializable.
         * </p>
         * <p>
         * An additional case occurs with instances of @c SortedSet and @c SortedMap.
         * These collections can be created with a @c Comparator that imposes an ordering on
         * the set elements or map keys. Such a collection is serializable only if the provided
         * @c Comparator is also serializable.
         * </p>
         *
         * @note The default method implementations (inherited or otherwise) do not apply any
         *       synchronization protocol.  If a @c Collection implementation has a
         *       specific synchronization protocol, then it must override default
         *       implementations to apply that protocol.
         *
         * @tparam E the type of elements in this collection
         *
         * @see Set
         * @see List
         * @see Map
         * @see SortedSet
         * @see SortedMap
         * @see HashSet
         * @see TreeSet
         * @see ArrayList
         * @see LinkedList
         * @see Vector
         */
        template<class E>
        class Collection : public virtual Iterable<E> {
            CORE_CHECK_SLIMMED_TYPE(E);
            CORE_ALIAS(UNSAFE, Object::UNSAFE);

        public:
            // Query Operations

            /**
             * Returns the number of elements in this collection.  If this collection
             * contains more than @c Integer::MAX_VALUE elements, returns @c Integer::MAX_VALUE.
             *
             * @return the number of elements in this collection.
             */
            virtual gint size() const = 0;

            /**
             * Return @c true if this collection contains no elements.
             *
             * @note By default, return <pre> size() == 0 </pre>.
             * @return @c true if this collection contains no elements.
             */
            virtual gbool isEmpty() const { return size() == 0; }

            /**
             * Returns @c true if this collection contains the specified element.
             * More formally, returns @c true if and only if this collection
             * contains at least one element @c e such that @code val.equals(e) @endcode.
             *
             * @note By default, This implementation iterates over the elements in the collection,
             *       checking each element in turn for equality with the specified element.
             * @param val element whose presence in this collection is to be tested.
             * @return @c true if this collection contains the specified element.
             */
            virtual gbool contains(E const &val) const {
                try {
                    Iterator2<E> &it = iterator();
                    gbool found = false;

                    while (it.hasNext()) {
                        if (val.equals(it.next())) {
                            found = true;
                            break;
                        }
                    }

                    UNSAFE::deleteInstance(it);
                    return found;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator over the elements in this collection.  There are no
             * guarantees concerning the order in which the elements are returned.
             *
             * @return an @c Iterator over the elements in this collection.
             */
            Iterator<E> &iterator() override = 0;

            /**
             * Returns an iterator over the elements in this collection.  There are no
             * guarantees concerning the order in which the elements are returned.
             *
             * @return an @c Iterator over the elements in this collection.
             */
            Iterator2<E> &iterator() const override = 0;

            /**
             * Returns an array containing all the elements in this collection.
             * If this collection makes any guarantees as to what order its elements
             * are returned by its iterator, this method must return the elements in
             * the same order.
             *
             * @note By default, This implementation returns an array containing all the elements
             *       returned by this collection's iterator, in the same order, stored in
             *       consecutive elements of the array, starting with index @c 0.
             *       The length of the returned array is equal to the number of elements
             *       returned by the iterator, even if the size of this collection changes
             *       during iteration, as might happen if the collection permits
             *       concurrent modification during iteration.  The @c Collection::size method is
             *       called only as an optimization hint; the correct result is returned
             *       even if the iterator returns a different number of elements.
             * @return an array containing all the elements in this collection.
             */
            virtual Array<E> toArray() const {
                try {
                    // Estimate size of array; be prepared to see more or fewer elements
                    CORE_IGNORE_DEPRECATIONS(Array<E> a = Array<E>(size());)
                    Iterator2<E> &it = iterator();
                    gint len = a.length();
                    gint i = 0;
                    for (; i < len; ++i) {
                        if (!it.hasNext()) {
                            // fewer elements than expected
                            return Arrays::copyOf(a, i);
                        }
                        a.set(i, it.next());
                    }
                    // Finish toArray
                    while (it.hasNext()) {
                        if (i == len) {
                            len = Arrays::newLength(len, 1, (len >> 1) + 1);
                            a = Arrays::copyOf(a, len);
                        }
                        a.set(i++, it.next());
                    }
                    // Trim if overallocated
                    if (i == len)
                        return a;

                    return Arrays::copyOf(a, i);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Ensures that this collection contains the specified element (optional
             * operation).  Returns @c true if this collection changed as a
             * result of the call.  (Returns @c false if this collection does
             * not permit duplicates and already contains the specified element.)
             * <p>
             * Collections that support this operation may place limitations on what
             * elements may be added to this collection.
             * Collection classes should clearly specify in their documentation any
             * restrictions on what elements may be added.
             * </p>
             * <p>
             * If a collection refuses to add a particular element for any reason
             * other than that it already contains the element, it <i>must</i> throw
             * an exception (rather than returning @c false).  This preserves
             * the invariant that a collection always contains the specified element
             * after this call returns.
             * </p>
             *
             * @note By default, This implementation always throws an @c UnsupportedOperationException.
             *
             * @param val element whose presence in this collection is to be ensured.
             * @return @c true if this collection changed as a result of the call.
             * @throws UnsupportedOperationException  if the @c add operation is not
             *         supported by this collection
             * @throws IllegalArgumentException if some property of the element prevents
             *         it from being added to this collection
             * @throws IllegalStateException if the element cannot be added at this time
             *         due to insertion restrictions
             */
            virtual gbool add(E const &val) {
                UnsupportedOperationException().throws($ftrace());
            }

            /**
             * Removes a single instance of the specified element from this
             * collection, if it is present (optional operation).  More formally,
             * removes an element @c e such that @c Objects.equals(o, e), if
             * this collection contains one or more such elements.  Returns
             * @c true if this collection contained the specified element (or
             * equivalently, if this collection changed as a result of the call).
             *
             * @note By default, This implementation iterates over the collection looking for the
             *       specified element.  If it finds the element, it removes the element
             *       from the collection using the iterator's remove method.
             *       <p>
             *       Note that this implementation throws an @c UnsupportedOperationException if the iterator
             *       returned by this collection's iterator method does not implement the @c Iterator::remove
             *       method and this collection contains the specified object.
             *       </p>
             *
             * @param val element to be removed from this collection, if present.
             * @return @c true if an element was removed as a result of this call.
             * @throws UnsupportedOperationException if the @c remove operation is not
             *          supported by this collection
             */
            virtual gbool remove(E const &val) {
                try {
                    Iterator<E> &it = iterator();
                    gbool modified = false;

                    while (it.hasNext()) {
                        if (val.equals(it.next())) {
                            it.remove();
                            modified = true;
                            break;
                        }
                    }

                    UNSAFE::deleteInstance(it);
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // Bulk Operations

            /**
             * Returns @c true if this collection contains all the elements
             * in the specified collection.
             *
             * @note By default, This implementation iterates over the specified collection,
             *       checking each element returned by the iterator in turn to see if it's
             *       contained in this collection.  If all elements are so contained @c true
             *       is returned, otherwise @c false.
             *
             * @param c collection to be checked for containment in this collection.
             * @return @c true if this collection contains all the elements.
             *         in the specified collection
             */
            virtual gbool containsAll(Collection const &c) const {
                try {
                    for (E const &val: c) {
                        if (!contains(val))
                            return false;
                    }

                    return true;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Adds all the elements in the specified collection to this collection
             * (optional operation).  The behavior of this operation is undefined if
             * the specified collection is modified while the operation is in progress.
             * (This implies that the behavior of this call is undefined if the
             * specified collection is this collection, and this collection is
             * nonempty.) If the specified collection has a defined encounter order,
             * processing of its elements generally occurs in that order.
             *
             * @note By default, This implementation iterates over the specified collection, and adds
             *       each object returned by the iterator to this collection, in turn.
             *       <p>
             *       Note that this implementation will throw an @c UnsupportedOperationException unless @c add is
             *       overridden (assuming the specified collection is non-empty).
             *       </p>
             *
             * @param c collection containing elements to be added to this collection.
             * @return @c true if this collection changed as a result of the call.
             * @throws UnsupportedOperationException if the @c addAll operation is not
             *         supported by this collection.
             * @throws IllegalArgumentException if some property of an element of the
             *         specified collection prevents it from being added to this
             *         collection.
             * @throws IllegalStateException if not all the elements can be added at
             *         this time due to insertion restrictions.
             */
            virtual gbool addAll(Collection const &c) {
                try {
                    gbool modified = false;

                    for (E const &val: c)
                        modified |= add(val);

                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all of this collection's elements that are also contained in the
             * specified collection (optional operation).  After this call returns,
             * this collection will contain no elements in common with the specified
             * collection.
             *
             * @note By default, This implementation iterates over this collection, checking each
             *       element returned by the iterator in turn to see if it's contained
             *       in the specified collection.  If it's so contained, it's removed from
             *       this collection with the iterator's @c Iterator::remove method.
             *       <p>
             *       Note that this implementation will throw an @c UnsupportedOperationException if
             *       the iterator returned by the @c Collection::iterator method does not implement
             *       the @c Iterator::remove method and this collection contains one or more elements
             *       in common with the specified collection.
             *       </p>
             *
             * @param c collection containing elements to be removed from this collection
             * @return @c true if this collection changed as a result of the call
             * @throws UnsupportedOperationException if the @c Collection::removeAll method
             *         is not supported by this collection.
             */
            virtual gbool removeAll(Collection const &c) {
                try {
                    Iterator<E> &it = iterator();
                    gbool modified = false;

                    while (it.hasNext()) {
                        if (c.contains(it.next())) {
                            it.remove();
                            modified = true;
                        }
                    }

                    UNSAFE::deleteInstance(it);
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all the elements of this collection that satisfy the given
             * predicate.  Errors or runtime exceptions thrown during iteration or by
             * the predicate are relayed to the caller.
             *
             * @note The default implementation traverses all elements of the collection
             * using its @c Collection::iterator.  Each matching element is removed using
             * @c Iterator::remove().  If the collection's iterator does not support removal
             * then an @c UnsupportedOperationException will be thrown on the first matching
             * element.
             *
             * @param filter a predicate which returns @c true for elements to be removed
             * @return @c true if any elements were removed
             * @throws UnsupportedOperationException if elements cannot be removed
             *         from this collection.  Implementations may throw this exception if a
             *         matching element cannot be removed or if, in general, removal is not
             *         supported.
             */
            virtual gbool removeIf(function::Predicate<E> const &filter) {
                try {
                    Iterator<E> &it = iterator();
                    gbool modified = false;

                    while (it.hasNext()) {
                        if (filter.test(it.next())) {
                            it.remove();
                            modified = true;
                        }
                    }

                    UNSAFE::deleteInstance(it);
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Retains only the elements in this collection that are contained in the
             * specified collection (optional operation).  In other words, removes from
             * this collection all of its elements that are not contained in the
             * specified collection.
             *
             * @note By default, This implementation iterates over this collection, checking each
             *       element returned by the iterator in turn to see if it's contained
             *       in the specified collection.  If it's not so contained, it's removed
             *       from this collection with the iterator's @c remove method.
             *       <p>
             *       Note that this implementation will throw an @c UnsupportedOperationException if
             *       the iterator returned by the @c Collection::iterator method does not implement
             *       the @c Iterator::remove method and this collection contains one or more elements
             *       not present in the specified collection.
             *       </p>
             *
             * @param c collection containing elements to be retained in this collection
             * @return @c true if this collection changed as a result of the call.
             * @throws UnsupportedOperationException if the @c Collection::retainAll operation
             *         is not supported by this collection.
             */
            virtual gbool retainAll(Collection const &c) {
                try {
                    Iterator<E> &it = iterator();
                    gbool modified = false;

                    while (it.hasNext()) {
                        if (!c.contains(it.next())) {
                            it.remove();
                            modified = true;
                        }
                    }

                    UNSAFE::deleteInstance(it);
                    return modified;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Removes all the elements from this collection (optional operation).
             * The collection will be empty after this method returns.
             *
             * @note By default, This implementation iterates over this collection, removing each
             *       element using the @c Iterator::remove operation. Most implementations will
             *       probably choose to override this method for efficiency.
             *       <p>
             *       Note that this implementation will throw an @c UnsupportedOperationException
             *       if the iterator returned by this collection's @c Collection::iterator method
             *       does not implement the @c Iterator::remove method and this collection is non-empty.
             *       </p>
             *
             * @throws UnsupportedOperationException if the @c clear operation
             *         is not supported by this collection
             */
            virtual void clear() {
                try {
                    Iterator<E> &it = iterator();

                    while (it.hasNext()) it.remove();

                    UNSAFE::deleteInstance(it);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            // Comparison and Hashing

            /**
             * Compares the specified object with this collection for equality.
             * <p>
             * While the @c Collection interface adds no stipulations to the
             * general contract for the @c Object::equals, programmers who
             * implement the @c Collection interface "directly" (in other words,
             * create a class that is a @c Collection but is not a @c Set
             * or a @c List) must exercise care if they choose to override the
             * @c Object::equals.  It is not necessary to do so, and the simplest
             * course of action is to rely on @c Object's implementation, but
             * the implementor may wish to implement a "value comparison" in place of
             * the default "reference comparison."  (The @c List and @c Set interfaces
             * mandate such value comparisons.)
             * </p>
             * <p>
             * The general contract for the @c Object::equals method states that
             * equals must be symmetric (in other words, @c a.equals(b) if and
             * only if @c b.equals(a)).  The contracts for @c List::equals
             * and @c Set::equals state that lists are only equal to other lists,
             * and sets to other sets.  Thus, a custom @c Collection::equals method for a
             * collection class that implements neither the @c List nor
             * @c Set interface must return @c false when this collection
             * is compared to any list or set.  (By the same logic, it is not possible
             * to write a class that correctly implements both the @c Set and
             * @c List interfaces.)
             * </p>
             *
             * @param o object to be compared for equality with this collection
             * @return @c true if the specified object is equal to this
             * collection
             */
            gbool equals(Object const &o) const override { return Object::equals(o); }

            /**
             * Returns the hash code value for this collection.  While the
             * @c Collection interface adds no stipulations to the general
             * contract for the @c Object::hash method, programmers should
             * take note that any class that overrides the @c Object::equals
             * method must also override the @c Object::hash method in order
             * to satisfy the general contract for the @c Object::hash method.
             * In particular, @c c1.equals(c2) implies that <pre>c1.hash() == c2.hash()</pre>.
             *
             * @return the hash code value for this collection
             */
            gint hash() const override { return Object::hash(); }

            /**
             * Creates a @b Spliterator over the elements in this collection.
             * <p>
             * Implementations should document characteristic values reported by the
             * spliterator.  Such characteristic values are not required to be reported
             * if the spliterator reports @b Spliterator::SIZED and this collection
             * contains no elements.
             * </p>
             * <p>
             * The default implementation should be overridden by subclasses that
             * can return a more efficient spliterator.  In order to
             * preserve expected laziness behavior for the @b stream() and
             * @b parallelStream() methods, spliterators should either have the
             * characteristic of @c IMMUTABLE or @c CONCURRENT, or be <em>late-binding</em>.
             * If none of these is practical, the overriding class should describe the
             * spliterator's documented policy of binding and structural interference,
             * and should override the @b stream() and @b parallelStream()
             * methods to create streams using a @c Supplier of the spliterator,
             * as in:
             * @code
             *     Stream<E> s = StreamSupport::stream(() -> spliterator(), spliteratorCharacteristics)
             * @endcode
             * <p>
             * These requirements ensure that streams produced by the
             * @b stream() and @b parallelStream() methods will reflect the
             * contents of the collection as of initiation of the terminal stream
             * operation.
             * </p>
             * @details
             * The default implementation creates a <em>late-binding</em> spliterator
             * from the collection's @c Iterator.  The spliterator inherits the
             * <em>fail-fast</em> properties of the collection's iterator.
             * <p>
             * The created @c Spliterator reports @b Spliterator::SIZED.
             * </p>
             * @note
             * The created @c Spliterator additionally reports
             * @b Spliterator::SUBSIZED.
             *
             * <p>
             * If a spliterator covers no elements then the reporting of additional
             * characteristic values, beyond that of @c SIZED and @c SUBSIZED,
             * does not aid clients to control, specialize or simplify computation.
             * However, this does enable shared use of an immutable and empty
             * spliterator instance (see @b Spliterators::emptySpliterator()) for
             * empty collections, and enables clients to determine if such a spliterator
             * covers no elements.
             * </p>
             * @return a @c Spliterator over the elements in this collection
             */
            Spliterator<E> &spliterator() override {
                CORE_ALIAS(ITERATOR, typename Class<Iterator<E>>::Pointer);
                static CORE_FAST gint BATCH_UNIT = 1 << 10;
                static CORE_FAST gint MAX_BATCH = 1 << 25;

                class Spliterator final : public util::Spliterator<E> {
                    Collection &c;
                    ITERATOR mutable it = null;
                    gint mutable estSize = 0;
                    gint mutable batch = 0;

                public:
                    CORE_EXPLICIT Spliterator(Collection &c): c(c) {
                    }

                    util::Spliterator<E> & trySplit() override {
                        /*
                         * Split into arrays of arithmetically increasing batch
                         * sizes.  This will only improve parallel performance if
                         * per-element Consumer actions are more costly than
                         * transferring them into an array.  The use of an
                         * arithmetic progression in split sizes provides overhead
                         * vs parallelism bounds that do not particularly favor or
                         * penalize cases of lightweight vs heavyweight element
                         * operations, across combinations of #elements vs #cores,
                         * whether either are known.  We generate
                         * O(sqrt(#elements)) splits, allowing O(sqrt(#cores))
                         * potential speedup.
                         */
                        glong s = 0;
                        if (it == null) {
                            try {
                                it = &c.iterator();
                                s = estSize = c.size();
                            } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        } else
                            s = estSize;

                        Iterator<E> &i = *it;
                        try {
                            if (s > 1 && i.hasNext()) {
                                gint n = batch + BATCH_UNIT;
                                if (n > s)
                                    n = (gint) s;
                                if (n > MAX_BATCH)
                                    n = MAX_BATCH;
                                CORE_IGNORE_DEPRECATIONS(Array<E> a = Array<E>(n);)
                                gint j = 0;
                                do { Arrays::fastSet(a, j, i.next()); } while (++j < n && i.hasNext());
                                batch = j;
                                estSize -= j;
                                return Spliterators::spliterator<E>(a, 0, j, 0);
                            }
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        return *this;
                    }

                    const util::Spliterator<E> & trySplit() const override {
                        /*
                         * Split into arrays of arithmetically increasing batch
                         * sizes.  This will only improve parallel performance if
                         * per-element Consumer actions are more costly than
                         * transferring them into an array.  The use of an
                         * arithmetic progression in split sizes provides overhead
                         * vs parallelism bounds that do not particularly favor or
                         * penalize cases of lightweight vs heavyweight element
                         * operations, across combinations of #elements vs #cores,
                         * whether either are known.  We generate
                         * O(sqrt(#elements)) splits, allowing O(sqrt(#cores))
                         * potential speedup.
                         */
                        glong s = 0;
                        if (it == null) {
                            try {
                                it = &c.iterator();
                                s = estSize = c.size();
                            } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        } else
                            s = estSize;

                        Iterator<E> &i = *it;
                        try {
                            if (s > 1 && i.hasNext()) {
                                gint n = batch + BATCH_UNIT;
                                if (n > s)
                                    n = (gint) s;
                                if (n > MAX_BATCH)
                                    n = MAX_BATCH;
                                CORE_IGNORE_DEPRECATIONS(Array<E> a = Array<E>(n);)
                                gint j = 0;
                                do { Arrays::fastSet(a, j, i.next()); } while (++j < n && i.hasNext());
                                batch = j;
                                estSize -= j;
                                return Spliterators::spliterator<E>(a, 0, j, 0);
                            }
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        return *this;
                    }

                    gbool tryAdvance(const function::Consumer<E &> &action) override {
                        if (it == null) {
                            try {
                                it = &c.iterator();
                                estSize = c.size();
                            } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        }
                        try {
                            if (it->hasNext()) {
                                action.accept(it->next());
                                return true;
                            }
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        return false;
                    }

                    void forEach(const function::Consumer<E &> &action) override {
                        try {
                            if (it == null) {
                                it = &c.iterator();
                                estSize = c.size();
                            }
                            it->forEach(action);
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    glong estimateSize() const override {
                        if (it == null) {
                            try {
                                it = &c.iterator();
                                estSize = c.size();
                            } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        }
                        return estSize;
                    }

                    gint characteristics() const override { return 0; }
                };

                try {
                    return UNSAFE::newInstance<Spliterator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Creates a @b Spliterator over the elements in this collection.
             * <p>
             * Implementations should document characteristic values reported by the
             * spliterator.  Such characteristic values are not required to be reported
             * if the spliterator reports @b Spliterator::SIZED and this collection
             * contains no elements.
             * </p>
             * <p>
             * The default implementation should be overridden by subclasses that
             * can return a more efficient spliterator.  In order to
             * preserve expected laziness behavior for the @b stream() and
             * @b parallelStream() methods, spliterators should either have the
             * characteristic of @c IMMUTABLE or @c CONCURRENT, or be <em>late-binding</em>.
             * If none of these is practical, the overriding class should describe the
             * spliterator's documented policy of binding and structural interference,
             * and should override the @b stream() and @b parallelStream()
             * methods to create streams using a @c Supplier of the spliterator,
             * as in:
             * @code
             *     Stream<E> s = StreamSupport::stream(() -> spliterator(), spliteratorCharacteristics)
             * @endcode
             * <p>
             * These requirements ensure that streams produced by the
             * @b stream() and @b parallelStream() methods will reflect the
             * contents of the collection as of initiation of the terminal stream
             * operation.
             * </p>
             * @details
             * The default implementation creates a <em>late-binding</em> spliterator
             * from the collection's @c Iterator.  The spliterator inherits the
             * <em>fail-fast</em> properties of the collection's iterator.
             * <p>
             * The created @c Spliterator reports @b Spliterator::SIZED.
             * </p>
             * @note
             * The created @c Spliterator additionally reports
             * @b Spliterator::SUBSIZED.
             *
             * <p>
             * If a spliterator covers no elements then the reporting of additional
             * characteristic values, beyond that of @c SIZED and @c SUBSIZED,
             * does not aid clients to control, specialize or simplify computation.
             * However, this does enable shared use of an immutable and empty
             * spliterator instance (see @b Spliterators::emptySpliterator()) for
             * empty collections, and enables clients to determine if such a spliterator
             * covers no elements.
             * </p>
             * @return a @c Spliterator over the elements in this collection
             */
            Spliterator2<E> &spliterator() const override {
                CORE_ALIAS(ITERATOR, typename Class<Iterator2<E>>::Pointer);
                static CORE_FAST gint BATCH_UNIT = 1 << 10;
                static CORE_FAST gint MAX_BATCH = 1 << 25;

                class Spliterator final : public Spliterator2<E> {
                    Collection const &c;
                    ITERATOR mutable it = null;
                    gint mutable estSize = 0;
                    gint mutable batch = 0;

                public:
                    CORE_EXPLICIT Spliterator(Collection const &c): c(c) {
                    }

                    Spliterator2<E> & trySplit() override {
                        /*
                         * Split into arrays of arithmetically increasing batch
                         * sizes.  This will only improve parallel performance if
                         * per-element Consumer actions are more costly than
                         * transferring them into an array.  The use of an
                         * arithmetic progression in split sizes provides overhead
                         * vs parallelism bounds that do not particularly favor or
                         * penalize cases of lightweight vs heavyweight element
                         * operations, across combinations of #elements vs #cores,
                         * whether either are known.  We generate
                         * O(sqrt(#elements)) splits, allowing O(sqrt(#cores))
                         * potential speedup.
                         */
                        glong s = 0;
                        if (it == null) {
                            try {
                                it = &c.iterator();
                                s = estSize = c.size();
                            } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        } else
                            s = estSize;

                        Iterator2<E> &i = *it;
                        try {
                            if (s > 1 && i.hasNext()) {
                                gint n = batch + BATCH_UNIT;
                                if (n > s)
                                    n = (gint) s;
                                if (n > MAX_BATCH)
                                    n = MAX_BATCH;
                                CORE_IGNORE_DEPRECATIONS(Array<E> a = Array<E>(n);)
                                gint j = 0;
                                do { Arrays::fastSet(a, j, (E &) i.next()); } while (++j < n && i.hasNext());
                                batch = j;
                                estSize -= j;
                                return Spliterators::spliterator2<E>(a, 0, j, 0);
                            }
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        return *this;
                    }

                    const Spliterator2<E> & trySplit() const override {
                        /*
                         * Split into arrays of arithmetically increasing batch
                         * sizes.  This will only improve parallel performance if
                         * per-element Consumer actions are more costly than
                         * transferring them into an array.  The use of an
                         * arithmetic progression in split sizes provides overhead
                         * vs parallelism bounds that do not particularly favor or
                         * penalize cases of lightweight vs heavyweight element
                         * operations, across combinations of #elements vs #cores,
                         * whether either are known.  We generate
                         * O(sqrt(#elements)) splits, allowing O(sqrt(#cores))
                         * potential speedup.
                         */
                        glong s = 0;
                        if (it == null) {
                            try {
                                it = &c.iterator();
                                s = estSize = c.size();
                            } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        } else
                            s = estSize;

                        Iterator2<E> &i = *it;
                        try {
                            if (s > 1 && i.hasNext()) {
                                gint n = batch + BATCH_UNIT;
                                if (n > s)
                                    n = (gint) s;
                                if (n > MAX_BATCH)
                                    n = MAX_BATCH;
                                CORE_IGNORE_DEPRECATIONS(Array<E> a = Array<E>(n);)
                                gint j = 0;
                                do { Arrays::fastSet(a, j, (E &) i.next()); } while (++j < n && i.hasNext());
                                batch = j;
                                estSize -= j;
                                return Spliterators::spliterator2<E>(a, 0, j, 0);
                            }
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        return *this;
                    }

                    gbool tryAdvance(const function::Consumer<E> &action) override {
                        if (it == null) {
                            try {
                                it = &c.iterator();
                                estSize = c.size();
                            } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        }
                        try {
                            if (it->hasNext()) {
                                action.accept(it->next());
                                return true;
                            }
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        return false;
                    }

                    void forEach(const function::Consumer<E> &action) override {
                        try {
                            if (it == null) {
                                it = &c.iterator();
                                estSize = c.size();
                            }
                            it->forEach(action);
                        } catch (Throwable const &ex) { ex.throws($ftrace()); }
                    }

                    glong estimateSize() const override {
                        if (it == null) {
                            try {
                                it = &c.iterator();
                                estSize = c.size();
                            } catch (Throwable const &ex) { ex.throws($ftrace()); }
                        }
                        return estSize;
                    }

                    gint characteristics() const override { return 0; }
                };

                try {
                    return UNSAFE::newInstance<Spliterator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a sequential @c Stream with this collection as its source.
             *
             * <p>
             * This method should be overridden when the @b spliterator()
             * method cannot return a spliterator that is {@code IMMUTABLE},
             * @c CONCURRENT, or <em>late-binding</em>. (See @b spliterator()
             * for details.)
             * </p>
             * @note
             * The default implementation creates a sequential @c Stream from the
             * collection's @c Spliterator.
             *
             * @return a sequential @c Stream over the elements in this collection
             */
            // stream::Stream<E> &stream() {
            //     return stream::Streams::stream(spliterator(), false);
            // }
            //

            /**
             * Returns a sequential @c Stream with this collection as its source.
             *
             * <p>
             * This method should be overridden when the @b spliterator()
             * method cannot return a spliterator that is @c IMMUTABLE,
             * @c CONCURRENT, or <em>late-binding</em>. (See @b spliterator()
             * for details.)
             * </p>
             * @note
             * The default implementation creates a sequential @c Stream from the
             * collection's @c Spliterator.
             *
             * @return a sequential @c Stream over the elements in this collection
             */
            // stream::Stream2<E> &stream() {
            //     return stream::Streams::stream(spliterator(), false);
            // }
            //

            // String conversion

            /**
             * Returns a string representation of this collection. The string representation consists
             * of a list of the collection's elements in the order they are returned by its iterator,
             * enclosed in square brackets (@c "[]").  Adjacent elements are separated by the
             * characters @c ", " (comma and space).  Elements are converted to strings as
             * by @c String::valueOf(Object).
             *
             * @return a string representation of this collection.
             */
            String toString() const override {
                try {
                    Iterator2<E> &it = iterator();
                    if (!it.hasNext()) {
                        UNSAFE::deleteInstance(it);
                        return "[]"_S;
                    }

                    XString str = XString();
                    str.append(u'[');
                    for (;;) {
                        Object const &val = it.next();
                        if (this == &val)
                            // Replace by "{..}"
                            str.append(u'{').append(u'.').append(u'.').append(u'}');
                        else
                            str.append(val);
                        if (!it.hasNext()) {
                            UNSAFE::deleteInstance(it);
                            return str.append(u']').toString();
                        }
                        str.append(u',').append(u' ');
                    }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator that has no elements.  More precisely,
             *
             * - @c hasNext always returns @c false .
             * - @c next always throws @c NoSuchElementException .
             * - @c remove always throws @c IllegalStateException .
             *
             * <p>
             * Implementations of this method are permitted, but not
             * required, to return the same object from multiple invocations.
             * </p>
             *
             * @tparam T type of elements, if there were any, in the iterator
             * @return an empty iterator
             */
            static Iterator<E> &emptyIterator() {
                class Itr final : public Iterator<E> {
                public:
                    gbool hasNext() const override { return false; }

                    E & next() override { NoSuchElementException().throws($ftrace()); }

                    void remove() override { UnsupportedOperationException().throws($ftrace()); }

                    void forEach(const function::Consumer<E &> &action) override {
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<Itr>::hasInstance(o);
                    }
                };

                try { return UNSAFE::newInstance<Itr>(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns an iterator that has no elements.  More precisely,
             *
             * - @c hasNext always returns @c false .
             * - @c next always throws @c NoSuchElementException .
             * - @c remove always throws @c IllegalStateException .
             *
             *
             * <p>
             * Implementations of this method are permitted, but not
             * required, to return the same object from multiple invocations.
             * </p>
             *
             * @tparam T type of elements, if there were any, in the iterator
             * @return an empty iterator
             */
            static Iterator2<E> &emptyIterator2() {
                class Itr final : public Iterator2<E> {
                public:
                    gbool hasNext() const override { return false; }

                    E const & next() override { NoSuchElementException().throws($ftrace()); }

                    void remove() override { UnsupportedOperationException().throws($ftrace()); }

                    void forEach(const function::Consumer<E> &action) override {
                    }

                    gbool equals(const Object &o) const override {
                        return this == &o || Class<Itr>::hasInstance(o);
                    }
                };

                try { return UNSAFE::newInstance<Itr>(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }
        };
    } // util
} // core

#endif //CORE24_COLLECTION_H
