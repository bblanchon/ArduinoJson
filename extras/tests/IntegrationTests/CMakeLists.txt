# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2023, Benoit BLANCHON
# MIT License

add_executable(IntegrationTests
	gbathree.cpp
	issue772.cpp
	round_trip.cpp
	openweathermap.cpp
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 6)
	target_compile_options(IntegrationTests
		PUBLIC
			-fsingle-precision-constant # issue 544
	)
endif()

add_test(IntegrationTests IntegrationTests)

set_tests_properties(IntegrationTests
	PROPERTIES
		LABELS "Catch"
)
