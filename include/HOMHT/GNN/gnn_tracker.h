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
    static inline int current_tick = -1;
    MeasurementVec free_measurements;

    void strobe(const MeasurementVec &measurements);
    void kalman_predict();
    void filter_tarcks();
    void kalman_update();
    void process_free_measurements();
};

struct GNN_Tracker::Track
{
    Track(const Measurement &z = Target_Freq);
    inline void update_hustory();
    inline bool delete_pending() const;
    inline bool confirmation_panding() const;
    inline void confirm();
    inline bool confirmed() const;
    void draw_history() const;

    Eigen::Vector2d x;
    Eigen::Matrix2d P;

    MeasurementAssociation z;
    double v; // inovation
    double B; // inovation covariance
    int confirmation_tick = 0;

    static inline int creation_count = -1;
    int id;
    int creation_tick;
    std::vector<double> x_history;
    std::vector<double> pred_history;
    std::vector<double> B_history;

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
