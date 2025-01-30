//
// Created by bruns on 12/10/2024.
//

#include <core/ArithmeticException.h>
#include <core/IllegalArgumentException.h>
#include <core/OutOfMemoryError.h>
#include <core/io/EOFException.h>
#include <core/io/InputStream.h>
#include <core/io/OutputStream.h>
#include <core/util/ArrayList.h>

namespace core {
    CORE_ALIAS(UNSAFE, misc::Unsafe);
    using namespace misc;
    using namespace util;

    namespace io {
        gint InputStream::read(ByteArray& b) {
            try {
                return read(b, 0, b.length());
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gint InputStream::read(ByteArray& b, gint off, gint len) {
            try {
                Preconditions::checkIndexFromSize(off, len, b.length());
                if (len == 0)
                    return 0;
                gint c = read();
                if (c == -1)
                    return -1;
                b[off] = CORE_CAST(gbyte, c);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            gint i = 1, c = 0;
            try {
                for (; i < len; i++) {
                    c = read();
                    if (c == -1)
                        break;
                    b[off + i] = CORE_CAST(gbyte, c);
                }
            } catch (IOException const& _) {} catch (Throwable const& ex) { ex.throws($ftrace()); }
            return i;
        }

        ByteArray InputStream::readAllBytes() {
            try {
                return readNBytes(Integer::MAX_VALUE);
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        ByteArray InputStream::readNBytes(gint len) {
            if (len < 0)
                IllegalArgumentException("Negative length").throws($ftrace());
            ArrayList<ByteArray> bufs;
            ByteArray result;
            gint total = 0;
            gint remaining = len;
            gint n;

            do {
                ByteArray& buf = UNSAFE::newInstance<ByteArray>(Math::min(remaining, DEFAULT_BUFFER_SIZE));
                gint nread = 0;

                // read to EOF which may read more or less than buffer size
                try {
                    while ((n = read(buf, nread, Math::min(buf.length() - nread, remaining))) > 0) {
                        nread += n;
                        remaining -= n;
                    }
                } catch (Throwable const& ex) {
                    // Cleaning all allocate instance
                    UNSAFE::deleteInstance(buf);
                    for (ByteArray& b : bufs) UNSAFE::deleteInstance(b);
                    // Error
                    ex.throws($ftrace());
                }

                if (nread > 0) {
                    if (MAX_BUFFER_SIZE - total < nread) {
                        // Cleaning all allocate instance
                        UNSAFE::deleteInstance(buf);
                        for (ByteArray& b : bufs) UNSAFE::deleteInstance(b);
                        // Error
                        OutOfMemoryError("Required array size too large").throws($ftrace());
                    }

                    if (nread < buf.length()) {
                        buf = Arrays::copyOf(buf, nread);
                    }
                    total += nread;
                    bufs.add(buf);
                }
                // if the last call to read returned -1 or the number of bytes
                // requested have been read then break
            } while (n >= 0 && remaining > 0);

            if (bufs.isEmpty())
                return ByteArray();

            try {
                result = ByteArray(total);
                gint offset = 0;
                remaining = total;
                for (ByteArray& b : bufs) {
                    gint count = Math::min(b.length(), remaining);
                    Arrays::copy(b, 0, result, offset, count);
                    offset += count;
                    remaining -= count;

                    // Destroy used instance
                    UNSAFE::deleteInstance(b);
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return result;
        }

        gint InputStream::readNBytes(ByteArray& b, gint off, gint len) {
            try {
                Preconditions::checkIndexFromSize(off, len, b.length());

                gint n = 0;
                while (n < len) {
                    gint count = read(b, off + n, len - n);
                    if (count < 0)
                        break;
                    n += count;
                }

                return n;
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        glong InputStream::skip(glong n) {
            glong remaining = n;
            gint nr;

            if (n <= 0)
                return 0;

            try {
                gint size = CORE_CAST(gint, Math::min(MAX_SKIP_BUFFER_SIZE, remaining));
                ByteArray skipBuffer = ByteArray(size);
                while (remaining > 0) {
                    nr = read(skipBuffer, 0, CORE_CAST(gint, Math::min(remaining, CORE_CAST(glong, size))));
                    if (nr < 0)
                        break;
                    remaining -= nr;
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }

            return n - remaining;
        }

        void InputStream::skipNBytes(glong n) {
            glong ns = -1;
            while (n > 0) {
                try {
                    ns = skip(n);
                } catch (Throwable const& ex) { ex.throws($ftrace()); }
                if (ns > 0 && ns < n) {
                    // adjust number to skip
                    n -= ns;
                } else if (ns == 0) {
                    // no bytes skipped
                    // read one byte to check for EOS
                    gint c = -1;
                    try {
                        c = read();
                    } catch (Throwable const& ex) { ex.throws($ftrace()); }
                    if (c == -1)
                        EOFException().throws($ftrace());
                    // one byte read so decrement number to skip
                    n--;
                } else {
                    // skipped negative or too many bytes
                    IOException("Unable to skip exactly").throws($ftrace());
                }
            }
        }

        gint InputStream::available() const {
            return 0;
        }

        void InputStream::close() {}

        void InputStream::mark(gint readlimit) {}

        void InputStream::reset() {
            IOException("mark/reset not supported"_S).throws($ftrace());
        }

        gbool InputStream::supportMark() const {
            return false;
        }

        glong InputStream::transferTo(OutputStream& out) {
            glong transferred = 0;
            try {
                ByteArray buf = ByteArray(DEFAULT_BUFFER_SIZE);
                gint n;
                while ((n = read(buf, 0, DEFAULT_BUFFER_SIZE)) >= 0) {
                    out.write(buf, 0, n);
                    if (transferred < Long::MAX_VALUE) {
                        try {
                            transferred = Math::addExact(transferred, CORE_CAST(glong, n));
                        } catch (ArithmeticException const& _) { transferred = Long::MAX_VALUE; }
                    }
                }
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
            return transferred;
        }

        InputStream& InputStream::nullStream() {
            class NullInputStream final : public InputStream {
                gbool volatile closed = false;

                void ensureOpen() const {
                    if (closed)
                        IOException("Stream already closed"_S).throws($xtrace());
                }

            public:
                gint available() const override {
                    try {
                        ensureOpen();
                        return 0;
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                }

                gint read() override {
                    try {
                        ensureOpen();
                        return -1;
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                }

                gint read(ByteArray& b, gint off, gint len) override {
                    try {
                        Preconditions::checkIndexFromSize(off, len, b.length());
                        if (len == 0)
                            return 0;
                        ensureOpen();
                        return -1;
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                }

                ByteArray readAllBytes() override {
                    try {
                        ensureOpen();
                        return ByteArray();
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                }

                gint readNBytes(ByteArray& b, gint off, gint len) override {
                    try {
                        Preconditions::checkIndexFromSize(off, len, b.length());
                        ensureOpen();
                        return 0;
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                }

                ByteArray readNBytes(gint len) override {
                    if (len < 0)
                        IllegalArgumentException("Negative length").throws($xtrace());
                    try {
                        ensureOpen();
                        return ByteArray();
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                }

                glong skip(glong n) override {
                    try {
                        ensureOpen();
                        return 0;
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                }

                void skipNBytes(glong n) override {
                    try {
                        ensureOpen();
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                    if (n > 0)
                        EOFException().throws($xtrace());
                }

                glong transferTo(OutputStream& out) override {
                    try {
                        ensureOpen();
                        return 0;
                    } catch (Throwable const& ex) { ex.throws($xtrace()); }
                }

                void close() override {
                    closed = true;
                }
            };

            try {
                return UNSAFE::newInstance<NullInputStream>();
            } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
    } // io
} // core
