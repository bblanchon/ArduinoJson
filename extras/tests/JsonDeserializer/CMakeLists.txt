# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(JsonDeserializerTests
	array.cpp
	array_static.cpp
	DeserializationError.cpp
	incomplete_input.cpp
	input_types.cpp
	number.cpp
	invalid_input.cpp
	misc.cpp
	nestingLimit.cpp
	object.cpp
	object_static.cpp
	string.cpp
)

target_link_libraries(JsonDeserializerTests catch)
set_target_properties(JsonDeserializerTests PROPERTIES UNITY_BUILD OFF)

add_test(JsonDeserializer JsonDeserializerTests)
