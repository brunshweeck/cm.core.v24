//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_INTUNARYOPERATOR_H
#define CORE24_INTUNARYOPERATOR_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents an operation on a single @c int - valued operand that produces
         * an @c int - valued result.  This is the primitive type specialization of
         * @c UnaryOperator for @c gint.
         * <p>
         * This is a functional interface whose functional method is @c IntUnaryOperator::apply.
         * </p>
         */
        class IntUnaryOperator final : public virtual Functional {
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
            CORE_IMPLICIT IntUnaryOperator() = default;

        public:
            /**
             * Construct new Operator with specified callable
             * object. The given callable may be:
             * @li a <em> unary static function </em>;
             *     @code
             *       gint myFunction(gint);
             *
             *       class MyClass {
             *         public:
             *             static gint myStaticMethod(gint);
             *       };
             *
             *       IntUnaryOperator op1 = myFunction;
             *       IntUnaryOperator op2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> unary lambda function </em>;
             *      @code
             *        IntUnaryOperator op1 = [](gint) -> gint {...}
             *        IntUnaryOperator op2 = [&](gint) -> gint {...}
             *        IntUnaryOperator op3 = [=](gint) -> gint {...}
             *      @endcode
             * @li an <em>object that implement call operator with one argument </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gint operator() (gint);
             *        };
             *
             *        IntUnaryOperator op = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Integer>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<C, Integer> >()> CaptureReturn = 1>
            CORE_IMPLICIT IntUnaryOperator(C &&callable): IntUnaryOperator() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Operator with class method.
             * @code
             * class MyClass {
             *  public:
             *      gint myMethod(gint);
             * };
             *
             * IntUnaryOperator op = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Integer>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<M, O, Integer> >()> CaptureReturn = 1>
            CORE_IMPLICIT IntUnaryOperator(O &&object, M method): IntUnaryOperator() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gint invoke(gint arg) const override {
                        return CORE_CAST(object.*method, arg);
                    }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new instance of @c IntUnaryOperator with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT IntUnaryOperator(IntUnaryOperator const &other) {
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
            ~IntUnaryOperator() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            IntUnaryOperator(IntUnaryOperator &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            IntUnaryOperator &operator=(const IntUnaryOperator &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            IntUnaryOperator &operator=(IntUnaryOperator &&other) CORE_NOTHROW {
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
            gint apply(gint arg) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(arg);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       (Class<IntUnaryOperator>::hasInstance(obj) &&
                       launcher == CORE_XCAST(IntUnaryOperator const, obj).launcher);
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<IntUnaryOperator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual gint invoke(gint arg) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gint invoke(gint arg) const override { return f(arg); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gint invoke(gint arg) const override { return obj(arg); }
            };
        };
    } // function
} // core

#endif //CORE24_INTUNARYOPERATOR_H
