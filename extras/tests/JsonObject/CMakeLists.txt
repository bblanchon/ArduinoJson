# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(JsonObjectTests 
	containsKey.cpp
	copy.cpp
	createNestedArray.cpp
	createNestedObject.cpp
	equals.cpp
	invalid.cpp
	isNull.cpp
	iterator.cpp
	memoryUsage.cpp
	nesting.cpp
	remove.cpp
	size.cpp
	std_string.cpp
	subscript.cpp
)

add_test(JsonObject JsonObjectTests)
