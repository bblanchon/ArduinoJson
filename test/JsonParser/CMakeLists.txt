# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonParserTests
	DynamicJsonArray.cpp
	DynamicJsonObject.cpp
	DynamicJsonVariant.cpp
	JsonError.cpp
	nestingLimit.cpp
	StaticJsonArray.cpp
	StaticJsonObject.cpp
	StaticJsonVariant.cpp
)

target_link_libraries(JsonParserTests catch)
add_test(JsonParser JsonParserTests)
