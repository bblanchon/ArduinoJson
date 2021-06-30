# ArduinoJson - https://arduinojson.org
# Copyright Benoit Blanchon 2014-2021
# MIT License

add_executable(NumbersTests 
	convertNumber.cpp
	parseFloat.cpp
	parseDouble.cpp
	parseInteger.cpp
	parseNumber.cpp
)


add_test(Numbers NumbersTests)

set_tests_properties(Numbers
	PROPERTIES
		LABELS 		"Catch"
)
