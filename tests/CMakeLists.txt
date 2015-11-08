################################
# C++11
################################
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

################################
# Unit tests
################################
add_library(unit_test_main main.cpp)

macro(add_unit_test test_name)
  set(target_name run_test_${test_name})
  add_executable(${target_name} ${test_name}.cpp)
  target_link_libraries(${target_name} gtest unit_test_main libzmq)
  add_test(NAME test_${test_name} COMMAND ${target_name})
endmacro(add_unit_test)

add_unit_test(version)
add_unit_test(transport)
