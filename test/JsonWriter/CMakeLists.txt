# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2017
# MIT License

add_executable(JsonWriterTests 
	writeFloat.cpp
	writeString.cpp
)

target_link_libraries(JsonWriterTests catch)
add_test(JsonWriter JsonWriterTests)
