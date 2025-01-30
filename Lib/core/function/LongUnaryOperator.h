//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_LONGUNARYOPERATOR_H
#define CORE24_LONGUNARYOPERATOR_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents an operation on a single @c long - valued operand that produces
         * a @c long - valued result.  This is the primitive type specialization of
         * @c UnaryOperator for @c glong.
         * <p>
         * This is a functional interface whose functional method is @c LongUnaryOperator::apply.
         * </p>
         */
        class LongUnaryOperator final : public virtual Functional {
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
            CORE_IMPLICIT LongUnaryOperator() = default;

        public:
            /**
             * Construct new Operator with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       glong myFunction(glong);
             *
             *       class MyClass {
             *         public:
             *             static glong myStaticMethod(glong);
             *       };
             *
             *       LongUnaryOperator op1 = myFunction;
             *       LongUnaryOperator op2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        LongUnaryOperator op1 = [](glong) -> glong {...}
             *        LongUnaryOperator op2 = [&](glong) -> glong {...}
             *        LongUnaryOperator op3 = [=](glong) -> glong {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          glong operator() (glong);
             *        };
             *
             *        LongUnaryOperator op = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Long>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Long>::accept<RT<C, Long> >()> CaptureReturn = 1>
            CORE_IMPLICIT LongUnaryOperator(C &&callable): LongUnaryOperator() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Operator with class method.
             * @code
             * class MyClass {
             *  public:
             *      glong myMethod(glong);
             * };
             *
             * LongUnaryOperator op = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Long>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Long>::accept<RT<M, O, Long> >()> CaptureReturn = 1>
            CORE_IMPLICIT LongUnaryOperator(O &&object, M method): LongUnaryOperator() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    glong invoke(glong arg) const override {
                        return CORE_CAST(object.*method, arg);
                    }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c LongUnaryOperator with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT LongUnaryOperator(LongUnaryOperator const &other) {
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
            ~LongUnaryOperator() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            LongUnaryOperator(LongUnaryOperator &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            LongUnaryOperator &operator=(const LongUnaryOperator &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            LongUnaryOperator &operator=(LongUnaryOperator &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Performs this operation on the given arguments
             * values.
             *
             * @param arg the input argument.
             * @return the function result
             */
            glong apply(glong arg) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(arg);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<LongUnaryOperator>::hasInstance(obj) &&
                       launcher == CORE_XCAST(LongUnaryOperator const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<LongUnaryOperator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual glong invoke(glong arg) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                glong invoke(glong arg) const override { return f(arg); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                glong invoke(glong arg) const override { return obj(arg); }
            };
        };
    } // function
} // core

#endif //CORE24_LONGUNARYOPERATOR_H
