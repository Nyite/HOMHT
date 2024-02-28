cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
  HOMHT
  VERSION 0.0.1
  DESCRIPTION "Hypothesis oriented multihypothesis tracking"
  LANGUAGES CXX C
)

include(cmake/Dependencies.cmake)
load_dependencies()

add_subdirectory(src)

include(cmake/ProjectOptions.cmake)
apply_project_options(${PROJECT_NAME})

if(${PROJECT_NAME}_BUILD_TESTS)
  message("-- Including tests...")
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()
