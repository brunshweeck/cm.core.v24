//
// Created by admin on 16/01/25.
//

#ifndef CORE24_CORE24_TRASH_H
#define CORE24_CORE24_TRASH_H

#include "core/String.h"
#include "core/io/File.h"

namespace core {
    namespace io {

        class Trash final: public Object {
            /*
             * On Unix System, the trash directory is subdivided in two subdirectories:
             * - ${trash}/files: contains the files and directories that were recycled (trashed).
             *   The name of files in this directory are to be determined by the implementation;
             *   the only limitation is that they must be unique within the directory. Even if a
             *   file with the same name and location gets trashed many times, each subsequent
             *   trashing must not overwrite a previous copy.
             *
             * - ${trash}/info: contains an `information` for every file and directory in ${trash}/files
             *    directory. This file must have exactly the same name as the file or directory in
             *    ${trash}/files, plus the extension `.trashinfo`
             */

            /**
             * The path of trash
             */
            File trash;

            /**
             * The path of file before recycling
             */
            File original;

            /**
             * The path of file info after recycling
             */
            File destInfo;

            /**
             * The path of file destination after recycling
             */
            File dest;

        public:

            /**
             * Initialize trash for specified file.
             */
            CORE_EXPLICIT Trash(const File &original);

            gbool commit();

        private:
            static File findTrashPath(File const& original);

            /**
             * Generate the name of trash info file
             */
            gbool generateInfoFile();
        };

    } // io
} // core

#endif //CORE24_CORE24_TRASH_H
