//
// Created by brunshweeck on 17 juin 2024.
//

#ifndef CORE24_BIFUNCTION_H
#define CORE24_BIFUNCTION_H

#include <core/function/Function.h>

namespace core {
    namespace function {
        /**
         * Represents a function that accepts two arguments and produces a result.
         * This is the two-arity specialization of @c Function.
         * <p>
         * This is a <em>functional interface</em> whose functional method is
         * @c BiFunction::apply.
         *
         * @tparam T the type of the first argument to the function
         * @tparam U the type of the second argument to the function
         * @tparam R the type of the result of the function
         */
        template<class T, class U, class R>
        class BiFunction : public virtual Functional {
            CORE_FULL_CHECK_TYPE(T);
            CORE_FULL_CHECK_TYPE(U);
            CORE_FULL_CHECK_TYPE(R);

            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());
            CORE_FAST_ASSERT(!Class<Void>::isSame<U>());
            CORE_FAST_ASSERT(!Class<Void>::isSame<R>());

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_TEMPLATES_AS_FRIEND(BinaryOperator,);

            /**
             * The type of first argument value accepted by this @c Function type
             */
            CORE_ALIAS(A, Arg<T>);

            /**
             * The type of second argument value accepted by this @c Function type.
             */
            CORE_ALIAS(B, Arg<U>);

            /**
             * The type of value returned by @c Function
             * instance of type T.
             * @note it's always void.
             */
            CORE_ALIAS(Re, Return<R>);

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, typename Class<Handle>::Pointer);

            Launcher launcher = null;

        protected:
            /**
             * Construct new Function that do nothing
             * @note Unlike consumer this constructor is only accessible
             * by derived classes
             */
            CORE_IMPLICIT BiFunction() = default;

        public:
            /**
             * Construct new Function with specified callable
             * object. The given callable may be: <br/>
             * - a <em> binary static function </em>;
             *     @code
             *       gint myFunction(String, gdouble);
             *
             *       class MyClass {
             *         public:
             *             static gint myStaticMethod(String, gdouble);
             *       };
             *
             *       BiFunction<String, Double, Integer> f1 = myFunction;
             *       BiFunction<String, Double, Integer> f2 = MyClass::myStaticMethod;
             *     @endcode
             * - a <em> binary lambda function </em>;
             *      @code
             *        BiFunction<String, Double, Integer> f1 = [](String, gdouble) -> gint {...}
             *        BiFunction<String, Double, Integer> f2 = [&](String, gdouble) -> gint {...}
             *        BiFunction<String, Double, Integer> f3 = [=](String, gdouble) -> gint {...}
             *      @endcode
             * - an <em>object that implement call operator with two arguments </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gint operator() (String, gdouble);
             *        };
             *
             *        BiFunction<String, Double, Integer> f = MyCallableClass(...);
             *      @endcode
             *
             * @note the given callable object must be callable with @c T
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A, B>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<C, A, B> >::template isConvertible<Re>()> CaptureReturn = 1>
            CORE_IMPLICIT BiFunction(C &&callable): BiFunction() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Function with class method.
             * @code
             * class MyClass {
             *  public:
             *      virtual gint myMethod(String);
             * };
             *
             * class MyDerivedClass: public MyClass {
             *  public:
             *      gint myMethod(String, gdouble) override;
             *      String myMethod(gint);
             * };
             *
             * BiFunction<String, Double, Integer> bc1 = { MyClass(), &MyClass::myMethod };
             * // ambiguous method detection
             * BiFunction<String, Double, Integer> bc2 = { MyDerivedClass(), &MyDerivedClass::myMethod };
             * // solution
             * BiFunction<String, Double, Integer> bc2 = { MyDerivedClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<O>::isClass()> CaptureObject = 1,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A, B>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<RT<M, O, A, B> >::template isConvertible<Re>()> CaptureReturn = 1>
            CORE_IMPLICIT BiFunction(O &&object, M method): BiFunction() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    Re invoke(A a, B b) const CORE_NOTHROW override {
                        return CORE_CAST(object.*method, a, b);
                    }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c BiFunction with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT BiFunction(BiFunction const &other) CORE_NOTHROW : BiFunction() {
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
            ~BiFunction() CORE_NOTHROW override {
                if (launcher && launcher->release() == 0) delete launcher;
            }

            BiFunction(BiFunction &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            BiFunction &operator=(const BiFunction &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            BiFunction &operator=(BiFunction &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given arguments
             * values.
             *
             * @param a the first input argument.
             * @param b the second input argument.
             * @return the function result
             */
            virtual Re apply(A a, B b) const CORE_NOTHROW {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(a, b);
            }

            /**
             * Obtains a composed function that first applies the @c after
             * function to its input, and then applies this function to the result.
             * If evaluation of either function throws an exception, it is relayed to
             * the caller of the composed function.
             *
             * @tparam X the type of input to the @c after function.
             * @tparam Y the type of return type of @c after function and composed function.
             * @param after the function to apply after this function is applied
             * @return a composed function that first applies this function and then
             *          applies the @c after function
             */
            template<class X, class Y,
                ClassOf(1)::OnlyIf<Class<X>::template accept<R>()> CaptureX = 1>
            BiFunction<T, U, Y> &andThen(Function<X, Y> const &after) const {
                return [&](A a, B b) -> Return<Y> { return after.apply(apply(a, b)); };
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<BiFunction>::hasInstance(obj) &&
                       launcher == CORE_XCAST(BiFunction const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<BiFunction>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual Re invoke(A a, B b) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                Re invoke(A a, B b) const CORE_NOTHROW override { return f(a, b); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                Re invoke(A a, B b) const CORE_NOTHROW override {
                    return obj(a, b);
                }
            };
        };
    } // function
} // core

#endif //CORE24_BIFUNCTION_H
