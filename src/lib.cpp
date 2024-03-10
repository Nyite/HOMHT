#include "HOMHT/lib.h"
#include "HOMHT/utils.h"

namespace HOMHT {
MeasurementVec &generate_mesurements(MeasurementVec &measurements, unsigned int seed)
{
    int freq = Target_Freq_Lower_Bound;
    for (; freq < Target_Freq; freq++) {
        if (HOMHT::random(0.0, 1.0, seed) < Bft) measurements.push_back(freq);
    }

    thread_local static int missing_count = 1;
    if ((HOMHT::random(0.0, 1.0, seed) < Pd || missing_count >= Missing_Threshhold)) {
        measurements.push_back(freq++);
        missing_count = 1;
    } else {
        ++missing_count;
    }

    for (; freq < Target_Freq_Upper_Bound + 1; freq++) {
        if (HOMHT::random(0.0, 1.0, seed) < Bft) measurements.push_back(freq);
    }

    return measurements;
}
} // namespace HOMHT