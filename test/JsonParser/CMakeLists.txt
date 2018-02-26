# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonParserTests
	JsonArray.cpp
	JsonObject.cpp
	JsonVariant.cpp
	nestingLimit.cpp
	StaticJsonArray.cpp
	StaticJsonObject.cpp
)

target_link_libraries(JsonParserTests catch)
add_test(JsonParser JsonParserTests)
