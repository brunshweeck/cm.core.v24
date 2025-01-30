//
// Created by admin on 12/01/25.
//

#ifndef CORE_PRINTER_H
#define CORE_PRINTER_H

#include <core/NativeConsole.h>

#include "core/Throwable.h"
#include "core/util/Locale.h"

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

    void printLn(const String& str) const {
        try {
            print(str);
            print("\n");
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void print(const Object& obj) const {
        try {
            print(obj.toString());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void printLn(const Object& obj) const {
        try {
            printLn(obj.toString());
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    void printf(const String& str) const {
        try {
            print(str);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    template <class... Args>
    void printf(const String& fmt, Args&&... args) const {
        try {
            printf(Locale::getDefault(Locale::FORMAT), fmt, (Args&&) args...);
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    template <class... Args>
    void printf(const Locale& locale, const String& fmt, Args&&... args) const {
        try {
            if (fmt.indexOf('%') == -1)
                print(fmt);
            else
                print(String::format(locale, fmt, (Args&&) args...));
        } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
};

static const Printer err = Printer(true);
static const Printer out = Printer(false);

#endif //CORE_PRINTER_H
