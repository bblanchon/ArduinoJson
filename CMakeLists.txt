# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

cmake_minimum_required(VERSION 3.0)

project(ArduinoJson VERSION 6.16.1)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	include(extras/CompileOptions.cmake)
	add_subdirectory(extras/tests)
	add_subdirectory(extras/fuzzing)
endif()
