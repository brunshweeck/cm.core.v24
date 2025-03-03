//
// Created by brunshweeck on 25 juin 2024.
//

#ifndef CORE24_LONGCONSUMER_H
#define CORE24_LONGCONSUMER_H

#include <core/lang/IllegalArgumentException.h>
#include <core/lang/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents an operation that accepts a single @c long-valued argument and
         * returns no result.  This is the primitive type specialization of
         * @c Consumer for @c long.  Unlike most other functional interfaces,
         * @c LongConsumer is expected to operate via side effects.
         */
        class LongConsumer : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

            class Handle;
            CORE_ALIAS(Launcher, Class<Handle>::Pointer);

            Launcher launcher = {};

        public:
            /**
             * Construct new Consumer that do nothing
             */
            CORE_IMPLICIT LongConsumer() = default;

            /**
             * Construct new Consumer with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       void myFunction(glong);
             *
             *       class MyClass {
             *         public:
             *             static void myStaticMethod(glong);
             *       };
             *
             *       LongConsumer op1 = myFunction;
             *       LongConsumer op2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        LongConsumer op1 = [](glong) {...}
             *        LongConsumer op2 = [&](glong) {...}
             *        LongConsumer op3 = [=](glong) {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          void operator() (glong);
             *        };
             *
             *        LongConsumer op = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Long>()> CaptureArgs = 1>
            CORE_IMPLICIT LongConsumer(C &&callable) {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Consumer with class method.
             * @code
             * class MyClass {
             *  public:
             *      void myMethod(glong);
             * };
             *
             * LongConsumer c = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Long>()> CaptureArgs = 1>
            CORE_IMPLICIT LongConsumer(O &&object, M method): LongConsumer() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    void invoke(glong arg) const override { return CORE_CAST(object.*method, arg); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new Consumer with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT LongConsumer(const LongConsumer &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
            }

            ~LongConsumer() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            LongConsumer(LongConsumer &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            LongConsumer & operator=(const LongConsumer &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            LongConsumer & operator=(LongConsumer &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given argument.
             *
             * @param value the input argument
             */
            virtual void accept(glong value) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(value);
            }

            /**
             * Returns a composed @c LongConsumer that performs, in sequence, this
             * operation followed by the @c after operation. If performing either
             * operation throws an exception, it is relayed to the caller of the
             * composed operation.  If performing this operation throws an exception,
             * the @c after operation will not be performed.
             *
             * @param after the operation to perform after this operation
             * @return a composed @c LongConsumer that performs in sequence this
             *          operation followed by the @c after operation
             */
            LongConsumer andThen(LongConsumer const &after) const {
                return [&](glong value) -> void {
                    accept(value);
                    after.accept(value);
                };
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<LongConsumer>::hasInstance(obj) &&
                       launcher == CORE_XCAST(LongConsumer const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<LongConsumer>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle: public SharedHandle {
            public:
                virtual void invoke(glong value) const = 0;
            };

            template<class T, Status status>
            class Callable;


            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                void invoke(glong value) const override { f(value); }
            };

            template<class T>
            class Callable<T, OBJECT_REFERENCE> final : public Handle {
                T obj;

            public:
                CORE_EXPLICIT Callable(T obj): obj(obj) {
                }

                void invoke(glong value) const override { obj(value); }
            };
        };
    } // function
} // core

#endif //CORE24_LONGCONSUMER_H
