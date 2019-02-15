# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

cmake_minimum_required(VERSION 3.0)
project(ArduinoJson)

enable_testing()

add_definitions(-DARDUINOJSON_DEBUG)
if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(-g -O0)
endif()

if(${COVERAGE})
	set(CMAKE_CXX_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
add_subdirectory(third-party/catch)
add_subdirectory(test)
add_subdirectory(fuzzing)
