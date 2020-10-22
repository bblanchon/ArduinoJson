# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(JsonDocumentTests
	add.cpp
	BasicJsonDocument.cpp
	compare.cpp
	containsKey.cpp
	createNested.cpp
	DynamicJsonDocument.cpp
	ElementProxy.cpp
	isNull.cpp
	MemberProxy.cpp
	nesting.cpp
	overflowed.cpp
	remove.cpp
	shrinkToFit.cpp
	size.cpp
	StaticJsonDocument.cpp
	subscript.cpp
)

add_test(JsonDocument JsonDocumentTests)
