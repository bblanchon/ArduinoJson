# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(MsgPackTests
	deserializationErrors.cpp
	deserializeArray.cpp
	deserializeObject.cpp
	deserializeVariant.cpp
	deserializeStaticVariant.cpp
	doubleToFloat.cpp
	MsgPackError.cpp
)

target_link_libraries(MsgPackTests catch)
add_test(MsgPack MsgPackTests)
