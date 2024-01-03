# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

add_executable(JsonDeserializerTests
	array.cpp
	DeserializationError.cpp
	destination_types.cpp
	errors.cpp
	filter.cpp
	input_types.cpp
	misc.cpp
	nestingLimit.cpp
	number.cpp
	object.cpp
	string.cpp
)

set_target_properties(JsonDeserializerTests PROPERTIES UNITY_BUILD OFF)

add_test(JsonDeserializer JsonDeserializerTests)

set_tests_properties(JsonDeserializer
	PROPERTIES
		LABELS "Catch"
)
