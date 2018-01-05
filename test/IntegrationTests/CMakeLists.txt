# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(IntegrationTests 
	gbathree.cpp
	round_trip.cpp
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	target_compile_options(IntegrationTests
		PUBLIC
		-fsingle-precision-constant # issue 544
	)
endif()

target_link_libraries(IntegrationTests catch)
add_test(IntegrationTests IntegrationTests)
