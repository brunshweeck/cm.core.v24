//
// Created by brunshweeck on 14 juin 2024.
//

#ifndef CORE24_CONSUMER_H
#define CORE24_CONSUMER_H

#include <core/IllegalArgumentException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents an operation that accepts a single input argument and returns no
         * result. Unlike most other functional interfaces, @c Consumer is expected
         * to operate via side effects.
         * <p>
         * This is a functional interface whose functional method is @em Consumer::accept.
         * </p>
         *
         * @tparam T the type of the input to the operation
         */
        template<class T>
        class Consumer : public virtual Functional {
            CORE_FULL_CHECK_TYPE(T);
            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            /**
             * The type of argument value accepted by
             * @c Consumer instance of type T.
             */
            CORE_ALIAS(A, Arg<T>);

            class Handle;

            CORE_ALIAS(Launcher, typename Class<Handle>::Pointer);

            /**
             * The consumer launcher field
             */
            Launcher launcher = {};

        public:
            /**
             * Construct new Consumer that do nothing
             */
            CORE_IMPLICIT Consumer() = default;

            /**
             * Construct new Consumer with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       void myFunction(String);
             *
             *       class MyClass {
             *         public:
             *             static void myStaticMethod(String);
             *       };
             *
             *       Consumer<String> c1 = myFunction;
             *       Consumer<String> c2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        Consumer<String> c1 = [](String) {...}
             *        Consumer<String> c2 = [&](String) {...}
             *        Consumer<String> c3 = [=](String) {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          void operator() (String);
             *        };
             *
             *        Consumer<String> c = MyCallableClass(...);
             *      @endcode
             *
             * @note the given callable object must be callable with @c T
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<A>()> CaptureArgs = 1>
            CORE_IMPLICIT Consumer(C &&callable): Consumer() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Consumer with class method.
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @tparam CaptureObject the value used to ensure that the first argument is class instance.
             * @tparam CaptureMethod the value used to ensure that the second argument is class method.
             * @tparam CaptureArgs the value used to ensure that the second argument is compatible with
             *                     first and this consumer argument.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<O>::isClass()> CaptureObject = 1,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureMethod = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, A>()> CaptureArgs = 1>
            CORE_IMPLICIT Consumer(O &&object, M method): Consumer() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    void invoke(const A &arg) const override { CORE_CAST(object.*method, arg); }
                };

                launcher = new MethodHandle(object, method);
            }

            /**
             * Construct new instance of @c Consumer with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT Consumer(Consumer const &other) CORE_NOTHROW: Consumer() {
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
            ~Consumer() CORE_NOTHROW override {
                if (launcher && launcher->release() == 0) delete launcher;
            }

            Consumer(Consumer &&other) CORE_NOTHROW  {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            Consumer & operator=(const Consumer &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            Consumer & operator=(Consumer &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given argument value.
             *
             * @param arg the input argument.
             */
            virtual void accept(A arg) const CORE_NOTHROW {
                if (launcher) launcher->invoke(arg);
            }

            /**
             * Obtains the composed @c Consumer that performs, in sequence,
             * this operation followed by @c after operation.
             * If performing either operation throws an exception, it is
             * relayed to the caller of the composed operation.
             * If performing this operation throws an exception, the
             * @c after operation will not be performed.
             *
             * @tparam X the argument type of specified consumer.
             * @param after the operation to performs after this operation.
             * @return A composed @c Consumer that performs in sequence this
             *          operation followed by the @c after operation.
             */
            template<class X, ClassOf(1)::OnlyIf<Class<X>::template isSuper<T>()> CaptureX = 1>
            Consumer &andThen(Consumer<X> const &after) const {
                return [&](A arg) -> void {
                    accept(arg);
                    after.accept(arg);
                };
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                    Class<Consumer>::hasInstance(obj) && launcher == CORE_XCAST(Consumer const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<Consumer>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual void invoke(A arg) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f) : f(f) {
                }

                void invoke(A arg) const override { f(arg); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj) : obj(obj) {
                }

                void invoke(A arg) const CORE_NOTHROW override { obj(arg); }
            };
        };
    } // function
} // core

#endif //CORE24_CONSUMER_H
