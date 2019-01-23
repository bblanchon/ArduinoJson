# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonDocumentTests
	DynamicJsonDocument.cpp
	nesting.cpp
	isNull.cpp
	StaticJsonDocument.cpp
	subscript.cpp
)

target_link_libraries(JsonDocumentTests catch)
add_test(JsonDocument JsonDocumentTests)
