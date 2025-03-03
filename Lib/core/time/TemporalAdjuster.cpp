//
// Created by brunshweeck on 21/02/25.
//

#include "TemporalAdjuster.h"

#include <core/lang/RuntimeException.h>
#include <core/function/UnaryOperator.h>
#include <core/time/LocalDate.h>
#include <core/time/ValueRange.h>

namespace core {
  namespace time {
    Temporal& TemporalAdjuster::adjustInto(Temporal const& temporal) const {
      RuntimeException("Undefined operation"_Sl).throws($ftrace());
    }

    TemporalAdjuster& TemporalAdjuster::ofDateAdjuster(LocalDateOperator const& dateBasedAdjuster) {
      class Adjuster final : public TemporalAdjuster {
        LocalDateOperator const& dateBasedAdjuster;

      public:
        CORE_EXPLICIT Adjuster(LocalDateOperator const& dateBasedAdjuster)
          : dateBasedAdjuster(dateBasedAdjuster) {}

        Temporal& adjustInto(Temporal const& temporal) const override {
          try {
            LocalDate input = LocalDate::from(temporal);
            LocalDate output = dateBasedAdjuster.apply(input);
            return adjustFieldTo(output, temporal);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      return UNSAFE::newInstance<Adjuster>(dateBasedAdjuster);
    }

    TemporalAdjuster& TemporalAdjuster::firstDayOfMonth() {
      class Adjuster final : public TemporalAdjuster {
      public:
        Temporal& adjustInto(Temporal const& temporal) const override {
          try {
            return adjustFieldTo(TemporalField::DAY_OF_MONTH, 1, temporal);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      static Adjuster& adjuster = UNSAFE::newInstance<Adjuster>();
      return adjuster;
    }

    TemporalAdjuster& TemporalAdjuster::lastDayOfMonth() {
      class Adjuster final : public TemporalAdjuster {
      public:
        Temporal& adjustInto(Temporal const& temporal) const override {
          try {
            return adjustFieldTo(TemporalField::DAY_OF_MONTH,
                                 temporal.range(TemporalField::DAY_OF_MONTH).maximum(),
                                 temporal);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      static Adjuster& adjuster = UNSAFE::newInstance<Adjuster>();
      return adjuster;
    }

    TemporalAdjuster& TemporalAdjuster::firstDayOfNextMonth() {
      class Adjuster final : public TemporalAdjuster {
      public:
        Temporal& adjustInto(Temporal const& temporal) const override {
          try {
            return adjustFieldTo(TemporalField::DAY_OF_MONTH, 1, temporal)
                .add(1, TemporalUnit::MONTHS);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      static Adjuster& adjuster = UNSAFE::newInstance<Adjuster>();
      return adjuster;
    }

    TemporalAdjuster& TemporalAdjuster::firstDayOfYear() {
      class Adjuster final : public TemporalAdjuster {
      public:
        Temporal& adjustInto(Temporal const& temporal) const override {
          try {
            return adjustFieldTo(TemporalField::DAY_OF_YEAR, 1, temporal);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      static Adjuster& adjuster = UNSAFE::newInstance<Adjuster>();
      return adjuster;
    }

    TemporalAdjuster& TemporalAdjuster::lastDayOfYear() {
      class Adjuster final : public TemporalAdjuster {
      public:
        Temporal& adjustInto(Temporal const& temporal) const override {
          try {
            return adjustFieldTo(TemporalField::DAY_OF_YEAR,
                                 temporal.range(TemporalField::DAY_OF_YEAR).maximum(),
                                 temporal);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      static Adjuster& adjuster = UNSAFE::newInstance<Adjuster>();
      return adjuster;
    }

    TemporalAdjuster& TemporalAdjuster::firstDayOfNextYear() {
      class Adjuster final : public TemporalAdjuster {
      public:
        Temporal& adjustInto(Temporal const& temporal) const override {
          try {
            return adjustFieldTo(TemporalField::DAY_OF_MONTH, 1, temporal)
                .add(1, TemporalUnit::YEARS);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      static Adjuster& adjuster = UNSAFE::newInstance<Adjuster>();
      return adjuster;
    }

    TemporalAdjuster& TemporalAdjuster::firstInMonth(DayOfWeek dayOfWeek) {
      return dayOfWeekInMonth(1, dayOfWeek);
    }

    TemporalAdjuster& TemporalAdjuster::lastInMonth(DayOfWeek dayOfWeek) {
      return dayOfWeekInMonth(-1, dayOfWeek);
    }

    TemporalAdjuster& TemporalAdjuster::dayOfWeekInMonth(gint ordinal, DayOfWeek dayOfWeek) {
      class Adjuster final : public TemporalAdjuster {
        gint ordinal;
        DayOfWeek dayOfWeek;

      public:
        Adjuster(gint ordinal, DayOfWeek dayOfWeek): ordinal(ordinal), dayOfWeek(dayOfWeek) {}

        Temporal& adjustInto(Temporal const& temporal) const override {
          gint dowValue = (gint) dayOfWeek;
          try {
            if (ordinal < 0) {
              Temporal& temp = adjustFieldTo(TemporalField::DAY_OF_MONTH,
                                             temporal.range(TemporalField::DAY_OF_MONTH).maximum(),
                                             temporal);
              int curDow = temp.get(TemporalField::DAY_OF_WEEK);
              int daysDiff = dowValue - curDow;
              daysDiff = (daysDiff == 0 ? 0 : (daysDiff > 0 ? daysDiff - 7 : daysDiff));
              daysDiff -= (int) ((-ordinal - 1L) * 7L); // safe from overflow
              return temp.add(daysDiff, TemporalUnit::DAYS);
            }
            Temporal& temp = adjustFieldTo(TemporalField::DAY_OF_MONTH, 1, temporal);
            gint curDow = temp.get(TemporalField::DAY_OF_WEEK);
            gint dowDiff = (dowValue - curDow + 7) % 7;
            dowDiff += (int) ((ordinal - 1L) * 7L); // safe from overflow
            return temp.add(dowDiff, TemporalUnit::DAYS);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      return UNSAFE::newInstance<Adjuster>(ordinal, dayOfWeek);
    }

    TemporalAdjuster& TemporalAdjuster::next(DayOfWeek dayOfWeek) {
      class Adjuster final : public TemporalAdjuster {
        DayOfWeek dayOfWeek;

      public:
        CORE_EXPLICIT Adjuster(DayOfWeek dayOfWeek): dayOfWeek(dayOfWeek) {}

        Temporal& adjustInto(Temporal const& temporal) const override {
          try {
            gint dowValue = (gint) dayOfWeek;
            gint calDow = temporal.get(TemporalField::DAY_OF_WEEK);
            gint daysDiff = calDow - dowValue;
            return temporal.add(daysDiff >= 0 ? 7 - daysDiff : -daysDiff, TemporalUnit::DAYS);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      return UNSAFE::newInstance<Adjuster>(dayOfWeek);
    }

    TemporalAdjuster& TemporalAdjuster::nextOrSame(DayOfWeek dayOfWeek) {
      class Adjuster final : public TemporalAdjuster {
        DayOfWeek dayOfWeek;

      public:
        CORE_EXPLICIT Adjuster(DayOfWeek dayOfWeek): dayOfWeek(dayOfWeek) {}

        Temporal& adjustInto(Temporal const& temporal) const override {
          gint dowValue = (gint) dayOfWeek;
          try {
            gint calDow = temporal.get(TemporalField::DAY_OF_WEEK);
            if (calDow == dowValue) {
              return UNSAFE::copyInstance(temporal);
            }
            gint daysDiff = calDow - dowValue;
            return temporal.add(daysDiff >= 0 ? 7 - daysDiff : -daysDiff, TemporalUnit::DAYS);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      return UNSAFE::newInstance<Adjuster>(dayOfWeek);
    }

    TemporalAdjuster& TemporalAdjuster::previous(DayOfWeek dayOfWeek) {
      class Adjuster final : public TemporalAdjuster {
        DayOfWeek dayOfWeek;

      public:
        CORE_EXPLICIT Adjuster(DayOfWeek dayOfWeek): dayOfWeek(dayOfWeek) {}

        Temporal& adjustInto(Temporal const& temporal) const override {
          gint dowValue = (gint) dayOfWeek;
          try {
            gint calDow = temporal.get(TemporalField::DAY_OF_WEEK);
            gint daysDiff = dowValue - calDow;
            return temporal.subtract(daysDiff >= 0 ? 7 - daysDiff : -daysDiff, TemporalUnit::DAYS);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      return UNSAFE::newInstance<Adjuster>(dayOfWeek);
    }

    TemporalAdjuster& TemporalAdjuster::previousOrSame(DayOfWeek dayOfWeek) {
      class Adjuster final : public TemporalAdjuster {
        DayOfWeek dayOfWeek;

      public:
        CORE_EXPLICIT Adjuster(DayOfWeek dayOfWeek): dayOfWeek(dayOfWeek) {}

        Temporal& adjustInto(Temporal const& temporal) const override {
          gint dowValue = (gint) dayOfWeek;
          try {
            int calDow = temporal.get(TemporalField::DAY_OF_WEEK);
            if (calDow == dowValue) {
              return UNSAFE::copyInstance(temporal);
            }
            int daysDiff = dowValue - calDow;
            return temporal.subtract(daysDiff >= 0 ? 7 - daysDiff : -daysDiff, TemporalUnit::DAYS);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      return UNSAFE::newInstance<Adjuster>(dayOfWeek);
    }

    TemporalAdjuster& TemporalAdjuster::ofMonth(Month month) {
      class Adjuster final : public TemporalAdjuster {
        Month month;

      public:
        CORE_EXPLICIT Adjuster(Month month) : month(month) {}

        Temporal& adjustInto(Temporal const& temporal) const override {
          gint monthValue = (gint) month;
          try {
            return adjustFieldTo(TemporalField::MONTH_OF_YEAR, monthValue, temporal);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      return UNSAFE::newInstance<Adjuster>(month);
    }

    TemporalAdjuster& TemporalAdjuster::ofDayOfWeek(DayOfWeek dayOfWeek) {
      class Adjuster final : public TemporalAdjuster {
        DayOfWeek dayOfWeek;

      public:
        CORE_EXPLICIT Adjuster(DayOfWeek dayOfWeek): dayOfWeek(dayOfWeek) {}

        Temporal& adjustInto(Temporal const& temporal) const override {
          gint dowValue = (gint) dayOfWeek;
          try {
            return adjustFieldTo(TemporalField::DAY_OF_WEEK, dowValue, temporal);
          } catch (Throwable const& ex) { ex.throws($ftrace()); }
        }
      };

      return UNSAFE::newInstance<Adjuster>(dayOfWeek);
    }

    Temporal& TemporalAdjuster::adjustFieldTo(TemporalField field, glong newValue, Temporal const& temporal) {
      try {
        return temporal.adjust(field, newValue);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }

    Temporal& TemporalAdjuster::adjustFieldTo(TemporalAdjuster const& adjuster, Temporal const& temporal) {
      try {
        return temporal.adjust(adjuster);
      } catch (Throwable const& ex) { ex.throws($ftrace()); }
    }
  } // time
} // core
