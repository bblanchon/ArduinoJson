# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

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
