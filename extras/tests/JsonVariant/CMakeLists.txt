# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

add_executable(JsonVariantTests
	add.cpp
	as.cpp
	clear.cpp
	compare.cpp
	converters.cpp
	copy.cpp
	is.cpp
	isnull.cpp
	misc.cpp
	nesting.cpp
	nullptr.cpp
	or.cpp
	overflow.cpp
	remove.cpp
	set.cpp
	size.cpp
	stl_containers.cpp
	subscript.cpp
	types.cpp
	unbound.cpp
)

add_test(JsonVariant JsonVariantTests)

set_tests_properties(JsonVariant
	PROPERTIES
		LABELS "Catch"
)
