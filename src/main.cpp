#include "HOMHT/gnn_tracker.h"
#include <Eigen/Eigen>
#include <iostream>
#include <matplotlibcpp.h>

int HOMHT::current_tick;

int main()
{
    HOMHT::GNN_Tracker gnn;

    namespace plt = matplotlibcpp;
    plt::figure_size(1200, 780);
    HOMHT::MeasurementVec measurements;
    measurements.reserve(HOMHT::Input_Widown_Width + 1);

    using HOMHT::current_tick;
    for (current_tick = 0; current_tick < HOMHT::Simulation_Duration; ++current_tick) {
        HOMHT::generate_mesurements(measurements);
        gnn.process(measurements);

        if (measurements.empty()) continue;
        plt::plot(std::vector<int>(measurements.size(), current_tick), measurements, "sk");
        measurements.clear();
    }

    // --current_tick; // Draw up to the last tick
    // for (const auto &track : gnn.tracks) track.draw_history();

    std::vector<int> xticks(HOMHT::Simulation_Duration);
    int i = -1;
    for (auto &tick : xticks) tick = ++i;
    plt::xticks(xticks);

    plt::grid(true);
    // plt::legend();
    plt::show();

    return 0;
}
