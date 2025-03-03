//
// Created by brunshweeck on 17 juin 2024.
//

#ifndef CORE24_BIPREDICATE_H
#define CORE24_BIPREDICATE_H

#include <core/function/Predicate.h>

namespace core {
    namespace function {
        /**
         * Represents a predicate (boolean-valued function) of two arguments.  This is
         * the two-arity specialization of @c Predicate.
         *
         * <p>
         * This is a <em>functional interface</em> whose functional method is @c BiPredicate::test.
         * </p>
         *
         * @tparam T the type of the first argument to the predicate
         * @tparam U the type of the second argument the predicate
         */
        template<class T, class U>
        class BiPredicate : public virtual Functional {
            CORE_FULL_CHECK_TYPE(T);
            CORE_FULL_CHECK_TYPE(U);

            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());
            CORE_FAST_ASSERT(!Class<Void>::isSame<U>());

            /**
             * The type of first argument value accepted by this @c Predicate type
             */
            CORE_ALIAS(A, Arg<T>);

            /**
             * The type of second argument value accepted by this @c Predicate type.
             */
            CORE_ALIAS(B, Arg<U>);

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;

            CORE_ALIAS(Launcher, typename Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            /**
             * Construct new Predicate instance that Do Nothing.
             * @note Unlike Consumers classes that not return value
             *       This Constructor is only accessible by derived classes.
             */
            CORE_IMPLICIT BiPredicate() = default;

        public:
            /**
             * Construct new Predicate with specified callable
             * object. The given callable may be: <br/>
             * - a <em> binary static function </em>;
             *     @code
             *       void myFunction(gint, String);
             *
             *       class MyClass {
             *         public:
             *             static gbool myStaticMethod(gint, String);
             *       };
             *
             *       BiPredicate<Integer, String> bp1 = myFunction;
             *       BiPredicate<Integer, String> bp2 = MyClass::myStaticMethod;
             *     @endcode
             * - a <em> binary lambda function </em>;
             *      @code
             *        BiPredicate<Integer, String> bp1 = [](gint, String) {...}
             *        BiPredicate<Integer, String> bp2 = [&](gint, String) {...}
             *        BiPredicate<Integer, String> bp3 = [=](gint, String) {...}
             *      @endcode
             * - an <em>object that implement call operator with two arguments </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gbool operator() (gint, String);
             *        };
             *
             *        BiPredicate<Integer, String> bp = MyCallableClass(...);
             *      @endcode
             *
             * @note the given callable object must be callable with @c T
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A, B>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Boolean>::accept<RT<C, A, B> >()> CaptureReturn = 1>
            CORE_IMPLICIT BiPredicate(C &&callable): BiPredicate() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Predicate with class method.
             * @code
             * class MyClass {
             *  public:
             *      virtual gbool myMethod(gint, String);
             * };
             *
             * class MyDerivedClass: public MyClass {
             *  public:
             *      gbool myMethod(gint, String) override;
             *      gbool myMethod(gint);
             * };
             *
             * BiPredicate<Integer, String> bp1 = { MyClass(), &MyClass::myMethod };
             * // ambiguous method detection
             * BiPredicate<Integer, String> bp2 = { MyDerivedClass(), &MyDerivedClass::myMethod };
             * // solution
             * BiPredicate<Integer, String> bp2 = { MyDerivedClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<O>::isClass()> CaptureObject = 1,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureMethod = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A, B>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Boolean>::accept<RT<M, O, A, B> >()> CaptureReturn = 1>
            CORE_IMPLICIT BiPredicate(O &&object, M method): BiPredicate() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gbool invoke(A a, B b) const override {
                        return CORE_CAST(object.*method, a, b);
                    }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c BiPredicate with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT BiPredicate(BiPredicate const &other) CORE_NOTHROW : BiPredicate() {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
            }

            /**
             * Destroy this instance.
             */
            ~BiPredicate() CORE_NOTHROW override {
                if (launcher && launcher->release() == 0) delete launcher;
            }

            BiPredicate(BiPredicate &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            BiPredicate & operator=(const BiPredicate &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            BiPredicate & operator=(BiPredicate &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given argument
             * value.
             *
             * @param a the first input argument.
             * @param b the second input argument.
             */
            virtual gbool test(A a, B b) const CORE_NOTHROW {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(a, b);
            }

            /**
             * Returns a predicate that represents the logical negation of this
             * predicate.
             *
             * @return a new predicate
             */
            BiPredicate &negate() const {
                return [&](A a, B b) -> gbool { return !test(a, b); };
            }

            /**
             * Obtains the composed @c Predicate that represents a short-circuiting
             * logical AND of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is @c false, then the @c other predicate
             *  is not evaluated.
             * Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller. If evaluation of this predicate throws an exception, the
             * @c other predicate will not be evaluated.
             *
             * @tparam T2 the argument type of specified predicate.
             * @param other a predicate that will be logically-ANDed with this predicate
             * @return new predicate
             */
            template<class T2, class U2>
            BiPredicate &logicalAnd(BiPredicate<T2, U2> const &other) const {
                return [&](A a, B b) -> gbool { return test(a, b) && other.test(a, b); };
            }

            /**
             * Obtains the composed @c Predicate that represents a short-circuiting
             * logical OR of this predicate and another.  When evaluating the composed
             * predicate, if this predicate is @c false, then the @c other predicate
             *  is not evaluated.
             * Any exceptions thrown during evaluation of either predicate are relayed
             * to the caller. If evaluation of this predicate throws an exception, the
             * @c other predicate will not be evaluated.
             *
             * @tparam T2 the argument type of specified predicate.
             * @param other a predicate that will be logically-ANDed with this predicate
             * @return new predicate
             */
            template<class T2, class U2>
            BiPredicate &logicalOr(BiPredicate<T2, U2> const &other) const {
                return [&](A a, B b) -> gbool { return test(a, b) || other.test(a, b); };
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<BiPredicate>::hasInstance(obj) && launcher == CORE_XCAST(BiPredicate const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<BiPredicate>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual gbool invoke(A a, B b) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gbool invoke(A a, B b) const override {
                    return f(a, b);
                }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gbool invoke(A a, B b) const CORE_NOTHROW override {
                    return obj(a, b);
                }
            };
        };
    } // function
} // core

#endif //CORE24_BIPREDICATE_H
