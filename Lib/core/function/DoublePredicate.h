//
// Created by brunshweeck on 25 juin 2024.
//

#ifndef CORE24_DOUBLEPREDICATE_H
#define CORE24_DOUBLEPREDICATE_H

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a predicate (boolean-valued function) of one @c double-valued
         * argument. This is the @c double-consuming primitive type specialization
         * of @c Predicate.
         */
        class DoublePredicate final : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            /**
             * Construct new Operator that do nothing
             * @note Unlike consumer this constructor is only accessible
             * by derived classes
             */
            CORE_IMPLICIT DoublePredicate() = default;

        public:
            /**
             * Construct new Predicate with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       gdouble myFunction(gdouble);
             *
             *       class MyClass {
             *         public:
             *             static gbool myStaticMethod(gdouble);
             *       };
             *
             *       DoublePredicate p1 = myFunction;
             *       DoublePredicate p2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        DoublePredicate p1 = [](gdouble) -> gbool {...}
             *        DoublePredicate p2 = [&](gdouble) -> gbool {...}
             *        DoublePredicate p3 = [=](gdouble) -> gbool {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gbool operator() (gdouble);
             *        };
             *
             *        DoublePredicate p = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Double>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Boolean>::accept<RT<C, Double> >()> CaptureReturn = 1>
            CORE_IMPLICIT DoublePredicate(C &&callable) {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Predicate with class method.
             * @code
             * class MyClass {
             *  public:
             *      gbool myMethod(gdouble);
             * };
             *
             * DoublePredicate p = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Double>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Boolean>::accept<RT<M, O, Double> >()> CaptureReturn = 1>
            CORE_IMPLICIT DoublePredicate(O &&object, M method): DoublePredicate() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gbool invoke(gdouble arg) const CORE_NOTHROW override {
                        return CORE_CAST(object.*method, arg);
                    }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new Predicate with another consumer.
             *
             * @param other the predicate.
             */
            CORE_IMPLICIT DoublePredicate(const DoublePredicate &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
            }

            ~DoublePredicate() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            DoublePredicate(DoublePredicate &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            DoublePredicate & operator=(const DoublePredicate &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            DoublePredicate & operator=(DoublePredicate &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Evaluates this predicate on the given argument.
             *
             * @param value the input argument
             * @return @c true if the input argument matches the predicate, otherwise @c false
             */
            gbool test(gdouble value) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(value);
            }

            /**
             * Returns a composed predicate that represents a short-circuiting logical
             * AND of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is @c false, then the @c other
             * predicate is not evaluated.
             * <p>
             * Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller; if evaluation of this predicate throws an exception, the
             * @c other predicate will not be evaluated.
             * </p>
             * @param other a predicate that will be logically-ANDed with this
             *              predicate
             * @return a composed predicate that represents the short-circuiting logical
             * AND of this predicate and the @c other predicate
             */
            DoublePredicate logicalAnd(DoublePredicate const &other) const {
                return [&](gdouble arg) -> gdouble { return test(arg) && other.test(arg); };
            }

            /**
             * Returns a predicate that represents the logical negation of this
             * predicate.
             *
             * @return a predicate that represents the logical negation of this
             * predicate
             */
            DoublePredicate negate() const {
                return [&](gdouble arg) -> gbool { return !test(arg); };
            }

            /**
             * Returns a composed predicate that represents a short-circuiting logical
             * OR of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is @c true, then the @c other
             * predicate is not evaluated.
             * <p>
             * Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller; if evaluation of this predicate throws an exception, the
             * @c other predicate will not be evaluated.
             * </p>
             * @param other a predicate that will be logically-ORed with this
             *              predicate
             * @return a composed predicate that represents the short-circuiting logical
             *          OR of this predicate and the @c other predicate
             */
            DoublePredicate logicalOr(DoublePredicate const &other) const {
                return [&](gdouble arg) -> gdouble { return test(arg) || other.test(arg); };
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<DoublePredicate>::hasInstance(obj) &&
                       launcher == CORE_XCAST(DoublePredicate const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<DoublePredicate>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle: public SharedHandle {
            public:
                virtual gbool invoke(gdouble value) const = 0;
            };

            template<class T, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gbool invoke(gdouble value) const override { return f(value); }
            };

            template<class T>
            class Callable<T, OBJECT_REFERENCE> final : public Handle {
                T obj;

            public:
                CORE_EXPLICIT Callable(T obj): obj(obj) {
                }

                gbool invoke(gdouble value) const override { return obj(value); }
            };
        };
    } // function
} // core

#endif //CORE24_DOUBLEPREDICATE_H
