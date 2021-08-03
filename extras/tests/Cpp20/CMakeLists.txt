# ArduinoJson - https://arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

if(MSVC_VERSION LESS 1910)
	return()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
	return()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10)
	return()
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cpp20Tests
	smoke_test.cpp
)

add_test(Cpp20 Cpp20Tests)

set_tests_properties(Cpp20
	PROPERTIES
		LABELS 		"Catch"
)
