//
// Created by brunshweeck on 22 juin 2024.
//

#ifndef CORE24_COMPARATOR_H
#define CORE24_COMPARATOR_H

#include <core/function/ToDoubleFunction.h>
#include <core/function/ToIntFunction.h>
#include <core/function/ToLongFunction.h>
#include <core/lang/ClassCastException.h>
#include <core/lang/XString.h>

namespace core {
    namespace util {
        /**
         * A comparison function, which imposes a <i>total ordering</i> on some collection of objects.
         * Comparators can be passed to a sort method (such as @c Collections::sort or @c Arrays::sort)
         * to allow precise control over the sort order. Comparators can also be used to control the order
         * of certain data structures (such as sorted sets or sorted maps), or to provide an ordering for
         * collections of objects that don't have a natural ordering.
         * <p>
         * The ordering imposed by a comparator @c c on a set of elements @c S is said to be <i>consistent
         * with equals</i> if and only if @code c.compare(e1, e2)==0 @endcode has the same boolean value as
         * @c e1.equals(e2) for every @c e1 and @c e2 in @c S.
         * </p>
         * <p>
         * Caution should be exercised when using a comparator capable of imposing an ordering inconsistent
         * with equals to order a sorted set (or sorted map). Suppose a sorted set (or sorted map) with an
         * explicit comparator @c c is used with elements (or keys) drawn from a set @c S.  If the
         * ordering imposed by @c c on @c S is inconsistent with equals, the sorted set (or sorted map) will
         * behave "strangely."  In particular the sorted set (or sorted map) will violate the general contract
         * for set (or map), which is defined in terms of @c Object::equals.
         * </p>
         * <p>
         * For example, suppose one adds two elements @c a and @c b such that
         * @code (a.equals(b) && c.compare(a, b) != 0) @endcode to an empty @c TreeSet with comparator @c c.
         * The second @c add operation will return true (and the size of the tree set will increase) because @c a
         * and @c b are not equivalent from the tree set's perspective, even though this is contrary to the
         * specification of the @c Set::add method.
         * </p>
         * <p>
         * Note: It is generally a good idea for comparators to also implement @c io::Serializable, as they may
         * be used as ordering methods in serializable data structures (like @c TreeSet, @c TreeMap).  In
         * order for the data structure to serialize successfully, the comparator (if provided) must implement
         * @c io::Serializable.
         * </p>
         * <p>
         * For the mathematically inclined, the <i>relation</i> that defines the <i>imposed ordering</i> that a
         * given comparator @c c imposes on a given set of objects @c S is:
         * <i>(x, y)</i> such that @code c.compare(x, y) <= 0 @endcode. The <i>quotient</i> for this total order is:
         * <i>(x, y)</i> such that @code c.compare(x, y) == 0 @endcode.
         * It follows immediately from the contract for @c compare that the quotient is an <i>equivalence relation</i>
         * on @c S, and that the imposed ordering is a <i>total order</i> on @c S.  When we say that the ordering
         * imposed by @c c on @c S is <i>consistent with equals</i>, we mean that the quotient for the ordering is
         * the equivalence relation defined by the objects' @c Object::equals method(s):
         * <i> (x, y)</i> such that @c x.equals(y).
         * In other words, when the imposed ordering is consistent with equals, the equivalence classes defined by
         * the equivalence relation of the @c equals method and the equivalence classes defined by the quotient of
         * the @c compare method are the same.
         * </p>
         * <p>
         * Unlike @c Comparable, a comparator may optionally permit comparison of null arguments, while maintaining
         * the requirements for an equivalence relation.
         * </p>
         *
         * @tparam T the type of objects that may be compared by this comparator
         */
        template<class T>
        class Comparator : public virtual Object {
            CORE_FULL_CHECK_SLIMMED_TYPE(T);
            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ALIAS(UNSAFE, misc::Unsafe);

        public:
            /**
             * Compares its two arguments for order.  Returns a negative integer,
             * zero, or a positive integer as the first argument is less than, equal
             * to, or greater than the second.
             * <p>
             * The implementor must ensure that
             * @code Integer::signum(compare(x, y)) == -Integer::signum(compare(y, x)) @endcode
             * for all @c x and @c y.  (This implies that @code compare(x, y) @endcode must throw an exception
             * if and only if @code compare(y, x) @endcode throws an exception.)
             * </p>
             * <p>
             * The implementor must also ensure that the relation is transitive:
             * @code ((compare(x, y) > 0) && (compare(y, z)>0)) @endcode implies @code compare(x, z) > 0 @endcode.
             * </p>
             * <p>
             * Finally, the implementor must ensure that @code compare(x, y)==0 @endcode implies that
             * @code Integer::signum(compare(x, z))==signum(compare(y, z)) @endcode for all @c z.
             * </p>
             * @note It is generally the case, but <i>not</i> strictly required that
             *       @code (compare(x, y)==0) == (x.equals(y)) @endcode.  Generally speaking, any comparator that
             *       violates this condition should clearly indicate this fact.  The recommended language is <<Note:
             *       this comparator imposes orderings that are inconsistent with equals>>.
             *
             * @param t1 the first object to be compared.
             * @param t2 the second object to be compared.
             * @return a negative integer, zero, or a positive integer as the first argument is less than,
             *         equal to, or greater than the second.
             */
            virtual gint compare(T const &t1, T const &t2) const CORE_NOTHROW = 0;

            /**
             * Indicates whether some other object is &quot;equal to `this comparator.
             * This method must obey the general contract of @c Object::equals.  Additionally, this method can
             * return @c true <i>only</i> if the specified object is also a comparator, and it imposes the
             * same ordering as this comparator.  Thus, @c comp1.equals(comp2) implies that
             * @code
             *  Integer::signum(comp1.compare(t1, t2)) == Integer::signum(comp2.compare(t1, t2))
             * @endcode
             * for every object reference @c t1 and @c t2.
             * <p>
             * Note that it is <i>always</i> safe <i>not</i> to override @c Object.equals(Object).  However,
             * overriding this method may, in some cases, improve performance by allowing programs to determine
             * that two distinct comparators impose the same order.
             * </p>
             * @param o the reference object with which to compare.
             * @return @c true only if the specified object is also
             *          a comparator, and it imposes the same ordering as this
             *          comparator.
             */
            gbool equals(const Object &o) const override {
                return Object::equals(o);
            }

            /**
             * Returns a comparator that imposes the reverse ordering of this
             * comparator.
             *
             * @return a comparator that imposes the reverse ordering of this
             *         comparator.
             */
            virtual Comparator &reversed() const CORE_NOTHROW {
                if (*this == naturalOrder())
                    return reverseOrder();
                if (*this == reverseOrder())
                    return naturalOrder();
                if (Class<ReversedOrder>::hasInstance(*this))
                    return CORE_CAST(Comparator &, CORE_XCAST(ReversedOrder const, *this).comparator);
                return UNSAFE::newInstance<ReversedOrder>(*this);
            }

            /**
             * Returns a lexicographic-order comparator with another comparator.
             * If this @c Comparator considers two elements equal, i.e.
             * @code Comparator::compare(a, b) == 0 @endcode, @c other is used to
             * determine the order.
             * <p>
             * For example, to sort a collection of @c String based on the length
             * and then case-insensitive natural ordering, the comparator can be composed
             * using following code,
             * @code
             *     auto &cmp = Comparator<String>.comparingInt(&String::length)
             *                  .thenComparing(String::CASE_INSENSITIVE_ORDER);
             * @endcode
             * </p>
             * @param  other the other comparator to be used when this comparator
             *         compares two objects that are equal.
             * @return a lexicographic-order comparator composed of this and then the
             *         other comparator
             */
            Comparator &thenComparing(Comparator const &other) const {
                class DualComparator final : public Comparator {
                    Comparator const &c1;
                    Comparator const &c2;

                public:
                    CORE_EXPLICIT DualComparator(Comparator const &c1, Comparator const &c2): c1(c1), c2(c2) {
                    }

                    gint compare(const T &t1, const T &t2) const CORE_NOTHROW override {
                        gint r = c1.compare(t1, t2);
                        return (r != 0) ? r : c2.compare(t1, t2);
                    }
                };

                try {
                    Comparator &c1 = UNSAFE::copyInstance(*this); // For Shared operation
                    Comparator &c2 = UNSAFE::copyInstance(other); // For Shared operation
                    return UNSAFE::newInstance<DualComparator>(c1, c2);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a lexicographic-order comparator with a function that
             * extracts a key to be compared with the given @c Comparator.
             *
             * @note This default implementation behaves as if
             *  @code
             *       Comparator::thenComparing(comparing(keyExtractor, cmp))
             *  @endcode
             *
             * @tparam  R  the type of the sort key
             * @param  keyExtractor the function used to extract the sort key
             * @param  keyComparator the @c Comparator used to compare the sort key
             * @return a lexicographic-order comparator composed of this comparator
             *         and then comparing on the key extracted by the keyExtractor function
             */
            template<class R>
            Comparator &thenComparing(function::Function<T, R> const &keyExtractor,
                                      Comparator<R> const &keyComparator) const {
                try {
                    return thenComparing(comparing(keyExtractor, keyComparator));
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Returns a lexicographic-order comparator with a function that
             * extracts a key to be compared with the given @c Comparator.
             *
             * @note This default implementation behaves as if
             *  @code
             *       Comparator::thenComparing(comparing(keyExtractor, cmp))
             *  @endcode
             *
             * @tparam  R  the type of the sort key
             * @param  keyExtractor the function used to extract the sort key
             * @return a lexicographic-order comparator composed of this comparator
             *         and then comparing on the key extracted by the keyExtractor function
             */
            template<class R>
            Comparator &thenComparing(function::Function<T, R> const &keyExtractor) const {
                try {
                    return thenComparing(comparing(keyExtractor));
                } catch (Throwable const &ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns a lexicographic-order comparator with a function that
             * extracts a @c gint sort key.
             *
             * @note This default implementation behaves as if
             *        @code
             *          Comparator::thenComparing(comparingInt(keyExtractor))
             *        @endcode .
             *
             * @param  keyExtractor the function used to extract the integer sort key
             * @return a lexicographic-order comparator composed of this and then the @c gint sort key
             */
            virtual Comparator &thenComparingInt(function::ToIntFunction<T> const &keyExtractor) const {
                try {
                    return thenComparing(comparingInt(keyExtractor));
                } catch (Throwable const &ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns a lexicographic-order comparator with a function that
             * extracts a @c glong sort key.
             *
             * @note This default implementation behaves as if
             *        @code
             *          Comparator::thenComparing(comparingLong(keyExtractor))
             *        @endcode .
             *
             * @param  keyExtractor the function used to extract the Long sort key
             * @return a lexicographic-order comparator composed of this and then the @c glong sort key
             */
            virtual Comparator &thenComparingLong(function::ToLongFunction<T> const &keyExtractor) const {
                try {
                    return thenComparing(comparingLong(keyExtractor));
                } catch (Throwable const &ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns a lexicographic-order comparator with a function that
             * extracts a @c gdouble sort key.
             *
             * @note This default implementation behaves as if
             *        @code
             *          Comparator::thenComparing(comparingDouble(keyExtractor))
             *        @endcode .
             *
             * @param  keyExtractor the function used to extract the double sort key
             * @return a lexicographic-order comparator composed of this and then the @c gdouble sort key
             */
            virtual Comparator &thenComparingDouble(function::ToDoubleFunction<T> const &keyExtractor) const {
                try {
                    return thenComparing(comparingDouble(keyExtractor));
                } catch (Throwable const &ex) {
                    ex.throws($ftrace());
                }
            }

            /**
             * Returns a comparator that imposes the reverse of the <em>natural ordering</em>.
             *
             * @return a comparator that imposes the reverse of the <i>natural ordering</i> on @c Comparable objects.
             * @throws UnsupportedOperationException If type T not support natural ordering
             */
            static Comparator &reverseOrder() {
                return REVERSE;
            }

            /**
             * Returns a comparator that imposes the reverse of the <em>natural ordering</em>.
             *
             * @return a comparator that imposes the reverse of the <i>natural ordering</i> on @c Comparable objects.
             */
            static Comparator &naturalOrder() {
                return NATURAL;
            }

            /**
             * Accepts a function that extracts a @c Comparable sort key from a type @c T,
             * and returns a @c Comparator<T> that compares by that sort key.
             * <p>
             * For example, to obtain a @c Comparator that compares @c Person objects by their
             * last name ignoring case differences,
             * @code
             *      auto &cmp = Comparator<Person>.comparing(&Person::lastName,
             *                                               String::CASE_INSENSITIVE_ORDER);
             * @endcode
             * </p>
             *
             * @tparam R the type of the sort key
             * @param keyExtractor the function used to extract the sort key
             * @param keyComparator the @c Comparator used to compare the sort key
             * @return a comparator that compares by an extracted key using the specified @c Comparator
             */
            template<class R>
            static Comparator &comparing(function::Function<T, R> const &keyExtractor,
                                         Comparator<R> const &keyComparator) {
                class KeyComparator final : public Comparator {
                    function::Function<T, R> const &keyExtractor;
                    Comparator<R> const &keyComparator;

                public:
                    CORE_EXPLICIT KeyComparator(function::Function<T, R> const &keyExtractor,
                                                Comparator<R> const &keyComparator)
                        : keyExtractor(keyExtractor), keyComparator(keyComparator) {
                    }

                    gint compare(const T &t1, const T &t2) const CORE_NOTHROW override {
                        CORE_ALIAS(Key, $(typename ClassOf(&function::Function<T, R>::apply)::template
                                       Return<function::Function<T, R> const &, T const &>));

                        Key key1 = keyExtractor.apply(t1);
                        Key key2 = keyExtractor.apply(t2);

                        return keyComparator.compare(key1, key2);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<KeyComparator>::hasInstance(o)) {
                            return false;
                        }
                        KeyComparator const &kc = CORE_XCAST(KeyComparator const, o);
                        return keyExtractor.equals(kc.keyExtractor) && keyComparator.equals(kc.keyComparator);
                    }
                };

                try {
                    Comparator<R> &kc = UNSAFE::copyInstance(keyComparator); // For Shared operations 
                    return UNSAFE::newInstance<KeyComparator>(keyExtractor, kc);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Accepts a function that extracts a @c Comparable sort key from a type @c T,
             * and returns a @c Comparator<T> that compares by that sort key.
             * <p>
             * For example, to obtain a @c Comparator that compares @c Person objects by their
             * last name,
             * @code
             *      auto &cmp = Comparator<Person>.comparing(&Person::lastName);
             * @endcode
             * </p>
             *
             * @tparam R the type of the sort key
             * @param keyExtractor the function used to extract the sort key
             * @return a comparator that compares by an extracted key using the specified @c Comparator
             */
            template<class R, ClassOf(1)::OnlyIf<Class<R>::template isExtends<Comparable<R> >()>  = 1>
            static Comparator &comparing(function::Function<T, R> const &keyExtractor) {
                class KeyComparator final : public Comparator {
                    function::ToIntFunction<T> const &keyExtractor;

                public:
                    CORE_EXPLICIT KeyComparator(function::ToIntFunction<T> const &keyExtractor)
                        : keyExtractor(keyExtractor) {
                    }

                    gint compare(const T &t1, const T &t2) const CORE_NOTHROW override {
                        CORE_ALIAS(Key, $(typename ClassOf(&function::Function<T, R>::apply)::template
                                       Return< function::Function<T, R> const &, T const &>));

                        Key key1 = keyExtractor.apply(t1);
                        Key key2 = keyExtractor.apply(t2);

                        return key1.compareTo(key2);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<KeyComparator>::hasInstance(o)) {
                            return false;
                        }
                        KeyComparator const &kc = CORE_XCAST(KeyComparator const, o);
                        return keyExtractor.equals(kc.keyExtractor);
                    }
                };

                try {
                    return UNSAFE::newInstance<KeyComparator>(keyExtractor);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Accepts a function that extracts a @c gint sort key from a type
             * @c T, and returns a @c Comparator<T> that compares by that
             * sort key.
             *
             *
             * @param  keyExtractor the function used to extract the integer sort key
             * @return a comparator that compares by an extracted key
             */
            static Comparator &comparingInt(function::ToIntFunction<T> const &keyExtractor) {
                class IntComparator final : public Comparator {
                    function::ToIntFunction<T> const &keyExtractor;

                public:
                    CORE_EXPLICIT IntComparator(function::ToIntFunction<T> const &keyExtractor)
                        : keyExtractor(keyExtractor) {
                    }

                    gint compare(const T &t1, const T &t2) const CORE_NOTHROW override {
                        const gint key1 = keyExtractor.apply(t1);
                        const gint key2 = keyExtractor.apply(t2);
                        return Integer::compare(key1, key2);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<IntComparator>::hasInstance(o)) {
                            return false;
                        }
                        IntComparator const &kc = CORE_XCAST(IntComparator const, o);
                        return keyExtractor.equals(kc.keyExtractor);
                    }
                };

                try {
                    return UNSAFE::newInstance<IntComparator>(keyExtractor);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Accepts a function that extracts a @c glong sort key from a type
             * @c T, and returns a @c Comparator<T> that compares by that
             * sort key.
             *
             *
             * @param  keyExtractor the function used to extract the integer sort key
             * @return a comparator that compares by an extracted key
             */
            static Comparator &comparingLong(function::ToLongFunction<T> const &keyExtractor) {
                class LongComparator final : public Comparator {
                    function::ToLongFunction<T> const &keyExtractor;

                public:
                    CORE_EXPLICIT LongComparator(function::ToLongFunction<T> const &keyExtractor)
                        : keyExtractor(keyExtractor) {
                    }

                    gint compare(const T &t1, const T &t2) const CORE_NOTHROW override {
                        glong key1 = keyExtractor.apply(t1);
                        glong key2 = keyExtractor.apply(t2);
                        return Long::compare(key1, key2);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<LongComparator>::hasInstance(o)) {
                            return false;
                        }
                        LongComparator const &kc = CORE_XCAST(LongComparator const, o);
                        return keyExtractor.equals(kc.keyExtractor);
                    }
                };

                try {
                    return UNSAFE::newInstance<LongComparator>(keyExtractor);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * Accepts a function that extracts a @c gdouble sort key from a type
             * @c T, and returns a @c Comparator<T> that compares by that
             * sort key.
             *
             *
             * @param  keyExtractor the function used to extract the integer sort key
             * @return a comparator that compares by an extracted key
             */
            static Comparator &comparingDouble(function::ToDoubleFunction<T> const &keyExtractor) {
                class DoubleComparator final : public Comparator {
                    function::ToDoubleFunction<T> const &keyExtractor;

                public:
                    CORE_EXPLICIT DoubleComparator(function::ToDoubleFunction<T> const &keyExtractor)
                        : keyExtractor(keyExtractor) {
                    }

                    gint compare(const T &t1, const T &t2) const CORE_NOTHROW override {
                        gdouble key1 = keyExtractor.apply(t1);
                        gdouble key2 = keyExtractor.apply(t2);
                        return Double::compare(key1, key2);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o) {
                            return true;
                        }
                        if (!Class<DoubleComparator>::hasInstance(o)) {
                            return false;
                        }
                        DoubleComparator const &kc = CORE_XCAST(DoubleComparator const, o);
                        return keyExtractor.equals(kc.keyExtractor);
                    }
                };

                return UNSAFE::newInstance<DoubleComparator>(keyExtractor);
            }

            template<class X,
                ClassOf(1)::OnlyIf<Class<X>::template isSuper<T>()> CaptureComparator = 1>
            static Comparator<T> &wrap(Comparator<X> const &comparator) {
                class Wrapper final : public Comparator<T> {
                    Comparator<X> const &comparator;

                public:
                    CORE_EXPLICIT Wrapper(Comparator<X> const &comparator) : comparator(comparator) {
                    }

                    gint compare(const T &t1, const T &t2) const noexcept override {
                        return comparator.compare(t1, t2);
                    }

                    gbool equals(const Object &o) const override {
                        if (this == &o)
                            return true;

                        if (!Class<Wrapper>::hasInstance(o))
                            return false;

                        Wrapper const &wrapper = CORE_XCAST(Wrapper const, o);
                        return comparator == wrapper.comparator;
                    }
                };

                if (comparator == Comparator<X>::naturalOrder())
                    return naturalOrder();

                if (comparator == Comparator<X>::reverseOrder())
                    return reverseOrder();

                try {
                    if (Class<X>::template isSame<T>())
                        return CORE_XCAST(Comparator<T>, UNSAFE::copyInstance(comparator));

                    return UNSAFE::newInstance<Wrapper>(UNSAFE::copyInstance(comparator));
                } catch (CloneNotSupportedException const &) {
                    try {
                        if (Class<X>::template isSame<T>())
                            return CORE_XCAST(Comparator<T>, CORE_CAST(Comparator<X> &, comparator));

                        return UNSAFE::newInstance<Wrapper>(CORE_CAST(Comparator<X> &, comparator));
                    } catch (Throwable const &ex) { ex.throws($ftrace()); }
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class NaturalOrder;
            class ReverseOrder;
            class ReversedOrder;

            static Comparator &NATURAL;
            static Comparator &REVERSE;
        };

        template<class T>
        class Comparator<T>::NaturalOrder final : public Comparator {
        public:
            gint compare(const T &t1, const T &t2) const noexcept override {
                if (Class<Comparable<T> >::hasInstance(t1))
                    return CORE_XCAST(Comparable<T> const, t1).compareTo(t2);
                ClassCastException().throws($ftrace());
            }
        };

        template<class T>
        class Comparator<T>::ReverseOrder final : public Comparator {
        public:
            gint compare(const T &t1, const T &t2) const noexcept override {
                if (Class<Comparable<T> >::hasInstance(t2))
                    return CORE_XCAST(Comparable<T> const, t2).compareTo(t1);
                ClassCastException().throws($ftrace());
            }
        };

        template<class T>
        class Comparator<T>::ReversedOrder final : public Comparator {
        public:
            Comparator const &comparator;

            CORE_EXPLICIT ReversedOrder(Comparator const &comparator) : comparator(comparator) {
            }

            gint compare(const T &t1, const T &t2) const noexcept override {
                CORE_TRY_RETHROW_AT(core::util::Comparator::ReversedOrder, return comparator.compare(t1, t2));
            }

            gbool equals(const Object &o) const override {
                return this == &o || Class<ReversedOrder>::hasInstance(o)
                       && comparator == CORE_XCAST(ReversedOrder const, o).comparator;
            }
        };

        template<class T>
        Comparator<T> &Comparator<T>::NATURAL = UNSAFE::newInstance<NaturalOrder>();

        template<class T>
        Comparator<T> &Comparator<T>::REVERSE = UNSAFE::newInstance<ReverseOrder>();
    } // util
} // core

#endif //CORE24_COMPARATOR_H
