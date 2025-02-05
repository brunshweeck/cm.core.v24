//
// Created by bruns on 14/05/2024.
//

#include <core/Array.h>
#include <core/IllegalStateException.h>
#include <native/Console.h>
#include <core/Throwable.h>
#include <core/UnsupportedOperationException.h>
#include <core/XString.h>
#include <core/util/Arrays.h>

namespace core {
    CORE_WARNING_PUSH
    CORE_WARNING_DISABLE_DEPRECATED

    Throwable::Throwable(): backtrace(initStacktrace()) {
        origin = null;
    }

    Throwable::Throwable(const String& message): Throwable() {
        detailMessage = message;
    }

    Throwable::Throwable(const String& message, const Throwable& cause): Throwable() {
        detailMessage = message;
        try {
            setCause(cause);
        } catch (CloneNotSupportedException const& ex) {
            throw IllegalArgumentException("non-cloneable throwable cause", ex);
        }
    }

    Throwable::Throwable(Throwable const& other): Throwable() {
        switch (other.status) {
            case Status::NEW:
            case Status::READY:
                detailMessage = other.detailMessage;
                origin = other.origin;
                backtrace = other.backtrace;
                status = other.status;
                break;
            case Status::CLONE:
                detailMessage = UNSAFE::moveInstance(other.detailMessage);
                UNSAFE::swapValues(origin, CORE_CAST(Throwable &, other).origin);
                backtrace = UNSAFE::moveInstance(other.backtrace);
                status = Status::READY;
            // UNSAFE::deleteInstance(CORE_CAST(Throwable &, other));
                break;
            default:
                throw IllegalArgumentException("Corrupted throwable object");
        }
    }

    String Throwable::message() const {
        return detailMessage;
    }

    String Throwable::localizedMessage() const {
        return message();
    }

    Throwable const& Throwable::cause() const {
        if (origin)
            return *origin;

        return *this;
    }

    Throwable& Throwable::setCause(Throwable const& cause) {
        if (origin)
            throw IllegalArgumentException("Can't overwrite cause with "_S + cause);

        if (this == &cause) {
            throw IllegalArgumentException("Self-causation not permitted", *this);
        }
        try {
            origin = &UNSAFE::copyInstance(cause, cause.status == Status::CLONE);
        } catch (CloneNotSupportedException const& ex) {
            IllegalArgumentException("non-cloneable throwable cause", ex).throws($xtrace());
        } catch (Throwable const& ex) {
            ex.throws($xtrace());
        }
        return *this;
    }

    String Throwable::toString() const {
        String type = typeName(*this);
        String message = localizedMessage();
        if (message.isEmpty())
            return type;
        return type + ": "_S + message;
    }

    void Throwable::dumpStackTrace() const {
        ORIGIN ex = CORE_CAST(ORIGIN, this);
        XString xs = XString(256);

        String LF = "\n";
        String TAB = "\t";
        String SEP = "at ";
        String CAUSED_BY = "CausedBy ";
        if (!ex)
            goto PRINTING;
        // what():
        // ...

        xs.append(LF);
        while (ex) {
            xs.append(*ex);

            for (Trace const& t : ex->backtrace) {
                xs.append(LF);
                xs.append(TAB);
                xs.append(SEP);
                xs.append(t);
            }

            xs.append(LF);
            xs.append(TAB);
            xs.append(SEP);
            xs.append("...");
            if (ex->origin && ex->origin != this) {
                xs.append(LF);
                xs.append(CAUSED_BY);
                ex = ex->origin;
                continue;
            }
            break;
        }
        xs.append(LF);

    PRINTING:
        String out = xs.toString();
        ByteArray bytes = out.toBytes();
        NativeConsole::WriteError(bytes);
    }

    Array<Trace> Throwable::stacktrace() const {
        return backtrace;
    }

    void Throwable::throws(Trace const& currentExecutionPoint) const CORE_XNOEXCEPT(0) {
        ORIGIN ex = null;
        Throwable const& self = *this;

        switch (status) {
            case Status::NEW: {
                try {
                    Throwable& clone = CORE_XCAST(Throwable, self.clone());
                    clone.status = Status::CLONE;
                    clone.backtrace = util::Arrays::copyOf(backtrace, backtrace.length() + 1);
                    clone.backtrace.set(backtrace.length(), currentExecutionPoint);
                    ex = &clone;
                } catch (CloneNotSupportedException const& cne) {
                    IllegalStateException("Couldn't thrown non-cloneable object."_S, cne).throws($ftrace());
                } catch (Throwable const& t) { Error(t).throws($ftrace()); }
                break;
            }
            case Status::READY:
                try {
                    ex = CORE_CAST(ORIGIN, this);
                    ex->backtrace = util::Arrays::copyOf(backtrace, backtrace.length() + 1);
                    ex->backtrace.set(backtrace.length() - 1, currentExecutionPoint);
                } catch (Throwable const& t) { Error(t).throws($ftrace()); }
                break;
            default:
                ex = CORE_CAST(ORIGIN, this);
                break;
        }

        if (ex)
            ex->selfThrow();

        CORE_UNREACHABLE();
    }

    void Throwable::rethrow() const CORE_XNOEXCEPT(0) {
        ORIGIN ex = null;
        Throwable const& self = *this;

        switch (status) {
            case Status::NEW: {
                try {
                    Throwable& clone = CORE_XCAST(Throwable, self.clone());
                    clone.status = Status::CLONE;
                    ex = &clone;
                } catch (CloneNotSupportedException const& cne) {
                    IllegalStateException("Couldn't thrown non-cloneable object."_S, cne).throws($ftrace());
                } catch (Throwable const& t) { Error(t).throws($ftrace()); }
                break;
            }
            case Status::READY:
            default:
                ex = CORE_CAST(ORIGIN, this);
                break;
        }

        if (ex)
            ex->selfThrow();

        CORE_UNREACHABLE();
    }

    void Throwable::selfThrow() const {
        UnsupportedOperationException().throws($ftrace());
    }

    Throwable::~Throwable() {
        UNSAFE::deleteInstance(backtrace);
        status = CORE_CAST(Status, 7);
    }

    Throwable::ByteStream Throwable::what() const CORE_NOTHROW {
        ORIGIN ex = CORE_CAST(ORIGIN, this);
        XString xs = XString(256);

        String LF = "\n";
        String TAB = "\t";
        String SEP = "at ";
        String CAUSED_BY = "CausedBy ";
        if (!ex)
            goto PRINTING;

        xs.append(LF);
    WRITING:
        xs.append(*ex);

        for (Trace const& t : ex->backtrace) {
            xs.append(LF);
            xs.append(TAB);
            xs.append(SEP);
            xs.append(t);
        }

        xs.append(LF);
        xs.append(TAB);
        xs.append(SEP);
        xs.append("..."_Sl);
        xs.append(LF);
        if (ex->origin && ex->origin != this) {
            xs.append(LF);
            xs.append(CAUSED_BY);
            ex = ex->origin;
            goto WRITING;
        }

    PRINTING:
        String out = xs.toString();
        ByteArray bytes = out.toBytes();

        // #ifdef CORE_COMPILER_MSVC
        //         NativeConsole::WriteError(bytes);
        // #endif

        try {
            char* str = CORE_CAST(char*, UNSAFE::allocateMemory(bytes.length()));
            gint i = 0;
            if (str)
                for (gbyte b : bytes) str[i++] = b;

            return str;
        } catch (...) {
            return "";
        }
    }

    Array<Trace>& Throwable::initStacktrace() {
        Array<Trace>& stacktrace = UNSAFE::newInstance<Array<Trace>>();
        // try {
        //     NativeConsole::ReadStacktrace(stacktrace);
        // } catch (...) {
        //     //
        // }
        return stacktrace;
    }

    CORE_WARNING_POP
} // core
