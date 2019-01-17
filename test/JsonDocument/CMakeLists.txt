# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonDocumentTests
	DynamicJsonDocument.cpp
	nesting.cpp
	StaticJsonDocument.cpp
)

target_link_libraries(JsonDocumentTests catch)
add_test(JsonDocument JsonDocumentTests)
