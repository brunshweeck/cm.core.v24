//
// Created by bruns on 22/09/2024.
//

#ifndef CORE24_SEQUENCEDSET_H
#define CORE24_SEQUENCEDSET_H

#include <core/util/SequencedCollection.h>
#include <core/util/Set.h>

namespace core {
    namespace util {
        /**
         * A collection that is both a @b SequencedCollection and a @b Set. As such,
         * it can be thought of either as a @c Set that also has a well-defined
         * <em>encounter order</em>, or as a
         * @c SequencedCollection that also has unique elements.
         * <p>
         * This interface has the same requirements on the @c equals and @c hash
         * methods as defined by @b Set::equals and @b Set::hash.
         * Thus, a @c Set and a @c SequencedSet will compare equals if and only
         * if they have equal elements, irrespective of ordering.
         * </p>
         * <p>
         * @c SequencedSet defines the @b reversed method, which provides a
         * reverse-ordered @em view of this set. The only difference
         * from the @b SequencedCollection::reversed method is
         * that the return type of @c SequencedSet::reversed is @c SequencedSet.
         * </p>
         *
         * @tparam E the type of elements in this sequenced set
         */
        template<class E>
        class SequencedSet : public virtual SequencedCollection<E>, public virtual Set<E> {
        public:
            /**
             *
             *
             * @return a reverse-ordered view of this collection, as a @c SequencedSet
             */
            SequencedSet const &reversed() const override = 0;

            /**
             *
             *
             * @return a reverse-ordered view of this collection, as a @c SequencedSet
             */
            SequencedSet &reversed() override = 0;
        };
    } // util
} // core

#endif //CORE24_SEQUENCEDSET_H
