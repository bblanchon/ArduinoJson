# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

add_executable(JsonVariantTests
	add.cpp
	as.cpp
	clear.cpp
	compare.cpp
	containsKey.cpp
	copy.cpp
	createNested.cpp
	is.cpp
	isnull.cpp
	memoryUsage.cpp
	misc.cpp
	nesting.cpp
	or.cpp
	overflow.cpp
	remove.cpp
	set.cpp
	subscript.cpp
	types.cpp
	undefined.cpp
)

add_test(JsonVariant JsonVariantTests)
