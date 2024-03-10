#pragma once
#include "HOMHT/HOMHT/track.h"
#include <Eigen/Eigen>
#include <vector>

namespace HOMHT {
struct Hypothesis
{
    struct Track
    {
        Track(Measurement z) : x{ z, 0 }, P(P_init) {}

        std::vector<size_t> N;
        Eigen::Vector2d x;
        Eigen::Matrix2d P;
        double B;

        bool used = false;
    };

    Hypothesis(double init_prob) : prob(init_prob){};
    std::vector<Track> tracks;
    double prob;
};
} // namespace HOMHT
