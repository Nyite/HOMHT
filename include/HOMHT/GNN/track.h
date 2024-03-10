#pragma once
#include "HOMHT/lib.h"
#include <Eigen/Eigen>
#include <fmt/ostream.h>
#include <iomanip>

namespace HOMHT {
struct GNN_Track
{
    GNN_Track(const Measurement &init_freq = Target_Freq);
    bool delete_pending() const;
    bool confirmation_panding() const;
    void confirm();
    bool confirmed() const;
    void draw_step() const;
    void draw_strobe() const;

    Eigen::Vector2d x, x_prev;
    Eigen::Matrix2d P;

    MeasurementAssociation<Measurement> z;
    double v; // inovation
    double B, B_prev; // inovation covariance
    double confirmation_tick = 0;
    double freq_est, prev_freq_est;

    static inline int creation_count = -1;
    int id;
    double creation_tick;

    friend std::ostream &operator<<(std::ostream &stream, const GNN_Track &track)
    {
        stream << "GNN Track #" << track.id << ": ";
        stream << "x = (" << track.x(0) << ", " << std::setprecision(2) << track.x(1) << ")";
        return stream;
    }
};

} // namespace HOMHT

template<>
struct fmt::formatter<HOMHT::GNN_Track> : ostream_formatter
{
};
