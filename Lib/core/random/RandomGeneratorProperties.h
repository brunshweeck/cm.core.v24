//
// Created by brunshweeck on 19 août 2024.
//

#ifndef CORE24_RANDOMGENERATORPROPERTIES_H
#define CORE24_RANDOMGENERATORPROPERTIES_H

#include <core/String.h>

namespace core {
    namespace random {
        class RandomGeneratorProperties final : public Object {
            /**
             * Name of algorithm.
             */
            String algoName;
            String algoGroup;
            gint algoPeriod[3];
            gint algoDistribution;
            gbool algoStochastic;
            gbool algoHardware;

        public:
            /**
             * Construct new instance of Random Generator Properties
             * @param name the name of algorithm
             * @param group the category of algorithm
             * @param i the first part of algorithm period. such as (period = ((1 << i) - j) << k)
             * @param j the second part of algorithm period. such as (period = ((1 << i) - j) << k)
             * @param k the third part of algorithm period. such as (period = ((1 << i) - j) << k)
             * @param equidistribution the equidistribution of the algorithm
             * @param isStochastic the boolean used to determine if the algorithm's based on entropy (true random.)
             * @param isHardware the boolean used to determine if the algorithm assisted by hardware (fast true random.)
             */
            CORE_EXPLICIT RandomGeneratorProperties(String name,
                                     String group = "Legacy"_S,
                                     gint i = 0, gint j = 0, gint k = 0,
                                     gint equidistribution = Integer::MAX_VALUE,
                                     gbool isStochastic = false,
                                     gbool isHardware = false);

            /**
             * Return the name of algorithm.
             *
             * @return the name of Algorithm.
             */
            String name() const;

            /**
             * Return the category of algorithm.
             *
             * @return the category of Algorithm.
             */
            String group() const;

            /**
             * Algorithm period defined as:
             * @code
             *   ((1 << i) - j) << k
             * @endcode
             */
            gint i() const;

            gint j() const;

            gint k() const;

            /**
             * The equidistribution of the algorithm.
             */
            gint equidistribution() const;

            /**
             * Is the algorithm based on entropy (true random.)
             */
            gbool isStochastic() const;

            /**
             * Is the algorithm assisted by hardware (fast true random.)
             */
            gbool isHardware() const;

            gbool equals(const Object &o) const override;

            String toString() const override;
        };
    } // random
} // core

#endif //CORE24_RANDOMGENERATORPROPERTIES_H
