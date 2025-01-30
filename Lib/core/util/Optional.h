//
// Created by brunshweeck on 19/11/24.
//

#ifndef CORE24_OPTIONAL_H
#define CORE24_OPTIONAL_H

#include <core/Runnable.h>
#include <core/function/Consumer.h>
#include <core/function/Predicate.h>
#include <core/function/Supplier.h>
#include <core/misc/Unsafe.h>
#include <core/util/NoSuchElementException.h>
#include <core/misc/OptionalValue.h>

#include <core/ClassCastException.h>

namespace core {
    namespace util {
        /**
         * A container object which may or may not contain a non-null value.
         * If a value is present, @c isPresent() returns @c true. If no
         * value is present, the object is considered <i>empty</i> and
         * @c isPresent() returns @c false.
         *
         * <p>
         * Additional methods that depend on the presence or absence of a contained
         * value are provided, such as @b orElse()
         * (returns a default value if no value is present) and
         * @b ifPresent() (performs an action if a value is present).
         * </p>
         * <p>
         * This is a value-based class; programmers should treat instances that are
         * @b equal as interchangeable and should not
         * use instances for synchronization, or unpredictable behavior may
         * occur. For example, in a future release, synchronization may fail.
         * </p>
         * @note
         * @c Optional is primarily intended for use as a method return type where
         * there is a clear need to represent "no result," and where using @c null
         * is likely to cause errors. A variable whose type is @c Optional should
         * never itself be @c null; it should always point to an @c Optional
         * instance.
         *
         * @tparam T the type of value
         */
        template<class T>
        class Optional final : public misc::OptionalValue {
            CORE_ALIAS(VALUE, typename Class<T>::Pointer);
            CORE_ALIAS(VALUE2, typename Class<T const>::Pointer);

            CORE_ADD_TEMPLATES_AS_FRIEND(Optional);

            CORE_ADD_AS_FRIEND(regex::Pattern);

            CORE_FULL_CHECK_SLIMMED_TYPE(T)

            /**
             * If non-null, the value;
             * if null, indicates no value is present
             */
            VALUE value = {};

        public:
            /**
             * Constructs an instance of empty optional.
             */
            CORE_FAST Optional() = default;

            /**
             * Constructs an instance with the described value.
             *
             * @param value the value to describe
             */
            Optional(T const &value) {
                T &ref = UNSAFE::copyInstance(value);
                Optional::value = &ref;
            }

            template<class X,
                    ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            Optional(const Optional<X> &other): value(other.value) {
            }

            Optional(const Optional &other) : value(other.value) {
            }

            Optional(Optional &&other) CORE_NOTHROW {
                UNSAFE::swapValues(value, other.value);
            }

            template<class X,
                    ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            Optional &operator=(const Optional<X> &other) {
                value = other.value;
                return *this;
            }

            Optional &operator=(const Optional &other) {
                if (this != &other)
                    value = other.value;
                return *this;
            }

            Optional &operator=(Optional &&other) CORE_NOTHROW {
                if (this != &other)
                    UNSAFE::swapValues(value, other.value);
                return *this;
            }

            /**
             * If a value is present, returns the value, otherwise throws
             * @c NoSuchElementException.
             *
             * @note
             * The preferred alternative to this method is @b orElseThrow().
             *
             * @return the value described by this @c Optional
             */
            T &get() {
                if (!value)
                    NoSuchElementException().throws($ftrace());
                return *value;
            }

            /**
             * If a value is present, returns the value, otherwise throws
             * @c NoSuchElementException.
             *
             * @note
             * The preferred alternative to this method is @b orElseThrow().
             *
             * @return the value described by this @c Optional
             */
            T const &get() const {
                if (!value)
                    NoSuchElementException().throws($ftrace());
                return *value;
            }

            /**
             * If a value is present, returns @c true, otherwise @c false.
             *
             * @return @c true if a value is present, otherwise @c false
             */
            gbool isPresent() const {
                return value;
            }

            /**
             * If a value is  not present, returns @c true, otherwise
             * @c false.
             *
             * @return  @c true if a value is not present, otherwise @c false
             */
            gbool isEmpty() const {
                return !value;
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise does nothing.
             *
             * @param action the action to be performed, if a value is present
             */
            void ifPresent(function::Consumer<T &> const &action) {
                try {
                    if (value) action.accept(*value);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise does nothing.
             *
             * @param action the action to be performed, if a value is present
             */
            void ifPresent(function::Consumer<T> const &action) const {
                try {
                    if (value) action.accept(*value);
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise performs the given empty-based action.
             *
             * @param action the action to be performed, if a value is present
             * @param emptyAction the empty-based action to be performed, if no value is
             *        present
             */
            void IfPresentOrElse(function::Consumer<T &> const &action, Runnable const &emptyAction) {
                try {
                    if (isPresent())
                        return action.accept(*value);

                    emptyAction.run();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, performs the given action with the value,
             * otherwise performs the given empty-based action.
             *
             * @param action the action to be performed, if a value is present
             * @param emptyAction the empty-based action to be performed, if no value is
             *        present
             */
            void IfPresentOrElse(function::Consumer<T> const &action, Runnable const &emptyAction) const {
                try {
                    if (isPresent())
                        return action.accept(*value);

                    emptyAction.run();
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, and the value matches the given predicate,
             * returns an @c Optional describing the value, otherwise returns an
             * empty @c Optional.
             *
             * @param predicate the predicate to apply to a value, if present
             * @return an @c Optional describing the value of this
             *         @c Optional, if a value is present and the value matches the
             *         given predicate, otherwise an empty @c Optional
             */
            Optional filter(function::Predicate<T &> const &predicate) {
                try {
                    if (!isEmpty() && predicate.test(*value))
                        return *this;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                return Optional();
            }

            /**
             * If a value is present, and the value matches the given predicate,
             * returns an @c Optional describing the value, otherwise returns an
             * empty @c Optional.
             *
             * @param predicate the predicate to apply to a value, if present
             * @return an @c Optional describing the value of this
             *         @c Optional, if a value is present and the value matches the
             *         given predicate, otherwise an empty @c Optional
             */
            Optional filter(function::Predicate<T> const &predicate) const {
                try {
                    if (!isEmpty() && predicate.test(*value))
                        return *this;
                } catch (Throwable const &ex) { ex.throws($ftrace()); }

                return Optional();
            }

            /**
             * If a value is present, returns an @c Optional describing (as if by
             * @b ofNullable) the result of applying the given mapping function to
             * the value, otherwise returns an empty @c Optional.
             *
             * <p>
             * If the mapping function returns a @c null result then this method
             * returns an empty @c Optional.
             * </p>
             * @note
             * This method supports post-processing on @c Optional values, without
             * the need to explicitly check for a return status.  For example, the
             * following code traverses a stream of URIs, selects one that has not
             * yet been processed, and creates a path from that URI, returning
             * an @c Optional<Path>:
             *
             * @code
             *     Optional<Path> p =
             *         uris.stream().filter(uri -> !isProcessedYet(uri))
             *                       .findFirst()
             *                       .map(Paths::get);
             * @endcode
             *
             * Here, @c findFirst returns an @c Optional<URI>, and then
             * @c map returns an @c Optional<Path> for the desired
             * URI if one exists.
             *
             * @param mapper the mapping function to apply to a value, if present
             * @tparam U The type of the value returned from the mapping function
             * @return an @c Optional describing the result of applying a mapping
             *         function to the value of this @c Optional, if a value is
             *         present, otherwise an empty @c Optional
             */
            template<class U>
            Optional<U> map(function::Function<T &, U> const &mapper) {
                if (isEmpty())
                    return Optional<U>();

                try { return mapper.apply(*value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns an @c Optional describing (as if by
             * @b ofNullable) the result of applying the given mapping function to
             * the value, otherwise returns an empty @c Optional.
             *
             * <p>
             * If the mapping function returns a @c null result then this method
             * returns an empty @c Optional.
             * </p>
             * @note
             * This method supports post-processing on @c Optional values, without
             * the need to explicitly check for a return status.  For example, the
             * following code traverses a stream of URIs, selects one that has not
             * yet been processed, and creates a path from that URI, returning
             * an @c Optional<Path>:
             *
             * @code
             *     Optional<Path> p =
             *         uris.stream().filter(uri -> !isProcessedYet(uri))
             *                       .findFirst()
             *                       .map(Paths::get);
             * @endcode
             *
             * Here, @c findFirst returns an @c Optional<URI>, and then
             * @c map returns an @c Optional<Path> for the desired
             * URI if one exists.
             *
             * @param mapper the mapping function to apply to a value, if present
             * @tparam U The type of the value returned from the mapping function
             * @return an @c Optional describing the result of applying a mapping
             *         function to the value of this @c Optional, if a value is
             *         present, otherwise an empty @c Optional
             */
            template<class U>
            Optional<U> map(function::Function<T, U> const &mapper) const {
                if (isEmpty())
                    return Optional<U>();

                try { return mapper.apply(*value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns the result of applying the given
             * @c Optional-bearing mapping function to the value, otherwise returns
             * an empty @c Optional.
             *
             * <p>This method is similar to @b map(Function), but the mapping
             * function is one whose result is already an @c Optional, and if
             * invoked, @c flatMap does not wrap it within an additional
             * @c Optional.
             *
             * @tparam U The type of value of the @c Optional returned by the
             *            mapping function
             * @param mapper the mapping function to apply to a value, if present
             * @return the result of applying an @c Optional-bearing mapping
             *         function to the value of this @c Optional, if a value is
             *         present, otherwise an empty @c Optional
             */
            template<class U>
            Optional<U> flatMap(function::Function<T &, Optional<U> > const &mapper) {
                if (isEmpty())
                    return Optional<U>();

                try { return mapper.apply(*value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns the result of applying the given
             * @c Optional-bearing mapping function to the value, otherwise returns
             * an empty @c Optional.
             *
             * <p>This method is similar to @b map(Function), but the mapping
             * function is one whose result is already an @c Optional, and if
             * invoked, @c flatMap does not wrap it within an additional
             * @c Optional.
             *
             * @tparam U The type of value of the @c Optional returned by the
             *            mapping function
             * @param mapper the mapping function to apply to a value, if present
             * @return the result of applying an @c Optional-bearing mapping
             *         function to the value of this @c Optional, if a value is
             *         present, otherwise an empty @c Optional
             */
            template<class U>
            Optional<U> flatMap(function::Function<T, Optional<U> > const &mapper) const {
                if (isEmpty())
                    return Optional<U>();

                try { return mapper.apply(*value); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns an @c Optional describing the value,
             * otherwise returns an @c Optional produced by the supplying function.
             *
             * @param supplier the supplying function that produces an @c Optional
             *        to be returned
             * @return returns an @c Optional describing the value of this
             *         @c Optional, if a value is present, otherwise an
             *         @c Optional produced by the supplying function.
             */
            Optional orElse(function::Supplier<Optional> const &supplier) {
                if (isPresent())
                    return *this;

                try { return supplier.get(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns an @c Optional describing the value,
             * otherwise returns an @c Optional produced by the supplying function.
             *
             * @param supplier the supplying function that produces an @c Optional
             *        to be returned
             * @return returns an @c Optional describing the value of this
             *         @c Optional, if a value is present, otherwise an
             *         @c Optional produced by the supplying function.
             */
            Optional orElse(function::Supplier<Optional> const &supplier) const {
                if (isPresent())
                    return *this;

                try { return supplier.get(); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns the value, otherwise returns
             * @c other.
             *
             * @param other the value to be returned, if no value is present.
             *        May be @c null.
             * @return the value, if present, otherwise @c other
             */
            T &orElse(T const &other) {
                if (isPresent())
                    return *value;

                try { return UNSAFE::copyInstance(other); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns the value, otherwise returns
             * @c other.
             *
             * @param other the value to be returned, if no value is present.
             *        May be @c null.
             * @return the value, if present, otherwise @c other
             */
            T const &orElse(T const &other) const {
                if (isPresent())
                    return *value;

                try { return UNSAFE::copyInstance(other); } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns the value, otherwise returns the result
             * produced by the supplying function.
             *
             * @param supplier the supplying function that produces a value to be returned
             * @return the value, if present, otherwise the result produced by the
             *         supplying function
             */
            T &orElseGet(function::Supplier<T> const &supplier) {
                if (isPresent())
                    return *value;

                try {
                    return UNSAFE::copyInstance(supplier.get());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns the value, otherwise returns the result
             * produced by the supplying function.
             *
             * @param supplier the supplying function that produces a value to be returned
             * @return the value, if present, otherwise the result produced by the
             *         supplying function
             */
            T const &orElseGet(function::Supplier<T> const &supplier) const {
                if (isPresent())
                    return *value;

                try {
                    return UNSAFE::copyInstance(supplier.get());
                } catch (Throwable const &ex) { ex.throws($ftrace()); }
            }

            /**
             * If a value is present, returns the value, otherwise throws
             * @c NoSuchElementException.
             *
             * @return the value described by this @c Optional
             * @since 10
             */
            T &orElseThrow() {
                if (isPresent())
                    return *value;

                NoSuchElementException().throws($ftrace());
            }

            /**
             * If a value is present, returns the value, otherwise throws
             * @c NoSuchElementException.
             *
             * @return the value described by this @c Optional
             * @since 10
             */
            T const &orElseThrow() const {
                if (isPresent())
                    return *value;

                NoSuchElementException().throws($ftrace());
            }

            /**
             * If a value is present, returns the value, otherwise throws an exception
             * produced by the exception supplying function.
             *
             * @note
             * A method reference to the exception constructor with an empty argument
             * list can be used as the supplier.
             *
             * @tparam X Type of the exception to be thrown
             * @param exceptionSupplier the supplying function that produces an
             *        exception to be thrown
             * @return the value, if present
             * @throws X if no value is present
             */
            template<class X,
                    ClassOf(1)::OnlyIf<Class<Throwable>::isSuper<X>()> CaptureX = 1>
            T &orElseThrow(function::Supplier<X> const &exceptionSupplier) {
                if (!isPresent())
                    exceptionSupplier.get().throws($ftrace());

                return *value;
            }

            /**
             * If a value is present, returns the value, otherwise throws an exception
             * produced by the exception supplying function.
             *
             * @note
             * A method reference to the exception constructor with an empty argument
             * list can be used as the supplier.
             *
             * @tparam X Type of the exception to be thrown
             * @param exceptionSupplier the supplying function that produces an
             *        exception to be thrown
             * @return the value, if present
             * @throws X if no value is present
             */
            template<class X,
                    ClassOf(1)::OnlyIf<Class<Throwable>::isSuper<X>()> CaptureX = 1>
            T const &orElseThrow(function::Supplier<X> const &exceptionSupplier) const {
                if (!isPresent())
                    exceptionSupplier.get().throws($ftrace());

                return *value;
            }

            /**
             * Indicates whether some other object is "equal to" this @c Optional.
             * The other object is considered equal if:
             * <ul>
             * <li>it is also an @c Optional and;
             * <li>both instances have no value present or;
             * <li>the present values are "equal to" each other via @c equals().
             * </ul>
             *
             * @param obj an object to be tested for equality
             * @return @c true if the other object is "equal to" this object
             *         otherwise @c false
             */
            gbool equals(const Object &obj) const override {
                return this == &obj || OptionalValue::equals(obj);
            }

            /**
             * Returns the hash code of the value, if present, otherwise @c 0
             * (zero) if no value is present.
             *
             * @return hash code value of the present value or @c 0 if no value is
             *         present
             */
            gint hash() const override {
                if (isEmpty())
                    return 0;
                return (*value).hash();
            }

            /**
             * Returns a non-empty string representation of this @c Optional
             * suitable for debugging.  The exact presentation format is unspecified and
             * may vary between implementations and versions.
             *
             * @implSpec
             * If a value is present the result must include its string representation
             * in the result.  Empty and present @c Optionals must be unambiguously
             * differentiable.
             *
             * @return the string representation of this instance
             */
            String toString() const override {
                if (isEmpty())
                    return "Optional.empty";

                return "Optional[" + String::valueOf(*value) + "]";
            }

            Object &clone() const override {
                return UNSAFE::newInstance<Optional>(*this);
            }

            ~Optional() override {
                value = {};
            }

            template<class X, ClassOf(1)::OnlyIf<Class<X>::template isExtends<T>()> CaptureX = 1>
            CORE_EXPLICIT operator Optional<X>() const {
                CORE_ALIAS(XVALUE, typename Optional<X>::VALUE);
                if (isEmpty())
                    return Optional<X>();
                Optional<X> result = {};
                result.value = CORE_DCAST(XVALUE, value);
                if (result.isPresent())
                    return result;
                ClassCastException("Couldn't cast instance of type " + typeName(*this)
                                   + " to " + typeName(Optional<X>())).throws($ftrace());
            }

            // VALUE operator->() { return &get(); }
            //
            // VALUE2 operator->() const { return &get(); }
            //
            // T& operator*() { return get(); }
            //
            // T const& operator*() const { return get(); }

            static CORE_FAST Optional empty() { return Optional(); }

        private:
            Object &getContent() override { return get(); }

            const Object &getContent() const override { return get(); }

            gbool hasContent() const override { return isPresent(); }
        };
    } // util
} // core

#endif //CORE24_OPTIONAL_H
