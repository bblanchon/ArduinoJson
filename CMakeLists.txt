# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

cmake_minimum_required(VERSION 3.0)
project(ArduinoJson)

enable_testing()

if(${COVERAGE})
	set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
add_subdirectory(third-party/catch)
add_subdirectory(test)
