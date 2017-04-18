# Copyright Benoit Blanchon 2014-2017
# MIT License
# 
# Arduino JSON library
# https://bblanchon.github.io/ArduinoJson/
# If you like this project, please add a star!

add_library(catch
	catch.hpp
	catch.cpp
)

target_include_directories(catch
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
