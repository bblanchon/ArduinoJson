# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2022, Benoit BLANCHON
# MIT License

add_executable(MsgPackDeserializerTests
	deserializeArray.cpp
	deserializeObject.cpp
	deserializeStaticVariant.cpp
	deserializeVariant.cpp
	doubleToFloat.cpp
	filter.cpp
	incompleteInput.cpp
	input_types.cpp
	misc.cpp
	nestingLimit.cpp
	notSupported.cpp
)

add_test(MsgPackDeserializer MsgPackDeserializerTests)

set_tests_properties(MsgPackDeserializer
	PROPERTIES
		LABELS 		"Catch"
)
