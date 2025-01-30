//
// Created by bruns on 14/05/2024.
//

#include <core/IllegalArgumentException.h>
#include <core/Trace.h>
#include <core/XString.h>
#include <core/misc/Unsafe.h>

namespace core {
    Trace::Trace(String const& moduleName, String const& moduleVersion,
                 String const& className, String const& methodName,
                 String const& fileName, gint lineNumber) {
        // :::::::::::::::::::::::::::::::: ANALYZING :::::::::::::::::::::::::::::::::::::::::::::::::::::
    MODULE_ANALYZING:
        module = resolve(moduleName);
    MODULE_VERSION_ANALYZING:
        version = resolve(moduleVersion);
    CLASS_ANALYZING:
        declaringClass = resolve(className);
        if (declaringClass.indexOf('~') >= 0)
            throw IllegalArgumentException("Invalid class name"_S);
        if (declaringClass.indexOf('.') >= 0)
            declaringClass = declaringClass.replace("."_S, "::"_S);
    METHOD_ANALYZING:
        method = resolve(methodName);
        // if (method.indexOf('~') >= 0)
        //     throw IllegalArgumentException("Invalid method name"_S);
        if (method.indexOf('.') >= 0)
            method = method.replace("."_S, "::"_S);
        // ::::::::::::::::::::::::::::::::::: NORMALIZING ::::::::::::::::::::::::::::::::::::::::::::::::::
    CLASS_METHOD_NORMALIZING: {
            String c, m;
            if (declaringClass.isEmpty() && method.indexOf("::"_S) >= 0) {
                gint i = method.lastIndexOf("::"_S);
                c = method.subString(0, i);
                m = method.subString(i + 2);
            } else
                m = normalize(declaringClass, method);

            if (!c.isEmpty())
                declaringClass = UNSAFE::moveInstance(c);
            if (!m.isEmpty())
                method = UNSAFE::moveInstance(m);
        }
        // ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
        file = fileName;
        line = lineNumber;
    }

    Trace::Trace(String const& className, String const& methodName, String const& fileName, gint lineNumber)
        : Trace(String(), String(), className, methodName, fileName, lineNumber) {}

    Trace::Trace(String const& functionName, String const& fileName, gint lineNumber)
        : Trace(String(), String(), String(), functionName, fileName, lineNumber) {}

    String Trace::moduleName() const { return module; }

    String Trace::moduleVersion() const { return version; }

    String Trace::className() const { return declaringClass; }

    String Trace::methodName() const { return method; }

    String Trace::functionName() const { return methodName(); }

    String Trace::fileName() const { return file; }

    gint Trace::lineNumber() const { return line; }

    gbool Trace::isNativeMethod() const { return line == -2; }

    String Trace::toString() const {
        XString str(12 + module.length() + version.length() + declaringClass.length() + method.length());
        if (!module.isEmpty()) {
            str.append(module);
            if (!version.isEmpty()) {
                str.append(u'@').append(version);
            }

            if (str.length() > 0) {
                str.append(u'/');
            }
        }

        if (!declaringClass.isEmpty())
            str.append(declaringClass).append("::"_S);

        str.append(method).append(u'(');

        if (isNativeMethod()) {
            str.append("#"_S);
        } else {
            file.isEmpty() ? str.append(u'?') : str.append(file);
            if (line > 0) {
                str.append(u':').append(line);
            }
        }
        str.append(u')');


        return str.toString();
    }

    gbool Trace::equals(Object const& obj) const {
        if (this == &obj) return true;

        if (!Class<Trace>::hasInstance(obj)) return false;

        Trace const& trace = CORE_XCAST(Trace const, obj);
        return module.equals(trace.module) &&
                version.equals(trace.version) &&
                declaringClass.equals(trace.declaringClass) &&
                method.equals(trace.method) &&
                file.equals(trace.file) &&
                line == trace.line;
    }

    gint Trace::hash() const {
        gint hash = 31 * module.hash() + version.hash();
        hash = 31 * hash + declaringClass.hash();
        hash = 31 * hash + method.hash();
        hash = 31 * hash + file.hash();
        hash = 31 * hash + line;

        return hash;
    }

    static gint SearchComplement1(String const& s, gint start) {
        gint level = 0;
        for (gint i = start + 1; i < s.length() && level >= 0; i++) {
            gchar c = s.charAt(i);
            if (c == '<') {
                if (i + 1 < s.length() && s.charAt(i + 1) == '<') {
                    i += 1;
                    continue;
                }
                level += 1;
            } else if (c == '>') {
                level -= 1;
                if (level == -1)
                    return i;
            }
        }
        return s.length();
    }

    static gint SearchComplement2(String const& s, gint start) {
        gint level = 0;
        for (gint i = start + 1; i < s.length() && level >= 0; i++) {
            gchar c = s.charAt(i);
            if (c == '(') {
                if (i + 1 < s.length() && s.charAt(i + 1) == '(') {
                    i += 1;
                    continue;
                }
                level += 1;
            } else if (c == ')') {
                level -= 1;
                if (level == -1)
                    return i;
            }
        }
        return s.length();
    }

    static gint SearchComplement3(String const& s, gint start) {
        gint level = 0;
        for (gint i = start + 1; i < s.length() && level >= 0; i++) {
            gchar c = s.charAt(i);
            if (c == '[') {
                if (i + 1 < s.length() && s.charAt(i + 1) == '[') {
                    i += 1;
                    continue;
                }
                level += 1;
            } else if (c == ']') {
                level -= 1;
                if (level == -1)
                    return i;
            }
        }
        return s.length();
    }

    String Trace::resolve(String const& mc) {
        String s = mc.strip();
        if (s.isEmpty())
            return s;

        String r = { };
        // Remove all template specifications (A<.*> => A)
        // Find the first index of opening character '<'
    GENERALIZATION: {
            gint i = 0;
            gint j = 0;
            gbool op = false;
            while ((i = s.indexOf('<', j)) >= j) {
                if (i + 1 < s.length() && !op && s.charAt(i + 1) == '<') {
                    // operator <<(.*)...
                    gint k = s.lastIndexOf("operator <<"_S, i + 1);
                    if (k < 0)
                        k = s.lastIndexOf("operator<<"_Sl, i + 1);
                    if (k >= 0) {
                        i += 2;
                        r += s.subString(j, i);
                        j = i;
                        op = true;
                        continue;
                    }
                } else {
                    // <lambda(...)>
                    gint k = s.indexOf("lambda"_Sl, i + 1);
                    if (k == i + 1) {
                        // We'll be replace <lambda(...)> with <lambda>
                        // into another method: method(...)::<lambda(...)> -> method#<lambda>
                        // into class or namespace: class::<lambda(...)> -> class::<lambda>
                        k = SearchComplement1(s, i);
                        if (k >= 0) {
                            if (r.isEmpty() || r.isBlank())
                                r += "<lambda>"_Sl;
                            else if (i - 3 > 0 && s.charAt(i - 3) == ')')
                                r += "#<lambda>"_Sl;
                            else
                                r += "::<lambda>"_Sl;
                            j = k + 1;
                            continue;
                        }
                    }
                }

                // class<.*>...
                // Search the index of closing character '>'
                op = false;
                gint k = SearchComplement1(s, i);
                if (k == s.length()) {
                    throw IllegalArgumentException("Malformed name after index "_S + i);
                    return { };
                }
                r += s.subString(j, i);
                j = k + 1;
            }
            if (j > 0) {
                if (j + 1 <= s.length())
                    r += s.subString(j);
                s = UNSAFE::moveInstance(r);
                r = String();
            }
        }
        // Remove all Function arguments specification (fn(.*) => fn)
        // Find the first index of opening character '('
    EXTRACTION: {
            gint i = 0;
            gint j = 0;
            gbool op = false;
            while ((i = s.indexOf('(', j)) >= j) {
                if (i + 1 < s.length() && !op && s.charAt(i + 1) == ')') {
                    // operator ()(.*)...
                    gint k = s.lastIndexOf("operator ()"_S, i + 1);
                    if (k < 0)
                        k = s.lastIndexOf("operator()"_S, i + 1);
                    if (k >= 0) {
                        i += 2;
                        r += s.subString(j, i);
                        j = i;
                        op = true;
                        continue;
                    }
                }

                // Search the index of closing character ')'
                op = false;
                gint k = SearchComplement2(s, i);
                if (k == s.length()) {
                    throw IllegalArgumentException("Malformed name after index "_S + i);
                    return { };
                }
                r += s.subString(j, i);
                j = k + 1;
            }
            if (j > 0) {
                if (j + 1 <= s.length())
                    r += s.subString(j);
                s = UNSAFE::moveInstance(r);
                r = String();
            }
        }
        // Remove Mingw Deduction for template types (name [with .*] => name)
        // Find the index of character '[' followed by "with "
    SIMPLICATION: {
            gint i = 0;
            gint j = 0;
            gbool op = false;
            while ((i = s.indexOf('[', j)) >= j) {
                if (i + 1 < s.length() && !op && s.charAt(i + 1) == ']') {
                    // operator [](.*)...
                    gint k = s.lastIndexOf("operator []"_S, i + 1);
                    if (k < 0)
                        k = s.lastIndexOf("operator[]"_S, i + 1);
                    if (k >= 0) {
                        i += 2;
                        r += s.subString(j, i);
                        j = i;
                        op = true;
                        continue;
                    }
                }

                // class [with .*]
                // Search the index of closing character ']'
                op = false;
                gint k = SearchComplement3(s, i);
                if (k == s.length()) {
                    throw IllegalArgumentException("Malformed name after index "_S + i);
                    return { };
                }
                r += s.subString(j, i);
                j = k + 1;
            }
            if (j > 0) {
                if (j + 1 <= s.length())
                    r += s.subString(j);
                s = UNSAFE::moveInstance(r);
                r = String();
            }
        }

        // Remove all type/function/method decorators
        // (static, class, struct, enum, union, __cdecl, __stdcall,
        // const, volatile, &, &&, *,
        // constexpr, nothrow, constval
        // ...)
        // static __cdecl int function -> int function -> function
    SLIMING: {
            gint i = 0;
            gint j = 0;
            while ((i = s.indexOf(' ', j)) >= j) {
                String decorator = s.subString(j, i);
                j = i + 1;
                gint n = decorator.length();
                gchar c = decorator.charAt(0);
                switch (c) {
                    case '&':
                        if (n == 1)
                            continue;
                        if (n == 2 && decorator.charAt(1) == '&')
                            continue;
                        r += decorator + " "_S;
                        break;
                    case 'c':
                        if (n == 5 && (decorator.equals("const"_S) || decorator.equals("class"_S)))
                            continue;
                        if (n == 8 && decorator.equals("constval"_S))
                            continue;
                        if (n == 9 && decorator.equals("constexpr"_S))
                            continue;
                        r += decorator + " "_S;
                        break;
                    case 'e':
                        if (n == 4 && decorator.equals("enum"_S))
                            continue;
                        r += decorator + " "_S;
                        break;
                    case 'n':
                        if (n == 7 && decorator.equals("nothrow"_S))
                            continue;
                        r += decorator + " "_S;
                        break;
                    case 's':
                        if (n == 6 && (decorator.equals("struct"_S) || decorator.equals("static"_S)))
                            continue;
                        r += decorator + " "_S;
                    case 't':
                        if (n == 5 && decorator.equals("throw"_S))
                            continue;
                        r += decorator + " "_S;
                    case 'u':
                        if (n == 5 && decorator.equals("union"_S))
                            continue;
                        r += decorator + " "_S;
                        break;
                    case 'v':
                        if (n == 7 && decorator.equals("virtual"_S))
                            continue;
                        if (n == 8 && decorator.equals("volatile"_S))
                            continue;
                        r += decorator + " "_S;
                        break;
                    case '_':
                        if (n == 7 && decorator.equals("__cdecl"_S))
                            continue;
                        if (n == 9 && decorator.equals("__stdcall"_S))
                            continue;
                        r += decorator + " "_S;
                        break;
                    default:
                        r += decorator + " "_S;
                        break;
                }
            }
            if (j > 0 && j < s.length()) {
                // The last part (const, volatile, __???...)
                String decorator = s.subString(j);
                gint n = decorator.length();
                gchar c = decorator.charAt(0);
                switch (c) {
                    case '&':
                        if (n == 1)
                            break;
                        if (n == 2 && decorator.charAt(1) == '&')
                            break;
                        r += decorator;
                        break;
                    case 'c':
                        if (n == 5 && (decorator.equals("const"_S) || decorator.equals("class"_S)))
                            break;
                        if (n == 8 && decorator.equals("constval"_S))
                            break;
                        if (n == 9 && decorator.equals("constexpr"_S))
                            break;
                        r += decorator;
                        break;
                    case 'e':
                        if (n == 4 && decorator.equals("enum"_S))
                            break;
                        r += decorator;
                        break;
                    case 'n':
                        if (n == 7 && decorator.equals("nothrow"_S))
                            break;
                        r += decorator;
                        break;
                    case 's':
                        if (n == 6 && (decorator.equals("struct"_S) || decorator.equals("static"_S)))
                            break;
                        r += decorator;
                    case 't':
                        if (n == 5 && decorator.equals("throw"_S))
                            break;
                        r += decorator;
                    case 'u':
                        if (n == 5 && decorator.equals("union"_S))
                            break;
                        r += decorator;
                        break;
                    case 'v':
                        if (n == 7 && decorator.equals("virtual"_S))
                            break;
                        if (n == 8 && decorator.equals("volatile"_S))
                            break;
                        r += decorator;
                        break;
                    case '_':
                        if (n == 7 && decorator.equals("__cdecl"_S))
                            break;
                        if (n == 9 && decorator.equals("__stdcall"_S))
                            break;
                        r += decorator;
                        break;
                    default:
                        r += decorator;
                        break;
                }
                j = s.length();
            }
            if (j > 0) {
                if (j + 1 <= s.length())
                    r += s.subString(j);
                s = UNSAFE::moveInstance(r);
                if (s.lastIndexOf(' ') == s.length() - 1)
                    s = s.stripTrailing();
                r = String();
            }

            // Remove return type. (int function -> function)
            if ((i = s.indexOf(' ')) > 0) {
                r = s.subString(i + 1);
                if (!r.isEmpty())
                    s = UNSAFE::moveInstance(r);
                else
                    s = s.subString(0, i);
            }
            r = String();

            // Remove return type. (function& -> function) and (function&& -> function)
            i = 0;
            j = 0;
            while ((i = s.indexOf('&', j)) >= j) {
                if (i == j) {
                    j += 1;
                    continue;
                }
                r += s.subString(j, i);
                if (!r.isEmpty())
                    s = UNSAFE::moveInstance(r);
                else
                    s = s.subString(0, i);
                j = i + 1;
            }

            if (j > 0) {
                if (j + 1 <= s.length())
                    r += s.subString(j);
                s = UNSAFE::moveInstance(r);
                r = String();
            }

            if (s.isEmpty())
                return s;
        }


        // Analyze the result
    ANALYZING: {
            gbool letter = false;
            gbool separator = false;
            for (gint i = 0; i < s.length(); i++) {
                gchar c = s.charAt(i);
                if (c > 0x7F) {
                    throw IllegalArgumentException("Non-ASCII name at index "_S + i);
                    return String();
                }
                if (Character::isLetterOrDigit(c) || c == '$' || c == '_') {
                    if (Character::isDigit(c) && !letter) {
                        throw IllegalArgumentException("Malformed name at index "_S + i);
                    }
                    letter = true;
                    separator = false;
                    continue;
                }

                if (c == ':') {
                    if (i + 1 < s.length() && s.charAt(i + 1) == ':') {
                        separator = true;
                        letter = false;
                        i += 1;
                        continue;
                    }

                    throw IllegalArgumentException("Malformed name at index "_S + i);
                }

                if (c == '(' || c == ')' ||
                    c == '<' || c == '>' ||
                    c == '[' || c == ']' ||
                    c == '.' || c == '~' || // For module version only
                    c == '{' || c == '}')
                    continue;

                if (c == '#' && i + 1 < s.length() && s.charAt(i + 1) == '<')
                    continue;

                throw IllegalArgumentException("Malformed name at index "_S + i);
                return String();
            }
        }

        return s;
    }

    String Trace::normalize(String const& c, String const& m) {
        if (c.isEmpty()) {
            if (m == "operator()"_S)
                return "operator ()"_S;

            return m;
        }

        gint i = m.indexOf(c);
        if (i + c.length() == m.length())
            return String();

        if (i + c.length() + 2 <= m.length())
            return m.subString(i + c.length() + 2);

        String r;
        i = m.lastIndexOf(':');
        if (i <= 0)
            return m;

        if (m.charAt(i - 1) == ':') {
            r = m.subString(i - 1);
            if (r == "::operator ()"_S)
                return c + r;

            if (r == "::operator()"_S)
                return c + "::operator ()"_S;

            return m;
        }

        return m;
    }

    Object& Trace::clone() const {
        try {
            return UNSAFE::newInstance<Trace>(*this);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
} // core
