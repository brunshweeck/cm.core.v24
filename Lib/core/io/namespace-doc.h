//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {
    /**
     * Provides for system input and output through data streams,
     * serialization and the file system.
     *
     * Unless otherwise noted, passing a @c null argument to a constructor or
     * method in any class or interface in this package will cause a
     * @c IllegalArgumentException to be thrown.
     *
     * A <i>pathname string</i> passed as a @c String argument to a
     * constructor or method in any class or interface in this package will be
     * interpreted as described in the class specification of @b File.
     *
     * <h3>Object Serialization</h3>
     * <p>
     * <b>Warning: Deserialization of untrusted data is inherently dangerous
     * and should be avoided</b>.
     * </p>
     */
    namespace io {}
}

#endif //NAMESPACE_DOC_H
