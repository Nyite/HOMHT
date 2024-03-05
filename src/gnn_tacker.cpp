#include "HOMHT/GNN/gnn_tracker.h"
#include <fmt/core.h>
#include <matplotlibcpp.h>

namespace HOMHT {
GNN_Tracker::Track::Track(const Measurement &z)
  : x{ z, 0 }, P(P_init), v{}, B(B_init), id(++creation_count), creation_tick(HOMHT::current_tick)
{
    x_history.reserve(Simulation_Duration);
    pred_history.reserve(Simulation_Duration);
    B_history.reserve(Simulation_Duration);

    pred_history.push_back(z);
    update_hustory();
    trace("Created {}", *this);
}

void GNN_Tracker::Track::draw_history() const
{
    namespace plt = matplotlibcpp;
    std::vector<int> life_ticks(x_history.size());
    int tick = creation_tick;
    for (size_t i = 0; tick <= HOMHT::current_tick; ++tick, ++i) life_ticks[i] = tick;
    plt::plot(life_ticks, x_history, "-r");

    if (confirmed()) {
        std::vector<double> strobe_upper_bound(B_history.size());
        std::vector<double> strobe_lower_bound(B_history.size());
        for (size_t i = 0; i < B_history.size(); ++i) {
            const auto freq_delta = pow(B_history[i], 5.0 / 8.0);
            const auto &strobe_center = pred_history[i];
            strobe_upper_bound[i] = strobe_center + freq_delta;
            strobe_lower_bound[i] = strobe_center - freq_delta;
        }
        plt::plot(life_ticks, strobe_upper_bound, "--b");
        plt::plot(life_ticks, strobe_lower_bound, "--b");

        plt::named_plot(fmt::format("Confirmed {}", *this),
          std::vector{ confirmation_tick },
          std::vector{ x_history[static_cast<size_t>(confirmation_tick - creation_tick)] },
          "go");
    }
}

void GNN_Tracker::Track::update_hustory()
{
    x_history.push_back(x(0));
    B_history.push_back(B);
}

bool GNN_Tracker::Track::delete_pending() const { return z.missing_count >= Missing_Threshhold; }

bool GNN_Tracker::Track::confirmation_panding() const
{
    return !confirmed() && z.assosiations >= Confirmation_Assosiations_Threshhold
           && z.consecutive_count >= Confirmation_Consecutive_Threshhold;
}

void GNN_Tracker::Track::confirm()
{
    confirmation_tick = HOMHT::current_tick;
    ftrace(fg(fmt::color::forest_green), "Confirmed {}", *this);
}

bool GNN_Tracker::Track::confirmed() const { return confirmation_tick != 0; }

void GNN_Tracker::process(const MeasurementVec &measurements)
{
    if (tracks.empty()) {
        for (const auto &z : measurements) {
            tracks.emplace_back(z);
        }
        return;
    }

    kalman_predict();
    strobe(measurements);
    filter_tarcks();
    kalman_update();
    process_free_measurements();
}

void GNN_Tracker::kalman_predict()
{
    for (auto &track : tracks) {
        track.x = F * track.x;
        track.P = F * track.P * F_T + Q;
        track.pred_history.push_back(H * track.x);
    }
}

/*
 * Note: track order dependent approach.
 *       Can result in mismatch of
 *       2 close measurements to 2 close tracks
 */
void GNN_Tracker::strobe(MeasurementVec measurements)
{
    for (auto &track : tracks) {
        track.z.reset();
        track.B = H * track.P * H_T + R; // Inovation covariance

        std::optional<size_t> strobe_i;
        double min_dist = std::numeric_limits<double>::max();
        for (size_t m_i = 0; m_i < measurements.size(); ++m_i) {
            const auto &z = measurements[m_i];

            const auto v = z - H * track.x; // Inovation
            const auto MDSQ = square(v / track.B * v); // Mahalanobis distance
            if (MDSQ > sqrt(track.B) || MDSQ >= min_dist) continue; // Filtering
            min_dist = MDSQ;
            strobe_i = m_i;
            track.z = z;
            track.v = v;
        }

        if (strobe_i) {
            std::swap(measurements[strobe_i.value()], measurements.back());
            measurements.pop_back();
        } else {
            track.z.miss();
        }
    }

    free_measurements = std::move(measurements);
}

void GNN_Tracker::filter_tarcks()
{
    for (size_t t_i = 0; t_i < tracks.size();) {
        auto &track = tracks[t_i];
        if (track.delete_pending()) {
            track.update_hustory();
            track.draw_history();
            ftrace(fg(fmt::color::pale_violet_red), "Deleted {}", track);
            std::swap(track, tracks.back());
            tracks.pop_back();
            continue;
        }
        if (track.confirmation_panding()) {
            track.confirm();
        }
        ++t_i;
    }
}

void GNN_Tracker::kalman_update()
{
    for (auto &track : tracks) {
        if (track.z) {
            const auto K = track.P * H_T / track.B;
            const auto IKH = I_P - K * H;
            track.x = track.x + K * track.v;
            track.P = IKH * track.P * IKH.transpose() + K * R * K.transpose();
        }

        track.update_hustory();
    }
}

void GNN_Tracker::process_free_measurements()
{
    if (free_measurements.empty()) return;
    for (const auto &z : free_measurements) tracks.emplace_back(z);
    free_measurements.clear();
}
} // namespace HOMHT
