cmake_minimum_required(VERSION 2.8)

project(conga)

include_directories(conga)

add_subdirectory(external)

################################
# ZeroMQ
################################
include_directories(${ZeroMQ_SOURCE_DIR}/include)
include_directories(external/cppzmq)

################################
# Testing
################################
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(tests)
