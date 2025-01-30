//
// Created by admin on 25/01/25.
//

#ifndef NAMESPACE_DOC_H
#define NAMESPACE_DOC_H

namespace core {
    /**
     * Provides the classes for implementing networking applications.
     *
     * <p>
     * The java.net package can be roughly divided in two sections:<br/>
     *  - <i>A Low Level API</i>, which deals with the following abstractions:<br/>
     *       - <i>Addresses</i>, which are networking identifiers, like IP addresses.<br/>
     *       - <i>Sockets</i>, which are basic bidirectional data communication mechanisms.<br/>
     *       - <i>Interfaces</i>, which describe network interfaces. <br/><br/>
     *  - <i>A High Level API</i>, which deals with the following abstractions:<br/>
     *       - <i>URIs</i>, which represent Universal Resource Identifiers.<br/>
     *       - <i>URLs</i>, which represent Universal Resource Locators.<br/>
     *       - <i>Connections</i>, which represents connections to the resource pointed to by <i>URLs</i>.<br/><br/>
     * </p>
     * <h3>Addresses.</h3>
     *
     * <p>
     * Addresses are used throughout the java.net APIs as either host
     *    identifiers, or socket endpoint identifiers.
     * </p>
     * <p>
     * The @b net::InetAddress class is the abstraction representing an
     *    IP (Internet Protocol) address.  It has two subclasses:<br/>
     * - @b net::Inet4Address for IPv4 addresses.<br/>
     * - @b net::Inet6Address for IPv6 addresses.<br/>
     * </p>
     * <p>
     * But, in most cases, there is no need to deal directly with the subclasses,
     *    as the InetAddress abstraction should cover most of the needed
     *    functionality.
     * </p>
     * <h3>About IPv6</h3>
     * <p>
     * Not all systems have support for the IPv6 protocol, and while the Java
     *    networking stack will attempt to detect it and use it transparently when
     *    available, it is also possible to disable its use with a system property.
     *    In the case where IPv6 is not available, or explicitly disabled,
     *    Inet6Address are not valid arguments for most networking operations anymore.
     *    While methods like @b net::InetAddress::getByName are
     *    guaranteed not to return an Inet6Address when looking up host names, it
     *    is possible, by passing literals, to create such an object. In which
     *    case, most methods, when called with an Inet6Address will throw an
     *    Exception.
     * </p>
     * <h3>Sockets.</h3>
     * <p>
     * Sockets are means to establish a communication link between machines over
     *    the network. The core::net namespace provides 4 kinds of Sockets:<br/>
     * - @b net::Socket is a TCP client API, and will typically
     *            be used to @em connect to a remote host.<br/>
     * - net::ServerSocket is a TCP server API, and will
     *            typically @em accept connections from client sockets.<br/>
     * - net::DatagramSocket is a UDP endpoint API and is used
     *            to @em send and @em receive @em datagram packets.<br/>
     * - net::MulticastSocket is a subclass of
     *            @c DatagramSocket used when dealing with multicast groups.<br/>
     * </p>
     * <p>
     * Sending and receiving with TCP sockets is done through InputStreams and
     *    OutputStreams which can be obtained via the
     *    @b net::Socket::getInputStream and @b net::Socket::getOutputStream methods.
     * </p>
     * <h3>Interfaces</h3>
     * <p>
     * The @b net::NetworkInterface class provides APIs to browse and
     *    query all the networking interfaces (e.g. ethernet connection or PPP
     *    endpoint) of the local machine. It is through that class that you can
     *    check if any of the local interfaces is configured to support IPv6.
     * </p>
     * <p>
     * Note, all conforming implementations must support at least one
     *    @c NetworkInterface object, which must either be connected to a
     *    network, or be a "loopback" interface that can only communicate with
     *    entities on the same machine.
     * </p>
     *
     * <h3>High level API</h3>
     * <p>
     * A number of classes in the java.net package do provide for a much higher
     *    level of abstraction and allow for easy access to resources on the
     *    network. The classes are:
     * - @b net::URI is the class representing a
     *            Universal Resource Identifier, as specified in RFC 2396.
     *            As the name indicates, this is just an Identifier and doesn't
     *            provide directly the means to access the resource.<br/>
     * - @b net::URL is the class representing a
     *            Universal Resource Locator, which is both an older concept for
     *            URIs and a means to access the resources.<br/>
     * - @b net::URLConnection is created from a URL and is the
     *            communication link used to access the resource pointed by the
     *            URL. This abstract class will delegate most of the work to the
     *            underlying protocol handlers like http or https.<br/>
     * - @b net::HttpURLConnection is a subclass of URLConnection
     *            and provides some additional functionalities specific to the
     *            HTTP protocol. This API has been superseded by the newer
     *            @em HTTP Client API.<br/>
     * </p>
     * <p>
     * The recommended usage is to use @b net::URI to identify
     *    resources, then convert it into a @b net::URL when it is time to
     *    access the resource. From that URL, you can either get the
     *    @b net::URLConnection for fine control, or get directly the
     *    InputStream.
     * </p>
     * <p>
     * Here is an example:
     * @code
     * auto uri = URI("http://www.example.com/");
     * auto url = uri.toURL();
     * InputStream &in = url.openStream();
     * @endcode
     * </p>
     * <h3>Protocol Handlers</h3>
     * As mentioned, URL and URLConnection rely on protocol handlers which must be
     * present, otherwise an Exception is thrown. This is the major difference with
     * URIs which only identify resources, and therefore don't need to have access
     * to the protocol handler. So, while it is possible to create an URI with any
     * kind of protocol scheme (e.g. @c myproto://myhost.mydomain/resource/),
     * a similar URL will try to instantiate the handler for the specified protocol;
     * if it doesn't exist an exception will be thrown.
     * <p>
     * By default, the protocol handlers are loaded dynamically from the default
     *    location. It is, however, possible to deploy additional protocols handlers
     *    as @b services.
     * </p>
     */
    namespace net {}
}

#endif //NAMESPACE_DOC_H
