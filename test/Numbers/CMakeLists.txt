# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2018
# MIT License

add_executable(NumbersTests 
	isFloat.cpp
	isInteger.cpp
	parseFloat.cpp
	parseInteger.cpp
)

target_link_libraries(NumbersTests catch)
add_test(Numbers NumbersTests)
