//
// Created by brunshweeck on 15 sept. 2024.
//

#ifndef CORE24_FILENAMEFILTER_H
#define CORE24_FILENAMEFILTER_H

#include <core/function/BiPredicate.h>
#include <core/io/File.h>

namespace core {
    namespace io {
        /**
         * Instances of classes that implement this interface are used to
         * filter filenames. These instances are used to filter directory
         * listings in the @c list method of class
         * @c File, and by the Abstract Window Toolkit is file
         * dialog component.
         */
        class FilenameFilter : public virtual function::BiPredicate<File, String> {
        public:
            /**
             * Construct new instance of FilenameFilter with specified callable
             * object. the call of FilenameFilter will be equivalent to:
             * @code
             *      return c(dir, name);
             * @endcode
             *
             * @tparam Callable The type of direct function reference.
             * @param c the callable object.
             */
            template<
                class Callable,
                ClassOf(1)::OnlyIfAll<!Class<Callable>::isFunctionMember()> Capture1 = 1,
                ClassOf(1)::OnlyIfAll<Class<Callable>::template isCallable<File, String>()> Capture2 = 1
            >
            CORE_IMPLICIT FilenameFilter(Callable &&c)
                : BiPredicate(UNSAFE::forwardInstance<Callable>(c)) {
            }

            /**
             * Construct new instance of FilenameFilter with specifieds object
             * and specified method handle.
             * This method of construction consist of use the given object has
             * dependency of method handle, like this:
             * @code
             *      return obj.method(dir, name);
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
                ClassOf(1)::OnlyIfAll<Class<Method>::template isCallable<Var, File, String>()> Capture2 = 1
            >
            CORE_EXPLICIT FilenameFilter(Var &&obj, Method &&m)
                : BiPredicate(UNSAFE::forwardInstance<Var>(obj), UNSAFE::forwardInstance<Method>(m)) {
            }

            /**
             * Tests if a specified file should be included in a file list.
             *
             * @param   dir    the directory in which the file was found.
             * @param   name   the name of the file.
             * @return  @c true if and only if the name should be
             * included in the file list; @c false otherwise.
             */
            virtual gbool accept(File const &dir, String const &name) const;

            gbool equals(const Object &obj) const override;

            Object &clone() const override;

        private:
            gbool test(File const &a, String const &b) const CORE_NOTHROW final;
        };
    } // io
} // core

#endif //CORE24_FILENAMEFILTER_H
