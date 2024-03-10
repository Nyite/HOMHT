#pragma once
#include "HOMHT/HOMHT/hypothesis.h"
#include "HOMHT/HOMHT/track.h"
#include <vector>

namespace HOMHT {
struct Cluster
{
    Cluster(Measurement z);
    std::vector<HOMHT_Track> tracks;
    std::vector<Hypothesis> hyps;
    MeasurementAssociation<MeasurementVec> measurements;

    static inline int creation_count = -1;
    int id;

    void predict_hyps();
    bool strobe(Measurement z);
    bool delete_pending() const;

    friend std::ostream &operator<<(std::ostream &stream, const Cluster &cluster)
    {
        stream << "Cluster #" << cluster.id << ": ";
        stream << "with " << cluster.hyps.size() << " hypothesis";
        return stream;
    }
};

} // namespace HOMHT

template<>
struct fmt::formatter<HOMHT::Cluster> : ostream_formatter
{
};