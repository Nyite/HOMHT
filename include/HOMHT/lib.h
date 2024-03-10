#pragma once
#include "HOMHT/constants.h"
#include <cassert>
#include <fmt/chrono.h>
#include <fmt/color.h>
#include <unordered_set>

namespace HOMHT {
typedef double Measurement;
typedef std::vector<Measurement> MeasurementVec;

template<typename T>
MeasurementVec to_vec(const std::unordered_set<T> &input)
{
    MeasurementVec vec;
    vec.reserve(input.size());
    std::copy(input.begin(), input.end(), vec.begin());
    return vec;
}

template<typename Value = Measurement>
struct MeasurementAssociation
{
    Value value;
    bool set = false;
    int missing_count = 0;
    int consecutive_count = 1;
    int assosiations = 0;

    MeasurementAssociation &operator=(const Value &other) &
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

#define HOMHT_TRACE
#define trace(...) _trace(__VA_ARGS__)
#define ftrace(color, ...) trace("{}", fmt::format(color, __VA_ARGS__))

#ifdef GNN_TRACE
#define gnn_trace(...) trace(__VA_ARGS__)
#define gnn_ftrace(...) ftrace(__VA_ARGS__)
#else
#define gnn_trace(...)
#define gnn_ftrace(...)
#endif

#ifdef HOMHT_TRACE
#define homht_trace(...) trace(__VA_ARGS__)
#define homht_ftrace(...) ftrace(__VA_ARGS__)
#else
#define homht_trace(...)
#define homht_ftrace(...)
#endif

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