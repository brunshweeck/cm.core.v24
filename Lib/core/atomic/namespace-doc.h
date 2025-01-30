//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {
    /**
     * A small toolkit of classes that support lock-free thread-safe
     * programming on single variables.  Instances of Atomic classes
     * maintain values that are accessed and updated using methods
     * otherwise available for fields using associated atomic operations.
     *
     * <p>
     * Instances of classes
     * @b concurrent::atomic::AtomicBoolean,
     * @b concurrent::atomic::AtomicInteger,
     * @b concurrent::atomic::AtomicLong, and
     * @b concurrent::atomic::AtomicReference
     * each provide access and updates to a single variable of the
     * corresponding type.  Each class also provides appropriate utility
     * methods for that type.  For example, classes @c AtomicLong and
     * @c AtomicInteger provide atomic increment methods.  One
     * application is to generate sequence numbers, as in:
     *
     * @code
     * class Sequencer {
     *   AtomicLong sequenceNumber = AtomicLong(17);
     *   public:
     *      glong next() {
     *          return sequenceNumber.getAndIncrement();
     *      }
     *  };
     * @endcode
     *
     * <p>
     * Arbitrary transformations of the contained value are provided both
     * by low-level read-modify-write operations such as @c compareAndSet
     * and by higher-level methods such as @c getAndUpdate.
     * </p>
     * <p>
     * These classes are not general purpose replacements for
     * @c core::Integer and related classes.  They do <em>not</em>
     * define methods such as @c equals, @c hash and @c compareTo.
     * Because atomic variables are expected to be mutated,
     * they are poor choices for hash table keys.
     * </p>
     * <p>
     * The
     * @b concurrent::atomic::AtomicIntegerArray,
     * @b concurrent::atomic::AtomicLongArray, and
     * @b concurrent::atomic::AtomicReferenceArray classes
     * further extend atomic operation support to arrays of these types.
     * These classes are also notable in providing @c volatile access
     * semantics for their array elements.
     * </p>
     * <p>
     * The @b concurrent::atomic::AtomicMarkableReference
     * class associates a single boolean with a reference.  For example, this
     * bit might be used inside a data structure to mean that the object
     * being referenced has logically been deleted.
     *
     * The @b concurrent::atomic::AtomicStampedReference
     * class associates an integer value with a reference.  This may be
     * used for example, to represent version numbers corresponding to
     * series of updates.
     */
    namespace atomic {}
}

#endif //NAMESPACE_DOC_H
