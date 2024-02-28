function(check_sanitizers_support)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endfunction()

function(load_project_options project_name)
  check_sanitizers_support()

  option(${project_name}_BUILD_TESTS "Build tests" ON)
  option(${project_name}_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
  option(${project_name}_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
  option(${project_name}_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
  option(${project_name}_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
  option(${project_name}_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
  option(${project_name}_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
endfunction()

function(apply_project_options project_name)
  load_project_options(${project_name})

  include(cmake/CompilerWarnings.cmake)
  set_project_warnings(${project_name})
  
  include(cmake/Sanitizers.cmake)
  enable_sanitizers(
    ${project_name}
    ${${project_name}_ENABLE_SANITIZER_ADDRESS}
    ${${project_name}_ENABLE_SANITIZER_LEAK}
    ${${project_name}_ENABLE_SANITIZER_UNDEFINED}
    ${${project_name}_ENABLE_SANITIZER_THREAD}
    ${${project_name}_ENABLE_SANITIZER_MEMORY})

endfunction()
