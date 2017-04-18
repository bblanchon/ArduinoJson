# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(JsonArrayTests 
	add.cpp
	basics.cpp
	copyFrom.cpp
	copyTo.cpp
	invalid.cpp
	iterator.cpp
	prettyPrintTo.cpp
	printTo.cpp
	remove.cpp
	set.cpp
	subscript.cpp
)

target_link_libraries(JsonArrayTests catch)
add_test(JsonArray JsonArrayTests)
