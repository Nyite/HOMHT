#include "HOMHT/HOMHT/track.h"
#include <fmt/core.h>
#include <matplotlibcpp.h>

namespace HOMHT {
HOMHT_Track::HOMHT_Track(const Measurement &init_freq)
  : x{ init_freq, 0 }, P(P_init), v{}, B(B_init), freq_est(init_freq), id(++creation_count),
    creation_tick(HOMHT::current_tick)
{
    trace("Created {}", *this);
}
} // namespace HOMHT
