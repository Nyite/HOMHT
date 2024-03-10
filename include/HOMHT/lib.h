#pragma once
#include "HOMHT/constants.h"
#include <cassert>
#include <fmt/chrono.h>
#include <fmt/color.h>

namespace HOMHT {
typedef double Measurement;
typedef std::vector<Measurement> MeasurementVec;

struct MeasurementAssociation
{
    Measurement value;
    bool set = false;
    int missing_count = 0;
    int consecutive_count = 1;
    int assosiations = 0;

    MeasurementAssociation &operator=(const Measurement &other) &
    {
        value = other;
        set = true;
        missing_count = 0;
        ++consecutive_count;
        ++assosiations;
        return *this;
    }

    MeasurementAssociation &reset()
    {
        set = false;
        return *this;
    }

    MeasurementAssociation &miss()
    {
        ++missing_count;
        consecutive_count = 0;
        return *this;
    }

    operator bool() const { return set; }
};

#define trace(...) _trace(__VA_ARGS__)
#define ftrace(color, ...) trace("{}", fmt::format(color, __VA_ARGS__))

template<typename... Args>
void _trace(fmt::format_string<Args...> format_string, Args &&...args)
{
    auto time = std::chrono::system_clock::now();
    fmt::print("{:%R:%S} ", time);
    fmt::print(fg(fmt::color::light_blue), "[TRACE tick {}] ", HOMHT::current_tick);
    fmt::println(format_string, std::forward<Args>(args)...);
}

MeasurementVec &generate_mesurements(MeasurementVec &measurements, unsigned int seed = 0);
} // namespace HOMHT