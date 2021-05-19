# ArduinoJson - https://arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

add_library(catch
	catch.hpp
	catch.cpp
)

target_include_directories(catch
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	# prevent "xxx will change in GCC x.x" with arm-linux-gnueabihf-gcc
	target_compile_options(catch PRIVATE -Wno-psabi)
endif()
