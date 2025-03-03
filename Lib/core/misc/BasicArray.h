//
// Created by brunshweeck on 4 sept. 2024.
//

#ifndef CORE24_BASICARRAY_H
#define CORE24_BASICARRAY_H

#include <initializer_list>
#include <core/lang/Class.h>

namespace core {
    namespace misc {
        class BasicArray: public Object {

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_AS_FRIEND(util::Arrays);
            CORE_ADD_TEMPLATES_AS_FRIEND(PrimitiveArray, T);
            CORE_ADD_TEMPLATES_AS_FRIEND(ValueArray, T);
            CORE_ADD_AS_FRIEND(ObjectArray);
            CORE_ADD_AS_FRIEND(BooleanArray);
            CORE_ADD_AS_FRIEND(ByteArray);
            CORE_ADD_AS_FRIEND(ShortArray);
            CORE_ADD_AS_FRIEND(CharArray);
            CORE_ADD_AS_FRIEND(IntArray);
            CORE_ADD_AS_FRIEND(FloatArray);
            CORE_ADD_AS_FRIEND(LongArray);
            CORE_ADD_AS_FRIEND(DoubleArray);
            CORE_ADD_AS_FRIEND(BooleanArray2D);
            CORE_ADD_AS_FRIEND(ByteArray2D);
            CORE_ADD_AS_FRIEND(ShortArray2D);
            CORE_ADD_AS_FRIEND(CharArray2D);
            CORE_ADD_AS_FRIEND(IntArray2D);
            CORE_ADD_AS_FRIEND(FloatArray2D);
            CORE_ADD_AS_FRIEND(LongArray2D);
            CORE_ADD_AS_FRIEND(DoubleArray2D);
            CORE_ADD_AS_FRIEND(BooleanArray3D);
            CORE_ADD_AS_FRIEND(ByteArray3D);
            CORE_ADD_AS_FRIEND(ShortArray3D);
            CORE_ADD_AS_FRIEND(CharArray3D);
            CORE_ADD_AS_FRIEND(IntArray3D);
            CORE_ADD_AS_FRIEND(FloatArray3D);
            CORE_ADD_AS_FRIEND(LongArray3D);
            CORE_ADD_AS_FRIEND(DoubleArray3D);
            CORE_ADD_AS_FRIEND(StringArray);
            CORE_ADD_AS_FRIEND(io::FileArray);

            CORE_ADD_AS_FRIEND(time::TemporalFieldArray);
            CORE_ADD_AS_FRIEND(time::TemporalUnitArray);
            CORE_ADD_AS_FRIEND(time::FormatStyleArray);
            CORE_ADD_AS_FRIEND(time::SignStyleArray);
            CORE_ADD_AS_FRIEND(time::TextStyleArray);
            CORE_ADD_AS_FRIEND(time::DayOfWeekArray);
            CORE_ADD_AS_FRIEND(time::MonthArray);
            CORE_ADD_AS_FRIEND(time::YearArray);
            CORE_ADD_AS_FRIEND(time::DurationArray);
            CORE_ADD_AS_FRIEND(time::InstantArray);
            CORE_ADD_AS_FRIEND(time::LocalDateArray);
            CORE_ADD_AS_FRIEND(time::LocalDateTimeArray);
            CORE_ADD_AS_FRIEND(time::LocalTimeArray);
            CORE_ADD_AS_FRIEND(time::MonthDayArray);
            CORE_ADD_AS_FRIEND(time::OffsetDateTimeArray);
            CORE_ADD_AS_FRIEND(time::OffsetTimeArray);
            CORE_ADD_AS_FRIEND(time::PeriodArray);
            CORE_ADD_AS_FRIEND(time::YearMonthArray);
            CORE_ADD_AS_FRIEND(time::ZonedDateTimeArray);
            CORE_ADD_AS_FRIEND(time::ZoneOffsetArray);
            CORE_ADD_AS_FRIEND(time::ZoneOffsetTransitionArray);
            CORE_ADD_AS_FRIEND(time::ZoneOffsetTransitionRuleArray);
            CORE_ADD_AS_FRIEND(time::ZoneRulesArray);

            CORE_ADD_TEMPLATES_AS_FRIEND(Array, T);

            /**
             * The field used to store number of values on this array
             */
            gint count = 0;

            /**
             * Construct new array instance.
             */
            CORE_IMPLICIT BasicArray();

        protected:
            CORE_ALIAS(UNSAFE, Unsafe);

        public:
            /**
             * Return the number of value stored on this array.
             *
             * @return the number of value on this array.
             */
            virtual gint length() const = 0;

            /**
             * Test if this array has no values.
             *
             * @return @c true if this array has no values.
             */
            virtual gbool isEmpty() const;

            /**
             * Return the String representation of this array.
             * If this array is empty, this method will return
             * @c "[]".
             *
             * @return the string representation of this array.
             */
            String toString() const override = 0;

            /**
             * Return shadow copy of this array.
             *
             * @return the shadow copy of this array.
             */
            Object & clone() const override = 0;
        };
    } // misc
} // core

#endif //CORE24_BASICARRAY_H
