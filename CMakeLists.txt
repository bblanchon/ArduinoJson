# ArduinoJson - https://arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

cmake_minimum_required(VERSION 3.3)

if(ESP_PLATFORM)
   # Build ArduinoJson as an ESP-IDF component
   idf_component_register(INCLUDE_DIRS src)
   target_compile_definitions(${COMPONENT_LIB} INTERFACE ARDUINOJSON_EMBEDDED_MODE=1)
   return()
endif()

project(ArduinoJson VERSION 6.18.1)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

add_subdirectory(src)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	include(extras/CompileOptions.cmake)
	add_subdirectory(extras/tests)
	add_subdirectory(extras/fuzzing)
endif()
