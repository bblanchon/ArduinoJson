# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonArrayTests 
	add.cpp
	basics.cpp
	copyFrom.cpp
	copyTo.cpp
	invalid.cpp
	iterator.cpp
	remove.cpp
	set.cpp
	size.cpp
	std_string.cpp
	subscript.cpp
)

target_link_libraries(JsonArrayTests catch)
add_test(JsonArray JsonArrayTests)
