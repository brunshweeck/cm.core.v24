//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

/**
 * Provides classes that are fundamental to the design of the
 * Core-24 programming framework. The most important classes are
 * @c Object, which is the root of the class hierarchy, and
 * @c Class, instances of which represent classes at run time.
 *
 * <p>
 * Frequently it is necessary to represent a value of primitive
 * type as if it were an object. The wrapper classes @c Boolean,
 * @c Character, @c Integer, @c Long, @c Float,
 * and @c Double serve this purpose.  An object of type
 * @c Double, for example, contains a field whose type is double,
 * representing that value in such a way that a reference to it can be
 * stored in a variable of reference type.  These classes also provide
 * a number of methods for converting among primitive values, as well
 * as supporting such standard methods as equals and hashCode.  The
 * @c Void class is a non-instantiable class that holds a
 * reference to a @c Class object representing the type void.
 * </p>
 * <p>
 * The class @c Math provides commonly used mathematical
 * functions such as sine, cosine, and square root. The classes 
 * @c String, @c StringBuffer, and @c StringBuilder similarly
 * provide commonly used operations on character strings.
 * </p>
 * <p>
 * Classes @c ClassLoader, @c Process,
 * @c ProcessBuilder, @c Runtime, @c SecurityManager, and
 * @c System provide "system operations" that manage the dynamic
 * loading of classes, creation of external processes, host
 * environment inquiries such as the time of day, and enforcement of
 * security policies.
 * </p>
 * <p>
 * Class @c Throwable encompasses objects that may be thrown
 * by the @c throw statement. Subclasses of @c Throwable
 * represent errors and exceptions.
 * </p>
 * <h3>Character Encodings</h3>
 *
 * The specification of the @b charset::Charset
 * class describes the naming conventions
 * for character encodings as well as the set of standard encodings
 * that must be supported by every implementation of the Java
 * platform.
 */
namespace core {}

#endif //NAMESPACE_DOC_H
