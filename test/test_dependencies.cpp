#include <Eigen/Eigen>
#include <gtest/gtest.h>
#include <matplotlibcpp.h>

TEST(TestDependencies, BasicAssertions)
{
    Eigen::Matrix2d mx;
    EXPECT_EQ(mx.size(), 4);
}
