# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(DynamicJsonBufferTests 
	alloc.cpp
	createArray.cpp
	no_memory.cpp
	createObject.cpp
	strdup.cpp
	startString.cpp
)

target_link_libraries(DynamicJsonBufferTests catch)
add_test(DynamicJsonBuffer DynamicJsonBufferTests)
