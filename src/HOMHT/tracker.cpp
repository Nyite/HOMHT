#include "HOMHT/HOMHT/tracker.h"

namespace HOMHT {
void HOMHT_Tracker::process(const MeasurementVec &measurements)
{
    if (clusters.empty()) {
        for (const auto &z : measurements) {
            clusters.emplace_back(z);
        }
        return;
    }

    if (HOMHT::current_tick > 7.0) return;
    clusterise(measurements);
}
} // namespace HOMHT
