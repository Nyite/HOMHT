#pragma once
#include <Eigen/Eigen>
#include <fmt/ostream.h>
#include <iostream>
#include <random>
#include <unordered_set>

template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &vec)
{
    for (const auto &item : vec) stream << item << " ";
    return stream << '\b';
}

template<typename T>
std::ostream &operator<<(std::ostream &stream, const std::unordered_set<T> &set)
{
    for (const auto &item : set) stream << item << " ";
    return stream << '\b';
}

template<typename T>
struct fmt::formatter<std::unordered_set<T>> : ostream_formatter
{
};

void removeRow(Eigen::MatrixXi &matrix, Eigen::Index rowToRemove);
void removeColumn(Eigen::MatrixXi &matrix, Eigen::Index colToRemove);

namespace HOMHT {
template<typename Numerical>
inline constexpr auto square(Numerical val)
{
    return val * val;
}

template<typename Numerical>
inline constexpr auto cube(Numerical val)
{
    return val * val * val;
}

template<typename Numeric, typename Generator = std::mt19937>
Numeric random(Numeric from, Numeric to, unsigned int seed = 0)
{
    thread_local static Generator gen(seed);

    using dist_type = typename std::conditional<std::is_integral<Numeric>::value,
      std::uniform_int_distribution<Numeric>,
      std::uniform_real_distribution<Numeric>>::type;

    thread_local static dist_type dist;

    return dist(gen, typename dist_type::param_type{ from, to });
}
} // namespace HOMHT
