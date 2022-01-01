# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2022, Benoit BLANCHON
# MIT License

add_executable(JsonObjectTests 
	clear.cpp
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

set_tests_properties(JsonObject
	PROPERTIES
		LABELS 		"Catch"
)
