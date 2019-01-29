# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonVariantTests
	add.cpp
	as.cpp
	compare.cpp
	copy.cpp
	createNested.cpp
	get.cpp
	is.cpp
	isnull.cpp
	memoryUsage.cpp
	misc.cpp
	nesting.cpp
	or.cpp
	set.cpp
	subscript.cpp
	types.cpp
	undefined.cpp
)

target_link_libraries(JsonVariantTests catch)
add_test(JsonVariant JsonVariantTests)
