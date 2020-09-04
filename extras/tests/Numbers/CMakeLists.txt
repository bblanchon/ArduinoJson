# ArduinoJson - arduinojson.org
# Copyright Benoit Blanchon 2014-2020
# MIT License

add_executable(NumbersTests 
	parseFloat.cpp
	parseDouble.cpp
	parseInteger.cpp
	parseNumber.cpp
)


add_test(Numbers NumbersTests)
