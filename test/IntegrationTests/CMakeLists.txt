# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

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
