# ArduinoJson - https://arduinojson.org
# Copyright © 2014-2022, Benoit BLANCHON
# MIT License

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

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
