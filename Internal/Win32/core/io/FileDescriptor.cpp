//
// Created by bruns on 12/10/2024.
//

#include <core/lang/Windows.h>
#include <core/lang/io.h>
#include <core/io/FileDescriptor.h>

namespace core {
    namespace io {

        void FileDescriptor::synchronize() const {
            FlushFileBuffers(CORE_CAST(HANDLE, handle));
        }

        glong FileDescriptor::getHandle(gint fd) {
            HANDLE handle = CORE_CAST(HANDLE, _get_osfhandle(fd));
            if(handle != INVALID_HANDLE_VALUE)
                return CORE_CAST(glong, handle);
            return -1;
        }

        glong FileDescriptor::getAppendStatus(gint fd) {
            HANDLE handle = CORE_CAST(HANDLE, _get_osfhandle(fd));
            if(handle == INVALID_HANDLE_VALUE)
                return false;
            FILE_STANDARD_INFO info;
            if(GetFileInformationByHandleEx(handle,
                FileStandardInfo,
                &info, Class<FILE_STANDARD_INFO>::size())) {
                if(info.AllocationSize.QuadPart == info.EndOfFile.QuadPart) {
                    CloseHandle(handle);
                    return true;
                }
            }
            return false;
        }

    }
}
