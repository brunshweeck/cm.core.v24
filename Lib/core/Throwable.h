//
// Created by bruns on 14/05/2024.
//

#ifndef CORE24_THROWABLE_H
#define CORE24_THROWABLE_H

#include <core/String.h>
#include <core/Trace.h>

namespace core {
    /**
     * The @c Throwable class is the superclass of all errors and
     * exceptions supported by Core24 platform. Only objects that are instances of this
     * class (or one of its subclasses) can be thrown by the @c throws and @c rethrow method. 
     * Similarly, only this class or one of its subclasses can be the argument type in a
     * @c catch clause after operation defined in this platform.
     * <p>
     * For the purposes of compile-time checking of exceptions,
     * @c Throwable and any subclass of @c Throwable that is not also a
     * subclass of either @b RuntimeException or @b Error are
     * regarded as checked exceptions.
     * </p>
     * <p>
     * Instances of two subclasses, @b core::Error and
     * @b core::Exception, are conventionally used to indicate
     * that exceptional situations have occurred. Typically, these instances
     * are freshly created in the context of the exceptional situation so
     * as to include relevant information (such as stack trace data).
     * </p>
     * <p>
     * A throwable contains a snapshot of the execution stack of its
     * thread at the time it was created. It can also contain a message
     * string that gives more information about the error.  
     * Finally, the throwable can also
     * contain a <i>cause</i>: another throwable that caused this
     * throwable to be constructed.  The recording of this causal information
     * is referred to as the <i>chained exception</i> facility, as the
     * cause can, itself, have a cause, and so on, leading to a "chain" of
     * exceptions, each caused by another.
     * </p>
     * <p>
     * One reason that a throwable may have a cause is that the class that
     * throws it is built atop a lower layered abstraction, and an operation on
     * the upper layer fails due to a failure in the lower layer.  It would be bad
     * design to let the throwable thrown by the lower layer propagate outward, as
     * it is generally unrelated to the abstraction provided by the upper layer.
     * Further, doing so would tie the API of the upper layer to the details of
     * its implementation, assuming the lower layer's exception was a checked
     * exception.  Throwing a "wrapped exception" (i.e., an exception containing a
     * cause) allows the upper layer to communicate the details of the failure to
     * its caller without incurring either of these shortcomings.  It preserves
     * the flexibility to change the implementation of the upper layer without
     * changing its API (in particular, the set of exceptions thrown by its
     * methods).
     * </p>
     * <p>
     * A second reason that a throwable may have a cause is that the method
     * that throws it must conform to a general-purpose interface that does not
     * permit the method to throw the cause directly.  For example, suppose
     * a persistent collection conforms to the @b Collection
     * interface, and that its persistence is implemented atop
     * @c core::io.  Suppose the internals of the @c add method
     * can throw an @b IOException.  The implementation
     * can communicate the details of the @c IOException to its caller
     * while conforming to the @c Collection interface by wrapping the
     * @c IOException in an appropriate unchecked exception.  (The
     * specification for the persistent collection should indicate that it is
     * capable of throwing such exceptions.)
     * </p>
     * <p>
     * A cause can be associated with a throwable in two ways: via a
     * constructor that takes the cause as an argument, or via the
     * @b setCause(Throwable) method.  New throwable classes that
     * wish to allow causes to be associated with them should provide constructors
     * that take a cause and delegate (perhaps indirectly) to one of the
     * @c Throwable constructors that takes a cause.
     *
     * Because the @c setCause method is public, it allows a cause to be
     * associated with any throwable, even a "legacy throwable" whose
     * implementation predates the addition of the exception chaining mechanism to
     * @c Throwable.
     * </p>
     * <p>
     * By convention, class @c Throwable and its subclasses have two
     * constructors, one that takes no arguments and one that takes a
     * @c String argument that can be used to produce a detail message.
     * Further, those subclasses that might likely have a cause associated with
     * them should have two more constructors, one that takes a
     * @c Throwable (the cause), and one that takes a
     * @c String (the detail message) and a @c Throwable (the cause).
     */
    class Throwable : public virtual Object, public virtual std::exception {
        CORE_ALIAS(ORIGIN, Class<Throwable>::Pointer);
        CORE_ALIAS(ByteStream, Class<ClassOf('\0')::Const>::Pointer);
        CORE_ADD_GLOBAL_FRIENDS();

        /**
         * Specific details about the Throwable.  For example, for
         * @c FileNotFoundException, this contains the name of
         * the file that could not be found.
         */
        String detailMessage;

        /**
         * The throwable that caused this throwable to get thrown, or @a itself if this
         * throwable was not caused by another throwable, or if the causative
         * throwable is unknown.
         *
         */
        ORIGIN origin = 0;

        /**
         * The stacktrace, as returned by @c Throwable::stacktrace().
         *
         * The field is initialized to a zero-length array.
         * A @c {} value of this field indicates subsequent calls to @c setStackTrace(Array<Trace>).
         */
        Array<Trace> &backtrace;

        /**
         * The throwable state required during thrown
         * operation.
         */
        enum class Status { NEW, CLONE, READY } status = Status::NEW;

    protected:
        CORE_ALIAS(UNSAFE, misc::Unsafe);

    public:
        /**
         * Constructs a new throwable with @c "" as its detail message.
         * The cause is not initialized.
         */
        CORE_IMPLICIT Throwable();

        /**
         * Constructs a new throwable with the specified detail message.  The
         * cause is not initialized.
         *
         * @param message the detail message. The detail message is saved for
         *          later retrieval by the @c Throwable::message() method.
         */
        CORE_EXPLICIT Throwable(const String &message);

        /**
         * Constructs a new throwable with the specified detail message and
         * cause.
         *
         * @note that the detail message associated with @c cause is @a not automatically
         *       incorporated in this throwable's detail message.
         *
         * @param  message the detail message (which is saved for later retrieval
         *         by the @c Throwable::message() method).
         * @param  cause the cause (which is saved for later retrieval by the
         *         @c Throwable::cause() method).
         *
         */
        CORE_EXPLICIT Throwable(const String &message, Throwable const &cause);

        /**
         * Construct new throwable object instance with another.
         *
         * @param other the throwable object
         */
        CORE_IMPLICIT Throwable(Throwable const &other);

        /**
         * Returns the detail message string of this throwable.
         *
         * @return  the detail message string of this @c Throwable instance.
         */
        virtual String message() const;

        /**
         * Creates a localized description of this throwable. Subclasses may
         * override this method in order to produce a locale-specific message.
         * For subclasses that do not override this method, the default
         * implementation returns the same result as @c message().
         *
         * @return  The localized description of this throwable.
         *
         */
        virtual String localizedMessage() const;

        /**
         * Returns the cause of this throwable or @a itself if the
         * cause is nonexistent or unknown.  (The cause is the throwable that
         * caused this throwable to get thrown.)
         *
         * This implementation returns the cause that was supplied via one of
         * the constructors requiring a @c Throwable, or that was set after
         * creation.  While it is typically unnecessary to override this method, a subclass can override
         * it to return a cause set by some other means.  This is appropriate for
         * a "legacy chained throwable" that predates the addition of chained
         * exceptions to @c Throwable.  Note that it is @a not
         * necessary to override any of the @c dumpStackTrace methods,
         * all of which invoke the @c cause method to determine the
         * cause of a throwable.
         *
         * @return  the cause of this throwable or @a itself if the
         *          cause is nonexistent or unknown.
         */
        Throwable const &cause() const;

        /**
         * Initializes the <i>cause</i> of this throwable to the specified value.
         * (The cause is the throwable that caused this throwable to get thrown.)
         *
         * <p>
         * This method can be called at most once.  It is generally called from
         * within the constructor, or immediately after creating the
         * throwable.  If this throwable was created with @b Throwable(Throwable) or
         * @b Throwable(String,Throwable), this method cannot be called
         * even once.
         * </p>
         * <p>
         * An example of using this method on a legacy throwable type
         * without other support for setting the cause is:
         *
         * @code
         * try {
         *     lowLevelOp();
         * } catch (LowLevelException le) {
         *     HighLevelException().setCause(le).throws(...); // Legacy constructor
         * }
         * @endcode
         * </p>
         * @param  cause the cause (which is saved for later retrieval by the
         *         @b getCause() method.
         * @return  a reference to this @c Throwable instance.
         * @throws IllegalArgumentException if @c cause is this
         *         throwable.  (A throwable cannot be its own cause.)
         * @throws IllegalStateException if this throwable was
         *         created with @b Throwable(Throwable) or
         *         @b Throwable(String,Throwable), or this method has already
         *         been called on this throwable.
         */
        Throwable &setCause(Throwable const &cause);

        /**
         * Returns a short description of this throwable.
         * The result is the concatenation of:
         * @li the @a name of the class of this object
         * @li @c ": " (a colon and a space)
         * @li the result of invoking this object's @c localizedMessage() method
         *
         * If @c localizedMessage returns @c "", then just the class name
         * is returned.
         *
         * @return a string representation of this throwable.
         */
        String toString() const override;

        /**
         * Prints this throwable and its backtrace to the
         * standard error stream. This method prints a stack trace for this
         * @c Throwable object on the error output stream that is
         * the value of the field @c Console::err. The first line of
         * output contains the result of the @c toString() method for
         * this object.  Remaining lines represent data previously recorded by
         * the method @c fillInStackTrace(). The format of this
         * information depends on the implementation, but the following
         * example may be regarded as typical:
         * @code
         * core::RuntimeException
         *         at MyClass.mash(MyClass.cpp:9)
         *         at MyClass.crunch(MyClass.cpp:6)
         *         ...
         * @endcode
         *
         * The backtrace for a throwable with an initialized, cause
         * should generally include the backtrace for the cause.  The format
         * of this information depends on the implementation, but the following
         * example may be regarded as typical:
         * @code
         * HighLevelException: MidLevelException: LowLevelException
         *         at Junk.a(Junk.cpp:13)
         *         at main(Main.cpp:4)
         * Caused by: MidLevelException: LowLevelException
         *         at Junk.c(Junk.cpp:23)
         *         at Junk.b(Junk.cpp:17)
         *         at Junk.a(Junk.cpp:11)
         *         ... 1 more
         * Caused by: LowLevelException
         *         at Junk.e(Junk.cpp:30)
         *         at Junk.d(Junk.cpp:27)
         *         at Junk.c(Junk.cpp:21)
         *         ... 3 more
         * @endcode
         *
         * Note the presence of lines containing the characters @c "...".
         * These lines indicate that the remainder of the stack trace for this
         * exception matches the indicated number of frames from the bottom of the
         * stack trace of the exception that was caused by this exception (the
         * "enclosing" exception).  This shorthand can greatly reduce the length
         * of the output in the common case where a wrapped exception is thrown
         * from the same method as the "causative exception" is caught.  The above
         * example was produced by running the program:
         * @code
         * class Junk {
         *   public:
         *     static void a() throws HighLevelException {
         *         try { b(); } catch(MidLevelException const &ex) { HighLevelException(ex).throws($ftrace()); }
         *     }
         *     static void b() throws MidLevelException {
         *         try { c(); } catch(Throwable const &ex) { ex.throws($ftrace()); }
         *     }
         *     static void c() throws MidLevelException {
         *         try { d(); } catch(LowLevelException const &ex) { MidLevelException(ex).throws($ftrace()); }
         *     }
         *     static void d() throws LowLevelException {
         *        try { e(); } catch(Throwable const &ex) { ex.throws($ftrace()); }
         *     }
         *     static void e() {
         *         LowLevelException().throws($ftrace());
         *     }
         * }
         *
         * class HighLevelException: public virtual  Exception {
         *     HighLevelException(Throwable const &cause): Exception(cause) {}
         * }
         *
         * class MidLevelException: public virtual Exception {
         *     MidLevelException(Throwable const &cause): Exception(cause)  {}
         * }
         *
         * class LowLevelException: public virtual Exception {
         * }
         *
         * int main() {
         *    try { Junk::a(); } catch(Throwable const &ex) { ex.dumpStackTrace(); }
         * }
         * @endcode
         */
        void dumpStackTrace() const;

        /**
         * Provides programmatic access to the stack trace information printed by
         * @c dumpStackTrace().  Returns an array of stack trace elements,
         * each representing one stack frame.  The zeroth element of the array
         * (assuming the array's length is non-zero) represents the top of the
         * stack, which is the last method invocation in the sequence.  Typically,
         * this is the point at which this throwable was created and thrown.
         * The last element of the array (assuming the array's length is non-zero)
         * represents the bottom of the stack, which is the first method invocation
         * in the sequence.
         *
         * <p>Some virtual machines may, under some circumstances, omit one
         * or more stack frames from the stack trace.  In the extreme case,
         * a virtual machine that has no stack trace information concerning
         * this throwable is permitted to return a zero-length array from this
         * method.  Generally speaking, the array returned by this method will
         * contain one element for every frame that would be printed by
         * @c dumpStackTrace.  Writes to the returned array do not
         * affect future calls to this method.
         *
         * @return an array of stack trace elements representing the stack trace
         *         pertaining to this throwable.
         *
         */
        Array<Trace> stacktrace() const;

        /**
         * Thrown or rethrow this throwable at the specified execution point.
         *
         * @param currentExecutionPoint the current execution point
         */
        CORE_NORETURN void throws(Trace const &currentExecutionPoint) const CORE_XNOEXCEPT(0);

        /**
         * Thrown this throwable again with ignoring the current execution point.
         *
         * @note this method should be called after call of @c throws method, otherwise it thrown
         *       @c IllegalStateException
         *
         * @throws IllegalStateException if this method is not called after one first call of @c throws method
         */
        CORE_NORETURN void rethrow() const CORE_XNOEXCEPT(0);

        ~Throwable() override;

    protected:
        /**
         * Thrown the perfect and temporary copy of itself.
         *
         * Example: @code
         *  class MyClass: public virtual Exception
         *  {
         *  protected:
         *      void selfThrow() && override
         *      {
         *          throw MyClass(*this); // ok
         *          // or throw *this; // (deprecated)
         *      }
         *  };
         * @endcode
         *
         * @note All subclass must be override this method to
         *       preserve perfect chaining and this method use
         *       the c++ native @c throw for compatibility with
         *       standard exception.
         */
        CORE_NORETURN virtual void selfThrow() const = 0;

    private:
        /**
         * Print the stacktrace of this throwable on C-String stream.
         *
         * @note this method is only called by standard exception class
         *       during throwing operation.
         *
         * @return stack trace printed into the C-String stream.
         */
        ByteStream what() const CORE_NOTHROW final;

        /**
         * Read stacktrace of current thread
         *
         * @return the stacktrace of current thread
         */
        static Array<Trace> &initStacktrace();
    };
} // core

#endif // CORE24_THROWABLE_H
