# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(JsonWriterTests 
	writeFloat.cpp
	writeString.cpp
)

target_link_libraries(JsonWriterTests catch)
add_test(JsonWriter JsonWriterTests)
