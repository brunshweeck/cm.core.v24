//
// Created by brunshweeck on 15 sept. 2024.
//

#ifndef CORE24_FILEFILTER_H
#define CORE24_FILEFILTER_H

#include <core/function/Predicate.h>
#include <core/io/File.h>

namespace core {
    namespace io {
        /**
         * A filter for abstract pathnames.
         *
         * <p>
         * Instances of this interface may be passed to the
         * @b childFiles(FileFilter) method of the @b io::File class.
         * </p>
         */
        class FileFilter : public virtual function::Predicate<File> {
        public:
            /**
             * Construct new instance of FilenameFilter with specified callable
             * object. the call of FilenameFilter will be equivalent to:
             * @code
             *      return c(pathname);
             * @endcode
             *
             * @tparam Callable The type of direct function reference.
             * @param c the callable object.
             */
            template<
                class Callable,
                ClassOf(1)::OnlyIfAll<!Class<Callable>::isFunctionMember()> Capture1 = 1,
                ClassOf(1)::OnlyIfAll<Class<Callable>::template isCallable<File>()> Capture2 = 1
            >
            CORE_IMPLICIT FileFilter(Callable &&c)
                : Predicate(UNSAFE::forwardInstance<Callable>(c)) {
            }

            /**
             * Construct new instance of FileFilter with specified object
             * and specified method handle.
             * This method of construction consist of use the given object has
             * dependency of method handle, like this:
             * @code
             *      return obj.method(pathname);
             * @endcode
             *
             * @tparam Var The type of object reference
             * @tparam Method The method handle
             *
             * @param obj the object instance required to use the given method handle.
             * @param m the method handle.
             */
            template<
                class Var,
                class Method,
                ClassOf(1)::OnlyIfAll<Class<Method>::isFunctionMember()> Capture1 = 1,
                ClassOf(1)::OnlyIfAll<Class<Method>::template isCallable<Var, File>()> Capture2 = 1
            >
            CORE_IMPLICIT FileFilter(Var &&obj, Method &&m)
                : Predicate(UNSAFE::forwardInstance<Var>(obj), UNSAFE::forwardInstance<Method>(m)) {  }

            /**
             * Tests whether the specified abstract pathname should be
             * included in a pathname list.
             *
             * @param  pathname  The abstract pathname to be tested
             * @return  @c true if and only if @c pathname should be included
             */
            virtual gbool accept(File const &pathname) const;

            gbool equals(const Object &obj) const override;

            Object &clone() const override;

        private:
            gbool test(File const &arg) const CORE_NOTHROW final;
        };
    } // io
} // core

#endif //CORE24_FILEFILTER_H
