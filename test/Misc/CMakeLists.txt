# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_executable(MiscTests
	conflicts.cpp
	FloatParts.cpp
	StringWriter.cpp
	TypeTraits.cpp
	unsigned_char.cpp
	version.cpp
)

target_link_libraries(MiscTests catch)
add_test(Misc MiscTests)
