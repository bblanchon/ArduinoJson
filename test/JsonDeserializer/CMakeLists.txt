# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonDeserializerTests
	DeserializationError.cpp
	deserializeJsonArray.cpp
	deserializeJsonArrayStatic.cpp
	deserializeJsonObject.cpp
	deserializeJsonObjectStatic.cpp
	deserializeJsonValue.cpp
	input_types.cpp
	nestingLimit.cpp
)

target_link_libraries(JsonDeserializerTests catch)
add_test(JsonDeserializer JsonDeserializerTests)
