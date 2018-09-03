# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(DynamicMemoryPoolTests 
	alloc.cpp
	no_memory.cpp
	size.cpp
	startString.cpp
)

target_link_libraries(DynamicMemoryPoolTests catch)
add_test(DynamicMemoryPool DynamicMemoryPoolTests)
