//
// Created by brunshweeck on 3 août 2024.
//

#ifndef CORE24_CHRONO_H
#define CORE24_CHRONO_H

#include <core/time/Temporal.h>

namespace core {
  namespace time {
    /**
     * The @b Chrono class represent the chronometer, it's usually used to
     * quickly calculate how much time has elapsed between two events.
     * However, unlike LocalTime, Chrono tries to use monotonic clocks if
     * possible. This means it's not possible to convert Chrono objects
     * to a human-readable time.
     * <p>
     * The typical use-case for the class is to determine how much time was
     * spent in a slow operation. The simplest example of such a case is for
     * debugging purposes.
     * </p>
     * <p>
     * In this class, the timer is started by a call to @c start method and the
     * elapsed time is calculated by the @c elapsedTime or @c duration methods.
     * The time elapsed can also be used to recalculate the time available for
     * another operation, after the first one is complete. This is useful when
     * the execution must complete within a certain time period, but several
     * steps are needed.
     * </p>
     * <p>
     * Another use-case is to execute a certain operation for a specific
     * time-slice. For this, Chrono provides the @c hasExpired convenience
     * method, which can be used to determine if a certain number of
     * milliseconds has already elapsed.
     * </p>
     * <h2> Reference Clocks </h2>
     * <p>
     * Chrono will use the platform's monotonic reference clock in all
     * platforms that support it. This has the added benefit that Chrono
     * is immune to time adjustments, such as the user correcting the time.
     * Also unlike @c LocalTime, Chrono is immune to changes in the timezone
     * settings, such as daylight-saving periods.
     * </p>
     * <p>
     * On the other hand, this means Chrono values can only be compared
     * with other values that use the same reference. This is especially true if
     * the time since the reference is extracted from the @c Chrono object
     * and serialised. These values should never be exchanged across the network or
     * saved to disk, since there's no telling whether the computer node receiving
     * the data is the same as the one originating i or if it has rebooted since.
     * </p>
     * <p>
     * It is, however, possible to exchange the value with other processes
     * running on the same machine, provided that they also use the same
     * reference clock. Chrono will always use the same clock, so it's
     * safe to compare with the value coming from another process in the same
     * machine. If comparing to values produced by other APIs, you should check
     * that the clock used is the same as Chrono (see Chrono::clock() method).
     * </p>
     * <h2> 32 bits overflows </h2>
     * <p>
     * Some of the clocks used by Chrono have a limited range and may
     * overflow after hitting the upper limit (usually 32 bits). Chrono
     * deals with this overflow issue and presents a consistent timing. However,
     * when extracting the time since reference from Chrono, two
     * different processes in the same machine may have different understanding
     * of how much time has actually elapsed.
     * </p>
     * @note It is not recommended to use this class as key map in the Map or as
     *       element in the Set.
     *
     * @see LocalTime
     * @see Duration
     * @see TemporalUnit
     */
    class Chrono final : public Object {
      /**
       * count of seconds since the start instant
       */
      glong seconds = -1L;

      /**
       * count of nanos of seconds since the start instant
       */
      gint nanos = 0L;

    public:
      /**
       * Create new instance of @c Chrono.
       *
       * @note the newly instance will be not started automatically.
       */
      CORE_FAST Chrono() = default;

      /**
       * Create new instance of @c Chrono.
       *
       * @param autoStart the boolean use to determine if this instance will be
       *          started automatically.
       */
      CORE_EXPLICIT Chrono(gbool autoStart);

      /**
       * Start or restart this chrono timer.
       *
       * @note If value of elapsed time exceeds limit the value
       *      returned is @c Long::MAX_VALUE
       *
       * @return the previous elapsed timeline in milliseconds or
       *          @c 0 if he has not started previously.
       */
      glong start();

      /**
       * Return true if chrono timer has started, false otherwise.
       * This calling are equivalent to <b>return this.elapsedTime() >= 0; </b>
       */
      gbool isStarted() const;

      /**
       * Return the current elapsed time (timeout) in milliseconds
       * or @c -1 if this chrono has not started.
       *
       * @note If the value of result exceed implementation limit, the
       *       returned value will be @c Long::MAX_VALUE.
       *       This calling is equivalent to @c this.elapsedTime(Chrono::MILLIS)
       */
      glong elapsedTime() const;

      /**
       * Return the current elapsed time (timeout) in milliseconds
       * or @c -1 if this chrono has not started.
       *
       * @note If the value of result exceed implementation limit, the
       *       returned value will be @c Long::MAX_VALUE.
       *       This calling is equivalent to @c this.elapsedTime(Chrono::MILLIS)
       */
      Duration duration() const;

      /**
       * Return the current elapsed time (timeout) in specified unit or
       * -1 if this chrono has not started.
       *
       * @note If the value of result exceed implementation limit, the
       *       returned value will be @c Long::MAX_VALUE.
       *
       * @param unit the result unit.
       * @return the current elapsed time (timeout) in specified unit.
       */
      glong elapsedTime(TemporalUnit unit) const;

      /**
       * Test if specified timeout has expired since the start instance.
       * In other words, return true if the current elapsed time great or
       * equals to specified timeout.
       *
       * @param time The except timeout in milliseconds
       */
      gbool hasExpired(glong time) const;

      /**
       * Test if specified timeout has expired since the start instance.
       * In other words, return true if the current elapsed time great or
       * equals to specified timeout.
       *
       * @param time The except timeout in milliseconds
       * @param unit The time unit
       */
      gbool hasExpired(glong time, TemporalUnit unit) const;

      /**
       * Test if specified duration has expired since the start instance.
       * In other words, return true if the current elapsed time great or
       * equals to specified timeout.
       *
       * @param duration The except duration
       */
      gbool hasExpired(Duration const& duration) const;

      /**
       * Return hash code of this object
       */
      gint hash() const override;

      /**
       * Return true if and only if specified object are Chrono instance
       * and have same instant value with this Chrono
       *
       * @param o object to be compared
       */
      gbool equals(const Object& o) const override;

      /**
       * Return String representation of this Chrono time.
       *
       * @note The value return by this function is not same
       * in any times.
       */
      String toString() const override;

      /**
       * Return shadow copy of this object
       */
      Object& clone() const override;
    };
  } // time
} // core

#endif //CORE24_CHRONO_H
