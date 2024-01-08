# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

add_executable(JsonArrayConstTests
	equals.cpp
	isNull.cpp
	iterator.cpp
	nesting.cpp
	size.cpp
	subscript.cpp
)

add_test(JsonArrayConst JsonArrayConstTests)

set_tests_properties(JsonArrayConst
	PROPERTIES
		LABELS "Catch"
)
