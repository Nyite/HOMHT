#pragma once
#include "HOMHT/lib.h"
#include <Eigen/Eigen>
#include <fmt/ostream.h>
#include <optional>

namespace HOMHT {
class GNN_Tracker final
{
  public:
    struct Track;
    std::vector<Track> tracks;
    void process(const MeasurementVec &measurements);

  private:
    MeasurementVec free_measurements;

    void strobe(MeasurementVec measurements);
    void kalman_predict();
    void filter_tarcks();
    void kalman_update();
    void draw_history();
    void process_free_measurements();
};

struct GNN_Tracker::Track
{
    Track(const Measurement &init_freq = Target_Freq);
    inline bool delete_pending() const;
    inline bool confirmation_panding() const;
    inline void confirm();
    inline bool confirmed() const;
    inline void draw_step() const;

    Eigen::Vector2d x, x_prev;
    Eigen::Matrix2d P;

    MeasurementAssociation z;
    double v; // inovation
    double B; // inovation covariance
    double confirmation_tick = 0;

    static inline int creation_count = -1;
    int id;
    double creation_tick;

    friend std::ostream &operator<<(std::ostream &stream, const Track &track)
    {
        stream << "GNN Track #" << track.id << ": ";
        stream << "x = (" << track.x(0) << ", " << track.x(1) << ")";
        return stream;
    }
};

} // namespace HOMHT

template<>
struct fmt::formatter<HOMHT::GNN_Tracker::Track> : ostream_formatter
{
};
