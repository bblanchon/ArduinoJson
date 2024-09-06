# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

add_executable(JsonObjectConstTests
	equals.cpp
	isNull.cpp
	iterator.cpp
	nesting.cpp
	size.cpp
	subscript.cpp
)

add_test(JsonObjectConst JsonObjectConstTests)

set_tests_properties(JsonObjectConst
	PROPERTIES
		LABELS "Catch"
)
