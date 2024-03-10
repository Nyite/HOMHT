#include "HOMHT/GNN/track.h"
#include <fmt/core.h>
#include <matplotlibcpp.h>

namespace HOMHT {
Track::Track(const Measurement &init_freq)
  : x{ init_freq, 0 }, P(P_init), v{}, B(B_init), freq_est(init_freq), id(++creation_count),
    creation_tick(HOMHT::current_tick)
{
    trace("Created {}", *this);
}

void Track::draw_step() const
{
    namespace plt = matplotlibcpp;
    std::map<std::string, std::any> style{ { "zorder", 3 } };
    if (confirmed()) {
        style["color"] = "g";
        style["linewidth"] = 3;
    } else {
        style["color"] = "r";
    }
    plt::plot<double>({ current_tick - 1, current_tick }, { x_prev(0), x(0) }, style);
}

void Track::draw_strobe() const
{
    const double prev_freq_delta = pow(B_prev, 5.0 / 8.0);
    const double freq_delta = pow(B, 5.0 / 8.0);

    namespace plt = matplotlibcpp;
    std::map<std::string, std::any> style{
        { "zorder", 4 }, { "color", "b" }, { "linestyle", "--" }
    };
    plt::plot<double>({ current_tick - 1, current_tick },
      { prev_freq_est + prev_freq_delta, freq_est + freq_delta },
      style);

    plt::plot<double>({ current_tick - 1, current_tick },
      { prev_freq_est - prev_freq_delta, freq_est - freq_delta },
      style);
}

bool Track::delete_pending() const { return z.missing_count >= Missing_Threshhold; }

bool Track::confirmation_panding() const
{
    return !confirmed() && z.assosiations >= Confirmation_Assosiations_Threshhold
           && z.consecutive_count >= Confirmation_Consecutive_Threshhold;
}

void Track::confirm()
{
    confirmation_tick = HOMHT::current_tick;

    namespace plt = matplotlibcpp;
    plt::plot({ confirmation_tick },
      { x(0) },
      { { "zorder", 4 },
        { "color", "g" },
        { "marker", "o" },
        { "markersize", 7 },
        { "label", fmt::format("Подтверждён {}", *this) } });
    ftrace(fg(fmt::color::forest_green), "Confirmed {}", *this);
}

bool Track::confirmed() const { return confirmation_tick != 0; }
} // namespace HOMHT
