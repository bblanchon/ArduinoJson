# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(JsonVariantTests 
	as.cpp
	compare.cpp
	copy.cpp
	is.cpp
	printTo.cpp
	set_get.cpp
	subscript.cpp
	success.cpp
	undefined.cpp
)

target_link_libraries(JsonVariantTests catch)
add_test(JsonVariant JsonVariantTests)
