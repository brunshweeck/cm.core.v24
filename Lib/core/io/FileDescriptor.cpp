//
// Created by bruns on 12/10/2024.
//

#include <core/io/FileDescriptor.h>
#include <core/concurrent/ReentrantLock.h>
#include <core/misc/Unsafe.h>

namespace core {
    namespace io {
        FileDescriptor::FileDescriptor(gint fd): FileDescriptor() {
            FileDescriptor::fd = fd;
            handle = getHandle(fd);
            append = getAppendStatus(fd);
            monitor = new concurrent::ReentrantLock();
        }

        gbool FileDescriptor::isValid() const {
            return fd == -1 || handle == -1;
        }

        void FileDescriptor::sync() const {
            if (!isValid())
                return;
            CORE_DCAST(concurrent::ReentrantLock*, monitor)->lock();
            synchronize();
            CORE_DCAST(concurrent::ReentrantLock*, monitor)->unlock();
        }

        FileDescriptor::FileDescriptor() {
            fd = -1;
            handle = -1;
            firstParent = null;
            otherParents = null;
            closed = false;
            append = false;
        }

        FileDescriptor::FileDescriptor(FileDescriptor &&desc) CORE_NOTHROW: FileDescriptor() {
            if(desc.isValid()) {
                CORE_DCAST(concurrent::ReentrantLock*, desc.monitor)->lock();
            }
            UNSAFE::swapValues(fd, desc.fd);
            UNSAFE::swapValues(handle, desc.handle);
            UNSAFE::swapValues(append, desc.append);
            UNSAFE::swapValues(firstParent, desc.firstParent);
            UNSAFE::swapValues(otherParents, desc.otherParents);
            UNSAFE::swapValues(closed, desc.closed);
            if(desc.isValid()) {
                CORE_DCAST(concurrent::ReentrantLock*, desc.monitor)->unlock();
            }
        }

        FileDescriptor const FileDescriptor::in = FileDescriptor(0);
        FileDescriptor const FileDescriptor::out = FileDescriptor(1);
        FileDescriptor const FileDescriptor::err = FileDescriptor(2);
    } // io
} // core
