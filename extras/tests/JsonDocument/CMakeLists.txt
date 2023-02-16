# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2023, Benoit BLANCHON
# MIT License

add_executable(JsonDocumentTests
	add.cpp
	BasicJsonDocument.cpp
	cast.cpp
	compare.cpp
	containsKey.cpp
	createNested.cpp
	DynamicJsonDocument.cpp
	ElementProxy.cpp
	isNull.cpp
	issue1120.cpp
	MemberProxy.cpp
	nesting.cpp
	overflowed.cpp
	remove.cpp
	shrinkToFit.cpp
	size.cpp
	StaticJsonDocument.cpp
	subscript.cpp
	swap.cpp
)

add_test(JsonDocument JsonDocumentTests)

set_tests_properties(JsonDocument
	PROPERTIES
		LABELS "Catch"
)
