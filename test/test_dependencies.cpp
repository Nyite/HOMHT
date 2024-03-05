#include <Eigen/Eigen>
#include <fmt/core.h>
#include <gtest/gtest.h>
#include <matplotlibcpp.h>

TEST(TestDependencies, BasicAssertions)
{
    Eigen::Matrix2d mx;
    EXPECT_EQ(mx.size(), 4);

    EXPECT_EQ(fmt::format("{}", "Hello"), "Hello");
}
