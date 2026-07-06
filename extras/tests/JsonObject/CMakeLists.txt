# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2026, Benoit BLANCHON
# MIT License

add_executable(JsonObjectTests
	clear.cpp
	compare.cpp
	equals.cpp
	isNull.cpp
	iterator.cpp
	nesting.cpp
	remove.cpp
	set.cpp
	size.cpp
	std_string.cpp
	subscript.cpp
	unbound.cpp
)

add_test(JsonObject JsonObjectTests)

set_tests_properties(JsonObject
	PROPERTIES
		LABELS "Catch"
)
