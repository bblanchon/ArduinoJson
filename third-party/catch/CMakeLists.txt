# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2017
# MIT License

add_library(catch
	catch.hpp
	catch.cpp
)

target_include_directories(catch
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
