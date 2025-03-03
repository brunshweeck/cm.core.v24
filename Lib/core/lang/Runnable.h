//
// Created by bruns on 20/09/2024.
//

#ifndef CORE24_RUNNABLE_H
#define CORE24_RUNNABLE_H

#include <core/function/Functional.h>

namespace core {
    /**
     * Represents an operation that does not return a result.
     *
     * <p>
     * This is a <em> functional interface</em>
     * whose functional method is @b run().
     */
    class Runnable : public function::Functional {
        class Handle;

        CORE_ALIAS(Executor, Class<Handle>::Pointer);

        Executor executor = {};

    public:
        /**
         * Construct new Runnable object that do nothing.
         */
        CORE_IMPLICIT Runnable() = default;

        /**
         * Create new Runnable with specified event
         * @tparam Event the type event
         * @param event the event executed by this runnable
         */
        template<class Event,
            ClassOf(1)::OnlyIf<Class<Event>::isCallable()>  = 1>
        CORE_IMPLICIT Runnable(Event &&event) {
            executor = new Launcher<Event, findStatus<Event>()>(event);
        }

        /**
         * Create new Runnable with event defined by class method
         * @param var the class instance
         * @param method the class method.
         */
        template<class Var, class Method,
            ClassOf(1)::OnlyIf<Class<Var>::isClass()> CaptureVar  = 1,
            ClassOf(1)::OnlyIf<Class<Method>::isFunctionMember()> CaptureMethod  = 1,
            ClassOf(1)::OnlyIf<Class<Method>::template isCallable<Var>()> CaptureArgs  = 1>
        CORE_IMPLICIT Runnable(Var &&var, Method method) {
            class MethodLauncher : public Handle {
                Var &&var;
                Method method;

            public:
                CORE_EXPLICIT MethodLauncher(Var &&var, Method method)
                    : var(var), method(method) {
                }

                void execute() const override { (var.*method)(); }
            };

            executor = new MethodLauncher(var, method);
        }

        CORE_IMPLICIT Runnable(const Runnable &other);

        ~Runnable() override;

        Runnable(Runnable &&other) CORE_NOTHROW;

        Runnable &operator=(const Runnable &other);

        Runnable &operator=(Runnable &&other) CORE_NOTHROW;

        /**
         * Runs this operation.
         */
        virtual void run() const;

        gbool equals(const Object &other) const override;

        Object &clone() const override;

    private:
        class Handle : public SharedHandle {
        public:
            virtual void execute() const = 0;
        };

        template<class Event, Status status>
        class Launcher;

        template<class Func>
        class Launcher<Func, FUNCTION_REFERENCE> final : public Handle {
            Func func;

        public:
            CORE_EXPLICIT Launcher(Func func) : func(func) {
            }

            void execute() const override { func(); }
        };

        template<class Obj>
        class Launcher<Obj, OBJECT_REFERENCE> final : public Handle {
            Obj obj;

        public:
            CORE_EXPLICIT Launcher(Obj obj): obj(obj) {
            }

            void execute() const override { obj(); }
        };
    };
} // core

#endif //CORE24_RUNNABLE_H
