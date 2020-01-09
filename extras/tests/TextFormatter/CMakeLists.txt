# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(TextFormatterTests 
	writeFloat.cpp
	writeString.cpp
)

target_link_libraries(TextFormatterTests catch)
set_target_properties(TextFormatterTests PROPERTIES UNITY_BUILD OFF)

add_test(TextFormatter TextFormatterTests)
