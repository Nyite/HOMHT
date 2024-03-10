#pragma once
#include "HOMHT/GNN/track.h"
#include "HOMHT/lib.h"

namespace HOMHT {
class GNN_Tracker final
{
  public:
    std::vector<Track> tracks;
    void process(const MeasurementVec &measurements);

  private:
    MeasurementVec free_measurements;

    void strobe(MeasurementVec measurements);
    void kalman_predict();
    void filter_tarcks();
    void kalman_update();
    void confirm_tracks();
    void process_free_measurements();

    void draw_history();
    void draw_strobe();
};

} // namespace HOMHT
