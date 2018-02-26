# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(StaticJsonBufferTests 
	alloc.cpp
	size.cpp
	startString.cpp
)

target_link_libraries(StaticJsonBufferTests catch)
add_test(StaticJsonBuffer StaticJsonBufferTests)
