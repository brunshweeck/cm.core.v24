//
// Created by admin on 12/01/25.
//

#ifndef CORE_PRINTER_H
#define CORE_PRINTER_H

#include <native/Console.h>

#include <core/lang/Throwable.h>
#include <core/misc/Unsafe.h>
#include <core/util/Locale.h>

using namespace core;
using namespace core::text;
using namespace core::time;
using namespace core::util;
using namespace core::charset;
using namespace core::io;
using namespace core::net;

class Printer final : public Object {
    gbool error;

public:
    CORE_EXPLICIT Printer(gbool error): error(error) {}

    void print(const String& str) const {
        try {
            ByteArray bytes = str.toBytes();
            if (!error)
                NativeConsole::Write(bytes);
            else
                NativeConsole::WriteError(bytes);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void println(const String& str) const {
        try {
            print(str);
            print("\n");
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void printf(const String& str) const { CORE_TRY_RETHROW(print(str.formatted())) }

    void printf(Locale const&, const String& str) const { CORE_TRY_RETHROW(printf(str)) }

    template <class... Args>
    void printf(const String& fmt, Args&&... args) const {
        Locale defaultLocale = Locale::getDefault(Locale::FORMAT);
        CORE_TRY_RETHROW(printf(defaultLocale, fmt, (Args&&) args...))
    }

    template <class... Args>
    void printf(const Locale& locale, const String& fmt, Args&&... args) const {
        String out;
        CORE_TRY_RETHROW(out = String::format(locale, fmt, UNSAFE::forwardInstance<Args>(args)...))
        print(out);
    }
};

static const Printer err = Printer(true);
static const Printer out = Printer(false);

#endif //CORE_PRINTER_H
