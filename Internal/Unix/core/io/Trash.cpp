//
// Created by admin on 16/01/25.
//

#include "Trash.h"
#include <fcntl.h>
#include <mntent.h>
#include <paths.h>
#include <stdio.h>
#include <core/random/Random.h>
#include <core/time/LocalDateTime.h>
#include <core/util/Locale.h>
#include <native/System.h>

#ifndef _PATH_MOUNTED
#  define _PATH_MOUNTED     "/etc/mnttab"
#endif

namespace core {
  using namespace random;
  using namespace util;
  using namespace time;

  namespace io {
    Trash::Trash(const File& original)
      : trash(""), original(original), destInfo(""), dest("") {}

    gbool Trash::generateInfoFile() {
      static Random rnd = { };
      // Trash_abcdef0123456789
      while (true) {
        ByteArray bytes = ByteArray(16);
        rnd.nextBytes(bytes);
        for (gbyte& byte : bytes) {
          if (byte < 0)
            byte = (-1 * byte) % 128;
          byte = byte % 36;
          if (byte < 26)
            byte += 97;
          else
            byte = (byte % 10) + 48;
        }
        destInfo = File(trash, "info/Trash_"_Sl + String(bytes) + ".trashinfo");
        dest = File(trash, "files/Trash_"_Sl + String(bytes));
        if (destInfo.exists() || dest.exists())
          continue;
        return true;
      }
    }

    File Trash::findTrashPath(const File& original) {
      // By default, the trash location on unix system is `/home/${user}/.local/share/Trash` for user,
      // `/Trash` for root user and ${device}/.Trash external device
      File trash = File("");
      File source = original.absoluteFile();
      {
        // 1) Find the current device
        FILE* fs = setmntent(_PATH_MOUNTED, "r");
        if (fs != null) {
          mntent next = { };
          CHAR buffer[1024] = { };
          while (true) {
            mntent* r = getmntent_r(fs, &next, buffer, 1024);
            if (r == null) {
              break;
            }
            String prefix = next.mnt_dir;
            if (prefix.isEmpty() || prefix == "/")
              continue;
            if (source.path().startsWith(prefix)) {
              trash = File(prefix, ".Trash");
              break;
            }
          }
          endmntent(fs);
        }
      }

      if (trash.path().isEmpty()) {
        // 2) Try with user trash (current user)
        String xdgHome = GetEnv("XDG_DATA_HOME");
        if (!xdgHome.isEmpty() && xdgHome.charAt(0) != '/')
          xdgHome = String();
        if (xdgHome.isEmpty()) {
          xdgHome = GetEnv("HOME");
          if (xdgHome.isEmpty())
            xdgHome = "/"_Sl;
          xdgHome = File(xdgHome, ".local/share"_Sl).absolutePath();
        }
        trash = File(xdgHome, "Trash");
      }
      if (!trash.exists())
        trash.createDirectories();
      File files = File(trash, "files");
      File info = File(trash, "info");
      if (!files.exists())
        files.createDirectories();
      if (!info.exists())
        info.createDirectories();
      return trash;
    }

    gbool Trash::commit() {
      trash = findTrashPath(original);
      File file = original.absoluteFile();
      generateInfoFile();
      String content = String::format(Locale::US, "[Trash Info]%nPath=%s%nDeletionDate=%s%n",
                                      file.path(),
                                      LocalDateTime::now().withNano(0));

      ByteArray bytes = destInfo.absolutePath().toBytes();
      LPCSTR path = (LPCSTR) &bytes[0];
      if (!destInfo.exists())
        destInfo.createFile();
      FILE* f = fopen(path, "w+");
      if (f == null)
        return false;
      bytes = content.toBytes();
      fwrite(&bytes[0], 1, bytes.length(), f);
      fclose(f);
      gbool r = file.moveTo(dest, File::DEFAULT_OPTION);
      if (!r && destInfo.exists())
        destInfo.deleteFile();
      return r;
    }
  } // io
} // core
