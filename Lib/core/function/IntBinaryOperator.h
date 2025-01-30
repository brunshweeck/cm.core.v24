//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_INTBINARYOPERATOR_H
#define CORE24_INTBINARYOPERATOR_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents an operation upon two @c int - valued operands and producing an
         * @c int - valued result.   This is the primitive type specialization of
         * @c BinaryOperator for @c gint.
         * <p>
         * This is a functional interface whose functional method is @c IntBinaryOperator::apply.
         * </p>
         */
        class IntBinaryOperator final : public virtual Functional {
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
            CORE_IMPLICIT IntBinaryOperator() = default;

        public:
            /**
             * Construct new Operator with specified callable
             * object. The given callable may be:
             * @li a <em> binary static function </em>;
             *     @code
             *       gint myFunction(gint, gint);
             *
             *       class MyClass {
             *         public:
             *             static gint myStaticMethod(gint, gint);
             *       };
             *
             *       IntBinaryOperator op1 = myFunction;
             *       IntBinaryOperator op2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> binary lambda function </em>;
             *      @code
             *        IntBinaryOperator op1 = [](gint, gint) -> gint {...}
             *        IntBinaryOperator op2 = [&](gint, gint) -> gint {...}
             *        IntBinaryOperator op3 = [=](gint, gint) -> gint {...}
             *      @endcode
             * @li an <em>object that implement call operator with two arguments </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gint operator() (gint, gint);
             *        };
             *
             *        IntBinaryOperator op = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::template isCallable<Integer, Integer>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<C, Integer, Integer> >()> CaptureReturn = 1>
            CORE_IMPLICIT IntBinaryOperator(C &&callable): IntBinaryOperator() {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Operator with class method.
             * @code
             * class MyClass {
             *  public:
             *      gint myMethod(gint, gint);
             * };
             *
             * IntBinaryOperator op = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O, Integer, Integer>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Integer>::accept<RT<M, O, Integer, Integer> >()> CaptureReturn = 1>
            CORE_IMPLICIT IntBinaryOperator(O &&object, M method): IntBinaryOperator() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gint invoke(gint a, gint b) const override { return CORE_CAST(object.*method, a, b); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new Operator with another consumer.
             *
             * @param other the function.
             */
            CORE_IMPLICIT IntBinaryOperator(IntBinaryOperator const &other) {
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
            ~IntBinaryOperator() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            IntBinaryOperator(IntBinaryOperator &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            IntBinaryOperator &operator=(const IntBinaryOperator &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            IntBinaryOperator &operator=(IntBinaryOperator &&other) CORE_NOTHROW {
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
            gint apply(gint a, gint b) const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke(a, b);
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<IntBinaryOperator>::hasInstance(obj) &&
                       launcher == CORE_XCAST(IntBinaryOperator const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<IntBinaryOperator>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual gint invoke(gint a, gint b) const = 0;
            };

            template<class C, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gint invoke(gint a, gint b) const override { return f(a, b); }
            };

            template<class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj) {
                }

                gint invoke(gint a, gint b) const override { return obj(a, b); }
            };
        };
    } // function
} // core

#endif //CORE24_INTBINARYOPERATOR_H
