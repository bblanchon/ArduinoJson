# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2019
# MIT License

add_executable(NumbersTests 
	parseFloat.cpp
	parseInteger.cpp
	parseNumber.cpp
)

target_link_libraries(NumbersTests catch)
add_test(Numbers NumbersTests)
