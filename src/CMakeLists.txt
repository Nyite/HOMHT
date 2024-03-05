file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include)

target_link_system_library(${PROJECT_NAME} PRIVATE fmt)
target_link_system_library(${PROJECT_NAME} PRIVATE Eigen)
target_link_system_library(${PROJECT_NAME} PRIVATE matplotlib_cpp)
