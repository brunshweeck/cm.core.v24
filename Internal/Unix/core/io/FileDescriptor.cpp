//
// Created by bruns on 12/10/2024.
//

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <core/io/FileDescriptor.h>

namespace core {
    namespace io {
        void FileDescriptor::synchronize() const {
            fflush(CORE_CAST(FILE *, handle));
        }

        glong FileDescriptor::getHandle(gint fd) {
            if (fd == 0)
                return CORE_CAST(glong, stdin);
            if (fd == 1)
                return CORE_CAST(glong, stdout);
            if (fd == 2)
                return CORE_CAST(glong, stderr);
            FILE *f = fdopen(fd, "r");
            return CORE_CAST(glong, f);
        }

        glong FileDescriptor::getAppendStatus(gint fd) {
            return false;
        }
    }
}
