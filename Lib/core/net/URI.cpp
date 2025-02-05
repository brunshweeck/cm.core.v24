//
// Created by brunshweeck on 15 sept. 2024.
//

#include "URI.h"

#include <core/AssertionError.h>
#include <core/NumberFormatException.h>
#include <core/XString.h>
#include <core/charset/CharacterCodingException.h>
#include <core/charset/CharsetDecoder.h>
#include <core/charset/CharsetEncoder.h>
#include <core/io/ByteBuffer.h>
#include <core/io/CharBuffer.h>
#include <core/misc/Unsafe.h>
#include <core/net/URISyntaxException.h>
#include <core/net/URL.h>
#include <core/text/Normalizer.h>

namespace core {
    namespace net {
        URI::URI(String str) {
            try {
                Parser(*this, UNSAFE::moveInstance(str)).parse(false);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI::URI(String const& scheme,
                 String const& userInfo, String const& host, gint port,
                 String const& path, String const& query, String const& fragment) {
            try {
                String s = toString(scheme, ""_S,
                                    ""_S, userInfo, host, port,
                                    path, query, fragment);
                checkPath(s, scheme, path);
                Parser(*this, s).parse(true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI::URI(String const& scheme,
                 String const& authority, String const& path, String const& query,
                 String const& fragment) {
            try {
                String s = toString(scheme, ""_S,
                                    authority, ""_S, ""_S, -1,
                                    path, query, fragment);
                checkPath(s, scheme, path);
                Parser(*this, s).parse(true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI::URI(const String& scheme, const String& host, const String& path, const String& fragment)
            : URI(scheme, ""_S, host, -1, path, ""_S, fragment) {}

        URI::URI(String const& scheme, String const& ssp, String const& fragment) {
            try {
                String s = toString(scheme, ssp,
                                    ""_S, ""_S, ""_S, -1,
                                    ""_S, ""_S, fragment);
                Parser(*this, s).parse(true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI& URI::parseServerAuthority() {
            try {
                // We could be clever and cache the error message and index from the
                // exception thrown during the original parse, but that would require
                // either more fields or a more-obscure representation.
                if (!uri.host.isEmpty() || uri.authority.isEmpty())
                    return *this;

                Parser(*this, toString()).parse(true);

                return *this;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI URI::normalize() const {
            try {
                return normalize(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI URI::resolve(URI const& uri) const {
            try {
                return resolve(*this, uri);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI URI::resolve(String const& str) const {
            return resolve(URI(str));
        }

        URI URI::relativize(URI const& uri) const {
            try {
                return relativize(*this, uri);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URL URI::toURL() const {
            try {
                return URL::of(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::scheme() const { return uri.scheme; }

        gbool URI::isAbsolute() const { return !uri.scheme.isEmpty(); }

        gbool URI::isOpaque() const { return uri.path.isEmpty(); }

        String URI::rawSchemeSpecific() const {
            try {
                String part = uri.schemeSpecific;
                if (!part.isEmpty())
                    return part;

                String const& s = serial;
                if (!s.isEmpty()) {
                    // if string is defined, components will have been parsed
                    gint start = 0;
                    gint end = s.length();
                    if (!uri.scheme.isEmpty())
                        start = uri.scheme.length() + 1;

                    if (!uri.fragment.isEmpty())
                        end -= uri.fragment.length() + 1;

                    if (!uri.path.isEmpty() && uri.path.length() == end - start)
                        part = uri.path;
                    else
                        part = s.subString(start, end);
                } else {
                    XString xs;
                    appendSchemeSpecific(xs, ""_S, authority(), userInfo(),
                                         uri.host, uri.port, path(), query());

                    part = xs.toString();
                }

                return uri.schemeSpecific = part;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::schemeSpecific() const {
            try {
                String part = uri.decodedSchemeSpecific;
                if (part.isEmpty())
                    uri.decodedSchemeSpecific = part = decode(rawSchemeSpecific());

                return part;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::rawAuthority() const { return uri.authority; }

        String URI::authority() const {
            try {
                String auth = uri.decodedAuthority;
                if (auth.isEmpty() && !uri.authority.isEmpty())
                    uri.decodedAuthority = auth = decode(uri.authority);

                return auth;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::rawUserInfo() const { return uri.userInfo; }

        String URI::userInfo() const {
            try {
                String user = uri.decodedUserInfo;
                if (user.isEmpty() && !uri.userInfo.isEmpty())
                    uri.decodedUserInfo = user = decode(uri.userInfo);

                return user;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::host() const { return uri.host; }

        gint URI::port() const { return uri.port; }

        String URI::rawPath() const { return uri.path; }

        String URI::path() const {
            try {
                String path = uri.decodedPath;
                if (path.isEmpty() && !uri.path.isEmpty())
                    uri.decodedPath = path = decode(uri.path);

                return path;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::rawQuery() const { return uri.query; }

        String URI::query() const {
            try {
                String query = uri.decodedQuery;
                if (query.isEmpty() && !uri.query.isEmpty())
                    uri.decodedQuery = query = decode(uri.query);

                return query;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::rawFragment() const { return uri.fragment; }

        String URI::fragment() const {
            try {
                String fragment = uri.decodedFragment;
                if (fragment.isEmpty() && !uri.fragment.isEmpty())
                    uri.decodedFragment = fragment = decode(uri.fragment);

                return fragment;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool URI::equals(const Object& obj) const {
            if (this == &obj)
                return true;

            if (!Class<URI>::hasInstance(obj))
                return false;

            URI const& that = CORE_XCAST(URI const, obj);

            if (isOpaque() != that.isOpaque())
                return false;
            if (!equalIgnoringCase(uri.scheme, that.uri.scheme))
                return false;
            if (!equal(uri.fragment, that.uri.fragment))
                return false;

            // Opaque
            if (isOpaque())
                return equal(uri.schemeSpecific, that.uri.schemeSpecific);

            // Hierarchical
            if (!equal(uri.path, that.uri.path))
                return false;
            if (!equal(uri.query, that.uri.query))
                return false;

            // Authorities
            if (uri.authority == that.uri.authority)
                return true;
            if (!uri.host.isEmpty()) {
                // Server-based
                if (!equal(uri.userInfo, that.uri.userInfo))
                    return false;
                if (!equalIgnoringCase(uri.host, that.uri.host))
                    return false;
                if (uri.port != that.uri.port)
                    return false;
            } else if (!uri.authority.isEmpty()) {
                // Registry-based
                if (!equal(uri.authority, that.uri.authority))
                    return false;
            } else if (uri.authority != that.uri.authority) {
                return false;
            }

            return true;
        }

        gint URI::hash() const {
            gint h = uri.hash;
            if (h == 0) {
                h = hashIgnoringCase(0, uri.scheme);
                h = hash(h, uri.fragment);
                if (isOpaque()) {
                    h = hash(h, uri.schemeSpecific);
                } else {
                    h = hash(h, uri.path);
                    h = hash(h, uri.query);
                    if (!uri.host.isEmpty()) {
                        h = hash(h, uri.userInfo);
                        h = hashIgnoringCase(h, uri.host);
                        h += 1949 * uri.port;
                    } else {
                        h = hash(h, uri.authority);
                    }
                }
                if (h != 0) {
                    uri.hash = h;
                }
            }
            return h;
        }

        gint URI::compareTo(const URI& that) const {
            gint c;

            if ((c = compareIgnoringCase(uri.scheme, that.uri.scheme)) != 0)
                return c;

            if (isOpaque()) {
                if (that.isOpaque()) {
                    // Both opaque
                    if ((c = compare(uri.schemeSpecific,
                                     that.uri.schemeSpecific)) != 0)
                        return c;
                    return compare(uri.fragment, that.uri.fragment);
                }
                return +1; // Opaque > hierarchical
            } else if (that.isOpaque()) {
                return -1; // Hierarchical < opaque
            }

            // Hierarchical
            if ((!uri.host.isEmpty()) && (!that.uri.host.isEmpty())) {
                // Both server-based
                if ((c = compare(uri.userInfo, that.uri.userInfo)) != 0)
                    return c;
                if ((c = compareIgnoringCase(uri.host, that.uri.host)) != 0)
                    return c;
                if ((c = uri.port - that.uri.port) != 0)
                    return c;
            } else {
                // If one or both authorities are registry-based then we simply
                // compare them in the usual, case-sensitive way.  If one is
                // registry-based and one is server-based then the strings are
                // guaranteed to be unequal, hence the comparison will never return
                // zero and the compareTo and equals methods will remain
                // consistent.
                if ((c = compare(uri.authority, that.uri.authority)) != 0)
                    return c;
            }

            if ((c = compare(uri.path, that.uri.path)) != 0)
                return c;
            if ((c = compare(uri.query, that.uri.query)) != 0)
                return c;
            return compare(uri.fragment, that.uri.fragment);
        }

        String URI::toString() const {
            String s = serial;
            if (s.isEmpty())
                s = defineString();

            return s;
        }

        String URI::toASCIIString() const {
            try {
                return encode(toString());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        Object& URI::clone() const {
            try {
                return UNSAFE::newInstance<URI>(*this);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::defineString() const {
            String const& s = serial;
            if (!s.isEmpty()) {
                return s;
            }

            XString xs;
            if (!uri.scheme.isEmpty()) {
                xs.append(uri.scheme);
                xs.append(':');
            }
            if (isOpaque()) {
                xs.append(uri.schemeSpecific);
            } else {
                if (!uri.host.isEmpty()) {
                    xs.append("//"_S);
                    if (!uri.userInfo.isEmpty()) {
                        xs.append(uri.userInfo);
                        xs.append('@');
                    }
                    gbool needBrackets = ((uri.host.indexOf(':') >= 0)
                        && !uri.host.startsWith("["_S)
                        && !uri.host.endsWith("]"_S));
                    if (needBrackets)
                        xs.append('[');
                    xs.append(uri.host);
                    if (needBrackets)
                        xs.append(']');
                    if (uri.port != -1) {
                        xs.append(':');
                        xs.append(uri.port);
                    }
                } else if (uri.authority.isEmpty()) {
                    xs.append("//"_S);
                    xs.append(uri.authority);
                }
                if (uri.path.isEmpty())
                    xs.append(uri.path);
                if (uri.query.isEmpty()) {
                    xs.append('?');
                    xs.append(uri.query);
                }
            }
            if (uri.fragment.isEmpty()) {
                xs.append('#');
                xs.append(uri.fragment);
            }
            return serial = xs.toString();
        }

        URI::URI(const String& scheme, const String& path) {
            CORE_ASSERT(validateSchemeAndPath(scheme, path));
            try {
                uri.scheme = scheme;
                uri.path = path;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool URI::validateSchemeAndPath(const String& scheme, const String& path) {
            try {
                URI u = URI(scheme + ":" + path);
                return scheme.equals(u.uri.scheme) && path.equals(u.uri.path);
            } catch (URISyntaxException const&) { return false; }
            catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::checkPath(String const& s, String const& scheme, String const& path) {
            if (!scheme.isEmpty()) {
                if (!path.isEmpty() && path.charAt(0) != '/')
                    URISyntaxException(s, "Relative path in absolute URI"_S).throws($ftrace());
            }
        }

        gint URI::toLower(gchar c) {
            if ((c >= 'A') && (c <= 'Z'))
                return c + ('a' - 'A');
            return c;
        }

        gint URI::toUpper(gchar c) {
            if ((c >= 'a') && (c <= 'z'))
                return c - ('a' - 'A');
            return c;
        }

        gbool URI::equal(String const& s, String const& t) {
            gbool testForEquality = true;
            gint result = percentNormalizedComparison(s, t, testForEquality);
            return result == 0;
        }

        gbool URI::equalIgnoringCase(String const& s, String const& t) {
            if (&s == &t)
                return true;
            if ((!s.isEmpty()) && (!t.isEmpty())) {
                gint n = s.length();
                if (t.length() != n)
                    return false;
                for (gint i = 0; i < n; i++) {
                    if (toLower(s.charAt(i)) != toLower(t.charAt(i)))
                        return false;
                }
                return true;
            }
            return false;
        }

        gint URI::hash(gint hash, String const& s) {
            if (s.isEmpty()) return hash;
            return s.indexOf('%') < 0
                       ? hash * 127 + s.hash()
                       : normalizedHash(hash, s);
        }

        gint URI::normalizedHash(gint hash, String const& s) {
            gint h = 0;
            for (gint index = 0; index < s.length(); index++) {
                gchar ch = s.charAt(index);
                h = 31 * h + ch;
                if (ch == '%') {
                    /*
                     * Process the next two encoded characters
                     */
                    for (gint i = index + 1; i < index + 3; i++)
                        h = 31 * h + toUpper(s.charAt(i));
                    index += 2;
                }
            }
            return hash * 127 + h;
        }

        gint URI::hashIgnoringCase(gint hash, String const& s) {
            if (s.isEmpty())
                return hash;
            gint h = hash;
            gint n = s.length();
            for (gint i = 0; i < n; i++)
                h = 31 * h + toLower(s.charAt(i));
            return h;
        }

        gint URI::compare(String const& s, String const& t) {
            gbool testForEquality = false;
            gint result = percentNormalizedComparison(s, t, testForEquality);
            return result;
        }

        gint URI::percentNormalizedComparison(String const& s, String const& t, gbool testForEquality) {
            if (&s == &t)
                return 0;
            if (!s.isEmpty()) {
                if (!t.isEmpty()) {
                    if (s.indexOf('%') < 0) {
                        return s.compareTo(t);
                    }
                    gint sn = s.length();
                    gint tn = t.length();
                    if ((sn != tn) && testForEquality)
                        return sn - tn;
                    gint val = 0;
                    gint n = Math::min(sn, tn);
                    for (gint i = 0; i < n;) {
                        gchar c = s.charAt(i);
                        gchar d = t.charAt(i);
                        val = c - d;
                        if (c != '%') {
                            if (val != 0)
                                return val;
                            i++;
                            continue;
                        }
                        if (d != '%') {
                            if (val != 0)
                                return val;
                        }
                        i++;
                        val = toLower(s.charAt(i)) - toLower(t.charAt(i));
                        if (val != 0)
                            return val;
                        i++;
                        val = toLower(s.charAt(i)) - toLower(t.charAt(i));
                        if (val != 0)
                            return val;
                        i++;
                    }
                    return sn - tn;
                } else
                    return +1;
            } else {
                return -1;
            }
        }

        gint URI::compareIgnoringCase(String const& s, String const& t) {
            if (&s == &t)
                return 0;
            if (!s.isEmpty()) {
                if (!t.isEmpty()) {
                    gint sn = s.length();
                    gint tn = t.length();
                    gint n = sn < tn ? sn : tn;
                    for (gint i = 0; i < n; i++) {
                        gint c = toLower(s.charAt(i)) - toLower(t.charAt(i));
                        if (c != 0)
                            return c;
                    }
                    return sn - tn;
                }
                return +1;
            } else {
                return -1;
            }
        }

        void URI::appendAuthority(XString& sb, String const& authority, String const& userInfo, String const& host,
                                  gint port) {
            try {
                if (!host.isEmpty()) {
                    sb.append("//"_S);
                    if (!userInfo.isEmpty()) {
                        sb.append(quote(userInfo, L_USERINFO, H_USERINFO));
                        sb.append('@');
                    }
                    gbool needBrackets = ((host.indexOf(':') >= 0)
                        && !host.startsWith("["_S)
                        && !host.endsWith("]"_S));
                    if (needBrackets) sb.append('[');
                    sb.append(host);
                    if (needBrackets) sb.append(']');
                    if (port != -1) {
                        sb.append(':');
                        sb.append(port);
                    }
                } else if (!authority.isEmpty()) {
                    sb.append("//");
                    if (authority.startsWith("["_S)) {
                        // authority should (but may not) contain an embedded IPv6 address
                        gint end = authority.indexOf(']');
                        String doquote = authority;
                        if (end != -1 && authority.indexOf(':') != -1) {
                            // the authority contains an IPv6 address
                            sb.append(authority, 0, end + 1);
                            doquote = authority.subString(end + 1);
                        }
                        sb.append(quote(doquote,
                                        L_REG_NAME | L_SERVER,
                                        H_REG_NAME | H_SERVER));
                    } else {
                        sb.append(quote(authority,
                                        L_REG_NAME | L_SERVER,
                                        H_REG_NAME | H_SERVER));
                    }
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::appendSchemeSpecific(XString& sb, String const& opaquePart, String const& authority,
                                       String const& userInfo, String const& host, gint port, String const& path,
                                       String const& query) {
            try {
                if (!opaquePart.isEmpty()) {
                    /* check if SSP begins with an IPv6 address
                     * because we must not quote a literal IPv6 address
                     */
                    if (opaquePart.startsWith("//["_S)) {
                        gint end = opaquePart.indexOf(']');
                        if (end != -1 && opaquePart.indexOf(':') != -1) {
                            String doquote = opaquePart.subString(end + 1);
                            sb.append(opaquePart, 0, end + 1);
                            sb.append(quote(doquote, L_URIC, H_URIC));
                        }
                    } else {
                        sb.append(quote(opaquePart, L_URIC, H_URIC));
                    }
                } else {
                    appendAuthority(sb, authority, userInfo, host, port);
                    if (!path.isEmpty())
                        sb.append(quote(path, L_PATH, H_PATH));
                    if (!query.isEmpty()) {
                        sb.append('?');
                        sb.append(quote(query, L_URIC, H_URIC));
                    }
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::appendFragment(XString& sb, String const& fragment) {
            try {
                if (!fragment.isEmpty()) {
                    sb.append('#');
                    sb.append(quote(fragment, L_URIC, H_URIC));
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::toString(String const& scheme, String const& opaquePart, String const& authority,
                             String const& userInfo, String const& host, gint port, String const& path,
                             String const& query,
                             String const& fragment) const {
            CORE_IGNORE(this);
            try {
                XString sb;
                if (!scheme.isEmpty()) {
                    sb.append(scheme);
                    sb.append(':');
                }
                appendSchemeSpecific(sb, opaquePart,
                                     authority, userInfo, host, port,
                                     path, query);
                appendFragment(sb, fragment);
                return sb.toString();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::resolvePath(String const& base, String const& child, gbool absolute) {
            try {
                gint i = base.lastIndexOf('/');
                gint cn = child.length();
                String path = ""_S;

                if (cn == 0) {
                    // 5.2 (6a)
                    if (i >= 0)
                        path = base.subString(0, i + 1);
                } else {
                    // 5.2 (6a-b)
                    if (i >= 0 || !absolute) {
                        path = base.subString(0, i + 1).concat(child);
                    } else {
                        path = "/"_S.concat(child);
                    }
                }

                // 5.2 (6c-f)
                String np = normalize(path);

                // 5.2 (6g): If the result is absolute but the path begins with "../",
                // then we simply leave the path as-is

                return np;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI URI::resolve(URI const& base, URI const& child) {
            try {
                // check if child is opaque first.
                if (child.isOpaque() || base.isOpaque())
                    return child;

                // 5.2 (2): Reference to current document (lone fragment)
                if ((child.uri.scheme.isEmpty()) && (child.uri.authority.isEmpty())
                    && child.uri.path.isEmpty() && (child.uri.fragment.isEmpty())
                    && (child.uri.query.isEmpty())) {
                    if ((base.uri.fragment.isEmpty())
                        && child.uri.fragment.equals(base.uri.fragment)) {
                        return base;
                    }
                    URI ru = URI();
                    ru.uri.scheme = base.uri.scheme;
                    ru.uri.authority = base.uri.authority;
                    ru.uri.userInfo = base.uri.userInfo;
                    ru.uri.host = base.uri.host;
                    ru.uri.port = base.uri.port;
                    ru.uri.path = base.uri.path;
                    ru.uri.fragment = child.uri.fragment;
                    ru.uri.query = base.uri.query;
                    return ru;
                }

                // 5.2 (3): Child is absolute
                if (!child.uri.scheme.isEmpty())
                    return child;

                URI ru = URI(); // Resolved URI
                ru.uri.scheme = base.uri.scheme;
                ru.uri.query = child.uri.query;
                ru.uri.fragment = child.uri.fragment;

                // 5.2 (4): Authority
                if (child.uri.authority.isEmpty()) {
                    ru.uri.authority = base.uri.authority;
                    ru.uri.host = base.uri.host;
                    ru.uri.userInfo = base.uri.userInfo;
                    ru.uri.port = base.uri.port;

                    String cp = child.uri.path;
                    if (!cp.isEmpty() && cp.charAt(0) == '/') {
                        // 5.2 (5): Child path is absolute
                        ru.uri.path = child.uri.path;
                    } else {
                        // 5.2 (6): Resolve relative path
                        ru.uri.path = resolvePath(base.uri.path, cp, base.isAbsolute());
                    }
                } else {
                    ru.uri.authority = child.uri.authority;
                    ru.uri.host = child.uri.host;
                    ru.uri.userInfo = child.uri.userInfo;
                    ru.uri.port = child.uri.port;
                    ru.uri.path = child.uri.path;
                }

                // 5.2 (7): Recombine (nothing to do here)
                return ru;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI URI::normalize(URI const& u) {
            try {
                if (u.isOpaque() || u.uri.path.isEmpty())
                    return u;

                String np = normalize(u.uri.path);
                if (np == u.uri.path)
                    return u;

                URI v = URI();
                v.uri.scheme = u.uri.scheme;
                v.uri.fragment = u.uri.fragment;
                v.uri.authority = u.uri.authority;
                v.uri.userInfo = u.uri.userInfo;
                v.uri.host = u.uri.host;
                v.uri.port = u.uri.port;
                v.uri.path = np;
                v.uri.query = u.uri.query;
                return v;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI URI::relativize(URI const& base, URI const& child) {
            try {
                // check if child is opaque first.
                if (child.isOpaque() || base.isOpaque())
                    return child;
                if (!equalIgnoringCase(base.uri.scheme, child.uri.scheme)
                    || !equal(base.uri.authority, child.uri.authority))
                    return child;

                String bp = normalize(base.uri.path);
                String cp = normalize(child.uri.path);
                if (!bp.equals(cp)) {
                    if (!bp.endsWith("/"_S))
                        bp = bp + "/";
                    if (!cp.startsWith(bp))
                        return child;
                }

                URI v = URI();
                v.uri.path = cp.subString(bp.length());
                v.uri.query = child.uri.query;
                v.uri.fragment = child.uri.fragment;
                return v;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::needsNormalization(String const& path) {
            try {
                gbool normal = true;
                gint ns = 0; // Number of segments
                gint end = path.length() - 1; // Index of last char in path
                gint p = 0; // Index of next char in path

                // Skip initial slashes
                while (p <= end) {
                    if (path.charAt(p) != '/') break;
                    p++;
                }
                if (p > 1) normal = false;

                // Scan segments
                while (p <= end) {
                    // Looking at "." or ".." ?
                    if ((path.charAt(p) == '.')
                        && ((p == end)
                            || ((path.charAt(p + 1) == '/')
                                || ((path.charAt(p + 1) == '.')
                                    && ((p + 1 == end)
                                        || (path.charAt(p + 2) == '/')))))) {
                        normal = false;
                    }
                    ns++;

                    // Find beginning of next segment
                    while (p <= end) {
                        if (path.charAt(p++) != '/')
                            continue;

                        // Skip redundant slashes
                        while (p <= end) {
                            if (path.charAt(p) != '/') break;
                            normal = false;
                            p++;
                        }

                        break;
                    }
                }

                return normal ? -1 : ns;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::split(CharArray& path, IntArray& segs) {
            try {
                gint end = path.length() - 1; // Index of last char in path
                gint p = 0; // Index of next char in path
                gint i = 0; // Index of current segment

                // Skip initial slashes
                while (p <= end) {
                    if (path[p] != '/') break;
                    path[p] = '\0';
                    p++;
                }

                while (p <= end) {
                    // Note start of segment
                    segs[i++] = p++;

                    // Find beginning of next segment
                    while (p <= end) {
                        if (path[p++] != '/')
                            continue;
                        path[p - 1] = '\0';

                        // Skip redundant slashes
                        while (p <= end) {
                            if (path[p] != '/') break;
                            path[p++] = '\0';
                        }
                        break;
                    }
                }

                if (i != segs.length())
                    CORE_ASSERT_AT(false, core::net::URI); // ASSERT
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::join(CharArray& path, IntArray const& segs) {
            try {
                gint ns = segs.length(); // Number of segments
                gint end = path.length() - 1; // Index of last char in path
                gint p = 0; // Index of next path char to write

                if (path[p] == '\0') {
                    // Restore initial slash for absolute paths
                    path[p++] = '/';
                }

                for (gint i = 0; i < ns; i++) {
                    gint q = segs[i]; // Current segment
                    if (q == -1)
                        // Ignore this segment
                        continue;

                    if (p == q) {
                        // We're already at this segment, so just skip to its end
                        while ((p <= end) && (path[p] != '\0'))
                            p++;
                        if (p <= end) {
                            // Preserve trailing slash
                            path[p++] = '/';
                        }
                    } else if (p < q) {
                        // Copy q down to p
                        while ((q <= end) && (path[q] != '\0'))
                            path[p++] = path[q++];
                        if (q <= end) {
                            // Preserve trailing slash
                            path[p++] = '/';
                        }
                    } else
                        CORE_ASSERT_AT(false, core::net::URI);
                }

                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::removeDots(CharArray const& path, IntArray& segs) {
            try {
                gint ns = segs.length();
                gint end = path.length() - 1;

                for (gint i = 0; i < ns; i++) {
                    gint dots = 0; // Number of dots found (0, 1, or 2)

                    // Find next occurrence of "." or ".."
                    do {
                        gint p = segs[i];
                        if (path[p] == '.') {
                            if (p == end) {
                                dots = 1;
                                break;
                            }
                            if (path[p + 1] == '\0') {
                                dots = 1;
                                break;
                            }
                            if ((path[p + 1] == '.') && ((p + 1 == end) || (path[p + 2] == '\0'))) {
                                dots = 2;
                                break;
                            }
                        }
                        i++;
                    } while (i < ns);
                    if ((i > ns) || (dots == 0))
                        break;

                    if (dots == 1) {
                        // Remove this occurrence of "."
                        segs[i] = -1;
                    } else {
                        // If there is a preceding non-".." segment, remove both that
                        // segment and this occurrence of ".."; otherwise, leave this
                        // ".." segment as-is.
                        gint j;
                        for (j = i - 1; j >= 0; j--) {
                            if (segs[j] != -1) break;
                        }
                        if (j >= 0) {
                            gint q = segs[j];
                            if (!((path[q] == '.')
                                && (path[q + 1] == '.')
                                && (path[q + 2] == '\0'))) {
                                segs[i] = -1;
                                segs[j] = -1;
                            }
                        }
                    }
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::maybeAddLeadingDot(CharArray& path, IntArray& segs) {
            try {
                if (path[0] == '\0')
                    // The path is absolute
                    return;

                gint ns = segs.length();
                gint f = 0; // Index of first segment
                while (f < ns) {
                    if (segs[f] >= 0)
                        break;
                    f++;
                }
                if ((f >= ns) || (f == 0))
                    // The path is empty, or else the original first segment survived,
                    // in which case we already know that no leading "." is needed
                    return;

                gint p = segs[f];
                while ((p < path.length()) && (path[p] != ':') && (path[p] != '\0')) p++;
                if (p >= path.length() || path[p] == '\0')
                    // No colon in first segment, so no "." needed
                    return;

                // At this point we know that the first segment is unused,
                // hence we can insert a "." segment at that position
                path[0] = '.';
                path[1] = '\0';
                segs[0] = 0;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::normalize(String const& ps) {
            try {
                // Does this path need normalization?
                gint ns = needsNormalization(ps); // Number of segments
                if (ns < 0)
                    // Nope -- just return it
                    return ps;

                CharArray path = ps.toChars(); // Path in char-array form

                // Split path into segments
                IntArray segs = IntArray(ns); // Segment-index array
                split(path, segs);

                // Remove dots
                removeDots(path, segs);

                // Prevent scheme-name confusion
                maybeAddLeadingDot(path, segs);

                // Join the remaining segments and return the result
                String s = String(path, 0, join(path, segs));
                if (s.equals(ps)) {
                    // string was already normalized
                    return ps;
                }
                return s;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool URI::match(gchar c, glong lowMask, glong highMask) {
            try {
                if (c == 0) // 0 doesn't have a slot in the mask. So, it never matches.
                    return false;
                if (c < 64)
                    return ((1L << c) & lowMask) != 0;
                if (c < 128)
                    return ((1L << (c - 64)) & highMask) != 0;
                return false;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        static CharArray const hexDigits = {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
        };

        using io::CharBuffer;
        using io::ByteBuffer;
        using charset::Charset;
        using charset::CharsetEncoder;
        using charset::CharsetDecoder;
        using charset::CharacterCodingException;
        using charset::CoderResult;
        using text::Normalizer;

        void URI::appendEscape(XString& sb, gbyte b) {
            try {
                sb.append('%');
                sb.append(hexDigits[(b >> 4) & 0x0f]);
                sb.append(hexDigits[(b >> 0) & 0x0f]);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::appendEncoded(CharsetEncoder& encoder, XString& sb, gchar c) {
            try {
                CharArray ca = {c};
                ByteBuffer& bb = encoder.encode(CharBuffer::wrap(ca));
                while (bb.hasRemaining()) {
                    gint b = bb.get() & 0xff;
                    if (b >= 0x80)
                        appendEscape(sb, CORE_CAST(gbyte, b));
                    else
                        sb.append((gchar) b);
                }
            } catch (CharacterCodingException const&) { CORE_ASSERT_AT(false, core::net::URI); }
            catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::quote(String const& s, glong lowMask, glong highMask) {
            try {
                XString sb;
                gbool allowNonASCII = ((lowMask & L_ESCAPED) != 0);
                for (gint i = 0; i < s.length(); i++) {
                    gchar c = s.charAt(i);
                    if (c < u'\u0080') {
                        if (!match(c, lowMask, highMask)) {
                            if (sb.isEmpty())
                                sb.append(s, 0, i);

                            appendEscape(sb, (gbyte) c);
                        } else {
                            if (!sb.isEmpty())
                                sb.append(c);
                        }
                    } else if (allowNonASCII
                        && (Character::isUnicodeSpace(c)
                            || Character::isISOControl(c))) {
                        CharsetEncoder& encoder = Charset::UTF_8.newEncoder();
                        if (sb.isEmpty()) {
                            sb.append(s, 0, i);
                        }
                        appendEncoded(encoder, sb, c);
                        UNSAFE::deleteInstance(encoder);
                    } else {
                        if (sb.isEmpty())
                            sb.append(c);
                    }
                }
                return (sb.isEmpty()) ? s : sb.toString();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::encode(String const& s) {
            try {
                gint n = s.length();
                if (n == 0)
                    return s;

                // First check whether we actually need to encode
                for (gint i = 0;;) {
                    if (s.charAt(i) >= u'\u0080')
                        break;
                    if (++i >= n)
                        return s;
                }

                String ns = text::Normalizer::normalize(s, Normalizer::Form::NFC);
                ByteBuffer& bb = Charset::UTF_8.newEncoder()
                                               .encode(CharBuffer::wrap(ns));

                XString sb;
                while (bb.hasRemaining()) {
                    gint b = bb.get() & 0xff;
                    if (b >= 0x80)
                        appendEscape(sb, CORE_CAST(gbyte, b));
                    else
                        sb.append((gchar) b);
                }
                return sb.toString();
            } catch (CharacterCodingException const&) { CORE_ASSERT_AT(false, core::net::URI); }
            catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::decode(gchar c) {
            try {
                if ((c >= '0') && (c <= '9'))
                    return c - '0';
                if ((c >= 'a') && (c <= 'f'))
                    return c - 'a' + 10;
                if ((c >= 'A') && (c <= 'F'))
                    return c - 'A' + 10;
                CORE_ASSERT(false);
                return -1;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbyte URI::decode(gchar c1, gchar c2) {
            try {
                return (gbyte) (((decode(c1) & 0xf) << 4)
                    | ((decode(c2) & 0xf) << 0));
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::decode(String const& s) {
            try {
                return decode(s, true);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        String URI::decode(String const& s, gbool ignorePercentInBrackets) {
            try {
                if (s == null)
                    return s;
                gint n = s.length();
                if (n == 0)
                    return s;
                if (s.indexOf('%') < 0)
                    return s;

                XString sb = XString(n);
                ByteBuffer& bb = ByteBuffer::allocate(n);
                CharBuffer& cb = CharBuffer::allocate(n);
                CharsetDecoder& dec = Charset::UTF_8.newDecoder()
                                                    .onMalformedInput(Charset::REPLACE)
                                                    .onUnmappableCharacter(Charset::REPLACE);

                // This is not horribly efficient, but it will do for now
                gchar c = s.charAt(0);
                gbool betweenBrackets = false;

                for (gint i = 0; i < n;) {
                    CORE_ASSERT(c == s.charAt(i)); // Loop invariant
                    if (c == '[') {
                        betweenBrackets = true;
                    } else if (betweenBrackets && c == ']') {
                        betweenBrackets = false;
                    }
                    if (c != '%' || (betweenBrackets && ignorePercentInBrackets)) {
                        sb.append(c);
                        if (++i >= n)
                            break;
                        c = s.charAt(i);
                        continue;
                    }
                    bb.clear();
                    for (;;) {
                        CORE_ASSERT(n - i >= 2);
                        bb.put(decode(s.charAt(++i), s.charAt(++i)));
                        if (++i >= n)
                            break;
                        c = s.charAt(i);
                        if (c != '%')
                            break;
                    }
                    bb.flip();
                    cb.clear();
                    dec.reset();
                    CoderResult cr = dec.decode(bb, cb, true);
                    CORE_ASSERT(cr.isUnderflow());
                    cr = dec.flush(cb);
                    CORE_ASSERT(cr.isUnderflow());
                    sb.append(cb.flip().toString());
                }

                return sb.toString();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        URI::Parser::Parser(URI& uri, String input): uri(uri), input(UNSAFE::moveInstance(input)) {}

        void URI::Parser::parse(gbool rsa) {
            try {
                requireServerAuthority = rsa;
                gint n = input.length();
                gint p = scan(0, n, "/?#"_S, ":"_S);
                if ((p >= 0) && at(p, n, ':')) {
                    if (p == 0)
                        failExpecting("scheme name"_S, 0);
                    checkChar(0, L_ALPHA, H_ALPHA, "scheme name"_S);
                    checkChars(1, p, L_SCHEME, H_SCHEME, "scheme name"_S);
                    uri.uri.scheme = input.subString(0, p);
                    p++; // Skip ':'
                    if (at(p, n, '/')) {
                        p = parseHierarchical(p, n);
                    } else {
                        // opaque; need to create the schemeSpecificPart
                        gint q = scan(p, n, "#"_S);
                        if (q <= p)
                            failExpecting("scheme-specific part"_S, p);
                        checkChars(p, q, L_URIC, H_URIC, "opaque part"_S);
                        uri.uri.schemeSpecific = input.subString(p, q);
                        p = q;
                    }
                } else {
                    p = parseHierarchical(0, n);
                }
                if (at(p, n, '#')) {
                    checkChars(p + 1, n, L_URIC, H_URIC, "fragment"_S);
                    uri.uri.fragment = input.subString(p + 1, n);
                    p = n;
                }
                if (p < n)
                    fail("end of URI"_S, p);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::parseHierarchical(gint start, gint n) {
            try {
                gint p = start;
                if (at(p, n, '/') && at(p + 1, n, '/')) {
                    p += 2;
                    gint q = scan(p, n, "/?#"_S);
                    if (q > p) {
                        p = parseAuthority(p, q);
                    } else if (q < n) {
                        // DEVIATION: Allow empty authority prior to non-empty
                        // path, query component or fragment identifier
                    } else
                        failExpecting("authority"_S, p);
                }
                gint q = scan(p, n, "?#"_S); // DEVIATION: May be empty
                checkChars(p, q, L_PATH, H_PATH, "path"_S);
                uri.uri.path = input.subString(p, q);
                p = q;
                if (at(p, n, '?')) {
                    p++;
                    q = scan(p, n, "#"_S);
                    checkChars(p, q, L_URIC, H_URIC, "query"_S);
                    uri.uri.query = input.subString(p, q);
                    p = q;
                }
                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::parseAuthority(gint start, gint n) {
            URISyntaxException const* ex = null;
            try {
                gint p = start;
                gint q = p;
                gint qreg = p;

                gbool serverChars = false;
                gbool regChars = false;
                gbool skipParseException = false;

                if (scan(p, n, "]"_S) > p) {
                    // contains a literal IPv6 address, therefore % is allowed
                    serverChars = (scan(p, n, L_SERVER_PERCENT, H_SERVER_PERCENT) == n);
                } else {
                    serverChars = (scan(p, n, L_SERVER, H_SERVER) == n);
                }
                regChars = ((qreg = scan(p, n, L_REG_NAME, H_REG_NAME)) == n);

                if (regChars && !serverChars) {
                    // Must be a registry-based authority
                    uri.uri.authority = input.subString(p, n);
                    return n;
                }

                // When parsing a URI, skip creating exception objects if the server-based
                // authority is not required and the registry parse is successful.
                //
                skipParseException = (!requireServerAuthority && regChars);
                if (serverChars) {
                    // Might be (probably is) a server-based authority, so attempt
                    // to parse it as such.  If the attempt fails, try to treat it
                    // as a registry-based authority.
                    try {
                        q = parseServer(p, n, skipParseException);
                        if (q < n) {
                            if (skipParseException) {
                                uri.uri.userInfo = { };
                                uri.uri.host = { };
                                uri.uri.port = -1;
                                q = p;
                            } else {
                                failExpecting("end of authority"_S, q);
                            }
                        } else {
                            uri.uri.authority = input.subString(p, n);
                        }
                    } catch (URISyntaxException const& x) {
                        // Undo results of failed parse
                        uri.uri.userInfo = { };
                        uri.uri.host = { };
                        uri.uri.port = -1;
                        if (requireServerAuthority) {
                            // If we're insisting upon a server-based authority,
                            // then just re-throw the exception
                            ex = &x;
                            goto END;
                        } else {
                            // Save the exception in case it doesn't parse as a
                            // registry either
                            ex = &x;
                            q = p;
                        }
                    }
                }

                if (q < n) {
                    if (regChars) {
                        // Registry-based authority
                        uri.uri.authority = input.subString(p, n);
                    } else if (ex != null) {
                        // Re-throw exception; it was probably due to
                        // a malformed IPv6 address
                        goto END;
                    } else {
                        fail("Illegal character in authority"_S, serverChars ? q : qreg);
                    }
                }

                return n;
            } catch (Throwable const& ex2) { ex2.throws($ftrace()); }
        END:
            ex->throws($ftrace());
        }

        gint URI::Parser::parseServer(gint start, gint n, gbool skipParseException) {
            try {
                gint p = start;
                gint q = 0;

                // userinfo
                q = scan(p, n, "/?#"_S, "@"_S);
                if ((q >= p) && at(q, n, '@')) {
                    checkChars(p, q, L_USERINFO, H_USERINFO, "user info"_S);
                    uri.uri.userInfo = input.subString(p, q);
                    p = q + 1; // Skip '@'
                }

                // hostname, IPv4 address, or IPv6 address
                if (at(p, n, '[')) {
                    // DEVIATION from RFC2396: Support IPv6 addresses, per RFC2732
                    p++;
                    q = scan(p, n, "/?#"_S, "]"_S);
                    if ((q > p) && at(q, n, ']')) {
                        // look for a "%" scope id
                        gint r = scan(p, q, "%"_S);
                        if (r > p) {
                            parseIPv6Reference(p, r);
                            if (r + 1 == q) {
                                fail("scope id expected"_S);
                            }
                            checkChars(r + 1, q, L_SCOPE_ID, H_SCOPE_ID,
                                       "scope id"_S);
                        } else {
                            parseIPv6Reference(p, q);
                        }
                        uri.uri.host = input.subString(p - 1, q + 1);
                        p = q + 1;
                    } else {
                        failExpecting("closing bracket for IPv6 address"_S, q);
                    }
                } else {
                    q = parseIPv4Address(p, n);
                    if (q <= p)
                        q = parseHostName(p, n, skipParseException);
                    p = q;
                }

                // port
                if (at(p, n, ':')) {
                    p++;
                    q = scan(p, n, "/"_S);
                    if (q > p) {
                        checkChars(p, q, L_DIGIT, H_DIGIT, "port number"_S);
                        try {
                            uri.uri.port = Integer::parseInt(input, p, q, 10);
                        } catch (NumberFormatException const&) {
                            fail("Malformed port number"_S, p);
                        }
                        p = q;
                    }
                } else if (p < n && skipParseException) {
                    return p;
                }

                if (p < n)
                    failExpecting("port number"_S, p);

                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scanByte(gint start, gint n) const {
            try {
                gint p = start;
                gint q = scan(p, n, L_DIGIT, H_DIGIT);
                if (q <= p) return q;
                if (Integer::parseInt(input, p, q, 10) > 255) return p;
                return q;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scanIPv4Address(gint start, gint n, gbool strict) const {
            try {
                gint p = start;
                gint q;
                gint m = scan(p, n, L_DIGIT | L_DOT, H_DIGIT | H_DOT);
                if ((m <= p) || (strict && (m != n)))
                    return -1;
                for (;;) {
                    // Per RFC2732: At most three digits per byte
                    // Further constraint: Each element fits in a byte
                    if ((q = scanByte(p, m)) <= p) break;
                    p = q;
                    if ((q = scan(p, m, '.')) <= p) break;
                    p = q;
                    if ((q = scanByte(p, m)) <= p) break;
                    p = q;
                    if ((q = scan(p, m, '.')) <= p) break;
                    p = q;
                    if ((q = scanByte(p, m)) <= p) break;
                    p = q;
                    if ((q = scan(p, m, '.')) <= p) break;
                    p = q;
                    if ((q = scanByte(p, m)) <= p) break;
                    p = q;
                    if (q < m) break;
                    return q;
                }
                fail("Malformed IPv4 address"_S, q);
                return -1;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::takeIPv4Address(gint start, gint n, String const& expected) const {
            try {
                gint p = scanIPv4Address(start, n, true);
                if (p <= start)
                    failExpecting(expected, start);
                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::parseIPv4Address(gint start, gint n) const {
            try {
                gint p;

                try {
                    p = scanIPv4Address(start, n, false);
                } catch (URISyntaxException const&) {
                    return -1;
                } catch (NumberFormatException const&) {
                    return -1;
                }

                if (p > start && p < n) {
                    // IPv4 address is followed by something - check that
                    // it's a ":" as this is the only valid character to
                    // follow an address.
                    if (input.charAt(p) != ':') {
                        p = -1;
                    }
                }

                if (p > start)
                    uri.uri.host = input.subString(start, p);

                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::parseHostName(gint start, gint n, gbool skipParseException) const {
            try {
                gint p = start;
                gint q = 0;
                gint l = -1; // Start of last parsed label

                do {
                    // domainlabel = alphanum [ *( alphanum | "-" ) alphanum ]
                    q = scan(p, n, L_ALPHANUM, H_ALPHANUM);
                    if (q <= p)
                        break;
                    l = p;
                    p = q;
                    q = scan(p, n, L_ALPHANUM | L_DASH, H_ALPHANUM | H_DASH);
                    if (q > p) {
                        if (input.charAt(q - 1) == '-')
                            fail("Illegal character in hostname"_S, q - 1);
                        p = q;
                    }
                    q = scan(p, n, '.');
                    if (q <= p)
                        break;
                    p = q;
                } while (p < n);

                if ((p < n) && !at(p, n, ':')) {
                    if (skipParseException) {
                        return p;
                    }
                    fail("Illegal character in hostname"_S, p);
                }
                if (l < 0)
                    failExpecting("hostname"_S, start);

                // for a fully qualified hostname check that the rightmost
                // label starts with an alpha character.
                if (l > start && !match(input.charAt(l), L_ALPHA, H_ALPHA)) {
                    fail("Illegal character in hostname"_S, l);
                }

                uri.uri.host = input.subString(start, p);
                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::parseIPv6Reference(gint start, gint n) {
            try {
                gint p = start;
                gint q = 0;
                gbool compressedZeros = false;

                q = scanHexSeq(p, n);

                if (q > p) {
                    p = q;
                    if (at(p, n, "::"_S)) {
                        compressedZeros = true;
                        p = scanHexPost(p + 2, n);
                    } else if (at(p, n, ':')) {
                        p = takeIPv4Address(p + 1, n, "IPv4 address"_S);
                        ipv6byteCount += 4;
                    }
                } else if (at(p, n, "::"_S)) {
                    compressedZeros = true;
                    p = scanHexPost(p + 2, n);
                }
                if (p < n)
                    fail("Malformed IPv6 address"_S, start);
                if (ipv6byteCount > 16)
                    fail("IPv6 address too long"_S, start);
                if (!compressedZeros && ipv6byteCount < 16)
                    fail("IPv6 address too short"_S, start);
                if (compressedZeros && ipv6byteCount == 16)
                    fail("Malformed IPv6 address"_S, start);

                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scanHexPost(gint start, gint n) {
            try {
                gint p = start;
                gint q = 0;

                if (p == n)
                    return p;

                q = scanHexSeq(p, n);
                if (q > p) {
                    p = q;
                    if (at(p, n, ':')) {
                        p++;
                        p = takeIPv4Address(p, n, "hex digits or IPv4 address"_S);
                        ipv6byteCount += 4;
                    }
                } else {
                    p = takeIPv4Address(p, n, "hex digits or IPv4 address"_S);
                    ipv6byteCount += 4;
                }
                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scanHexSeq(gint start, gint n) {
            try {
                gint p = start;
                gint q = 0;

                q = scan(p, n, L_HEX, H_HEX);
                if (q <= p)
                    return -1;
                if (at(q, n, '.')) // Beginning of IPv4 address
                    return -1;
                if (q > p + 4)
                    fail("IPv6 hexadecimal digit sequence too long"_S, p);
                ipv6byteCount += 2;
                p = q;
                while (p < n) {
                    if (!at(p, n, ':'))
                        break;
                    if (at(p + 1, n, ':'))
                        break; // "::"
                    p++;
                    q = scan(p, n, L_HEX, H_HEX);
                    if (q <= p)
                        failExpecting("digits for an IPv6 address"_S, p);
                    if (at(q, n, '.')) {
                        // Beginning of IPv4 address
                        p--;
                        break;
                    }
                    if (q > p + 4)
                        fail("IPv6 hexadecimal digit sequence too long"_S, p);
                    ipv6byteCount += 2;
                    p = q;
                }

                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::Parser::fail(String const& reason) const {
            URISyntaxException(input, reason).throws($ftrace());
        }

        void URI::Parser::fail(String const& reason, gint p) const {
            URISyntaxException(input, reason, p).throws($ftrace());
        }

        void URI::Parser::failExpecting(String const& expected, gint p) const {
            fail("Expected "_S + expected, p);
        }

        gbool URI::Parser::at(gint start, gint end, gchar c) const {
            try {
                return (start < end) && (input.charAt(start) == c);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool URI::Parser::at(gint start, gint end, String const& s) {
            try {
                gint p = start;
                gint sn = s.length();
                if (sn > end - p)
                    return false;
                gint i = 0;
                while (i < sn) {
                    if (input.charAt(p++) != s.charAt(i)) {
                        break;
                    }
                    i++;
                }
                return (i == sn);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scan(gint start, gint end, gchar c) const {
            try {
                if ((start < end) && (input.charAt(start) == c))
                    return start + 1;
                return start;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scan(gint start, gint end, String const& err, String const& stop) const {
            try {
                gint p = start;
                while (p < end) {
                    gchar c = input.charAt(p);
                    if (err.indexOf(c) >= 0)
                        return -1;
                    if (stop.indexOf(c) >= 0)
                        break;
                    p++;
                }
                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scan(gint start, gint end, String const& stop) const {
            try {
                gint p = start;
                while (p < end) {
                    gchar c = input.charAt(p);
                    if (stop.indexOf(c) >= 0)
                        break;
                    p++;
                }
                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scanEscape(gint start, gint n, gchar first) const {
            try {
                gint p = start;
                gchar c = first;
                if (c == '%') {
                    // Process escape pair
                    if ((p + 3 <= n)
                        && match(input.charAt(p + 1), L_HEX, H_HEX)
                        && match(input.charAt(p + 2), L_HEX, H_HEX)) {
                        return p + 3;
                    }
                    fail("Malformed escape pair"_S, p);
                } else if ((c > 128)
                    && !Character::isUnicodeSpace(c)
                    && !Character::isISOControl(c)) {
                    // Allow unescaped but visible non-US-ASCII chars
                    return p + 1;
                }
                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint URI::Parser::scan(gint start, gint n, glong lowMask, glong highMask) const {
            try {
                gint p = start;
                while (p < n) {
                    gchar c = input.charAt(p);
                    if (match(c, lowMask, highMask)) {
                        p++;
                        continue;
                    }
                    if ((lowMask & L_ESCAPED) != 0) {
                        gint q = scanEscape(p, n, c);
                        if (q > p) {
                            p = q;
                            continue;
                        }
                    }
                    break;
                }
                return p;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::Parser::checkChars(gint start, gint end, glong lowMask, glong highMask, String const& what) {
            try {
                gint p = scan(start, end, lowMask, highMask);
                if (p < end)
                    fail("Illegal character in "_S + what, p);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        void URI::Parser::checkChar(gint p, glong lowMask, glong highMask, String const& what) {
            try {
                checkChars(p, p + 1, lowMask, highMask, what);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // net
} // core
