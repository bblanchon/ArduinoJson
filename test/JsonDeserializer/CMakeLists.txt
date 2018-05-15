# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonDeserializerTests
	deserializeJsonArray.cpp
	deserializeJsonArrayStatic.cpp
	deserializeJsonObject.cpp
	deserializeJsonObjectStatic.cpp
	deserializeJsonValue.cpp
	DeserializationError.cpp
	nestingLimit.cpp
	std_istream.cpp
	std_string.cpp
)

target_link_libraries(JsonDeserializerTests catch)
add_test(JsonDeserializer JsonDeserializerTests)
