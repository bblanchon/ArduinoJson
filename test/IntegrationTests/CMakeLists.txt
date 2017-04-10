# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_executable(IntegrationTests 
	gbathree.cpp
	parse_print.cpp
)

target_link_libraries(IntegrationTests gtest)
add_test(IntegrationTests IntegrationTests)
