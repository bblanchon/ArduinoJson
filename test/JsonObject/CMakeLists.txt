# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(JsonObjectTests 
	basics.cpp
	containsKey.cpp
	get.cpp
	invalid.cpp
	iterator.cpp
	prettyPrintTo.cpp
	printTo.cpp
	remove.cpp
	set.cpp
	subscript.cpp
)

target_link_libraries(JsonObjectTests catch)
add_test(JsonObject JsonObjectTests)
