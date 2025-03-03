//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {
    /**
     * <em>Functional interfaces</em> provide target types for lambda expressions
     * and method references.  Each functional interface has a single abstract
     * method, called the <em>functional method</em> for that functional interface,
     * to which the lambda expression's parameter and return types are matched or
     * adapted.  Functional interfaces can provide a target type in multiple
     * contexts, such as assignment context, method invocation, or cast context:
     *
     * @code
     *     // Assignment context
     *     Predicate<String> p = [](auto& s) { return s.isEmpty(); };
     *
     *     // Method invocation context
     *     stream.filter([](auto& e) { return e.size() > 10; })...
     *
     *     // Cast context
     *     stream.map((ToIntFunction) [](auto& e) { return e.size(); })...
     * @endcode
     *
     * <p>
     * The interfaces in this package are general purpose functional interfaces
     * used by the JDK, and are available to be used by user code as well.  While
     * they do not identify a complete set of function shapes to which lambda
     * expressions might be adapted, they provide enough to cover common
     * requirements. Other functional interfaces provided for specific purposes,
     * such as @b io::FileFilter, are defined in the packages where they
     * are used.
     * </p>
     *
     * <p>
     * Functional interfaces often represent concepts like functions,
     * actions, or predicates.  In documenting functional interfaces, or referring
     * to variables typed as functional interfaces, it is common to refer directly
     * to those abstract concepts, for example using "this function" instead of
     * "the function represented by this object".  When an API method is said to
     * accept or return a functional interface in this manner, such as "applies the
     * provided function to...", this is understood to mean a <i>non-null</i>
     * reference to an object implementing the appropriate functional interface,
     * unless potential nullity is explicitly specified.
     * </p>
     * <p>
     * The functional interfaces in this package follow an extensible naming
     * convention, as follows: <br/>
     *  - There are several basic function shapes, including
     *     @b function::Function (unary function from @c T to @c R),
     *     @b function::Consumer (unary function from @c T to @c void),
     *     @b function::Predicate (unary function from @c T to @c bool),
     *     and @b function::Supplier (nullary function to @c R).
     *     <br/>
     *
     *  - Function shapes have a natural arity based on how they are most
     *     commonly used.  The basic shapes can be modified by an arity prefix to
     *     indicate a different arity, such as
     *     @b function::BiFunction (binary function from @c T and
     *     @c U to @c R).
     *     <br/>
     *
     *  - There are additional derived function shapes which extend the basic
     *     function shapes, including @b function::UnaryOperator
     *     (extends @c Function) and @b function::BinaryOperator
     *     (extends @c BiFunction).
     *     <br/>
     *
     *  - Type parameters of functional interfaces can be specialized to
     *     primitives with additional type prefixes.  To specialize the return type
     *     for a type that has both generic return type and generic arguments, we
     *     prefix @c ToXxx, as in @b function::ToIntFunction.
     *     Otherwise, type arguments are specialized left-to-right, as in
     *     @b function::DoubleConsumer
     *     or @b function::ObjIntConsumer.
     *     (The type prefix @c Obj is used to indicate that we don't want to
     *     specialize this parameter, but want to move on to the next parameter,
     *     as in @b function::ObjIntConsumer.)
     *     These schemes can be combined, as in @c IntToDoubleFunction.
     *     <br/>
     *
     *  - If there are specialization prefixes for all arguments, the arity
     *     prefix may be left out (as in @b function::ObjIntConsumer).
     *     <br/>
     */
    namespace function {}
}

#endif //NAMESPACE_DOC_H
