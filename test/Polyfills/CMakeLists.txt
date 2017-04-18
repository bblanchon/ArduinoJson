# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(PolyfillsTests 
	isFloat.cpp
	isInteger.cpp
	parseFloat.cpp
	parseInteger.cpp
)

target_link_libraries(PolyfillsTests catch)
add_test(Polyfills PolyfillsTests)
