#include "HOMHT/HOMHT/cluster.h"

namespace HOMHT {
Cluster::Cluster(Measurement z) : id(++creation_count)
{
    tracks.emplace_back(z);
    hyps.reserve(Max_Hypothesis_Number);

    hyps.emplace_back(ft_hyp_init_prob);

    auto &hyp = hyps.emplace_back(nt_hyp_init_prob);
    hyp.tracks.emplace_back(z);

    homht_ftrace(fg(fmt::color::blue), "Created {}", *this);
}

// Estimate new position of tracks in hypothesis
void Cluster::predict_hyps()
{
    for (auto &h : hyps) {
        for (auto &track : h.tracks) {
            track.x = F * track.x;
            track.P = F * track.P * F_T + Q;
            track.B = H * track.P * H_T + R; // Inovation covariance
        }
    }
}

bool Cluster::strobe(Measurement z)
{
    for (auto &h : hyps) {
        for (auto &track : h.tracks) {
            const auto v = z - H * track.x; // Inovation
            const auto MDSQ = square(v / track.B * v); // Mahalanobis distance
            if (MDSQ < sqrt(track.B)) return true;
        }
    }
    return false;
}

bool Cluster::delete_pending() const { return measurements.missing_count >= Missing_Threshhold; }

} // namespace HOMHT
