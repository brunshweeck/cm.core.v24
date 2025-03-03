//
// Created by admin on 13/12/24.
//

#include "TemporalQuery.h"

#include <core/time/Temporal.h>
#include <core/util/Optional.h>

namespace core {
  using namespace util;

  namespace time {
    Optional<> TemporalQuery::queryFrom(TemporalAccessor const& temporal) const {
      return Optional::empty();
    }

    TemporalQuery& TemporalQuery::zoneId() {
      class Query final : public TemporalQuery {
      public:
        Optional queryFrom(TemporalAccessor const& temporal) const override {
          try {
            return temporal.query(*this);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool equals(Object const& other) const override {
          return this == &other || Class<Query>::hasInstance(other);
        }

        String toString() const override {
          return "ZoneId"_Sl;
        }
      };

      static TemporalQuery& query = UNSAFE::newInstance<Query>();
      return query;
    }

    TemporalQuery& TemporalQuery::chronology() {
      class Query final : public TemporalQuery {
      public:
        Optional queryFrom(TemporalAccessor const& temporal) const override {
          try {
            return temporal.query(*this);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool equals(Object const& other) const override {
          return this == &other || Class<Query>::hasInstance(other);
        }

        String toString() const override {
          return "Chronology"_Sl;
        }
      };

      static TemporalQuery& query = UNSAFE::newInstance<Query>();
      return query;
    }

    TemporalQuery& TemporalQuery::precision() {
      class Query final : public TemporalQuery {
      public:
        Optional queryFrom(TemporalAccessor const& temporal) const override {
          try {
            return temporal.query(*this);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool equals(Object const& other) const override {
          return this == &other || Class<Query>::hasInstance(other);
        }

        String toString() const override {
          return "Precision"_Sl;
        }
      };

      static TemporalQuery& query = UNSAFE::newInstance<Query>();
      return query;
    }

    TemporalQuery& TemporalQuery::zone() {
      class Query final : public TemporalQuery {
      public:
        Optional queryFrom(TemporalAccessor const& temporal) const override {
          try {
            return temporal.query(*this);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool equals(Object const& other) const override {
          return this == &other || Class<Query>::hasInstance(other);
        }

        String toString() const override {
          return "Zone"_Sl;
        }
      };

      static TemporalQuery& query = UNSAFE::newInstance<Query>();
      return query;
    }

    TemporalQuery& TemporalQuery::offset() {
      class Query final : public TemporalQuery {
      public:
        Optional queryFrom(TemporalAccessor const& temporal) const override {
          try {
            return temporal.query(*this);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool equals(Object const& other) const override {
          return this == &other || Class<Query>::hasInstance(other);
        }

        String toString() const override {
          return "Offset"_Sl;
        }
      };

      static TemporalQuery& query = UNSAFE::newInstance<Query>();
      return query;
    }

    TemporalQuery& TemporalQuery::localDate() {
      class Query final : public TemporalQuery {
      public:
        Optional queryFrom(TemporalAccessor const& temporal) const override {
          try {
            return temporal.query(*this);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool equals(Object const& other) const override {
          return this == &other || Class<Query>::hasInstance(other);
        }

        String toString() const override {
          return "LocalDate"_Sl;
        }
      };

      static TemporalQuery& query = UNSAFE::newInstance<Query>();
      return query;
    }

    TemporalQuery& TemporalQuery::localTime() {
      class Query final : public TemporalQuery {
      public:
        Optional queryFrom(TemporalAccessor const& temporal) const override {
          try {
            return temporal.query(*this);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }

        gbool equals(Object const& other) const override {
          return this == &other || Class<Query>::hasInstance(other);
        }

        String toString() const override {
          return "LocalTime"_Sl;
        }
      };

      static TemporalQuery& query = UNSAFE::newInstance<Query>();
      return query;
    }
  } // time
} // core
