# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(JsonArrayTests 
	add.cpp
	copyArray.cpp
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

add_test(JsonArray JsonArrayTests)
