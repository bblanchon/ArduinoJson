# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2026, Benoit BLANCHON
# MIT License

add_executable(JsonVariantConstTests
	as.cpp
	is.cpp
	isnull.cpp
	nesting.cpp
	size.cpp
	subscript.cpp
)

add_test(JsonVariantConst JsonVariantConstTests)

set_tests_properties(JsonVariantConst
	PROPERTIES
		LABELS "Catch"
)
