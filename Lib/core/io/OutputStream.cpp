//
// Created by bruns on 13/10/2024.
//

#include <core/io/OutputStream.h>
#include <core/misc/Preconditions.h>
#include <core/misc/Unsafe.h>

namespace core {
    using namespace misc;
    using namespace util;

    namespace io {
        void OutputStream::write(ByteArray const &b) {
            try {
                write(b, 0, b.length());
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void OutputStream::write(ByteArray const &b, gint off, gint len) {
            try {
                Preconditions::checkIndexFromSize(off, len, b.length());
                // len == 0 condition implicitly handled by loop bounds
                for (gint i = 0; i < len; i++)
                    write(b[i + off]);
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }

        void OutputStream::flush() {
        }

        void OutputStream::close() {
        }

        OutputStream &OutputStream::nullStream() {
            class NullOutputStream : public OutputStream {
                gbool volatile closed = false;

                void ensureOpen() const {
                    if (closed)
                        IOException("Stream already closed").throws($xtrace());
                }

            public:
                void write(gint b) override {
                    try {
                        ensureOpen();
                    } catch (Throwable const &ex) { ex.throws($xtrace()); }
                }

                void write(const ByteArray &b, gint off, gint len) override {
                    try {
                        Preconditions::checkIndexFromSize(off, len, b.length());
                        ensureOpen();
                    } catch (Throwable const &ex) { ex.throws($xtrace()); }
                }

                void close() override {
                    closed = true;
                }

                ~NullOutputStream() override { NullOutputStream::close(); }
            };

            try {
                return UNSAFE::newInstance<NullOutputStream>();
            } catch (Throwable const &ex) { ex.throws($ftrace()); }
        }
    } // io
} // core
