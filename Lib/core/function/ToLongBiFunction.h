//
// Created by brunshweeck on 27 juin 2024.
//

#ifndef CORE24_TOLONGBIFUNCTION_H
#define CORE24_TOLONGBIFUNCTION_H

#include <core/AssertionError.h>
#include <core/Long.h>
#include <core/IllegalArgumentException.h>
#include <core/function/Functional.h>

#include <core/misc/Unsafe.h>

namespace core
{
    namespace function
    {

        /**
         * Represents a function that accepts two arguments and produces a long-valued
         * result.  This is the @c long - producing primitive specialization for
         * @c BiFunction.
         * <p>
         * This is a functional interface whose functional method is @c ToLongBiFunction::apply.
         * </p>
         *
         * @tparam T the type of the first argument to the function
         * @tparam U the type of the second argument to the function
         */
        template <class T, class U>
        class ToLongBiFunction : public virtual Functional
        {
            CORE_FULL_CHECK_TYPE(T);
            CORE_FULL_CHECK_TYPE(U);

            CORE_ALIAS(VH, misc::Unsafe);

            CORE_FAST_ASSERT(!Class<Void>::isSame<T>());
            CORE_FAST_ASSERT(!Class<Void>::isSame<U>());

            /**
             * The type of argument value accepted by
             * @c Function instance of type T.
             */
            CORE_ALIAS(A,) Arg<T>;
            CORE_ALIAS(B,) Arg<U>;

            class Handle
            {
            public:
                virtual ~Handle() = default;
                gint volatile counter = 0;

                virtual gbool equals(Handle const &o) const { return this == &o; }

                virtual glong invoke(A a, B b) const CORE_NOTHROW = 0;
            };

            template <class C, Status status>
            class Callable;

            CORE_ALIAS(CallableHandle, typename Class<Handle>::Pointer);

            CallableHandle handle = null;

        protected:
            CORE_IMPLICIT ToLongBiFunction() CORE_NOTHROW
            {
                CORE_IGNORE(this);
            }

        public:
            /**
             * Construct new instance of @c ToLongBiFunction with specified callable
             * object. The given callable may be a <em> unary function pointer,
             * lambda function, object that implement operator() or function
             * members </em>.
             *
             * @note the given callable object must be callable with @c T
             *
             * @tparam C the type of callable
             * @tparam CaptureArg the value used to ensure that the given callable
             *                     accept argument supported by this type of @c ToLongBiFunction.
             * @tparam CaptureRet the value used to ensure that the given callable
             *                     return the value convertible to long value.
             * @param callable the callable object.
             */
            template <class C,
                      ClassOf(1)::OnlyIfAll<Class<C>::template isCallable<A, B>()> CaptureArg = 1,
                      class R2 = typename Class<C>::template Return<A, B>,
                      ClassOf(1)::OnlyIfOne<Class<R2>::template isConvertible<Prime<Long>>()> CaptureRet = 1>
            CORE_IMPLICIT ToLongBiFunction(C&& callable): ToLongBiFunction()
            {
                if (!Checker<C>::isValid(callable))
                {
                    IllegalArgumentException().throws($ftrace());
                }

                CORE_FAST Status status = ClassOf(callable)::isToLongBiFunctionMember()
                                              ? METHOD_REFERENCE
                                              : ClassOf(callable)::isPointer()
                                                    ? FUNCTION_REFERENCE
                                                    : OBJECT_REFERENCE;

                handle = new Callable<C, status>(Functional::forward<C>(callable));
            }

            /**
             *
             * @tparam O the class supporting specified non-static method reference.
             * @tparam M the type of method signature.
             * @tparam CaptureArg the value used to ensure that the given callable
             *                     accept arguments supported by this type of @c ToLongBiFunction.
             * @tparam CaptureRet the value used to ensure that the given callable
             *                     return the value convertible to long value.
             * @param object the instance used call specified method
             * @param method the reference of method support by class @c O
             */
            template <class O, class M,
                      ClassOf(1)::OnlyIfAll<Class<O>::isClass(), Class<M>::isFunctionMember(),
                                            Class<M>::template isCallable<O, A, B>()> CaptureArg = 1,
                      class R2 = typename Class<M>::template Return<O, A, B>,
                      ClassOf(1)::OnlyIfOne<Class<R2>::template isConvertible<Prime<Long>>()> CaptureRet = 1>
            CORE_IMPLICIT ToLongBiFunction(O&& object, M method): ToLongBiFunction()
            {
                class MethodHandle final : public Handle
                {
                    O object;
                    M method;

                public:
                    MethodHandle(O object, M method) : object(object), method(method)
                    {
                    }

                    glong invoke(A a, B b) const CORE_NOTHROW override
                    {
                        return CORE_CAST(object.*method, a, b);
                    }

                    gbool equals(const Handle& o) const override
                    {
                        if (this == &o)
                            return true;
                        if (Class<MethodHandle>::hasInstance(o))
                        {
                            MethodHandle const& h = CORE_XCAST(MethodHandle const, o);
                            return (&object == &h.object) && Checker<M>::isEquals(method, h.method);
                        }
                        return false;
                    }
                };

                handle = new MethodHandle(object, method);
            }

            /**
             * Construct new instance of @c ToLongBiFunction with another consumer.
             *
             * @param other the consumer.
             */
            CORE_IMPLICIT ToLongBiFunction(ToLongBiFunction const& other) CORE_NOTHROW : ToLongBiFunction()
            {
                if (this != &other)
                {
                    handle = other.handle;
                    if (handle != null)
                        handle->counter -= 1;
                }
            }

            /**
             * Destroy this instance.
             */
            ~ToLongBiFunction() CORE_NOTHROW override
            {
                if (handle != null)
                {
                   const gint counter = handle->counter++;
                    if (counter == 0)
                    {
                        CallableHandle handle2 = handle;
                        handle = null;
                        delete handle2;
                    }
                }
            }

            /**
             * Performs this operation on the given arguments
             * values.
             *
             * @param a the first input argument.
             * @param b the second input argument.
             * @return the function result
             */
            virtual glong apply(A a, B b) const  CORE_NOTHROW
            {
                if (handle != null)
                {
                    Handle const& callable = *handle;
                    return callable.invoke(a, b);
                }
                AssertionError(false).throws($ftrace());
            }

            gbool equals(const Object& obj) const override
            {
                if (this == &obj)
                {
                    return true;
                }
                if (Class<ToLongBiFunction>::hasInstance(obj))
                {
                    ToLongBiFunction const& function = CORE_XCAST(ToLongBiFunction const, obj);
                    if(handle == null)
                    {
                        return function.handle == null;
                    }
                    if(function.handle == null)
                    {
                        return false;
                    }
                    Handle &handle1 = *handle;
                    Handle &handle2 = *function.handle;
                    return handle1.equals(handle2);
                }
                return false;
            }

            Object& clone() const override
            {
                try
                {
                    return VH::newInstance<ToLongBiFunction>(*this);
                }
                catch (Throwable const& ex)
                {
                    ex.throws($ftrace());
                }
            }

        private:
            template <class Fn>
            class Callable<Fn, FUNCTION_REFERENCE> final : public Handle
            {
                Fn f;

            public:
                CORE_EXPLICIT Callable(Fn f): f(f)
                {
                }

                glong invoke(A a, B b) const CORE_NOTHROW override
                {
                    return f(a, b);
                }

                gbool equals(const Handle& obj) const override
                {
                    return (this == &obj)
                        || Class<Callable>::hasInstance(obj)
                        && f == CORE_XCAST(Callable const, obj).f;
                }
            };

            template <class O>
            class Callable<O, OBJECT_REFERENCE> final : public Handle
            {
                O obj;

            public:
                CORE_EXPLICIT Callable(O obj): obj(obj)
                {
                }

                glong invoke(A a, B b) const CORE_NOTHROW override
                {
                    return obj(a, b);
                }

                gbool equals(const Handle& o) const override
                {
                    return (this == &o) ||
                        Class<Callable>::hasInstance(o)
                        && Checker<O>::isEquals(obj, CORE_XCAST(Callable const, o).obj);
                }
            };

            template <class M>
            class Callable<M, METHOD_REFERENCE> final : public Handle
            {
                M m;

            public:
                CORE_EXPLICIT Callable(M m) : m(m)
                {
                }

                glong invoke(A a, B b) const CORE_NOTHROW override
                {
                    return (a.*m)(b);
                }

                gbool equals(const Handle& o) const override
                {
                    return (this == &o) ||
                        Class<Callable>::hasInstance(o)
                        && m == CORE_XCAST(Callable const, o).m;
                }
            };
        };
    } // function
} // core

#endif //CORE24_TOLONGBIFUNCTION_H
