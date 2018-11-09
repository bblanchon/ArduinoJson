# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(StaticMemoryPoolTests 
	allocVariant.cpp
	allocString.cpp
	clear.cpp
	size.cpp
	StringBuilder.cpp
)

target_link_libraries(StaticMemoryPoolTests catch)
add_test(StaticMemoryPool StaticMemoryPoolTests)
