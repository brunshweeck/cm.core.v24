//
// Created by brunshweeck on 25 juin 2024.
//

#ifndef CORE24_DOUBLECONSUMER_H
#define CORE24_DOUBLECONSUMER_H

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents an operation that accepts a single @c double-valued argument and
         * returns no result.  This is the primitive type specialization of
         * @c Consumer for @c double.  Unlike most other functional interfaces,
         * @c DoubleConsumer is expected to operate via side effects.
         */
        class DoubleConsumer : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

            class Handle;
            CORE_ALIAS(Launcher, Class<Handle>::Pointer);

            Launcher launcher = {};

        public:
            /**
             * Construct new Consumer that do nothing
             */
            CORE_IMPLICIT DoubleConsumer() = default;

            /**
             * Construct new Consumer with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       void myFunction(gdouble);
             *
             *       class MyClass {
             *         public:
             *             static void myStaticMethod(gdouble);
             *       };
             *
             *       DoubleConsumer op1 = myFunction;
             *       DoubleConsumer op2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        DoubleConsumer op1 = [](gdouble) {...}
             *        DoubleConsumer op2 = [&](gdouble) {...}
             *        DoubleConsumer op3 = [=](gdouble) {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          void operator() (gdouble);
             *        };
             *
             *        DoubleConsumer op = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Double>()> CaptureArgs = 1>
            CORE_IMPLICIT DoubleConsumer(C &&callable) {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Consumer with class method.
             * @code
             * class MyClass {
             *  public:
             *      void myMethod(gdouble);
             * };
             *
             * DoubleConsumer c = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Double>()> CaptureArgs = 1>
            CORE_IMPLICIT DoubleConsumer(O &&object, M method): DoubleConsumer() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    void invoke(gdouble arg) const override { return CORE_CAST(object.*method, arg); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new Consumer with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT DoubleConsumer(const DoubleConsumer &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
            }

            ~DoubleConsumer() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            DoubleConsumer(DoubleConsumer &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            DoubleConsumer & operator=(const DoubleConsumer &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            DoubleConsumer & operator=(DoubleConsumer &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given argument.
             *
             * @param value the input argument
             */
            virtual void accept(gdouble value) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(value);
            }

            /**
             * Returns a composed @c DoubleConsumer that performs, in sequence, this
             * operation followed by the @c after operation. If performing either
             * operation throws an exception, it is relayed to the caller of the
             * composed operation.  If performing this operation throws an exception,
             * the @c after operation will not be performed.
             *
             * @param after the operation to perform after this operation
             * @return a composed @c DoubleConsumer that performs in sequence this
             *          operation followed by the @c after operation
             */
            DoubleConsumer andThen(DoubleConsumer const &after) const {
                return [&](gdouble value) -> void {
                    accept(value);
                    after.accept(value);
                };
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<DoubleConsumer>::hasInstance(obj) &&
                       launcher == CORE_XCAST(DoubleConsumer const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<DoubleConsumer>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle: public SharedHandle {
            public:
                virtual void invoke(gdouble value) const = 0;
            };

            template<class T, Status status>
            class Callable;


            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                void invoke(gdouble value) const override { f(value); }
            };

            template<class T>
            class Callable<T, OBJECT_REFERENCE> final : public Handle {
                T obj;

            public:
                CORE_EXPLICIT Callable(T obj): obj(obj) {
                }

                void invoke(gdouble value) const override { obj(value); }
            };
        };
    } // function
} // core

#endif //CORE24_DOUBLECONSUMER_H
