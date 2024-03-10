#pragma once
#include "HOMHT/lib.h"
#include <Eigen/Eigen>
#include <iomanip>

namespace HOMHT {
struct HOMHT_Track
{
    HOMHT_Track(const Measurement &init_freq);
    Eigen::Vector2d x, x_prev;
    Eigen::Matrix2d P;

    double prob; // joint track probability
    std::vector<size_t> H; // list of hypothesis that include this track

    double v; // inovation
    double B, B_prev; // inovation covariance
    double confirmation_tick = 0;
    double freq_est, prev_freq_est;

    static inline int creation_count = -1;
    int id;
    double creation_tick;

    friend std::ostream &operator<<(std::ostream &stream, const HOMHT_Track &track)
    {
        stream << "HOMHT Track #" << track.id << ": ";
        stream << "x = (" << track.x(0) << ", " << std::setprecision(2) << track.x(1) << ")";
        return stream;
    }
};

} // namespace HOMHT

template<>
struct fmt::formatter<HOMHT::HOMHT_Track> : ostream_formatter
{
};
