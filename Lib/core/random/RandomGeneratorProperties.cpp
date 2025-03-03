//
// Created by brunshweeck on 19 août 2024.
//

#include "RandomGeneratorProperties.h"

#include <core/misc/Unsafe.h>

namespace core {
    namespace random {
        RandomGeneratorProperties::RandomGeneratorProperties(String name,
                                                             String group,
                                                             gint i, gint j, gint k,
                                                             gint equidistribution,
                                                             gbool isStochastic,
                                                             gbool isHardware)
            : algoName(UNSAFE::moveInstance(name)),
              algoGroup(UNSAFE::moveInstance(group)),
              algoPeriod{i, j, k},
              algoDistribution(equidistribution),
              algoStochastic(isStochastic),
              algoHardware(isHardware) {}

        String RandomGeneratorProperties::name() const {
            return algoName;
        }

        String RandomGeneratorProperties::group() const {
            return algoGroup;
        }

        gint RandomGeneratorProperties::i() const {
            return algoPeriod[0];
        }

        gint RandomGeneratorProperties::j() const {
            return algoPeriod[1];
        }

        gint RandomGeneratorProperties::k() const {
            return algoPeriod[2];
        }

        gint RandomGeneratorProperties::equidistribution() const {
            return algoDistribution;
        }

        gbool RandomGeneratorProperties::isStochastic() const {
            return algoStochastic;
        }

        gbool RandomGeneratorProperties::isHardware() const {
            return algoHardware;
        }

        gbool RandomGeneratorProperties::equals(const Object& o) const {
            if (this == &o)
                return true;
            if (!Class<RandomGeneratorProperties>::hasInstance(o))
                return true;
            RandomGeneratorProperties const& prop = CORE_XCAST(RandomGeneratorProperties const, o);
            return algoName == prop.algoName &&
                    algoGroup == prop.algoGroup &&
                    algoPeriod[0] == prop.algoPeriod[0] &&
                    algoPeriod[1] == prop.algoPeriod[1] &&
                    algoPeriod[2] == prop.algoPeriod[2] &&
                    algoDistribution == prop.algoDistribution &&
                    algoStochastic == prop.algoStochastic &&
                    algoHardware == prop.algoHardware;
        }

        String RandomGeneratorProperties::toString() const {
            return String::format("RandomGeneratorProperties[name=%s, group=%s, period=[i=%d, j=%d, k=%d]]",
                                  algoName, algoGroup, algoPeriod[0], algoPeriod[1], algoPeriod[2]);
            // return "RandomGeneratorProperties[name="_S + algoName
            //        + ", group="_S + algoGroup
            //        + "period[i="_S + algoPeriod[0] + ", j="_S + algoPeriod[1] + ", k="_S + algoPeriod[2] + "]]"_S;
        }
    } // random
} // core
