# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonArrayTests 
	add.cpp
	copyFrom.cpp
	copyTo.cpp
	createNested.cpp
	equals.cpp
	get.cpp
	isNull.cpp
	iterator.cpp
	memoryUsage.cpp
	nesting.cpp
	remove.cpp
	size.cpp
	std_string.cpp
	subscript.cpp
	undefined.cpp
)

target_link_libraries(JsonArrayTests catch)
add_test(JsonArray JsonArrayTests)
