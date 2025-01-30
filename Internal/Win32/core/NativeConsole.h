//
// Created by bruns on 19/10/2024.
//

#ifndef CORE24_NATIVECONSOLE_H
#define CORE24_NATIVECONSOLE_H

#include <core/Object.h>
#include <core/Trace.h>

namespace core {
    class NativeConsole final : public Object {
    public:
        static glong WriteError(ByteArray const& text);
        static glong Write(ByteArray const& text);
        static glong WriteLn(ByteArray const& text);
        
        static gint ReadByte();

        static gbool ReadStacktrace(Array<Trace>& trace);
    };
} // core

#endif //CORE24_NATIVECONSOLE_H
