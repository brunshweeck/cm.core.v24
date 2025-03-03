//
// Created by bruns on 12/10/2024.
//

#ifndef CORE24_AUTOCLOSEABLE_H
#define CORE24_AUTOCLOSEABLE_H

#include <core/lang/Object.h>

namespace core {
    /**
     * An object that may hold resources (such as file or socket handles)
     * until it is closed. The @b close() method of an @c AutoCloseable
     * object is called automatically when at the class destruction.
     */
    class AutoCloseable : public virtual Object {
    protected:
        CORE_ALIAS(UNSAFE, misc::Unsafe);

    public:
        /**
         * Closes this resource, relinquishing any underlying resources.
         *
         * @apiNote
         * While this interface method is declared to throw
         * @c Exception, implementers are <em>strongly</em> encouraged to
         * declare concrete implementations of the @c close method to
         * throw more specific exceptions, or to throw no exception at all
         * if the close operation cannot fail.
         *
         * <p>
         * Cases where the close operation may fail require careful
         * attention by implementers. It is strongly advised to relinquish
         * the underlying resources and to internally <em>mark</em> the
         * resource as closed, prior to throwing the exception. The
         * @c close method is unlikely to be invoked more than once and so
         * this ensures that the resources are released in a timely manner.
         * Furthermore, it reduces problems that could arise when the resource
         * wraps, or is wrapped, by another resource.
         * </p>
         * <p>
         * <em>Implementers of this interface are also strongly advised
         * to not have the close method throw
         * InterruptedException.</em>
         *
         * More generally, if it would cause problems for an
         * exception to be suppressed, the @c AutoCloseable::close
         * method should not throw it.
         * </p>
         * <p>
         * Note that unlike the @b close
         * method of @b io::Closeable, this @c close method
         * is <em>not</em> required to be idempotent.  In other words,
         * calling this @c close method more than once may have some
         * visible side effect, unlike @c Closeable::close which is
         * required to have no effect if called more than once.
         *
         * However, implementers of this interface are strongly encouraged
         * to make their @c close methods idempotent.
         * </p>
         * @throws Exception if this resource cannot be closed
         */
        virtual void close() = 0;
    };
} // core

#endif //CORE24_AUTOCLOSEABLE_H
