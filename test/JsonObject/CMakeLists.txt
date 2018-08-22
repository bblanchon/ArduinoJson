# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonObjectTests 
	containsKey.cpp
	createNestedArray.cpp
	createNestedObject.cpp
	get.cpp
	invalid.cpp
	is.cpp
	isNull.cpp
	iterator.cpp
	remove.cpp
	set.cpp
	size.cpp
	std_string.cpp
	subscript.cpp
)

target_link_libraries(JsonObjectTests catch)
add_test(JsonObject JsonObjectTests)
