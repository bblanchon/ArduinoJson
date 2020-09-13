# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(MsgPackDeserializerTests
	deserializeArray.cpp
	deserializeObject.cpp
	deserializeStaticVariant.cpp
	deserializeVariant.cpp
	doubleToFloat.cpp
	incompleteInput.cpp
	input_types.cpp
	misc.cpp
	nestingLimit.cpp
	notSupported.cpp
)

add_test(MsgPackDeserializer MsgPackDeserializerTests)
