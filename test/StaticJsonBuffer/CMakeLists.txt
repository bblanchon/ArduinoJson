# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_executable(StaticJsonBufferTests 
	alloc.cpp
	createArray.cpp
	createObject.cpp
	parseArray.cpp
	parseObject.cpp
	size.cpp
	startString.cpp
)

target_link_libraries(StaticJsonBufferTests catch)
add_test(StaticJsonBuffer StaticJsonBufferTests)
