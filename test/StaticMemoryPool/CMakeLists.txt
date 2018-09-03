# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(StaticMemoryPoolTests 
	alloc.cpp
	size.cpp
	startString.cpp
)

target_link_libraries(StaticMemoryPoolTests catch)
add_test(StaticMemoryPool StaticMemoryPoolTests)
