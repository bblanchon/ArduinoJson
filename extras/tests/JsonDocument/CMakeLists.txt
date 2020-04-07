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
	isNull.cpp
	nesting.cpp
	remove.cpp
	shrinkToFit.cpp
	size.cpp
	StaticJsonDocument.cpp
	subscript.cpp
)

target_link_libraries(JsonDocumentTests
	ArduinoJson
	catch
)

add_test(JsonDocument JsonDocumentTests)
