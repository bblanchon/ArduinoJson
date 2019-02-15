# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_executable(DynamicJsonBufferTests 
	alloc.cpp
	createArray.cpp
	createObject.cpp
	no_memory.cpp
	size.cpp
	startString.cpp
)

target_link_libraries(DynamicJsonBufferTests catch)
add_test(DynamicJsonBuffer DynamicJsonBufferTests)
