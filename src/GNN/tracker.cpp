#include "HOMHT/GNN/tracker.h"
#include <any>
#include <fmt/core.h>
#include <matplotlibcpp.h>
#include <optional>

namespace HOMHT {
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

    draw_history();
    draw_strobe();

    confirm_tracks();
    process_free_measurements();
}

void GNN_Tracker::kalman_predict()
{
    for (auto &track : tracks) {
        track.x_prev = track.x;
        track.B_prev = track.B;

        track.x = F * track.x;
        track.P = F * track.P * F_T + Q;
        track.B = H * track.P * H_T + R; // Inovation covariance

        track.prev_freq_est = track.freq_est;
        track.freq_est = track.x(0);
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
            track.draw_step();
            if (track.confirmed()) track.draw_strobe();
            gnn_ftrace(fg(fmt::color::pale_violet_red), "Deleted {}", track);

            std::swap(track, tracks.back());
            tracks.pop_back();
            continue;
        }
        ++t_i;
    }
}

void GNN_Tracker::confirm_tracks()
{
    for (auto &track : tracks) {
        if (track.confirmation_panding()) track.confirm();
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
    }
}

void GNN_Tracker::draw_strobe()
{
    namespace plt = matplotlibcpp;
    for (auto &track : tracks) {
        if (!track.confirmed()) continue;
        track.draw_strobe();
    }
}

void GNN_Tracker::draw_history()
{
    namespace plt = matplotlibcpp;
    for (auto &track : tracks) {
        if (track.creation_tick == current_tick) continue;
        track.draw_step();
    }
}

void GNN_Tracker::process_free_measurements()
{
    if (free_measurements.empty()) return;
    for (const auto &z : free_measurements) tracks.emplace_back(z);
    free_measurements.clear();
}
} // namespace HOMHT
