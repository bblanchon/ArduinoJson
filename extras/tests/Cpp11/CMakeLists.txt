# ArduinoJson - https://arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

if("cxx_nullptr" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
	list(APPEND SOURCES nullptr.cpp)
	add_definitions(-DARDUINOJSON_HAS_NULLPTR=1)
endif()

if("cxx_auto_type" IN_LIST CMAKE_CXX_COMPILE_FEATURES AND "cxx_constexpr" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
	list(APPEND SOURCES issue1120.cpp)
endif()

if("cxx_long_long_type" IN_LIST CMAKE_CXX_COMPILE_FEATURES)
	list(APPEND SOURCES use_long_long_0.cpp use_long_long_1.cpp)
endif()

if(NOT SOURCES)
	return()
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cpp11Tests ${SOURCES})

add_test(Cpp11 Cpp11Tests)

set_tests_properties(Cpp11
	PROPERTIES
		LABELS 		"Catch"
)
