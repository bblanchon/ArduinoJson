# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2023, Benoit BLANCHON
# MIT License

if(MSVC_VERSION LESS 1910)
	return()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5)
	return()
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 7)
	return()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cpp17Tests
	string_view.cpp
)

add_test(Cpp17 Cpp17Tests)

set_tests_properties(Cpp17
	PROPERTIES
		LABELS "Catch"
)
