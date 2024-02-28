#include "HOMHT/constants.h"
#include <Eigen/Eigen>
#include <iostream>
#include <matplotlibcpp.h>

int main()
{
    Eigen::Vector4i mx{ 1, 2, 3, 4 };
    std::cout << mx.transpose();

    namespace plt = matplotlibcpp;
    plt::plot({ 1, 3, 2, 4 });
    plt::show();
    return 0;
}
