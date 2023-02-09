# ArduinoJson - https://arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

add_library(catch
	catch.hpp
	catch.cpp
)

target_include_directories(catch
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

if(MINGW)
	# prevent "too many sections (32837)" with MinGW
	target_compile_options(catch PRIVATE -Wa,-mbig-obj)
endif()
