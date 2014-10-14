cmake_minimum_required(VERSION 2.8.4)
project(ArduinoJson)

file(GLOB_RECURSE SRCS_FILES srcs/*.cpp)
file(GLOB TESTS_FILES tests/*.cpp)

add_library(ArduinoJson ${SRCS_FILES})

include_directories(
    srcs
    third-party/gtest-1.7.0
    third-party/gtest-1.7.0/include)

add_executable(ArduinoJsonTests 
    ${TESTS_FILES}
    third-party/gtest-1.7.0/src/gtest-all.cc
    third-party/gtest-1.7.0/src/gtest_main.cc)

target_link_libraries(ArduinoJsonTests ArduinoJson)