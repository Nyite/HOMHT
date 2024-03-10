#include "HOMHT/GNN/tracker.h"
#include <Eigen/Eigen>
#include <HOMHT/HOMHT/tracker.h>
#include <any>
#include <iostream>
#include <map>
#include <matplotlibcpp.h>

double HOMHT::current_tick;

int main()
{
    HOMHT::GNN_Tracker gnn;
    HOMHT::HOMHT_Tracker homht;

    namespace plt = matplotlibcpp;
    plt::figure_size(1200, 780);
    HOMHT::MeasurementVec measurements;
    measurements.reserve(HOMHT::Input_Widown_Width + 1);

    using HOMHT::current_tick;
    for (current_tick = 0; current_tick < HOMHT::Simulation_Duration; ++current_tick) {
        HOMHT::generate_mesurements(measurements);
        gnn.process(measurements);

        if (measurements.empty()) continue;
        plt::plot(std::vector<double>(measurements.size(), current_tick), measurements, "sk");
        measurements.clear();
    }

    std::vector<double> xticks(HOMHT::Simulation_Duration);
    double i = -1;
    for (auto &tick : xticks) tick = ++i;
    plt::xticks(xticks);

    plt::grid(true);
    plt::legend({ { "loc", "lower left" } });
    plt::xlabel("Такты");
    plt::ylabel("Фильтры частот");
    plt::show();

    return 0;
}
