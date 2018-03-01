# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonSerializerTests
	JsonArray.cpp
	JsonArrayPretty.cpp
	JsonObject.cpp
	JsonObjectPretty.cpp
	JsonVariant.cpp
)

target_link_libraries(JsonSerializerTests catch)
add_test(JsonSerializer JsonSerializerTests)
