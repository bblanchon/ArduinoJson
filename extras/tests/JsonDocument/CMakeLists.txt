# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2026, Benoit BLANCHON
# MIT License

add_executable(JsonDocumentTests
	add.cpp
	assignment.cpp
	cast.cpp
	clear.cpp
	compare.cpp
	constructor.cpp
	ElementProxy.cpp
	isNull.cpp
	issue1120.cpp
	MemberProxy.cpp
	nesting.cpp
	overflowed.cpp
	remove.cpp
	set.cpp
	shrinkToFit.cpp
	size.cpp
	subscript.cpp
	swap.cpp
)

add_test(JsonDocument JsonDocumentTests)

set_tests_properties(JsonDocument
	PROPERTIES
		LABELS "Catch"
)
