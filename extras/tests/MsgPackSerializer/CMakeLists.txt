# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

add_executable(MsgPackSerializerTests
	destination_types.cpp
	measure.cpp
	misc.cpp
	serializeArray.cpp
	serializeObject.cpp
	serializeVariant.cpp
)

add_test(MsgPackSerializer MsgPackSerializerTests)
