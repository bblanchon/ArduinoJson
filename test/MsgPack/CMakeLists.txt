# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(MsgPackTests
	MsgPackError.cpp
	deserializeArray.cpp
	deserializeObject.cpp
	deserializeStaticVariant.cpp
	deserializeVariant.cpp
	doubleToFloat.cpp
	incompleteInput.cpp
	nestingLimit.cpp
	notSupported.cpp
	std_string.cpp
	std_istream.cpp
)

target_link_libraries(MsgPackTests catch)
add_test(MsgPack MsgPackTests)
