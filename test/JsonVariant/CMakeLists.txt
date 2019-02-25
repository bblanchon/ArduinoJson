# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_executable(JsonVariantTests
	add.cpp
	as.cpp
	clear.cpp
	compare.cpp
	copy.cpp
	createNested.cpp
	is.cpp
	isnull.cpp
	memoryUsage.cpp
	misc.cpp
	nesting.cpp
	or.cpp
	remove.cpp
	set.cpp
	subscript.cpp
	types.cpp
	undefined.cpp
)

target_link_libraries(JsonVariantTests catch)
add_test(JsonVariant JsonVariantTests)
