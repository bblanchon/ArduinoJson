# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonVariantTests 
	as.cpp
	compare.cpp
	copy.cpp
	is.cpp
	or.cpp
	printTo.cpp
	set_get.cpp
	subscript.cpp
	success.cpp
	undefined.cpp
)

target_link_libraries(JsonVariantTests catch)
add_test(JsonVariant JsonVariantTests)
