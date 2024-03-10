#pragma once
#include "HOMHT/utils.h"
#include <Eigen/Eigen>

namespace HOMHT {
extern double current_tick;
/**************** Algorithm parameters ****************/
constexpr double Pd = 0.3; // Probability of detection
constexpr double Bft = 0.05; // Dencity of false targets
constexpr double Bnt = Bft / 5; // Dencity of new targets
constexpr double Phi_s = 0.1; // Physical model white noise power

// Minimal number of times a track must be assosiated in a row to be consudered for confirmation
constexpr int Confirmation_Consecutive_Threshhold = 3;
// Minimal number of times a track must be assosiated to be consudered for confirmation
constexpr int Confirmation_Assosiations_Threshhold = 4;
// Maximum tick distance between two traget points
constexpr int Missing_Threshhold = 3;

/**************** Simulation parameters ****************/
constexpr double Simulation_Duration = 40.0; // Total number of simulated ticks
constexpr double Delta_t = 0.1; // Time delta between ticks

constexpr int Target_Freq = 1600; // Freqency of the target
constexpr int Input_Widown_Width = 40; // 3-sigma Frequency region
constexpr int Target_Freq_Lower_Bound = Target_Freq - Input_Widown_Width / 2;
constexpr int Target_Freq_Upper_Bound = Target_Freq + Input_Widown_Width / 2;

/**************** Init parameters ****************/
constexpr double Delta_Freq = 5;
constexpr double Sigma_Freq = Delta_Freq / 3.0;
constexpr double Delta_Change = 0.1;
constexpr double Sigma_Change = Delta_Change / 3.0;

const Eigen::RowVector2d H{ 1, 0 };
const Eigen::Vector2d H_T = H.transpose();
constexpr double R = square(Sigma_Freq);
const Eigen::Matrix2d F{ { 1, Delta_t }, { 0, 1 } };
const Eigen::Matrix2d F_T = F.transpose();
const Eigen::Matrix2d Q{ { cube(Delta_t) / 3, square(Delta_t) / 2 }, { square(Delta_t) / 2, 1 } };

const Eigen::Matrix2d P_init{ { square(Sigma_Freq), 0 }, { 0, square(Sigma_Change) } };
const double B_init = H * P_init * H_T + R;
const auto I_P = Eigen::Matrix2d::Identity();
} // namespace HOMHT
