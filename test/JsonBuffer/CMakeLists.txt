# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(JsonBufferTests
	nested.cpp
	nestingLimit.cpp
	parse.cpp
	parseArray.cpp
	parseObject.cpp
)

target_link_libraries(JsonBufferTests catch)
add_test(JsonBuffer JsonBufferTests)
