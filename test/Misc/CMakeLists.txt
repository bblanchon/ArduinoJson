# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(MiscTests 
	deprecated.cpp
	FloatParts.cpp
	std_stream.cpp
	std_string.cpp
	StringBuilder.cpp
	TypeTraits.cpp
	unsigned_char.cpp
	vla.cpp
)

target_link_libraries(MiscTests catch)
add_test(Misc MiscTests)
