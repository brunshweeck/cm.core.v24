//
// Created by brunshweeck on 28/11/24.
//

#ifndef CORE24_AUTOSYNC_H
#define CORE24_AUTOSYNC_H

#include "Version.h"

namespace core {
  namespace text {
    class AutoSync final : public Object {
      concurrent::Lock& lock;

    public:
      AutoSync(concurrent::Lock& lock);

      void relock() const;

      void unlock() const;

      ~AutoSync() override;
    };
  } // icu
} // core

#endif //CORE24_AUTOSYNC_H
