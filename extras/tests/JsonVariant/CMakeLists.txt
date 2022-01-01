# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2022, Benoit BLANCHON
# MIT License

add_executable(JsonVariantTests
	add.cpp
	as.cpp
	clear.cpp
	compare.cpp
	containsKey.cpp
	copy.cpp
	converters.cpp
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
	unbound.cpp
)

add_test(JsonVariant JsonVariantTests)

set_tests_properties(JsonVariant
	PROPERTIES
		LABELS 		"Catch"
)
