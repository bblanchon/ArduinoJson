# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(JsonBufferTests
	nested.cpp
	nestingLimit.cpp
	parse.cpp
	parseArray.cpp
	parseObject.cpp
)

target_link_libraries(JsonBufferTests catch)
add_test(JsonBuffer JsonBufferTests)
