# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2023, Benoit BLANCHON
# MIT License

if(CMAKE_CXX_COMPILER_ID MATCHES "(GNU|Clang)")
	add_compile_options(
		-Wno-deprecated-declarations
	)
endif()

if(MSVC)
	add_compile_options(
		/wd4996
	)
endif()

add_executable(DeprecatedTests
	DynamicJsonDocument.cpp
	StaticJsonDocument.cpp
)

add_test(Deprecated DeprecatedTests)

set_tests_properties(Deprecated
	PROPERTIES
	LABELS "Catch"
)
