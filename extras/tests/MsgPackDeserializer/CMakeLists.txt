# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2024, Benoit BLANCHON
# MIT License

add_executable(MsgPackDeserializerTests
	deserializeArray.cpp
	deserializeObject.cpp
	deserializeVariant.cpp
	destination_types.cpp
	doubleToFloat.cpp
	errors.cpp
	filter.cpp
	input_types.cpp
	nestingLimit.cpp
)

add_test(MsgPackDeserializer MsgPackDeserializerTests)

set_tests_properties(MsgPackDeserializer
	PROPERTIES
		LABELS "Catch"
)
