//
// Created by brunshweeck on 4 sept. 2024.
//

#ifndef CORE24_VALUEARRAY_H
#define CORE24_VALUEARRAY_H

#include "BasicArray.h"

namespace core {
    namespace misc {
        template <class T>
        class ValueArray : public BasicArray {
            CORE_ALIAS(VALUE, typename Class<T>::Slim);
            CORE_ALIAS(ARRAY, typename Class<VALUE>::Pointer);

            CORE_CHECK_SLIMMED_TYPE(T);
            CORE_FAST_ASSERT(!Class<T>::isAbstract());

            CORE_ADD_GLOBAL_FRIENDS();
            CORE_ADD_AS_FRIEND(StringArray);
            CORE_ADD_AS_FRIEND(io::FileArray);
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

            CORE_ADD_AS_FRIEND(util::Arrays);

            /**
             * The field used to store this array values
             */
            ARRAY value = {};

        protected:
            CORE_ALIAS(VarArgs, std::initializer_list<VALUE>);

            /**
             * Construct new primitive array.
             */
            CORE_IMPLICIT ValueArray() {}

        public:
            /**
             * Return the value of elements at specified index
             *
             * @param i The index of desired element.
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            virtual T &get(gint i) = 0;

            /**
             * Return the value of elements at specified index
             *
             * @param i The index of desired element.
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            virtual T const &get(gint i) const = 0;

            /**
             * Set value of element at specified index with
             * specified new value
             *
             * @param i The index of desired element.
             * @param value The replacement value
             * @throws IndexOutOfBoundsException If the given index out of bounds
             */
            virtual T set(gint i, T value) = 0;

            /**
             * The Array2D class represent the optimization of ValueArray for
             * treatment of 2-dimensional value array.
             *
             * @tparam A the Array2D
             */
            template <class A>
            class Array2D : public ValueArray<A> {

            protected:
                /**
                 * Construct new instance of Array2D
                 */
                CORE_IMPLICIT Array2D() {};

            public:
                /**
                 * Return the value of elements at specified index (2D)
                 *
                 * @param ix The first coordinate of index of desired element.
                 * @param iy The second coordinate of index of desired element.
                 * @throws IndexOutOfBoundsException If the given index out of bounds
                 */
                virtual T &get(gint ix, gint iy) = 0;

                /**
                 * Return the value of elements at specified index (2D)
                 *
                 * @param ix The first coordinate of index of desired element.
                 * @param iy The second coordinate of index of desired element.
                 * @throws IndexOutOfBoundsException If the given index out of bounds
                 */
                virtual T const &get(gint ix, gint iy) const = 0;

                /**
                 * Set value of element at specified index (2D) with
                 * specified new value
                 *
                 * @param ix The first coordinate of index of desired element.
                 * @param iy The second coordinate of index of desired element.
                 * @param value The replacement value
                 * @throws IndexOutOfBoundsException If the given index out of bounds
                 */
                virtual T set(gint ix, gint iy, T value) = 0;
            };
        };
    } // misc
} // core

#endif //CORE24_VALUEARRAY_H
