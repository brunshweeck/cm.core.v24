//
// Created by bruns on 14/05/2024.
//

#ifndef CORE24_TRACE_H
#define CORE24_TRACE_H

#include <core/lang/String.h>

namespace core {
  class Trace final : public Object {
    CORE_ALIAS(UNSAFE, misc::Unsafe);

    String module;
    String version;
    String declaringClass;
    String method;
    String file;
    gint line = 0;

  public:
    /**
     * Creates a stack trace element representing the specified execution point.
     *
     * @param moduleName the module name if the class containing the execution point
     *                   represented by the trace is in a named module, otherwise @c "".
     * @param moduleVersion the module version if the class containing the execution point
     *                   represented by the trace is in a named module that has a version;
     *                   otherwise @c "".
     * @param className the fully qualified name of the class containing the execution
     *                   point represented by the trace.
     * @param methodName the name of the method containing the execution point represented
     *                   by the trace.
     * @param fileName the name of the file containing the execution point represented by
     *                   the trace, or @c "" if this information is unavailable.
     * @param lineNumber the line number of the source line containing the execution point
     *                   represented by this trace, or a negative number if this information
     *                   is unavailable. A value of -2 indicates that the method containing
     *                   the execution point is a native method.
     */
    CORE_EXPLICIT Trace(String const& moduleName, String const& moduleVersion,
                        String const& className, String const& methodName,
                        String const& fileName, gint lineNumber);

    /**
     * Creates a stack trace element representing the specified execution point.
     *
     * @param className the fully qualified name of the class containing the execution
     *                   point represented by the trace.
     * @param methodName the name of the method containing the execution point represented
     *                   by the trace.
     * @param fileName the name of the file containing the execution point represented by
     *                   the trace, or @c "" if this information is unavailable.
     * @param lineNumber the line number of the source line containing the execution point
     *                   represented by this trace, or a @a negative number if this information
     *                   is unavailable. A value of @c -2 indicates that the method containing
     *                   the execution point is a native method.
     */
    CORE_EXPLICIT Trace(String const& className, String const& methodName,
                        String const& fileName, gint lineNumber);

    /**
     * Creates a stack trace element representing the specified execution point.
     *
     * @note This constructor is used for @a statics functions and @a lambdas functions.
     *       is not recommaned to use on @a class methods (static or not)
     *
     * @param functionName the fully name of the function containing the execution point represented
     *                   by the trace (use macro CORE_FUNCTION_SIGNATURE).
     * @param fileName the name of the file containing the execution point represented by
     *                   the trace, or @c "" if this information is unavailable.
     * @param lineNumber the line number of the source line containing the execution point
     *                   represented by this trace, or a negative number if this information
     *                   is unavailable. A value of -2 indicates that the method containing
     *                   the execution point is a native method.
     */
    CORE_EXPLICIT Trace(String const& functionName, String const& fileName, gint lineNumber);

    /**
     * Returns the module name of the module containing the execution point
     * represented by this trace.
     *
     * @return the name of the @c module containing the execution
     *         point represented by this stack trace element; @c ""
     *         if the module name is not available.
     */
    String moduleName() const;

    /**
     * Returns the module version of the module containing the execution point
     * represented by this trace.
     *
     * @return the version of the @c module containing the execution
     *         point represented by this stack trace element; @c ""
     *         if the module version is not available.
     */
    String moduleVersion() const;

    /**
     * Returns the fully qualified name of the class containing the
     * execution point represented by this trace.
     *
     * @return the fully qualified name of the @c class containing
     *         the execution point represented by this stack trace element.
     */
    String className() const;

    /**
     * Returns the name of the method containing the execution point
     * represented by this trace.
     * @li If the execution point is contained in an instance or class initializer,
     *     this method will return the appropriate @a special method name, @a <ctor>.
     * @li If the execution point is contained in a class destroyer,
     *     this method will return the appropriate @a special method name, @a <dtor>.
     * @li If the execution point is contained in a relationanal or equality operators,
     *     this method will return the appropriate @a special method name, @a <not>, @a <eq>, @a <ne>,
     *     @a <lt>, @a <le>, @a <gt> or @a <ge>.
     * @li etc.
     *
     * @return the name of the method containing the execution point
     *         represented by this stack trace element.
     */
    String methodName() const;

    /**
     * Returns the name of the function containing the execution point
     * represented by this trace.
     * @li If the execution point is contained in a relationanal or equality operators,
     *     this method will return the appropriate @a special method name, @a <not>, @a <eq>, @a <ne>,
     *     @a <lt>, @a <le>, @a <gt> or @a <ge>.
     * @li etc.
     *
     * @return the name of the function containing the execution point
     *         represented by this stack trace element.
     */
    String functionName() const;

    /**
     * Returns the name of the source file containing the execution point
     * represented by this trace.  Generally, this corresponds
     * to the @a SourceFile attribute of the relevant @c class
     * file.  In some systems, the name may refer to some source code unit
     * other than a file, such as an entry in source repository.
     *
     * @return the name of the file containing the execution point
     *         represented by this stack trace element, or @c "" if
     *         this information is unavailable.
     */
    String fileName() const;

    /**
     * Returns the line number of the source line containing the execution
     * point represented by this trace.  Generally, this is
     * derived from the @c LineNumberTable attribute of the relevant
     * @c class file.
     *
     * @return the line number of the source line containing the execution
     *         point represented by this stack trace element, or a negative
     *         number if this information is unavailable.
     */
    gint lineNumber() const;

    /**
     * Returns true if the method containing the execution point
     * represented by this stack trace element is a native method.
     *
     * @return @c true if the method containing the execution point
     *         represented by this stack trace element is a native method.
     */
    gbool isNativeMethod() const;

    /**
     * Returns a string representation of this stack trace element.
     *
     * @apiNote The format of this string depends on the implementation, but the
     * following examples may be regarded as typical:
     * @li @c "foo@9.0/com::foo::Main.run(Main.cpp:101)"
     * - See the description below.
     * @li @c "com.foo.loader/foo@9.0/com::foo::Main.run(Main.cpp)"
     * - The line number is unavailable.
     * @li @c "foo@9.0/com.foo.Main.run(?)"
     * - Neither the file name nor the line number is available.
     * @li @c "foo@9.0/com::foo::Main.run(#)"
     * - The method containing the execution point is a native method.
     * @li @c "com::foo::bar::App.run(App.cpp:12)"
     * - The class of the execution point is defined in the unnamed module.
     * @li @c "acme@2.1/org::acme.Lib.test(Lib.cpp:80)"
     * - The class of the execution point is defined in @c acme module.
     * @li @c "MyClass.mash(MyClass.cpp:9)"
     * - @c MyClass class is on the application class path.
     *
     * The first example shows a stack trace element consisting of
     * three elements, each separated by @c "/", followed by
     * the source file name and the line number of the source line
     * containing the execution point.
     *
     * The first element @c "foo@9.0" is the module name and version.
     * The seconds element is the method containing the execution point;
     * @c "com.foo.Main"" is the fully-qualified class name and "@c run"
     * is the name of the method. @c "Main.cpp" is the source file name
     * and @c "101" is the line number.
     *
     * If a class is defined in an <em>unnamed module</em>
     * then the second element is omitted as shown in
     * @c "com::foo::bar::App.run(App.cpp:12)".
     *
     * If the module is an unnamed module, the second element
     * and its following @c "/" are also omitted as shown in
     * @c "MyClass.mash(MyClass.cpp:9)".
     *
     * The @c toString method may return two different values on two
     * @c Trace instances that are @a equal, for example one created via the
     * constructor, and one obtained from @c Throwable, where an implementation may
     * choose to omit some element in the returned string.
     */
    String toString() const override;

    /**
     * Returns true if the specified object is another @c Trace instance
     * representing the same execution point as this instance.  Two stack
     * trace @c a and @c b are equal if and only if:
     * @code
     *     a.moduleName().equals(b.moduleName()) &&
     *     a.moduleVersion().equals(b.moduleVersion()) &&
     *     a.className().equals(b.className()) &&
     *     a.methodName().equals(b.methodName()) &&
     *     a.fileName().equals(b.fileName()) &&
     *     a.lineNumber() == b.lineNumber()
     * @endcode
     *
     * @param  obj the object to be compared with this stack trace element.
     * @return @c true if the specified object is another @c Trace instance
     *         representing the same execution point as this instance.
     */
    gbool equals(Object const& obj) const override;

    /**
     * Returns a hash code value for this trace.
     */
    gint hash() const override;

  private:
    /**
     * Check, simplify and resolve method name
     *
     * @param mc the name of method/class to be resolved
     * @return the resolved method name
     */
    static String resolve(String const& mc);

    static String normalize(String const& c, String const& m);

  public:
    Object& clone() const override;
  };
} // core

#endif // CORE24_TRACE_H
