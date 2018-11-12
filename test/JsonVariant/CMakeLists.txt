# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonVariantTests
	as.cpp
	compare.cpp
	copy.cpp
	get.cpp
	is.cpp
	isnull.cpp
	misc.cpp
	or.cpp
	set.cpp
	subscript.cpp
	to.cpp
	undefined.cpp
)

target_link_libraries(JsonVariantTests catch)
add_test(JsonVariant JsonVariantTests)
