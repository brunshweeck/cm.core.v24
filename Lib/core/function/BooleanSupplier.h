//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_BOOLEANSUPPLIER_H
#define CORE24_BOOLEANSUPPLIER_H

#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/function/Functional.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace function {
        /**
         * Represents a supplier of @c boolean-valued results.  This is the
         * @c boolean-producing primitive specialization of @c Supplier.
         * <p>
         * There is no requirement that a distinct result be returned each
         * time the supplier is invoked.
         * </p>
         */
        class BooleanSupplier final : public virtual Functional {
            CORE_ADD_GLOBAL_FRIENDS();

            template<class C, class... A>
            CORE_ALIAS(RT, typename Class<C>::template Return<A...>);

            class Handle;
            CORE_ALIAS(Launcher, Class<Handle>::Pointer);

            Launcher launcher = {};

        protected:
            /**
             * Construct new Supplier that do nothing
             * @note Unlike consumer this constructor is only accessible
             * by derived classes
             */
            CORE_IMPLICIT BooleanSupplier() = default;

        public:
            /**
             * Construct new Supplier with specified callable
             * object. The given callable may be:
             * @li a <em> static function </em>;
             *     @code
             *       gbool myFunction();
             *
             *       class MyClass {
             *         public:
             *             static gbool myStaticMethod();
             *       };
             *
             *       BooleanSupplier s1 = myFunction;
             *       BooleanSupplier s2 = MyClass::myStaticMethod;
             *     @endcode
             * @li a <em> lambda function </em>;
             *      @code
             *        BooleanSupplier s1 = []() -> gbool {...}
             *        BooleanSupplier s2 = [&]() -> gbool {...}
             *        BooleanSupplier s3 = [=]() -> gbool {...}
             *      @endcode
             * @li an <em>object that implement call operator without arguments </em>.
             *      @code
             *        class MyCallableClass {
             *        public:
             *          gbool operator() ();
             *        };
             *
             *        BooleanSupplier s = MyCallableClass(...);
             *      @endcode
             *
             * @param callable the callable object.
             */
            template<class C,
                ClassOf(1)::OnlyIf<!Class<C>::isFunctionMember() && !Class<Functional>::isSuper<C>()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<C>::isCallable()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Boolean>::accept<RT<C> >()> CaptureReturn = 1>
            CORE_IMPLICIT BooleanSupplier(C &&callable) {
                if (!Checker<C>::isValid(callable))
                    IllegalArgumentException().throws($ftrace());

                launcher = new Callable<C, findStatus<C>()>(UNSAFE::forwardInstance<C>(callable));
            }

            /**
             * Construct new Supplier with class method.
             * @code
             * class MyClass {
             *  public:
             *      gbool myMethod();
             * };
             *
             * BooleanSupplier s = { MyClass(), &MyClass::myMethod };
             * @endcode
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template<class O, class M,
                ClassOf(1)::OnlyIf<Class<M>::isFunctionMember()> CaptureCallable = 1,
                ClassOf(1)::OnlyIf<Class<M>::template isCallable<O>()> CaptureArgs = 1,
                ClassOf(1)::OnlyIf<Class<Boolean>::accept<RT<M, O> >()> CaptureReturn = 1>
            CORE_IMPLICIT BooleanSupplier(O &&object, M method): BooleanSupplier() {
                class MethodHandle final : public Handle {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method) {
                    }

                    gbool invoke() const override { return CORE_CAST(object.*method,); }
                };

                launcher = new MethodHandle(UNSAFE::forwardInstance<O>(object), method);
            }

            /**
             * Construct new Supplier with another consumer.
             *
             * @param other the supplier.
             */
            CORE_IMPLICIT BooleanSupplier(const BooleanSupplier &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
            }

            ~BooleanSupplier() override {
                if (launcher && launcher->release() == 0) delete launcher;
                launcher = null;
            }

            BooleanSupplier(BooleanSupplier &&other) CORE_NOTHROW {
                UNSAFE::swapValues(launcher, other.launcher);
            }

            BooleanSupplier &operator=(const BooleanSupplier &other) {
                if (this != &other && other.launcher) {
                    if (launcher && launcher->release() == 0)
                        delete launcher;
                    launcher = other.launcher;
                    launcher->acquire();
                }
                return *this;
            }

            BooleanSupplier &operator=(BooleanSupplier &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(launcher, other.launcher);
                return *this;
            }

            /**
             * Gets a result.
             *
             * @return a result
             */
            gbool get() const {
                if (!launcher) IllegalStateException().throws($ftrace());
                return launcher->invoke();
            }

            gbool equals(const Object &obj) const override {
                return this == &obj ||
                       Class<BooleanSupplier>::hasInstance(obj) &&
                       launcher == CORE_XCAST(BooleanSupplier const, obj).launcher;
            }

            Object &clone() const override {
                try {
                    return UNSAFE::newInstance<BooleanSupplier>(*this);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

        private:
            class Handle : public SharedHandle {
            public:
                virtual gbool invoke() const = 0;
            };

            template<class T, Status status>
            class Callable;

            template<class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f) {
                }

                gbool invoke() const override { return f(); }
            };

            template<class T>
            class Callable<T, OBJECT_REFERENCE> final : public Handle {
                T obj;

            public:
                CORE_EXPLICIT Callable(T obj): obj(obj) {
                }

                gbool invoke() const override { return obj(); }
            };
        };
    } // function
} // core

#endif //CORE24_BOOLEANSUPPLIER_H
