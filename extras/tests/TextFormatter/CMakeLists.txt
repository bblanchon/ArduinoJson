# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_executable(TextFormatterTests 
	writeFloat.cpp
	writeString.cpp
)

target_link_libraries(TextFormatterTests catch)
add_test(TextFormatter TextFormatterTests)
