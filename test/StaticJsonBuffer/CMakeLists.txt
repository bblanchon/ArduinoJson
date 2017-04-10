# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(StaticJsonBufferTests 
	basics.cpp
	createArray.cpp
	createObject.cpp
	parseArray.cpp
	parseObject.cpp
	string.cpp
)

target_link_libraries(StaticJsonBufferTests gtest)
add_test(StaticJsonBuffer StaticJsonBufferTests)
