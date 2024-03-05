function(load_dependencies)
    include(cmake/CPM.cmake)
    include(cmake/SystemLink.cmake)

    # Load fmt
    CPMAddPackage("gh:fmtlib/fmt#10.2.1")

    # Load Eigen lib
    CPMAddPackage(
    NAME Eigen
    VERSION 3.4.0
    URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
    DOWNLOAD_ONLY YES 
    )
    if(Eigen_ADDED)
    add_library(Eigen INTERFACE IMPORTED)
    target_include_directories(Eigen INTERFACE ${Eigen_SOURCE_DIR})
    endif()

    # Load matplotlib
    CPMAddPackage("gh:Nyite/matplotlib-cpp#master")
    target_include_system_directories(matplotlib_cpp INTERFACE "${matplotlib_cpp_SOURCE_DIR}")

    # Load GTest
    CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        VERSION 1.14.0
        OPTIONS
            "INSTALL_GTEST OFF"
            "gtest_force_shared_crt ON"
    )
endfunction()
