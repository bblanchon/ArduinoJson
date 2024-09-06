# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(
		-w
	)
endif()

if(MSVC)
	add_compile_options(
		/wd4996
	)
endif()

add_executable(DeprecatedTests
	add.cpp
	BasicJsonDocument.cpp
	containsKey.cpp
	createNestedArray.cpp
	createNestedObject.cpp
	DynamicJsonDocument.cpp
	macros.cpp
	memoryUsage.cpp
	shallowCopy.cpp
	StaticJsonDocument.cpp
)

add_test(Deprecated DeprecatedTests)

set_tests_properties(Deprecated
	PROPERTIES
	LABELS "Catch"
)
