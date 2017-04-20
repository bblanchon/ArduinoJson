# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

cmake_minimum_required(VERSION 3.0)
project(ArduinoJson)

enable_testing()

if(${COVERAGE})
	set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
endif()

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
add_subdirectory(third-party/catch)
add_subdirectory(test)
