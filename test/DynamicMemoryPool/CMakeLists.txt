# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(DynamicMemoryPoolTests
	allocString.cpp
	allocVariant.cpp
	blocks.cpp
	clear.cpp
	no_memory.cpp
	size.cpp
	StringBuilder.cpp
)

target_link_libraries(DynamicMemoryPoolTests catch)
add_test(DynamicMemoryPool DynamicMemoryPoolTests)
